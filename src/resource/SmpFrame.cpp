/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2018  Mikko "Tapsa" P

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

#include "genie/resource/SmpFrame.h"

#include <iostream>
//Debug
#include <cassert>
#include <stdexcept>
#include <chrono>

#include "genie/resource/Color.h"

namespace genie
{

Logger& SmpFrame::log = Logger::getLogger("genie.SmpFrame");
extern const char* CNT_SETS;

//------------------------------------------------------------------------------
SmpFrame::SmpFrame()
{
}

//------------------------------------------------------------------------------
SmpFrame::~SmpFrame()
{
}

uint32_t SmpFrame::getWidth(void) const
{
  return layer_width_;
}

uint32_t SmpFrame::getHeight(void) const
{
  return layer_height_;
}

void SmpFrame::serializeObject(void)
{
}

void SmpFrame::setLoadParams(std::istream &istr, uint32_t smp_offset_)
{
  setIStream(istr);
  setOperation(OP_READ);
  // This is needed because SMP frames are not stored immediately in contiguous order.
  smp_frame_pos_ = smp_offset_;
  istr.seekg(smp_offset_);
}

void SmpFrame::setSaveParams(std::ostream &ostr, uint32_t &smp_offset_)
{
  setOStream(ostr);
  setOperation(OP_WRITE);
}

//------------------------------------------------------------------------------
void SmpFrame::serializeHeader(void)
{
  serialize<uint32_t>(width_);
  serialize<uint32_t>(height_);

  serialize<int32_t>(hotspot_x);
  serialize<int32_t>(hotspot_y);

  serialize<uint32_t>(type);
  serialize<uint32_t>(diffuse_palette_xid);
  serialize<uint32_t>(diffuse_palette_num);
  serialize<uint32_t>(num_layers);

// Immediately after there is layer info.
// I guess as many times as there are layers.
// Read in the basic image layer only for now.
  if (num_layers > 0)
  {
    serialize<uint32_t>(layer_width_);
    serialize<uint32_t>(layer_height_);

    serialize<int32_t>(layer_hotspot_x);
    serialize<int32_t>(layer_hotspot_y);

    serialize<uint32_t>(layer_type_);
    serialize<uint32_t>(layer_outline_offsets_);
    serialize<uint32_t>(layer_data_offsets_);
    serialize<uint32_t>(layer_flags_);
  }
}

//------------------------------------------------------------------------------
void SmpFrame::load(std::istream &istr)
{
  setIStream(istr);

  if (0 == num_layers)
  {
     log.error("No layers");
     return;
  }

  img_data.pixel_indexes.resize(layer_width_ * layer_height_);
  img_data.alpha_channel.resize(layer_width_ * layer_height_, 0);

  istr.seekg(smp_frame_pos_ + std::streampos(layer_outline_offsets_));
  readEdges();

  istr.seekg(smp_frame_pos_ + std::streampos(layer_data_offsets_));
  serialize<uint32_t>(cmd_offsets_, layer_height_);

  // Each row has its commands.
  for (uint32_t row = 0; row < layer_height_; ++row)
  {
    if (0xFFFF == left_edges_[row] || 0 == cmd_offsets_[row])
    {
      continue; // Skip transparent lines.
    }
    uint32_t pix_pos = left_edges_[row]; //Position where to start putting pixels
    istr.seekg(smp_frame_pos_ + std::streampos(cmd_offsets_[row]));
    assert(!istr.eof());

    uint8_t data = 0;
    while (true)
    {
      data = read<uint8_t>();

      uint8_t cmd = data & 0x3;

      if (0x3 == cmd)
      {
        break;
      }

      uint8_t pix_cnt = 1 + ((data & 0xFC) >> 2);

      switch (cmd)
      {
        case 0x0: // Transparent skip
          pix_pos += pix_cnt;
          break;

        case 0x1: // Copy pixels
          readPixelsToImage(row, pix_pos, pix_cnt, false);
          break;

        case 0x2: // Player color pixels
          readPixelsToImage(row, pix_pos, pix_cnt, true);
          break;

        default:
          log.error("Unknown command [%X]", data);
          return;
      }
    }
  }
}

//------------------------------------------------------------------------------
void SmpFrame::readEdges(void)
{
  left_edges_.resize(layer_height_);
  right_edges_.resize(layer_height_);

  for (uint32_t row = 0; row < layer_height_; ++row)
  {
    serialize<uint16_t>(left_edges_[row]);
    serialize<uint16_t>(right_edges_[row]);
  }
}

//------------------------------------------------------------------------------
void SmpFrame::readPixelsToImage(uint32_t row, uint32_t &col,
                                 uint32_t count, bool player_col)
{
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    uint32_t color_index = read<uint32_t>();
    assert(row * layer_width_ + col < img_data.pixel_indexes.size());
    img_data.pixel_indexes[row * layer_width_ + col] = color_index;
    img_data.alpha_channel[row * layer_width_ + col] = 255;
    if (player_col)
    {
      img_data.player_color_mask.push_back({col, row, uint8_t(color_index)});
    }
    ++col;
  }
}

//------------------------------------------------------------------------------
void SmpFrame::save(std::ostream &ostr)
{
  setOStream(ostr);
}

}
