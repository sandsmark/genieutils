/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml

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

#include "genie/file/Compressor.h"

#include <vector>

#include <zstr.hpp>

namespace genie {

//------------------------------------------------------------------------------
Compressor::Compressor(ISerializable *obj) :
    obj_(obj)
{
}

//------------------------------------------------------------------------------
void Compressor::beginCompression(void)
{
    switch (obj_->getOperation()) {
    case ISerializable::OP_READ:
        istream_ = obj_->getIStream();

        startDecompression();

        obj_->setIStream(*uncompressedIstream_);
        break;

    case ISerializable::OP_WRITE:
        obj_->setIStream(*istream_);
        ostream_ = obj_->getOStream();

        startCompression();

        obj_->setOStream(*bufferedStream_);
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------
void Compressor::endCompression(void)
{
    switch (obj_->getOperation()) {
    case ISerializable::OP_READ:
        stopDecompression();
        break;

    case ISerializable::OP_WRITE:
        stopCompression();
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------
void Compressor::decompress(std::istream &source, std::ostream &sink)
{
    Compressor cmp;

    cmp.istream_ = &source;
    cmp.startDecompression();

    while (!cmp.uncompressedIstream_->eof() && sink.good()) {
        char buffer[4096];
        source.get(buffer, sizeof buffer);
        sink.write(buffer, source.gcount());
    }
}

//------------------------------------------------------------------------------
void Compressor::startDecompression(void)
{
    try {
        // Important thing here is window_bits = 15
        uncompressedIstream_ = std::make_shared<zstr::istream>(*istream_, (std::size_t)1 << 20, false, -15);
    } catch (const zstr::Exception &exception) {
        uncompressedIstream_.reset();
        std::cerr << "Zlib decompression failed with error code: "
                  << exception.what() << std::endl;
    }
}

//------------------------------------------------------------------------------
void Compressor::stopDecompression(void)
{
    obj_->setIStream(*istream_);
    istream_ = 0;

    uncompressedIstream_.reset();
}

//------------------------------------------------------------------------------
void Compressor::startCompression(void)
{
    try {
        // Important thing here is window_bits = 15
        bufferedStream_ = std::make_shared<zstr::ostream>(*ostream_, (std::size_t)1 << 20, false, -15);
    } catch (const zstr::Exception &exception) {
        bufferedStream_.reset();
        std::cerr << "Zlib compression failed with error code: "
                  << exception.what() << std::endl;
    }
}

//------------------------------------------------------------------------------
void Compressor::stopCompression(void)
{
    ostream_ = 0;
    bufferedStream_.reset();
}
} // namespace genie

/* vim: set ts=2 sw=2 tw=0 cindent softtabstop=2 :*/
