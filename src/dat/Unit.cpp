/*
    genie/dat - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2011 - 2020  Mikko "Tapsa" P

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

#include "genie/dat/Unit.h"

#include "TestHelpers.h"

#include <unordered_map>

namespace genie {

//------------------------------------------------------------------------------
void Unit::setGameVersion(GameVersion gv)
{
    ISerializable::setGameVersion(gv);

    updateGameVersion(DamageGraphics);
    Moving.setGameVersion(gv);
    Action.setGameVersion(gv);
    Combat.setGameVersion(gv);
    Creatable.setGameVersion(gv);
    Building.setGameVersion(gv);
}

const std::string &Unit::className(const int16_t classId, const GameVersion gameVersion)
{
    if (gameVersion < GV_AoKA) {
        static const std::unordered_map<int16_t, std::string> oldNames = {
            { 18, "Priest" },
            { 23, "Chariot Archer" },
            { 35, "Chariot" },
            { 39, "Slinger" },

        };

        if (oldNames.find(classId) != oldNames.end()) {
            return oldNames.at(classId);
        }
    }

    static const std::unordered_map<int16_t, std::string> knownNames = {
        { 0, "Archer" },
        { 1, "Artifact" },
        { 2, "Trade Boat" },
        { 3, "Building" },
        { 4, "Civilian" },
        { 5, "Ocean Fish" },
        { 6, "Infantry" },
        { 7, "Berry Bush" },
        { 8, "Stone Mine" },
        { 9, "Prey Animal" },
        { 10, "Predator Animal" },
        { 11, "Miscellaneous" },
        { 12, "Cavalry" },
        { 13, "Siege Weapon" },
        { 14, "Terrain" },
        { 15, "Tree" },
        { 16, "Tree Stump" },
        { 17, "Healer" },
        { 18, "Monk" },
        { 19, "Trade Cart" },
        { 20, "Transport Boat" },
        { 21, "Fishing Boat" },
        { 22, "Warship" },
        { 23, "Conquistador" },
        { 24, "War Elephant" },
        { 25, "Hero" },
        { 26, "Elephant Archer" },
        { 27, "Wall" },
        { 28, "Phalanx" },
        { 29, "Domestic Animal" },
        { 30, "Flag" },
        { 31, "Deep Sea Fish" },
        { 32, "Gold Mine" },
        { 33, "Shore Fish" },
        { 34, "Cliff" },
        { 35, "Petard" },
        { 36, "Cavalry Archer" },
        { 37, "Doppelganger" },
        { 38, "Bird" },
        { 39, "Gate" },
        { 40, "Salvage Pile" },
        { 41, "Resource Pile" },
        { 42, "Relic" },
        { 43, "Monk with Relic" },
        { 44, "Hand Cannoneer" },
        { 45, "Two Handed Swordsman" },
        { 46, "Pikeman" },
        { 47, "Scout" },
        { 48, "Ore Mine" },
        { 49, "Farm" },
        { 50, "Spearman" },
        { 51, "Packed Unit" },
        { 52, "Tower" },
        { 53, "Boarding Boat" },
        { 54, "Unpacked Siege Unit" },
        { 55, "Ballista" },
        { 56, "Raider" },
        { 57, "Cavalry Raider" },
        { 58, "Livestock" },
        { 59, "King" },
        { 60, "Misc Building" },
        { 61, "Controlled Animal" },
    };

    if (knownNames.find(classId) == knownNames.end()) {
        static const std::string unknownString("Unknown class");
        return unknownString;
    }

    return knownNames.at(classId);
}

bool Unit::compareTo(const Unit &other) const
{
    COMPARE_MEMBER_XYF(ClearanceSize);
    COMPARE_MEMBER_XYF(OutlineSize);
    COMPARE_MEMBER_XYZF(Size);
    COMPARE_MEMBER(HPBarHeight);
    COMPARE_MEMBER(LineOfSight);
    COMPARE_MEMBER(MultipleAttributeMode);
    COMPARE_MEMBER(ResourceDecay);
    COMPARE_MEMBER(Speed);
    COMPARE_MEMBER(BaseID);
    COMPARE_MEMBER(BloodUnitID);
    COMPARE_MEMBER(Class);
    COMPARE_MEMBER(CopyID);
    COMPARE_MEMBER(DamageSound);
    COMPARE_MEMBER(DeadUnitID);
    COMPARE_MEMBER(DyingGraphic);
    COMPARE_MEMBER(DyingSound);
    COMPARE_MEMBER(HitPoints);
    COMPARE_MEMBER(ID);
    COMPARE_MEMBER(IconID);
    COMPARE_MEMBER(Nothing);
    COMPARE_MEMBER(OldPortraitPict);
    COMPARE_MEMBER(ResourceCapacity);
    COMPARE_MEMBER(SelectionSound);
    COMPARE_MEMBER(TelemetryID);
    COMPARE_MEMBER(TerrainRestriction);
    COMPARE_MEMBER(TrainSound);
    COMPARE_MEMBER(UndeadGraphic);
    COMPARE_MEMBER(Unitline);
    COMPARE_MEMBER(HotKey);
    COMPARE_MEMBER(LanguageDLLHelp);
    COMPARE_MEMBER(LanguageDLLHotKeyText);
    COMPARE_MEMBER(BlastDefenseLevel);
    COMPARE_MEMBER(CanBeBuiltOn);
    COMPARE_MEMBER(CanBeGathered);
    COMPARE_MEMBER(Civilization);
    COMPARE_MEMBER(CombatLevel);
    COMPARE_MEMBER(ConvertTerrain);
    COMPARE_MEMBER(CreateDoppelgangerOnDeath);
    COMPARE_MEMBER(Disabled);
    COMPARE_MEMBER(Enabled);
    COMPARE_MEMBER(FlyMode);
    COMPARE_MEMBER(FogVisibility);
    COMPARE_MEMBER(GarrisonCapacity);
    COMPARE_MEMBER(HideInEditor);
    COMPARE_MEMBER(HillMode);
    COMPARE_MEMBER(InteractionMode);
    COMPARE_MEMBER(InterfaceKind);
    COMPARE_MEMBER(MinTechLevel);
    COMPARE_MEMBER(MinimapMode);
    COMPARE_MEMBER(ObstructionClass);
    COMPARE_MEMBER(ObstructionType);
    COMPARE_MEMBER(OldAttackReaction);
    COMPARE_MEMBER(Recyclable);
    COMPARE_MEMBER(ResourceGatherGroup);
    COMPARE_MEMBER(SelectionEffect);
    COMPARE_MEMBER(SortNumber);
    COMPARE_MEMBER(Type);
    COMPARE_MEMBER(UndeadMode);
    COMPARE_MEMBER_PAIR(PlacementSideTerrain);
    COMPARE_MEMBER_PAIR(PlacementTerrain);
    COMPARE_MEMBER_PAIR(StandingGraphic);
    COMPARE_MEMBER(Name);
    COMPARE_MEMBER(Name2);
    COMPARE_MEMBER_OBJ_VEC(ResourceStorages);
    COMPARE_MEMBER_OBJ_VEC(DamageGraphics);
    COMPARE_MEMBER(LanguageDLLCreation);
    COMPARE_MEMBER(LanguageDLLName);
    COMPARE_MEMBER(WwiseDamageSoundID);
    COMPARE_MEMBER(WwiseDyingSoundID);
    COMPARE_MEMBER(WwiseSelectionSoundID);
    COMPARE_MEMBER(WwiseTrainSoundID);
    COMPARE_MEMBER(EditorSelectionColour);
    COMPARE_MEMBER(MinimapColor);
    COMPARE_MEMBER(OcclusionMode);
    COMPARE_MEMBER(Trait);


    return Moving.compareTo(other.Moving) &&
            Action.compareTo(other.Action) &&
            Combat.compareTo(other.Combat) &&
            Missile.compareTo(other.Missile) &&
            Creatable.compareTo(other.Creatable) &&
            Building.compareTo(other.Building);
}

//------------------------------------------------------------------------------
void Unit::serializeObject()
{
    GameVersion gv = getGameVersion();

    //Type 10+
    if (gv < GV_AoEB && isOperation(OP_WRITE)) {
        Type = UnitType(Type / 10);
    }

    serialize<int8_t>(Type); // 7 = 70 in AoE alphas etc

    if (gv < GV_AoEB && isOperation(OP_READ)) {
        Type = UnitType(Type * 10);
    }

    uint16_t name_len{};

    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        serializeSize<uint16_t>(name_len, Name);
    }

    serialize<int16_t>(ID);

    if (gv <= GV_LatestDE2 && gv >= GV_C15) {
        serialize<int32_t>(LanguageDLLName);
        serialize<int32_t>(LanguageDLLCreation);
    } else {
        uint16_t data = LanguageDLLName;
        serialize<uint16_t>(data);
        LanguageDLLName = data;
        if (gv >= GV_MATT) {
            data = LanguageDLLCreation;
            serialize<uint16_t>(data);
            LanguageDLLCreation = data;
        }
    }

    serialize<int16_t>(Class);
    serializePair<int16_t>(StandingGraphic, (gv >= GV_AoKB) ? false : true);
    serialize<int16_t>(DyingGraphic);
    serialize<int16_t>(UndeadGraphic);
    serialize<int8_t>(UndeadMode);
    serialize<int16_t>(HitPoints);
    serialize<float>(LineOfSight);
    serialize<int8_t>(GarrisonCapacity);
    serialize<float>(Size.x);
    serialize<float>(Size.y);
    serialize<float>(Size.z);
    serialize<int16_t>(TrainSound);

    if (gv >= GV_AoKE3) {
        serialize<int16_t>(DamageSound);
    }

    serialize<int16_t>(DeadUnitID);

    if ((gv >= GV_T6 && gv <= GV_LatestTap) || (gv >= GV_C7 && gv <= GV_LatestDE2)) {
        serialize<int16_t>(BloodUnitID);
    }

    serialize<int8_t>(SortNumber);
    serialize<int8_t>(CanBeBuiltOn);
    serialize<int16_t>(IconID);
    serialize<int8_t>(HideInEditor);
    serialize<int16_t>(OldPortraitPict);
    serialize<int8_t>(Enabled);

    if (gv >= GV_AoK) { // 11.48
        serialize<int8_t>(Disabled);
    }

    if (gv >= GV_MIK) {
        serializePair<int16_t>(PlacementSideTerrain);
    }

    serializePair<int16_t>(PlacementTerrain); // Before AoE, this also contains side terrain.
    serialize<float>(ClearanceSize.x);
    serialize<float>(ClearanceSize.y);
    serialize<int8_t>(HillMode);
    serialize<int8_t>(FogVisibility);
    serialize<int16_t>(TerrainRestriction);
    serialize<int8_t>(FlyMode);
    serialize<int16_t>(ResourceCapacity);
    serialize<float>(ResourceDecay);
    serialize<int8_t>(BlastDefenseLevel);
    serialize<int8_t>(CombatLevel);
    serialize<int8_t>(InteractionMode);
    serialize<int8_t>(MinimapMode);
    serialize<int8_t>(InterfaceKind);
    serialize<float>(MultipleAttributeMode);
    serialize<uint8_t>(MinimapColor);

    if (gv >= GV_AoEB) { // 7.04
        serialize<int32_t>(LanguageDLLHelp);
        serialize<int32_t>(LanguageDLLHotKeyText);
        serialize<int32_t>(HotKey);
        serialize<int8_t>(Recyclable);
        serialize<int8_t>(CanBeGathered);
        serialize<int8_t>(CreateDoppelgangerOnDeath);
        serialize<int8_t>(ResourceGatherGroup);

        if (gv >= GV_AoKE3) { // 9.15
            serialize<uint8_t>(OcclusionMode);

            if (gv >= GV_AoKA) { // 9.53
                serialize<int8_t>(ObstructionType);
                serialize<int8_t>(ObstructionClass); // 9.56

                if (gv >= GV_TC) { // 11.55
                    serialize<uint8_t>(Trait);
                    serialize<int8_t>(Civilization);
                    serialize<int16_t>(Nothing);
                }
            }
        } else if (gv >= GV_T7 && gv <= GV_LatestTap) {
            serialize<int8_t>(ObstructionType);
            serialize<int8_t>(ObstructionClass);
        }


        serialize<int8_t>(SelectionEffect);
        serialize<uint8_t>(EditorSelectionColour);
        serialize<float>(OutlineSize.x);
        serialize<float>(OutlineSize.y);
        serialize<float>(HPBarHeight);

        if (gv >= GV_CK && gv <= GV_LatestDE2) {
            // This data is for scenario triggers.
            uint32_t data = -2;
            serialize<uint32_t>(data);
            data = 0;
            serialize<uint32_t>(data);
        }
    }

    serialize<ResourceStorage, 3>(ResourceStorages);

    uint8_t damage_graphic_count{};
    serializeSize<uint8_t>(damage_graphic_count, DamageGraphics.size());
    serialize(DamageGraphics, damage_graphic_count);

    serialize<int16_t>(SelectionSound);
    serialize<int16_t>(DyingSound);
    if (gv >= GV_C4 && gv <= GV_LatestDE2) {
        serialize<uint32_t>(WwiseTrainSoundID);
        serialize<uint32_t>(WwiseDamageSoundID);
        serialize<uint32_t>(WwiseSelectionSoundID);
        serialize<uint32_t>(WwiseDyingSoundID);
    }
    serialize<int8_t>(OldAttackReaction);
    serialize<int8_t>(ConvertTerrain);

    if ((gv > GV_LatestTap && gv < GV_C2) || gv < GV_Tapsa || gv > GV_LatestDE2) {
        serialize(Name, name_len);

        if (gv >= GV_SWGB) {
            serializeSize<uint16_t>(name_len, Name2);
            serialize(Name2, name_len);
            serialize<int16_t>(Unitline);
            serialize<int8_t>(MinTechLevel);
        }
    } else {
        serializeDebugString(Name);
    }

    if (gv >= GV_AoE) { // 7.12
        serialize<int16_t>(CopyID);
    }

    if (gv >= GV_AoKA) { // 9.49
        serialize<int16_t>(BaseID);
    }

    if (gv >= GV_T5 && gv <= GV_LatestTap) {
        serialize<int16_t>(TelemetryID);
    }


    if (Type == AoeTreeType) {
        return;
    }

    if (Type >= FlagType) {
        serialize<float>(Speed);
    } else {
        return;
    }

    if (Type >= MovingType) {
        serialize<ISerializable>(Moving);
    }

    if (Type >= ActionType) {
        serialize<ISerializable>(Action);
    }

    if (Type >= CombatantType) {
        serialize<ISerializable>(Combat);
    }

    if (Type == MissileType) {
        serialize(Missile);
    }

    if (Type >= CreatableType) {
        serialize<ISerializable>(Creatable);
    }

    if (Type == BuildingType) {
        serialize<ISerializable>(Building);
    }
}
} // namespace genie
