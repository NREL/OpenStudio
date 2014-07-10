/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
    EXPECT_FALSE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)1, outdoorAirSystem.reliefComponents().size() );
  }

  CoilHeatingGas testObjectClone = testObject.clone(m).cast<CoilHeatingGas>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );
}
