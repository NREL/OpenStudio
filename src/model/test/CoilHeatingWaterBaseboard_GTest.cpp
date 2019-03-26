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
