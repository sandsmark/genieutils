/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P

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

#ifndef GENIE_BLENDOMATICFILE_H
#define GENIE_BLENDOMATICFILE_H

#include <istream>
#include <vector>

#include "genie/file/IFile.h"
#include "genie/util/Logger.h"
#include "PalFile.h"

namespace genie
{

struct BlendMode
{
    uint32_t pixelCount;

    // one per tile, 0 == no alpha, 1 == has alpha
    // apparently all tiles usually have alpha
    std::vector<uint8_t> tileHasAlpha;

    // one bit per pixel, indicating if there is alpha
    // the bits for the tiles are interleaved
    // bit for pixel in tile: alphaBitmap[pixel] & 1 << tile
    std::vector<uint32_t> alphaBitmap;

    // alpha value in 0x0 - 0x80 (0 - 128)
    std::vector<std::vector<uint8_t>> alphaValues;

    uint32_t unknown;
};

typedef std::shared_ptr<BlendMode> BlendModePtr;

class BlendomaticFile : public IFile
{

public:
  //----------------------------------------------------------------------------
  /// Constructor
  //
  BlendomaticFile();

  //----------------------------------------------------------------------------
  /// Destructor
  //
  virtual ~BlendomaticFile();

  //----------------------------------------------------------------------------
  /// Frees all content of a slp file.
  //
  void unload(void);

  void setBlendMode(uint32_t number, BlendModePtr mode);
  BlendModePtr getBlendMode(uint32_t id = 0);

private:
  static Logger &log;

//  bool loaded_ = false;

  // 4 in AoK, 9 in AoC, apparently ignored by the game
  uint32_t modeCount_;
  // 31, apparently ignored by the game
  uint32_t tileCount_;
  std::vector<BlendModePtr> modes_;

  //----------------------------------------------------------------------------
  virtual void serializeObject(void);
};

typedef std::shared_ptr<BlendomaticFile> BlendomaticFilePtr;

}

#endif // GENIE_BLENDOMATICFILE_H
