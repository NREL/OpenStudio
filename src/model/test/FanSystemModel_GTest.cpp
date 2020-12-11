/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../FanSystemModel.hpp"
#include "../FanSystemModel_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../Curve.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveExponent.hpp"
#include "../CurveExponent_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../HVACComponent.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlow.hpp"

#include "../AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../AirLoopHVACUnitarySystem.hpp"

#include "../AirflowNetworkFan.hpp"

#include "../AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../AirTerminalSingleDuctParallelPIUReheat.hpp"

#include "../WaterHeaterHeatPump.hpp"
#include "../WaterHeaterHeatPumpWrappedCondenser.hpp"

#include "../ZoneHVACEnergyRecoveryVentilator.hpp"
#include "../ZoneHVACFourPipeFanCoil.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACUnitHeater.hpp"
#include "../ZoneHVACUnitVentilator.hpp"
#include "../ZoneHVACWaterToAirHeatPump.hpp"

// Stuff that's not supported
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../FanOnOff.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanSystemModel_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      FanSystemModel testObject = FanSystemModel(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, FanSystemModel_GettersSetters) {
  Model m;
  FanSystemModel fan(m);

  fan.setName("My FanSystemModel");

  // Availability Schedule Name: Required Object
  ScheduleConstant sch(m);
  EXPECT_TRUE(fan.setAvailabilitySchedule(sch));
  EXPECT_EQ(sch, fan.availabilitySchedule());

  // Design Maximum Air Flow Rate: Required Double
  fan.autosizeDesignMaximumAirFlowRate();
  EXPECT_TRUE(fan.isDesignMaximumAirFlowRateAutosized());
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(0.1));
  ASSERT_TRUE(fan.designMaximumAirFlowRate());
  EXPECT_EQ(0.1, fan.designMaximumAirFlowRate().get());
  EXPECT_FALSE(fan.isDesignMaximumAirFlowRateAutosized());

  // Speed Control Method: Required String
  EXPECT_TRUE(fan.setSpeedControlMethod("Continuous"));
  EXPECT_EQ("Continuous", fan.speedControlMethod());
  EXPECT_TRUE(fan.setSpeedControlMethod("Discrete"));
  EXPECT_EQ("Discrete", fan.speedControlMethod());
  EXPECT_FALSE(fan.setSpeedControlMethod("BADENUM"));
  EXPECT_EQ("Discrete", fan.speedControlMethod());

  // Electric Power Minimum Flow Rate Fraction: Required Double
  EXPECT_TRUE(fan.setElectricPowerMinimumFlowRateFraction(0.775));
  EXPECT_EQ(0.775, fan.electricPowerMinimumFlowRateFraction());

  // Design Pressure Rise: Required Double
  EXPECT_TRUE(fan.setDesignPressureRise(121.5));
  EXPECT_EQ(121.5, fan.designPressureRise());

  // Motor Efficiency: Required Double
  EXPECT_TRUE(fan.setMotorEfficiency(0.54));
  EXPECT_EQ(0.54, fan.motorEfficiency());

  // Motor In Air Stream Fraction: Required Double
  EXPECT_TRUE(fan.setMotorInAirStreamFraction(0.87));
  EXPECT_EQ(0.87, fan.motorInAirStreamFraction());

  // Design Electric Power Consumption: Required Double
  fan.autosizeDesignElectricPowerConsumption();
  EXPECT_TRUE(fan.isDesignElectricPowerConsumptionAutosized());
  EXPECT_TRUE(fan.setDesignElectricPowerConsumption(3112.8));
  ASSERT_TRUE(fan.designElectricPowerConsumption());
  EXPECT_EQ(3112.8, fan.designElectricPowerConsumption().get());
  EXPECT_FALSE(fan.isDesignElectricPowerConsumptionAutosized());

  // Design Power Sizing Method: Required String
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("PowerPerFlow"));
  EXPECT_EQ("PowerPerFlow", fan.designPowerSizingMethod());
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("TotalEfficiencyAndPressure"));
  EXPECT_EQ("TotalEfficiencyAndPressure", fan.designPowerSizingMethod());
  EXPECT_FALSE(fan.setDesignPowerSizingMethod("BADENUM"));
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("TotalEfficiencyAndPressure"));

  // Electric Power Per Unit Flow Rate: Required Double
  EXPECT_TRUE(fan.setElectricPowerPerUnitFlowRate(1254.0));
  EXPECT_EQ(1254.0, fan.electricPowerPerUnitFlowRate());

  // Electric Power Per Unit Flow Rate Per Unit Pressure: Required Double
  EXPECT_TRUE(fan.setElectricPowerPerUnitFlowRatePerUnitPressure(1.345));
  EXPECT_EQ(1.345, fan.electricPowerPerUnitFlowRatePerUnitPressure());

  // Fan Total Efficiency: Required Double
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.59));
  EXPECT_EQ(0.59, fan.fanTotalEfficiency());

  // Electric Power Function of Flow Fraction Curve Name: Optional Object
  CurveCubic fanPowerFuncFlowCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));
  ASSERT_TRUE(fan.electricPowerFunctionofFlowFractionCurve());
  EXPECT_EQ(fanPowerFuncFlowCurve, fan.electricPowerFunctionofFlowFractionCurve().get());
  fan.resetElectricPowerFunctionofFlowFractionCurve();
  EXPECT_FALSE(fan.electricPowerFunctionofFlowFractionCurve());
  CurveBiquadratic badCurve(m);
  EXPECT_FALSE(fan.setElectricPowerFunctionofFlowFractionCurve(badCurve));
  EXPECT_FALSE(fan.electricPowerFunctionofFlowFractionCurve());

  // Night Ventilation Mode Pressure Rise: Optional Double
  EXPECT_TRUE(fan.setNightVentilationModePressureRise(356.0));
  ASSERT_TRUE(fan.nightVentilationModePressureRise());
  EXPECT_EQ(356.0, fan.nightVentilationModePressureRise().get());

  // Night Ventilation Mode Flow Fraction: Optional Double
  EXPECT_TRUE(fan.setNightVentilationModeFlowFraction(0.37));
  ASSERT_TRUE(fan.nightVentilationModeFlowFraction());
  EXPECT_EQ(0.37, fan.nightVentilationModeFlowFraction().get());

  // Motor Loss Zone Name: Optional Object
  ThermalZone z(m);
  EXPECT_TRUE(fan.setMotorLossZone(z));
  ASSERT_TRUE(fan.motorLossZone());
  EXPECT_EQ(z, fan.motorLossZone().get());

  // Motor Loss Radiative Fraction: Required Double
  EXPECT_TRUE(fan.setMotorLossRadiativeFraction(0.15));
  EXPECT_EQ(0.15, fan.motorLossRadiativeFraction());

  // End-Use Subcategory: Required String
  EXPECT_TRUE(fan.setEndUseSubcategory("My Fan"));
  EXPECT_EQ("My Fan", fan.endUseSubcategory());

  // Speed is tested in its own test

}  // End of Getter_Setters test

