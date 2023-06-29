/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkHorizontalOpening.hpp"
#include "../AirflowNetworkHorizontalOpening_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_HorizontalOpening) {
  Model model;

  AirflowNetworkHorizontalOpening horizontal0(model, 1.0, 0.5);
  AirflowNetworkHorizontalOpening horizontal1(model, 1.0, 0.5, 0.5, 0.5);

  EXPECT_EQ(1, horizontal0.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.65, horizontal0.airMassFlowExponentWhenOpeningisClosed());
  EXPECT_EQ(90, horizontal0.slopingPlaneAngle());
  EXPECT_EQ(0.5, horizontal0.dischargeCoefficient());

  EXPECT_EQ(1, horizontal1.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.5, horizontal1.airMassFlowExponentWhenOpeningisClosed());
  EXPECT_EQ(0.5, horizontal1.slopingPlaneAngle());
  EXPECT_EQ(0.5, horizontal1.dischargeCoefficient());
}
