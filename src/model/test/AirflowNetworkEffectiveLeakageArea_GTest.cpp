/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkEffectiveLeakageArea.hpp"
#include "../AirflowNetworkEffectiveLeakageArea_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_EffectiveLeakageArea) {
  Model model;

  AirflowNetworkEffectiveLeakageArea ela0(model, 10.0);
  AirflowNetworkEffectiveLeakageArea ela1(model, 10.0, 0.5, 50.0, 0.5);

  EXPECT_EQ(10.0, ela0.effectiveLeakageArea());
  EXPECT_EQ(1.0, ela0.dischargeCoefficient());
  EXPECT_TRUE(ela0.isDischargeCoefficientDefaulted());
  EXPECT_EQ(4.0, ela0.referencePressureDifference());
  EXPECT_TRUE(ela0.isReferencePressureDifferenceDefaulted());
  EXPECT_EQ(0.65, ela0.airMassFlowExponent());
  EXPECT_TRUE(ela0.isAirMassFlowExponentDefaulted());

  EXPECT_EQ(10.0, ela1.effectiveLeakageArea());
  EXPECT_EQ(0.5, ela1.dischargeCoefficient());
  EXPECT_FALSE(ela1.isDischargeCoefficientDefaulted());
  EXPECT_EQ(50.0, ela1.referencePressureDifference());
  EXPECT_FALSE(ela1.isReferencePressureDifferenceDefaulted());
  EXPECT_EQ(0.5, ela1.airMassFlowExponent());
  EXPECT_FALSE(ela1.isAirMassFlowExponentDefaulted());
}
