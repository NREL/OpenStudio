/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"

#include "../WaterHeaterHeatPump.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../Schedule.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingElectric.hpp"

using namespace openstudio;
using namespace openstudio::model;

CoilSystemIntegratedHeatPumpAirSource makeCoilSystem(const Model& m) {

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);

  return CoilSystemIntegratedHeatPumpAirSource(m, spaceCoolingCoil, spaceHeatingCoil, dedicatedWaterHeatingCoil, scwhCoil, scdwhCoolingCoil,
                                               scdwhWaterHeatingCoil, shdwhHeatingCoil, shdwhWaterHeatingCoil);
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_CoilSystemIntegratedHeatPumpAirSource) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
      CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
      CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
      CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);

      CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil, dedicatedWaterHeatingCoil, scwhCoil, scdwhCoolingCoil,
                                                       scdwhWaterHeatingCoil, shdwhHeatingCoil, shdwhWaterHeatingCoil);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil, dedicatedWaterHeatingCoil, scwhCoil, scdwhCoolingCoil,
                                                   scdwhWaterHeatingCoil, shdwhHeatingCoil, shdwhWaterHeatingCoil);

  EXPECT_EQ(spaceCoolingCoil, coilSystem.spaceCoolingCoil());
  EXPECT_EQ(spaceHeatingCoil, coilSystem.spaceHeatingCoil());
  EXPECT_EQ(dedicatedWaterHeatingCoil, coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_EQ(scwhCoil, coilSystem.scwhCoil());
  EXPECT_EQ(scdwhCoolingCoil, coilSystem.scdwhCoolingCoil());
  EXPECT_EQ(scdwhWaterHeatingCoil, coilSystem.scdwhWaterHeatingCoil());
  EXPECT_EQ(shdwhHeatingCoil, coilSystem.shdwhHeatingCoil());
  EXPECT_EQ(shdwhWaterHeatingCoil, coilSystem.shdwhWaterHeatingCoil());

  EXPECT_EQ(20.0, coilSystem.indoorTemperatureLimitForSCWHMode());
  EXPECT_EQ(27.0, coilSystem.ambientTemperatureLimitForSCWHMode());
  EXPECT_EQ(20.0, coilSystem.indoorTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(20.0, coilSystem.ambientTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(0, coilSystem.flagtoIndicateLoadControlInSCWHMode());
  EXPECT_EQ(1, coilSystem.minimumSpeedLevelForSCWHMode());
  EXPECT_EQ(0.0, coilSystem.maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode());
  EXPECT_EQ(1, coilSystem.minimumSpeedLevelForSCDWHMode());
  EXPECT_EQ(360.0, coilSystem.maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode());
  EXPECT_EQ(1, coilSystem.minimumSpeedLevelForSHDWHMode());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_SetGetFields) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem = makeCoilSystem(m);

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);

  // Test coil setters and getters
  EXPECT_TRUE(coilSystem.setSpaceCoolingCoil(spaceCoolingCoil));
  EXPECT_TRUE(coilSystem.setSpaceHeatingCoil(spaceHeatingCoil));
  EXPECT_TRUE(coilSystem.setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSCWHCoil(scwhCoil));
  EXPECT_TRUE(coilSystem.setSCDWHCoolingCoil(scdwhCoolingCoil));
  EXPECT_TRUE(coilSystem.setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHHeatingCoil(shdwhHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil));
  EXPECT_EQ(spaceCoolingCoil, coilSystem.spaceCoolingCoil());
  EXPECT_EQ(spaceHeatingCoil, coilSystem.spaceHeatingCoil());
  EXPECT_EQ(dedicatedWaterHeatingCoil, coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_EQ(scwhCoil, coilSystem.scwhCoil());
  EXPECT_EQ(scdwhCoolingCoil, coilSystem.scdwhCoolingCoil());
  EXPECT_EQ(scdwhWaterHeatingCoil, coilSystem.scdwhWaterHeatingCoil());
  EXPECT_EQ(shdwhHeatingCoil, coilSystem.shdwhHeatingCoil());
  EXPECT_EQ(shdwhWaterHeatingCoil, coilSystem.shdwhWaterHeatingCoil());

  // Test containingHVACComponent
  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent());
  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, spaceCoolingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, spaceHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(dedicatedWaterHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(dedicatedWaterHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, dedicatedWaterHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(scwhCoil.containingHVACComponent());
  ASSERT_TRUE(scwhCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, scwhCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(scdwhCoolingCoil.containingHVACComponent());
  ASSERT_TRUE(scdwhCoolingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, scdwhCoolingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(scdwhWaterHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(scdwhWaterHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, scdwhWaterHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(shdwhHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(shdwhHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, shdwhHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(shdwhWaterHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(shdwhWaterHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, shdwhWaterHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  // Test regular fields
  EXPECT_TRUE(coilSystem.setIndoorTemperatureLimitForSCWHMode(21.0));
  EXPECT_TRUE(coilSystem.setAmbientTemperatureLimitForSCWHMode(28.0));
  EXPECT_TRUE(coilSystem.setIndoorTemperatureAboveWhichWHHasHigherPriority(22.0));
  EXPECT_TRUE(coilSystem.setAmbientTemperatureAboveWhichWHHasHigherPriority(23.0));
  EXPECT_TRUE(coilSystem.setFlagtoIndicateLoadControlInSCWHMode(1));
  EXPECT_TRUE(coilSystem.setMinimumSpeedLevelForSCWHMode(2));
  EXPECT_TRUE(coilSystem.setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(1.0));
  EXPECT_TRUE(coilSystem.setMinimumSpeedLevelForSCDWHMode(3));
  EXPECT_TRUE(coilSystem.setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(380.0));
  EXPECT_TRUE(coilSystem.setMinimumSpeedLevelForSHDWHMode(4));

  EXPECT_EQ(21.0, coilSystem.indoorTemperatureLimitForSCWHMode());
  EXPECT_EQ(28.0, coilSystem.ambientTemperatureLimitForSCWHMode());
  EXPECT_EQ(22.0, coilSystem.indoorTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(23.0, coilSystem.ambientTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(1, coilSystem.flagtoIndicateLoadControlInSCWHMode());
  EXPECT_EQ(2, coilSystem.minimumSpeedLevelForSCWHMode());
  EXPECT_EQ(1.0, coilSystem.maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode());
  EXPECT_EQ(3, coilSystem.minimumSpeedLevelForSCDWHMode());
  EXPECT_EQ(380.0, coilSystem.maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode());
  EXPECT_EQ(4, coilSystem.minimumSpeedLevelForSHDWHMode());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Clone) {
  Model m;
  CoilSystemIntegratedHeatPumpAirSource coilSystem = makeCoilSystem(m);

  size_t nCoilCoolingDXVariableSpeed = 2;
  size_t nCoilHeatingDXVariableSpeed = 2;
  size_t nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed = 4;
  EXPECT_EQ(nCoilCoolingDXVariableSpeed, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilHeatingDXVariableSpeed, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

  // Cloning clones the coils too.

  {
    auto coilSystemClone = coilSystem.clone(m).cast<CoilSystemIntegratedHeatPumpAirSource>();

    EXPECT_EQ(2 * nCoilCoolingDXVariableSpeed, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
    EXPECT_EQ(2 * nCoilHeatingDXVariableSpeed, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
    EXPECT_EQ(2 * nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed,
              m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

    // It should have reassigned to the cloned coils too
    EXPECT_NE(coilSystem.spaceCoolingCoil(), coilSystemClone.spaceCoolingCoil());
    EXPECT_NE(coilSystem.spaceHeatingCoil(), coilSystemClone.spaceHeatingCoil());
    EXPECT_NE(coilSystem.dedicatedWaterHeatingCoil(), coilSystemClone.dedicatedWaterHeatingCoil());
    EXPECT_NE(coilSystem.scwhCoil(), coilSystemClone.scwhCoil());
    EXPECT_NE(coilSystem.scdwhCoolingCoil(), coilSystemClone.scdwhCoolingCoil());
    EXPECT_NE(coilSystem.scdwhWaterHeatingCoil(), coilSystemClone.scdwhWaterHeatingCoil());
    EXPECT_NE(coilSystem.shdwhHeatingCoil(), coilSystemClone.shdwhHeatingCoil());
    EXPECT_NE(coilSystem.shdwhWaterHeatingCoil(), coilSystemClone.shdwhWaterHeatingCoil());
  }

  {
    Model m2;
    auto coilSystemClone = coilSystem.clone(m2).cast<CoilSystemIntegratedHeatPumpAirSource>();

    EXPECT_EQ(nCoilCoolingDXVariableSpeed, m2.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
    EXPECT_EQ(nCoilHeatingDXVariableSpeed, m2.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
    EXPECT_EQ(nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed, m2.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

    EXPECT_NO_THROW(coilSystemClone.spaceCoolingCoil());
    EXPECT_NO_THROW(coilSystemClone.spaceHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.dedicatedWaterHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.scwhCoil());
    EXPECT_NO_THROW(coilSystemClone.scdwhCoolingCoil());
    EXPECT_NO_THROW(coilSystemClone.scdwhWaterHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.shdwhHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.shdwhWaterHeatingCoil());
  }
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Remove) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem = makeCoilSystem(m);

  size_t nCoilCoolingDXVariableSpeed = 2;
  size_t nCoilHeatingDXVariableSpeed = 2;
  size_t nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed = 4;
  EXPECT_EQ(nCoilCoolingDXVariableSpeed, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilHeatingDXVariableSpeed, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

  EXPECT_FALSE(coilSystem.remove().empty());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_DefaultConstructor) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  EXPECT_TRUE(coilSystem.spaceCoolingCoil().optionalCast<CoilCoolingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.spaceHeatingCoil().optionalCast<CoilHeatingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.dedicatedWaterHeatingCoil().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
  EXPECT_TRUE(coilSystem.scwhCoil().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
  EXPECT_TRUE(coilSystem.scdwhCoolingCoil().optionalCast<CoilCoolingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.scdwhWaterHeatingCoil().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
  EXPECT_TRUE(coilSystem.shdwhHeatingCoil().optionalCast<CoilHeatingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.shdwhWaterHeatingCoil().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_containingHVACComponent_AirLoopHVACUnitaryHeatPumpAirToAir) {

  Model m;

  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);
  CoilHeatingElectric coilHeatingElectric(m, s);

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);
  EXPECT_FALSE(coilSystem.containingHVACComponent());

  AirLoopHVACUnitaryHeatPumpAirToAir unitary(m, s, supplyFan, coilSystem, coilSystem, coilHeatingElectric);
  EXPECT_EQ(coilSystem.handle(), unitary.coolingCoil().handle());
  EXPECT_EQ(coilSystem.handle(), unitary.heatingCoil().handle());

  // Test containingHVAC
  ASSERT_TRUE(coilSystem.containingHVACComponent());
  EXPECT_EQ(unitary.handle(), coilSystem.containingHVACComponent()->handle());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_containingHVACComponent_WaterHeaterHeatPumpPumpedCondenser) {

  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);
  EXPECT_FALSE(coilSystem.containingHVACComponent());

  WaterHeaterHeatPump hpwh(m);
  EXPECT_TRUE(hpwh.setDXCoil(coilSystem));
  EXPECT_EQ(coilSystem.handle(), hpwh.dXCoil().handle());

  // Test containingHVAC
  ASSERT_TRUE(coilSystem.containingHVACComponent());
  EXPECT_EQ(hpwh.handle(), coilSystem.containingHVACComponent()->handle());
}
