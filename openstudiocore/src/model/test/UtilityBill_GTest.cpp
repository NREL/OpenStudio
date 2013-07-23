/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/UtilityBill.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, UtilityBill_Electricity) {
  Model model;

  UtilityBill utilityBill(FuelType::Electricity, model);
  EXPECT_EQ(FuelType::Electricity, utilityBill.fuelType().value());
  EXPECT_EQ(InstallLocationType::Facility, utilityBill.meterInstallLocation().value());
  EXPECT_TRUE(utilityBill.isMeterInstallLocationDefaulted());
  EXPECT_FALSE(utilityBill.meterSpecificInstallLocation());
  EXPECT_FALSE(utilityBill.meterEndUse());
  EXPECT_FALSE(utilityBill.meterSpecificEndUse());
  EXPECT_EQ("kWh", utilityBill.consumptionUnit());
  EXPECT_EQ(888, utilityBill.consumptionUnitConversionFactor());
  ASSERT_TRUE(utilityBill.peakDemandUnit());
  EXPECT_EQ("kW", utilityBill.peakDemandUnit().get());
  EXPECT_EQ(0, utilityBill.billingPeriods().size());

}

TEST_F(ModelFixture, UtilityBill_Coverage) {
  Model model;

  BOOST_FOREACH(FuelType fuelType, FuelType::getValues()){
    UtilityBill utilityBill(fuelType, model);
    EXPECT_EQ(fuelType, utilityBill.fuelType().value());
    EXPECT_EQ(InstallLocationType::Facility, utilityBill.meterInstallLocation().value());
    EXPECT_TRUE(utilityBill.isMeterInstallLocationDefaulted());
    EXPECT_FALSE(utilityBill.meterSpecificInstallLocation());
    EXPECT_FALSE(utilityBill.meterEndUse());
    EXPECT_FALSE(utilityBill.meterSpecificEndUse());
    EXPECT_NE("", utilityBill.consumptionUnit());
    EXPECT_NE(0, utilityBill.consumptionUnitConversionFactor());
    EXPECT_EQ(0, utilityBill.billingPeriods().size());
  }
}
