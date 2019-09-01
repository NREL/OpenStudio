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

#include "../ShadowCalculation.hpp"
#include "../ShadowCalculation_Impl.hpp"


using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ShadowCalculation) {
  Model model;

  ShadowCalculation sc = model.getUniqueModelObject<ShadowCalculation>();

  EXPECT_TRUE(sc.isCalculationMethodDefaulted());
  EXPECT_EQ("AverageOverDaysInFrequency", sc.calculationMethod());
  EXPECT_FALSE(sc.isCalculationFrequencyDefaulted()); // set in constructor
  EXPECT_EQ(20, sc.calculationFrequency());
  EXPECT_FALSE(sc.isMaximumFiguresInShadowOverlapCalculationsDefaulted()); // set in constructor
  EXPECT_EQ(15000, sc.maximumFiguresInShadowOverlapCalculations());
  ASSERT_TRUE(sc.polygonClippingAlgorithm());
  EXPECT_EQ("SutherlandHodgman", sc.polygonClippingAlgorithm().get());
  ASSERT_TRUE(sc.skyDiffuseModelingAlgorithm());
  EXPECT_EQ("SimpleSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm().get());

  EXPECT_TRUE(sc.setCalculationMethod("TimestepFrequency"));
  EXPECT_TRUE(sc.setCalculationFrequency(1));
  EXPECT_TRUE(sc.setMaximumFiguresInShadowOverlapCalculations(200));
  EXPECT_TRUE(sc.setPolygonClippingAlgorithm("ConvexWeilerAtherton"));
  EXPECT_TRUE(sc.setSkyDiffuseModelingAlgorithm("DetailedSkyDiffuseModeling"));

  EXPECT_FALSE(sc.isCalculationMethodDefaulted());
  EXPECT_EQ("TimestepFrequency", sc.calculationMethod());
  EXPECT_FALSE(sc.isCalculationFrequencyDefaulted());
  EXPECT_EQ(1, sc.calculationFrequency());
  EXPECT_FALSE(sc.isMaximumFiguresInShadowOverlapCalculationsDefaulted());
  EXPECT_EQ(200, sc.maximumFiguresInShadowOverlapCalculations());
  ASSERT_TRUE(sc.polygonClippingAlgorithm());
  EXPECT_EQ("ConvexWeilerAtherton", sc.polygonClippingAlgorithm().get());
  ASSERT_TRUE(sc.skyDiffuseModelingAlgorithm());
  EXPECT_EQ("DetailedSkyDiffuseModeling", sc.skyDiffuseModelingAlgorithm().get());
}
