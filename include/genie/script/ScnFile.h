/*
    genieutils - <description>
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2015  Mikko "Tapsa" P <email>

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

#include "scn/ScnPlayerData.h"
#include "scn/MapDescription.h"
#include "scn/Trigger.h"

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

  int32_t saveType;

  /// Timestamp of last save
  uint32_t lastSaveTime;

  std::string scenarioInstructions;

  uint32_t victoryType;

  uint32_t playerCount;

  // Compressed header:

  /// aokts description: "Next unit ID to place" ??
  uint32_t nextUnitID;

  ScnPlayerData1 playerData1;

  MapDescription map;
  MapUnits units;
  ScnPlayerData3 playerData3;
  Trigger triggers;

  std::string version;

private:
  uint32_t headerLength_; //starting after this

  Compressor compressor_;

  virtual void serializeObject(void);

  void serializeVersion(void);
};

}

#endif // GENIE_SCNFILE_H
