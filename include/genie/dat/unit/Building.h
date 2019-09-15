/*
    geniedat - A library for reading and writing data files of genie
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

#pragma once

#include "genie/file/ISerializable.h"

namespace genie {

namespace unit {

    /// A building can hold annexes like the town center.
    class BuildingAnnex : public ISerializable
    {
    public:
        int16_t UnitID = -1;
        std::pair<float, float> Misplacement = { 0.f, 0.f };

    private:
        void serializeObject(void) override;
    };

    class Building : public ISerializable
    {
    public:
        int16_t ConstructionGraphicID = -1;
        int16_t SnowGraphicID = -1; //not in aok/aoe/ror

        /// Enabler for annex units.
        /// 0   Default
        /// 1   Adjacent buildings can change graphics of this unit
        /// This changes the graphic angle
        int8_t AdjacentMode = 0;

        /// Effect attribute 17 changes this
        /// 0   Default
        /// 1+ Use icon from 2nd age etc
        /// In AoE 1 can be used to set the unit
        /// have icon graphics of later ages straight in stone age
        int16_t GraphicsAngle = 0;

        /// Useful for stack unit placement
        /// 0   Default
        /// 1   Makes the building disappear when built
        int8_t DisappearsWhenBuilt = 0;

        /// These four (4) variables are used when building is created
        /// Second building to be placed directly on top of this building
        /// Additional building gets added every time you load the scenario
        int16_t StackUnitID = -1;

        /// Terrain set underneath when built
        int16_t FoundationTerrainID = -1;

        /// ES forgot to remove this when AoE was still in its alpha stages
        /// This points to removed data block that was like terrain borders.
        /// You could build roads back in 1996.
        int16_t OldOverlayID = -1;

        /// Causes that technology to be researched when the building is created
        int16_t TechID = -1;

        /// Unused in AoC?
        int8_t CanBurn = 0;

        static constexpr unsigned short BUILDING_ANNEXES_SIZE = 4;
        std::array<unit::BuildingAnnex, BUILDING_ANNEXES_SIZE> Annexes;

        /// The building that an annex building is attached to
        int16_t HeadUnit = -1;

        /// Determines what the unit changes into when given the order to unpack
        int16_t TransformUnit = -1;

        /// Sound when unpacking
        int16_t TransformSound = -1;

        int16_t ConstructionSound = -1;

        /// Bit field for what can be garrisoned
        /// You can garrison any type,
        /// if you add the garrison action targeting this class/unit,
        /// but you may need to hold the alt key while choosing to garrison.
        ///
        /// 0 Villager/Worker
        /// 1 Infantry
        /// 2 Cavalry/Mounted
        /// 3 Monk/Jedi
        /// 4 SW: Livestock
        int8_t GarrisonType = 0;

        /// Healing speed factor of units garrisoned in building
        float GarrisonHealRate = 0;

        float GarrisonRepairRate = 0;

        /// Appears when the building dies
        /// Does not appear with delete command
        int16_t PileUnit = -1;

        /// Related to annexes in any way?
        /// Widespread usage in the AoK alpha 09.07.0222
        static constexpr unsigned short LOOTABLE_RES_COUNT = 6;
        std::array<int8_t, LOOTABLE_RES_COUNT> LootingTable{};

    protected:
        void serializeObject(void) override;
    };
}
}
