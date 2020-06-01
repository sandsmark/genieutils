/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015 - 2016  Mikko "Tapsa" P
    Copyright (C) 2015  JustTesting1234

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

#include "genie/script/scn/ScnResource.h"

#include <fstream>
#include <string>

namespace genie {

void ScnPlayerResources::serializeObject(void)
{
    serialize<float>(food);
    serialize<float>(wood);
    serialize<float>(gold);
    serialize<float>(stone);

    if (scn_internal_ver > 1.12f) {
        serialize<float>(ore);

        // this seems wrong, 1.3 is way too high, is always true?
        if (scn_internal_ver < 1.3f) {
            serialize<float>(goods);
        }
    }

    if (scn_internal_ver >= 1.14f) {
        serialize<float>(popLimit); // game forces range from 25 to 200, defaults to 75
    }

    if (scn_internal_ver > 1.13f) {
        serialize<uint32_t>(playerId);
    }
}

void ScnPlayerUnits::serializeObject(void)
{
    serializeSize<uint32_t>(unitCount_, units.size());
    if (unitCount_ > 10000) {
        throw std::runtime_error("too many units in campaign file (" + std::to_string(unitCount_) + "), corrupt or unsupported file");
    }
    serialize(units, unitCount_);
}

void ScnUnit::serializeObject(void)
{
    // The resource units need some displacement to get to the right position
    if (isOperation(OP_WRITE)) {
        if (objectID == 59 // forage bush
                || objectID == 66 // gold mine
                || objectID == 102 // stone mine
                ) {
            positionX -= 0.5;
            positionY -= 0.5;
        }
    }

    serialize<float>(positionX);
    serialize<float>(positionY);
    serialize<float>(positionZ);
    serialize<uint32_t>(spawnID);
    serialize<uint16_t>(objectID);

    if (isOperation(OP_READ)) {
        if (objectID == 59 // forage bush
                || objectID == 66 // gold mine
                || objectID == 102 // stone mine
                ) {
            positionX += 0.5;
            positionY += 0.5;
        }
    }

    // Some replacements, from danielpereira on aok.heavengames.com:
    //   IDs 768/770 should be kept disabled. They are used as a layer to
    //   enable the special Spanish Cannon Galleon bonuses
    //   If you check in the Technologies data in the .dat, you'll see
    //   that unit 768 is enabled for Spanish instead of the usual
    //   Cannon Galleon, and that the Elite Cannon Galleon technology
    //   upgrades this unit to ID 770.
    //   And, when I said that this unit is used as a layer,
    //   I mean that, when the game loads this unit,
    //   it replaces it with a Cannon Galleon and applies the Spanish bonuses
    // http://aok.heavengames.com/cgi-bin/forums/display.cgi?action=st&fn=4&tn=42049&st=96
//    if (objectID == 768) {
//        objectID = 420; // cannon galleon
//    }
//    if (objectID == 770) {
//        objectID = 691; // elite cannon galleon
//    }

    serialize<uint8_t>(state);
    serialize<float>(rotation);

    if (scn_ver != "1.14") {
        serialize<uint16_t>(initAnimationFrame);
    }

    serialize<int32_t>(garrisonedInID);

    if (!garrisonedInID && (
                scn_ver == "1.13" ||
                scn_ver == "1.14" ||
                scn_ver == "1.15" ||
                scn_ver == "1.16" ||
                scn_ver == "1.17" ||
                scn_ver == "1.18" ||
                scn_ver == "1.19" ||
                scn_ver == "1.20")) {
        garrisonedInID = -1;
    }
}
} // namespace genie