// OS:AirLoopHVAC
TEST_F(ModelFixture, FanSystemModel_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(fan.airLoopHVAC());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_TRUE(fan.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(fan.airLoopHVAC());
  ASSERT_TRUE(airLoop.supplyFan());
  EXPECT_EQ(fan, airLoop.supplyFan().get());

  Node demandNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(fan.addToNode(demandNode));
  // 5u: inlet splitter node mixer outlet.
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(fan.airLoopHVAC());

  EXPECT_TRUE(fan.removeFromLoop());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_FALSE(fan.airLoopHVAC());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(fan.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(fan.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
}

/********************************************************************************************************************
*                                        H V A C    C O M P O N E N T S                                             *
********************************************************************************************************************/

// OS:AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass
TEST_F(ModelFixture, FanSystemModel_containingHVACComponent_AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);
  CoilHeatingElectric heatingCoil(m, s);
  CoilCoolingDXSingleSpeed coolingCoil(m);
  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, fan, coolingCoil, heatingCoil);

  boost::optional<HVACComponent> component = fan.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), unitary.handle());
}

// OS:AirLoopHVAC:UnitarySystem
TEST_F(ModelFixture, FanSystemModel_containingHVACComponent_AirLoopHVACUnitarySystem) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem unitary(m);
  FanSystemModel fan(m);
  CoilHeatingElectric heatingCoil(m);
  CoilHeatingElectric suppHeatingCoil(m);
  CoilCoolingWater coolingCoil(m);

  EXPECT_TRUE(unitary.setSupplyFan(fan));
  boost::optional<HVACComponent> component = fan.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), unitary.handle());
}

