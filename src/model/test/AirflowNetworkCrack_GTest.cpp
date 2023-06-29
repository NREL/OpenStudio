/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkCrack.hpp"
#include "../AirflowNetworkCrack_Impl.hpp"
#include "../AirflowNetworkReferenceCrackConditions.hpp"
#include "../AirflowNetworkReferenceCrackConditions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_Crack) {
  Model model;

  AirflowNetworkCrack crack0(model, 1.0, 0.5);
  EXPECT_EQ(1, crack0.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack0.airMassFlowExponent());
  boost::optional<AirflowNetworkReferenceCrackConditions> opt = crack0.referenceCrackConditions();
  EXPECT_FALSE(opt);

  AirflowNetworkReferenceCrackConditions cond(model);
  EXPECT_EQ(101325, cond.barometricPressure());
  EXPECT_EQ(0, cond.humidityRatio());
  EXPECT_EQ(20, cond.temperature());
  cond.setBarometricPressure(0);
  EXPECT_EQ(101325, cond.barometricPressure());

  crack0.setReferenceCrackConditions(cond);
  EXPECT_EQ(20, crack0.referenceCrackConditions().get().temperature());
  cond.setTemperature(25);
  EXPECT_EQ(25, crack0.referenceCrackConditions().get().temperature());

  AirflowNetworkCrack crack1(model, 2.0, 0.5, cond);
  EXPECT_EQ(2, crack1.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack1.airMassFlowExponent());
  opt = crack1.referenceCrackConditions();
  EXPECT_TRUE(opt);
  EXPECT_EQ(25, opt.get().temperature());
}
