/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2013 - 2017  Mikko "Tapsa" P

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

#include "genie/file/ISerializable.h"

#include <cstring>

namespace genie {

//------------------------------------------------------------------------------
void ISerializable::readObject(std::istream &istr)
{
    setOperation(OP_READ);
    istr_ = &istr;

    istr_->seekg(init_read_pos_);

    serializeObject();
}

//------------------------------------------------------------------------------
void ISerializable::writeObject(std::ostream &ostr)
{
    setOperation(OP_WRITE);
    ostr_ = &ostr;
    serializeObject();
}

//------------------------------------------------------------------------------
size_t ISerializable::objectSize()
{
    size_ = 0;

    setOperation(OP_CALC_SIZE);
    serializeObject();

    return size_;
}

//------------------------------------------------------------------------------
void ISerializable::serializeSubObject(ISerializable *const other)
{
    istr_ = other->istr_;
    ostr_ = other->ostr_;
    operation_ = other->operation_;
    setGameVersion(other->gameVersion_);
    serializeObject();
}

//------------------------------------------------------------------------------
size_t ISerializable::strnlen(const char *str, size_t maxLen)
{
    if (!str) {
        return 0;
    }

    size_t len = 0;

    for (len = 0; len < maxLen; len++) {
        if (str[len] == '\0') {
            return len;
        }
    }

    return maxLen;
}

//------------------------------------------------------------------------------
std::streampos ISerializable::tellg() const
{
    if (isOperation(OP_READ)) {
        return istr_->tellg();
    }

    return 0;
}

//------------------------------------------------------------------------------
std::string ISerializable::readString(size_t len)
{
    if (len > 0 && !istr_->eof()) {
        std::string buf(len, '\0');

        istr_->read(buf.data(), len);
        if (istr_->bad()) {
            throw std::ios_base::failure("Failed to read string");
        }

        if (istr_->eof()) {
            throw std::ios_base::failure("Short read of string");
        }

        size_t tmp_len = ISerializable::strnlen(buf.data(), len);

        if (tmp_len < len) {
            buf.resize(tmp_len);
        }

        return buf;
    }

    return "";
}

//------------------------------------------------------------------------------
void ISerializable::writeString(const std::string &str, size_t len)
{
    char *buf = new char[len];

    str.copy(buf, len);

    for (size_t i = str.size(); i < len; i++) {
        buf[i] = 0; // fill up with 0
    }

    ostr_->write(buf, len);

    delete[] buf;
}
} // namespace genie
