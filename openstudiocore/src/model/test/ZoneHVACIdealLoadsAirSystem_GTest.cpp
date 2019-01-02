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

#include "../ZoneHVACIdealLoadsAirSystem.hpp"
#include "../ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"
#include "../AirLoopHVACReturnPlenum_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumHeatingSupplyAirTemperature_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirTemperature(true).units(); // Get IP units.

  // We assign a Maximum Supply temp of 80F
  double value(80.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirTemperature(testQ));
  // We get it as IP units
  Quantity q = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirTemperature(true);
  // Value should match
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MinimumCoolingSupplyAirTemperature_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirTemperature(true).units(); // Get IP units.
  double value(55.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirTemperature(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumHeatingSupplyAirHumidityRatio_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirHumidityRatio(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirHumidityRatio(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirHumidityRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MinimumCoolingSupplyAirHumidityRatio_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirHumidityRatio(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirHumidityRatio(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirHumidityRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumHeatingAirFlowRate_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumHeatingAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumHeatingAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumHeatingAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumSensibleHeatingCapacity_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumSensibleHeatingCapacity(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumSensibleHeatingCapacity(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumSensibleHeatingCapacity(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumCoolingAirFlowRate_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumCoolingAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumCoolingAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumCoolingAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumTotalCoolingCapacity_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumTotalCoolingCapacity(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumTotalCoolingCapacity(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumTotalCoolingCapacity(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_CoolingSensibleHeatRatio_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getCoolingSensibleHeatRatio(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setCoolingSensibleHeatRatio(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getCoolingSensibleHeatRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_SensibleHeatRecoveryEffectiveness_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getSensibleHeatRecoveryEffectiveness(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setSensibleHeatRecoveryEffectiveness(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getSensibleHeatRecoveryEffectiveness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_LatentHeatRecoveryEffectiveness_Quantity) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getLatentHeatRecoveryEffectiveness(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setLatentHeatRecoveryEffectiveness(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getLatentHeatRecoveryEffectiveness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_ReturnPlenum) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  ThermalZone zone(model);
  ThermalZone plenumzone(model);

  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.addToThermalZone(zone));
  auto z = zoneHVACIdealLoadsAirSystem.thermalZone();
  ASSERT_TRUE(z);
  EXPECT_EQ(z.get(), zone);

  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setReturnPlenum(plenumzone));
  auto p = zoneHVACIdealLoadsAirSystem.returnPlenum();
  ASSERT_TRUE(p);

  auto pz = p->thermalZone();
  ASSERT_TRUE(pz);
  EXPECT_EQ(pz.get(), plenumzone);
}

