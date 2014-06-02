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

#include "../CoilHeatingWaterBaseboard.hpp"
#include "../CoilHeatingWaterBaseboard_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingWaterBaseboard_Test) {
  Model model;
  // test constructor
  CoilHeatingWaterBaseboard coilHeatingWaterBaseboard(model);

  // test uFactorTimesAreaValue
  coilHeatingWaterBaseboard.setUFactorTimesAreaValue(500);
  boost::optional<double> testUAValue = coilHeatingWaterBaseboard.uFactorTimesAreaValue();
  EXPECT_EQ((*testUAValue),500);
 
  //default UFactorTimesAreaValue should be autosize
  EXPECT_FALSE(coilHeatingWaterBaseboard.isUFactorTimesAreaValueDefaulted());
  coilHeatingWaterBaseboard.resetUFactorTimesAreaValue();
  EXPECT_FALSE(coilHeatingWaterBaseboard.uFactorTimesAreaValue());
  EXPECT_TRUE(coilHeatingWaterBaseboard.isUFactorTimesAreaValueDefaulted());

  // test maximum water flow rate getter and setter
  coilHeatingWaterBaseboard.setMaximumWaterFlowRate(100);
  boost::optional<double> testMaxHWMFR = coilHeatingWaterBaseboard.maximumWaterFlowRate();
  EXPECT_EQ((*testMaxHWMFR),100);

  EXPECT_FALSE(coilHeatingWaterBaseboard.isMaximumWaterFlowRateAutosized());
  coilHeatingWaterBaseboard.resetMaximumWaterFlowRate();
  EXPECT_FALSE(coilHeatingWaterBaseboard.maximumWaterFlowRate());
  coilHeatingWaterBaseboard.autosizeMaximumWaterFlowRate();
  EXPECT_TRUE(coilHeatingWaterBaseboard.isMaximumWaterFlowRateAutosized());

  // test maximum water flow rate getter and setter
  coilHeatingWaterBaseboard.setConvergenceTolerance(0.005);
  boost::optional<double> testHCT = coilHeatingWaterBaseboard.convergenceTolerance();
  EXPECT_EQ((*testHCT),0.005);
 
  EXPECT_FALSE(coilHeatingWaterBaseboard.isConvergenceToleranceDefaulted());
  coilHeatingWaterBaseboard.resetConvergenceTolerance();
  EXPECT_EQ(coilHeatingWaterBaseboard.convergenceTolerance(),0.001);
  EXPECT_TRUE(coilHeatingWaterBaseboard.isConvergenceToleranceDefaulted());

  // test inlet and outlet ports
  // make a plant loop
    PlantLoop hotWaterPlant(model);

  // make a coil
    CoilHeatingWaterBaseboard coilBBConv(model);

  // find the empty plant node to add the coil
    Node node = hotWaterPlant.demandSplitter().lastOutletModelObject()->cast<Node>();

  // add the coil
    EXPECT_TRUE(coilBBConv.addToNode(node));
   
  // disconnect the coil and check if it works
    coilBBConv.disconnect();
  /* get a vector of demand equipment on the plantloop called hotwaterPlant,
     we know that plantloop.demandcomponents() returns a variable type std::vector<ModelObject>
     and we want to make sure that the vector is empty, because we have removed the coil */
  
  // variable type variable name = plant loop class object function(), no argument goes back to default
    std::vector<ModelObject> hotwaterdemandComponents = hotWaterPlant.demandComponents();
    EXPECT_EQ(hotwaterdemandComponents.size(),0);
}

TEST_F(ModelFixture,CoilHeatingWaterBaseboard_addToNode) {
  Model m;
  CoilHeatingWaterBaseboard testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );
}
