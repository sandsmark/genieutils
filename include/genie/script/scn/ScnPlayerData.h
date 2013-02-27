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

#ifndef GENIE_SCN_PLAYER_DATA_H
#define GENIE_SCN_PLAYER_DATA_H

#include "genie/file/ISerializable.h"

#include <stdint.h>

namespace genie
{
  
class AiFile;
class Resources;
  
class ScnPlayerData1 : public ISerializable
{
public:
  ScnPlayerData1();
  virtual ~ScnPlayerData1();
  
  /// boolean
  uint32_t active; 
  
  /// boolean
  uint32_t human;
  
  uint32_t civilizationId;
  
  /// constant = 4 ??
  uint32_t unknown1;
  
private:
  virtual void serializeObject(void);
};
  
class ScnPlayerData2 : public ISerializable
{
public:
  ScnPlayerData2();
  virtual ~ScnPlayerData2();
  
  /// 32
  std::vector<std::string> unknownStrings;
  
  /// 16
  std::vector<std::string> aiNames;
  
  std::vector<AiFile> aiFiles;
  
  /// 0 = custom, 1 = standard, 2 = none. TODO: enum?
  std::vector<uint8_t> aiTypes;
  
  std::vector<Resources> resources;
  
private:
  uint32_t separator_;
  
  virtual void serializeObject(void);
  
};
  
class AiFile : public ISerializable
{
public:
  AiFile();
  virtual ~AiFile();
  
  /// Always 0
  uint32_t unknown1;
  
  /// Always 0
  uint32_t unknown2;
  
  /// .per text 
  std::string perFile;
  
private:
  virtual void serializeObject(void);
  
};

class Resources : public ISerializable
{
public:
  Resources();
  virtual ~Resources();
  
  uint32_t gold;
  uint32_t wood;
  uint32_t food;
  uint32_t stone;
  
  /// ?
  uint32_t oreX; 
  
  /// always 0
  uint32_t padding;
  
private:
  virtual void serializeObject(void);
};


}

#endif // GENIE_SCN_PLAYER_DATA_H