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
#include <ruleset/test/RulesetFixture.hpp>

#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <utilities/core/Containers.hpp>

using namespace openstudio;
using namespace openstudio::standardsinterface;
using namespace openstudio::ruleset;

TEST_F(RulesetFixture,StandardsRulesetOption_EnumRulesetOption_Construction)
{
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Performance");
  EXPECT_FALSE(option.defaultValue().empty());
  EXPECT_EQ(option.defaultValue(),option.value());

  StringVector validValues = option.validValues();
  EXPECT_TRUE(validValues == DataDictionary::enumValues(EnumAttribute(EnumAttribute::ComplianceApproach)));
  EXPECT_TRUE(validValues.size() > 1);
  EXPECT_FALSE(validValues[1] == option.defaultValue());
  EXPECT_TRUE(option.setValue(validValues[1]));
  EXPECT_EQ(validValues[1],option.value());
}
