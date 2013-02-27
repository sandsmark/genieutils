/*
    genieutils - <description>
    Copyright (C) 2013  Armin Preiml <email>

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

#include "pcrio/pcrio.h"

namespace genie
{
  
const char *LangFile::CONV_DEFAULT_CHARSET = "UTF-8";
  
Logger& LangFile::log = Logger::getLogger("freeaoe.DrsFile");

//------------------------------------------------------------------------------
PcrioError::PcrioError(int error) : 
  std::ios::failure(pcr_error_message((pcr_error_code)error)), error_(error)
{
}

//------------------------------------------------------------------------------
void PcrioError::check(int error)
{
  switch(error)
  {
    case PCR_ERROR_NONE:
      return;
    case PCR_ERROR_BAD_ALLOC:
      throw std::bad_alloc();
    default:
      throw PcrioError(error);
  }
}

//------------------------------------------------------------------------------
LangFile::LangFile() 
{
  pfile_ = 0;
  
  defaultCultureId_ = 0;
  defaultCodepage_ = 0;
  
  toDefaultCharsetCd_ = (iconv_t)0;
  fromDefaultCharsetCd_ = (iconv_t)0;
  
  systemDefaultCharset_ = CONV_DEFAULT_CHARSET;
}
  
//------------------------------------------------------------------------------
LangFile::~LangFile() 
{
  if (toDefaultCharsetCd_)
    iconv_close(toDefaultCharsetCd_);
  
  if (fromDefaultCharsetCd_)
    iconv_close(fromDefaultCharsetCd_);
  
  if (pfile_)
    pcr_free(pfile_);
}

//------------------------------------------------------------------------------
void LangFile::load(const char *filename) throw (std::ios_base::failure)
{
  pcr_error_code errorCode_ = PCR_ERROR_NONE;  
  
  setFileName(filename);
  
  log.info("-------");
  log.info("Loading \"%s\"", filename);
  
  if (pfile_)
    pcr_free(pfile_);
  
  pfile_ = pcr_read_file(filename, &errorCode_);
  
  PcrioError::check(errorCode_); // on error throw
  
  std::stringstream cName;
  
  const Pcr_language *lang = pcr_get_default_language(pfile_);
  
  if (!lang)
  {
    log.info("Trying to choose default language...");
    
    const Language_info_array *linfos = pcr_get_language_info(pfile_);
    
    if (linfos->count == 0)
      throw std::string("Not supported: There are no resources in dll file");
    
    Language_info *linfo = &linfos->array[0];
    
    // Get most common language desc
    for (unsigned int i=1; i<linfos->count; i++)
      if (linfo->item_count < linfos->array[i].item_count)
        linfo = &linfos->array[i];
      
    defaultCultureId_ = linfo->lang.id;
    defaultCodepage_ = linfo->lang.codepage;
        
  }
  else
  {
    defaultCultureId_ = lang->id;
    defaultCodepage_ = lang->codepage;
  }
  
  log.info("Culture Id: %d, Codepage: %d.", defaultCultureId_, defaultCodepage_); 

  if (defaultCodepage_ > 0)
  {
    cName << "WINDOWS-" << defaultCodepage_;
    
    log.info("Loading \"%s\" charset converter description.", cName.str().c_str());
    
    toDefaultCharsetCd_ = iconv_open(systemDefaultCharset_.c_str(), cName.str().c_str());
    fromDefaultCharsetCd_ = iconv_open(cName.str().c_str(), systemDefaultCharset_.c_str());
    
    std::cout << cName.str().c_str() << std::endl;
    
    if (toDefaultCharsetCd_ == (iconv_t) - 1 || fromDefaultCharsetCd_ == (iconv_t)-1)
    {
      log.error("Can't open default converter");
      throw IconvError("Can't open default converter.");
    }
  }
}

//------------------------------------------------------------------------------
void LangFile::saveAs(const char *filename) throw (std::ios_base::failure)
{
  pcr_error_code errorCode = PCR_ERROR_NONE;
  
  if (pfile_ == 0)
    throw std::ios_base::failure("Save: Can't save unloaded file: "
                                 + std::string(filename));
  
  pcr_write_file(filename, pfile_, &errorCode);
  
  PcrioError::check(errorCode);
}

//----------------------------------------------------------------------------
std::string LangFile::getString(unsigned int id)
{
  std::string encodedStr, decodedStr;
  char *strBuf;
  int strBufSize = pcr_get_strlenL(pfile_, id, defaultCultureId_) + 1;
  
  if (strBufSize <= 1)
  {
    log.debug("%s: String [%d] not found!", getFileName(), id);
    return std::string("");
  }
  
  strBuf = new char[strBufSize];
  
  log.info("%s: getString(%d);", getFileName(), id);
  
  int flag = pcr_get_stringL(pfile_, id, defaultCultureId_, strBuf, strBufSize);
  
  encodedStr = std::string(strBuf, strBufSize - 1); // excluding \0
  
  int codepage;
  
  if (flag)
  {
    codepage = pcr_get_codepageL(pfile_, id, defaultCultureId_);
    
    log.info("Codepage differs, loading converter for cp [%d]", codepage);
  }
  else
    codepage = defaultCodepage_;
  
  decodedStr = convertFrom(encodedStr, defaultCodepage_);
  
  log.info("| Result: \"%s\"", decodedStr.c_str());
  
  delete strBuf;
  
  return decodedStr;
}

//----------------------------------------------------------------------------
void LangFile::setString(unsigned int id, std::string str)
{
  std::string encodedStr;
  
  log.info("%s: setString(%d, %s);", getFileName(), id, str.c_str());
  
  encodedStr = convertTo(str, defaultCodepage_);
  
  log.info("| Convert from \"%s\" to \"%s\".", str.c_str(), encodedStr.c_str());
  
  Pcr_language lang;
  lang.id = defaultCultureId_;
  lang.codepage = defaultCodepage_;
  
  int err = pcr_set_stringC(pfile_, id, lang, encodedStr.c_str());
  
  if (err > 0)
    PcrioError::check(err);
  else if (err == -1)   // wrong codepage
  {
    log.info("Trying to rewrite string converted to wrong codepage");
    
    lang.codepage = pcr_get_codepageL(pfile_, id, lang.id);
    
    encodedStr = convertTo(str, lang.codepage);
    
    PcrioError::check(pcr_set_stringC(pfile_, id, lang, encodedStr.c_str()));
  }
}
  
void LangFile::setDefaultCharset(const char *charset)
{
  systemDefaultCharset_ = std::string(charset);
}
  
//----------------------------------------------------------------------------
void LangFile::unload(void)
{
  if (pfile_)
    pcr_free(pfile_);
  
  pfile_ = 0;
}

//----------------------------------------------------------------------------
std::string LangFile::convertTo(std::string in, uint32_t codepage)
{
  iconv_t cd;
  std::string encodedStr;
  
  if (codepage == 0)
    return in;
  
  if (codepage == defaultCodepage_)
    cd = fromDefaultCharsetCd_;
  else
  {
    
    std::stringstream conv_name;
    
    conv_name << "WINDOWS-" << codepage;
    
    if ((cd = iconv_open(conv_name.str().c_str(), systemDefaultCharset_.c_str())) == (iconv_t)(-1))
    {
      std::string error = "Cannot open converter from " + systemDefaultCharset_ +
                          " to " + conv_name.str();
                          
      throw error;
    }
  }
  
  encodedStr = convert(cd, in);
  
  if (codepage != defaultCodepage_)
    iconv_close(cd);
  
  return encodedStr;
}

//----------------------------------------------------------------------------
std::string LangFile::convertFrom(std::string in, uint32_t codepage)
{
  iconv_t cd;
  std::string decodedStr;
  
  if (codepage == 0)
    return in;
  
  if (codepage == defaultCodepage_)
    cd = toDefaultCharsetCd_;
  else
  {
    
    std::stringstream conv_name;
    
    conv_name << "WINDOWS-" << codepage;
    
    if ((cd = iconv_open(systemDefaultCharset_.c_str(), conv_name.str().c_str())) == (iconv_t)(-1))
    {
      std::string error = "Cannot open converter from " + conv_name.str() +
                          " to " + systemDefaultCharset_;
                          
      throw error;
    }
  }
  
  decodedStr = convert(cd, in);
  
  if (codepage != defaultCodepage_)
    iconv_close(cd);
  
  return decodedStr;
}
  
//----------------------------------------------------------------------------
std::string LangFile::convert(iconv_t cd, std::string input)
{
  size_t inleft = input.size();
  char *inbuf = new char[inleft];
  char *inptr = inbuf;
  
  char buf[CONV_BUF_SIZE];
  size_t outleft = CONV_BUF_SIZE, iconv_value = 0;
  char *outptr = buf;

  std::string decodedStr;
  
  strncpy(inbuf, input.c_str(), inleft);
  
  while (cd != (iconv_t)-1 && inleft > 0 && iconv_value == 0)
  { 
    iconv_value = iconv(cd, &inptr, &inleft, &outptr, &outleft);
    
    if (iconv_value == (size_t)-1) 
    {
      if(errno == E2BIG)
      {
        decodedStr += std::string(buf, CONV_BUF_SIZE);
        outleft = CONV_BUF_SIZE;
        outptr = buf;
        iconv_value = 0;
      }
      else  
      {
        delete (inbuf);
        
        std::string error("Error in converting characters: ");

        if(errno == EILSEQ)
          error += "EILSEQ";
        if(errno == EINVAL)
          error += "EINVAL";
        
        log.error("%s", error.c_str());
        
        throw error;
      }
    }
    else
      decodedStr += std::string(buf, CONV_BUF_SIZE - outleft);
      
  }
  
  delete (inbuf);
  
  return decodedStr;
}

}
