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

TEST_F(ModelFixture,BoilerSteam_GettersSetters)
{

  Model m;
  BoilerSteam boiler(m);

  // Fuel Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(boiler.setFuelType("Electricity"));
  EXPECT_EQ("Electricity", boiler.fuelType());
  // Test an invalid choice
  EXPECT_FALSE(boiler.setFuelType("BadChoice"));
  EXPECT_EQ("Electricity", boiler.fuelType());


  // Maximum Operating Pressure: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setMaximumOperatingPressure(10.03));
  ASSERT_TRUE(boiler.maximumOperatingPressure());
  EXPECT_EQ(10.03, boiler.maximumOperatingPressure().get());
  boiler.resetMaximumOperatingPressure();
  EXPECT_FALSE(boiler.maximumOperatingPressure());


  // Theoretical Efficiency: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setTheoreticalEfficiency(0.5));
  ASSERT_TRUE(boiler.theoreticalEfficiency());
  EXPECT_EQ(0.5, boiler.theoreticalEfficiency().get());
  boiler.resetTheoreticalEfficiency();
  EXPECT_FALSE(boiler.theoreticalEfficiency());


  // Design Outlet Steam Temperature: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setDesignOutletSteamTemperature(10.03));
  ASSERT_TRUE(boiler.designOutletSteamTemperature());
  EXPECT_EQ(10.03, boiler.designOutletSteamTemperature().get());
  boiler.resetDesignOutletSteamTemperature();
  EXPECT_FALSE(boiler.designOutletSteamTemperature());


  // Nominal Capacity: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setNominalCapacity(10.03));
  ASSERT_TRUE(boiler.nominalCapacity());
  EXPECT_EQ(10.03, boiler.nominalCapacity().get());
  boiler.resetNominalCapacity();
  EXPECT_FALSE(boiler.nominalCapacity());


  // Minimum Part Load Ratio: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setMinimumPartLoadRatio(1.0));
  ASSERT_TRUE(boiler.minimumPartLoadRatio());
  EXPECT_EQ(1.0, boiler.minimumPartLoadRatio().get());
  boiler.resetMinimumPartLoadRatio();
  EXPECT_FALSE(boiler.minimumPartLoadRatio());


  // Maximum Part Load Ratio: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setMaximumPartLoadRatio(1.0));
  ASSERT_TRUE(boiler.maximumPartLoadRatio());
  EXPECT_EQ(1.0, boiler.maximumPartLoadRatio().get());
  boiler.resetMaximumPartLoadRatio();
  EXPECT_FALSE(boiler.maximumPartLoadRatio());


  // Optimum Part Load Ratio: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setOptimumPartLoadRatio(1.0));
  ASSERT_TRUE(boiler.optimumPartLoadRatio());
  EXPECT_EQ(1.0, boiler.optimumPartLoadRatio().get());
  boiler.resetOptimumPartLoadRatio();
  EXPECT_FALSE(boiler.optimumPartLoadRatio());


  // Coefficient 1 of Fuel Use Function of Part Load Ratio Curve: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(10.03));
  ASSERT_TRUE(boiler.coefficient1ofFuelUseFunctionofPartLoadRatioCurve());
  EXPECT_EQ(10.03, boiler.coefficient1ofFuelUseFunctionofPartLoadRatioCurve().get());
  boiler.resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();
  EXPECT_FALSE(boiler.coefficient1ofFuelUseFunctionofPartLoadRatioCurve());


  // Coefficient 2 of Fuel Use Function of Part Load Ratio Curve: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(10.03));
  ASSERT_TRUE(boiler.coefficient2ofFuelUseFunctionofPartLoadRatioCurve());
  EXPECT_EQ(10.03, boiler.coefficient2ofFuelUseFunctionofPartLoadRatioCurve().get());
  boiler.resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();
  EXPECT_FALSE(boiler.coefficient2ofFuelUseFunctionofPartLoadRatioCurve());


  // Coefficient 3 of Fuel Use Function of Part Load Ratio Curve: Optional Double
  // No Default
  EXPECT_TRUE(boiler.setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(10.03));
  ASSERT_TRUE(boiler.coefficient3ofFuelUseFunctionofPartLoadRatioCurve());
  EXPECT_EQ(10.03, boiler.coefficient3ofFuelUseFunctionofPartLoadRatioCurve().get());
  boiler.resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();
  EXPECT_FALSE(boiler.coefficient3ofFuelUseFunctionofPartLoadRatioCurve());


  // Water Inlet Node Name:  Object

  // Steam Outlet Node Name:  Object


  // Sizing Factor:  Double
  // Check Idd default: 1.0
  EXPECT_EQ(1.0, boiler.sizingFactor());
  EXPECT_TRUE(boiler.setSizingFactor(0.5));
  EXPECT_EQ(0.5, boiler.sizingFactor());
  boiler.resetSizingFactor();
  EXPECT_EQ(1.0, boiler.sizingFactor());


  // End-Use Subcategory:  String
  // Check Idd default: "General"
  EXPECT_EQ("General", boiler.endUseSubcategory());
  EXPECT_TRUE(boiler.setEndUseSubcategory("SteamBoiler"));
  EXPECT_EQ("SteamBoiler", boiler.endUseSubcategory());

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