// OS:AirTerminal:SingleDuct:SeriesPIU:Reheat
TEST_F(ModelFixture, FanSystemModel_containingHVACComponent_AirTerminalSingleDuctSeriesPIUReheat) {
  Model m;
  FanSystemModel fan(m);
  CoilHeatingElectric reheatCoil(m);
  AirTerminalSingleDuctSeriesPIUReheat piu(m, fan, reheatCoil);

  boost::optional<HVACComponent> component = fan.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), piu.handle());
}

// OS:AirTerminal:SingleDuct:ParallelPIU:Reheat
TEST_F(ModelFixture, FanSystemModel_containingHVACComponent_AirTerminalSingleDuctParallelPIUReheat) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);
  CoilHeatingElectric reheatCoil(m);
  AirTerminalSingleDuctParallelPIUReheat piu(m, s, fan, reheatCoil);

  boost::optional<HVACComponent> component = fan.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), piu.handle());
}

/********************************************************************************************************************
*                                 Z O N E    H V A C    C O M P O N E N T S                                         *
********************************************************************************************************************/

//OS:WaterHeater:HeatPump
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_WaterHeaterHeatPump) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  WaterHeaterHeatPump hpwh(m);

  EXPECT_TRUE(hpwh.setFan(fan));

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), hpwh.handle());
}
//OS:WaterHeater:HeatPump:WrappedCondenser
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_WaterHeaterHeatPumpWrappedCondenser) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  WaterHeaterHeatPumpWrappedCondenser hpwh(m);

  EXPECT_TRUE(hpwh.setFan(fan));

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), hpwh.handle());
}

// OS:ZoneHVAC:EnergyRecoveryVentilator
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACEnergyRecoveryVentilator) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  ZoneHVACEnergyRecoveryVentilator zoneHVACEnergyRecoveryVentilator(m);

  EXPECT_TRUE(zoneHVACEnergyRecoveryVentilator.setSupplyAirFan(fan));

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACEnergyRecoveryVentilator.handle());
}

// OS:ZoneHVAC:FourPipeFanCoil
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACFourPipeFanCoil) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);

  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(m, s, fan, coolingCoil, heatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACFourPipeFanCoil.handle());
}

// OS:ZoneHVAC:PackagedTerminalAirConditioner
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACPackagedTerminalAirConditioner) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);
  CoilHeatingDXSingleSpeed heatingCoil(m);
  CoilCoolingDXSingleSpeed coolingCoil(m);

  ZoneHVACPackagedTerminalAirConditioner zoneHVACPackagedTerminalAirConditioner(m, s, fan, heatingCoil, coolingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACPackagedTerminalAirConditioner.handle());
}

// OS:ZoneHVAC:PackagedTerminalHeatPump
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACPackagedTerminalHeatPump) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);
  CoilHeatingDXSingleSpeed heatingCoil(m);
  CoilCoolingDXSingleSpeed coolingCoil(m);
  CoilHeatingElectric supplementalHeatingCoil(m);

  ZoneHVACPackagedTerminalHeatPump zoneHVACPackagedTerminalHeatPump(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACPackagedTerminalHeatPump.handle());
}

// OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACTerminalUnitVariableRefrigerantFlow) {
  Model m;

  CoilCoolingDXVariableRefrigerantFlow coolingCoil(m);
  CoilHeatingDXVariableRefrigerantFlow heatingCoil(m);
  FanSystemModel fan(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow zoneHVACTerminalUnitVariableRefrigerantFlow(m, coolingCoil, heatingCoil, fan);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACTerminalUnitVariableRefrigerantFlow.handle());
}

// OS:ZoneHVAC:UnitHeater
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACUnitHeater) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  CoilHeatingWater heatingCoil(m, s);

  ZoneHVACUnitHeater zoneHVACUnitHeater(m, s, fan, heatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACUnitHeater.handle());
}

