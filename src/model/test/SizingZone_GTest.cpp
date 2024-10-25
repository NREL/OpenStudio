/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SizingZone.hpp"
#include "../SizingZone_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SizingZone_GettersSetters) {

  Model m;
  ThermalZone z(m);
  SizingZone sz = z.sizingZone();

  // Zone or ZoneList Name:  Object
  EXPECT_EQ(z, sz.thermalZone());

  // Zone Cooling Design Supply Air Temperature Input Method:  String
  // Default in Ctor
  EXPECT_EQ("SupplyAirTemperature", sz.zoneCoolingDesignSupplyAirTemperatureInputMethod());
  // Test a valid choice
  EXPECT_TRUE(sz.setZoneCoolingDesignSupplyAirTemperatureInputMethod("TemperatureDifference"));
  EXPECT_EQ("TemperatureDifference", sz.zoneCoolingDesignSupplyAirTemperatureInputMethod());
  // Test an invalid choice
  EXPECT_FALSE(sz.setZoneCoolingDesignSupplyAirTemperatureInputMethod("BadChoice"));
  EXPECT_EQ("TemperatureDifference", sz.zoneCoolingDesignSupplyAirTemperatureInputMethod());

  // Zone Cooling Design Supply Air Temperature:  Double
  // Default in Ctor
  EXPECT_EQ(14.0, sz.zoneCoolingDesignSupplyAirTemperature());
  EXPECT_TRUE(sz.setZoneCoolingDesignSupplyAirTemperature(10.03));
  EXPECT_EQ(10.03, sz.zoneCoolingDesignSupplyAirTemperature());

  // Zone Cooling Design Supply Air Temperature Difference:  Double
  // Default in Ctor
  EXPECT_EQ(11.11, sz.zoneCoolingDesignSupplyAirTemperatureDifference());
  EXPECT_TRUE(sz.setZoneCoolingDesignSupplyAirTemperatureDifference(15.03));
  EXPECT_EQ(15.03, sz.zoneCoolingDesignSupplyAirTemperatureDifference());

  // Zone Heating Design Supply Air Temperature Input Method:  String
  // Default in Ctor
  EXPECT_EQ("SupplyAirTemperature", sz.zoneHeatingDesignSupplyAirTemperatureInputMethod());
  // Test a valid choice
  EXPECT_TRUE(sz.setZoneHeatingDesignSupplyAirTemperatureInputMethod("TemperatureDifference"));
  EXPECT_EQ("TemperatureDifference", sz.zoneHeatingDesignSupplyAirTemperatureInputMethod());
  // Test an invalid choice
  EXPECT_FALSE(sz.setZoneHeatingDesignSupplyAirTemperatureInputMethod("BadChoice"));
  EXPECT_EQ("TemperatureDifference", sz.zoneHeatingDesignSupplyAirTemperatureInputMethod());

  // Zone Heating Design Supply Air Temperature:  Double
  // Default in Ctor
  EXPECT_EQ(40.0, sz.zoneHeatingDesignSupplyAirTemperature());
  EXPECT_TRUE(sz.setZoneHeatingDesignSupplyAirTemperature(38.03));
  EXPECT_EQ(38.03, sz.zoneHeatingDesignSupplyAirTemperature());

  // Zone Heating Design Supply Air Temperature Difference:  Double
  // Default in Ctor
  EXPECT_EQ(11.11, sz.zoneHeatingDesignSupplyAirTemperatureDifference());
  EXPECT_TRUE(sz.setZoneHeatingDesignSupplyAirTemperatureDifference(9.03));
  EXPECT_EQ(9.03, sz.zoneHeatingDesignSupplyAirTemperatureDifference());

  // Zone Cooling Design Supply Air Humidity Ratio:  Double
  // Default in Ctor
  EXPECT_EQ(0.0085, sz.zoneCoolingDesignSupplyAirHumidityRatio());
  EXPECT_TRUE(sz.setZoneCoolingDesignSupplyAirHumidityRatio(0.0095));
  EXPECT_EQ(0.0095, sz.zoneCoolingDesignSupplyAirHumidityRatio());

  // Zone Heating Design Supply Air Humidity Ratio:  Double
  // Default in Ctor
  EXPECT_EQ(0.0080, sz.zoneHeatingDesignSupplyAirHumidityRatio());
  EXPECT_TRUE(sz.setZoneHeatingDesignSupplyAirHumidityRatio(0.0075));
  EXPECT_EQ(0.0075, sz.zoneHeatingDesignSupplyAirHumidityRatio());

  // Zone Heating Sizing Factor: Optional Double
  // No Default
  EXPECT_FALSE(sz.zoneHeatingSizingFactor());
  EXPECT_TRUE(sz.setZoneHeatingSizingFactor(1.0));
  ASSERT_TRUE(sz.zoneHeatingSizingFactor());
  EXPECT_EQ(1.0, sz.zoneHeatingSizingFactor().get());
  // Test reset
  sz.resetZoneHeatingSizingFactor();
  EXPECT_FALSE(sz.zoneHeatingSizingFactor());

  // Zone Cooling Sizing Factor: Optional Double
  // No Default
  EXPECT_FALSE(sz.zoneCoolingSizingFactor());
  EXPECT_TRUE(sz.setZoneCoolingSizingFactor(1.20));
  ASSERT_TRUE(sz.zoneCoolingSizingFactor());
  EXPECT_EQ(1.2, sz.zoneCoolingSizingFactor().get());
  // Test reset
  sz.resetZoneCoolingSizingFactor();
  EXPECT_FALSE(sz.zoneCoolingSizingFactor());

  // Cooling Design Air Flow Method:  String
  // Check Idd default: "DesignDay", overriden in Ctor with same value
  // EXPECT_TRUE(sz.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ("DesignDay", sz.coolingDesignAirFlowMethod());
  // Test a valid choice
  EXPECT_TRUE(sz.setCoolingDesignAirFlowMethod("Flow/Zone"));
  EXPECT_FALSE(sz.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ("Flow/Zone", sz.coolingDesignAirFlowMethod());
  // Test an invalid choice
  EXPECT_FALSE(sz.setCoolingDesignAirFlowMethod("BadChoice"));
  EXPECT_EQ("Flow/Zone", sz.coolingDesignAirFlowMethod());
  // Test reset
  sz.resetCoolingDesignAirFlowMethod();
  EXPECT_TRUE(sz.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ("DesignDay", sz.coolingDesignAirFlowMethod());

  // Cooling Design Air Flow Rate:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(sz.isCoolingDesignAirFlowRateDefaulted());
  EXPECT_EQ(0.0, sz.coolingDesignAirFlowRate());
  EXPECT_TRUE(sz.setCoolingDesignAirFlowRate(0.5));
  EXPECT_FALSE(sz.isCoolingDesignAirFlowRateDefaulted());
  EXPECT_EQ(0.5, sz.coolingDesignAirFlowRate());
  // Test reset
  sz.resetCoolingDesignAirFlowRate();
  EXPECT_EQ(0.0, sz.coolingDesignAirFlowRate());
  EXPECT_TRUE(sz.isCoolingDesignAirFlowRateDefaulted());

  // Cooling Minimum Air Flow per Zone Floor Area:  Double
  // Check Idd default: 0.000762
  EXPECT_TRUE(sz.isCoolingMinimumAirFlowperZoneFloorAreaDefaulted());
  EXPECT_EQ(0.000762, sz.coolingMinimumAirFlowperZoneFloorArea());
  EXPECT_TRUE(sz.setCoolingMinimumAirFlowperZoneFloorArea(0.0001));
  EXPECT_FALSE(sz.isCoolingMinimumAirFlowperZoneFloorAreaDefaulted());
  EXPECT_EQ(0.0001, sz.coolingMinimumAirFlowperZoneFloorArea());
  // Test reset
  sz.resetCoolingMinimumAirFlowperZoneFloorArea();
  EXPECT_EQ(0.000762, sz.coolingMinimumAirFlowperZoneFloorArea());
  EXPECT_TRUE(sz.isCoolingMinimumAirFlowperZoneFloorAreaDefaulted());

  // Cooling Minimum Air Flow:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(sz.isCoolingMinimumAirFlowDefaulted());
  EXPECT_EQ(0.0, sz.coolingMinimumAirFlow());
  EXPECT_TRUE(sz.setCoolingMinimumAirFlow(0.35));
  EXPECT_FALSE(sz.isCoolingMinimumAirFlowDefaulted());
  EXPECT_EQ(0.35, sz.coolingMinimumAirFlow());
  // Test reset
  sz.resetCoolingMinimumAirFlow();
  EXPECT_EQ(0.0, sz.coolingMinimumAirFlow());
  EXPECT_TRUE(sz.isCoolingMinimumAirFlowDefaulted());

  // Cooling Minimum Air Flow Fraction:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(sz.isCoolingMinimumAirFlowFractionDefaulted());
  EXPECT_EQ(0.0, sz.coolingMinimumAirFlowFraction());
  EXPECT_TRUE(sz.setCoolingMinimumAirFlowFraction(0.15));
  EXPECT_FALSE(sz.isCoolingMinimumAirFlowFractionDefaulted());
  EXPECT_EQ(0.15, sz.coolingMinimumAirFlowFraction());
  // Test reset
  sz.resetCoolingMinimumAirFlowFraction();
  EXPECT_EQ(0.0, sz.coolingMinimumAirFlowFraction());
  EXPECT_TRUE(sz.isCoolingMinimumAirFlowFractionDefaulted());

  // Heating Design Air Flow Method:  String
  // Check Idd default: "DesignDay", overriden in Ctor by same value
  //EXPECT_TRUE(sz.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ("DesignDay", sz.heatingDesignAirFlowMethod());
  // Test a valid choice
  EXPECT_TRUE(sz.setHeatingDesignAirFlowMethod("Flow/Zone"));
  EXPECT_FALSE(sz.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ("Flow/Zone", sz.heatingDesignAirFlowMethod());
  // Test an invalid choice
  EXPECT_FALSE(sz.setHeatingDesignAirFlowMethod("BadChoice"));
  EXPECT_EQ("Flow/Zone", sz.heatingDesignAirFlowMethod());
  // Test reset
  sz.resetHeatingDesignAirFlowMethod();
  EXPECT_TRUE(sz.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ("DesignDay", sz.heatingDesignAirFlowMethod());

  // Heating Design Air Flow Rate:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(sz.isHeatingDesignAirFlowRateDefaulted());
  EXPECT_EQ(0.0, sz.heatingDesignAirFlowRate());
  EXPECT_TRUE(sz.setHeatingDesignAirFlowRate(0.28));
  EXPECT_FALSE(sz.isHeatingDesignAirFlowRateDefaulted());
  EXPECT_EQ(0.28, sz.heatingDesignAirFlowRate());
  // Test reset
  sz.resetHeatingDesignAirFlowRate();
  EXPECT_EQ(0.0, sz.heatingDesignAirFlowRate());
  EXPECT_TRUE(sz.isHeatingDesignAirFlowRateDefaulted());

  // Heating Maximum Air Flow per Zone Floor Area:  Double
  // Check Idd default: 0.002032
  EXPECT_TRUE(sz.isHeatingMaximumAirFlowperZoneFloorAreaDefaulted());
  EXPECT_EQ(0.002032, sz.heatingMaximumAirFlowperZoneFloorArea());
  EXPECT_TRUE(sz.setHeatingMaximumAirFlowperZoneFloorArea(0.075));
  EXPECT_FALSE(sz.isHeatingMaximumAirFlowperZoneFloorAreaDefaulted());
  EXPECT_EQ(0.075, sz.heatingMaximumAirFlowperZoneFloorArea());
  // Test reset
  sz.resetHeatingMaximumAirFlowperZoneFloorArea();
  EXPECT_EQ(0.002032, sz.heatingMaximumAirFlowperZoneFloorArea());
  EXPECT_TRUE(sz.isHeatingMaximumAirFlowperZoneFloorAreaDefaulted());

  // Heating Maximum Air Flow:  Double
  // Check Idd default: 0.1415762
  EXPECT_TRUE(sz.isHeatingMaximumAirFlowDefaulted());
  EXPECT_EQ(0.1415762, sz.heatingMaximumAirFlow());
  EXPECT_TRUE(sz.setHeatingMaximumAirFlow(0.07));
  EXPECT_FALSE(sz.isHeatingMaximumAirFlowDefaulted());
  EXPECT_EQ(0.07, sz.heatingMaximumAirFlow());
  // Test reset
  sz.resetHeatingMaximumAirFlow();
  EXPECT_EQ(0.1415762, sz.heatingMaximumAirFlow());
  EXPECT_TRUE(sz.isHeatingMaximumAirFlowDefaulted());

  // Heating Maximum Air Flow Fraction:  Double
  // Check Idd default: 0.3
  EXPECT_TRUE(sz.isHeatingMaximumAirFlowFractionDefaulted());
  EXPECT_EQ(0.3, sz.heatingMaximumAirFlowFraction());
  EXPECT_TRUE(sz.setHeatingMaximumAirFlowFraction(0.125));
  EXPECT_FALSE(sz.isHeatingMaximumAirFlowFractionDefaulted());
  EXPECT_EQ(0.125, sz.heatingMaximumAirFlowFraction());
  // Test reset
  sz.resetHeatingMaximumAirFlowFraction();
  EXPECT_EQ(0.3, sz.heatingMaximumAirFlowFraction());
  EXPECT_TRUE(sz.isHeatingMaximumAirFlowFractionDefaulted());

  // Design Zone Air Distribution Effectiveness in Cooling Mode:  Double
  // Check Idd default: 1.0
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted());
  EXPECT_EQ(1.0, sz.designZoneAirDistributionEffectivenessinCoolingMode());
  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinCoolingMode(0.65));
  EXPECT_FALSE(sz.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted());
  EXPECT_EQ(0.65, sz.designZoneAirDistributionEffectivenessinCoolingMode());
  // Test reset
  sz.resetDesignZoneAirDistributionEffectivenessinCoolingMode();
  EXPECT_EQ(1.0, sz.designZoneAirDistributionEffectivenessinCoolingMode());
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted());

  // Design Zone Air Distribution Effectiveness in Heating Mode:  Double
  // Check Idd default: 1.0
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted());
  EXPECT_EQ(1.0, sz.designZoneAirDistributionEffectivenessinHeatingMode());
  EXPECT_TRUE(sz.setDesignZoneAirDistributionEffectivenessinHeatingMode(0.45));
  EXPECT_FALSE(sz.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted());
  EXPECT_EQ(0.45, sz.designZoneAirDistributionEffectivenessinHeatingMode());
  // Test reset
  sz.resetDesignZoneAirDistributionEffectivenessinHeatingMode();
  EXPECT_EQ(1.0, sz.designZoneAirDistributionEffectivenessinHeatingMode());
  EXPECT_TRUE(sz.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted());

  // Design Zone Secondary Recirculation Fraction:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(sz.isDesignZoneSecondaryRecirculationFractionDefaulted());
  EXPECT_EQ(0.0, sz.designZoneSecondaryRecirculationFraction());
  EXPECT_TRUE(sz.setDesignZoneSecondaryRecirculationFraction(0.35));
  EXPECT_FALSE(sz.isDesignZoneSecondaryRecirculationFractionDefaulted());
  EXPECT_EQ(0.35, sz.designZoneSecondaryRecirculationFraction());
  // Test reset
  sz.resetDesignZoneSecondaryRecirculationFraction();
  EXPECT_EQ(0.0, sz.designZoneSecondaryRecirculationFraction());
  EXPECT_TRUE(sz.isDesignZoneSecondaryRecirculationFractionDefaulted());

  // Design Minimum Zone Ventilation Efficiency:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(sz.isDesignMinimumZoneVentilationEfficiencyDefaulted());
  EXPECT_EQ(0.0, sz.designMinimumZoneVentilationEfficiency());
  EXPECT_TRUE(sz.setDesignMinimumZoneVentilationEfficiency(0.25));
  EXPECT_FALSE(sz.isDesignMinimumZoneVentilationEfficiencyDefaulted());
  EXPECT_EQ(0.25, sz.designMinimumZoneVentilationEfficiency());
  // Test reset
  sz.resetDesignMinimumZoneVentilationEfficiency();
  EXPECT_EQ(0.0, sz.designMinimumZoneVentilationEfficiency());
  EXPECT_TRUE(sz.isDesignMinimumZoneVentilationEfficiencyDefaulted());

  // Account for Dedicated Outdoor Air System:  Boolean
  // Default in Ctor
  EXPECT_FALSE(sz.accountforDedicatedOutdoorAirSystem());
  // Test true
  EXPECT_TRUE(sz.setAccountforDedicatedOutdoorAirSystem(true));
  EXPECT_TRUE(sz.accountforDedicatedOutdoorAirSystem());
  // Test false
  EXPECT_TRUE(sz.setAccountforDedicatedOutdoorAirSystem(false));
  EXPECT_FALSE(sz.accountforDedicatedOutdoorAirSystem());

  // Dedicated Outdoor Air System Control Strategy:  String
  // Default in Ctor
  EXPECT_TRUE(sz.setDedicatedOutdoorAirSystemControlStrategy("NeutralSupplyAir"));

  // Test a valid choice
  EXPECT_TRUE(sz.setDedicatedOutdoorAirSystemControlStrategy("NeutralDehumidifiedSupplyAir"));
  EXPECT_EQ("NeutralDehumidifiedSupplyAir", sz.dedicatedOutdoorAirSystemControlStrategy());
  // Test an invalid choice
  EXPECT_FALSE(sz.setDedicatedOutdoorAirSystemControlStrategy("BadChoice"));
  EXPECT_EQ("NeutralDehumidifiedSupplyAir", sz.dedicatedOutdoorAirSystemControlStrategy());

  // Dedicated Outdoor Air Low Setpoint Temperature for Design: Optional Double
  // Defaults to autosize in Ctor
  EXPECT_TRUE(sz.isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized());

  EXPECT_TRUE(sz.setDedicatedOutdoorAirLowSetpointTemperatureforDesign(21.1));
  ASSERT_TRUE(sz.dedicatedOutdoorAirLowSetpointTemperatureforDesign());
  EXPECT_EQ(21.1, sz.dedicatedOutdoorAirLowSetpointTemperatureforDesign().get());
  // Test autosizing
  EXPECT_FALSE(sz.isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized());
  sz.autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();
  EXPECT_TRUE(sz.isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized());

  // Dedicated Outdoor Air High Setpoint Temperature for Design: Optional Double
  // Defaults to autosize in Ctor
  EXPECT_TRUE(sz.isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized());

  EXPECT_TRUE(sz.setDedicatedOutdoorAirHighSetpointTemperatureforDesign(23.9));
  ASSERT_TRUE(sz.dedicatedOutdoorAirHighSetpointTemperatureforDesign());
  EXPECT_EQ(23.9, sz.dedicatedOutdoorAirHighSetpointTemperatureforDesign().get());
  // Test autosizing
  EXPECT_FALSE(sz.isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized());
  sz.autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();
  EXPECT_TRUE(sz.isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized());

  // Zone Load Sizing Method: String
  // Default value from IDD, set in Ctor
  EXPECT_EQ("Sensible Load Only No Latent Load", sz.zoneLoadSizingMethod());
  // Set
  EXPECT_TRUE(sz.setZoneLoadSizingMethod("Sensible Load"));
  EXPECT_EQ("Sensible Load", sz.zoneLoadSizingMethod());
  // Bad Value
  EXPECT_FALSE(sz.setZoneLoadSizingMethod("BADENUM"));
  EXPECT_EQ("Sensible Load", sz.zoneLoadSizingMethod());

  // Zone Latent Cooling Design Supply Air Humidity Ratio Input Method: String
  // Default value from IDD, set in Ctor
  EXPECT_EQ("HumidityRatioDifference", sz.zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod());
  // Set
  EXPECT_TRUE(sz.setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod("SupplyAirHumidityRatio"));
  EXPECT_EQ("SupplyAirHumidityRatio", sz.zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod());
  // Bad Value
  EXPECT_FALSE(sz.setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod("BADENUM"));
  EXPECT_EQ("SupplyAirHumidityRatio", sz.zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod());

  // Zone Dehumidification Design Supply Air Humidity Ratio: Optional Double
  // IDD has no default, I don't have a good one, so empty...
  EXPECT_FALSE(sz.zoneDehumidificationDesignSupplyAirHumidityRatio());
  // Set
  EXPECT_TRUE(sz.setZoneDehumidificationDesignSupplyAirHumidityRatio(2.9));
  ASSERT_TRUE(sz.zoneDehumidificationDesignSupplyAirHumidityRatio());
  EXPECT_EQ(2.9, sz.zoneDehumidificationDesignSupplyAirHumidityRatio().get());
  // Bad Value
  EXPECT_FALSE(sz.setZoneDehumidificationDesignSupplyAirHumidityRatio(-10.0));
  ASSERT_TRUE(sz.zoneDehumidificationDesignSupplyAirHumidityRatio());
  EXPECT_EQ(2.9, sz.zoneDehumidificationDesignSupplyAirHumidityRatio().get());

  // Zone Cooling Design Supply Air Humidity Ratio Difference: Double
  // Default value from IDD, set in Ctor
  EXPECT_EQ(0.005, sz.zoneCoolingDesignSupplyAirHumidityRatioDifference());
  // Set
  EXPECT_TRUE(sz.setZoneCoolingDesignSupplyAirHumidityRatioDifference(3.0));
  EXPECT_EQ(3.0, sz.zoneCoolingDesignSupplyAirHumidityRatioDifference());
  // Bad Value
  EXPECT_FALSE(sz.setZoneCoolingDesignSupplyAirHumidityRatioDifference(-10.0));
  EXPECT_EQ(3.0, sz.zoneCoolingDesignSupplyAirHumidityRatioDifference());

  // Zone Latent Heating Design Supply Air Humidity Ratio Input Method: Optional String
  // Default value from IDD, set in IDD
  EXPECT_EQ("HumidityRatioDifference", sz.zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod());
  // Set
  EXPECT_TRUE(sz.setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod("SupplyAirHumidityRatio"));
  EXPECT_EQ("SupplyAirHumidityRatio", sz.zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod());
  // Bad Value
  EXPECT_FALSE(sz.setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod("BADENUM"));
  EXPECT_EQ("SupplyAirHumidityRatio", sz.zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod());

  // Zone Humidification Design Supply Air Humidity Ratio: Optional Double
  // IDD has no default, I don't have a good one, so empty...
  EXPECT_FALSE(sz.zoneHumidificationDesignSupplyAirHumidityRatio());
  // Set
  EXPECT_TRUE(sz.setZoneHumidificationDesignSupplyAirHumidityRatio(3.2));
  ASSERT_TRUE(sz.zoneHumidificationDesignSupplyAirHumidityRatio());
  EXPECT_EQ(3.2, sz.zoneHumidificationDesignSupplyAirHumidityRatio().get());
  // Bad Value
  EXPECT_FALSE(sz.setZoneHumidificationDesignSupplyAirHumidityRatio(-10.0));
  ASSERT_TRUE(sz.zoneHumidificationDesignSupplyAirHumidityRatio());
  EXPECT_EQ(3.2, sz.zoneHumidificationDesignSupplyAirHumidityRatio().get());

  // Zone Humidification Design Supply Air Humidity Ratio Difference: Double
  // Default value from IDD, set in Ctor
  EXPECT_EQ(0.005, sz.zoneHumidificationDesignSupplyAirHumidityRatioDifference());
  // Set
  EXPECT_TRUE(sz.setZoneHumidificationDesignSupplyAirHumidityRatioDifference(3.3));
  EXPECT_EQ(3.3, sz.zoneHumidificationDesignSupplyAirHumidityRatioDifference());
  // Bad Value
  EXPECT_FALSE(sz.setZoneHumidificationDesignSupplyAirHumidityRatioDifference(-10.0));
  EXPECT_EQ(3.3, sz.zoneHumidificationDesignSupplyAirHumidityRatioDifference());

  // Zone Humidistat Dehumidification Set Point Schedule Name: Optional Object
  ScheduleConstant dehumSch(m);
  EXPECT_TRUE(sz.setZoneHumidistatDehumidificationSetPointSchedule(dehumSch));
  ASSERT_TRUE(sz.zoneHumidistatDehumidificationSetPointSchedule());
  EXPECT_EQ(dehumSch, sz.zoneHumidistatDehumidificationSetPointSchedule().get());

  // Zone Humidistat Humidification Set Point Schedule Name: Optional Object
  ScheduleConstant humSch(m);
  EXPECT_TRUE(sz.setZoneHumidistatHumidificationSetPointSchedule(humSch));
  ASSERT_TRUE(sz.zoneHumidistatHumidificationSetPointSchedule());
  EXPECT_EQ(humSch, sz.zoneHumidistatHumidificationSetPointSchedule().get());

  // Sizing Option: String
  // Default value from IDD, set in Ctor
  EXPECT_EQ("Coincident", sz.sizingOption());
  // Set
  EXPECT_TRUE(sz.setSizingOption("NonCoincident"));
  EXPECT_EQ("NonCoincident", sz.sizingOption());
  // Bad Value
  EXPECT_FALSE(sz.setSizingOption("BADENUM"));
  EXPECT_EQ("NonCoincident", sz.sizingOption());
}
