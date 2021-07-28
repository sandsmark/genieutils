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

bool ScnPlayerResources::verbose;
bool ScnPlayerUnits::verbose;
static bool s_verbose;
static bool s_veryVerbose;

void ScnPlayerResources::serializeObject()
{
    s_verbose = verbose;

    if (s_verbose) std::cout << " ---- " << std::endl;
    serialize<float>(food);
    if (s_verbose) std::cout << " - Food " << food << std::endl;
    serialize<float>(wood);
    if (s_verbose) std::cout << " - wood " << wood << std::endl;
    serialize<float>(gold);
    if (s_verbose) std::cout << " - gold " << gold << std::endl;
    serialize<float>(stone);
    if (s_verbose) std::cout << " - stone " << stone << std::endl;

    if (scn_internal_ver > 1.12f) {
        serialize<float>(ore);
        if (s_verbose) std::cout << " - ore " << ore << std::endl;

        // this seems wrong, 1.3 is way too high, is always true?
        if (scn_internal_ver < 1.15f) {
            serialize<float>(goods);
            if (s_verbose) std::cout << " - goods " << goods << std::endl;
        }
    }

    if (scn_internal_ver >= 1.14f) {
        serialize<float>(popLimit); // game forces range from 25 to 200, defaults to 75
        if (s_verbose) std::cout << " - poplimit " << popLimit << std::endl;
    }

    if (scn_internal_ver > 1.13f && (scn_internal_ver < 1.14f || (scn_internal_ver > 1.15f))) {
        serialize<uint32_t>(playerId);
        if (s_verbose) std::cout << " - player id " << playerId << std::endl;
    }
}

void ScnPlayerUnits::serializeObject()
{
    serializeSize<uint32_t>(unitCount_, units.size());
    if (s_verbose) std::cout << " - unit count " << unitCount_ << std::endl;
    if (unitCount_ > 20000) {
        throw std::runtime_error("too many units in campaign file (" + std::to_string(unitCount_) + "), corrupt or unsupported file");
    }
    serialize(units, unitCount_);
}

void ScnUnit::serializeObject()
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
    if (s_veryVerbose) std::cout << "positionX " << positionX << std::endl;
    serialize<float>(positionY);
    if (s_veryVerbose) std::cout << "positionY " << positionY << std::endl;
    serialize<float>(positionZ);
    if (s_veryVerbose) std::cout << "positionZ " << positionZ << std::endl;
    serialize<uint32_t>(spawnID);
    if (s_veryVerbose) std::cout << "spawnID " << spawnID << std::endl;
    serialize<uint16_t>(objectID);
    if (s_veryVerbose) std::cout << "objectID " << objectID << std::endl;

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
    if (s_veryVerbose) std::cout << "state " << int(state) << std::endl;
    serialize<float>(rotation);
    if (s_veryVerbose) std::cout << "rotation " << rotation << std::endl;

    if (scn_ver != "1.14" && scn_ver != "1.07" && scn_ver != "1.09") {
        serialize<uint16_t>(initAnimationFrame);
        if (s_veryVerbose) std::cout << "init anim frame " << initAnimationFrame << std::endl;
    }

    if (scn_ver != "1.07" && scn_ver != "1.09") {
        serialize<int32_t>(garrisonedInID);
        if (s_veryVerbose) std::cout << "garrisoned in id " << garrisonedInID << std::endl;

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
            if (s_veryVerbose) std::cout << "garrisoned in id " << garrisonedInID << std::endl;
        }
    }
}
} // namespace genie
