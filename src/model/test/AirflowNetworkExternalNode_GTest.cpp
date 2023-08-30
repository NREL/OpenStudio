/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkExternalNode.hpp"
#include "../AirflowNetworkExternalNode_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_ExternalNode) {
  Model model;

  AirflowNetworkExternalNode extnode0(model);
  auto cpt_ = extnode0.windPressureCoefficientCurve().optionalCast<CurveLinear>();
  ASSERT_TRUE(cpt_);
  EXPECT_EQ(1.0, cpt_->coefficient1Constant());
  EXPECT_EQ(0.0, cpt_->coefficient2x());

  EXPECT_TRUE(extnode0.isExternalNodeHeightDefaulted());
  EXPECT_TRUE(extnode0.isSymmetricWindPressureCoefficientCurveDefaulted());
  EXPECT_EQ("Absolute", extnode0.windAngleType());
  EXPECT_TRUE(extnode0.isWindAngleTypeDefaulted());
  EXPECT_TRUE(extnode0.setWindAngleType("Relative"));
  EXPECT_EQ("Relative", extnode0.windAngleType());
  EXPECT_FALSE(extnode0.isWindAngleTypeDefaulted());

  CurveLinear curve1(model);
  curve1.setCoefficient1Constant(0.5);
  curve1.setCoefficient2x(0.0);
  curve1.setMinimumValueofx(0.0);
  curve1.setMaximumValueofx(360.0);

  extnode0.setWindPressureCoefficientCurve(curve1);
  EXPECT_EQ(curve1, extnode0.windPressureCoefficientCurve());

  AirflowNetworkExternalNode extnode1(model, curve1);
  EXPECT_EQ(curve1, extnode1.windPressureCoefficientCurve());
}
