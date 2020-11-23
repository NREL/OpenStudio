/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
  EXPECT_FALSE(referencesEqual(designDayName, iddZone.getField(0).get()));
  candidate = IddFactory::instance().getObject(IddObjectType::SizingPeriod_WeatherFileDays);
  ASSERT_TRUE(candidate);
  ASSERT_TRUE(candidate->nonextensibleFields().size() > 1);
  IddField weatherFileDaysName = candidate->getField(0).get();
  EXPECT_TRUE(referencesEqual(weatherFileDaysName, designDayName));
  IddField beginMonth = candidate->getField(1).get();
  EXPECT_FALSE(referencesEqual(weatherFileDaysName, beginMonth));
}

TEST_F(IddFixture, IddField_Keys) {
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
  fields[3].print(ss, false);
  OptionalIddField fieldClone = IddField::load(fields[3].name(), ss.str(), "Lights");
  ASSERT_TRUE(fieldClone);
  // cppcheck-suppress arrayIndexOutOfBounds
  EXPECT_EQ(fields[3], *fieldClone);
}
