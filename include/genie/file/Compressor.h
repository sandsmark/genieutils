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

#pragma once

#include <iostream>
#include <memory>
#include "ISerializable.h"

namespace genie {

//------------------------------------------------------------------------------
/// Utility to compress and decompress streams handled in ISerializeable
/// objects.
//
class Compressor
{
public:
    Compressor(const Compressor &) = delete;
    Compressor &operator=(const Compressor &) = delete;

    //----------------------------------------------------------------------------
    /// Object that calls Compressors methods needs to register itself here.
    ///
    /// @param obj object that calls compressors methods
    //
    Compressor(ISerializable *obj);

    //----------------------------------------------------------------------------
    void beginCompression();

    //----------------------------------------------------------------------------
    void endCompression();

    //----------------------------------------------------------------------------
    static void decompress(std::istream &source, std::ostream &sink);

private:
    ISerializable *obj_ = nullptr;

    std::istream *istream_ = nullptr;
    std::shared_ptr<std::istream> uncompressedIstream_;

    std::ostream *ostream_ = nullptr;
    std::shared_ptr<std::ostream> bufferedStream_;

    Compressor() = default;

    //----------------------------------------------------------------------------
    /// Decompresses istream and sets uncompressedIstream_.
    //
    void startDecompression();

    //----------------------------------------------------------------------------
    /// Closes decompressed stream.
    //
    void stopDecompression();

    //----------------------------------------------------------------------------
    void startCompression();

    //----------------------------------------------------------------------------
    void stopCompression();
};
} // namespace genie

