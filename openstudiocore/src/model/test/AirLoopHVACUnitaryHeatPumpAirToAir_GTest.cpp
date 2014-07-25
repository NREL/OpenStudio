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
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../Schedule.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,AirLoopHVACUnitaryHeatPumpAirToAir_AirLoopHVACUnitaryHeatPumpAirToAir)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanConstantVolume supplyFan(m,s);

    CurveBiquadratic  ctotalHeatingCapacityFunctionofTemperatureCurve(m);
    CurveQuadratic  ctotalHeatingCapacityFunctionofFlowFractionCurve(m);
    CurveBiquadratic  cenergyInputRatioFunctionofTemperatureCurve(m);
    CurveQuadratic  cenergyInputRatioFunctionofFlowFractionCurve(m);
    CurveQuadratic  cpartLoadFractionCorrelationCurve(m);

    CoilHeatingDXSingleSpeed coolingCoil(m, s,
                                        ctotalHeatingCapacityFunctionofTemperatureCurve,
                                        ctotalHeatingCapacityFunctionofFlowFractionCurve,
                                        cenergyInputRatioFunctionofTemperatureCurve,
                                        cenergyInputRatioFunctionofFlowFractionCurve,
                                        cpartLoadFractionCorrelationCurve ); 

    CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
    CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
    CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
    CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
    CurveQuadratic  partLoadFractionCorrelationCurve(m);

    CoilHeatingDXSingleSpeed heatingCoil(m, s,
                                        totalHeatingCapacityFunctionofTemperatureCurve,
                                        totalHeatingCapacityFunctionofFlowFractionCurve,
                                        energyInputRatioFunctionofTemperatureCurve,
                                        energyInputRatioFunctionofFlowFractionCurve,
                                        partLoadFractionCorrelationCurve ); 

    CoilHeatingElectric coilHeatingElectric(m,s);

    AirLoopHVACUnitaryHeatPumpAirToAir coil(m, s, supplyFan, heatingCoil, coolingCoil, coilHeatingElectric);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirLoopHVACUnitaryHeatPumpAirToAir_addToNode)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m,s);

  CurveBiquadratic  ctotalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic  ctotalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic  cenergyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic  cenergyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic  cpartLoadFractionCorrelationCurve(m);

  CoilCoolingDXSingleSpeed coolingCoil(m, s,
                                      ctotalHeatingCapacityFunctionofTemperatureCurve,
                                      ctotalHeatingCapacityFunctionofFlowFractionCurve,
                                      cenergyInputRatioFunctionofTemperatureCurve,
                                      cenergyInputRatioFunctionofFlowFractionCurve,
                                      cpartLoadFractionCorrelationCurve ); 

  CurveBiquadratic  totalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic  totalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic  energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic  partLoadFractionCorrelationCurve(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s,
                                      totalHeatingCapacityFunctionofTemperatureCurve,
                                      totalHeatingCapacityFunctionofFlowFractionCurve,
                                      energyInputRatioFunctionofTemperatureCurve,
                                      energyInputRatioFunctionofFlowFractionCurve,
                                      partLoadFractionCorrelationCurve ); 

  CoilHeatingElectric coilHeatingElectric(m,s);

  AirLoopHVACUnitaryHeatPumpAirToAir testObject(m, s, supplyFan, heatingCoil, coolingCoil, coilHeatingElectric);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)3, airLoop.supplyComponents().size() );

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

  AirLoopHVACUnitaryHeatPumpAirToAir testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
}
