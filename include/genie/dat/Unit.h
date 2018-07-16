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
#include "unit/DamageGraphic.h"
#include "UnitCommand.h"
#include "unit/AttackOrArmor.h"
#include "unit/Moving.h"
#include "unit/Action.h"
#include "unit/Combat.h"
#include "unit/Missile.h"
#include "unit/Creatable.h"
#include "unit/Building.h"
#include "ResourceUsage.h"

namespace genie {

//------------------------------------------------------------------------------
/// Stores properties of genie units.
//
class Unit : public ISerializable
{
public:
    Unit();
    virtual ~Unit();
    virtual void setGameVersion(GameVersion gv);

    /// Available unit types
    enum UnitType : uint8_t {
        /// Basic units like rubble and flares.
        EyeCandyType = 10,

        /// Trees, used to be 90?
        TreeType = 15,

        /// With Speed but mostly flags.
        FlagType = 20,

        /// Only one unit has this type. AOK, DOPL (id 243) same properties as UT_Flag
        DoppelgangerType = 25,

        /// Dead and fish units. It seems to be unused in SWGB as units just explode
        /// and do not leave carcasses.
        MovingType = 30,

        /// Only birds in aoe and ror are of this type.
        ActionType = 40,

        /// Shared class inherited by combat objects.
        CombatantType = 50,

        /// Projectiles
        MissileType = 60,

        /// Units that can be created or trained like Army, Villagers and Ships.
        CreatableType = 70,

        /// Buildings
        BuildingType = 80,

        /// Trees in aoe and ror are of this type
        AoeTreeType = 90
    };
    int8_t Type = 10;

    int16_t ID = -1;

    /// ID of the name string stored in the language.dll file
    /// Usual Unit File Pattern for The Conquerors
    /// Name: 5000-5999
    /// Creation: Name +1000
    /// Hotkey: Name +11000
    /// Help: Name +100000, in file Name +21000
    /// Hotkey Text: Name +150000, in file Name +10000
    /// Tech tree: Name +9000
    uint16_t LanguageDLLName = 5000;

    /// ID of the name string stored in the language.dll file
    uint16_t LanguageDLLCreation = 6000;

    /// Class that gives certain properties
    enum UnitClass : int16_t {
        InvalidClass = -1,

        // AoE1-ish
        Archer = 0,
        Artifact = 1,
        TradeBoat = 2,
        BuildingClass = 3,
        Civilian = 4,
        OceanFish = 5,
        Infantry = 6,
        BerryBush = 7,
        StoneMine = 8,
        PreyAnimal = 9,
        PredatorAnimal = 10,
        Miscellaneous = 11,
        Cavalry = 12,
        SiegeWeapon = 13,
        TerrainClass = 14,
        Tree = 15,
        TreeStump = 16,
        Healer = 17,
        Monk = 18,
        TradeCart = 19,
        TransportBoat = 20,
        FishingBoat = 21,
        Warship = 22,
        Conquistador = 23,
        WarElephant = 24,
        Hero = 25,
        ElephantArcher = 26,
        Wall = 27,
        Phalanx = 28,
        DomesticAnimal = 29,
        Flag = 30,
        DeepSeaFish = 31,
        GoldMine = 32,
        ShoreFish = 34,
        Cliff = 34,
        Petard = 35,
        CavalryArcher = 36,
        Doppelganger = 37,
        Bird = 38,

        // AoE2
        Gate = 39,
        SalvagePile = 40,
        ResourcePile = 41,
        Relic = 42,
        MonkWithRelic = 43,
        HandCannoneer = 44,
        TwoHandedSwordsman = 45,
        Pikeman = 46,
        Scout = 47,
        OreMine = 48,
        Farm = 49,
        Spearman = 50,
        PackedUnit = 51,
        Tower = 52,
        BoardingBoat = 53,
        UnpackedSiegeUnit = 54,
        Ballista = 55,
        Raider = 56,
        CavalryRaider = 57,
        Livestock = 58,
        King = 59,
        MiscBuilding = 60,
        ControlledAnimal = 61,
    };
    int16_t Class = InvalidClass;
    static const std::string &className(const int16_t classId, const GameVersion gameVersion = GV_TC);
    const std::string &className() const { return className(Class); }

    /// Default standing graphic
    /// Half of units in group use 1st,
    /// the rest use 2nd
    std::pair<int16_t, int16_t> StandingGraphic = { -1, -1 };

    /// Graphic shown while the units is dying.
    int16_t DyingGraphic = -1;

