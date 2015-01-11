/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml <email>
    Copyright (C) 2011 - 2015  Mikko T P

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

#include <boost/interprocess/streams/vectorstream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>

#include "genie/Types.h"

namespace genie
{

typedef boost::interprocess::basic_vectorstream<std::vector<char>> v_stream;

//------------------------------------------------------------------------------
DatFile::DatFile() : verbose_(false), file_name_(""), file_(0), compressor_(this)
{
  SUnknown2 = -1;
  SUnknown3 = -1;
  SUnknown4 = -1;
  SUnknown5 = -1;
  SUnknown7 = -1;
  SUnknown8 = -1;
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
  updateGameVersion(Techages);
  updateGameVersion(UnitLines);
  updateGameVersion(UnitHeaders);
  updateGameVersion(Civs);
  updateGameVersion(Researchs);
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

  serialize<std::string>(FileVersion, FILE_VERSION_SIZE);

  if (getGameVersion() >= genie::GV_SWGB)
  {
    serializeSize<uint16_t>(civ_countSW_, Civs.size());
    serialize<int32_t>(SUnknown2);
    serialize<int32_t>(SUnknown3);
    serialize<int32_t>(SUnknown4);
    serialize<int32_t>(SUnknown5);

    if (verbose_)
    {
      std::cout << "Unkown1: " << civ_countSW_ << std::endl;
      std::cout << "Unkown2: " << SUnknown2 << std::endl;
      std::cout << "Unkown3: " << SUnknown3 << std::endl;
      std::cout << "Unkown4: " << SUnknown4 << std::endl;
      std::cout << "Unkown5: " << SUnknown5 << std::endl;
    }
  }

  serializeSize<uint16_t>(terrain_restriction_count_, TerrainRestrictions.size());
  serialize<uint16_t>(TerrainsUsed1);

  if (verbose_)
  {
    std::cout << FileVersion;
    std::cout << std::endl << "TerRestrictionCount: " <<terrain_restriction_count_ << std::endl;
    std::cout << "TerCount: " << TerrainsUsed1 << std::endl;
  }

  serialize<int32_t>(TerrainRestrictionPointers1, terrain_restriction_count_);

  if (getGameVersion() >= genie::GV_AoKA)
    serialize<int32_t>(TerrainRestrictionPointers2, terrain_restriction_count_);

  TerrainRestriction::setTerrainCount(TerrainsUsed1);
  serializeSub<TerrainRestriction>(TerrainRestrictions, terrain_restriction_count_);

  serializeSize<uint16_t>(player_color_count_, PlayerColours.size());

  if (verbose_)
    std::cout << "PlayerColours: " << player_color_count_ << std::endl;

  serializeSub<PlayerColour>(PlayerColours, player_color_count_);

  serializeSize<uint16_t>(sound_count_, Sounds.size());

  if (verbose_)
    std::cout << "Sounds: " << sound_count_ << std::endl;

  serializeSub<Sound>(Sounds, sound_count_);

  serializeSize<uint16_t>(graphic_count_, GraphicPointers.size());
  if (getGameVersion() < genie::GV_AoE)
  {
    serializeSub<Graphic>(Graphics, graphic_count_);
  }
  else
  {
    serialize<int32_t>(GraphicPointers, graphic_count_);
    serializeSubWithPointers<Graphic>(Graphics, graphic_count_, GraphicPointers);
  }

  if (verbose_)
  {
    std::cout << "Graphics: " << Graphics.size() << std::endl;

    std::cout << "Terrain block " << "(0x" << std::hex << tellg();
  }
  serialize<ISerializable>(TerrainBlock);

  // This data seems to be needed only in AoE and RoR.
  // In later games it is removable.
  // It exists in Star Wars games too, but is not used.
  if (getGameVersion() >= genie::GV_AoEB) // Temp fix
  serialize<ISerializable>(RandomMaps);

  if (verbose_)
    std::cout << " to 0x" <<std::hex << tellg() << std::dec << ")" << std::endl;

  serializeSize<uint32_t>(techage_count_, Techages.size());

  if (verbose_)
    std::cout << "Techage: " << techage_count_ << std::endl;

  serializeSub<Techage>(Techages, techage_count_);

  if (getGameVersion() >= genie::GV_SWGB) //pos: 0x111936
  {
    serializeSize<uint16_t>(unit_line_count_, UnitLines.size());
    serializeSub<UnitLine>(UnitLines, unit_line_count_);
  }

  if (getGameVersion() >= genie::GV_AoK)
  {
    serializeSize<uint32_t>(unit_count_, UnitHeaders.size());

    if (verbose_)
      std::cout << "Unitcount: " << unit_count_ << std::endl;

    serializeSub<UnitHeader>(UnitHeaders, unit_count_);
  }

  serializeSize<uint16_t>(civ_count_, Civs.size());

  if (verbose_)
    std::cout << "Civcount: " << civ_count_ << std::endl;

  serializeSub<Civ>(Civs, civ_count_);

  if (getGameVersion() >= genie::GV_SWGB)
    serialize<int8_t>(SUnknown7);

  serializeSize<uint16_t>(research_count_, Researchs.size());

  if (verbose_)
    std::cout << "Researchcount: " << research_count_ << std::endl;

  serializeSub<Research>(Researchs, research_count_);

  if (verbose_)
    std::cout << "TechTrees (before eof) (0x" << std::hex << tellg();

  if (getGameVersion() >= genie::GV_SWGB)
    serialize<int8_t>(SUnknown8);

  if (getGameVersion() >= genie::GV_AoKA)
  {
    serialize<int32_t>(UnknownPreTechTree, 7);
    serialize<ISerializable>(TechTree);
  }

  if (verbose_)
    std::cout << "to 0x" <<std::hex << tellg() << std::dec << ")" << std::endl;

  compressor_.endCompression();
}

//------------------------------------------------------------------------------
void DatFile::unload()
{
  TerrainRestrictionPointers1.clear();
  TerrainRestrictionPointers2.clear();
  TerrainRestrictions.clear();
  PlayerColours.clear();
  Sounds.clear();
  GraphicPointers.clear();
  Graphics.clear();
  TerrainBlock.Terrains.clear();
  TerrainBlock.TerrainBorders.clear();
  TerrainBlock.AoEAlphaUnknown.clear();
  TerrainBlock.ZeroSpace.clear();
  TerrainBlock.CivData.clear();
  TerrainBlock.SomeBytes.clear();
  TerrainBlock.SomeInt32.clear();
  RandomMaps.MapHeaders.clear();
  RandomMaps.Maps.clear();
  Techages.clear();
  UnitLines.clear();
  UnitHeaders.clear();
  Civs.clear();
  Researchs.clear();
  UnknownPreTechTree.clear();
  TechTree.TechTreeAges.clear();
  TechTree.BuildingConnections.clear();
  TechTree.UnitConnections.clear();
  TechTree.ResearchConnections.clear();
}

}
