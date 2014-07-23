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
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../Schedule.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingDXSingleSpeed_CoilHeatingDXSingleSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m;
     Schedule s = m.alwaysOnDiscreteSchedule();
     CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
     CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
     CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
     CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
     CurveQuadratic  partLoadFractionCorrelationCurve(m);

     CoilHeatingDXSingleSpeed coil(m, s,
                                   totalHeatingCapacityFunctionofTemperatureCurve,
                                   totalHeatingCapacityFunctionofFlowFractionCurve,
                                   energyInputRatioFunctionofTemperatureCurve,
                                   energyInputRatioFunctionofFlowFractionCurve,
                                   partLoadFractionCorrelationCurve); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,CoilHeatingDXSingleSpeed_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic  partLoadFractionCorrelationCurve(m);

  CoilHeatingDXSingleSpeed testObject(m, s,
                                      totalHeatingCapacityFunctionofTemperatureCurve,
                                      totalHeatingCapacityFunctionofFlowFractionCurve,
                                      energyInputRatioFunctionofTemperatureCurve,
                                      energyInputRatioFunctionofFlowFractionCurve,
                                      partLoadFractionCorrelationCurve); 

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

  CurveBiquadratic  c1(m);
  CurveQuadratic  c2(m);
  CurveBiquadratic  c3(m);
  CurveQuadratic  c4(m);
  CurveQuadratic  c5(m);

  CoilHeatingDXSingleSpeed testObject2(m, s, c1, c2, c3, c4, c5);

  if( boost::optional<Node> OANode = outdoorAirSystem.outboardOANode() ) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)3, outdoorAirSystem.oaComponents().size() );
  }

  CurveBiquadratic  c1_2(m);
  CurveQuadratic  c2_2(m);
  CurveBiquadratic  c3_2(m);
  CurveQuadratic  c4_2(m);
  CurveQuadratic  c5_2(m);

  CoilHeatingDXSingleSpeed testObject3(m, s, c1_2, c2_2, c3_2, c4_2, c5_2);

  if( boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode() ) {
    EXPECT_FALSE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)1, outdoorAirSystem.reliefComponents().size() );
  }

  CoilHeatingDXSingleSpeed testObjectClone = testObject.clone(m).cast<CoilHeatingDXSingleSpeed>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );
}