    /// Graphic to use if undead mode is enabled
    int16_t UndeadGraphic = -1;

    /// Enables undead state
    /// After 1st dying graphic:
    /// 0   Transform into dead unit
    /// 1   Show undead graphic
    int8_t UndeadMode = 0;

    /// Unit's hit points
    /// -1 Instantly dying unit
    int16_t HitPoints = 1;

    /// Default line of sight measured in tiles
    /// Maximum (effective) value is 20
    float LineOfSight = 2;

    /// Maximum number of units that can be garrisonned at the same time.
    int8_t GarrisonCapacity = 0;

    /// Collision detection area taken by the unit.
    /// No other unit can move into this area except flying units.
    /// Setting "can be built on" to 1 and height to 0 causes
    /// farms to be walkable in AoE/RoR.
    XYZF Size;

    /// Sound played when the unit is created
    int16_t TrainSound = -1;
    int16_t DamageSound = -1;

    /// ID of the unit to change to when this one dies.
    int16_t DeadUnitID = -1;

    int16_t BloodUnitID = -1;

    /// Units with low sort numbers are drawn last
    /// 0   Can be placed on top of other units in scenario editor
    /// 5   Cannot be placed on top of other units in scenario editor
    int8_t SortNumber = 0;

    /// 0   Default
    /// 1   Graphic stays at highest elevation until destination is reached
    /// 2+ Graphic is not affected by elevation
    int8_t CanBeBuiltOn = 0;

    /// ID of the icon shown at the bottom when selected or when building
    int16_t IconID = -1;

    /// If true, unit can not be placed in the editor
    int8_t HideInEditor = 0;

    /// TODO Unknown
    int16_t OldPortraitPict = -1; // Always -1

    /// 0   Requires a technology to be available
    /// 1   Available without a technology
    int8_t Enabled = 1;

    /// Not scanned but set to 0
    /// Can change during gameplay
    /// Mostly for different game modes and disables defined in scenarios
    /// 0   Default
    /// 1   Prevents enabling/disabling with a tech
    int8_t Disabled = 0;

    /// Required terrain on some side
    /// E. g. water for docks
    std::pair<int16_t, int16_t> PlacementSideTerrain = { -1, -1 };

    /// Required terrain underneath
    std::pair<int16_t, int16_t> PlacementTerrain = { -1, -1 };

    XYF ClearanceSize;

    /// Restrictions on placement on hills
    enum HillModes : int8_t {
        AnyHillPlacement = 0, ///< Gates, farms, walls, towers
        NoHillCornerPlacement = 1, ///< Can't be placed on hill corners
        NoHillPlacement = 2, ///< Only flat land; town center, port, trade workshop
        OneElevDiffHillRestriction = 3 ///< All hills, as long as there isn't more than 1 elevation difference
    };
    int8_t HillMode = 0;

    enum FogVisibilities : int8_t {
        /// Normal units
        InvisibleInFog = 0,

        /// Buildings
        VisibleInFog = 1,

        /// Visible in fog if alive
        AliveVisibleInFog = 2,

        /// Invertred
        OnlyVisibleInFog = 3,

        /// Replaced by a "doppelganger" unit when in fog (i. e. a dummy unit that is shown until area is visible again)
        DoppelgangerFogVisibility = 4
    };
    int8_t FogVisibility = 0;

    /// ID of terrain restrictions that are imposed on the unit.
    int16_t TerrainRestriction = 0;

    /// Controls graphic altitude when teleporting
    /// 0   Stay on ground
    /// 1   Graphics appear higher than the shadow
    int8_t FlyMode = 0;

    /// How much resources this unit is able to carry
    int16_t ResourceCapacity = 0;

    /// Can alter decay time of corpses\nSet to -1 for never decaying
    float ResourceDecay = 0;

    /// Receive blast damage from units that have lower or same blast attack level
    int8_t BlastDefenseLevel = 0;

    /// Mainly used in trigger conditions
    /// 0   None
    /// 1   Base
    /// 2   Building
    /// 3   Civilian
    /// 4   Soldier
    /// 5   Priest/Monk
    int8_t CombatLevel = 0;

    enum InteractionModes : int8_t {
        NoInteraction = 0, ///< Can't be interacted with
        ObjectInteraction = 1, ///< Can pick (be targeted?)
        ResourceInteraction = 2, ///< Can be selected, can't attack or move
        BuildingInteraction = 3, ///< Can be selected and attack, can't move
        UnitInteraction = 4 ///< Can be selected, attack and move
    };
    int8_t InteractionMode = 0;

