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

namespace genie {

bool CombinedResources::playerInfo = false;

ScnMainPlayerData::~ScnMainPlayerData()
{
    delete []bitmap;
    delete bmpHeader;
}

void ScnMainPlayerData::serializeObject(void)
{
    serializePlayerDataVersion();

    if (scn_plr_data_ver > 1.13f) {
        for (std::string &playerName : playerNames) {
            serialize(playerName, 256); // 1.14 <-- this is read much later in AoE 1
        }

        if (scn_plr_data_ver > 1.15f) {
            serialize<uint32_t, 16>(playerNamesStringTable);
        }

        CombinedResources::playerInfo = true;
        serialize<CombinedResources, 16>(resourcesPlusPlayerInfo);
    }

    if (scn_plr_data_ver > 1.06f) {
        serialize<uint8_t>(conquestVictory);
    }

    serialize<ISerializable>(timeline);
    serializeSizedString<uint16_t>(originalFileName, false);

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

    if (scn_plr_data_ver > 1.1f) {
        serializeSizedString<uint16_t>(hints, false);
        serializeSizedString<uint16_t>(victory, false);
        serializeSizedString<uint16_t>(loss, false);
        serializeSizedString<uint16_t>(history, false);
    }

    if (scn_plr_data_ver > 1.21f) {
        serializeSizedString<uint16_t>(scouts, false);
    }

    if (scn_plr_data_ver < 1.03f) {
        serializeSizedString<uint16_t>(oldFilename1, false);
        serializeSizedString<uint16_t>(oldFilename2, false);
        serializeSizedString<uint16_t>(oldFilename3, false);
    }

    serializeSizedString<uint16_t>(pregameCinematicFilename, false);
    serializeSizedString<uint16_t>(victoryCinematicFilename, false);
    serializeSizedString<uint16_t>(lossCinematicFilename, false);

    if (scn_plr_data_ver > 1.08f) {
        serializeSizedString<uint16_t>(backgroundFilename, false);
    }

    if (scn_plr_data_ver > 1.0f) {
        serializeBitmap();
    }

    serializeSizedStrings<uint16_t, 16>(aiNames, false);
    serializeSizedStrings<uint16_t, 16>(cityNames, false);

    if (scn_plr_data_ver > 1.07f) {
        serializeSizedStrings<uint16_t, 16>(personalityNames, false);
    }

    serialize<AiFile, 16>(aiFiles);

    if (scn_plr_data_ver > 1.1f) {
        serialize<uint8_t, 16>(aiTypes);
    }

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);
    }

    // <- here actually switches the reading function in exe

    if (scn_plr_data_ver < 1.14f) {
        for (std::string &playerName : playerNames) {
            serialize(playerName, 256);
        }

        serialize<CombinedResources, 16>(resourcesPlusPlayerInfo);
    } else {
        CombinedResources::playerInfo = false;
        serialize<CombinedResources, 16>(resourcesPlusPlayerInfo);
    }

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);
    }

    serialize<ISerializable>(victoryConditions);
    serialize<ISerializable>(diplomacy);

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);
    }

    serialize<uint32_t>(alliedVictory, scn_plr_data_ver < 1.02f ? 16 * 16 : 16);

    if (scn_plr_data_ver > 1.22f) {
        serialize<uint32_t>(unused1);
    }

    if (scn_plr_data_ver > 1.03f) {
        serialize<ISerializable>(disables);
    }

    if (scn_plr_data_ver > 1.04f) {
        serialize<uint32_t>(unused1);
    }

    if (scn_plr_data_ver > 1.11f) {
        serialize<uint32_t>(unused2);
        serialize<uint32_t>(allTechs);
    }

    if (scn_plr_data_ver > 1.05f) {
        serialize<int32_t, 16>(startingAge);
    }

    if (scn_plr_data_ver > 1.01f) {
        serialize<uint32_t>(separator_);
    }

    if (scn_plr_data_ver > 1.18f) {
        serialize<int32_t>(player1CameraX);
        serialize<int32_t>(player1CameraY);
    }

    if (scn_plr_data_ver > 1.2f) {
        serialize<int32_t>(aiType);
    }

    if (scn_plr_data_ver > 1.23f) {
        serialize<uint8_t, 16>(aiTypes);
    }
}

void CombinedResources::serializeObject(void)
{
    if (playerInfo || scn_plr_data_ver < 1.14f) {
        serialize<uint32_t>(enabled);
    }

    if (!playerInfo || scn_plr_data_ver < 1.14f) {
        serialize<uint32_t>(gold);
        serialize<uint32_t>(wood);
        serialize<uint32_t>(food);
        serialize<uint32_t>(stone);
    }

    if (playerInfo || scn_plr_data_ver < 1.14f) {
        serialize<uint32_t>(isHuman);
        serialize<uint32_t>(civilizationID);
        serialize<uint32_t>(unknown1);
    }

    if (!playerInfo && scn_plr_data_ver > 1.16f) {
        serialize<uint32_t>(ore);
        serialize<uint32_t>(goods);

        if (scn_plr_data_ver > 1.23f) {
            serialize<uint32_t>(goods);
        }
    }
}

