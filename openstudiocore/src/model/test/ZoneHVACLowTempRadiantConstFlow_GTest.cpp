/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACLowTempRadiantConstFlow_SetGetFields) {
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

  CoilCoolingLowTempRadiantConstFlow testCC(model,coolingHighWaterTempSched,coolingLowWaterTempSched,coolingHighControlTempSched,coolingLowControlTempSched);
  CoilHeatingLowTempRadiantConstFlow testHC(model,heatingHighWaterTempSched,heatingLowWaterTempSched,heatingHighControlTempSched,heatingLowControlTempSched);

  HVACComponent testCC1 = testCC.cast<HVACComponent>();
  HVACComponent testHC1 = testHC.cast<HVACComponent>();

  ZoneHVACLowTempRadiantConstFlow testRad(model,availabilitySched,testHC1,testCC1,200.0);

  testRad.setHeatingCoil(testHC1);
  EXPECT_EQ(testRad.heatingCoil(),testHC1);

  testRad.setCoolingCoil(testCC1);
  EXPECT_EQ(testRad.coolingCoil(),testCC1);

  testRad.setRadiantSurfaceType("Floors");
  boost::optional<std::string> str1 = testRad.radiantSurfaceType();
  EXPECT_EQ(*str1,"Floors");

  testRad.setHydronicTubingInsideDiameter(0.01);
  double inDia = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingInsideDiameterDefaulted());
  testRad.resetHydronicTubingInsideDiameter();
  EXPECT_TRUE(testRad.isHydronicTubingInsideDiameterDefaulted());
  double inDia1 = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia1,0.013);

  testRad.setHydronicTubingLength(200);
  boost::optional<double> length = testRad.hydronicTubingLength();
  EXPECT_EQ(*length, 200);

  testRad.setTemperatureControlType("MeanRadiantTemperature");
  boost::optional<std::string> str2 = testRad.temperatureControlType();
  EXPECT_EQ(*str2,"MeanRadiantTemperature");
  EXPECT_FALSE(testRad.isTemperatureControlTypeDefaulted());
  testRad.resetTemperatureControlType();
  EXPECT_TRUE(testRad.isTemperatureControlTypeDefaulted());
  boost::optional<std::string> str3 = testRad.temperatureControlType();
  EXPECT_EQ(*str3,"MeanAirTemperature");

  //test Pump Flow Rate Schedule
  ScheduleConstant pumpFlowRateSched(model);
  pumpFlowRateSched.setValue(1.0);
  testRad.setPumpFlowRateSchedule(pumpFlowRateSched);
  boost::optional<Schedule> pumpFRSch = testRad.pumpFlowRateSchedule();
  EXPECT_EQ(*pumpFRSch,pumpFlowRateSched);

  //test Rated Pump Head
  testRad.setRatedPumpHead(20000);
  double head = testRad.ratedPumpHead();
  EXPECT_EQ(head,20000);
  EXPECT_FALSE(testRad.isRatedPumpHeadDefaulted());

  //test Rated Power Consumption
  testRad.setRatedPowerConsumption(700);
  boost::optional<double> power = testRad.ratedPowerConsumption();
  EXPECT_EQ(*power,700);

  //test Motor Efficiency
  testRad.setMotorEfficiency(0.9);
  double eff=testRad.motorEfficiency();
  EXPECT_EQ(eff,0.9);

  //test fraction of motor ineffficiencies to Fluid Stream
  testRad.setFractionofMotorInefficienciestoFluidStream(1.0);
  double inEff=testRad.fractionofMotorInefficienciestoFluidStream();
  EXPECT_EQ(inEff,1.0);

  //test number of circuits
  testRad.setNumberofCircuits("CalculateFromCircuitLength");
  std::string numCirc = testRad.numberofCircuits();
  EXPECT_EQ(numCirc,"CalculateFromCircuitLength");

  //test circuit length
  testRad.setCircuitLength(200.0);
  double circLength = testRad.circuitLength();
  EXPECT_EQ(circLength,200.0);

  //test add and remove from thermal zone
  ThermalZone thermalZone(model);
  EXPECT_EQ(0u,thermalZone.equipment().size());

  EXPECT_TRUE(testRad.addToThermalZone(thermalZone));

  EXPECT_EQ(1u,thermalZone.equipment().size());

  EXPECT_TRUE(testHC1.containingZoneHVACComponent());

  EXPECT_TRUE(testCC1.containingZoneHVACComponent());


}

