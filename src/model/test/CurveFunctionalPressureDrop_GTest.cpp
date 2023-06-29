/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
