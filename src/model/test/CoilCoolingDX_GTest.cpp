/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>
#include "ModelFixture.hpp"

#include "../CoilCoolingDX.hpp"
#include "../CoilCoolingDX_Impl.hpp"

#include "../CoilCoolingDXCurveFitPerformance.hpp"
#include "../CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../AirLoopHVACUnitarySystem.hpp"
#include "../AirLoopHVACUnitarySystem_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ScheduleCompact.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingWater.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDX_CoilCoolingDX) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit operating mode object to use
      CoilCoolingDXCurveFitOperatingMode operatingMode(model);

      // create a coil cooling dx curve fit performance object to use
      CoilCoolingDXCurveFitPerformance performance(model, operatingMode);

      // create a coil cooling dx object to use
      CoilCoolingDX dx(model, performance);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a coil cooling dx curve fit operating mode object to use
  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  // create a coil cooling dx curve fit performance object to use
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);

  // create a coil cooling dx object to use
  CoilCoolingDX dx(model, performance);

  auto alwaysOn = model.alwaysOnDiscreteSchedule();

  EXPECT_EQ(alwaysOn, dx.availabilitySchedule());
  ASSERT_FALSE(dx.condenserZone());
  ASSERT_TRUE(dx.performanceObject().optionalCast<CoilCoolingDXCurveFitPerformance>());
  // EXPECT_FALSE(dx.condenserInletNodeName());
  // EXPECT_FALSE(dx.condenserOutletNodeName());
  // ASSERT_FALSE(dx.condensateCollectionWaterStorageTankName());
  // ASSERT_FALSE(dx.evaporativeCondenserSupplyWaterStorageTankName());
}

TEST_F(ModelFixture, CoilCoolingDX_GettersSetters) {
  // create a model to use
  Model model;

  // create a coil cooling dx curve fit operating mode object to use
  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  // create a coil cooling dx curve fit performance object to use
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);

  // create a coil cooling dx object to use
  CoilCoolingDX dx(model, performance);

  Schedule schedule = dx.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  ASSERT_TRUE(scheduleConstant);
  EXPECT_FALSE(dx.condenserZone());
  // EXPECT_FALSE(dx.condenserInletNodeName());
  // EXPECT_FALSE(dx.condenserOutletNodeName());
  CoilCoolingDXCurveFitPerformance performanceObject = dx.performanceObject();
  EXPECT_EQ(performanceObject.name().get(), performance.name().get());

  ScheduleConstant scheduleConstant2(model);
  scheduleConstant2.setValue(0.5);
  dx.setAvailabilitySchedule(scheduleConstant2);
  ThermalZone thermalZone(model);
  dx.setCondenserZone(thermalZone);
  // dx.setCondenserInletNodeName("Node 1");
  // dx.setCondenserOutletNodeName("Node 2");
  CoilCoolingDXCurveFitPerformance performance2(model, operatingMode);
  dx.setPerformanceObject(performance2);

  Schedule schedule2 = dx.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant3 = schedule2.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant3);
  EXPECT_EQ((*scheduleConstant3).value(), 0.5);
  ASSERT_TRUE(dx.condenserZone());
  // ASSERT_TRUE(dx.condenserInletNodeName());
  // EXPECT_EQ(dx.condenserInletNodeName().get(), "Node 1");
  // ASSERT_TRUE(dx.condenserOutletNodeName());
  // EXPECT_EQ(dx.condenserOutletNodeName().get(), "Node 2");
  CoilCoolingDXCurveFitPerformance performanceObject2 = dx.performanceObject();
  EXPECT_EQ(performanceObject2.name().get(), performance2.name().get());

  dx.resetCondenserZone();
  EXPECT_FALSE(dx.condenserZone());

  // dx.resetCondenserInletNodeName();
  // EXPECT_FALSE(dx.condenserInletNodeName());
  // dx.resetCondenserOutletNodeName();
  // EXPECT_FALSE(dx.condenserOutletNodeName());
}

TEST_F(ModelFixture, CoilCoolingDX_containingHVACComponent) {
  {  // AirLoopHVACUnitarySystem
    Model model;

    CoilCoolingDXCurveFitOperatingMode operatingMode(model);
    CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
    CoilCoolingDX dx(model, performance);
    EXPECT_TRUE(dx.isRemovable());

    AirLoopHVACUnitarySystem unitary(model);
    EXPECT_TRUE(unitary.setCoolingCoil(dx));
    ASSERT_TRUE(unitary.coolingCoil());
    EXPECT_EQ(dx, unitary.coolingCoil().get());

    ASSERT_TRUE(dx.containingHVACComponent());
    EXPECT_EQ(unitary, dx.containingHVACComponent().get());

    // Shouldn't be able to remove DX since it's contained in a HVAC Component
    EXPECT_FALSE(dx.isRemovable());
    auto rmed = dx.remove();
    EXPECT_EQ(0u, rmed.size());
    ASSERT_TRUE(unitary.coolingCoil());
    EXPECT_EQ(dx, unitary.coolingCoil().get());
  }

  {  // CoilSystemCoolingDXHeatExchangerAssisted
    Model model;

    CoilCoolingDXCurveFitOperatingMode operatingMode(model);
    CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
    CoilCoolingDX dx(model, performance);
    EXPECT_TRUE(dx.isRemovable());

    CoilSystemCoolingDXHeatExchangerAssisted hx(model);
    EXPECT_TRUE(hx.setCoolingCoil(dx));
    EXPECT_EQ(dx, hx.coolingCoil());

    ASSERT_TRUE(dx.containingHVACComponent());
    EXPECT_EQ(hx, dx.containingHVACComponent().get());

    // Shouldn't be able to remove DX since it's contained in a HVAC Component
    EXPECT_FALSE(dx.isRemovable());
    auto rmed = dx.remove();
    EXPECT_EQ(0u, rmed.size());
    EXPECT_EQ(dx, hx.coolingCoil());
  }
}

