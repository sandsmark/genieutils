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

#include "genie/script/scn/ScnPlayerData.h"
#include "genie/script/ScnFile.h"
#include "genie/util/Utility.h"

#include <string>

namespace genie {

bool CombinedResources::playerInfo = false;

static bool s_verbose = false;

ScnMainPlayerData::~ScnMainPlayerData()
{
    delete []bitmap;
}

void ScnMainPlayerData::serializeObject()
{
    s_verbose = verbose;

    serializePlayerDataVersion();

    if (scn_plr_data_ver > 1.13f) {
        int num = 0;
        for (std::string &playerName : playerNames) {
            serialize(playerName, 256); // 1.14 <-- this is read much later in AoE 1
            if (s_verbose) std::cout << "Player " << (num++) << " name " << playerName << std::endl;
        }

        if (scn_plr_data_ver > 1.15f) {
            serialize<uint32_t, 16>(playerNamesStringTable);

            if (s_verbose) std::cout << "Read name string table" << std::endl;
        }

        CombinedResources::playerInfo = true;
        serialize<CombinedResources, 16>(resourcesPlusPlayerInfo);
    }

    if (scn_plr_data_ver > 1.06f) {
        serialize<uint8_t>(conquestVictory);
        if (s_verbose) std::cout << "Conquest victory " << conquestVictory << std::endl;
    }

    serialize<Timeline>(timeline);
    serializeSizedString<uint16_t>(originalFileName, false);
    if (s_verbose) std::cout << "Original filename " << originalFileName << std::endl;

    if (scn_plr_data_ver > 1.15f) {
        serialize<uint32_t>(instructionsStringTable);
        serialize<uint32_t>(hintsStringTable);
        serialize<uint32_t>(victoryStringTable);
        serialize<uint32_t>(lossStringTable);
        serialize<uint32_t>(historyStringTable);
    }

    if (scn_plr_data_ver > 1.21f) {
        serialize<uint32_t>(scoutsStringTable);
    }

    serializeSizedString<uint16_t>(instructions, false);
    if (s_verbose) std::cout << "Player instructions: " << instructions << std::endl;

    if (scn_plr_data_ver > 1.1f) {
        serializeSizedString<uint16_t>(hints, false);
        serializeSizedString<uint16_t>(victory, false);
        serializeSizedString<uint16_t>(loss, false);
        serializeSizedString<uint16_t>(history, false);
    }

    if (scn_plr_data_ver > 1.21f) {
        serializeSizedString<uint16_t>(scouts, false);
        if (s_verbose) std::cout << "Scouts: " << scouts << std::endl;
    }

    if (scn_plr_data_ver < 1.03f) {
        serializeSizedString<uint16_t>(oldFilename1, false);
        if (s_verbose) std::cout << "Old filename 1: " << oldFilename1 << std::endl;
        serializeSizedString<uint16_t>(oldFilename2, false);
        if (s_verbose) std::cout << "Old filename 2: " << oldFilename2 << std::endl;
        serializeSizedString<uint16_t>(oldFilename3, false);
        if (s_verbose) std::cout << "Old filename 3: " << oldFilename3 << std::endl;
    }

    serializeSizedString<uint16_t>(pregameCinematicFilename, false);
    if (s_verbose) std::cout << "Pregame cinematic " << pregameCinematicFilename << std::endl;
    serializeSizedString<uint16_t>(victoryCinematicFilename, false);
    if (s_verbose) std::cout << "Victory cinematic " << victoryCinematicFilename << std::endl;
    serializeSizedString<uint16_t>(lossCinematicFilename, false);
    if (s_verbose) std::cout << "Loss cinematic " << lossCinematicFilename << std::endl;

    if (scn_plr_data_ver > 1.08f) {
        serializeSizedString<uint16_t>(backgroundFilename, false);
        if (s_verbose) std::cout << "Background filename " << backgroundFilename << std::endl;
    }

    if (scn_plr_data_ver > 1.07f) {
        serializeBitmap();
    }

    serializeSizedStrings<uint16_t, 16>(aiNames, false);
    if (s_verbose) std::cout << "Loaded ai names " << util::sanitizeAscii(aiNames[0]) << " [...] " << util::sanitizeAscii(aiNames[15]) << std::endl;
    serializeSizedStrings<uint16_t, 16>(cityNames, false);
    if (s_verbose) std::cout << "Loaded city names " << cityNames[0] << " [...] " << cityNames[15] << std::endl;

    if (scn_plr_data_ver > 1.07f) {
        serializeSizedStrings<uint16_t, 16>(personalityNames, false);
        if (s_verbose) std::cout << "Loaded personality names " << personalityNames[0] << " [...] " << personalityNames[15] << std::endl;
    }

    serialize<AiFile, 16>(aiFiles);

    if (scn_plr_data_ver > 1.1f && scn_plr_data_ver != 1.14f) {
        serialize<uint8_t, 16>(aiTypes);
    }

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);
        if (separator_ != expectedSeparator) {
            throw std::runtime_error("Invalid separator before player resources " + std::to_string(separator_));
        }
        if (s_verbose) std::cout << "Separator " << separator_ << std::endl;
    }

    // <- here actually switches the reading function in exe

    if (scn_plr_data_ver < 1.14f) {
        for (std::string &playerName : playerNames) {
            serialize(playerName, 256);
            if (s_verbose) std::cout << "Player name old '" << util::sanitizeAscii(playerName) << "'" << std::endl;
        }

        serialize<CombinedResources, 16>(resourcesPlusPlayerInfo);
    } else {
        CombinedResources::playerInfo = false;
        serialize<CombinedResources, 16>(resourcesPlusPlayerInfo);
        if (s_verbose) std::cout << "Read resources and player info " << std::endl;
    }

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);
        if (separator_ != expectedSeparator) {
            throw std::runtime_error("Invalid separator before victory conditions " + std::to_string(separator_));
        }
    }

    serialize<ISerializable>(victoryConditions);
    serialize<ISerializable>(diplomacy);

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);

        if (separator_ != expectedSeparator) {
            throw std::runtime_error("Invalid separator before allied victories " + std::to_string(separator_));
        }
    }

    if (scn_plr_data_ver < 1.02f) {
        serialize<uint32_t>(alliedVictory, 16 * 16);
    } else {
        serialize<uint32_t>(alliedVictory, 16);
        if (s_verbose) {
            for (const auto s : alliedVictory) {
                std::cout << "Allied victory: " << s << std::endl;
            }
        }
    }

    if (scn_plr_data_ver > 1.22f) {
        serialize<uint32_t>(unused1);
        if (s_verbose) std::cout << "Unused 1 " << unused1 << std::endl;
    }

    if (scn_plr_data_ver > 1.03f) {
        serialize<ScnDisables>(disables);
    }

    if (scn_plr_data_ver > 1.04f) {
        serialize<uint32_t>(unused2);
        if (s_verbose) std::cout << "Unused 2 " << unused2 << std::endl;
    }

    if (scn_plr_data_ver > 1.11f) {
        serialize<uint32_t>(unused3);
        if (s_verbose) std::cout << "Unused 3 " << unused3 << std::endl;
        serialize<uint32_t>(allTechs);
        if (s_verbose) std::cout << "All techs " << allTechs << std::endl;
    }

    if (scn_plr_data_ver > 1.05f) {
        serialize<int32_t, 16>(startingAge);
        if (s_verbose) std::cout << "Starting age " << startingAge[0] << " [...] " << startingAge[15] << std::endl;
    }

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);

        if (separator_ != expectedSeparator) {
            throw std::runtime_error("Invalid separator before camera " + std::to_string(separator_));
        }
    }

    if (scn_plr_data_ver > 1.18f) {
        serialize<int32_t>(player1CameraX);
        serialize<int32_t>(player1CameraY);
        if (s_verbose) std::cout << "Player 1 camera x, y " << player1CameraX << "," << player1CameraY << std::endl;
    }

    if (scn_plr_data_ver > 1.2f) {
        serialize<int32_t>(aiType);
    }

    if (scn_plr_data_ver > 1.23f) {
        serialize<uint8_t, 16>(aiTypes);
    }
}

