/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneAirMassFlowConservation.hpp"
#include "../ZoneAirMassFlowConservation_Impl.hpp"
#include "../Model.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneAirMassFlowConservation) {
  Model model;

  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());

  auto zamfc = model.getUniqueModelObject<ZoneAirMassFlowConservation>();

  EXPECT_EQ("None", zamfc.adjustZoneMixingandReturnForAirMassFlowBalance());
  EXPECT_TRUE(zamfc.isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted());

  // deprecated
  EXPECT_FALSE(zamfc.adjustZoneMixingForZoneAirMassFlowBalance());
  EXPECT_TRUE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());

  EXPECT_EQ("AddInfiltrationFlow", zamfc.infiltrationBalancingMethod());
  EXPECT_TRUE(zamfc.isInfiltrationBalancingMethodDefaulted());

  EXPECT_EQ("MixingSourceZonesOnly", zamfc.infiltrationBalancingZones());
  EXPECT_TRUE(zamfc.isInfiltrationBalancingZonesDefaulted());

  EXPECT_TRUE(zamfc.setAdjustZoneMixingandReturnForAirMassFlowBalance("AdjustReturnThenMixing"));
  EXPECT_EQ("AdjustReturnThenMixing", zamfc.adjustZoneMixingandReturnForAirMassFlowBalance());
  EXPECT_FALSE(zamfc.isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted());

  // deprecated
  EXPECT_FALSE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());
  zamfc.setAdjustZoneMixingForZoneAirMassFlowBalance(true);
  EXPECT_EQ("AdjustMixingOnly", zamfc.adjustZoneMixingandReturnForAirMassFlowBalance());
  EXPECT_TRUE(zamfc.adjustZoneMixingForZoneAirMassFlowBalance());
  EXPECT_FALSE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());

  EXPECT_TRUE(zamfc.setInfiltrationBalancingMethod("AdjustInfiltrationFlow"));
  EXPECT_EQ("AdjustInfiltrationFlow", zamfc.infiltrationBalancingMethod());
  EXPECT_FALSE(zamfc.isInfiltrationBalancingMethodDefaulted());

  EXPECT_TRUE(zamfc.setInfiltrationBalancingZones("AllZones"));
  EXPECT_EQ("AllZones", zamfc.infiltrationBalancingZones());
  EXPECT_FALSE(zamfc.isInfiltrationBalancingZonesDefaulted());

  EXPECT_TRUE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
}
