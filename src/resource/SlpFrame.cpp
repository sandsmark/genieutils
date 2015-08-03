/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015  Mikko "Tapsa" P

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


#include "genie/resource/SlpFrame.h"

#include <iostream>
//Debug
#include <assert.h>

#include "genie/resource/Color.h"


namespace genie
{

Logger& SlpFrame::log = Logger::getLogger("genie.SlpFrame");

//------------------------------------------------------------------------------
SlpFrame::SlpFrame()
{
  image_pixel_indexes_ = 0;

  player_color_index_ = -1;

  transparent_index_ = 255;
}

//------------------------------------------------------------------------------
SlpFrame::~SlpFrame()
{
  delete image_pixel_indexes_;
  image_pixel_indexes_ = 0;
}

//------------------------------------------------------------------------------
void SlpFrame::setSlpFilePos(std::streampos pos)
{
  slp_file_pos_ = pos;
}

//----------------------------------------------------------------------------
uint8_t SlpFrame::getTransparentPixelIndex(void) const
{
  return transparent_index_;
}

uint32_t SlpFrame::getWidth(void) const
{
  return width_;
}

uint32_t SlpFrame::getHeight(void) const
{
  return height_;
}

//----------------------------------------------------------------------------
const uint8_t* SlpFrame::getPixelIndexes(void) const
{
  return image_pixel_indexes_;
}

//------------------------------------------------------------------------------
/*sf::Image* SlpFrame::getOutline() const
{
  return outline_;
}
*/

//------------------------------------------------------------------------------
/*
sf::Image* SlpFrame::getPlayerColorMask(uint8_t player) const
{
  sf::Image *cmask = new sf::Image();
  cmask->Create(width_, height_, sf::Color(0,0,0,0));

  for (std::vector<PlayerColorElement>::const_iterator
       it = player_color_mask_.begin(); it != player_color_mask_.end(); ++it)
  {
    cmask->SetPixel(it->x, it->y, (*palette_)[it->index + ((player + 1) * 16)].toSfColor());
  }

  return cmask; //TODO auto pointer (but doesn't work, maybe problem with
                // images copy constructor, try again in sfml2.0
}
*/

//------------------------------------------------------------------------------
int32_t SlpFrame::getHotspotX() const
{
  return hotspot_x_;
}

//------------------------------------------------------------------------------
int32_t SlpFrame::getHotspotY() const
{
  return hotspot_y_;
}

// void SlpFrame::serializeHeader(void)
// {
//
// }

void SlpFrame::serializeObject(void)
{
}

//------------------------------------------------------------------------------
void SlpFrame::loadHeader(std::istream &istr)
{
  setIStream(istr);
  setOperation(OP_READ);

  cmd_table_offset_     = read<uint32_t>();
  outline_table_offset_ = read<uint32_t>();
  palette_offset_       = read<uint32_t>();
  properties_           = read<uint32_t>();

  width_     = read<uint32_t>();
  height_    = read<uint32_t>();

  hotspot_x_ = read<int32_t>();
  hotspot_y_ = read<int32_t>();
  /*log.info("Frame header [%u], [%u], [%u], [%u], [%u], [%u], [%d], [%d], ",
    cmd_table_offset_, outline_table_offset_, palette_offset_, properties_,
    width_, height_, hotspot_x_, hotspot_y_);*/
}

//------------------------------------------------------------------------------
void SlpFrame::load(std::istream &istr)
{
  setIStream(istr);

  image_pixel_indexes_ = new uint8_t[width_ * height_];
  std::fill_n(image_pixel_indexes_, width_ * height_, transparent_index_);

  uint16_t integrity = 0;
  //log.info("Edges beg [%u]", tellg() - slp_file_pos_);
  readEdges(integrity);
  //log.info("Edges end [%u]", tellg() - slp_file_pos_);

  // Skipping command offsets. They are not needed now but
  // they can be used for checking file integrity.
  //log.info("Command offsets beg [%u]", tellg() - slp_file_pos_);
  std::vector<uint32_t> cmd_offsets(height_);
  for (uint32_t i=0; i < height_; ++i)
  {
    uint32_t cmd_offset = read<uint32_t>();
    cmd_offsets[i] = cmd_offset;
    //log.info("Command [%u] at [%u]", i, cmd_offset);
  }
  //log.info("Command offsets end [%u], integrity [%X]", tellg() - slp_file_pos_, integrity);
  //log.info("IS TRANSPARENT FRAME [%X]", integrity == 0x8000);

  if (integrity != 0x8000) // At least one visible row.
  // Each row has it's commands, 0x0F signals the end of a rows commands.
  for (uint32_t row = 0; row < height_; ++row)
  {
    istr.seekg(slp_file_pos_ + std::streampos(cmd_offsets[row]));
    if (istr.eof())
    {
        log.error("Oops! End of stream.");
        return;
    }
    // Transparent rows apparently read one byte anyway. NO THEY DO NOT! Ignore and use seekg()
    if (0x8000 == left_edges_[row] || 0x8000 == right_edges_[row]) // Remember signedness!
    {
      //log.info("Reading [%X][%X] transparent row [%u] at [%u]",
        //left_edges_[row], right_edges_[row], row, tellg() - slp_file_pos_);
      continue; // Pretend it does not exist.
    }
    //log.info("Handling row [%u] commands beg [%u]", row, tellg() - slp_file_pos_);
    uint32_t pix_pos = left_edges_[row]; //pos where to start putting pixels

    uint8_t data = 0;
    while (true)
    {
      data = read<uint8_t>();

      //if ( (tellg() - file_pos_) == 0x1630)
      //  std::cout << row << ": " << std::hex << (int)(tellg() - file_pos_) << " cmd: " << (int)data<< std::endl;
      if (data == 0x0F) break;

      uint8_t cmd = data & 0x0F;
      uint8_t sub = data & 0xF0;

      uint8_t color_index = 0;
      uint32_t pix_cnt = 0;

      switch (cmd) //0x00
      {
        case 0x0: // Lesser block copy
        case 0x4:
        case 0x8:
        case 0xC:
          pix_cnt = (data & 0xFC) >> 2;
          readPixelsToImage(row, pix_pos, pix_cnt);
          break;

        case 0x1: // lesser skip (making pixels transparent)
        case 0x5:
        case 0x9:
        case 0xD:
          pix_cnt = (data & 0xFC) >> 2;
          pix_pos += pix_cnt;
          break;

        case 0x2: // greater block copy
          pix_cnt = (sub << 4) + read<uint8_t>();

          readPixelsToImage(row, pix_pos, pix_cnt);
          break;

        case 0x3: // greater skip
          pix_cnt = (sub << 4) + read<uint8_t>();
          pix_pos += pix_cnt;
          break;

        case 0x6: // copy and transform (player color)
          pix_cnt = getPixelCountFromData(data);

          // TODO: player color
          readPixelsToImage(row, pix_pos, pix_cnt, true);

          break;

        case 0x7: // Run of plain color
          pix_cnt = getPixelCountFromData(data);

          color_index = read<uint8_t>();
          setPixelsToColor(row, pix_pos, pix_cnt, color_index);
        break;

        case 0xA: // Transform block (player color)
          pix_cnt = getPixelCountFromData(data);

          // TODO: file_.readuint8_t() | player_color
          color_index = read<uint8_t>();
          setPixelsToColor(row, pix_pos, pix_cnt, color_index, true);
        break;

        case 0xB: // Shadow pixels
          //TODO: incomplete
          pix_cnt = getPixelCountFromData(data);
          setPixelsToColor(row, pix_pos, pix_cnt, 56);

        break;

        case 0xE: // extended commands.. TODO

          switch (data)
          {
            case 0x0E: //xflip?? skip?? TODO
            case 0x1E:
              log.error("Cmd [%X] not implemented", data);
              //row-= 1;
            break;

            case 0x2E:
            case 0x3E:
              log.error("Cmd [%X] not implemented", data);
            break;

            case 0x4E: //Outline pixel TODO player color
              setPixelsToColor(row, pix_pos, 1, 242);
            break;
            case 0x6E:
              setPixelsToColor(row, pix_pos, 1, 0);
            break;

            case 0x5E: //Outline run TODO player color
              pix_cnt = read<uint8_t>();
              setPixelsToColor(row, pix_pos, pix_cnt, 242);
            break;
            case 0x7E:
              pix_cnt = read<uint8_t>();
              setPixelsToColor(row, pix_pos, pix_cnt, 0);
            break;
          }

        break;
        default:
          log.error("Unknown cmd [%X]", data);
          std::cerr << "SlpFrame: Unknown cmd at " << std::hex << std::endl;
                  //(int)(tellg() - slp_file_pos_)<< ": " << (int) data << std::endl;
          break;
      }
    }
  }
}

//------------------------------------------------------------------------------
void SlpFrame::readEdges(uint16_t &integrity)
{
  //std::streampos cmd_table_pos = slp_file_pos_ + std::streampos(cmd_table_offset_);

  //log.info("Edges height [%u]", height_);
  left_edges_.resize(height_);
  right_edges_.resize(height_);

  uint32_t row_cnt = 0;

  //while (tellg() < cmd_table_pos)
  while (row_cnt < height_)
  {
    left_edges_[row_cnt] = read<int16_t>();
    right_edges_[row_cnt] = read<int16_t>();
    integrity |= left_edges_[row_cnt];

    ++row_cnt;
  }

}

//------------------------------------------------------------------------------
void SlpFrame::readPixelsToImage(uint32_t row, uint32_t &col,
                                 uint32_t count, bool player_col)
{
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    uint8_t color_index = read<uint8_t>();

    image_pixel_indexes_[row * width_ + col] = color_index;

    if (color_index == transparent_index_)
      std::cout << "Color index == transparent index" << std::endl;

    if (player_col)
    {
      PlayerColorElement pce = {col, row, color_index};
      player_color_mask_.push_back(pce);
    }
    //  player_color_mask_->SetPixel(col, row, palette_->getColorAt(pixel_index));

    ++col;
  }

}

//------------------------------------------------------------------------------
void SlpFrame::setPixelsToColor(uint32_t row, uint32_t &col,
                                uint32_t count, uint8_t color_index,
                                bool player_col)
{
  uint32_t to_pos = col + count;

  //log.info("Setting pixels to color [%u] [%u] [%u] [%u] [%u]", row, col, count, color_index, player_col);
  while (col < to_pos)
  {
    image_pixel_indexes_[row * width_ + col] = color_index;

    if (color_index == transparent_index_)
      std::cout << "Color index == transparent index" << std::endl;

    //if (player_col)
    //  player_color_mask_->SetPixel(col, row, color);
    if (player_col)
    {
      PlayerColorElement pce = {col, row, color_index};
      player_color_mask_.push_back(pce);
    }

    ++col;
  }
}

//------------------------------------------------------------------------------
uint8_t SlpFrame::getPixelCountFromData(uint8_t data)
{
  uint8_t pix_cnt;

  data = (data & 0xF0) >> 4;

  if (data == 0)
    pix_cnt = read<uint8_t>();
  else
    pix_cnt = data;

  return pix_cnt;
}

}
