/*
    genieutils - <description>
    Copyright (C) 2013  Armin Preiml <email>

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

#ifndef GENIE_SCNFILE_H
#define GENIE_SCNFILE_H

#include <stdint.h>

#include "genie/file/IFile.h"
#include "genie/file/Compressor.h"

#include "scn/ScnResource.h"
#include "scn/ScnPlayerData.h"
#include "scn/MapDescription.h"

namespace genie
{
  
//------------------------------------------------------------------------------
/// Class to read and write scenario files (.scn and .scx). The version of the
/// game will be auto-detected on read. 
//
class ScnFile : public IFile
{
public:
  ScnFile();
  virtual ~ScnFile();
  
  //----------------------------------------------------------------------------
  /// Extracts a scenario (for debugging purpose).
  //
  void extractRaw(const char *from, const char *to);
  
  static uint32_t getSeparator(void);
  
  // Uncompressed Header:
  
  int32_t unknown1;
  
  /// Timestamp of last save
  uint32_t lastSaveTime;
  
  std::string scenarioInstructions;
  
  uint32_t unknown2;
  
  uint32_t playerCount;
  
  // Compressed header:
  
  /// aokts description: "Next unit ID to place" ??
  uint32_t unknown3; 
  
  /// 16
  std::vector<std::string> playerNames;
  
  /// GameVersion >= AoK, 16
  std::vector<uint32_t> playerNamesStringTable;
  
  /// 16
  std::vector<ScnPlayerData1> playerData1;
  
  /// usually 1
  uint32_t unknown4;
  
  char unknown5;
  
  /// always -1 ?
  float unknown6;
  
  std::string originalFileName;
  
  ScnResource resource;
  
  ScnPlayerData2 playerData2;
  
  MapDescription map;
  
private:
  std::string version_;
  
  uint32_t headerLength_; //starting after this
  
  float version2_;
  
  Compressor compressor_;
  
  virtual void serializeObject(void);
  
  void serializeVersion(void);
  void serializeVersion2(void);
};
  
}

#endif // GENIE_SCNFILE_H