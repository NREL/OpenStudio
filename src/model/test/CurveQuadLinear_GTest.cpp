/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../CurveQuadLinear.hpp"
#include "../CurveQuadLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveQuadLinear_GettersSetters) {
  Model m;

  CurveQuadLinear curveQuadLinear(m);

  curveQuadLinear.setName("My CurveQuadLinear");

  // Coefficient1 Constant: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient1Constant(0.1));
  EXPECT_EQ(0.1, curveQuadLinear.coefficient1Constant());

  // Coefficient2 w: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient2w(0.2));
  EXPECT_EQ(0.2, curveQuadLinear.coefficient2w());

  // Coefficient3 x: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient3x(0.3));
  EXPECT_EQ(0.3, curveQuadLinear.coefficient3x());

  // Coefficient4 y: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient4y(0.4));
  EXPECT_EQ(0.4, curveQuadLinear.coefficient4y());

  // Coefficient5 z: Required Double
  EXPECT_TRUE(curveQuadLinear.setCoefficient5z(0.5));
  EXPECT_EQ(0.5, curveQuadLinear.coefficient5z());

  // Minimum Value of w: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofw(2.0));
  EXPECT_EQ(2.0, curveQuadLinear.minimumValueofw());

  // Maximum Value of w: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofw(12.0));
  EXPECT_EQ(12.0, curveQuadLinear.maximumValueofw());

  // Minimum Value of x: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofx(3.0));
  EXPECT_EQ(3.0, curveQuadLinear.minimumValueofx());

  // Maximum Value of x: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofx(13.0));
  EXPECT_EQ(13.0, curveQuadLinear.maximumValueofx());

  // Minimum Value of y: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofy(4.0));
  EXPECT_EQ(4.0, curveQuadLinear.minimumValueofy());

  // Maximum Value of y: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofy(14.0));
  EXPECT_EQ(14.0, curveQuadLinear.maximumValueofy());

  // Minimum Value of z: Required Double
  EXPECT_TRUE(curveQuadLinear.setMinimumValueofz(5.0));
  EXPECT_EQ(5.0, curveQuadLinear.minimumValueofz());

  // Maximum Value of z: Required Double
  EXPECT_TRUE(curveQuadLinear.setMaximumValueofz(15.0));
  EXPECT_EQ(15.0, curveQuadLinear.maximumValueofz());

  // Minimum Curve Output: Optional Double
  EXPECT_TRUE(curveQuadLinear.setMinimumCurveOutput(0.015));
  ASSERT_TRUE(curveQuadLinear.minimumCurveOutput());
  EXPECT_EQ(0.015, curveQuadLinear.minimumCurveOutput().get());

  // Maximum Curve Output: Optional Double
  EXPECT_TRUE(curveQuadLinear.setMaximumCurveOutput(0.98));
  ASSERT_TRUE(curveQuadLinear.maximumCurveOutput());
  EXPECT_EQ(0.98, curveQuadLinear.maximumCurveOutput().get());

  // Input Unit Type for w: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypeforw("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforw());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypeforw("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforw());

  // Input Unit Type for x: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypeforx("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforx());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypeforx("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforx());

  // Input Unit Type for y: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypefory("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypefory());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypefory("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypefory());

  // Input Unit Type for z: Optional String
  EXPECT_TRUE(curveQuadLinear.setInputUnitTypeforz("Dimensionless"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforz());
  // Bad Value
  EXPECT_FALSE(curveQuadLinear.setInputUnitTypeforz("BADENUM"));
  EXPECT_EQ("Dimensionless", curveQuadLinear.inputUnitTypeforz());
}

TEST_F(ModelFixture, CurveQuadLinear_evaluate) {
  Model m;

  CurveQuadLinear c(m);
  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double c5 = 5.0;
  c.setCoefficient1Constant(c1);
  c.setCoefficient2w(c2);
  c.setCoefficient3x(c3);
  c.setCoefficient4y(c4);
  c.setCoefficient5z(c5);

  double w = 0.1;
  double x = 0.2;
  double y = 0.3;
  double z = 0.4;

  double result = c.evaluate(w, x, y, z);
  double expectedResult = c1 + c2 * w + c3 * x + c4 * y + c5 * z;
  EXPECT_EQ(expectedResult, result);
}