    /// \brief Defines if and how a unit is shown on the minimap.
    /// Following values are known to work
    enum MinimapModes : int8_t {
        /// does not place a dot on the mini-map.
        /// Eye candy, resources, fish traps, OUTLWs, and farms have this value
        MinimapInvisible = 0,

        /// places a square dot on the min-map for the unit.
        /// When the unit is selected, the dot will turn white
        MinimapUnit = 1,

        /// places a diamond-shaped dot on the mini-map for the unit.
        /// When the unit is selected, the dot will turn white
        MinimapBuilding = 2,

        /// places a diamond-shaped dot on the mini-map for the unit.
        /// When the unit is selected, the dot remains the player's color.
        MinimapTerrain = 3,

        /// places a larger black spot on the minimap which does not follow the unit.
        /// Whenever you open the chat box (enter) and close it, the black spots will move to the unit on the minimap.
        /// No blinking occurs when the unit is attacked.
        /// Both allied and enemy units are visible in the black area on the minimap.
        /// CLF units have this value and a brown spot is placed instead.
        MinimapLargeTerrain = 4,

        /// does the same thing as “4”
        MinimapLargeTerrain2 = 5,

        /// does not place a dot on the minimap
        MinimapNoDot = 6,
        /// does not place a dot on the minimap, same as 6
        MinimapNoDot2 = 7,
        /// does not place a dot on the minimap, same as 6
        MinimapNoDot3 = 8,
        /// does not place a dot on the minimap, same as 6
        MinimapNoDot4 = 9,

        /// 10 does the same thing as “6”, no dot on minimap.
        /// Hawks, macaws, and flying dogs have this value.
        MinimapFlying = 10
    };
    int8_t MinimapMode = 0;

    /// Class and this together selects the buttons to show on the bottom left for this unit
    enum InterfaceKinds : int8_t {
        /// None
        NoInterface = 0,
        /// Resource
        ResourcesInterface = 1,
        /// Building (build page 1)
        BuildingsInterface = 2,
        /// Civilian
        CiviliansInterface = 3,
        /// Soldier
        SoldiersInterface = 4,
        /// Trade Unit
        TradeUnitsInterface = 5,
        /// Priest/monk without relic
        MonksInterface = 6,
        /// Transport Ship
        TransportShipInterface = 7,
        /// Relic / Priest with Relic
        MonkWithRelicInterface = 8,
        /// Fishing Boat
        FishingBoatInterface = 9,
        /// (A=2&8) Military Building (build page 2)
        MilitaryBuildingsInterface = 10,
        /// Shield Building (build page 3)");
        ShieldedBuildingsInterface = 11,

        UnknownInterface = 12
    };
    int8_t InterfaceKind = 0;

    float MultipleAttributeMode = 0;

    /// Minimap modes 3 and 4 allow this to work
    uint8_t MinimapColor = 0;

    /// 100000 + Language File Name
    /// This is the help text, stored in the language DLL
    int32_t LanguageDLLHelp = 105000;

    /// 150000 + Language File Name
    /// This seems to be used only in AoE (not RoR)
    /// This language line has other purposes in SWGB and CC
    int32_t LanguageDLLHotKeyText = 155000;

    /// 10000 + Language File Creation (usually)
    int32_t HotKey = 16000;

    /// Unselectable
    /// Not scanned but set to 1 for class 11
    /// Can change during gameplay
    int8_t Recyclable = 0;

    /// Track as Resource
    /// Allows automatic gathering and handles fog visibility
    int8_t EnableAutoGather = 0;

    /// Create doppelganger on death.
    /// 0   None
    /// 1   After death
    /// 2   When dying
    int8_t CreateDoppelgangerOnDeath = 0;

    /// Visible resource group
    /// Needs to be gatherable
    enum ResourceGatherGroups : int8_t {
        ResourceTree = 0,
        ResourceBerry = 1,
        ResourceFish = 2,
        ResourceStoneOre = 3,
        ResourceGold = 4
    };
    int8_t ResourceGatherGroup = 0;

    /// Combinable bit field
    /// 0 No outline/occlusion
    /// 1 Outline shown (bit 0)
    /// 2 Occludes others (bit 1)
    /// 4 Outline shown while constructing (bit 2)
    enum OcclusionTypes {
        NoOcclusion = 0,
        ShowOutline = 1 << 0,
        OccludeOthers = 1 << 1,
        OcclusionWhileConstructing = 1 << 1
    };
    int8_t OcclusionMode = 0;

