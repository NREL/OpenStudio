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

