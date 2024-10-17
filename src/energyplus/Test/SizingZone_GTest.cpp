/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/DesignDay.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SizingZone) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // We need a ThermalZone, with at least one equipment (or useIdealAirLoads) AND a DesignDay, for the Sizing:Zone to be translated
  // The Zone itself needs at least one space to be translated
  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  SizingZone sz = z.sizingZone();

  DesignDay d(m);

  // The Zone isn't "conditioned" yet, so not translated
  {
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Zone).size());
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    EXPECT_EQ(0u, idfObjs.size());
  }

  // Zone is conditioned, it's translated
  // Since all the fields that belong to DesignSpecification:ZoneAirDistribution (DSZAD) are not filled, the DSZAD isn't translated
  EXPECT_TRUE(z.setUseIdealAirLoads(true));
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted());
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted());
  EXPECT_TRUE(sz.isDesignZoneSecondaryRecirculationFractionDefaulted());
  EXPECT_TRUE(sz.isDesignMinimumZoneVentilationEfficiencyDefaulted());
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    EXPECT_EQ("", idfObjs[0].getString(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName).get());
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::DesignSpecification_ZoneAirDistribution).size());
  }

  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinCoolingMode(0.8));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DesignSpecification_ZoneAirDistribution).size());
    boost::optional<WorkspaceObject> _i_dszad = idf_sz.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);
    ASSERT_TRUE(_i_dszad);
    EXPECT_EQ(0.8, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode).get());
    EXPECT_FALSE(_i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode));
    EXPECT_FALSE(_i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction));
    EXPECT_FALSE(_i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency));
  }

  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinHeatingMode(0.7));
  EXPECT_TRUE(sz.setDesignZoneSecondaryRecirculationFraction(0.6));
  EXPECT_TRUE(sz.setDesignMinimumZoneVentilationEfficiency(0.5));
  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinCoolingMode(0.8));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::DesignSpecification_ZoneAirDistribution).size());
    boost::optional<WorkspaceObject> _i_dszad = idf_sz.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);
    ASSERT_TRUE(_i_dszad);
    EXPECT_EQ(0.8, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode).get());
    EXPECT_EQ(0.7, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode).get());
    EXPECT_EQ(0.6, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction).get());
    EXPECT_EQ(0.5, _i_dszad->getDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency).get());

    // New E+ 22.2.0 fields: getting the defaults
    EXPECT_EQ("Sensible Load Only No Latent Load", idf_sz.getString(Sizing_ZoneFields::ZoneLoadSizingMethod).get());
    EXPECT_EQ("HumidityRatioDifference", idf_sz.getString(Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio));
    EXPECT_EQ(0.005, idf_sz.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference).get());
    EXPECT_EQ("HumidityRatioDifference", idf_sz.getString(Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio));
    EXPECT_EQ(0.005, idf_sz.getDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName));
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName));
  }

  EXPECT_TRUE(sz.setZoneLoadSizingMethod("Sensible Load Only No Latent Load"));
  EXPECT_TRUE(sz.setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod("SupplyAirHumidityRatio"));
  EXPECT_TRUE(sz.setZoneDehumidificationDesignSupplyAirHumidityRatio(0.008));
  EXPECT_TRUE(sz.setZoneCoolingDesignSupplyAirHumidityRatioDifference(0.0051));
  EXPECT_TRUE(sz.setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod("SupplyAirHumidityRatio"));
  EXPECT_TRUE(sz.setZoneHumidificationDesignSupplyAirHumidityRatio(0.004));
  EXPECT_TRUE(sz.setZoneHumidificationDesignSupplyAirHumidityRatioDifference(0.0049));

  ScheduleConstant dehumSch(m);
  EXPECT_TRUE(sz.setZoneHumidistatDehumidificationSetPointSchedule(dehumSch));

  ScheduleConstant humSch(m);
  EXPECT_TRUE(sz.setZoneHumidistatHumidificationSetPointSchedule(humSch));

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ("Sensible Load Only No Latent Load", idf_sz.getString(Sizing_ZoneFields::ZoneLoadSizingMethod).get());
    // SupplyAirHumidityRatio: the Difference field isn't used
    EXPECT_EQ("SupplyAirHumidityRatio", idf_sz.getString(Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod).get());
    EXPECT_EQ(0.008, idf_sz.getDouble(Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference));
    EXPECT_EQ("SupplyAirHumidityRatio", idf_sz.getString(Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod).get());
    EXPECT_EQ(0.004, idf_sz.getDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference));

    // Sensible Load Only No Latent Load -> Hum/dehum schedules are ignored
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName));
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName));
  }

  EXPECT_TRUE(sz.setZoneLoadSizingMethod("Sensible And Latent Load"));
  EXPECT_TRUE(sz.setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod("HumidityRatioDifference"));
  EXPECT_TRUE(sz.setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod("HumidityRatioDifference"));

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ("Sensible And Latent Load", idf_sz.getString(Sizing_ZoneFields::ZoneLoadSizingMethod).get());

    // HumidityRatioDifference: the Ratio field isn't used
    EXPECT_EQ("HumidityRatioDifference", idf_sz.getString(Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio));
    EXPECT_EQ(0.0051, idf_sz.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference).get());
    EXPECT_EQ("HumidityRatioDifference", idf_sz.getString(Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod).get());
    EXPECT_TRUE(idf_sz.isEmpty(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio));
    EXPECT_EQ(0.0049, idf_sz.getDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference).get());
    EXPECT_EQ(dehumSch.nameString(), idf_sz.getString(Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName).get());
    EXPECT_EQ(humSch.nameString(), idf_sz.getString(Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName).get());
  }

  EXPECT_TRUE(sz.setSizingOption("NonCoincident"));

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Sizing_Zone);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_sz(idfObjs[0]);

    EXPECT_EQ("NonCoincident", idf_sz.getString(Sizing_ZoneFields::TypeofSpaceSumtoUse).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SizingZone) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto wo_zone = w.addObject(IdfObject(IddObjectType::Zone)).get();
  wo_zone.setName("Zone1");

  auto wo_sz = w.addObject(IdfObject(IddObjectType::Sizing_Zone)).get();
  wo_sz.setName("Zone1 SizingZone");
  EXPECT_TRUE(wo_sz.setPointer(Sizing_ZoneFields::ZoneorZoneListName, wo_zone.handle()));

  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, "SupplyAirTemperature"));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, 14.0));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, 11.11));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, "SupplyAirTemperature"));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, 40.0));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference, 11.11));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, 0.0085));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, 0.008));

  auto wo_dsoa = w.addObject(IdfObject(IddObjectType::DesignSpecification_OutdoorAir)).get();
  wo_dsoa.setName("Thermal Zone 1 Zero air DSOA");
  EXPECT_TRUE(wo_dsoa.setString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, "Sum"));
  EXPECT_TRUE(wo_dsoa.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, 0.0));
  EXPECT_TRUE(wo_dsoa.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea, 0.0));
  EXPECT_TRUE(wo_dsoa.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone, 0.0));
  EXPECT_TRUE(wo_dsoa.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, 0.0));
  EXPECT_TRUE(wo_dsoa.setString(DesignSpecification_OutdoorAirFields::OutdoorAirScheduleName, ""));
  EXPECT_TRUE(wo_dsoa.setString(DesignSpecification_OutdoorAirFields::ProportionalControlMinimumOutdoorAirFlowRateScheduleName, ""));
  EXPECT_TRUE(wo_sz.setPointer(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName, wo_dsoa.handle()));

  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneHeatingSizingFactor, 1.1));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneCoolingSizingFactor, ""));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::CoolingDesignAirFlowMethod, "DesignDay"));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::CoolingDesignAirFlowRate, ""));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, 0.00081));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::CoolingMinimumAirFlow, ""));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlowFraction, 0.1));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::HeatingDesignAirFlowMethod, "DesignDay"));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::HeatingDesignAirFlowRate, ""));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, 0.0024));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlow, 0.145));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction, 0.3));

  auto wo_dszad = w.addObject(IdfObject(IddObjectType::DesignSpecification_ZoneAirDistribution)).get();
  EXPECT_TRUE(wo_dszad.setDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode, 0.8));
  EXPECT_TRUE(wo_dszad.setDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode, 0.7));
  EXPECT_TRUE(wo_dszad.setDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction, 0.6));
  EXPECT_TRUE(wo_dszad.setDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency, 0.5));
  EXPECT_TRUE(wo_sz.setPointer(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName, wo_dszad.handle()));

  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, "No"));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy, "NeutralSupplyAir"));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, 18.0));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, 19.0));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneLoadSizingMethod, "Sensible And Latent Load"));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod, "HumidityRatioDifference"));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio, ""));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference, 0.0051));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod, "SupplyAirHumidityRatio"));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio, 0.004));
  EXPECT_TRUE(wo_sz.setString(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference, ""));

  auto wo_dehumSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  wo_dehumSch.setName("dehumSch");
  EXPECT_TRUE(wo_dehumSch.setDouble(Schedule_ConstantFields::HourlyValue, 60.0));
  EXPECT_TRUE(wo_sz.setPointer(Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName, wo_dehumSch.handle()));

  auto wo_humSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  wo_humSch.setName("humSch");
  EXPECT_TRUE(wo_humSch.setDouble(Schedule_ConstantFields::HourlyValue, 40.0));
  EXPECT_TRUE(wo_sz.setPointer(Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName, wo_humSch.handle()));

  // RT
  Model m = rt.translateWorkspace(w);

  ASSERT_EQ(1, m.getConcreteModelObjects<ThermalZone>().size());
  auto szs = m.getConcreteModelObjects<SizingZone>();
  ASSERT_EQ(1, szs.size());
  auto& sz = szs[0];

  EXPECT_EQ("SupplyAirTemperature", sz.zoneCoolingDesignSupplyAirTemperatureInputMethod());
  EXPECT_EQ(14.0, sz.zoneCoolingDesignSupplyAirTemperature());
  EXPECT_EQ(11.11, sz.zoneCoolingDesignSupplyAirTemperatureDifference());
  EXPECT_EQ("SupplyAirTemperature", sz.zoneHeatingDesignSupplyAirTemperatureInputMethod());
  EXPECT_EQ(40.0, sz.zoneHeatingDesignSupplyAirTemperature());
  EXPECT_EQ(11.11, sz.zoneHeatingDesignSupplyAirTemperatureDifference());
  EXPECT_EQ(0.0085, sz.zoneCoolingDesignSupplyAirHumidityRatio());
  EXPECT_EQ(0.008, sz.zoneHeatingDesignSupplyAirHumidityRatio());
  ASSERT_TRUE(sz.zoneHeatingSizingFactor());
  EXPECT_EQ(1.1, sz.zoneHeatingSizingFactor().get());
  EXPECT_FALSE(sz.zoneCoolingSizingFactor());

  EXPECT_EQ("DesignDay", sz.coolingDesignAirFlowMethod());

  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, 0.0024));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlow, 0.145));
  EXPECT_TRUE(wo_sz.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction, 0.3));

  EXPECT_TRUE(sz.isCoolingDesignAirFlowRateDefaulted());

  EXPECT_FALSE(sz.isCoolingMinimumAirFlowperZoneFloorAreaDefaulted());
  EXPECT_EQ(0.00081, sz.coolingMinimumAirFlowperZoneFloorArea());

  EXPECT_TRUE(sz.isCoolingMinimumAirFlowDefaulted());

  EXPECT_FALSE(sz.isCoolingMinimumAirFlowFractionDefaulted());
  EXPECT_EQ(0.1, sz.coolingMinimumAirFlowFraction());

  EXPECT_EQ("DesignDay", sz.heatingDesignAirFlowMethod());

  EXPECT_TRUE(sz.isHeatingDesignAirFlowRateDefaulted());

  EXPECT_FALSE(sz.isHeatingMaximumAirFlowperZoneFloorAreaDefaulted());
  EXPECT_EQ(0.0024, sz.heatingMaximumAirFlowperZoneFloorArea());

  EXPECT_FALSE(sz.isHeatingMaximumAirFlowDefaulted());
  EXPECT_EQ(0.145, sz.heatingMaximumAirFlow());

  EXPECT_FALSE(sz.isHeatingMaximumAirFlowFractionDefaulted());
  EXPECT_EQ(0.3, sz.heatingMaximumAirFlowFraction());

  EXPECT_FALSE(sz.accountforDedicatedOutdoorAirSystem());
  EXPECT_EQ("NeutralSupplyAir", sz.dedicatedOutdoorAirSystemControlStrategy());
  ASSERT_TRUE(sz.dedicatedOutdoorAirLowSetpointTemperatureforDesign());
  EXPECT_EQ(18.0, sz.dedicatedOutdoorAirLowSetpointTemperatureforDesign().get());
  ASSERT_TRUE(sz.dedicatedOutdoorAirHighSetpointTemperatureforDesign());
  EXPECT_EQ(19.0, sz.dedicatedOutdoorAirHighSetpointTemperatureforDesign().get());

  EXPECT_EQ("Sensible And Latent Load", sz.zoneLoadSizingMethod());

  EXPECT_EQ("HumidityRatioDifference", sz.zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod());
  EXPECT_FALSE(sz.zoneDehumidificationDesignSupplyAirHumidityRatio());
  EXPECT_EQ(0.0051, sz.zoneCoolingDesignSupplyAirHumidityRatioDifference());

  EXPECT_EQ("SupplyAirHumidityRatio", sz.zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod());
  ASSERT_TRUE(sz.zoneHumidificationDesignSupplyAirHumidityRatio());
  EXPECT_EQ(0.004, sz.zoneHumidificationDesignSupplyAirHumidityRatio().get());

  ASSERT_TRUE(sz.zoneHumidistatDehumidificationSetPointSchedule());
  EXPECT_EQ("dehumSch", sz.zoneHumidistatDehumidificationSetPointSchedule()->nameString());
  ASSERT_TRUE(sz.zoneHumidistatHumidificationSetPointSchedule());
  EXPECT_EQ("humSch", sz.zoneHumidistatHumidificationSetPointSchedule()->nameString());

  // From the ZoneAirDistribution object
  EXPECT_FALSE(sz.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted());
  EXPECT_EQ(0.8, sz.designZoneAirDistributionEffectivenessinCoolingMode());

  EXPECT_FALSE(sz.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted());
  EXPECT_EQ(0.7, sz.designZoneAirDistributionEffectivenessinHeatingMode());

  EXPECT_FALSE(sz.isDesignZoneSecondaryRecirculationFractionDefaulted());
  EXPECT_EQ(0.6, sz.designZoneSecondaryRecirculationFraction());

  EXPECT_FALSE(sz.isDesignMinimumZoneVentilationEfficiencyDefaulted());
  EXPECT_EQ(0.5, sz.designMinimumZoneVentilationEfficiency());
}