// OS:ZoneHVAC:UnitVentilator
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACUnitVentilator) {
  Model m;
  FanSystemModel fan(m);

  ZoneHVACUnitVentilator zoneHVACUnitVentilator(m, fan);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACUnitVentilator.handle());
}

// OS:ZoneHVAC:WaterToAirHeatPump
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACWaterToAirHeatPump) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  CoilHeatingWaterToAirHeatPumpEquationFit heatingCoil(m);
  CoilCoolingWaterToAirHeatPumpEquationFit coolingCoil(m);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  ZoneHVACWaterToAirHeatPump zoneHVACWaterToAirHeatPump(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACWaterToAirHeatPump.handle());
}

/********************************************************************************************************************
*                                 N O T    S U P P O R T E D                                                        *
********************************************************************************************************************/

// Obviously, this shouldn't do!
TEST_F(ModelFixture, FanSystemModel_containingZoneHVACComponent_ZoneHVACBaseboardConvectiveElectric) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);

  ZoneHVACBaseboardConvectiveElectric zoneHVACBaseboardConvectiveElectric(m);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_FALSE(component);
}

// Not supported by E+
TEST_F(ModelFixture, FanSystemModel_containingHVACComponent_AirLoopHVACUnitaryHeatPumpAirToAir) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);
  FanOnOff fanOnOff(m);
  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir unitary(m, s, fanOnOff, heatingCoil, coolingCoil, supplementalHeatingCoil);
  EXPECT_FALSE(unitary.setSupplyAirFan(fan));
}

TEST_F(ModelFixture, FanSystemModel_Clone_SameModel) {
  Model m;
  FanSystemModel fan(m);
  EXPECT_TRUE(fan.setDesignPressureRise(999.0));
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.99));
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(999.0));

  CurveExponent fanPowerFuncFlowCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());
  EXPECT_EQ(1, m.getModelObjects<FanSystemModel>().size());

  FanSystemModel fanClone = fan.clone(m).cast<FanSystemModel>();

  EXPECT_EQ(2, m.getModelObjects<FanSystemModel>().size());

  powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());

  EXPECT_DOUBLE_EQ(999.0, fanClone.designPressureRise());
  EXPECT_DOUBLE_EQ(0.99, fanClone.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(999.0, fanClone.designMaximumAirFlowRate().get());
  ASSERT_TRUE(fanClone.electricPowerFunctionofFlowFractionCurve());
  EXPECT_EQ(fan.electricPowerFunctionofFlowFractionCurve().get(), fanClone.electricPowerFunctionofFlowFractionCurve().get());
}

TEST_F(ModelFixture, FanSystemModel_Clone_OtherModel) {
  Model m;
  FanSystemModel fan(m);
  EXPECT_TRUE(fan.setDesignPressureRise(999.0));
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.99));
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(999.0));

  CurveExponent fanPowerFuncFlowCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));

  Model m2;
  EXPECT_EQ(1, m.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m.getModelObjects<CurveExponent>().size());
  EXPECT_EQ(0, m2.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(0, m2.getModelObjects<CurveExponent>().size());

  FanSystemModel fanClone = fan.clone(m2).cast<FanSystemModel>();
  EXPECT_EQ(1, m.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m.getModelObjects<CurveExponent>().size());
  EXPECT_EQ(1, m2.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m2.getModelObjects<CurveExponent>().size());

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());

  std::vector<CurveExponent> powerCurves2 = m2.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves2.size());

  for (const auto& c : powerCurves) {
    EXPECT_TRUE(c.parent());
  }
  for (const auto& c : powerCurves2) {
    EXPECT_TRUE(c.parent());
  }

  EXPECT_DOUBLE_EQ(999.0, fanClone.designPressureRise());
  EXPECT_DOUBLE_EQ(0.99, fanClone.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(999.0, fanClone.designMaximumAirFlowRate().get());
  ASSERT_TRUE(fanClone.electricPowerFunctionofFlowFractionCurve());
  EXPECT_NE(fan.electricPowerFunctionofFlowFractionCurve().get().handle(), fanClone.electricPowerFunctionofFlowFractionCurve().get().handle());
}

