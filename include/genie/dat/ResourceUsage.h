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
    WoodStorage,
    StoneStorage,
    GoldStorage,
    PopulationHeadroom,
    ConversionRange,
    CurrentAge,
    RelicsCaptured,
    UnusedTradeBonus,
    TradeGoods,
    UnusedTradeProduction,
    CurrentPopulation,
    CorpseDecayTime,
    RemarkableDiscovery,
    MonumentsCaptured,
    MeatStorage,
    BerryStorage,
    FishStorage,
    Unused,
    TotalUnitsOwned,
    UnitsKilled,
    TechnologyCount,
    PercentMapExplored,
    CastleAgeTechID,
    ImperialAgeTechID,
    FeudalAgeTechID,
    AttackWarningSoundID,
    EnableMonkConversion,
    EnableBuildingConversion,
    UnusedType1,
    UnusedBuildingLimit,
    UnusedFoodLimit,
    BonusPopulationCap,
    FoodMaintenance,
    Faith,
    FaithRechargingRate,
    FarmFoodAmount,
    CivilianPopulation,
    UnusedType2,
    AllTechsAchieved,
    MilitaryPopulation,
    Conversions,
    StandingWonders,
    Razings,
    KillRatio,
    SurvivaltoFinish,
    TributeInefficiency,
    GoldMiningProductivity,
    TownCenterUnavailable,
    GoldCounter,
    RevealAlly,
    UnusedHouses,
    Monasteries,
    TributeSent,
    AllMonumentsCaptured,
    AllRelicsCaptured,
    OreStorage,
    KidnapStorage,
    DarkAgeTechID,
    UnusedTradeGoodQuality,
    UnusedTradeMarketLevel,
    UnusedFormations,
    BuildingHousingRate,
    TaxGatherRate,
    GatherAccumulator,
    SalvageDecayRate,
    UnusedAllowFormations,
    CanConvert,
    HitPointsKilled,
    KilledP1,
    KilledP2,
    KilledP3,
    KilledP4,
    KilledP5,
    KilledP6,
    KilledP7,
    KilledP8,
    ConversionResistance,
    TradeVigRate,
    StoneMiningProductivity,
    QueuedUnits,
    TrainingCount,
    StartwithUnit444PTWC,
    BoardingRechargeRate,
    StartingVillagers,
    ResearchCostModifier,
    ResearchTimeModifier,
    ConvertBoats,
    FishTrapFoodAmount,
    HealRateModifier,
    HealingRange, //90alphaendshere
    StartingFood,
    StartingWood,
    StartingStone,
    StartingGold,
    EnablePTWCKidnapLoot,
    BerserkerHealTimer,
    DominantSheepControl,
    BuildingCostSum,
    TechCostSum,
    RelicIncomeSum,
    TradeIncomeSum,
    P1Tribute,
    P2Tribute,
    P3Tribute,
    P4Tribute,
    P5Tribute,
    P6Tribute,
    P7Tribute,
    P8Tribute,
    P1KillValue,
    P2KillValue,
    P3KillValue,
    P4KillValue,
    P5KillValue,
    P6KillValue,
    P7KillValue,
    P8KillValue,
    P1Razings,
    P2Razings,
    P3Razings,
    P4Razings,
    P5Razings,
    P6Razings,
    P7Razings,
    P8Razings,
    P1RazingValue,
    P2RazingValue,
    P3RazingValue,
    P4RazingValue,
    P5RazingValue,
    P6RazingValue,
    P7RazingValue,
    P8RazingValue,
    StandingCastles,
    HitPointsRazed,
    KillsbyP1,
    KillsbyP2,
    KillsbyP3,
    KillsbyP4,
    KillsbyP5,
    KillsbyP6,
    KillsbyP7,
    KillsbyP8,
    RazingsbyP1,
    RazingsbyP2,
    RazingsbyP3,
    RazingsbyP4,
    RazingsbyP5,
    RazingsbyP6,
    RazingsbyP7,
    RazingsbyP8,
    ValueKilledbyOthers,
    ValueRazedbyOthers,
    KilledbyOthers,
    RazedbyOthers,
    TributefromP1,
    TributefromP2,
    TributefromP3,
    TributefromP4,
    TributefromP5,
    TributefromP6,
    TributefromP7,
    TributefromP8,
    ValueCurrentUnits,
    ValueCurrentBuildings,
    FoodTotal,
    WoodTotal,
    StoneTotal,
    GoldTotal,
    TotalValueofKills,
    TotalTributeReceived,
    TotalValueofRazings,
    TotalCastlesBuilt,
    TotalWondersBuilt,
    TributeScore,
    ConvertMinAdjustment,
    ConvertMaxAdjustment,
    ConvertResistMinAdjustment,
    ConvertResistMaxAdjustment,
    ConvertBuildingMin,
    ConvertBuildingMax,
    ConvertBuildingChance,
    RevealEnemy,
    ValueWondersCastles,
    FoodScore,
    WoodScore,
    StoneScore,
    GoldScore,
    TCChoppingProductivity,
    TCFoodGatheringProductivity,
    TCRelicGoldProductionRate,
    TCConvertedUnitsDie,
    TCTheocracy,
    TCCrenellations,
    TCConstructionRateModifier,
    TCHunWonderBonus,
    TCSpiesDiscount,
    AKUnusedType3,
    AKUnusedType4,
    AKUnusedType5,
    AKUnusedType6,
    AKUnusedType7,
    AKUnusedType8,
    AKUnusedType9,
    AKFeitoriaFoodProductivity,
    AKFeitoriaWoodProductivity,
    AKFeitoriaStoneProductivity,
    AKFeitoriaGoldProductivity,
    RRRevealEnemyTownCenters,
    RRRelicsVisibleonMap,
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
    E Flag = 0;

private:
    virtual void serializeObject(void)
    {
        serialize<int16_t>(Type);
        serialize<A>(Amount);
        serialize<E>(Flag);
    }
};
}

#endif // GENIE_RESOURCEUSAGE_H
