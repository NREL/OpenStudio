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
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../ZoneHVACLowTempRadiantConstFlow.hpp"
#include "../ZoneHVACLowTempRadiantConstFlow_Impl.hpp"
#include "../CoilCoolingLowTempRadiantConstFlow.hpp"
#include "../CoilCoolingLowTempRadiantConstFlow_Impl.hpp"
#include "../CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Space.hpp"
#include "../Construction.hpp"
#include "../ConstructionWithInternalSource.hpp"

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACLowTempRadiantConstFlow_SetGetFields) {
  Model model;
  ScheduleConstant availabilitySched(model);
  ScheduleConstant coolingHighWaterTempSched(model);
  ScheduleConstant coolingLowWaterTempSched(model);
  ScheduleConstant coolingHighControlTempSched(model);
  ScheduleConstant coolingLowControlTempSched(model);
  ScheduleConstant heatingHighWaterTempSched(model);
  ScheduleConstant heatingLowWaterTempSched(model);
  ScheduleConstant heatingHighControlTempSched(model);
  ScheduleConstant heatingLowControlTempSched(model);

  availabilitySched.setValue(1.0);
  coolingHighWaterTempSched.setValue(15.0);
  coolingLowWaterTempSched.setValue(10.0);
  coolingHighControlTempSched.setValue(26.0);
  coolingLowControlTempSched.setValue(22.0);
  heatingHighWaterTempSched.setValue(50.0);
  heatingLowWaterTempSched.setValue(30.0);
  heatingHighControlTempSched.setValue(21.0);
  heatingLowControlTempSched.setValue(15.0);

  CoilCoolingLowTempRadiantConstFlow testCC(model, coolingHighWaterTempSched, coolingLowWaterTempSched, coolingHighControlTempSched,
                                            coolingLowControlTempSched);
  CoilHeatingLowTempRadiantConstFlow testHC(model, heatingHighWaterTempSched, heatingLowWaterTempSched, heatingHighControlTempSched,
                                            heatingLowControlTempSched);

  HVACComponent testCC1 = testCC.cast<HVACComponent>();
  HVACComponent testHC1 = testHC.cast<HVACComponent>();

  ZoneHVACLowTempRadiantConstFlow testRad(model, availabilitySched, testHC1, testCC1, 200.0);

  testRad.setHeatingCoil(testHC1);
  EXPECT_EQ(testRad.heatingCoil(), testHC1);

  testRad.setCoolingCoil(testCC1);
  EXPECT_EQ(testRad.coolingCoil(), testCC1);

  testRad.setRadiantSurfaceType("Floors");
  boost::optional<std::string> str1 = testRad.radiantSurfaceType();
  EXPECT_EQ(*str1, "Floors");

  testRad.setFluidtoRadiantSurfaceHeatTransferModel("ISOStandard");
  boost::optional<std::string> str2 = testRad.fluidtoRadiantSurfaceHeatTransferModel();
  EXPECT_EQ(*str2, "ISOStandard");
  EXPECT_FALSE(testRad.isFluidtoRadiantSurfaceHeatTransferModelDefaulted());
  testRad.resetFluidtoRadiantSurfaceHeatTransferModel();
  EXPECT_TRUE(testRad.isFluidtoRadiantSurfaceHeatTransferModelDefaulted());
  boost::optional<std::string> str3 = testRad.fluidtoRadiantSurfaceHeatTransferModel();
  EXPECT_EQ(*str3, "ConvectionOnly");

  testRad.setHydronicTubingInsideDiameter(0.01);
  double inDia = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingInsideDiameterDefaulted());
  testRad.resetHydronicTubingInsideDiameter();
  EXPECT_TRUE(testRad.isHydronicTubingInsideDiameterDefaulted());
  double inDia1 = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia1, 0.013);

  testRad.setHydronicTubingOutsideDiameter(0.01);
  double outDia = testRad.hydronicTubingOutsideDiameter();
  EXPECT_EQ(outDia, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingOutsideDiameterDefaulted());
  testRad.resetHydronicTubingOutsideDiameter();
  EXPECT_TRUE(testRad.isHydronicTubingOutsideDiameterDefaulted());
  double outDia1 = testRad.hydronicTubingOutsideDiameter();
  EXPECT_EQ(outDia1, 0.016);

  testRad.setHydronicTubingLength(200);
  boost::optional<double> length = testRad.hydronicTubingLength();
  EXPECT_EQ(*length, 200);
  EXPECT_FALSE(testRad.isHydronicTubingLengthAutosized());
  testRad.resetHydronicTubingLength();
  testRad.autosizeHydronicTubingLength();
  EXPECT_TRUE(testRad.isHydronicTubingLengthAutosized());

  testRad.setHydronicTubingConductivity(0.01);
  double cond = testRad.hydronicTubingConductivity();
  EXPECT_EQ(cond, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingConductivityDefaulted());
  testRad.resetHydronicTubingConductivity();
  EXPECT_TRUE(testRad.isHydronicTubingConductivityDefaulted());
  double cond1 = testRad.hydronicTubingConductivity();
  EXPECT_EQ(cond1, 0.35);

  testRad.setTemperatureControlType("MeanRadiantTemperature");
  str2 = testRad.temperatureControlType();
  EXPECT_EQ(*str2, "MeanRadiantTemperature");
  EXPECT_FALSE(testRad.isTemperatureControlTypeDefaulted());
  testRad.resetTemperatureControlType();
  EXPECT_TRUE(testRad.isTemperatureControlTypeDefaulted());
  str3 = testRad.temperatureControlType();
  EXPECT_EQ(*str3, "MeanAirTemperature");

  testRad.setRunningMeanOutdoorDryBulbTemperatureWeightingFactor(0.5);
  double running = testRad.runningMeanOutdoorDryBulbTemperatureWeightingFactor();
  EXPECT_EQ(running, 0.5);
  EXPECT_FALSE(testRad.isRunningMeanOutdoorDryBulbTemperatureWeightingFactorDefaulted());
  testRad.resetRunningMeanOutdoorDryBulbTemperatureWeightingFactor();
  EXPECT_TRUE(testRad.isRunningMeanOutdoorDryBulbTemperatureWeightingFactorDefaulted());
  double running1 = testRad.runningMeanOutdoorDryBulbTemperatureWeightingFactor();
  EXPECT_EQ(running1, 0.8);

  //test Pump Flow Rate Schedule
  ScheduleConstant pumpFlowRateSched(model);
  pumpFlowRateSched.setValue(1.0);
  testRad.setPumpFlowRateSchedule(pumpFlowRateSched);
  boost::optional<Schedule> pumpFRSch = testRad.pumpFlowRateSchedule();
  EXPECT_EQ(*pumpFRSch, pumpFlowRateSched);

  //test Rated Pump Head
  testRad.setRatedPumpHead(20000);
  double head = testRad.ratedPumpHead();
  EXPECT_EQ(head, 20000);
  EXPECT_FALSE(testRad.isRatedPumpHeadDefaulted());

  //test Rated Power Consumption
  testRad.setRatedPowerConsumption(700);
  boost::optional<double> power = testRad.ratedPowerConsumption();
  EXPECT_EQ(*power, 700);

  //test Motor Efficiency
  testRad.setMotorEfficiency(0.9);
  double eff = testRad.motorEfficiency();
  EXPECT_EQ(eff, 0.9);

  //test fraction of motor ineffficiencies to Fluid Stream
  testRad.setFractionofMotorInefficienciestoFluidStream(1.0);
  double inEff = testRad.fractionofMotorInefficienciestoFluidStream();
  EXPECT_EQ(inEff, 1.0);

  //test number of circuits
  testRad.setNumberofCircuits("CalculateFromCircuitLength");
  std::string numCirc = testRad.numberofCircuits();
  EXPECT_EQ(numCirc, "CalculateFromCircuitLength");

  //test circuit length
  testRad.setCircuitLength(200.0);
  double circLength = testRad.circuitLength();
  EXPECT_EQ(circLength, 200.0);

  ScheduleConstant sch(model);
  EXPECT_TRUE(testRad.setChangeoverDelayTimePeriodSchedule(sch));
  ASSERT_TRUE(testRad.changeoverDelayTimePeriodSchedule());
  EXPECT_EQ(sch, testRad.changeoverDelayTimePeriodSchedule().get());
  testRad.resetChangeoverDelayTimePeriodSchedule();
  EXPECT_FALSE(testRad.changeoverDelayTimePeriodSchedule());

  //test add and remove from thermal zone
  ThermalZone thermalZone(model);
  EXPECT_EQ(0u, thermalZone.equipment().size());

  EXPECT_TRUE(testRad.addToThermalZone(thermalZone));

  EXPECT_EQ(1u, thermalZone.equipment().size());

  EXPECT_TRUE(testHC1.containingZoneHVACComponent());

  EXPECT_TRUE(testCC1.containingZoneHVACComponent());

  EXPECT_TRUE(testRad.isRatedFlowRateAutosized());
  EXPECT_FALSE(testRad.ratedFlowRate());
  EXPECT_TRUE(testRad.setRatedFlowRate(0.75));
  EXPECT_FALSE(testRad.isRatedFlowRateAutosized());
  ASSERT_TRUE(testRad.ratedFlowRate());
  EXPECT_EQ(0.75, testRad.ratedFlowRate().get());
  testRad.autosizeRatedFlowRate();
  EXPECT_TRUE(testRad.isRatedFlowRateAutosized());
  EXPECT_FALSE(testRad.ratedFlowRate());

  testRad.autosizeHydronicTubingLength();
  EXPECT_TRUE(testRad.isHydronicTubingLengthAutosized());
  EXPECT_FALSE(testRad.hydronicTubingLength());
  EXPECT_TRUE(testRad.setHydronicTubingLength(150.0));
  EXPECT_FALSE(testRad.isHydronicTubingLengthAutosized());
  ASSERT_TRUE(testRad.hydronicTubingLength());
  EXPECT_EQ(150.0, testRad.hydronicTubingLength().get());
  testRad.autosizeHydronicTubingLength();
  EXPECT_TRUE(testRad.isHydronicTubingLengthAutosized());
  EXPECT_FALSE(testRad.hydronicTubingLength());
}