TEST_F(ModelFixture, FanSystemModelSpeed) {

  FanSystemModelSpeed speed(0.5, 0.45);
  EXPECT_EQ(0.5, speed.flowFraction());
  EXPECT_EQ(0.45, speed.electricPowerFraction());

  ASSERT_THROW(FanSystemModelSpeed(-0.1, 0.45), openstudio::Exception);
  ASSERT_THROW(FanSystemModelSpeed(1.1, 0.45), openstudio::Exception);
  ASSERT_THROW(FanSystemModelSpeed(0.5, -0.1), openstudio::Exception);
  ASSERT_THROW(FanSystemModelSpeed(0.5, 1.1), openstudio::Exception);
}

TEST_F(ModelFixture, FanSystemModel_Speeds) {
  Model m;
  FanSystemModel fan(m);

  // If no extensible groups, same as having one speed (= the design one)
  EXPECT_EQ(0, fan.numExtensibleGroups());
  EXPECT_EQ(1, fan.numberofSpeeds());

  EXPECT_TRUE(fan.addSpeed(0.45, 0.5));
  EXPECT_EQ(1, fan.numExtensibleGroups());
  EXPECT_EQ(1, fan.numberofSpeeds());

  EXPECT_TRUE(fan.addSpeed(0.15, 0.7));
  EXPECT_EQ(2, fan.numExtensibleGroups());
  EXPECT_EQ(2, fan.numberofSpeeds());

  // This should have been sorted...
  std::vector<FanSystemModelSpeed> speeds = fan.speeds();
  EXPECT_EQ(2, speeds.size());
  EXPECT_EQ(0.15, speeds[0].flowFraction());
  EXPECT_EQ(0.7, speeds[0].electricPowerFraction());
  EXPECT_EQ(0.45, speeds[1].flowFraction());
  EXPECT_EQ(0.5, speeds[1].electricPowerFraction());

  EXPECT_TRUE(fan.addSpeed(FanSystemModelSpeed(0.75, 0.8)));
  EXPECT_EQ(3, fan.numExtensibleGroups());
  EXPECT_EQ(3, fan.numberofSpeeds());
  speeds = fan.speeds();
  EXPECT_EQ(3, speeds.size());
  EXPECT_EQ(0.15, speeds[0].flowFraction());
  EXPECT_EQ(0.7, speeds[0].electricPowerFraction());
  EXPECT_EQ(0.45, speeds[1].flowFraction());
  EXPECT_EQ(0.5, speeds[1].electricPowerFraction());
  EXPECT_EQ(0.75, speeds[2].flowFraction());
  EXPECT_EQ(0.8, speeds[2].electricPowerFraction());

  fan.removeSpeed(1);
  EXPECT_EQ(2, fan.numExtensibleGroups());
  EXPECT_EQ(2, fan.numberofSpeeds());
  speeds = fan.speeds();
  EXPECT_EQ(2, speeds.size());
  EXPECT_EQ(0.15, speeds[0].flowFraction());
  EXPECT_EQ(0.7, speeds[0].electricPowerFraction());
  EXPECT_EQ(0.75, speeds[1].flowFraction());
  EXPECT_EQ(0.8, speeds[1].electricPowerFraction());

  fan.removeAllSpeeds();
  EXPECT_EQ(0, fan.numExtensibleGroups());
  EXPECT_EQ(0, fan.speeds().size());
  EXPECT_EQ(1, fan.numberofSpeeds());

  // Use the setSpeeds, which should sort too
  speeds.push_back(FanSystemModelSpeed(0.35, 0.72));
  EXPECT_TRUE(fan.setSpeeds(speeds));
  EXPECT_EQ(3, fan.numExtensibleGroups());
  EXPECT_EQ(3, fan.numberofSpeeds());
  speeds = fan.speeds();
  EXPECT_EQ(3, speeds.size());
  EXPECT_EQ(0.15, speeds[0].flowFraction());
  EXPECT_EQ(0.7, speeds[0].electricPowerFraction());
  EXPECT_EQ(0.35, speeds[1].flowFraction());
  EXPECT_EQ(0.72, speeds[1].electricPowerFraction());
  EXPECT_EQ(0.75, speeds[2].flowFraction());
  EXPECT_EQ(0.8, speeds[2].electricPowerFraction());
}
