/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../SizingSystem.hpp"
#include "../SizingSystem_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SizingSystem_SizingSystem) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirLoopHVAC airLoopHVAC(m);

      airLoopHVAC.sizingSystem();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  AirLoopHVAC airLoopHVAC(m);
  SizingSystem sizingSystem = airLoopHVAC.sizingSystem();
  EXPECT_EQ(sizingSystem.airLoopHVAC().handle(), airLoopHVAC.handle());

  EXPECT_EQ("Sensible", sizingSystem.typeofLoadtoSizeOn());
  EXPECT_TRUE(sizingSystem.isTypeofLoadtoSizeOnDefaulted());
  EXPECT_FALSE(sizingSystem.designOutdoorAirFlowRate());
  EXPECT_TRUE(sizingSystem.isDesignOutdoorAirFlowRateDefaulted());
  EXPECT_TRUE(sizingSystem.isDesignOutdoorAirFlowRateAutosized());
  ASSERT_TRUE(sizingSystem.centralHeatingMaximumSystemAirFlowRatio());
  EXPECT_EQ(0.3, sizingSystem.centralHeatingMaximumSystemAirFlowRatio().get());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioDefaulted());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioAutosized());
  EXPECT_EQ(7.0, sizingSystem.preheatDesignTemperature());
  EXPECT_EQ(0.008, sizingSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(12.8, sizingSystem.precoolDesignTemperature());
  EXPECT_EQ(0.008, sizingSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(12.8, sizingSystem.centralCoolingDesignSupplyAirTemperature());
  EXPECT_EQ(16.7, sizingSystem.centralHeatingDesignSupplyAirTemperature());
  EXPECT_EQ("NonCoincident", sizingSystem.sizingOption());
  EXPECT_TRUE(sizingSystem.isSizingOptionDefaulted());

  // Have to force these two to 'true' to preserve historical behavior, even though it was wrong
  EXPECT_TRUE(sizingSystem.allOutdoorAirinCooling());
  EXPECT_FALSE(sizingSystem.isAllOutdoorAirinCoolingDefaulted());
  EXPECT_TRUE(sizingSystem.allOutdoorAirinHeating());
  EXPECT_FALSE(sizingSystem.isAllOutdoorAirinHeatingDefaulted());

  EXPECT_EQ(0.0085, sizingSystem.centralCoolingDesignSupplyAirHumidityRatio());
  EXPECT_FALSE(sizingSystem.isCentralCoolingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ(0.008, sizingSystem.centralHeatingDesignSupplyAirHumidityRatio());
  EXPECT_TRUE(sizingSystem.isCentralHeatingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ("DesignDay", sizingSystem.coolingDesignAirFlowMethod());
  EXPECT_TRUE(sizingSystem.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(0.0, sizingSystem.coolingDesignAirFlowRate());
  EXPECT_TRUE(sizingSystem.isCoolingDesignAirFlowRateDefaulted());
  EXPECT_EQ("DesignDay", sizingSystem.heatingDesignAirFlowMethod());
  EXPECT_TRUE(sizingSystem.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(0.0, sizingSystem.heatingDesignAirFlowRate());
  EXPECT_TRUE(sizingSystem.isHeatingDesignAirFlowRateDefaulted());
  EXPECT_EQ("ZoneSum", sizingSystem.systemOutdoorAirMethod());
  EXPECT_TRUE(sizingSystem.isSystemOutdoorAirMethodDefaulted());
  EXPECT_EQ(1.0, sizingSystem.zoneMaximumOutdoorAirFraction());
  EXPECT_EQ(9.9676501E-3, sizingSystem.coolingSupplyAirFlowRatePerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.coolingFractionofAutosizedCoolingSupplyAirFlowRate());
  EXPECT_EQ(3.9475456E-5, sizingSystem.coolingSupplyAirFlowRatePerUnitCoolingCapacity());
  EXPECT_EQ(9.9676501E-3, sizingSystem.heatingSupplyAirFlowRatePerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.heatingFractionofAutosizedHeatingSupplyAirFlowRate());
  EXPECT_EQ(1.0, sizingSystem.heatingFractionofAutosizedCoolingSupplyAirFlowRate());
  EXPECT_EQ(3.1588213E-5, sizingSystem.heatingSupplyAirFlowRatePerUnitHeatingCapacity());
  EXPECT_EQ("CoolingDesignCapacity", sizingSystem.coolingDesignCapacityMethod());
  EXPECT_FALSE(sizingSystem.coolingDesignCapacity());
  EXPECT_TRUE(sizingSystem.isCoolingDesignCapacityAutosized());
  EXPECT_EQ(234.7, sizingSystem.coolingDesignCapacityPerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_EQ("HeatingDesignCapacity", sizingSystem.heatingDesignCapacityMethod());
  EXPECT_FALSE(sizingSystem.heatingDesignCapacity());
  EXPECT_TRUE(sizingSystem.isHeatingDesignCapacityAutosized());
  EXPECT_EQ(157.0, sizingSystem.heatingDesignCapacityPerFloorArea());
  EXPECT_EQ(1.0, sizingSystem.fractionofAutosizedHeatingDesignCapacity());
  EXPECT_EQ("OnOff", sizingSystem.centralCoolingCapacityControlMethod());
  EXPECT_FALSE(sizingSystem.occupantDiversity());
  EXPECT_TRUE(sizingSystem.isOccupantDiversityAutosized());
}

TEST_F(ModelFixture, SizingSystem_GettersSetters) {
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  EXPECT_TRUE(sizingSystem.setTypeofLoadtoSizeOn("VentilationRequirement"));
  EXPECT_TRUE(sizingSystem.setDesignOutdoorAirFlowRate(1));
  EXPECT_TRUE(sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(0.25));
  EXPECT_TRUE(sizingSystem.setPreheatDesignTemperature(2));
  EXPECT_TRUE(sizingSystem.setPreheatDesignHumidityRatio(3));
  EXPECT_TRUE(sizingSystem.setPrecoolDesignTemperature(4));
  EXPECT_TRUE(sizingSystem.setPrecoolDesignHumidityRatio(5));
  EXPECT_TRUE(sizingSystem.setCentralCoolingDesignSupplyAirTemperature(6));
  EXPECT_TRUE(sizingSystem.setCentralHeatingDesignSupplyAirTemperature(7));
  EXPECT_TRUE(sizingSystem.setSizingOption("Coincident"));
  EXPECT_TRUE(sizingSystem.setAllOutdoorAirinCooling(true));
  EXPECT_TRUE(sizingSystem.setAllOutdoorAirinHeating(true));
  EXPECT_TRUE(sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(8));
  EXPECT_TRUE(sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(9));
  EXPECT_TRUE(sizingSystem.setCoolingDesignAirFlowMethod("Flow/System"));
  EXPECT_TRUE(sizingSystem.setCoolingDesignAirFlowRate(10));
  EXPECT_TRUE(sizingSystem.setHeatingDesignAirFlowMethod("FlowPerFloorArea"));
  EXPECT_TRUE(sizingSystem.setHeatingDesignAirFlowRate(11));
  EXPECT_TRUE(sizingSystem.setSystemOutdoorAirMethod("Standard62.1VentilationRateProcedure"));
  EXPECT_TRUE(sizingSystem.setZoneMaximumOutdoorAirFraction(12));
  EXPECT_TRUE(sizingSystem.setCoolingSupplyAirFlowRatePerFloorArea(13));
  EXPECT_TRUE(sizingSystem.setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(14));
  EXPECT_TRUE(sizingSystem.setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(15));
  EXPECT_TRUE(sizingSystem.setHeatingSupplyAirFlowRatePerFloorArea(16));
  EXPECT_TRUE(sizingSystem.setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(17));
  EXPECT_TRUE(sizingSystem.setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(18));
  EXPECT_TRUE(sizingSystem.setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(19));
  EXPECT_TRUE(sizingSystem.setCoolingDesignCapacityMethod("CapacityPerFloorArea"));
  EXPECT_TRUE(sizingSystem.setCoolingDesignCapacity(20));
  EXPECT_TRUE(sizingSystem.setCoolingDesignCapacityPerFloorArea(21));
  EXPECT_TRUE(sizingSystem.setFractionofAutosizedCoolingDesignCapacity(22));
  EXPECT_TRUE(sizingSystem.setHeatingDesignCapacityMethod("FractionOfAutosizedHeatingCapacity"));
  EXPECT_TRUE(sizingSystem.setHeatingDesignCapacity(23));
  EXPECT_TRUE(sizingSystem.setHeatingDesignCapacityPerFloorArea(24));
  EXPECT_TRUE(sizingSystem.setFractionofAutosizedHeatingDesignCapacity(25));
  EXPECT_TRUE(sizingSystem.setCentralCoolingCapacityControlMethod("VAV"));
  EXPECT_TRUE(sizingSystem.setOccupantDiversity(0.5));

  EXPECT_EQ("VentilationRequirement", sizingSystem.typeofLoadtoSizeOn());
  EXPECT_FALSE(sizingSystem.isTypeofLoadtoSizeOnDefaulted());
  ASSERT_TRUE(sizingSystem.designOutdoorAirFlowRate());
  EXPECT_EQ(1, sizingSystem.designOutdoorAirFlowRate().get());
  EXPECT_FALSE(sizingSystem.isDesignOutdoorAirFlowRateDefaulted());
  EXPECT_FALSE(sizingSystem.isDesignOutdoorAirFlowRateAutosized());
  ASSERT_TRUE(sizingSystem.centralHeatingMaximumSystemAirFlowRatio());
  EXPECT_EQ(0.25, sizingSystem.centralHeatingMaximumSystemAirFlowRatio().get());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioDefaulted());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioAutosized());
  EXPECT_EQ(2, sizingSystem.preheatDesignTemperature());
  EXPECT_EQ(3, sizingSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(4, sizingSystem.precoolDesignTemperature());
  EXPECT_EQ(5, sizingSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(6, sizingSystem.centralCoolingDesignSupplyAirTemperature());
  EXPECT_EQ(7, sizingSystem.centralHeatingDesignSupplyAirTemperature());
  EXPECT_EQ("Coincident", sizingSystem.sizingOption());
  EXPECT_FALSE(sizingSystem.isSizingOptionDefaulted());
  EXPECT_TRUE(sizingSystem.allOutdoorAirinCooling());
  EXPECT_FALSE(sizingSystem.isAllOutdoorAirinCoolingDefaulted());
  EXPECT_TRUE(sizingSystem.allOutdoorAirinHeating());
  EXPECT_FALSE(sizingSystem.isAllOutdoorAirinHeatingDefaulted());
  EXPECT_EQ(8, sizingSystem.centralCoolingDesignSupplyAirHumidityRatio());
  EXPECT_FALSE(sizingSystem.isCentralCoolingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ(9, sizingSystem.centralHeatingDesignSupplyAirHumidityRatio());
  EXPECT_FALSE(sizingSystem.isCentralHeatingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ("Flow/System", sizingSystem.coolingDesignAirFlowMethod());
  EXPECT_FALSE(sizingSystem.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(10, sizingSystem.coolingDesignAirFlowRate());
  EXPECT_FALSE(sizingSystem.isCoolingDesignAirFlowRateDefaulted());
  EXPECT_EQ("FlowPerFloorArea", sizingSystem.heatingDesignAirFlowMethod());
  EXPECT_FALSE(sizingSystem.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(11, sizingSystem.heatingDesignAirFlowRate());
  EXPECT_FALSE(sizingSystem.isHeatingDesignAirFlowRateDefaulted());
  EXPECT_EQ("Standard62.1VentilationRateProcedure", sizingSystem.systemOutdoorAirMethod());
  EXPECT_FALSE(sizingSystem.isSystemOutdoorAirMethodDefaulted());
  EXPECT_EQ(12, sizingSystem.zoneMaximumOutdoorAirFraction());
  EXPECT_EQ(13, sizingSystem.coolingSupplyAirFlowRatePerFloorArea());
  EXPECT_EQ(14, sizingSystem.coolingFractionofAutosizedCoolingSupplyAirFlowRate());
  EXPECT_EQ(15, sizingSystem.coolingSupplyAirFlowRatePerUnitCoolingCapacity());
  EXPECT_EQ(16, sizingSystem.heatingSupplyAirFlowRatePerFloorArea());
  EXPECT_EQ(17, sizingSystem.heatingFractionofAutosizedHeatingSupplyAirFlowRate());
  EXPECT_EQ(18, sizingSystem.heatingFractionofAutosizedCoolingSupplyAirFlowRate());
  EXPECT_EQ(19, sizingSystem.heatingSupplyAirFlowRatePerUnitHeatingCapacity());
  EXPECT_EQ("CapacityPerFloorArea", sizingSystem.coolingDesignCapacityMethod());
  ASSERT_TRUE(sizingSystem.coolingDesignCapacity());
  EXPECT_EQ(20, sizingSystem.coolingDesignCapacity().get());
  EXPECT_FALSE(sizingSystem.isCoolingDesignCapacityAutosized());
  EXPECT_EQ(21, sizingSystem.coolingDesignCapacityPerFloorArea());
  EXPECT_EQ(22, sizingSystem.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_EQ("FractionOfAutosizedHeatingCapacity", sizingSystem.heatingDesignCapacityMethod());
  ASSERT_TRUE(sizingSystem.heatingDesignCapacity());
  EXPECT_EQ(23, sizingSystem.heatingDesignCapacity().get());
  EXPECT_FALSE(sizingSystem.isHeatingDesignCapacityAutosized());
  EXPECT_EQ(24, sizingSystem.heatingDesignCapacityPerFloorArea());
  EXPECT_EQ(25, sizingSystem.fractionofAutosizedHeatingDesignCapacity());
  EXPECT_EQ("VAV", sizingSystem.centralCoolingCapacityControlMethod());
  ASSERT_TRUE(sizingSystem.occupantDiversity());
  EXPECT_EQ(0.5, sizingSystem.occupantDiversity().get());
  EXPECT_FALSE(sizingSystem.isOccupantDiversityAutosized());

  sizingSystem.resetTypeofLoadtoSizeOn();
  sizingSystem.resetDesignOutdoorAirFlowRate();
  EXPECT_TRUE(sizingSystem.isDesignOutdoorAirFlowRateDefaulted());
  sizingSystem.autosizeDesignOutdoorAirFlowRate();
  sizingSystem.resetCentralHeatingMaximumSystemAirFlowRatio();
  EXPECT_TRUE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioDefaulted());
  sizingSystem.autosizeCentralHeatingMaximumSystemAirFlowRatio();
  sizingSystem.resetSizingOption();
  sizingSystem.resetAllOutdoorAirinCooling();
  sizingSystem.resetAllOutdoorAirinHeating();
  sizingSystem.resetCentralCoolingDesignSupplyAirHumidityRatio();
  sizingSystem.resetCentralHeatingDesignSupplyAirHumidityRatio();
  sizingSystem.resetCoolingDesignAirFlowMethod();
  sizingSystem.resetCoolingDesignAirFlowRate();
  sizingSystem.resetHeatingDesignAirFlowMethod();
  sizingSystem.resetHeatingDesignAirFlowRate();
  sizingSystem.resetSystemOutdoorAirMethod();
  sizingSystem.autosizeCoolingDesignCapacity();
  sizingSystem.autosizeHeatingDesignCapacity();
  sizingSystem.autosizeOccupantDiversity();

  EXPECT_EQ("Sensible", sizingSystem.typeofLoadtoSizeOn());
  EXPECT_TRUE(sizingSystem.isTypeofLoadtoSizeOnDefaulted());
  EXPECT_FALSE(sizingSystem.designOutdoorAirFlowRate());
  EXPECT_FALSE(sizingSystem.isDesignOutdoorAirFlowRateDefaulted());
  EXPECT_TRUE(sizingSystem.isDesignOutdoorAirFlowRateAutosized());
  EXPECT_FALSE(sizingSystem.centralHeatingMaximumSystemAirFlowRatio());
  EXPECT_FALSE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioDefaulted());
  EXPECT_TRUE(sizingSystem.isCentralHeatingMaximumSystemAirFlowRatioAutosized());
  EXPECT_EQ("NonCoincident", sizingSystem.sizingOption());
  EXPECT_TRUE(sizingSystem.isSizingOptionDefaulted());
  EXPECT_FALSE(sizingSystem.allOutdoorAirinCooling());
  EXPECT_TRUE(sizingSystem.isAllOutdoorAirinCoolingDefaulted());
  EXPECT_FALSE(sizingSystem.allOutdoorAirinHeating());
  EXPECT_TRUE(sizingSystem.isAllOutdoorAirinHeatingDefaulted());
  EXPECT_EQ(0.008, sizingSystem.centralCoolingDesignSupplyAirHumidityRatio());
  EXPECT_TRUE(sizingSystem.isCentralCoolingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ(0.008, sizingSystem.centralHeatingDesignSupplyAirHumidityRatio());
  EXPECT_TRUE(sizingSystem.isCentralHeatingDesignSupplyAirHumidityRatioDefaulted());
  EXPECT_EQ("DesignDay", sizingSystem.coolingDesignAirFlowMethod());
  EXPECT_TRUE(sizingSystem.isCoolingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(0.0, sizingSystem.coolingDesignAirFlowRate());
  EXPECT_TRUE(sizingSystem.isCoolingDesignAirFlowRateDefaulted());
  EXPECT_EQ("DesignDay", sizingSystem.heatingDesignAirFlowMethod());
  EXPECT_TRUE(sizingSystem.isHeatingDesignAirFlowMethodDefaulted());
  EXPECT_EQ(0.0, sizingSystem.heatingDesignAirFlowRate());
  EXPECT_TRUE(sizingSystem.isHeatingDesignAirFlowRateDefaulted());
  EXPECT_EQ("ZoneSum", sizingSystem.systemOutdoorAirMethod());
  EXPECT_TRUE(sizingSystem.isSystemOutdoorAirMethodDefaulted());
  EXPECT_FALSE(sizingSystem.coolingDesignCapacity());
  EXPECT_TRUE(sizingSystem.isCoolingDesignCapacityAutosized());
  EXPECT_FALSE(sizingSystem.heatingDesignCapacity());
  EXPECT_TRUE(sizingSystem.isHeatingDesignCapacityAutosized());
  EXPECT_FALSE(sizingSystem.occupantDiversity());
  EXPECT_TRUE(sizingSystem.isOccupantDiversityAutosized());
}

TEST_F(ModelFixture, SizingSystem_remove) {
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  auto size = m.modelObjects().size();
  EXPECT_FALSE(sizingSystem.remove().empty());
  EXPECT_EQ(size - 1, m.modelObjects().size());  // SizingSystem
  EXPECT_EQ(1u, m.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<SizingSystem>().size());
  EXPECT_THROW(airLoopHVAC.sizingSystem(), openstudio::Exception);
}

TEST_F(ModelFixture, SizingSystem_remove2) {
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  EXPECT_FALSE(airLoopHVAC.remove().empty());
  EXPECT_EQ(2, m.modelObjects().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<SizingSystem>().size());
}

TEST_F(ModelFixture, SizingSystem_clone) {
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  EXPECT_EQ(1u, m.getConcreteModelObjects<SizingSystem>().size());
  auto sizingSystemClone = sizingSystem.clone(m).cast<SizingSystem>();
  EXPECT_EQ(1u, m.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<SizingSystem>().size());

  // SizingSystem and its clone referencing the same AirLoopHVAC
  EXPECT_EQ(sizingSystemClone.airLoopHVAC().handle(), airLoopHVAC.handle());
  EXPECT_EQ(sizingSystem.airLoopHVAC().handle(), airLoopHVAC.handle());

  // FIXME: AirLoopHVAC returns the original SizingSystem OR the cloned SizingSystem
  // EXPECT_EQ(airLoopHVAC.sizingSystem().handle(), sizingSystem.handle());
  // EXPECT_NE(airLoopHVAC.sizingSystem().handle(), sizingSystemClone.handle());
}

TEST_F(ModelFixture, SizingSystem_ctor) {
  Model m;
  AirLoopHVAC airLoopHVAC(m);
  SizingSystem sizingSystem1 = airLoopHVAC.sizingSystem();
  SizingSystem sizingSystem2(m, airLoopHVAC);
  EXPECT_NE(sizingSystem1, sizingSystem2);

  // FIXME: AirLoopHVAC returns the original SizingSystem OR the new SizingSystem
  // EXPECT_EQ(airLoopHVAC.sizingSystem().handle(), sizingSystem1.handle());
  // EXPECT_NE(airLoopHVAC.sizingSystem().handle(), sizingSystem2.handle());
}
