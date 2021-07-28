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

#include "genie/dat/unit/Building.h"

#include "../TestHelpers.h"

namespace genie {

namespace unit {
bool Building::compareTo(const Building &other) const
{
    COMPARE_MEMBER(GarrisonHealRate);
    COMPARE_MEMBER(GarrisonRepairRate);
    COMPARE_MEMBER(ConstructionGraphicID);
    COMPARE_MEMBER(ConstructionSound);
    COMPARE_MEMBER(DestructionGraphicID);
    COMPARE_MEMBER(DestructionRubbleGraphicID);
    COMPARE_MEMBER(FoundationTerrainID);
    COMPARE_MEMBER(GraphicsAngle);
    COMPARE_MEMBER(HeadUnit);
    COMPARE_MEMBER(OldOverlayID);
    COMPARE_MEMBER(PileUnit);
    COMPARE_MEMBER(ResearchCompletedGraphic);
    COMPARE_MEMBER(ResearchingGraphic);
    COMPARE_MEMBER(SnowGraphicID);
    COMPARE_MEMBER(StackUnitID);
    COMPARE_MEMBER(TechID);
    COMPARE_MEMBER(TransformSound);
    COMPARE_MEMBER(TransformUnit);
    COMPARE_MEMBER(AdjacentMode);
    COMPARE_MEMBER(CanBurn);
    COMPARE_MEMBER(DisappearsWhenBuilt);
    COMPARE_MEMBER(GarrisonType);
    COMPARE_MEMBER_VEC(LootingTable);;
    COMPARE_MEMBER_OBJ_VEC(Annexes);
    COMPARE_MEMBER(WwiseConstructionSoundID);
    COMPARE_MEMBER(WwiseTransformSoundID);

    return true;
}

void Building::serializeObject()
{
    GameVersion gv = getGameVersion();

    serialize<int16_t>(ConstructionGraphicID);

    if (gv >= GV_TC) { // 11.53
        serialize<int16_t>(SnowGraphicID);

        if (gv <= GV_LatestDE2 && gv >= GV_C10) {
            serialize<int16_t>(DestructionGraphicID);

            if (gv >= GV_C12) {
                serialize<int16_t>(DestructionRubbleGraphicID);

                if (gv >= GV_C13) {
                    serialize<int16_t>(ResearchingGraphic);
                    serialize<int16_t>(ResearchCompletedGraphic);
                }
            }
        }
    }

    serialize<int8_t>(AdjacentMode);
    serialize<int16_t>(GraphicsAngle);
    serialize<int8_t>(DisappearsWhenBuilt);
    serialize<int16_t>(StackUnitID);
    serialize<int16_t>(FoundationTerrainID);
    serialize<int16_t>(OldOverlayID); // No longer used
    serialize<int16_t>(TechID);

    if (gv >= GV_AoKE3) {
        serialize<int8_t>(CanBurn);
        serialize<unit::BuildingAnnex, BUILDING_ANNEXES_SIZE>(Annexes); // 40 bytes

        if (gv >= GV_AoKA) {
            serialize<int16_t>(HeadUnit); // 9.89
        }

        serialize<int16_t>(TransformUnit);
        serialize<int16_t>(TransformSound);
    }

    serialize<int16_t>(ConstructionSound);

    if (gv >= GV_AoKE3) {
        if (gv >= GV_C4 && gv <= GV_LatestDE2) {
            serialize<uint32_t>(WwiseTransformSoundID);
            serialize<uint32_t>(WwiseConstructionSoundID);
        }

        serialize<int8_t>(GarrisonType);
        serialize<float>(GarrisonHealRate);
        serialize<float>(GarrisonRepairRate);
        {
            serialize<int16_t>(PileUnit); // 9.06
            // 9.06 - 9.25 -> 5 x 2 x int16_t
            serialize<int8_t, LOOTABLE_RES_COUNT>(LootingTable); // 9.26
        }
    }
}

bool BuildingAnnex::compareTo(const BuildingAnnex &other) const
{
    COMPARE_MEMBER(UnitID);
    COMPARE_MEMBER_PAIR(Misplacement);

    return true;
}

void BuildingAnnex::serializeObject()
{
    serialize<int16_t>(UnitID);
    serializePair<float>(Misplacement);
}

} // namespace unit
} // namespace genie
