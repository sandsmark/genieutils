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
#include <cassert>
#include <stdexcept>
#include <chrono>

#include "genie/resource/Color.h"

namespace genie
{

Logger& SlpFrame::log = Logger::getLogger("genie.SlpFrame");
const char* CNT_SETS[] = {"CNT_LEFT", "CNT_SAME", "CNT_DIFF", "CNT_TRANSPARENT", "CNT_FEATHERING", "CNT_PLAYER", "CNT_SHIELD", "CNT_PC_OUTLINE", "CNT_SHADOW"};

//------------------------------------------------------------------------------
SlpFrame::SlpFrame()
{
}

//------------------------------------------------------------------------------
SlpFrame::~SlpFrame()
{
}

//------------------------------------------------------------------------------
void SlpFrame::setSlpFilePos(std::streampos pos)
{
  slp_file_pos_ = pos;
}

uint32_t SlpFrame::getWidth(void) const
{
  return width_;
}

uint32_t SlpFrame::getHeight(void) const
{
  return height_;
}

void SlpFrame::setSize(const uint32_t width, const uint32_t height)
{
  width_ = width;
  height_ = height;
  if (is32bit())
  {
    img_data.bgra_channels.clear();
    img_data.bgra_channels.resize(width * height, 0);
  }
  else
  {
    img_data.pixel_indexes.clear();
    img_data.alpha_channel.clear();
    img_data.pixel_indexes.resize(width * height, 0);
    img_data.alpha_channel.resize(width * height, 0);
  }
}

void SlpFrame::enlarge(const uint32_t width, const uint32_t height, const int32_t offset_x, const int32_t offset_y)
{
  if(width_ > width && height_ > height) return;

  if (is32bit())
  {
    //std::vector<uint32_t> new_bgra_channels(width * height, 0);
    //img_data.transparency_mask;
  }
  else
  {
    std::vector<uint8_t> new_pixel_indexes(width * height, 0);
    std::vector<uint8_t> new_alpha_channel(width * height, 0);
    // If offsets are negative, the old pixels are lost. Care to fix?
    for(size_t r = 0, row = offset_y; r < height_ && row < height; ++r, ++row)
    {
      for(size_t c = 0, col = offset_x; c < width_ && col < width; ++c, ++col)
      {
        new_pixel_indexes[row * width + col] = img_data.pixel_indexes[r * width_ + c];
        new_alpha_channel[row * width + col] = img_data.alpha_channel[r * width_ + c];
      }
    }
    img_data.pixel_indexes = new_pixel_indexes;
    img_data.alpha_channel = new_alpha_channel;
  }

  // You better not crop the frame.
  for(auto &xy: img_data.shadow_mask)
  {
    xy.x += offset_x;
    xy.y += offset_y;
  }
  for(auto &xy: img_data.shield_mask)
  {
    xy.x += offset_x;
    xy.y += offset_y;
  }
  for(auto &xy: img_data.outline_pc_mask)
  {
    xy.x += offset_x;
    xy.y += offset_y;
  }
  for(auto &xy: img_data.player_color_mask)
  {
    xy.x += offset_x;
    xy.y += offset_y;
  }

  hotspot_x += offset_x;
  hotspot_y += offset_y;
  width_ = width;
  height_ = height;
}

void SlpFrame::enlargeForMerge(const SlpFrame &frame, int32_t &os_x, int32_t &os_y)
{
  const int32_t hsdx = frame.hotspot_x - hotspot_x, hsdy = frame.hotspot_y - hotspot_y,
    hsrdx = int32_t(frame.getWidth()) - frame.hotspot_x - (int32_t(width_) - hotspot_x),
    hsrdy = int32_t(frame.getHeight()) - frame.hotspot_y - (int32_t(height_) - hotspot_y);
  uint32_t width = width_, height = height_;
  int32_t offset_x = 0, offset_y = 0;
  if(uint32_t(hsdx) < width_)
  {
    // Add space to left side.
    offset_x = hsdx;
    width += hsdx;
  }
  if(uint32_t(hsdy) < height_)
  {
    // Add space to top side.
    offset_y = hsdy;
    height += hsdy;
  }
  if(uint32_t(hsrdx) < width_)
  {
    // Add space to right side.
    width += hsrdx;
  }
  if(uint32_t(hsrdy) < height_)
  {
    // Add space to bottom side.
    height += hsrdy;
  }
  os_x = -hsdx + offset_x;
  os_y = -hsdy + offset_y;
  enlarge(width, height, offset_x, offset_y);
}

uint32_t SlpFrame::getPaletteOffset(void) const
{
  return palette_offset_;
}

uint32_t SlpFrame::getProperties(void) const
{
  return properties_;
}

bool SlpFrame::is32bit(void) const
{
  return (properties_ & 7) == 7;
}

void SlpFrame::serializeObject(void)
{
}

void SlpFrame::setLoadParams(std::istream &istr)
{
  setIStream(istr);
  setOperation(OP_READ);
}

void SlpFrame::setSaveParams(std::ostream &ostr, uint32_t &slp_offset_)
{
  setOStream(ostr);
  setOperation(OP_WRITE);
#ifndef NDEBUG
  std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
#endif

  assert(height_ < 4096);
  outline_table_offset_ = slp_offset_;
  cmd_table_offset_ = slp_offset_ + 4 * height_;
  slp_offset_ = cmd_table_offset_ + 4 * height_;

  // TODO: Build integers from image data.
  left_edges_.resize(height_);
  right_edges_.resize(height_);
  cmd_offsets_.resize(height_);
  commands_.resize(height_);
  uint32_t player_color_slot = 0;
  uint32_t shadow_slot = 0;
  uint32_t shield_slot = 0;
  uint32_t outline_pc_slot = 0;
  uint32_t transparent_slot = 0;
  // Ensure that all 8-bit masks get saved.
  for(auto const &pixel: img_data.outline_pc_mask)
    img_data.alpha_channel[pixel.y * width_ + pixel.x] = 255;
  for(auto const &pixel: img_data.shield_mask)
    img_data.alpha_channel[pixel.y * width_ + pixel.x] = 255;
  {
    std::vector<XY> new_shadow_mask;
    new_shadow_mask.reserve(img_data.shadow_mask.size());
    for(auto const &pixel: img_data.shadow_mask)
    {
      auto loc = pixel.y * width_ + pixel.x;
      if(img_data.alpha_channel[loc] == 0)
      {
        new_shadow_mask.emplace_back(pixel);
        img_data.alpha_channel[loc] = 255;
      }
    }
    new_shadow_mask.shrink_to_fit();
    img_data.shadow_mask = new_shadow_mask;
  }

  for (uint32_t row = 0; row < height_; ++row)
  {
    cmd_offsets_[row] = slp_offset_;
    // Count left edge
    left_edges_[row] = 0;
    if (is32bit())
    {
      for (uint32_t col = 0; col < width_; ++col)
      {
        if (img_data.bgra_channels[row * width_ + col] == 0)
          ++left_edges_[row];
        else break;
      }
    }
    else
    {
      for (uint32_t col = 0; col < width_; ++col)
      {
        if (img_data.alpha_channel[row * width_ + col] == 0)
          ++left_edges_[row];
        else break;
      }
    }
    // Fully transparent row
    if (left_edges_[row] == width_)
    {
      left_edges_[row] = 0x8000;
      continue;
    }
    // Read colors and count right edge
    uint16_t color_index = 0x100;
    uint32_t bgra = 0;
    uint32_t pixel_set_size = 0;
    cnt_type count_type = CNT_LEFT;
    for (uint32_t col = left_edges_[row]; col < width_; ++col)
    {
      ++pixel_set_size;
      uint16_t last_color = color_index;
      uint32_t last_bgra = bgra;
      cnt_type old_count = count_type;

      if (player_color_slot < img_data.player_color_mask.size())
      {
        if (img_data.player_color_mask[player_color_slot].x == col
          && img_data.player_color_mask[player_color_slot].y == row)
        {
          count_type = CNT_PLAYER;
          ++player_color_slot;
          goto COUNT_SWITCH;
        }
      }
      if (outline_pc_slot < img_data.outline_pc_mask.size())
      {
        if (img_data.outline_pc_mask[outline_pc_slot].x == col
          && img_data.outline_pc_mask[outline_pc_slot].y == row)
        {
          count_type = CNT_PC_OUTLINE;
          ++outline_pc_slot;
          goto COUNT_SWITCH;
        }
      }
      if (shield_slot < img_data.shield_mask.size())
      {
        if (img_data.shield_mask[shield_slot].x == col
          && img_data.shield_mask[shield_slot].y == row)
        {
          count_type = CNT_SHIELD;
          ++shield_slot;
          goto COUNT_SWITCH;
        }
      }
      if (shadow_slot < img_data.shadow_mask.size())
      {
        if (img_data.shadow_mask[shadow_slot].x == col
          && img_data.shadow_mask[shadow_slot].y == row)
        {
          count_type = CNT_SHADOW;
          ++shadow_slot;
          goto COUNT_SWITCH;
        }
      }
      if (is32bit())
      {
        bgra = img_data.bgra_channels[row * width_ + col];
        if (transparent_slot < img_data.transparency_mask.size())
        {
          if (img_data.transparency_mask[transparent_slot].x == col
            && img_data.transparency_mask[transparent_slot].y == row)
          {
            count_type = CNT_FEATHERING;
            ++transparent_slot;
            goto COUNT_SWITCH;
          }
        }
        count_type = last_bgra == bgra ? CNT_SAME : CNT_DIFF;
      }
      else
      {
        if (img_data.alpha_channel[row * width_ + col] == 0)
        {
          count_type = CNT_TRANSPARENT;
          goto COUNT_SWITCH;
        }
        color_index = img_data.pixel_indexes[row * width_ + col];
        count_type = last_color == color_index ? CNT_SAME : CNT_DIFF;
        goto KEEP_COLOR;
      }

COUNT_SWITCH:
      color_index = 0x100;
KEEP_COLOR:
      if (old_count != count_type)
      {
        switch (old_count)
        {
          case CNT_LEFT:
            break;
          case CNT_DIFF:
            if (count_type == CNT_SAME)
            {
              handleColors(CNT_DIFF, row, col - 1, pixel_set_size - 2);
              pixel_set_size = 2;
            }
            else
            {
              handleColors(CNT_DIFF, row, col, --pixel_set_size);
              pixel_set_size = 1;
            }
            break;
          default:
            handleColors(old_count, row, col, --pixel_set_size);
            pixel_set_size = 1;
            break;
        }
      }
    }
    // Handle last colors
    if (is32bit() ? bgra == 0 : color_index == 0x100)
    {
      right_edges_[row] = pixel_set_size;
    }
    else
    {
      right_edges_[row] = 0;
      handleColors(count_type, row, width_, pixel_set_size);
    }
    // End of line
    commands_[row].push_back(0x0F);
    slp_offset_ += commands_[row].size();
  }
#ifndef NDEBUG
  std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
  log.debug("Frame (%u bytes) encoding took [%u] milliseconds", slp_offset_ - outline_table_offset_, std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
#endif
}

//------------------------------------------------------------------------------
void SlpFrame::serializeHeader(void)
{
  serialize<uint32_t>(cmd_table_offset_);
  serialize<uint32_t>(outline_table_offset_);
  serialize<uint32_t>(palette_offset_);

  // 0x00 = use default palette
  // 0x08 = only 1 pcs in TC, seems to be useless leftover from AoE 1, mostly containing player colors.
  // 0x10 = tree SLPs 147 and 152 in RoR have two shadows, mask and black pixels. Has pure black shadow? No
  // 0x18 = use default palette, 0x08 uses outline? No
  // 0x78 = has embedded palette at palette offset: 4 bytes tell RGB count, then 3 bytes per each RGB
  serialize<uint32_t>(properties_);

  serialize<uint32_t>(width_);
  serialize<uint32_t>(height_);

  serialize<int32_t>(hotspot_x);
  serialize<int32_t>(hotspot_y);

/*#ifndef NDEBUG
  log.debug("Frame header [%u], [%u], [%u], [%u], [%u], [%u], [%d], [%d], ",
    cmd_table_offset_, outline_table_offset_, palette_offset_, properties_,
    width_, height_, hotspot_x, hotspot_y);
#endif*/
}

//------------------------------------------------------------------------------
void SlpFrame::load(std::istream &istr)
{
  setIStream(istr);

  if (is32bit())
  {
    img_data.bgra_channels.resize(width_ * height_, 0);
  }
  else
  {
    img_data.pixel_indexes.resize(width_ * height_);
    img_data.alpha_channel.resize(width_ * height_, 0);
  }

  uint16_t integrity = 0;
  istr.seekg(slp_file_pos_ + std::streampos(outline_table_offset_));
  readEdges(integrity);

  istr.seekg(slp_file_pos_ + std::streampos(cmd_table_offset_));
  serialize<uint32_t>(cmd_offsets_, height_);

  // Read embedded palette
  if (properties_ == 0x78)
  {
    istr.seekg(slp_file_pos_ + std::streampos(palette_offset_));
    img_data.palette.resize(read<uint32_t>());
    for (auto &rgba: img_data.palette)
    {
      rgba.r = read<uint8_t>();
      rgba.g = read<uint8_t>();
      rgba.b = read<uint8_t>();
    }
  }

  if (integrity != 0x8000) // At least one visible row.
  // Each row has it's commands, 0x0F signals the end of a rows commands.
  for (uint32_t row = 0; row < height_; ++row)
  {
    istr.seekg(slp_file_pos_ + std::streampos(cmd_offsets_[row]));
    assert(!istr.eof());
    // Transparent rows apparently read one byte anyway. NO THEY DO NOT! Ignore and use seekg()
    if (0x8000 == left_edges_[row] || 0x8000 == right_edges_[row]) // Remember signedness!
    {
      continue; // Pretend it does not exist.
    }
    uint32_t pix_pos = left_edges_[row]; //pos where to start putting pixels

    uint8_t data = 0;
    while (true)
    {
      data = read<uint8_t>();

      if (data == 0x0F) break;

      uint8_t cmd = data & 0x0F;
      uint8_t sub = data & 0xF0;

      uint32_t pix_cnt = 0;

      switch (cmd) //0x00
      {
        case 0x0: // Lesser block copy
        case 0x4:
        case 0x8:
        case 0xC:
          pix_cnt = (data & 0xFC) >> 2;
          if (is32bit())
            readPixelsToImage32(row, pix_pos, pix_cnt);
          else
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
          if (is32bit())
            readPixelsToImage32(row, pix_pos, pix_cnt);
          else
            readPixelsToImage(row, pix_pos, pix_cnt);
          break;

        case 0x3: // greater skip
          pix_cnt = (sub << 4) + read<uint8_t>();
          pix_pos += pix_cnt;
          break;

        case 0x6: // copy and transform (player color)
          pix_cnt = getPixelCountFromData(data);
          if (is32bit())
            readPixelsToImage32(row, pix_pos, pix_cnt, 1);
          else
            readPixelsToImage(row, pix_pos, pix_cnt, true);
          break;

        case 0x7: // Run of plain color
          pix_cnt = getPixelCountFromData(data);
          if (is32bit())
            setPixelsToColor32(row, pix_pos, pix_cnt);
          else
            setPixelsToColor(row, pix_pos, pix_cnt);
          break;

        case 0xA: // Transform block (player color)
          pix_cnt = getPixelCountFromData(data);
          if (is32bit())
            setPixelsToColor32(row, pix_pos, pix_cnt, true);
          else
            setPixelsToColor(row, pix_pos, pix_cnt, true);
          break;

        case 0xB: // Shadow pixels
          pix_cnt = getPixelCountFromData(data);
          setPixelsToShadow(row, pix_pos, pix_cnt);
          break;

        case 0xE: // extended commands.. TODO
          switch (data)
          {
            case 0x0E: //xflip?? skip?? TODO
            case 0x1E:
              log.error("Cmd [%X] not implemented", data);
              return;
              //row-= 1;
              break;

            case 0x2E:
            case 0x3E:
              log.error("Cmd [%X] not implemented", data);
              return;
              break;

            case 0x4E:
              setPixelsToPcOutline(row, pix_pos, 1);//, 242);
              break;
            case 0x6E:
              setPixelsToShield(row, pix_pos, 1);//, 0);
              break;

            case 0x5E:
              pix_cnt = read<uint8_t>();
              setPixelsToPcOutline(row, pix_pos, pix_cnt);//, 242);
              break;
            case 0x7E:
              pix_cnt = read<uint8_t>();
              setPixelsToShield(row, pix_pos, pix_cnt);//, 0);
              break;
            case 0x9E: // Apparently some kind of edge blending to background.
              pix_cnt = read<uint8_t>();
              if (is32bit())
              {
                readPixelsToImage32(row, pix_pos, pix_cnt, 2);
                break;
              }
            default:
              log.error("Cmd [%X] not implemented", data);
              return;
          }
          break;
        default:
          log.error("Unknown cmd [%X]", data);
          std::cerr << "SlpFrame: Unknown cmd at " << std::hex << std::endl;
          return;
      }
    }
  }
}

//------------------------------------------------------------------------------
void SlpFrame::readEdges(uint16_t &integrity)
{
  left_edges_.resize(height_);
  right_edges_.resize(height_);

  for (uint32_t row = 0; row < height_; ++row)
  {
    serialize<uint16_t>(left_edges_[row]);
    serialize<uint16_t>(right_edges_[row]);
    integrity |= left_edges_[row];
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
    assert(row * width_ + col < img_data.pixel_indexes.size());
    img_data.pixel_indexes[row * width_ + col] = color_index;
    img_data.alpha_channel[row * width_ + col] = 255;
    if (player_col)
    {
      img_data.player_color_mask.push_back({col, row, color_index});
    }
    ++col;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::readPixelsToImage32(uint32_t row, uint32_t &col,
                                 uint32_t count, uint8_t special)
{
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    uint32_t bgra = read<uint32_t>();
    assert(row * width_ + col < img_data.bgra_channels.size());
    img_data.bgra_channels[row * width_ + col] = bgra;
    if (special == 1)
    {
      img_data.player_color_mask.push_back({col, row, 0});
    }
    else if (special == 2)
    {
      img_data.transparency_mask.push_back({col, row});
    }
    ++col;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::setPixelsToColor(uint32_t row, uint32_t &col, uint32_t count,
                                bool player_col)
{
  uint8_t color_index = read<uint8_t>();
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    assert(row * width_ + col < img_data.pixel_indexes.size());
    img_data.pixel_indexes[row * width_ + col] = color_index;
    img_data.alpha_channel[row * width_ + col] = 255;
    if (player_col)
    {
      img_data.player_color_mask.push_back({col, row, color_index});
    }
    ++col;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::setPixelsToColor32(uint32_t row, uint32_t &col, uint32_t count,
                                bool player_col)
{
  uint32_t bgra = read<uint32_t>();
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    assert(row * width_ + col < img_data.bgra_channels.size());
    img_data.bgra_channels[row * width_ + col] = bgra;
    if (player_col)
    {
      img_data.player_color_mask.push_back({col, row, 0});
    }
    ++col;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::setPixelsToShadow(uint32_t row, uint32_t &col, uint32_t count)
{
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    img_data.shadow_mask.push_back({col, row});
    ++col;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::setPixelsToShield(uint32_t row, uint32_t &col, uint32_t count)
{
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    img_data.shield_mask.push_back({col, row});
    ++col;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::setPixelsToPcOutline(uint32_t row, uint32_t &col, uint32_t count)
{
  uint32_t to_pos = col + count;
  while (col < to_pos)
  {
    img_data.outline_pc_mask.push_back({col, row});
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

//------------------------------------------------------------------------------
void SlpFrame::handleColors(cnt_type count_type, uint32_t row, uint32_t col, uint32_t count)
{
  if (count == 0) return;
  switch (count_type)
  {
    case CNT_TRANSPARENT:
      if (count > 0x3F) // Greater skip.
      {
        commands_[row].push_back(0x3 | (count & 0xF00) >> 4);
        commands_[row].push_back(count);
      }
      else // Lesser skip.
      {
        commands_[row].push_back(0x1 | count << 2);
      }
      break;
    case CNT_SAME:
      handleSpecial(0x7, row, col, count, 1);
      break;
    case CNT_DIFF:
      if (count > 0x3F) // Greater copy.
      {
        commands_[row].push_back(0x2 | (count & 0xF00) >> 4);
        commands_[row].push_back(count);
        pushPixelsToBuffer(row, col, count);
      }
      else // Lesser copy.
      {
        commands_[row].push_back(count << 2);
        pushPixelsToBuffer(row, col, count);
      }
      break;
    case CNT_FEATHERING:
      commands_[row].push_back(0x9E);
      commands_[row].push_back(count);
      pushPixelsToBuffer(row, col, count);
      break;
    case CNT_PLAYER:
      handleSpecial(0x6, row, col, count, count);
      break;
    case CNT_SHIELD:
      if (count == 1)
      {
        commands_[row].push_back(0x6E);
      }
      else
      {
        commands_[row].push_back(0x7E);
        commands_[row].push_back(count);
      }
      break;
    case CNT_PC_OUTLINE:
      if (count == 1)
      {
        commands_[row].push_back(0x4E);
      }
      else
      {
        commands_[row].push_back(0x5E);
        commands_[row].push_back(count);
      }
      break;
    case CNT_SHADOW:
      handleSpecial(0xB, row, col, count, 0);
      break;
    default: break;
  }
}

//------------------------------------------------------------------------------
void SlpFrame::handleSpecial(uint8_t cmd, uint32_t row, uint32_t col, uint32_t count, uint32_t pixs)
{
  while (count > 0xFF)
  {
    count -= 0xFF;
    commands_[row].push_back(cmd);
    commands_[row].push_back(0xFF);
    pushPixelsToBuffer(row, col, pixs);
  }
  if (count > 0xF)
  {
    commands_[row].push_back(cmd);
    commands_[row].push_back(count);
    pushPixelsToBuffer(row, col, pixs);
  }
  else
  {
    commands_[row].push_back(cmd | count << 4);
    pushPixelsToBuffer(row, col, pixs);
  }
}

//------------------------------------------------------------------------------
void SlpFrame::pushPixelsToBuffer(uint32_t row, uint32_t col, uint32_t count)
{
  if (is32bit())
  {
    for (uint32_t pix = col - count; pix < col; ++pix)
    {
      uint32_t bgra = img_data.bgra_channels[row * width_ + pix];
      commands_[row].push_back(bgra);
      commands_[row].push_back(bgra >> 8);
      commands_[row].push_back(bgra >> 16);
      commands_[row].push_back(bgra >> 24);
    }
  }
  else
  {
    for (uint32_t pix = col - count; pix < col; ++pix)
    {
      commands_[row].push_back(img_data.pixel_indexes[row * width_ + pix]);
    }
  }
}

//------------------------------------------------------------------------------
void SlpFrame::save(std::ostream &ostr)
{
  setOStream(ostr);
#ifndef NDEBUG
  std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
#endif

  //Write edges
  for (uint32_t row = 0; row < height_; ++row)
  {
    serialize<uint16_t>(left_edges_[row]);
    serialize<uint16_t>(right_edges_[row]);
  }

  //Write cmd offsets
  serialize<uint32_t>(cmd_offsets_, height_);
  cmd_offsets_.clear();

  for (auto &commands: commands_)
    for (auto &col: commands)
      serialize<uint8_t>(col);

  commands_.clear();
#ifndef NDEBUG
  std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
  log.debug("SLP frame data saving took [%u] milliseconds", std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
#endif
}

}
