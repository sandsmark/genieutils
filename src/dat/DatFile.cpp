/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
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

typedef boost::interprocess::basic_vectorstream< std::vector<char> > v_stream;

//------------------------------------------------------------------------------
DatFile::DatFile() : GraphicsRendering(0), ZeroSpace(0), Rendering(0), Something(0), CivSkip(0),
               UnknownPreTechTree(0),
               verbose_(false), file_name_(""), file_(0),
               file_version_(0), compressor_(this)
{
  SUnknown2 = -1;
  SUnknown3 = -1;
  SUnknown4 = -1;
  SUnknown5 = -1;
  Unknown2 = 0;
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
  updateGameVersion(Terrains);
  updateGameVersion(TerrainBorders);
  updateGameVersion(Techages);
  updateGameVersion(UnitHeaders);
  updateGameVersion(Civs);
  updateGameVersion(Researchs);
  updateGameVersion(UnitLines);
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
void DatFile::serializeObject(void )
{
  compressor_.beginCompression();

  serialize<char>(&file_version_, FILE_VERSION_LEN);

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
  serialize<uint16_t>(NumberOfTerrainsUsed);

  if (verbose_)
  {
    std::cout << file_version_ << std::endl;
    std::cout << "TerRestrictionCount: " <<terrain_restriction_count_ << std::endl;
    std::cout << "TerCount: " << NumberOfTerrainsUsed << std::endl;
  }

  serialize<int32_t>(TerrainRestrictionPointers1, terrain_restriction_count_);

  if (getGameVersion() >= genie::GV_AoKA)
    serialize<int32_t>(TerrainRestrictionPointers2, terrain_restriction_count_);

  TerrainRestriction::setTerrainCount(NumberOfTerrainsUsed);
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
  serialize<int32_t>(GraphicPointers, graphic_count_);
  serializeSubWithPointers<Graphic>(Graphics, graphic_count_, GraphicPointers);

  if (verbose_)
  {
    std::cout << "Graphics: " << Graphics.size() << std::endl;

    std::cout << "GraphicsRendering " << "(0x" << std::hex << tellg();
  }
  //TODO: AoE/RoR: maybe:
  // struct { short unknown[3]; } terrainheader[terrain_count_]
  serialize<int16_t>(&GraphicsRendering, TERRAIN_HEADER_SIZE);

  if (verbose_)
    std::cout << " to 0x" << std::hex << tellg() << std::dec << ")" << std::endl;

  switch (getGameVersion())
  {
    case genie::GV_AoE:
    case genie::GV_RoR:
    case genie::GV_AoKA:
    case genie::GV_AoK:  serializeSub<Terrain>(Terrains, 32); break;
    case genie::GV_TC:   serializeSub<Terrain>(Terrains, 42); break;
    case genie::GV_SWGB:
    case genie::GV_CC:   serializeSub<Terrain>(Terrains, 55); break;
    default: break;
  }

  if (verbose_)
    std::cout << "Unknown2 (empty) (0x" << std::hex << tellg() << std::dec;

  serialize<int16_t>(Unknown2);

  if (verbose_)
    std::cout << " to 0x" << std::hex << tellg() << std::dec << ")" << std::endl;


  // TerrainBorders seem to be unused (are empty) in GV > RoR
  serializeSub<TerrainBorder>(TerrainBorders, 16); //TODO: fixed size?

  switch (getGameVersion()) // Empty space.
  {
    case genie::GV_AoE:
    case genie::GV_RoR:  serialize<int32_t>(&ZeroSpace, 1); break;
    case genie::GV_AoKA: serialize<int32_t>(&ZeroSpace, 5); break;
    case genie::GV_AoK:
    case genie::GV_TC:
    case genie::GV_SWGB:
    case genie::GV_CC:   serialize<int32_t>(&ZeroSpace, 7); break;
    default: break;
  }

  serialize<uint16_t>(NumberOfTerrainsUsed2);

  if (verbose_)
    std::cout << "RenderingPlusSomething?? (0x" << std::hex << tellg() << std::dec;

  //serialize<int16_t>(&Rendering, 19);
  switch (getGameVersion())
  {
    case genie::GV_AoE:
    case genie::GV_RoR:  serialize<int16_t>(&Rendering, 21); break;
    case genie::GV_AoKA:
    case genie::GV_AoK:
    case genie::GV_TC:   serialize<int16_t>(&Rendering, 29); break;
    case genie::GV_SWGB:
    case genie::GV_CC:   serialize<int16_t>(&Rendering, 31); break;
    default: break;
  }

  switch (getGameVersion()) // Few pointers and small numbers.
  {
    case genie::GV_AoE:
    case genie::GV_RoR:  serialize<int32_t>(&Something, 5); break;
    case genie::GV_AoKA: serialize<int32_t>(&Something, 6); break;
    case genie::GV_AoK:
    case genie::GV_TC:   serialize<int32_t>(&Something, 157); break;
    case genie::GV_SWGB:
    case genie::GV_CC:   serialize<int32_t>(&Something, 157); break;
    default: break;
  }

  // This data seems to be needed only in AoE and RoR.
  // In later games it is removable.
  // It exists in Star Wars games too, but is not used.
  serialize<ISerializable>(Unknown);

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

  if (getGameVersion() == genie::GV_AoKA)
  {
    serialize<int8_t>(&CivSkip, 2642677);
    //2.642.679
  }
  else
  serializeSub<Civ>(Civs, civ_count_);

  if (getGameVersion() >= genie::GV_SWGB)
    serialize<char>(SUnknown7);

  serializeSize<uint16_t>(research_count_, Researchs.size());

  if (verbose_)
    std::cout << "Researchcount: " << research_count_ << std::endl;

  serializeSub<Research>(Researchs, research_count_);

  if (verbose_)
    std::cout << "TechTrees (before eof) (0x" << std::hex << tellg();

  if (getGameVersion() >= genie::GV_SWGB)
    serialize<char>(SUnknown8);

  if(getGameVersion() == genie::GV_AoKA) // Temporarily don't read tech trees
  {
    compressor_.endCompression();
	return;
  }

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
  Terrains.clear();
  Unknown.Unknown1stBlocks.clear();
  Unknown.Unknown2ndBlocks.clear();
  Techages.clear();
  TechTree.TechTreeAges.clear();
  TechTree.BuildingConnections.clear();
  TechTree.UnitConnections.clear();
  TechTree.ResearchConnections.clear();
  UnitHeaders.clear();
  Civs.clear();
  Researchs.clear();
  UnitLines.clear();
  TerrainBorders.clear();
  UnknownPreTechTree.clear();

  delete [] file_version_;
  delete [] GraphicsRendering;
  delete [] ZeroSpace;
  delete [] Rendering;
  delete [] Something;
  delete [] CivSkip;

  file_version_ = 0;
  GraphicsRendering = 0;
  ZeroSpace = 0;
  Rendering = 0;
  Something = 0;
  CivSkip = 0;
}

}

