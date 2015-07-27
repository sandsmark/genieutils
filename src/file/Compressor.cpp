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

#include <boost/interprocess/streams/vectorstream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>

using namespace boost;
using namespace boost::iostreams;

namespace genie
{
  
typedef boost::interprocess::basic_vectorstream< std::vector<char> > v_stream;

Compressor::Compressor()
{
}

//------------------------------------------------------------------------------
Compressor::Compressor(ISerializable *obj)
{
  obj_ = obj;
  istream_ = 0;
}

//------------------------------------------------------------------------------
Compressor::~Compressor()
{
}

//------------------------------------------------------------------------------
void Compressor::beginCompression(void)
{
  switch(obj_->getOperation())
  {
    case ISerializable::OP_READ:
      istream_ = obj_->getIStream();
      
      startDecompression();
      
      obj_->setIStream(*uncompressedIstream_);
      break;
    
    case ISerializable::OP_WRITE:
      obj_->setIStream(*istream_);
      
      startCompression();
      break;
    
    default:
      break;
    
  }
}
  
//------------------------------------------------------------------------------
void Compressor::endCompression(void)
{
  switch(obj_->getOperation())
  {
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
  
  copy(*cmp.uncompressedIstream_, sink);
}

//------------------------------------------------------------------------------
boost::iostreams::zlib_params Compressor::getZlibParams(void ) const
{  
  zlib_params params;
  
  // important
  params.window_bits = -15;
  
  // default
  params.level = -1;
  params.method = zlib::deflated;
  params.mem_level = 9;
  params.strategy = zlib::default_strategy;
  
  return params;
}
  
//------------------------------------------------------------------------------
void Compressor::startDecompression(void)
{ 
  try
  {
    filtering_istreambuf in;
    
    // register decompressor
    in.push(zlib_decompressor(getZlibParams()));
    
    in.push(*istream_);
        
    // extract file to buffer
    std::vector<char> file_buf;
    back_insert_device< std::vector<char> > b_ins(file_buf); 
    
    copy(in, b_ins);
    
    uncompressedIstream_ = shared_ptr<std::istream>(new v_stream(file_buf)); 
  }
  catch ( const zlib_error &z_err)
  {
    uncompressedIstream_.reset();
    std::cerr << "Zlib decompression failed with error code: "
              <<  z_err.error() << std::endl;
    throw z_err;
  }
}

//------------------------------------------------------------------------------
void Compressor::stopDecompression(void)
{
  istream_ = 0;
  
  uncompressedIstream_.reset();
}

//------------------------------------------------------------------------------
void Compressor::startCompression(void)
{
  // create buffer
  std::vector<char> file_buf;
  
  bufferedStream_ = boost::shared_ptr<std::iostream> (new v_stream(file_buf));

  ostream_ = obj_->getOStream();
  
  obj_->setOStream(*bufferedStream_);
}

//------------------------------------------------------------------------------
void Compressor::stopCompression(void)
{
  try
  {
    filtering_ostreambuf out;
    
    out.push(zlib_compressor(getZlibParams()));
    
    out.push(*ostream_);
    
    copy (*bufferedStream_, out);
    
    obj_->setOStream(*ostream_);
    ostream_ = 0;
    
    bufferedStream_.reset();
    
  }
  catch (const zlib_error &z_err)
  {
    std::cerr << "Zlib compression failed with error code: "
              <<  z_err.error() << std::endl;
    throw z_err;
  }
}
  
}