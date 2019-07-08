/*
    genieutils - A library for reading and writing data files of genie
               engine games.
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

#ifndef GENIE_SCN_TRIGGER_H
#define GENIE_SCN_TRIGGER_H

#include "genie/file/ISerializable.h"

#include <stdint.h>

namespace genie {

struct MapPoint
{
    int32_t x, y;
};

class TriggerCondition : public ISerializable
{
public:
    enum Type {
        None, // 0
        BringObjectToArea, // 1
        BringObjectToObject, // 2
        OwnObjects, // 3
        OwnFewerObjects, // 4
        ObjectsInArea, // 5
        DestroyObject, // 6
        CaptureObject, // 7
        AccumulateAttribute, // 8
        ResearchTehcnology, // 9
        Timer, // 10
        ObjectSelected, // 11
        AISignal, // 12
        PlayerDefeated, // 13
        ObjectHasTarget, // 14
        ObjectVisible, //
        ObjectNotVisible,
        ResearchingTechnology,
        UnitsGarrisoned,
        DifficultyLevel,
        SWGB_OwnFewerFoundations,
        HD_Chance = SWGB_OwnFewerFoundations,
        SWGB_SelectedObjectsInArea,
        SWGB_PoweredObjectsInArea,
        SWGB_UnitsQueuedPastPopCap,

        ConditionsTypeCount
    };

    TriggerCondition();
    virtual ~TriggerCondition();

    int32_t type = 0;
    int32_t usedVariables = 0; //always = 0x10 (for internal use)
    int32_t amount = -1; //of objects, difficult level
    int32_t resource = -1; //resource type (see constants below)
    int32_t setObject = -1;
    int32_t nextObject = -1;
    int32_t object = -1;
    int32_t sourcePlayer = -1;
    int32_t technology = -1;
    int32_t timer = -1;
    int32_t trigger = -1;
    MapPoint areaFrom = { -1, -1 };
    MapPoint areaTo = { -1, -1 };
    int32_t objectGroup = -1;

    // same as Unit
    enum ObjectType {
        Undefined = -1,
        Other = 0,
        Base = 1,
        Building = 2,
        Civilian = 3,
        Military = 4,
        MonkNoRelic = 5,
    };
    int32_t objectType = -1; //Civilian, Military, Building, Other

    int32_t aiSignal = -1;

private:
    virtual void serializeObject(void);
};

class TriggerEffect : public ISerializable
{
public:
    enum Type {
        None = 0,
        ChangeDiplomacy = 1,
        ResearchTechnology = 2,
        SendChat = 3,
        Sound = 4,
        SendTribute = 5,
        UnlockGate = 6,
        LockGate = 7,
        ActivateTrigger = 8,
        DeactivateTrigger = 9,
        AIScriptGoal = 10,
        CreateObject = 11,
        TaskObject = 12,
        DeclareVictory = 13,
        KillObject = 14,
        RemoveObject = 15,
        ChangeView = 16,
        Unload = 17,
        ChangeOwnership = 18,
        Patrol = 19,
        DisplayInstructions = 20,
        ClearInstructions = 21,
        FreezeUnit = 22,
        UseAdvancedButtons = 23,
        DamageObject = 24,
        PlaceFoundation = 25,
        ChangeObjectName = 26,
        ChangeObjectHP = 27,
        ChangeObjectAttack = 28,
        StopUnit = 29,
        UserPatch_ChangeSpeed,
        SWGB_SnapView = UserPatch_ChangeSpeed,
        HD_AttackMove = UserPatch_ChangeSpeed,
        UserPatch_ChangeRange,
        SWGB_DisableAdvancedButtons = UserPatch_ChangeRange,
        HD_ChangeArmor = UserPatch_ChangeRange,
        UserPatch_ChangeMeleArmor,
        HD_ChangeRange = UserPatch_ChangeMeleArmor,
        SWGB_EnableTech = UserPatch_ChangeMeleArmor,
        UserPatch_ChangePiercingArmor,
        HD_ChangeSpeed = UserPatch_ChangePiercingArmor,
        SWGB_DisableTech = UserPatch_ChangePiercingArmor,
        SWGB_EnableUnit,
        HD_HealObject = SWGB_EnableUnit,
        SWGB_DisableUnit,
        HD_TeleportObject = SWGB_DisableUnit,
        SWGB_FlashUnit,
        HD_ChangeUnitStance = SWGB_FlashUnit,
        SWGBCC_InputOff,
        SWGBCC_InputOn,
    };


    TriggerEffect();
    virtual ~TriggerEffect();

    int32_t type = 0;
    int32_t usedVariables = 0;
    int32_t aiGoal = -1;
    int32_t amount = -1;
    int32_t resource = -1;
    int32_t diplomacy = -1;
    int32_t setObjects = -1;
    int32_t nextObject = -1;
    int32_t object = -1;
    int32_t sourcePlayer = -1;
    int32_t targetPlayer = -1;
    int32_t technology = -1;
    int32_t stringTableID = -1;
    int32_t soundResourceID = -1;
    int32_t timer = -1;
    int32_t trigger = -1;
    MapPoint location = { -1, -1 };
    MapPoint areaFrom = { -1, -1 }; //lower-left corner of area
    MapPoint areaTo = { -1, -1 }; //upper-right corner
    int32_t objectGroup = -1;
    int32_t objectType = -1; //Civilian, Military, Building, Other
    int32_t instructionPanel = -1;
    std::string message = ""; //Instructions/Chat
    std::string soundFile = "";
    std::vector<int32_t> selectedUnits;

private:
    virtual void serializeObject(void);
};

class Trigger : public ISerializable
{
public:
    Trigger();
    virtual ~Trigger();

    int32_t startingState;
    int8_t looping;
    int32_t stringTableID;
    int8_t isObjective;
    int32_t descriptionOrder;
    int32_t startingTime;
    std::string description;
    std::string name;
    std::vector<TriggerEffect> effects;
    std::vector<int32_t> effectDisplayOrder;
    std::vector<TriggerCondition> conditions;
    std::vector<int32_t> conditionDisplayOrder;

private:
    int32_t numEffects_;
    int32_t numConditions_;
    virtual void serializeObject(void);
};
}

#endif // GENIE_SCN_TRIGGER_H
