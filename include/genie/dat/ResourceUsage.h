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

//------------------------------------------------------------------------------
/// Template class describing quantities of Resources.
/// It is a template class because different objects need different datatypes
/// for ResourceUsage members.
//
template <typename A, typename E>
class ResourceUsage : public ISerializable
{
public:
    ResourceUsage() {}

    virtual ~ResourceUsage() {}

    virtual void setGameVersion(GameVersion gv)
    {
        ISerializable::setGameVersion(gv);
    }

    /// Resource IDs, AoK
    enum ResourceTypes : int16_t {
        FoodStorageType = 0,
        WoodStorageType,
        StoneStorageType,
        GoldStorageType,
        PopulationHeadroomType,
        ConversionRangeType,
        CurrentAgeType,
        RelicsCapturedType,
        UnusedTradeBonusType,
        TradeGoodsType,
        UnusedTradeProductionType,
        CurrentPopulationType,
        CorpseDecayTimeType,
        RemarkableDiscoveryType,
        MonumentsCapturedType,
        MeatStorageType,
        BerryStorageType,
        FishStorageType,
        UnusedType,
        TotalUnitsOwnedType,
        UnitsKilledType,
        TechnologyCountType,
        PercentMapExploredType,
        CastleAgeTechIDType,
        ImperialAgeTechIDType,
        FeudalAgeTechIDType,
        AttackWarningSoundIDType,
        EnableMonkConversionType,
        EnableBuildingConversionType,
        UnusedType1,
        UnusedBuildingLimitType,
        UnusedFoodLimitType,
        BonusPopulationCapType,
        FoodMaintenanceType,
        FaithType,
        FaithRechargingRateType,
        FarmFoodAmountType,
        CivilianPopulationType,
        UnusedType2,
        AllTechsAchievedType,
        MilitaryPopulationType,
        ConversionsType,
        StandingWondersType,
        RazingsType,
        KillRatioType,
        SurvivaltoFinishType,
        TributeInefficiencyType,
        GoldMiningProductivityType,
        TownCenterUnavailableType,
        GoldCounterType,
        RevealAllyType,
        UnusedHousesType,
        MonasteriesType,
        TributeSentType,
        AllMonumentsCapturedType,
        AllRelicsCapturedType,
        OreStorageType,
        KidnapStorageType,
        DarkAgeTechIDType,
        UnusedTradeGoodQualityType,
        UnusedTradeMarketLevelType,
        UnusedFormationsType,
        BuildingHousingRateType,
        TaxGatherRateType,
        GatherAccumulatorType,
        SalvageDecayRateType,
        UnusedAllowFormationsType,
        CanConvertType,
        HitPointsKilledType,
        KilledP1Type,
        KilledP2Type,
        KilledP3Type,
        KilledP4Type,
        KilledP5Type,
        KilledP6Type,
        KilledP7Type,
        KilledP8Type,
        ConversionResistanceType,
        TradeVigRateType,
        StoneMiningProductivityType,
        QueuedUnitsType,
        TrainingCountType,
        StartwithUnit444PTWCType,
        BoardingRechargeRateType,
        StartingVillagersType,
        ResearchCostModifierType,
        ResearchTimeModifierType,
        ConvertBoatsType,
        FishTrapFoodAmountType,
        HealRateModifierType,
        HealingRangeType, //90alphaendshere
        StartingFoodType,
        StartingWoodType,
        StartingStoneType,
        StartingGoldType,
        EnablePTWCKidnapLootType,
        BerserkerHealTimerType,
        DominantSheepControlType,
        BuildingCostSumType,
        TechCostSumType,
        RelicIncomeSumType,
        TradeIncomeSumType,
        P1TributeType,
        P2TributeType,
        P3TributeType,
        P4TributeType,
        P5TributeType,
        P6TributeType,
        P7TributeType,
        P8TributeType,
        P1KillValueType,
        P2KillValueType,
        P3KillValueType,
        P4KillValueType,
        P5KillValueType,
        P6KillValueType,
        P7KillValueType,
        P8KillValueType,
        P1RazingsType,
        P2RazingsType,
        P3RazingsType,
        P4RazingsType,
        P5RazingsType,
        P6RazingsType,
        P7RazingsType,
        P8RazingsType,
        P1RazingValueType,
        P2RazingValueType,
        P3RazingValueType,
        P4RazingValueType,
        P5RazingValueType,
        P6RazingValueType,
        P7RazingValueType,
        P8RazingValueType,
        StandingCastlesType,
        HitPointsRazedType,
        KillsbyP1Type,
        KillsbyP2Type,
        KillsbyP3Type,
        KillsbyP4Type,
        KillsbyP5Type,
        KillsbyP6Type,
        KillsbyP7Type,
        KillsbyP8Type,
        RazingsbyP1Type,
        RazingsbyP2Type,
        RazingsbyP3Type,
        RazingsbyP4Type,
        RazingsbyP5Type,
        RazingsbyP6Type,
        RazingsbyP7Type,
        RazingsbyP8Type,
        ValueKilledbyOthersType,
        ValueRazedbyOthersType,
        KilledbyOthersType,
        RazedbyOthersType,
        TributefromP1Type,
        TributefromP2Type,
        TributefromP3Type,
        TributefromP4Type,
        TributefromP5Type,
        TributefromP6Type,
        TributefromP7Type,
        TributefromP8Type,
        ValueCurrentUnitsType,
        ValueCurrentBuildingsType,
        FoodTotalType,
        WoodTotalType,
        StoneTotalType,
        GoldTotalType,
        TotalValueofKillsType,
        TotalTributeReceivedType,
        TotalValueofRazingsType,
        TotalCastlesBuiltType,
        TotalWondersBuiltType,
        TributeScoreType,
        ConvertMinAdjustmentType,
        ConvertMaxAdjustmentType,
        ConvertResistMinAdjustmentType,
        ConvertResistMaxAdjustmentType,
        ConvertBuildingMinType,
        ConvertBuildingMaxType,
        ConvertBuildingChanceType,
        RevealEnemyType,
        ValueWondersCastlesType,
        FoodScoreType,
        WoodScoreType,
        StoneScoreType,
        GoldScoreType,
        TCChoppingProductivityType,
        TCFoodGatheringProductivityType,
        TCRelicGoldProductionRateType,
        TCConvertedUnitsDieType,
        TCTheocracyType,
        TCCrenellationsType,
        TCConstructionRateModifierType,
        TCHunWonderBonusType,
        TCSpiesDiscountType,
        AKUnusedType3,
        AKUnusedType4,
        AKUnusedType5,
        AKUnusedType6,
        AKUnusedType7,
        AKUnusedType8,
        AKUnusedType9,
        AKFeitoriaFoodProductivityType,
        AKFeitoriaWoodProductivityType,
        AKFeitoriaStoneProductivityType,
        AKFeitoriaGoldProductivityType,
        RRRevealEnemyTownCentersType,
        RRRelicsVisibleonMapType,
    };
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
