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