void CombinedResources::serializeObject()
{
    if (playerInfo || scn_plr_data_ver < 1.14f) {
        serialize<uint32_t>(enabled);
        if (s_verbose) std::cout << "Combined Resources enabled " << enabled << std::endl;
    }

    if (!playerInfo || scn_plr_data_ver < 1.14f) {
        serialize<uint32_t>(gold);
        if (s_verbose) std::cout << "Gold " << gold << std::endl;
        serialize<uint32_t>(wood);
        if (s_verbose) std::cout << "Wood " << wood << std::endl;
        serialize<uint32_t>(food);
        if (s_verbose) std::cout << "Food " << food << std::endl;
        serialize<uint32_t>(stone);
        if (s_verbose) std::cout << "Stone " << stone << std::endl;
    }

    if (playerInfo || scn_plr_data_ver < 1.14f) {
        serialize<uint32_t>(isHuman);
        if (s_verbose) std::cout << "Is human " << isHuman << std::endl;
        serialize<uint32_t>(civilizationID);
        if (s_verbose) std::cout << "Civ ID " << civilizationID << std::endl;
        serialize<uint32_t>(unknown1);
        if (s_verbose) std::cout << "Unknown " << unknown1 << std::endl;
    }

    if (!playerInfo && scn_plr_data_ver > 1.16f) {
        serialize<uint32_t>(ore);
        if (s_verbose) std::cout << "Ore " << ore << std::endl;
        serialize<uint32_t>(goods);
        if (s_verbose) std::cout << "Goods " << goods << std::endl;

        if (scn_plr_data_ver > 1.23f) {
            serialize<uint32_t>(goods);
            if (s_verbose) std::cout << "Goods again " << goods << std::endl;
        }
    }
}

