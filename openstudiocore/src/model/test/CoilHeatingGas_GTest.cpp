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
#include "../CoilHeatingGas.hpp"
#include "../CoilHeatingGas_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingGas) {
  Model model;
  ScheduleConstant scheduleConstant(model);

  CoilHeatingGas coilHeatingGas(model, scheduleConstant);

  coilHeatingGas.setGasBurnerEfficiency(0.6);
  EXPECT_EQ(coilHeatingGas.gasBurnerEfficiency(),0.6);

  coilHeatingGas.setNominalCapacity(1535.0);
  EXPECT_EQ(coilHeatingGas.nominalCapacity(),1535.0);

  coilHeatingGas.setParasiticElectricLoad(48.0);
  EXPECT_EQ(coilHeatingGas.parasiticElectricLoad(),48.0);

  coilHeatingGas.setParasiticGasLoad(51.0);
  EXPECT_EQ(coilHeatingGas.parasiticGasLoad(),51.0);

  ScheduleConstant schedule2(model);
  coilHeatingGas.setAvailableSchedule(schedule2);
  EXPECT_EQ(coilHeatingGas.availableSchedule(),schedule2);

  EXPECT_EQ(coilHeatingGas.fuelType(), "NaturalGas");
  bool isOk = coilHeatingGas.setFuelType("Propane");
  EXPECT_TRUE(isOk);
  EXPECT_EQ(coilHeatingGas.fuelType(), "Propane");
  isOk = coilHeatingGas.setFuelType("PropaneGas");
  EXPECT_TRUE(isOk);
  EXPECT_EQ(coilHeatingGas.fuelType(), "PropaneGas");
  coilHeatingGas.resetFuelType();
  EXPECT_EQ(coilHeatingGas.fuelType(), "NaturalGas");

  std::vector<std::string> validFuelTypes(coilHeatingGas.validFuelTypeValues());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "PropaneGas"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "NaturalGas"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Diesel"), validFuelTypes.end());
  EXPECT_EQ(validFuelTypes.size(), 9);

}

TEST_F(ModelFixture,CoilHeatingGas_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();

  CoilHeatingGas testObject(m, s);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m,controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  CoilHeatingGas testObject2(m, s);
  CoilHeatingGas testObject3(m, s);

  if( boost::optional<Node> OANode = outdoorAirSystem.outboardOANode() ) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)3, outdoorAirSystem.oaComponents().size() );
  }

  if( boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode() ) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)3, outdoorAirSystem.reliefComponents().size() );
  }

  CoilHeatingGas testObjectClone = testObject.clone(m).cast<CoilHeatingGas>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );
}
