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
#include "../CoilCoolingDXTwoSpeed.hpp"
#include "../CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../Curve.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../LifeCycleCost.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

//Test construction of the coil, then check that fields in default constructor
//are being set to the expected values
TEST_F(ModelFixture,CoilCoolingDXTwoSpeed_CoilCoolingDXTwoSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      //create a model to use in testing this code.
      Model m;

      //create a schedule and the curves to use in the constructor
      Schedule s = m.alwaysOnDiscreteSchedule();
      CurveBiquadratic ccfot(m);
      CurveCubic ccfof(m);
      CurveBiquadratic eirfot(m);
      CurveQuadratic eirfof(m);
      CurveCubic plf(m);
      CurveBiquadratic lsccfot(m);
      CurveBiquadratic lseirfot(m);

      //make a coil to do the testing on
      CoilCoolingDXTwoSpeed coil(m, s, ccfot, ccfof, eirfot, eirfof, plf, lsccfot, lseirfot);
      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );

      //create a model to use in testing this code.
      Model m;

      //create a schedule and the curves to use in the constructor
      Schedule s = m.alwaysOnDiscreteSchedule();
      CurveBiquadratic ccfot(m);
      CurveCubic ccfof(m);
      CurveBiquadratic eirfot(m);
      CurveQuadratic eirfof(m);
      CurveCubic plf(m);
      CurveBiquadratic lsccfot(m);
      CurveBiquadratic lseirfot(m);

      //make a coil to do the testing on
      CoilCoolingDXTwoSpeed coil(m, s, ccfot, ccfof, eirfot, eirfof, plf, lsccfot, lseirfot);

    //ASSERT_EQ("Autosize",coil.getRatedHighSpeedTotalCoolingCapacity().get());
    ASSERT_EQ(3,coil.getRatedHighSpeedCOP().get());
    ASSERT_EQ(3,coil.getRatedLowSpeedCOP().get());
    ASSERT_EQ(0,coil.getHighSpeedEvaporativeCondenserEffectiveness().get());
    ASSERT_EQ(0,coil.getLowSpeedEvaporativeCondenserEffectiveness().get());
    ASSERT_EQ(0,coil.getBasinHeaterCapacity().get());
    ASSERT_EQ(10,coil.getBasinHeaterSetpointTemperature().get());
    ASSERT_EQ("AirCooled",coil.getCondenserType());
}

