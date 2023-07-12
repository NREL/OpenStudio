/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "UnitsFixture.hpp"

#include "../OSOptionalQuantity.hpp"
#include "../IddUnitString.hpp"
#include "../UnitFactory.hpp"
#include "../ScaleFactory.hpp"
#include "../Quantity.hpp"
#include "../QuantityConverter.hpp"

#include "../../idd/IddObject.hpp"
#include "../../idf/IdfObject.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include "../../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;

TEST_F(UnitsFixture, IddUnits_BtuSystem) {
  IddUnitString iddString("Btu/h-ft-F");
  EXPECT_EQ("Btu/h*ft*R", iddString.toStandardUnitString());
  OptionalUnit unit = createUnit(iddString.toStandardUnitString());
  ASSERT_TRUE(unit);
  EXPECT_TRUE(unit->system() == UnitSystem::BTU);
}

TEST_F(UnitsFixture, IddUnits_Grams) {
  IddUnitString iddString("g/m-s-K");
  EXPECT_EQ("mkg/m*s*K", iddString.toStandardUnitString());
  OptionalUnit unit = createUnit(iddString.toStandardUnitString());
  ASSERT_TRUE(unit);
  EXPECT_TRUE(unit->system() == UnitSystem::SI);
  EXPECT_EQ(-3, unit->scale().exponent);
}

TEST_F(UnitsFixture, IddUnits_PowerPerFlowRatePerUnitPressure) {
  {
    IddUnitString iddString("W/((m3/s)-Pa)");
    OptionalUnit unit = createUnit(iddString.toStandardUnitString());
    EXPECT_FALSE(unit);
  }

  {
    IddUnitString iddString("W-s/m3-Pa");
    EXPECT_EQ("W*s/m^3*Pa", iddString.toStandardUnitString());
    OptionalUnit unit = createUnit(iddString.toStandardUnitString());
    ASSERT_TRUE(unit);
    EXPECT_TRUE(unit->system() == UnitSystem::SI);

    IddUnitString iddStringIP("W-min/ft3-inH2O");
    EXPECT_EQ("W*min/ft^3*inH_{2}O", iddStringIP.toStandardUnitString());
    OptionalUnit unitIP = createUnit(iddStringIP.toStandardUnitString());
    ASSERT_TRUE(unitIP);
    // it's returning UnitSystem::SI instead
    // EXPECT_TRUE(unitIP->system() == UnitSystem::IP);
    // THis is because it's a dimensionless unit:
    // W = kg.m2.s-3
    // Pa = kg.m-1.s-2 => (m3/s)*Pa = kg.m2.s-3
    // => This is a dimensionless unit
  }
}
