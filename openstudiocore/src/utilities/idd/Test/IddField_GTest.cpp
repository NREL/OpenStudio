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
#include "IddFixture.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../IddField.hpp"
#include "../IddFieldProperties.hpp"
#include "../IddKey.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IddFixture, IddField_Properties) {
  
  OptionalIddObject candidate = IddFactory::instance().getObject(IddObjectType::Zone);
  ASSERT_TRUE(candidate);
  IddObject iddZone = *candidate;
  candidate = IddFactory::instance().getObject(IddObjectType::ZoneList);
  ASSERT_TRUE(candidate);
  IddObject iddZoneList = *candidate;

  // ==, != operators
  ASSERT_TRUE(iddZone.nonextensibleFields().size() > 2); 
  ASSERT_TRUE(iddZoneList.nonextensibleFields().size() > 0);
  EXPECT_TRUE(iddZone.nonextensibleFields()[0].properties() == iddZone.nonextensibleFields()[0].properties());
  // ETH@20100326 Would expect these to be the same--probably only differ by note/memo. 
  // Is that the behavior we want?
  EXPECT_FALSE(iddZone.nonextensibleFields()[0].properties() == iddZoneList.nonextensibleFields()[0].properties());
  EXPECT_TRUE(iddZone.nonextensibleFields()[2].properties() != iddZoneList.nonextensibleFields()[0].properties());

  // referencesEqual
  candidate = IddFactory::instance().getObject(IddObjectType::SizingPeriod_DesignDay);
  ASSERT_TRUE(candidate);
  ASSERT_TRUE(candidate->nonextensibleFields().size() > 0);
  IddField designDayName = candidate->getField(0).get();
  EXPECT_FALSE(referencesEqual(designDayName,iddZone.getField(0).get()));
  candidate = IddFactory::instance().getObject(IddObjectType::SizingPeriod_WeatherFileDays);
  ASSERT_TRUE(candidate);
  ASSERT_TRUE(candidate->nonextensibleFields().size() > 1);
  IddField weatherFileDaysName = candidate->getField(0).get();
  EXPECT_TRUE(referencesEqual(weatherFileDaysName,designDayName));
  IddField beginMonth = candidate->getField(1).get();
  EXPECT_FALSE(referencesEqual(weatherFileDaysName,beginMonth));
}


TEST_F(IddFixture, IddField_Keys)
{
  OptionalIddObject candidate = IddFactory::instance().getObject(IddObjectType::Lights);
  ASSERT_TRUE(candidate);
  IddObject iddLights = *candidate;
  IddFieldVector fields = iddLights.nonextensibleFields();

  // translated correctly, in expected field and in order
  ASSERT_TRUE(fields.size() >= 4);
  OptionalIddKey key = fields[3].getKey("LightingLevel");
  ASSERT_TRUE(key);
  key = fields[3].getKey("Watts/Area");
  ASSERT_TRUE(key);
  key = fields[3].getKey("Watts/Person");
  ASSERT_TRUE(key);

  // ==, != operators
  EXPECT_TRUE(key == *(fields[3].getKey("Watts/Person")));
  EXPECT_TRUE(key != *(fields[3].getKey("LightingLevel")));
}

TEST_F(IddFixture, IddField) {
  OptionalIddObject candidate = IddFactory::instance().getObject(IddObjectType::Lights);
  ASSERT_TRUE(candidate);
  IddObject iddLights = *candidate;
  IddFieldVector fields = iddLights.nonextensibleFields();

  ASSERT_TRUE(fields.size() >= 4);
  EXPECT_TRUE(fields[0] == fields[0]);
  EXPECT_TRUE(fields[1] != fields[3]);

  // has to check everything (not just impl address)
  std::stringstream ss;
  fields[3].print(ss,false);
  OptionalIddField fieldClone = IddField::load(fields[3].name(),ss.str(),"Lights");
  ASSERT_TRUE(fieldClone);
  // cppcheck-suppress arrayIndexOutOfBounds
  EXPECT_EQ(fields[3],*fieldClone);
}
