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

#pragma once

#include <genie/file/IFile.h>
#include <genie/util/Logger.h>
#include <iconv.h> //Sorry no iconv for msvc

struct pcr_file;

namespace genie {

class PcrioError : public std::ios::failure
{
public:
    explicit PcrioError(int error, const std::string &filename);
    int getError() const { return error_; }

    static void check(int error, const std::string &filename);

private:
    int error_;
};

/// TODO Proper error handling
class LangFile : public IFile
{

public:
    LangFile();
    ~LangFile() override;

    void load(const std::string &fileName) override;
    void saveAs(const std::string &fileName) override;

    // get/set strings in default_charset (utf-8)
    std::string getString(unsigned int id);
    void setString(unsigned int id, const std::string &str);

    /// Change the default charset. See libiconv doc for available ones.
    /// If not set, default = UTF8.
    void setDefaultCharset(const char *charset);
    uint32_t getDefaultCodepage() { return defaultCodepage_; }

    /// Convert a utf8 string to codepage
    std::string convertTo(const std::string &in, uint32_t codepage);

    const std::vector<uint32_t> &allIds() const { return m_allIds; }

protected:
    void unload() override;

    void serializeObject() override {}

private:
    static Logger &log;

    struct pcr_file *pfile_ = nullptr;

    uint32_t defaultCultureId_ = 0;
    uint32_t defaultCodepage_ = 0;
    std::vector<uint32_t> m_allIds;

    static const unsigned int CONV_BUF_SIZE = 100;
    static const char *CONV_DEFAULT_CHARSET;

    std::string systemDefaultCharset_; // all strings will be converted from/to this charset

    iconv_t toDefaultCharsetCd_{};
    iconv_t fromDefaultCharsetCd_{};

    /// Convert a string from codepage to utf8
    std::string convertFrom(const std::string &in, uint32_t codepage);

    std::string convert(iconv_t cd, const std::string &input);
};
} // namespace genie