//Test cloning the coil
TEST_F(ModelFixture,CoilCoolingDXTwoSpeed_Clone)
{
    //create a model to use in testing this code.
    Model m;

    //create a schedule and the curves to use in the constructor
    Schedule s = m.alwaysOnDiscreteSchedule();
    CurveBiquadratic ccfot1(m);
      ccfot1.setCoefficient1Constant(1);
      ccfot1.setCoefficient2x(1);
      ccfot1.setCoefficient3xPOW2(1);
    CurveCubic ccfof2(m);
      ccfof2.setCoefficient1Constant(2);
      ccfof2.setCoefficient2x(2);
      ccfof2.setCoefficient3xPOW2(2);
    CurveBiquadratic eirfot3(m);
      eirfot3.setCoefficient1Constant(3);
      eirfot3.setCoefficient2x(3);
      eirfot3.setCoefficient3xPOW2(3);
    CurveQuadratic eirfof4(m);
      eirfof4.setCoefficient1Constant(4);
      eirfof4.setCoefficient2x(4);
      eirfof4.setCoefficient3xPOW2(4);
    CurveCubic plf5(m);
      plf5.setCoefficient1Constant(5);
      plf5.setCoefficient2x(5);
      plf5.setCoefficient3xPOW2(5);
    CurveBiquadratic lsccfot6(m);
      lsccfot6.setCoefficient1Constant(6);
      lsccfot6.setCoefficient2x(6);
      lsccfot6.setCoefficient3xPOW2(6);
    CurveBiquadratic lseirfot7(m);
      lseirfot7.setCoefficient1Constant(7);
      lseirfot7.setCoefficient2x(7);
      lseirfot7.setCoefficient3xPOW2(7);

    //make a coil to do the testing on
    CoilCoolingDXTwoSpeed coil(m, s, ccfot1, ccfof2, eirfot3, eirfof4, plf5, lsccfot6, lseirfot7);

    //Clone into the same model
    CoilCoolingDXTwoSpeed  coilClone = coil.clone(m).cast<CoilCoolingDXTwoSpeed>();

    ASSERT_EQ(1,coilClone.getTotalCoolingCapacityFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
    ASSERT_EQ(2,coilClone.getTotalCoolingCapacityFunctionOfFlowFractionCurve().cast<CurveCubic>().coefficient1Constant());
    ASSERT_EQ(3,coilClone.getEnergyInputRatioFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
    ASSERT_EQ(4,coilClone.getEnergyInputRatioFunctionOfFlowFractionCurve().cast<CurveQuadratic>().coefficient1Constant());
    ASSERT_EQ(5,coilClone.getPartLoadFractionCorrelationCurve().cast<CurveCubic>().coefficient1Constant());
    ASSERT_EQ(6,coilClone.getLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
    ASSERT_EQ(7,coilClone.getLowSpeedEnergyInputRatioFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());

    //Clone into another model
    Model m2;
    CoilCoolingDXTwoSpeed  coilClone2 = coil.clone(m2).cast<CoilCoolingDXTwoSpeed>();

    ASSERT_EQ(1,coilClone2.getTotalCoolingCapacityFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
    ASSERT_EQ(2,coilClone2.getTotalCoolingCapacityFunctionOfFlowFractionCurve().cast<CurveCubic>().coefficient1Constant());
    ASSERT_EQ(3,coilClone2.getEnergyInputRatioFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
    ASSERT_EQ(4,coilClone2.getEnergyInputRatioFunctionOfFlowFractionCurve().cast<CurveQuadratic>().coefficient1Constant());
    ASSERT_EQ(5,coilClone2.getPartLoadFractionCorrelationCurve().cast<CurveCubic>().coefficient1Constant());
    ASSERT_EQ(6,coilClone2.getLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
    ASSERT_EQ(7,coilClone2.getLowSpeedEnergyInputRatioFunctionOfTemperatureCurve().cast<CurveBiquadratic>().coefficient1Constant());
}

//Test the methods that set and get all the fields
TEST_F(ModelFixture,CoilCoolingDXTwoSpeed_SetGetFields)
{
    //create a model to use in testing this code.
    Model m;

    //create a schedule and the curves to use in the constructor
    Schedule s = m.alwaysOnDiscreteSchedule();
    CurveBiquadratic ccfot1(m);
    CurveCubic ccfof2(m);
    CurveBiquadratic eirfot3(m);
    CurveQuadratic eirfof4(m);
    CurveCubic plf5(m);
    CurveBiquadratic lsccfot6(m);
    CurveBiquadratic lseirfot7(m);

    //make a coil to do the testing on
    CoilCoolingDXTwoSpeed coil(m, s, ccfot1, ccfof2, eirfot3, eirfof4, plf5, lsccfot6, lseirfot7);

    //  A2 , \field Availability Schedule Name
    ScheduleCompact availSch(m);
    coil.setAvailabilitySchedule(availSch);
    ASSERT_EQ(availSch,coil.getAvailabilitySchedule());

    //  N1 , \field Rated High Speed Total Cooling Capacity
    coil.setRatedHighSpeedTotalCoolingCapacity(999);
    ASSERT_EQ(999,coil.getRatedHighSpeedTotalCoolingCapacity().get());

    //  N2 , \field Rated High Speed Sensible Heat Ratio
    coil.setRatedHighSpeedSensibleHeatRatio(0.999);
    ASSERT_EQ(0.999,coil.getRatedHighSpeedSensibleHeatRatio().get());

     //  N3 , \field Rated High Speed COP
    coil.setRatedHighSpeedCOP(0.999);
    ASSERT_EQ(0.999,coil.getRatedHighSpeedCOP().get());

    //  N4 , \field Rated High Speed Air Flow Rate
    coil.setRatedHighSpeedAirFlowRate(999);
    ASSERT_EQ(999,coil.getRatedHighSpeedAirFlowRate().get());

    //create some curves for testing
    CurveBiquadratic  testBiquadratic(m);
    CurveCubic  testCubic(m);
    CurveQuadratic  testQuadratic(m);

    //  A5 , \field Total Cooling Capacity Function of Temperature Curve Name
    //\object-list BiquadraticCurves
    ASSERT_TRUE(coil.setTotalCoolingCapacityFunctionOfTemperatureCurve(testBiquadratic));
    ASSERT_EQ(testBiquadratic,coil.getTotalCoolingCapacityFunctionOfTemperatureCurve());

    //  A6 , \field Total Cooling Capacity Function of Flow Fraction Curve Name
    //\object-list QuadraticCubicCurves
    ASSERT_TRUE(coil.setTotalCoolingCapacityFunctionOfFlowFractionCurve(testQuadratic));
    ASSERT_EQ(testQuadratic,coil.getTotalCoolingCapacityFunctionOfFlowFractionCurve());
    ASSERT_TRUE(coil.setTotalCoolingCapacityFunctionOfFlowFractionCurve(testCubic));
    ASSERT_EQ(testCubic,coil.getTotalCoolingCapacityFunctionOfFlowFractionCurve());

    //  A7 , \field Energy Input Ratio Function of Temperature Curve Name
    //\object-list BiquadraticCurves
    ASSERT_TRUE(coil.setEnergyInputRatioFunctionOfTemperatureCurve(testBiquadratic));
    ASSERT_EQ(testBiquadratic,coil.getEnergyInputRatioFunctionOfTemperatureCurve());

    //  A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
    //\object-list QuadraticCubicCurves
    ASSERT_TRUE(coil.setEnergyInputRatioFunctionOfFlowFractionCurve(testQuadratic));
    ASSERT_EQ(testQuadratic,coil.getEnergyInputRatioFunctionOfFlowFractionCurve());
    ASSERT_TRUE(coil.setEnergyInputRatioFunctionOfFlowFractionCurve(testCubic));
    ASSERT_EQ(testCubic,coil.getEnergyInputRatioFunctionOfFlowFractionCurve());

    //  A9 , \field Part Load Fraction Correlation Curve Name
    //\object-list QuadraticCubicCurves
    ASSERT_TRUE(coil.setPartLoadFractionCorrelationCurve(testQuadratic));
    ASSERT_EQ(testQuadratic,coil.getPartLoadFractionCorrelationCurve());
    ASSERT_TRUE(coil.setPartLoadFractionCorrelationCurve(testCubic));
    ASSERT_EQ(testCubic,coil.getPartLoadFractionCorrelationCurve());

    //  N5 , \field Rated Low Speed Total Cooling Capacity
    coil.setRatedLowSpeedTotalCoolingCapacity(999);
    ASSERT_EQ(999,coil.getRatedLowSpeedTotalCoolingCapacity().get());

    //  N6 , \field Rated Low Speed Sensible Heat Ratio
    coil.setRatedLowSpeedSensibleHeatRatio(0.999);
    ASSERT_EQ(0.999,coil.getRatedLowSpeedSensibleHeatRatio().get());

    //  N7 , \field Rated Low Speed COP
    coil.setRatedLowSpeedCOP(0.999);
    ASSERT_EQ(0.999,coil.getRatedLowSpeedCOP().get());

    //  N8 , \field Rated Low Speed Air Flow Rate
    coil.setRatedLowSpeedAirFlowRate(999);
    ASSERT_EQ(999,coil.getRatedLowSpeedAirFlowRate().get());

    //  A10, \field Low Speed Total Cooling Capacity Function of Temperature Curve Name
    //\object-list BiquadraticCurves
    ASSERT_TRUE(coil.setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(testBiquadratic));
    ASSERT_EQ(testBiquadratic,coil.getLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve());

    //  A11, \field Low Speed Energy Input Ratio Function of Temperature Curve Name
    //\object-list BiquadraticCurves
    ASSERT_TRUE(coil.setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(testBiquadratic));
    ASSERT_EQ(testBiquadratic,coil.getLowSpeedEnergyInputRatioFunctionOfTemperatureCurve());

    //  A12, \field Condenser Air Inlet Node Name
    coil.setCondenserAirInletNodeName("testNode");
    ASSERT_EQ("testNode",coil.getCondenserAirInletNodeName().get());

    //  A13, \field Condenser Type
    coil.setCondenserType("EvaporativelyCooled");
    ASSERT_EQ("EvaporativelyCooled",coil.getCondenserType());

    //   N9, \field High Speed Evaporative Condenser Effectiveness
    coil.setHighSpeedEvaporativeCondenserEffectiveness(0.999);
    ASSERT_EQ(0.999,coil.getHighSpeedEvaporativeCondenserEffectiveness().get());

    //  N10, \field High Speed Evaporative Condenser Air Flow Rate
    coil.setHighSpeedEvaporativeCondenserAirFlowRate(999);
    ASSERT_EQ(999,coil.getHighSpeedEvaporativeCondenserAirFlowRate().get());

    //  N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
    coil.setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(999);
    ASSERT_EQ(999,coil.getHighSpeedEvaporativeCondenserPumpRatedPowerConsumption().get());

    //  N12, \field Low Speed Evaporative Condenser Effectiveness
    coil.setLowSpeedEvaporativeCondenserEffectiveness(0.999);
    ASSERT_EQ(0.999,coil.getLowSpeedEvaporativeCondenserEffectiveness().get());

    //  N13, \field Low Speed Evaporative Condenser Air Flow Rate
    coil.setLowSpeedEvaporativeCondenserAirFlowRate(999);
    ASSERT_EQ(999,coil.getLowSpeedEvaporativeCondenserAirFlowRate().get());

    //  N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
    coil.setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(999);
    ASSERT_EQ(999,coil.getLowSpeedEvaporativeCondenserPumpRatedPowerConsumption().get());

    //  A14, \field Supply Water Storage Tank Name
    //getSupplyWaterStorageTankName
    //setSupplyWaterStorageTankName

    //  A15, \field Condensate Collection Water Storage Tank Name
    //getCondensateCollectionWaterStorageTankName
    //setCondensateCollectionWaterStorageTankName

    //  N15, \field Basin Heater Capacity
    coil.setBasinHeaterCapacity(999);
    ASSERT_EQ(999,coil.getBasinHeaterCapacity().get());

    //  N16, \field Basin Heater Setpoint Temperature
    coil.setBasinHeaterSetpointTemperature(35);
    ASSERT_EQ(35,coil.getBasinHeaterSetpointTemperature().get());

    //  A16; \field Basin Heater Operating Schedule Name
    ScheduleCompact basinSch(m);
    coil.setBasinHeaterOperatingSchedule(basinSch);
    ASSERT_EQ(basinSch,coil.getBasinHeaterOperatingSchedule().get());
}

//Test adding LifeCycleCost
TEST_F(ModelFixture, CoilCoolingDXTwoSpeed_Cost)
{
    //create a model to use in testing this code.
    Model m;

    //create a schedule and the curves to use in the constructor
    Schedule s = m.alwaysOnDiscreteSchedule();
    CurveBiquadratic ccfot1(m);
    CurveCubic ccfof2(m);
    CurveBiquadratic eirfot3(m);
    CurveQuadratic eirfof4(m);
    CurveCubic plf5(m);
    CurveBiquadratic lsccfot6(m);
    CurveBiquadratic lseirfot7(m);

    //make a coil to do the testing on
    CoilCoolingDXTwoSpeed coil(m, s, ccfot1, ccfof2, eirfot3, eirfof4, plf5, lsccfot6, lseirfot7);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Install", coil, 1000.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost1);

  EXPECT_DOUBLE_EQ(1000.0, cost1->totalCost());
}

TEST_F(ModelFixture, CoilCoolingDXTwoSpeed_addToNode)
{
  //create a model to use in testing this code.
  Model m;

  //create a schedule and the curves to use in the constructor
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic ccfot1(m);
  CurveCubic ccfof2(m);
  CurveBiquadratic eirfot3(m);
  CurveQuadratic eirfof4(m);
  CurveCubic plf5(m);
  CurveBiquadratic lsccfot6(m);
  CurveBiquadratic lseirfot7(m);

  //make a coil to do the testing on
  CoilCoolingDXTwoSpeed testObject(m, s, ccfot1, ccfof2, eirfot3, eirfof4, plf5, lsccfot6, lseirfot7);

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
  CurveCubic  c2(m);
  CurveBiquadratic  c3(m);
  CurveQuadratic  c4(m);
  CurveCubic  c5(m);
  CurveBiquadratic c6(m);
  CurveBiquadratic c7(m);

  CoilCoolingDXTwoSpeed testObject2(m, s, c1, c2, c3, c4, c5, c6, c7);

  if( boost::optional<Node> OANode = outdoorAirSystem.outboardOANode() ) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)3, outdoorAirSystem.oaComponents().size() );
  }

  CurveBiquadratic  c1_2(m);
  CurveCubic  c2_2(m);
  CurveBiquadratic  c3_2(m);
  CurveQuadratic  c4_2(m);
  CurveCubic  c5_2(m);
  CurveBiquadratic c6_2(m);
  CurveBiquadratic c7_2(m);

  CoilCoolingDXTwoSpeed testObject3(m, s, c1_2, c2_2, c3_2, c4_2, c5_2, c6_2, c7_2);

  if( boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode() ) {
    EXPECT_FALSE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)1, outdoorAirSystem.reliefComponents().size() );
  }

  CoilCoolingDXTwoSpeed testObjectClone = testObject.clone(m).cast<CoilCoolingDXTwoSpeed>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );
}