TEST_F(ModelFixture, CoilCoolingDX_containingZoneHVACComponent) {
  {  // ZoneHVACPackagedTerminalAirConditioner
    Model model;

    CoilCoolingDXCurveFitOperatingMode operatingMode(model);
    CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
    CoilCoolingDX dx(model, performance);
    EXPECT_TRUE(dx.isRemovable());
    ScheduleCompact availabilitySchedule(model);
    FanConstantVolume fan(model, availabilitySchedule);
    CoilHeatingWater heatingCoil(model, availabilitySchedule);

    ZoneHVACPackagedTerminalAirConditioner ptac(model, availabilitySchedule, fan, heatingCoil, dx);
    EXPECT_EQ(dx, ptac.coolingCoil());

    ASSERT_TRUE(dx.containingZoneHVACComponent());
    EXPECT_EQ(ptac, dx.containingZoneHVACComponent().get());

    // Shouldn't be able to remove DX since it's contained in a HVAC Component
    EXPECT_FALSE(dx.isRemovable());
    auto rmed = dx.remove();
    EXPECT_EQ(0u, rmed.size());
    EXPECT_EQ(dx, ptac.coolingCoil());
  }

  {  // ZoneHVACPackagedTerminalHeatPump
    Model model;

    CoilCoolingDXCurveFitOperatingMode operatingMode(model);
    CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
    CoilCoolingDX dx(model, performance);
    EXPECT_TRUE(dx.isRemovable());
    ScheduleCompact availabilitySchedule(model);
    FanConstantVolume fan(model, availabilitySchedule);
    CoilHeatingDXSingleSpeed heatingCoil(model);
    CoilHeatingElectric supplementalHeatingCoil(model, availabilitySchedule);

    ZoneHVACPackagedTerminalHeatPump pthp(model, availabilitySchedule, fan, heatingCoil, dx, supplementalHeatingCoil);
    EXPECT_EQ(dx, pthp.coolingCoil());

    ASSERT_TRUE(dx.containingZoneHVACComponent());
    EXPECT_EQ(pthp, dx.containingZoneHVACComponent().get());

    // Shouldn't be able to remove DX since it's contained in a HVAC Component
    EXPECT_FALSE(dx.isRemovable());
    auto rmed = dx.remove();
    EXPECT_EQ(0u, rmed.size());
    EXPECT_EQ(dx, pthp.coolingCoil());
  }
}

TEST_F(ModelFixture, CoilCoolingDX_addToNode) {
  Model m;

  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX dx(m, performance);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(dx.airLoopHVAC());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_TRUE(dx.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(dx.airLoopHVAC());

  Node demandNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(dx.addToNode(demandNode));
  // 5u: inlet splitter node mixer outlet.
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(dx.airLoopHVAC());

  EXPECT_TRUE(dx.removeFromLoop());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_FALSE(dx.airLoopHVAC());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(dx.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(dx.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, CoilCoolingDX_performanceObject) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
  CoilCoolingDX dx(model, performance);

  EXPECT_EQ(performance, dx.performanceObject());
  CoilCoolingDXCurveFitPerformance performance2(model, operatingMode);
  EXPECT_TRUE(dx.setPerformanceObject(performance2));
  EXPECT_EQ(performance2, dx.performanceObject());
}

TEST_F(ModelFixture, CoilCoolingDX_clone) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
  CoilCoolingDX dx(model, performance);

  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  auto dxClone = dx.clone(model).cast<CoilCoolingDX>();
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(performance, dxClone.performanceObject());

  dx.remove();
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(performance, dxClone.performanceObject());

  dxClone.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
}

TEST_F(ModelFixture, CoilCoolingDX_cloneParent) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
  CoilCoolingDX dx(model, performance);

  AirLoopHVACUnitarySystem unitary(model);
  EXPECT_TRUE(unitary.setCoolingCoil(dx));

  ASSERT_TRUE(dx.containingHVACComponent());
  EXPECT_EQ(unitary, dx.containingHVACComponent().get());

  ASSERT_TRUE(unitary.coolingCoil());
  EXPECT_EQ(dx, unitary.coolingCoil().get());

  EXPECT_EQ(1u, model.getConcreteModelObjects<AirLoopHVACUnitarySystem>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  // Cloning the unitary should clone the coolingCoil, but the clone of the DX should still share resources
  auto unitaryClone = unitary.clone(model).cast<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2u, model.getConcreteModelObjects<AirLoopHVACUnitarySystem>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
  EXPECT_EQ(dx, unitary.coolingCoil().get());
  EXPECT_NE(dx, unitaryClone.coolingCoil().get());
}

TEST_F(ModelFixture, CoilCoolingDX_cloneOtherModel) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
  CoilCoolingDX dx(model, performance);

  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  Model model2;
  auto dxClone = dx.clone(model2).cast<CoilCoolingDX>();
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  EXPECT_EQ(1u, model2.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model2.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model2.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_NE(performance, dxClone.performanceObject());
}
