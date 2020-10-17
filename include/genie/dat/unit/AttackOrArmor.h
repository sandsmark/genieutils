/*
    geniedat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2016  Mikko "Tapsa" P

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

/// Stores information about the class and amount of an armor or attack
struct AttackOrArmor
{
public:
    enum Class {
        Unused = 0,
        Infantry = 1,
        TurtleShips = 2,
        BasePierce = 3,
        BaseMelee = 4,
        WarElephants = 5,
        Unused6 = 6,
        Unused7 = 7,
        Cavalry = 8,
        Unused9 = 9,
        Unused10 = 10,
        AllBuildingsExceptPort = 11,
        Unused12 = 12,
        StoneDefense = 13,
        FEPredatorAnimals = 14,
        Archers = 15,
        ShipsCamelsSaboteurs = 16,
        Rams = 17,
        Trees = 18,
        UniqueUnitsExceptTurtleShip = 19,
        SiegeWeapons = 20,
        StandardBuildings = 21,
        WallsGates = 22,
        FEGunpowderUnits = 23,
        Boars = 24,
        Monks = 25,
        Castle = 26,
        Spearmen = 27,
        CavalryArchers = 28,
        EagleWarriors = 29,
        HDCamels = 30,
    };

    int16_t Class;
    int16_t Amount;

    bool compareTo(const AttackOrArmor &other) const;
};

static_assert(sizeof(AttackOrArmor) == 4);

} // namespace unit
} // namespace genie

