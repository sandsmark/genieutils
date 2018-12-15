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

#ifndef GENIE_RESOURCEUSAGE_H
#define GENIE_RESOURCEUSAGE_H
#include "genie/file/ISerializable.h"

namespace genie {

/// Resource IDs, AoK
enum class ResourceType : int16_t {
    InvalidResource = -1,
    FoodStorage = 0,
    WoodStorage = 1,
    StoneStorage = 2,
    GoldStorage = 3,
    PopulationHeadroom = 4,
    ConversionRange = 5,
    CurrentAge = 6,
    RelicsCaptured = 7,
    UnusedTradeBonus = 8,
    TradeGoods = 9,
    UnusedTradeProduction = 10,
    CurrentPopulation = 11,
    CorpseDecayTime = 12,
    RemarkableDiscovery = 13,
    MonumentsCaptured = 14,
    MeatStorage = 15,
    BerryStorage = 16,
    FishStorage = 17,
    Unused = 18,
    TotalUnitsOwned = 19,
    UnitsKilled = 20,
    TechnologyCount = 21,
    PercentMapExplored =22,
    CastleAgeTechID = 23,
    ImperialAgeTechID = 24,
    FeudalAgeTechID = 25,
    AttackWarningSoundID = 26,
    EnableMonkConversion = 27,
    EnableBuildingConversion = 28,
    UnusedType1 = 29,
    UnusedBuildingLimit = 30,
    UnusedFoodLimit = 31,
    BonusPopulationCap = 32,
    FoodMaintenance = 33,
    Faith = 34,
    FaithRechargingRate = 35,
    FarmFoodAmount = 36,
    CivilianPopulation = 37,
    UnusedType2 = 38,
    AllTechsAchieved = 49,
    MilitaryPopulation = 40,
    Conversions = 41,
    StandingWonders = 42,
    Razings = 43,
    KillRatio = 44,
    SurvivaltoFinish = 45,
    TributeInefficiency = 46,
    GoldMiningProductivity = 47,
    TownCenterUnavailable = 48,
    GoldCounter = 49,
    RevealAlly = 50,
    UnusedHouses = 51,
    Monasteries = 52,
    TributeSent = 53,
    AllMonumentsCaptured = 54,
    AllRelicsCaptured = 55,
    OreStorage = 56,
    KidnapStorage = 57,
    DarkAgeTechID = 58,
    UnusedTradeGoodQuality = 59,
    UnusedTradeMarketLevel = 60,
    UnusedFormations = 61,
    BuildingHousingRate = 62,
    TaxGatherRate = 63,
    GatherAccumulator = 64,
    SalvageDecayRate = 65,
    UnusedAllowFormations = 66,
    CanConvert = 67,
    HitPointsKilled = 68,
    KilledP1 = 69,
    KilledP2 = 70,
    KilledP3 = 71,
    KilledP4 = 72,
    KilledP5 = 73,
    KilledP6 = 74,
    KilledP7 = 75,
    KilledP8 = 76,
    ConversionResistance = 77,
    TradeVigRate = 78,
    StoneMiningProductivity = 79,
    QueuedUnits = 80,
    TrainingCount = 81,
    StartwithUnit444PTWC = 82,
    BoardingRechargeRate = 83,
    StartingVillagers = 84,
    ResearchCostModifier = 85,
    ResearchTimeModifier = 86,
    ConvertBoats = 87,
    FishTrapFoodAmount = 88,
    HealRateModifier = 89,
    HealingRange = 90,

    // Alpha ends here

