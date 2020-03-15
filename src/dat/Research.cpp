/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2019  Mikko "Tapsa" P

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

#include "genie/dat/Research.h"

#include "TestHelpers.h"

namespace genie {

//------------------------------------------------------------------------------
Tech::Tech()
{
}

void Tech::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    RequiredTechs.resize(getRequiredTechsSize(), -1);
}

//------------------------------------------------------------------------------
unsigned short Tech::getRequiredTechsSize()
{
    if (getGameVersion() >= GV_AoKA) {
        return 6;
    } else {
        return 4;
    }
}

bool Tech::compareTo(const Tech &other) const
{
    COMPARE_MEMBER(Civ);
    COMPARE_MEMBER(EffectID);
    COMPARE_MEMBER(FullTechMode);
    COMPARE_MEMBER(IconID);
    COMPARE_MEMBER(RequiredTechCount);
    COMPARE_MEMBER(ResearchLocation);
    COMPARE_MEMBER(ResearchTime);
    COMPARE_MEMBER(Type);
    COMPARE_MEMBER(HotKey);
    COMPARE_MEMBER(LanguageDLLHelp);
    COMPARE_MEMBER(LanguageDLLTechTree);
    COMPARE_MEMBER(ButtonID);
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER_VEC(Name2);
    COMPARE_MEMBER_OBJ_VEC(ResourceCosts);
    COMPARE_MEMBER_VEC(RequiredTechs);
    COMPARE_MEMBER(LanguageDLLDescription);
    COMPARE_MEMBER(LanguageDLLName);

    return true;
}

//------------------------------------------------------------------------------
void Tech::serializeObject(void)
{
    GameVersion gv = getGameVersion();

    serialize<int16_t>(RequiredTechs, getRequiredTechsSize());

    serialize<ResearchResourceCost, 3>(ResourceCosts);
    serialize<int16_t>(RequiredTechCount);

    if (gv >= GV_AoKB) {
        serialize<int16_t>(Civ); // 10.22
        serialize<int16_t>(FullTechMode); // 10.77
    }

    serialize<int16_t>(ResearchLocation);

    if (gv >= GV_MATT) {
        serialize<uint16_t>(LanguageDLLName);
        serialize<uint16_t>(LanguageDLLDescription);
    }

    serialize<int16_t>(ResearchTime);
    serialize<int16_t>(EffectID);
    serialize<int16_t>(Type);
    serialize<int16_t>(IconID);
    serialize<int8_t>(ButtonID);

    if (gv >= GV_AoEB) {
        serialize<int32_t>(LanguageDLLHelp);
        serialize<int32_t>(LanguageDLLTechTree);
        serialize<int32_t>(HotKey);
    }

    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        uint16_t name_len{};
        serializeSize<uint16_t>(name_len, Name);
        serialize(Name, name_len);

        if (gv >= GV_SWGB) {
            serializeSize<uint16_t>(name_len, Name2);
            serialize(Name2, name_len);
        }
    } else {
        serializeDebugString(Name);
    }
}
} // namespace genie
