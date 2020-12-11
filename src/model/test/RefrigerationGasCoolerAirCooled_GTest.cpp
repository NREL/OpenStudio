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

#include "../RefrigerationGasCoolerAirCooled.hpp"
#include "../RefrigerationGasCoolerAirCooled_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"
// #include <model/ThermalZone.hpp>
// #include <model/ThermalZone_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

//Test construction of air-cooled gas cooler
TEST_F(ModelFixture, RefrigerationGasCoolerAirCooled_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      RefrigerationGasCoolerAirCooled testObject = RefrigerationGasCoolerAirCooled(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

//Test removal of air-cooled gas cooler
TEST_F(ModelFixture, RefrigerationGasCoolerAirCooled_Remove) {
  Model model;
  RefrigerationGasCoolerAirCooled testObject = RefrigerationGasCoolerAirCooled(model);

  std::vector<RefrigerationGasCoolerAirCooled> refrigerationAirCooledGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(1, refrigerationAirCooledGasCoolers.size());

  std::vector<CurveLinear> ratedTotalHeatRejectionRateCurve = model.getModelObjects<CurveLinear>();
  EXPECT_EQ(1, ratedTotalHeatRejectionRateCurve.size());

  testObject.remove();

  refrigerationAirCooledGasCoolers = model.getModelObjects<RefrigerationGasCoolerAirCooled>();
  EXPECT_EQ(0, refrigerationAirCooledGasCoolers.size());

  // Curve was used only by this object, so should have been removed
  ratedTotalHeatRejectionRateCurve = model.getModelObjects<CurveLinear>();
  EXPECT_EQ(0, ratedTotalHeatRejectionRateCurve.size());
}

//Test the methods that set and get the fields
TEST_F(ModelFixture, RefrigerationGasCoolerAirCooled_set_get) {
  Model model;

  RefrigerationGasCoolerAirCooled gascooler(model);

  gascooler.setGasCoolerFanSpeedControlType("Fixed");
  gascooler.setRatedFanPower(99.0);
  gascooler.setMinimumFanAirFlowRatio(0.9);
  gascooler.setTransitionTemperature(99.0);
  gascooler.setTranscriticalApproachTemperature(9.0);
  gascooler.setSubcriticalTemperatureDifference(9.0);
  gascooler.setMinimumCondensingTemperature(9.0);
  gascooler.setEndUseSubcategory("EndUseCat");
  gascooler.setGasCoolerRefrigerantOperatingChargeInventory(99.0);
  gascooler.setGasCoolerReceiverRefrigerantInventory(99.0);
  gascooler.setGasCoolerOutletPipingRefrigerantInventory(99.0);

  EXPECT_EQ(gascooler.gasCoolerFanSpeedControlType(), "Fixed");
  EXPECT_DOUBLE_EQ(gascooler.ratedFanPower(), 99.0);
  EXPECT_DOUBLE_EQ(gascooler.minimumFanAirFlowRatio(), 0.9);
  EXPECT_DOUBLE_EQ(gascooler.transitionTemperature(), 99.0);
  EXPECT_DOUBLE_EQ(gascooler.transcriticalApproachTemperature(), 9.0);
  EXPECT_DOUBLE_EQ(gascooler.subcriticalTemperatureDifference(), 9.0);
  EXPECT_DOUBLE_EQ(gascooler.minimumCondensingTemperature(), 9.0);
  EXPECT_EQ(gascooler.endUseSubcategory(), "EndUseCat");
  EXPECT_DOUBLE_EQ(gascooler.gasCoolerRefrigerantOperatingChargeInventory(), 99.0);
  EXPECT_DOUBLE_EQ(gascooler.gasCoolerReceiverRefrigerantInventory(), 99.0);
  EXPECT_DOUBLE_EQ(gascooler.gasCoolerOutletPipingRefrigerantInventory(), 99.0);
}

//Test clone model with default data
TEST_F(ModelFixture, RefrigerationGasCoolerAirCooled_CloneModelWithDefaultData) {
  Model model;

  RefrigerationGasCoolerAirCooled testObject = RefrigerationGasCoolerAirCooled(model);

  RefrigerationGasCoolerAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationGasCoolerAirCooled>();

  std::vector<CurveLinear> refrigerationGasCoolerCurves = model.getModelObjects<CurveLinear>();
  for (auto it = refrigerationGasCoolerCurves.begin(); it != refrigerationGasCoolerCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_EQ(testObjectClone.gasCoolerFanSpeedControlType(), "VariableSpeed");
  EXPECT_DOUBLE_EQ(testObjectClone.ratedFanPower(), 12000.0);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumFanAirFlowRatio(), 0.2);
  EXPECT_DOUBLE_EQ(testObjectClone.transitionTemperature(), 27.0);
  EXPECT_DOUBLE_EQ(testObjectClone.transcriticalApproachTemperature(), 3.0);
  EXPECT_DOUBLE_EQ(testObjectClone.subcriticalTemperatureDifference(), 10.0);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumCondensingTemperature(), 10.0);
  EXPECT_EQ(testObjectClone.endUseSubcategory(), "General");
  EXPECT_DOUBLE_EQ(testObjectClone.gasCoolerRefrigerantOperatingChargeInventory(), 0.0);
  EXPECT_DOUBLE_EQ(testObjectClone.gasCoolerReceiverRefrigerantInventory(), 0.0);
  EXPECT_DOUBLE_EQ(testObjectClone.gasCoolerOutletPipingRefrigerantInventory(), 0.0);
  EXPECT_EQ(testObject.ratedTotalHeatRejectionRateCurve().get().handle(), testObjectClone.ratedTotalHeatRejectionRateCurve().get().handle());
}

//Test clone model with custom data
TEST_F(ModelFixture, RefrigerationGasCoolerAirCooled_CloneModelWithCustomData) {
  Model model;
  // ThermalZone thermalZone(model);

  RefrigerationGasCoolerAirCooled testObject = RefrigerationGasCoolerAirCooled(model);

  testObject.setGasCoolerFanSpeedControlType("VariableSpeed");
  testObject.setRatedFanPower(99.0);
  testObject.setMinimumFanAirFlowRatio(0.9);
  testObject.setTransitionTemperature(99.0);
  testObject.setTranscriticalApproachTemperature(9.0);
  testObject.setSubcriticalTemperatureDifference(9.0);
  testObject.setMinimumCondensingTemperature(9.0);
  testObject.setEndUseSubcategory("EndUseCat");
  testObject.setGasCoolerRefrigerantOperatingChargeInventory(99.0);
  testObject.setGasCoolerReceiverRefrigerantInventory(99.0);
  testObject.setGasCoolerOutletPipingRefrigerantInventory(99.0);
  // testObject.setAirInletNode(thermalZone);

  RefrigerationGasCoolerAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationGasCoolerAirCooled>();

  EXPECT_EQ(testObjectClone.gasCoolerFanSpeedControlType(), "VariableSpeed");
  EXPECT_DOUBLE_EQ(testObjectClone.ratedFanPower(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumFanAirFlowRatio(), 0.9);
  EXPECT_DOUBLE_EQ(testObjectClone.transitionTemperature(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone.transcriticalApproachTemperature(), 9.0);
  EXPECT_DOUBLE_EQ(testObjectClone.subcriticalTemperatureDifference(), 9.0);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumCondensingTemperature(), 9.0);
  EXPECT_EQ(testObjectClone.endUseSubcategory(), "EndUseCat");
  EXPECT_DOUBLE_EQ(testObjectClone.gasCoolerRefrigerantOperatingChargeInventory(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone.gasCoolerReceiverRefrigerantInventory(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone.gasCoolerOutletPipingRefrigerantInventory(), 99.0);
  // EXPECT_FALSE(testObjectClone.airInletNode());
}

//Test clone model with custom data
TEST_F(ModelFixture, RefrigerationGasCoolerAirCooled_CloneTwoModelWithCustomData) {
  Model model;
  // ThermalZone thermalZone(model);

  RefrigerationGasCoolerAirCooled testObject = RefrigerationGasCoolerAirCooled(model);

  testObject.setGasCoolerFanSpeedControlType("VariableSpeed");
  testObject.setRatedFanPower(99.0);
  testObject.setMinimumFanAirFlowRatio(0.9);
  testObject.setTransitionTemperature(99.0);
  testObject.setTranscriticalApproachTemperature(9.0);
  testObject.setSubcriticalTemperatureDifference(9.0);
  testObject.setMinimumCondensingTemperature(9.0);
  testObject.setEndUseSubcategory("EndUseCat");
  testObject.setGasCoolerRefrigerantOperatingChargeInventory(99.0);
  testObject.setGasCoolerReceiverRefrigerantInventory(99.0);
  testObject.setGasCoolerOutletPipingRefrigerantInventory(99.0);
  // testObject.setAirInletNode(thermalZone);

  RefrigerationGasCoolerAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationGasCoolerAirCooled>();

  Model model2;
  RefrigerationGasCoolerAirCooled testObjectClone2 = testObject.clone(model2).cast<RefrigerationGasCoolerAirCooled>();

  EXPECT_EQ(testObjectClone2.gasCoolerFanSpeedControlType(), "VariableSpeed");
  EXPECT_DOUBLE_EQ(testObjectClone2.ratedFanPower(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.minimumFanAirFlowRatio(), 0.9);
  EXPECT_DOUBLE_EQ(testObjectClone2.transitionTemperature(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.transcriticalApproachTemperature(), 9.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.subcriticalTemperatureDifference(), 9.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.minimumCondensingTemperature(), 9.0);
  EXPECT_EQ(testObjectClone2.endUseSubcategory(), "EndUseCat");
  EXPECT_DOUBLE_EQ(testObjectClone2.gasCoolerRefrigerantOperatingChargeInventory(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.gasCoolerReceiverRefrigerantInventory(), 99.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.gasCoolerOutletPipingRefrigerantInventory(), 99.0);
  EXPECT_TRUE(testObjectClone2.ratedTotalHeatRejectionRateCurve());
  EXPECT_NE(testObjectClone2.ratedTotalHeatRejectionRateCurve().get(), testObject.ratedTotalHeatRejectionRateCurve().get());
  // EXPECT_FALSE(testObjectClone2.airInletNode());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone, testObject);
}
