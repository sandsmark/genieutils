/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2017  Mikko "Tapsa" P

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

#include "genie/dat/DatFile.h"

#include <fstream>
#include <vector>

#include "genie/Types.h"

namespace genie
{

float ISerializable::dat_internal_ver = 0.f;
GameVersion GV_LatestTap = GV_T8;

//------------------------------------------------------------------------------
DatFile::DatFile() : compressor_(this)
{
}

//------------------------------------------------------------------------------
DatFile::~DatFile()
{
  unload();
}

//------------------------------------------------------------------------------
void DatFile::setGameVersion(GameVersion gv)
{
  ISerializable::setGameVersion(gv);

  updateGameVersion(TerrainRestrictions);
  updateGameVersion(PlayerColours);
  updateGameVersion(Sounds);
  updateGameVersion(Graphics);
  updateGameVersion(Effects);
  updateGameVersion(UnitLines);
  updateGameVersion(UnitHeaders);
  updateGameVersion(Civs);
  updateGameVersion(Techs);
  TerrainBlock.setGameVersion(gv);
  RandomMaps.setGameVersion(gv);
  TechTree.setGameVersion(gv);
}

//------------------------------------------------------------------------------
void DatFile::extractRaw(const char *inFile, const char *outFile)
{
  std::ifstream ifs;
  std::ofstream ofs;

  ifs.open(inFile, std::ios::binary);
  ofs.open(outFile, std::ios::binary);

  Compressor::decompress(ifs, ofs);

  ifs.close();
  ofs.close();
}

//------------------------------------------------------------------------------
void DatFile::setVerboseMode(bool verbose)
{
  verbose_ = verbose;
}

//------------------------------------------------------------------------------
void DatFile::serializeObject(void)
{
  compressor_.beginCompression();

  serialize(FileVersion, FILE_VERSION_SIZE);

  // Handle all different versions while in development.
  if(getGameVersion() == GV_Tapsa)
  {
    if("VER 4.5" == FileVersion)
      setGameVersion(GV_T8);
    else if("VER 4.4" == FileVersion)
      setGameVersion(GV_T7);
    else if("VER 4.3" == FileVersion)
      setGameVersion(GV_T6);
    else if("VER 4.2" == FileVersion)
      setGameVersion(GV_T5);
    else if("VER 4.1" == FileVersion)
      setGameVersion(GV_T4);
    else if("VER 4.0" == FileVersion)
      setGameVersion(GV_T3);
    else if("VER 3.9" == FileVersion)
      setGameVersion(GV_T2);
  }

  GameVersion gv = getGameVersion();
  uint16_t count16;
  uint32_t count32;

  if (gv >= GV_SWGB)
  {
    serializeSize<uint16_t>(count16, Civs.size());
    serialize<int32_t>(SUnknown2);
    serialize<int32_t>(SUnknown3);
    serialize<int32_t>(SUnknown4);
    serialize<int32_t>(SUnknown5);

    if (verbose_)
    {
      std::cout << "Unkown1: " << count16 << std::endl;
      std::cout << "Unkown2: " << SUnknown2 << std::endl;
      std::cout << "Unkown3: " << SUnknown3 << std::endl;
      std::cout << "Unkown4: " << SUnknown4 << std::endl;
      std::cout << "Unkown5: " << SUnknown5 << std::endl;
    }
  }

  serializeSize<uint16_t>(count16, TerrainRestrictions.size());
  serialize<uint16_t>(TerrainsUsed1);

  if (verbose_)
  {
    std::cout << FileVersion;
    std::cout << std::endl << "TerRestrictionCount: " << count16 << std::endl;
    std::cout << "TerCount: " << TerrainsUsed1 << std::endl;
  }

  serialize<int32_t>(FloatPtrTerrainTables, count16);

  if (gv >= GV_AoKA)
    serialize<int32_t>(TerrainPassGraphicPointers, count16);

  TerrainRestriction::setTerrainCount(TerrainsUsed1);
  serializeSub<TerrainRestriction>(TerrainRestrictions, count16);

  serializeSize<uint16_t>(count16, PlayerColours.size());

  if (verbose_)
    std::cout << "PlayerColours: " << count16 << std::endl;

  serializeSub<PlayerColour>(PlayerColours, count16);

  serializeSize<uint16_t>(count16, Sounds.size());

  if (verbose_)
    std::cout << "Sounds: " << count16 << std::endl;

  serializeSub<Sound>(Sounds, count16);

  serializeSize<uint16_t>(count16, Graphics.size());
  if (gv < GV_AoE)
  {
    serializeSub<Graphic>(Graphics, count16);
  }
  else
  {
    serialize<int32_t>(GraphicPointers, count16);
    serializeSubWithPointers<Graphic>(Graphics, count16, GraphicPointers);
  }

  auto pos_cnt = tellg();
  if (verbose_)
  {
    std::cout << "Graphics: " << Graphics.size() << std::endl;
  }
  serialize<ISerializable>(TerrainBlock);

  if (verbose_)
  {
    pos_cnt = tellg() - pos_cnt;
    std::cout << "Terrain block " << "(0x" << std::hex << pos_cnt;
    std::cout << " to 0x" << tellg() << std::dec << ") size " << pos_cnt << std::endl;
  }

  // This data seems to be needed only in AoE and RoR.
  // In later games it is removable.
  // It exists in Star Wars games too, but is not used.
  serialize<ISerializable>(RandomMaps);

  serializeSize<uint32_t>(count32, Effects.size());

  if (verbose_)
    std::cout << "Effects: " << count32 << std::endl;

  serializeSub<Effect>(Effects, count32);

  if (gv >= GV_SWGB) //pos: 0x111936
  {
    serializeSize<uint16_t>(count16, UnitLines.size());
    serializeSub<UnitLine>(UnitLines, count16);
  }

  if (gv >= GV_AoK)
  {
    serializeSize<uint32_t>(count32, UnitHeaders.size());

    if (verbose_)
      std::cout << "Units: " << count32 << std::endl;

    serializeSub<UnitHeader>(UnitHeaders, count32);
  }

  serializeSize<uint16_t>(count16, Civs.size());

  if (verbose_)
    std::cout << "Civs: " << count16 << std::endl;

  serializeSub<Civ>(Civs, count16);

  if (gv >= GV_SWGB)
    serialize<int8_t>(SUnknown7);

  serializeSize<uint16_t>(count16, Techs.size());

  if (verbose_)
    std::cout << "Techs: " << count16 << std::endl;

  serializeSub<Tech>(Techs, count16);

  if (verbose_)
  {
    pos_cnt = tellg();
    std::cout << "TechTrees (before eof) (0x" << std::hex << pos_cnt;
  }

  if (gv >= GV_SWGB)
    serialize<int8_t>(SUnknown8);

  if (gv >= GV_AoKA) // 9.38
  {
    serialize<int32_t>(TimeSlice);
    serialize<int32_t>(UnitKillRate);
    serialize<int32_t>(UnitKillTotal);
    serialize<int32_t>(UnitHitPointRate);
    serialize<int32_t>(UnitHitPointTotal);
    serialize<int32_t>(RazingKillRate);
    serialize<int32_t>(RazingKillTotal);

    serialize<ISerializable>(TechTree);
  }

  if (verbose_)
  {
    pos_cnt = tellg() - pos_cnt;
    std::cout << " to 0x" << tellg() << std::dec << ") size " << pos_cnt << std::endl;
  }

  compressor_.endCompression();
}

//------------------------------------------------------------------------------
void DatFile::unload()
{
  FloatPtrTerrainTables.clear();
  TerrainPassGraphicPointers.clear();
  TerrainRestrictions.clear();
  PlayerColours.clear();
  Sounds.clear();
  GraphicPointers.clear();
  Graphics.clear();
  TerrainBlock.Terrains.clear();
  TerrainBlock.TerrainBorders.clear();
  TerrainBlock.AoEAlphaUnknown.clear();
  TerrainBlock.SomeBytes.clear();
  TerrainBlock.SomeInt32.clear();
  RandomMaps.Maps.clear();
  Effects.clear();
  UnitLines.clear();
  UnitHeaders.clear();
  Civs.clear();
  Techs.clear();
  TechTree.TechTreeAges.clear();
  TechTree.BuildingConnections.clear();
  TechTree.UnitConnections.clear();
  TechTree.ResearchConnections.clear();
}

}