void Timeline::serializeObject(void)
{
    serializeSize<uint16_t>(entryCount, events.size());
    serialize<uint16_t>(availableId);
    serialize<float>(time);

    serialize(events, entryCount);
}

void TimelineEvent::serializeObject(void)
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

void ScnMainPlayerData::serializeBitmap(void)
{
    serialize<uint32_t>(bitmapIncluded);

    serialize<uint32_t>(bitmapWidth);
    serialize<uint32_t>(bitmapHeigth);
    serialize<int16_t>(hasBitmap);

    if (bitmapIncluded == 0) {
        return;
    }

    serialize<char>(&bmpHeader, 0x28);

    if (isOperation(OP_READ)) {
        bitmapByteSize = *reinterpret_cast<uint32_t *>(bmpHeader + 20);

        bitmap = new char[bitmapByteSize];

        for (unsigned int i = 0; i < 0x28; ++i) {
            bitmap[i] = bmpHeader[i];
        }

        char *bitmapStart = (bitmap + 0x28);

        serialize<char>(&bitmapStart, bitmapByteSize - 0x28);
    } else if (isOperation(OP_WRITE)) {
        serialize<char>(&bitmap, bitmapByteSize);
    }
}

void AiFile::serializeObject(void)
{
    serializeSize<uint32_t>(aiFilenameSize, aiFilename, true);
    serializeSize<uint32_t>(cityFileSize, cityFilename, true);

    if (scn_plr_data_ver > 1.07f) {
        serializeSize<uint32_t>(perFileSize, perFilename, true);
    }

    // crap in exe, says these are >= 1.15
    serialize(aiFilename, aiFilenameSize);
    serialize(cityFilename, cityFileSize);

    if (scn_plr_data_ver > 1.07f) {
        serialize(perFilename, perFileSize);
    }
}

void ScnVictory::serializeObject(void)
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

void ScnDiplomacy::serializeObject(void)
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

void ScnDisables::serializeObject(void)
{
    if (scn_plr_data_ver > 1.17f) {
        serialize<uint32_t, 16>(numDisabledTechs);
    }

    serialize<uint32_t, 16>(disabledTechs, scn_plr_data_ver < 1.04f ? 20 : scn_plr_data_ver < 1.3f ? 30 : 60);

    if (scn_plr_data_ver > 1.17f) {
        serialize<uint32_t, 16>(numDisabledUnits);
        serialize<uint32_t, 16>(disabledUnits, scn_plr_data_ver < 1.3f ? 30 : 60);
        serialize<uint32_t, 16>(numDisabledBuildings);
        serialize<uint32_t, 16>(disabledBuildings, scn_plr_data_ver < 1.3f ? 20 : 60);
    }
}

void ScnMorePlayerData::serializeObject(void)
{
    serializeForcedString<uint16_t>(playerName);
    serialize<float>(initCameraX);
    serialize<float>(initCameraY);
    serialize<int16_t>(initCameraX2);
    serialize<int16_t>(initCameraY2);
    serialize<uint8_t>(alliedVictory);
    serializeSize<uint16_t>(playerCount_, diplomacy1.size());
    if (playerCount_ > 100) {
        throw std::runtime_error("Too many players, corrupt file? " + std::to_string(playerCount_));
    }
    serialize<uint8_t>(diplomacy1, playerCount_);

    if (scn_internal_ver >= 1.08) {
        serialize<uint32_t>(diplomacy2, playerCount_);
    }

    serialize<uint32_t>(playerColor);

    // victory condition version
    if (scn_internal_ver >= 1.09) {
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
//    printf("conditions count: %d\n", victoryConditionsCount);

    serialize<uint8_t>(victory);
//    printf("victory: %d\n", victory);

    if (victoryConditionsCount > 100) {
        throw std::runtime_error("Invalid amount of victory conditions " + std::to_string(victoryConditionsCount));
    }
    serialize<ScnPlayerVictoryCondition>(victoryConditions, victoryConditionsCount);

    if (victoryConditionVersion < 1.0) {
        totalVictoryPoints = 0;
    } else {
        serialize<uint32_t>(pointConditionsCount);
        serialize<uint32_t>(totalVictoryPoints);
//        printf("total victory points: %u\n", totalVictoryPoints);
//        printf("victory points count: %u\n", victoryPointsCount);
    }

    // Might have fucked up the size above
    // Haven't found more scenario files to test with that have victory conditions here, that aren't also 2.0
    if (victoryConditionVersion > 1.0f) {
        serialize<int32_t>(unknown1);
        serialize<int32_t>(unknown2);
//        printf("unknown 1: %d\n", unknown1);
//        printf("unknown 2: %d\n", unknown2);
    }

    if (scn_internal_ver > 1.13f) {
        serialize<int32_t>(playerID);
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

//    serialize<uint32_t>(x0);
//    serialize<uint32_t>(y0);
//    serialize<uint32_t>(x1);
//    serialize<uint32_t>(y1);
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