void Timeline::serializeObject()
{
    serializeSize<uint16_t>(entryCount, events.size());
    if (s_verbose) std::cout << "Timeline events: " << entryCount << std::endl;
    serialize<uint16_t>(availableId);
    if (s_verbose) std::cout << "Available ID: " << availableId << std::endl;
    serialize<float>(time);
    if (s_verbose) std::cout << "Time: " << time << std::endl;

    serialize(events, entryCount);
    if (s_verbose) std::cout << "Timeline events: " << events.size() << std::endl;
}

void TimelineEvent::serializeObject()
{
    serialize<float>(timestamp);
    serialize<uint8_t>(command);
    serialize<uint16_t>(objectType);
    serialize<uint8_t>(playerId);

    serialize<float>(x);
    serialize<float>(y);
    serialize<float>(z);

    serialize<uint16_t>(task);

    serialize<uint16_t>(objectId);

    serialize<uint16_t>(targetId);
    serialize<uint16_t>(targetPlayerId);

    /*/ 48 bytes? Lots of data if count is over 0
    ReadData((HANDLE)_hScenFile, hUnknown, 4u); // int
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 4, 1u); // char
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 6, 2u); // float?
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 8, 1u); // char
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 12, 4u); // int
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 16, 4u); // int
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 20, 4u); // in
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 24, 2u); // int16
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 32, 2u); // int16
    ReadData((HANDLE)_hScenFile, &hScenFile,            2u); // int16
    ReadData((HANDLE)_hScenFile, (char *)hUnknown + 40, 2u); // int16
    ReadData((HANDLE)_hScenFile, &v15, 2u);*/                // int16
}

