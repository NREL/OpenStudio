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
#include "../BoilerSteam.hpp"
#include "../BoilerSteam_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,BoilerSteam_BoilerSteam)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     BoilerSteam boiler(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,BoilerSteam_addToNode) {
  Model m;
  BoilerSteam testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  BoilerSteam testObjectClone = testObject.clone(m).cast<BoilerSteam>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
}

TEST_F(ModelFixture,BoilerSteam_MaximumOperatingPressure_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getMaximumOperatingPressure(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setMaximumOperatingPressure(testQ));
  OSOptionalQuantity q = boilerSteam.getMaximumOperatingPressure(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_TheoreticalEfficiency_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getTheoreticalEfficiency(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setTheoreticalEfficiency(testQ));
  OSOptionalQuantity q = boilerSteam.getTheoreticalEfficiency(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_DesignOutletSteamTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getDesignOutletSteamTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setDesignOutletSteamTemperature(testQ));
  OSOptionalQuantity q = boilerSteam.getDesignOutletSteamTemperature(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_NominalCapacity_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getNominalCapacity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setNominalCapacity(testQ));
  OSOptionalQuantity q = boilerSteam.getNominalCapacity(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_MinimumPartLoadRatio_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getMinimumPartLoadRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setMinimumPartLoadRatio(testQ));
  OSOptionalQuantity q = boilerSteam.getMinimumPartLoadRatio(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_MaximumPartLoadRatio_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getMaximumPartLoadRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setMaximumPartLoadRatio(testQ));
  OSOptionalQuantity q = boilerSteam.getMaximumPartLoadRatio(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_OptimumPartLoadRatio_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getOptimumPartLoadRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setOptimumPartLoadRatio(testQ));
  OSOptionalQuantity q = boilerSteam.getOptimumPartLoadRatio(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_Coefficient1ofFuelUseFunctionofPartLoadRatioCurve_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(testQ));
  OSOptionalQuantity q = boilerSteam.getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_Coefficient2ofFuelUseFunctionofPartLoadRatioCurve_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(testQ));
  OSOptionalQuantity q = boilerSteam.getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_Coefficient3ofFuelUseFunctionofPartLoadRatioCurve_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(testQ));
  OSOptionalQuantity q = boilerSteam.getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,BoilerSteam_SizingFactor_Quantity) {
  Model model;
  // TODO: Check constructor.
  BoilerSteam boilerSteam(model);

  Unit units = boilerSteam.getSizingFactor(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(boilerSteam.setSizingFactor(testQ));
  Quantity q = boilerSteam.getSizingFactor(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

