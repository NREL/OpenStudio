/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../CurveFunctionalPressureDrop.hpp"
#include "../CurveFunctionalPressureDrop_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveFunctionalPressureDrop_GettersSetters) {
  Model m;

  CurveFunctionalPressureDrop curveFunctionalPressureDrop(m);
  curveFunctionalPressureDrop.setName("My CurveFunctionalPressureDrop");

  // Diameter: Required Double
  EXPECT_TRUE(curveFunctionalPressureDrop.setDiameter(0.1));
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.diameter());
  // Bad Value
  EXPECT_FALSE(curveFunctionalPressureDrop.setDiameter(-10.0));
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.diameter());

  // Minor Loss Coefficient: Optional Double
  EXPECT_TRUE(curveFunctionalPressureDrop.setMinorLossCoefficient(0.1));
  ASSERT_TRUE(curveFunctionalPressureDrop.minorLossCoefficient());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.minorLossCoefficient().get());
  // Bad Value
  EXPECT_FALSE(curveFunctionalPressureDrop.setMinorLossCoefficient(-10.0));
  ASSERT_TRUE(curveFunctionalPressureDrop.minorLossCoefficient());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.minorLossCoefficient().get());
  // reset
  curveFunctionalPressureDrop.resetMinorLossCoefficient();
  EXPECT_FALSE(curveFunctionalPressureDrop.minorLossCoefficient());

  // Length: Optional Double
  EXPECT_TRUE(curveFunctionalPressureDrop.setLength(0.1));
  ASSERT_TRUE(curveFunctionalPressureDrop.length());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.length().get());
  // Bad Value
  EXPECT_FALSE(curveFunctionalPressureDrop.setLength(-10.0));
  ASSERT_TRUE(curveFunctionalPressureDrop.length());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.length().get());
  // reset
  curveFunctionalPressureDrop.resetLength();
  EXPECT_FALSE(curveFunctionalPressureDrop.length());

  // Roughness: Optional Double
  EXPECT_TRUE(curveFunctionalPressureDrop.setRoughness(0.1));
  ASSERT_TRUE(curveFunctionalPressureDrop.roughness());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.roughness().get());
  // Bad Value
  EXPECT_FALSE(curveFunctionalPressureDrop.setRoughness(-10.0));
  ASSERT_TRUE(curveFunctionalPressureDrop.roughness());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.roughness().get());
  // reset
  curveFunctionalPressureDrop.resetRoughness();
  EXPECT_FALSE(curveFunctionalPressureDrop.roughness());

  // Fixed Friction Factor: Optional Double
  EXPECT_TRUE(curveFunctionalPressureDrop.setFixedFrictionFactor(0.1));
  ASSERT_TRUE(curveFunctionalPressureDrop.fixedFrictionFactor());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.fixedFrictionFactor().get());
  // Bad Value
  EXPECT_FALSE(curveFunctionalPressureDrop.setFixedFrictionFactor(-10.0));
  ASSERT_TRUE(curveFunctionalPressureDrop.fixedFrictionFactor());
  EXPECT_EQ(0.1, curveFunctionalPressureDrop.fixedFrictionFactor().get());
  // reset
  curveFunctionalPressureDrop.resetFixedFrictionFactor();
  EXPECT_FALSE(curveFunctionalPressureDrop.fixedFrictionFactor());

  EXPECT_ANY_THROW(curveFunctionalPressureDrop.numVariables());
  EXPECT_ANY_THROW(curveFunctionalPressureDrop.evaluate(10));
}
