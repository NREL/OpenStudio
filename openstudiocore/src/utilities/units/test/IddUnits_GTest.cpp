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
#include <utilities/units/test/UnitsFixture.hpp>

#include <utilities/units/OSOptionalQuantity.hpp>
#include <utilities/units/IddUnitString.hpp>
#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/ScaleFactory.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/QuantityConverter.hpp>

#include <utilities/idd/IddObject.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Building_FieldEnums.hxx>

using namespace openstudio;

TEST_F(UnitsFixture,IddUnits_BtuSystem) {
  IddUnitString iddString("Btu/h-ft-F");
  EXPECT_EQ("Btu/h*ft*R",iddString.toStandardUnitString());
  OptionalUnit unit = createUnit(iddString.toStandardUnitString());
  ASSERT_TRUE(unit);
  EXPECT_TRUE(unit->system() == UnitSystem::BTU);
}

TEST_F(UnitsFixture,IddUnits_Grams) {
  IddUnitString iddString("g/m-s-K");
  EXPECT_EQ("mkg/m*s*K",iddString.toStandardUnitString());
  OptionalUnit unit = createUnit(iddString.toStandardUnitString());
  ASSERT_TRUE(unit);
  EXPECT_TRUE(unit->system() == UnitSystem::SI);
  EXPECT_EQ(-3,unit->scale().exponent);
}

TEST_F(UnitsFixture,IddUnits_DefaultValue) {
  IdfObject idfObject(IddObjectType::OS_Building);
  // IdfObject::getQuantity(unsigned index, bool returnDefault=false, bool returnIP=false) const;

  // this field is empty but has a default value of 3 m
  EXPECT_TRUE(idfObject.getQuantity(OS_BuildingFields::NominalFloortoFloorHeight, false).empty());

  OSOptionalQuantity siQ = idfObject.getQuantity(OS_BuildingFields::NominalFloortoFloorHeight, true, false);
  ASSERT_FALSE(siQ.empty());
  OptionalUnit mUnit = createUnit("m");
  ASSERT_TRUE(mUnit);
  EXPECT_EQ(*mUnit, siQ.get().units());
  EXPECT_EQ(3.0, siQ.get().value());

  OSOptionalQuantity ipQ = idfObject.getQuantity(OS_BuildingFields::NominalFloortoFloorHeight, true, true);
  ASSERT_FALSE(ipQ.empty());
  OptionalUnit ftUnit = createUnit("ft");
  ASSERT_TRUE(ftUnit);
  EXPECT_EQ(*ftUnit, ipQ.get().units());
  EXPECT_NE(3.0, ipQ.get().value());
  
  OptionalQuantity q = QuantityConverter::instance().convert(ipQ.get(), *mUnit);
  ASSERT_TRUE(q);
  EXPECT_DOUBLE_EQ(3.0, q->value());
}