void ScnMainPlayerData::serializeBitmap()
{
    serialize<uint32_t>(bitmapIncluded);
    if (s_verbose) std::cout << "Bitmap included? " << bitmapIncluded << std::endl;
    serialize<uint32_t>(bitmapWidth);
    if (s_verbose) std::cout << "Bitmap width " << bitmapWidth << std::endl;
    serialize<uint32_t>(bitmapHeigth);
    if (s_verbose) std::cout << "Bitmap height " << bitmapHeigth << std::endl;
    serialize<int16_t>(hasBitmap);
    if (s_verbose) std::cout << "Has bitmap " << hasBitmap << std::endl;

    if (bitmapIncluded == 0) {
        return;
    }

    serialize<BitmapHeader>(bmpHeader);

    if (bmpHeader.headerSize != sizeof(BitmapHeader)) {
        throw std::runtime_error("Invalid bmp header size " + std::to_string(bmpHeader.headerSize));
    }
    if (bmpHeader.compression != BitmapHeader::Uncompressed) {
        throw std::runtime_error("Unsupported compression " + std::to_string(bmpHeader.compression));
    }
    if (bmpHeader.colorPlanes != 1) {
        throw std::runtime_error("Unsupported number of color planes (must be 1): " + std::to_string(bmpHeader.colorPlanes));
    }
    if (bmpHeader.rawSize != bitmapWidth * bitmapHeigth) {
        throw std::runtime_error("Unsupported bitmap format, byte count " + std::to_string(bmpHeader.rawSize) + ", width x height: " + std::to_string(bitmapWidth) + "x" + std::to_string(bitmapHeigth));
    }
    if (bmpHeader.width != bitmapWidth || bmpHeader.height != bitmapHeigth) {
        throw std::runtime_error("Invalid bitmap size, BMP header " + std::to_string(bmpHeader.width) + " x" + std::to_string(bmpHeader.height) + " vs " + std::to_string(bitmapWidth) + "x" + std::to_string(bitmapHeigth));
    }
    if (bmpHeader.rawSize < sizeof(BitmapHeader)) {
        throw std::runtime_error("Invalid bmp size, less than header " + std::to_string(bmpHeader.rawSize));
    }
    if (bmpHeader.paletteSize != std::pow(2, bmpHeader.bitsPerPixel)) {
        throw std::runtime_error("Invalid BMP palette size: " + std::to_string(bmpHeader.paletteSize) + " for " + std::to_string(bmpHeader.bitsPerPixel) + " bits per pixel");
    }

    if (s_verbose) {
        std::cout << "palette size: " << bmpHeader.paletteSize << std::endl;
        std::cout << "raw bitmap size: " << bmpHeader.rawSize << std::endl;
    }

    serialize<BitmapColor>(bitmapPalette, bmpHeader.paletteSize);
    serialize<char>(bitmapData, bmpHeader.rawSize);
}

void AiFile::serializeObject()
{
    serializeSize<uint32_t>(aiFilenameSize, aiFilename, true);
    if (s_verbose) std::cout << "AI Filename size " << aiFilenameSize << std::endl;
    serializeSize<uint32_t>(cityFileSize, cityFilename, true);
    if (s_verbose) std::cout << "city Filename " << cityFileSize << std::endl;

    if (scn_plr_data_ver > 1.07f) {
        serializeSize<uint32_t>(perFileSize, perFile, true);
        if (s_verbose) std::cout << "Personality file file size " << perFileSize << std::endl;
    }

    // crap in exe, says these are >= 1.15
    serialize(aiFilename, aiFilenameSize);
    if (s_verbose) std::cout << "AI filename: " << util::sanitizeAscii(aiFilename) << std::endl;
    serialize(cityFilename, cityFileSize);
    if (s_verbose) std::cout << "City filename: " << util::sanitizeAscii(cityFilename) << std::endl;

    if (scn_plr_data_ver > 1.07f) {
        serialize(perFile, perFileSize);
        if (s_verbose) std::cout << "Personality file size: " << perFileSize << std::endl;
    }
}

void ScnVictory::serializeObject()
{
    {
        serialize<uint32_t>(conquestRequired);
        serialize<uint32_t>(unused1);
        serialize<uint32_t>(numRelicsRequired);
        serialize<uint32_t>(unused2);
        serialize<uint32_t>(exploredPerCentRequired);
        serialize<uint32_t>(unused3);
    }
    serialize<uint32_t>(allConditionsRequired);

    if (scn_plr_data_ver > 1.12f) {
        serialize<uint32_t>(victoryMode);
        serialize<uint32_t>(scoreRequired);
        serialize<uint32_t>(timeForTimedGame);
    }
}

