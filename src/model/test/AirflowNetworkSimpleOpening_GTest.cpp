/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkSimpleOpening.hpp"
#include "../AirflowNetworkSimpleOpening_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_SimpleOpening) {
  Model model;

  AirflowNetworkSimpleOpening simple0(model, 1.0, 0.5, 0.5);
  AirflowNetworkSimpleOpening simple1(model, 1.0, 0.5, 0.5, 0.5);

  EXPECT_EQ(1, simple0.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.5, simple0.minimumDensityDifferenceforTwoWayFlow());
  EXPECT_EQ(0.5, simple0.dischargeCoefficient());
  EXPECT_EQ(0.65, simple0.airMassFlowExponentWhenOpeningisClosed());

  EXPECT_EQ(1, simple1.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.5, simple1.minimumDensityDifferenceforTwoWayFlow());
  EXPECT_EQ(0.5, simple1.dischargeCoefficient());
  EXPECT_EQ(0.5, simple1.airMassFlowExponentWhenOpeningisClosed());
}
