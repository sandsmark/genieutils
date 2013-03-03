/*
    genieutils - <description>
    Copyright (C) 2011 - 2013  Armin Preiml <email>

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

#ifndef GENIE_SCN_RESOURCE_H
#define GENIE_SCN_RESOURCE_H

#include "genie/file/ISerializable.h"

#include <stdint.h>

namespace genie
{
  
// TODO: right name?
class ScnResource : public ISerializable
{
public:
  ScnResource();
  virtual ~ScnResource();
  
  /// GV >= AoK
  uint32_t instructionsStringTable;
  
  /// GV >= AoK
  uint32_t hintsStringTable;
  
  /// GV >= AoK
  uint32_t victoryStringTable;
  
  /// GV >= AoK
  uint32_t lossStringTable;
  
  /// GV >= AoK
  uint32_t historyStringTable;
  
  /// GV >= TC
  uint32_t scoutsStringTable;
  
  // Messages
  
  std::string instructions;
  
  std::string hints;
  
  std::string victory;
  
  std::string loss;
  
  std::string history;
  
  /// GV >= TC
  std::string scouts;
  
  std::string pregameCinematicFilename;
  
  std::string victoryCinematicFilename;
  
  std::string lossCinematicFilename;
  
  std::string backgroundFilename;
  
  // Bitmap
  
  /// Boolean
  uint32_t bitmapIncluded;
  
  uint32_t bitmapWidth;
  
  uint32_t bitmapHeigth;
  
  /// -1 if there's a bitmap, 1 otherwise
  int16_t unknown1;
  
  uint16_t bitmapByteSize;
  
  char* bitmap;
    
  
private:
  virtual void serializeObject(void);
  
  void serializeBitmap(void);
  
};
  
}

#endif // GENIE_SCN_RESOURCE_H