TEST_F(ModelFixture, ZoneHVACLowTempRadiantConstFlow_surfaces) {

  Model m;

  // make a space with some surfaces
  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  boost::optional<Space> _space1 = Space::fromFloorPrint(points, 3, m);
  ASSERT_TRUE(_space1);
  EXPECT_EQ(6u, _space1->surfaces().size());

  // make a zone, add the space
  ThermalZone z(m);
  _space1->setThermalZone(z);

  // Make a radiant low temperature system
  ScheduleConstant availabilitySched(m);
  ScheduleConstant coolingHighWaterTempSched(m);
  ScheduleConstant coolingLowWaterTempSched(m);
  ScheduleConstant coolingHighControlTempSched(m);
  ScheduleConstant coolingLowControlTempSched(m);
  ScheduleConstant heatingHighWaterTempSched(m);
  ScheduleConstant heatingLowWaterTempSched(m);
  ScheduleConstant heatingHighControlTempSched(m);
  ScheduleConstant heatingLowControlTempSched(m);

  availabilitySched.setValue(1.0);
  coolingHighWaterTempSched.setValue(15.0);
  coolingLowWaterTempSched.setValue(10.0);
  coolingHighControlTempSched.setValue(26.0);
  coolingLowControlTempSched.setValue(22.0);
  heatingHighWaterTempSched.setValue(50.0);
  heatingLowWaterTempSched.setValue(30.0);
  heatingHighControlTempSched.setValue(21.0);
  heatingLowControlTempSched.setValue(15.0);

  CoilCoolingLowTempRadiantConstFlow clg_coil(m, coolingHighWaterTempSched, coolingLowWaterTempSched, coolingHighControlTempSched,
                                              coolingLowControlTempSched);
  CoilHeatingLowTempRadiantConstFlow htg_coil(m, heatingHighWaterTempSched, heatingLowWaterTempSched, heatingHighControlTempSched,
                                              heatingLowControlTempSched);

  ZoneHVACLowTempRadiantConstFlow testRad(m, availabilitySched, htg_coil, clg_coil);
  EXPECT_TRUE(testRad.setRadiantSurfaceType("AllSurfaces"));
  EXPECT_TRUE(testRad.addToThermalZone(z));

  // The surfaces don't have any constructions assigned
  ASSERT_NO_THROW(testRad.surfaces());
  EXPECT_EQ(0u, testRad.surfaces().size());

  Construction c(m);
  for (auto& s : _space1->surfaces()) {
    s.setConstruction(c);
  }

  // The surfaces have constructions, but not internal source
  ASSERT_NO_THROW(testRad.surfaces());
  EXPECT_EQ(0u, testRad.surfaces().size());

  ConstructionWithInternalSource cInternalSource(m);
  for (auto& s : _space1->surfaces()) {
    s.setConstruction(cInternalSource);
  }

  // The surfaces have constructions, with  internal source
  ASSERT_NO_THROW(testRad.surfaces());
  EXPECT_EQ(6u, testRad.surfaces().size());
}