    /// values for ObstructionType
    enum ObstructionTypes : int8_t {
        /// Farm, gate, dead bodies, town center
        /// Square outline and passable
        PassableObstruction = 0,

        /// Square outline and passable
        PassableObstruction2 = 1,

        /// Buildings, gold mine
        /// Solid square outline, has collision box
        BuildingObstruction = 2,

        /// Berserk, flag x
        /// Square outline, has collision box
        BerserkObstruction = 3,

        /// Is passable, but has no outline
        PassableNoOutlineObstruction = 4,

        /// Normal units
        /// Round outline, has collision box
        UnitObstruction = 5,

        ///< Mountain, matches selection mask
        /// Solid square outline, has collision box
        MountainObstruction = 10
    };
    int8_t ObstructionType = 0;

    /// 0 default, 1+ above
    enum ObstructionClasses : int8_t {
        DefaultObstructionClass = 0,
        ResourceObstrctionClass = 1,
        UnitObstructionClass = 2,
        BuildingObstructionClass = 3,
        WallObstructionClass = 4,
        GateObstructionClass = 5, ///< Allows movement through
        CliffObstructionClass = 6 ///< Blocks walling
    };
    int8_t ObstructionClass = 0;

    /// Bitmask defining traits
    enum Traits : uint8_t {
        GarrisonableTrait = 0x01, ///< Can be garrisoned
        NoFormationTrait = 0x02, ///< Ships

        /// Below are just for SW
        StealthTrait = 0x04,
        DetectorTrait = 0x08,
        MechanicalTrait = 0x10,
        BiologicalTrait = 0x20,
        SelfShieldingTrait = 0x40,
        InvisibleTrait = 0x80
    };
    uint8_t Trait = 0;

    /// Seems to be used only in SWGB/CC
    int8_t Civilization = 0;
    int16_t Nothing = 0;

    /// Note: this doe not work with all units
    /// These are from AGE, openage has a different understanding:
    ///   0: "NONE",
    ///   1: "HPBAR_ON_OUTLINE_DARK",  # permanent, editor only
    ///   2: "HPBAR_ON_OUTLINE_NORMAL",
    ///   3: "HPBAR_OFF_SELECTION_SHADOW",
    ///   4: "HPBAR_OFF_OUTLINE_NORMAL",
    ///   5: "HPBAR_ON_5",
    ///   6: "HPBAR_OFF_6",
    ///   7: "HPBAR_OFF_7",
    ///   8: "HPBAR_ON_8",
    ///   9: "HPBAR_ON_9",
    enum SelectionEffects : int8_t {
        /// HP Bar on, selection shadow (permanent darker outline in editor only, disappears in game)
        ShowHPHideOutline = 0,

        /// HP Bar on, normal outline
        ShowHPShowOutline = 1,

        /// HP Bar off, selection shadow
        HideHPHideOutline = 2,

        /// HP Bar off, normal outline
        HideHPShowOutline = 3
    };
    int8_t SelectionEffect = 1;

    uint8_t EditorSelectionColour = 52;
    XYF OutlineSize;

    /// Determines HP bar location
    /// Vertical half tile (elevation height?) distance from the top corner?
    float HPBarHeight = 0.f;

    typedef Resource<float, int8_t> ResourceStorage;

    /// Resource cost of a unit
    std::vector<ResourceStorage> ResourceStorages;

    ///
    std::vector<unit::DamageGraphic> DamageGraphics;

    /// Sound that is played when this unit is selected
    int16_t SelectionSound = -1;
    int16_t DyingSound = -1;

    /// 0   None
    /// 1   Run
    /// 2   Run work
    /// 3   Fight
    /// 4   Fight work
    /// 5   Fight run
    /// 6   Fight run work
    int8_t OldAttackReaction = 0;

    /// Some alpha feature that let units change terrain under them.
    /// Specifically from passable to impassable.
    int8_t ConvertTerrain = 0;

    std::string Name = "";
    std::string Name2 = "";

    /// MinGameVersion: SWGB
    int16_t Unitline = -1;

    /// MinGameVersion: SWGB
    int8_t MinTechLevel = -1;

    int16_t CopyID = -1;
    int16_t BaseID = -1; //not in aoe/ror
    int16_t TelemetryID = -1;

    //      Type 20+

    float Speed = 0;

    unit::Moving Moving;

    unit::Action Action;

    unit::Combat Combat;

    unit::Missile Missile;

    unit::Creatable Creatable;

    unit::Building Building;

protected:
    virtual void serializeObject(void);
};
}