void ScnDiplomacy::serializeObject()
{
    serialize<uint32_t, 16, 16>(stances); // Diplomacy (16*16*4)
    serialize<uint32_t, 16, 180>(individualVictory); // Individual Victory (12*60)
    /* Individual victory conditions were eventually reformed into triggers.
    00 +12 Quantity
    01 +16 Resource
    02 +20 Set Object
    03 +24 Next Object
    04 +28 Object Constant
    05 +32 Source Player
    06 +36 Technology
    07 +40 Timer
    08 +44 Trigger
    09 +48 Area X (FROM)
    10 +52 Area Y (FROM)
    11 +56 Area X (TO)
    12 +60 Area Y (TO)
    13 +64 Object Group
    14 +68 Object Type
    15 +72 AI Signal
    */
}

void ScnDisables::serializeObject()
{
    if (scn_plr_data_ver > 1.17f) {
        serialize<uint32_t, 16>(numDisabledTechs);
        if (s_verbose) std::cout << "Disabled tech counts " << numDisabledTechs[0] << " [...] " << numDisabledTechs[15] << std::endl;
    }

    if (scn_plr_data_ver < 1.04f) {
        serialize<uint32_t, 16>(disabledTechs, 20);
        if (s_verbose) std::cout << "20 disabled tech " << disabledTechs[0][0] << " [...] " << disabledTechs[15][19] << std::endl;
    } else if (scn_plr_data_ver <= 1.14f) {
        // TODO: the total amount read here seems correct, but there's some
        // wrong offset here (they seemed to be grouped in groups of 5?)
        // So 20 * uint8_t for something, and then 1260 * uint8_t for something else, idk
        serialize<uint32_t, 16>(disabledTechs, 20);
        if (s_verbose) std::cout << "20 disabled tech " << disabledTechs[0][0] << " [...] " << disabledTechs[15][19] << std::endl;
    } else if (scn_plr_data_ver < 1.3f) {
        serialize<uint32_t, 16>(disabledTechs, 30);
        if (s_verbose) std::cout << "30 disabled tech " << disabledTechs[0][0] << " [...] " << disabledTechs[15][29] << std::endl;
    } else {
        serialize<uint32_t, 16>(disabledTechs, 60);
        if (s_verbose) std::cout << "Disabled tech " << disabledTechs[0][0] << " [...] " << disabledTechs[15][59] << std::endl;
    }

    if (scn_plr_data_ver > 1.17f) {
        serialize<uint32_t, 16>(numDisabledUnits);
        serialize<uint32_t, 16>(disabledUnits, scn_plr_data_ver < 1.3f ? 30 : 60);
        serialize<uint32_t, 16>(numDisabledBuildings);
        serialize<uint32_t, 16>(disabledBuildings, scn_plr_data_ver < 1.3f ? 20 : 60);
    }
}

