/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneAirMassFlowConservation.hpp"
#include "../ZoneAirMassFlowConservation_Impl.hpp"
#include "../Model.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneAirMassFlowConservation)
{ 
  Model model;

  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());

  ZoneAirMassFlowConservation zamfc = model.getUniqueModelObject<ZoneAirMassFlowConservation>();

  EXPECT_FALSE(zamfc.adjustZoneMixingForZoneAirMassFlowBalance());
  EXPECT_TRUE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());

  EXPECT_EQ("AddInfiltrationFlow", zamfc.infiltrationBalancingMethod());
  EXPECT_TRUE(zamfc.isInfiltrationBalancingMethodDefaulted());

  EXPECT_EQ("MixingSourceZonesOnly", zamfc.infiltrationBalancingZones());
  EXPECT_TRUE(zamfc.isInfiltrationBalancingZonesDefaulted());

  zamfc.setAdjustZoneMixingForZoneAirMassFlowBalance(true);
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
