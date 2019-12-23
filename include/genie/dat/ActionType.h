#pragma once

#include <cstdint>

namespace genie {

/// What type of command/action is it
/// From AGE
namespace ActionType {
enum Types : int16_t {
    None = 0, // not set for anything

    /// In AoC
    Garrison = 3,
    GatherRebuild = 5, // villagers, fishing ship
    Graze = 6,
    Combat = 7,
    Fly = 10,
    ScareHunt = 11, // Predatory animals
    UnloadBoat = 12, // longboat, elite longboat, transport ship
    Guard = 13,
    SetInitialResources = 21, // farms and fish traps
    Build = 101,
    Convert = 104,
    Heal = 105,
    Repair = 106,
    GetAutoConverted = 107, // sheep, turkey, monument, relic cart
    RetreatToShootingRage = 109, // automatically run away when attacked or something? doesn't set any parameters except autosearch and search wait time
    Hunt = 110,
    Trade = 111,
    GenerateWonderVictory = 120,
    FarmDeselectWhenBuilt = 121, // farms and fish traps
    Loot = 122, // villager, scouts, light cavalry
    UnpackAndAttack = 125, // trebuchets
    OffMapTrade1 = 131, // trade carts
    PickupRelic = 132, // monks without relics
    KidnapUnit = 135, // scout/light cavalry
    DepositRelic = 136, // monks with relics

    /// Not in AoC
    MoveTo = 1,
    Follow = 2,
    Explore = 4,
    Shoot = 8,
    Attack = 9,
    Escape = 20,
    Num = 100,
    MakeUnit = 102,
    MakeTech = 103,
    DiscoveryArtifact = 108,
    Housing = 123,
    Pack = 124,
    OffMapTrade0 = 130,
    ChargeAttack = 133,
    TransformUnit = 134,
    Shear = 149,

    /// Only in HD, I guess
    HDSiegeTowerAbility = 14,
    HDRegeneration = 150,
    HDFeitoria = 151,

    Stop = 10000, // not used in aoc
    AboutFace = 10001, // not used in aoc
    Wheel = 10002, // not used in aoc
};
} //namespace Actions

} // namespace genie