void ScnMorePlayerData::serializeObject()
{
    serialize<uint16_t>(playerNameLength);
    if (scn_internal_ver == 1.07f) {
        if (s_verbose) std::cout << "player name length " << playerNameLength << std::endl;
        if (playerNameLength > 9) {
            serialize<uint8_t>(unknownAoEAlpha, playerNameLength - 9);
            if (s_verbose) std::cout << "unknown player name prefix " << unknownAoEAlpha.size() << std::endl;
        }
        serialize(playerName, 9);
    } else {
        serialize(playerName, playerNameLength);
    }

    if (s_verbose) std::cout << "player name " << util::sanitizeAscii(playerName) << std::endl;
    serialize<float>(initCameraX);
    if (s_verbose) std::cout << "init camera X " << initCameraX << std::endl;
    serialize<float>(initCameraY);
    if (s_verbose) std::cout << "init camera Y " << initCameraY << std::endl;
    serialize<int16_t>(initCameraX2);
    if (s_verbose) std::cout << "init camera X2 " << initCameraX2 << std::endl;
    serialize<int16_t>(initCameraY2);
    if (s_verbose) std::cout << "init camera Y2 " << initCameraY2 << std::endl;
    serialize<uint8_t>(alliedVictory);
    if (s_verbose) std::cout << "allied victory " << alliedVictory << std::endl;
    serializeSize<uint16_t>(playerCount_, diplomacy1.size());
    if (s_verbose) std::cout << "diplomacy player count " << playerCount_ << std::endl;
    if (playerCount_ > 100) {
        throw std::runtime_error("Too many players, corrupt file? " + std::to_string(playerCount_));
    }
    serialize<uint8_t>(diplomacy1, playerCount_);
    if (s_verbose) std::cout << "diplomacy 1 " << int(diplomacy1[0]) << " [...] " << int(diplomacy1[playerCount_ - 1]) << std::endl;

    if (scn_internal_ver >= 1.08f) {
        serialize<uint32_t>(diplomacy2, playerCount_);
        if (s_verbose) std::cout << "diplomacy 2 " << diplomacy2[0] << std::endl;
    }

    if (scn_internal_ver != 1.07f && scn_internal_ver != 1.09f) {
        serialize<uint32_t>(playerColor);
        if (s_verbose) std::cout << "player color " << playerColor << std::endl;
    }

    // victory condition version
    if (scn_internal_ver >= 1.09f) {
        serialize<float>(victoryConditionVersion);
        if (victoryConditionVersion <= 0) {
            throw std::runtime_error("victory conditions version invalid: " + std::to_string(victoryConditionsCount));
        }
    } else {
        victoryConditionVersion = 0;
    }

//    printf("victory condition version: %f\n", victoryConditionVersion);

    // I think this is some victory condition stuff
    serializeSize<uint32_t>(victoryConditionsCount, victoryConditions.size());
    if (s_verbose) printf("Victory conditions count: %u\n", victoryConditionsCount);

    serialize<uint8_t>(victory);
    if (s_verbose) printf("Victory: %d\n", victory);

    if (victoryConditionsCount > 100) {
        throw std::runtime_error("Invalid amount of victory conditions " + std::to_string(victoryConditionsCount));
    }
    serialize<ScnPlayerVictoryCondition>(victoryConditions, victoryConditionsCount);

    if (victoryConditionVersion < 1.0f) {
        totalVictoryPoints = 0;
    } else {
        serialize<uint32_t>(pointConditionsCount);
        if (s_verbose) printf("victory points count: %u\n", pointConditionsCount);
        serialize<uint32_t>(totalVictoryPoints);
        if (s_verbose) printf("total victory points: %u\n", totalVictoryPoints);
    }

    // Might have fucked up the size above
    // Haven't found more scenario files to test with that have victory conditions here, that aren't also 2.0
    if (victoryConditionVersion > 1.0f) {
        serialize<int32_t>(unknown1);
        if (s_verbose) std::cout << "Unknown1 " << unknown1 << std::endl;
        serialize<int32_t>(unknown2);
        if (s_verbose) std::cout << "Unknown2 " << unknown2 << std::endl;
//        printf("unknown 1: %d\n", unknown1);
//        printf("unknown 2: %d\n", unknown2);
    }

    if (scn_internal_ver > 1.13f && (scn_internal_ver < 1.14f || (scn_internal_ver > 1.15f))) {
        serialize<int32_t>(playerID);
        if (s_verbose) std::cout << "Player ID " << playerID << std::endl;
    }
}

void ScnPlayerVictoryCondition::serializeObject()
{
//    printf("\n---\n");
    serialize<uint8_t>(type);
//    printf("type: %d\n", type);
    serialize<int32_t>(objectType);
//    printf("object type: %d\n", objectType);
    serialize<int32_t>(targetPlayer);
//    printf("target player: %d\n", targetPlayer);

    serialize<float>(x0);
    serialize<float>(y0);
    serialize<float>(x1);
    serialize<float>(y1);

//    printf("%fx%f, %fx%f\n", x0, y0, x1, y1);
//    printf("%x %x, %x %x\n", x0, y0, x1, y1);

    serialize<int32_t>(number);
//    printf("number: %d\n", number);
    serialize<int32_t>(count);
//    printf("count: %d\n", count);
    serialize<int32_t>(object);
//    printf("object: %d\n", object);
    serialize<int32_t>(target);
//    printf("target: %d\n", target);

    serialize<uint8_t>(victoryGroup);
//    printf("victory group: %d\n", target);
    serialize<uint8_t>(allyFlag);
//    printf("ally: %d\n", allyFlag);
    serialize<uint8_t>(state);
//    printf("state: %d\n", state);
//    printf("\n---\n");
}

} // namespace genie