    StartingFood = 91,
    StartingWood = 92,
    StartingStone = 93,
    StartingGold = 94,
    EnablePTWCKidnapLoot = 95,
    BerserkerHealTimer = 96,
    DominantSheepControl = 97,
    BuildingCostSum = 98,
    TechCostSum = 99,
    RelicIncomeSum = 100,
    TradeIncomeSum = 101,
    P1Tribute = 102,
    P2Tribute = 103,
    P3Tribute = 104,
    P4Tribute = 105,
    P5Tribute = 106,
    P6Tribute = 107,
    P7Tribute = 108,
    P8Tribute = 109,
    P1KillValue = 110,
    P2KillValue = 111,
    P3KillValue = 112,
    P4KillValue = 113,
    P5KillValue = 114,
    P6KillValue = 115,
    P7KillValue = 116,
    P8KillValue = 117,
    P1Razings = 118,
    P2Razings = 119,
    P3Razings = 120,
    P4Razings = 121,
    P5Razings = 122,
    P6Razings = 123,
    P7Razings = 124,
    P8Razings = 125,
    P1RazingValue = 126,
    P2RazingValue = 127,
    P3RazingValue = 128,
    P4RazingValue = 129,
    P5RazingValue = 130,
    P6RazingValue = 131,
    P7RazingValue = 132,
    P8RazingValue = 133,
    StandingCastles = 134,
    HitPointsRazed = 135,
    KillsbyP1 = 136,
    KillsbyP2 = 137,
    KillsbyP3 = 138,
    KillsbyP4 = 139,
    KillsbyP5 = 140,
    KillsbyP6 = 141,
    KillsbyP7 = 142,
    KillsbyP8 = 143,
    RazingsbyP1 = 144,
    RazingsbyP2 = 145,
    RazingsbyP3 = 146,
    RazingsbyP4 = 147,
    RazingsbyP5 = 148,
    RazingsbyP6 = 149,
    RazingsbyP7 = 150,
    RazingsbyP8 = 151,
    ValueKilledbyOthers = 152,
    ValueRazedbyOthers = 153,
    KilledbyOthers = 154,
    RazedbyOthers = 155,
    TributefromP1 = 156,
    TributefromP2 = 157,
    TributefromP3 = 158,
    TributefromP4 = 159,
    TributefromP5 = 160,
    TributefromP6 = 161,
    TributefromP7 = 162,
    TributefromP8 = 163,
    ValueCurrentUnits = 164,
    ValueCurrentBuildings = 165,
    FoodTotal = 166,
    WoodTotal = 167,
    StoneTotal = 168,
    GoldTotal = 169,
    TotalValueofKills = 170,
    TotalTributeReceived = 171,
    TotalValueofRazings = 172,
    TotalCastlesBuilt = 173,
    TotalWondersBuilt = 174,
    TributeScore = 175,
    ConvertMinAdjustment = 176,
    ConvertMaxAdjustment = 177,
    ConvertResistMinAdjustment = 178,
    ConvertResistMaxAdjustment = 179,
    ConvertBuildingMin = 180,
    ConvertBuildingMax = 181,
    ConvertBuildingChance = 182,
    RevealEnemy = 183,
    ValueWondersCastles = 184,
    FoodScore = 185,
    WoodScore = 186,
    StoneScore = 187,
    GoldScore = 188,
    TCChoppingProductivity = 189,
    TCFoodGatheringProductivity = 190,
    TCRelicGoldProductionRate = 191,
    TCConvertedUnitsDie = 192,
    TCTheocracy = 193,
    TCCrenellations = 194,
    TCConstructionRateModifier = 195,
    TCHunWonderBonus = 196,
    TCSpiesDiscount = 197,
    AKUnusedType3 = 198,
    AKUnusedType4 = 199,
    AKUnusedType5 = 200,
    AKUnusedType6 = 201,
    AKUnusedType7 = 202,
    AKUnusedType8 = 203,
    AKUnusedType9 = 204,
    AKFeitoriaFoodProductivity = 205,
    AKFeitoriaWoodProductivity = 206,
    AKFeitoriaStoneProductivity = 207,
    AKFeitoriaGoldProductivity = 208,
    RRRevealEnemyTownCenters = 209,
    RRRelicsVisibleonMap = 210,
    NumberOfTypes = 211
};

//------------------------------------------------------------------------------
/// Template class describing quantities of Resources.
/// It is a template class because different objects need different datatypes
/// for ResourceUsage members.
//
template <typename A, typename E>
class Resource : public ISerializable
{
public:
    Resource() {}

    virtual ~Resource() {}

    virtual void setGameVersion(GameVersion gv)
    {
        ISerializable::setGameVersion(gv);
    }
    int16_t Type = -1;

    /// Amount of the resource available/required/used
    A Amount = 0;

    /// Bool that determines whether it is paid or only needed.
    E Paid = 0;

private:
    virtual void serializeObject(void)
    {
        serialize<int16_t>(Type);
        serialize<A>(Amount);
        serialize<E>(Paid);
    }
};
}

#endif // GENIE_RESOURCEUSAGE_H
