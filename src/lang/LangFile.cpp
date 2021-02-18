/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011  Armin Preiml

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "genie/lang/LangFile.h"

#include <sstream>

#include <iconv.h>
#include <errno.h>

extern "C" {
#include <pcrio.h>
}

namespace genie {

const char *LangFile::CONV_DEFAULT_CHARSET = "UTF-8";

Logger &LangFile::log = Logger::getLogger("genie.LangFile");

//------------------------------------------------------------------------------
PcrioError::PcrioError(int error, const std::string &filename) :
    std::ios::failure(filename + ": " + pcr_error_message((pcr_error_code)error)), error_(error)
{
}

//------------------------------------------------------------------------------
void PcrioError::check(int error, const std::string &filename)
{
    switch (error) {
    case PCR_ERROR_NONE:
        return;

    case PCR_ERROR_BAD_ALLOC:
        throw std::bad_alloc();

    default:
        throw PcrioError(error, filename);
    }
}

//------------------------------------------------------------------------------
LangFile::LangFile() :
    systemDefaultCharset_(CONV_DEFAULT_CHARSET)
{
}

//------------------------------------------------------------------------------
LangFile::~LangFile()
{
    if (toDefaultCharsetCd_) {
        iconv_close(toDefaultCharsetCd_);
    }

    if (fromDefaultCharsetCd_) {
        iconv_close(fromDefaultCharsetCd_);
    }

    if (pfile_) {
        pcr_free(pfile_);
        pfile_ = nullptr;
    }
}

//------------------------------------------------------------------------------
void LangFile::load(const std::string &filename)
{
    pcr_error_code errorCode_ = PCR_ERROR_NONE;

    setFileName(filename);

    log.info("Loading \"%\"", filename);

    if (pfile_) {
        pcr_free(pfile_);
        pfile_ = nullptr;
    }

    pfile_ = pcr_read_file(filename.c_str(), &errorCode_);

    PcrioError::check(errorCode_, filename); // on error throw

    if (!pfile_) {
        throw std::iostream::failure("pcrio failed to load, but returned no error");
    }

    std::stringstream cName;

    const struct pcr_language *lang = pcr_get_default_language(pfile_);

    if (!lang) {
        log.info("Trying to choose default language...");

        const struct language_info_array *linfos = pcr_get_language_info(pfile_);

        if (linfos->count == 0) {
            throw std::string("Not supported: There are no resources in dll file");
        }

        struct language_info *linfo = &linfos->array[0];

        // Get most common language desc
        for (unsigned int i = 1; i < linfos->count; i++) {
            if (linfo->item_count < linfos->array[i].item_count) {
                linfo = &linfos->array[i];
            }
        }

        defaultCultureId_ = linfo->lang.id;
        defaultCodepage_ = linfo->lang.codepage;

    } else {
        defaultCultureId_ = lang->id;
        defaultCodepage_ = lang->codepage;
    }

    log.info("Culture Id: %, Codepage: %.", defaultCultureId_, defaultCodepage_);

    if (defaultCodepage_ > 0) {
        cName << "WINDOWS-" << defaultCodepage_;

        log.info("Loading \"%\" charset converter description.", cName.str().c_str());

        toDefaultCharsetCd_ = iconv_open(systemDefaultCharset_.c_str(), cName.str().c_str());
        fromDefaultCharsetCd_ = iconv_open(cName.str().c_str(), systemDefaultCharset_.c_str());

        std::cout << cName.str().c_str() << std::endl;

        if (toDefaultCharsetCd_ == (iconv_t) -1 || fromDefaultCharsetCd_ == (iconv_t) -1) {
            log.error("Can't open default converter");
            throw std::iostream::failure("Can't open default converter.");
        }
    }

    uint32_t *ids;
    size_t numIds = pcr_get_all_string_ids(pfile_, &ids);

    m_allIds.resize(numIds);
    memcpy(m_allIds.data(), ids, numIds * sizeof(uint32_t));
    free(ids);
}

//------------------------------------------------------------------------------
void LangFile::saveAs(const std::string &filename)
{
    pcr_error_code errorCode = PCR_ERROR_NONE;

    if (!pfile_) {
        throw std::ios_base::failure("Save: Can't save unloaded file: "
                                     + std::string(filename));
    }

    pcr_write_file(filename.c_str(), pfile_, &errorCode);

    PcrioError::check(errorCode, filename);
}

//----------------------------------------------------------------------------
std::string LangFile::getString(unsigned int id)
{
    if (!pfile_) {
        throw std::ios_base::failure("Language file not loaded");
    }

    std::string encodedStr, decodedStr;
    char *strBuf;
    int strBufSize = pcr_get_strlenL(pfile_, id, defaultCultureId_) + 1;

    if (strBufSize <= 1) {
        log.debug("%: String [%] not found!", getFileName(), id);
        return std::string("");
    }

    strBuf = new char[strBufSize];

    log.info("%: getString(%);", getFileName(), id);

    int flag = pcr_get_stringL(pfile_, id, defaultCultureId_, strBuf, strBufSize);

    encodedStr = std::string(strBuf, strBufSize - 1); // excluding \0

    int codepage;

    if (flag) {
        codepage = pcr_get_codepageL(pfile_, id, defaultCultureId_);

        log.info("Codepage differs, loading converter for cp [%]", codepage);
    } else {
        codepage = defaultCodepage_;
    }

    decodedStr = convertFrom(encodedStr, codepage);

    log.info("| Result: \"%\"", decodedStr.c_str());

    delete[] strBuf;

    return decodedStr;
}

//----------------------------------------------------------------------------
void LangFile::setString(unsigned int id, const std::string &str)
{
    std::string encodedStr;

    log.info("%: setString(%, %);", getFileName(), id, str.c_str());

    encodedStr = convertTo(str, defaultCodepage_);

    log.info("| Convert from \"%\" to \"%\".", str.c_str(), encodedStr.c_str());

    struct pcr_language lang;
    lang.id = defaultCultureId_;
    lang.codepage = defaultCodepage_;

    int err = pcr_set_stringC(pfile_, id, lang, encodedStr.c_str());

    if (err > 0) {
        PcrioError::check(err, getFileName());
    } else if (err == -1) { // wrong codepage
        log.info("Trying to rewrite string converted to wrong codepage");

        lang.codepage = pcr_get_codepageL(pfile_, id, lang.id);

        encodedStr = convertTo(str, lang.codepage);

        PcrioError::check(pcr_set_stringC(pfile_, id, lang, encodedStr.c_str()), getFileName());
    }
}

void LangFile::setDefaultCharset(const char *charset)
{
    systemDefaultCharset_ = std::string(charset);
}

//----------------------------------------------------------------------------
void LangFile::unload()
{
    if (pfile_) {
        pcr_free(pfile_);
    }

    pfile_ = nullptr;
}

//----------------------------------------------------------------------------
std::string LangFile::convertTo(const std::string &in, uint32_t codepage)
{
    iconv_t cd;
    std::string encodedStr;

    if (codepage == 0) {
        return in;
    }

    if (codepage == defaultCodepage_) {
        cd = fromDefaultCharsetCd_;
    } else {

        std::stringstream conv_name;

        conv_name << "WINDOWS-" << codepage;

        if ((cd = iconv_open(conv_name.str().c_str(), systemDefaultCharset_.c_str())) == (iconv_t)(-1)) {
            std::string error = "Cannot open converter from " + systemDefaultCharset_ + " to " + conv_name.str();

            throw error;
        }
    }

    encodedStr = convert(cd, in);

    if (codepage != defaultCodepage_) {
        iconv_close(cd);
    }

    return encodedStr;
}

//----------------------------------------------------------------------------
std::string LangFile::convertFrom(const std::string &in, uint32_t codepage)
{
    iconv_t cd;
    std::string decodedStr;

    if (codepage == 0) {
        return in;
    }

    if (codepage == defaultCodepage_) {
        cd = toDefaultCharsetCd_;
    } else {

        std::stringstream conv_name;

        conv_name << "WINDOWS-" << codepage;

        if ((cd = iconv_open(systemDefaultCharset_.c_str(), conv_name.str().c_str())) == (iconv_t)(-1)) {
            std::string error = "Cannot open converter from " + conv_name.str() + " to " + systemDefaultCharset_;

            throw error;
        }
    }

    decodedStr = convert(cd, in);

    if (codepage != defaultCodepage_) {
        iconv_close(cd);
    }

    return decodedStr;
}

//----------------------------------------------------------------------------
std::string LangFile::convert(iconv_t cd, const std::string &input)
{
    size_t inleft = input.size();
    char *inbuf = new char[inleft];

#ifdef ICONV_SECOND_ARGUMENT_IS_CONST
    const
#endif
    char *inptr = inbuf;

    char buf[CONV_BUF_SIZE];
    size_t outleft = CONV_BUF_SIZE, iconv_value = 0;
    char *outptr = buf;

    std::string decodedStr;

    input.copy(inbuf, inleft);

    while (cd != (iconv_t) -1 && inleft > 0 && iconv_value == 0) {
        iconv_value = iconv(cd, &inptr, &inleft, &outptr, &outleft);

        if (iconv_value == (size_t) -1) {
            if (errno == E2BIG) {
                decodedStr += std::string(buf, CONV_BUF_SIZE);
                outleft = CONV_BUF_SIZE;
                outptr = buf;
                iconv_value = 0;
            } else {
                delete[] inbuf;

                std::string error("Error in converting characters: ");

                if (errno == EILSEQ) {
                    error += "EILSEQ";
                }

                if (errno == EINVAL) {
                    error += "EINVAL";
                }

                log.error("%", error.c_str());

                throw error;
            }
        } else {
            decodedStr += std::string(buf, CONV_BUF_SIZE - outleft);
        }
    }

    delete[] inbuf;

    return decodedStr;
}
} // namespace genie
