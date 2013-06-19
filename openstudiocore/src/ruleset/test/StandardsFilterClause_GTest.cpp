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

#include <ruleset/StandardsFilterClause.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>

#include <standardsinterface/DataDictionary.hpp>

using namespace openstudio::ruleset;
using namespace openstudio::standardsinterface;

TEST_F(RulesetFixture,StandardsFilterClause_StandardsFilterAvailableModel) {
  StandardsFilterAvailableModel filter(StandardsRulesetModelType::Original);
  ASSERT_TRUE(filter.modelType());
  EXPECT_TRUE(filter.modelType().get() == StandardsRulesetModelType::Original);
  
  EXPECT_FALSE(filter.print().empty());
  EXPECT_FALSE(filter.printRetractAction().empty());

  ASSERT_TRUE(filter.optionalCast<FilterClause>());
  StandardsFilterAvailableModel sharedDataCopy(filter);
  EXPECT_TRUE(sharedDataCopy == filter);
}

TEST_F(RulesetFixture,StandardsFilterClause_StandardsFilterRulesetOption) {
  StandardsEnumRulesetOption option(EnumAttribute::ComplianceApproach,
                                    ComplianceApproach(ComplianceApproach::Performance).valueName());
  StandardsFilterRulesetOption filter(option);
  std::string filterString = filter.print();
  EXPECT_FALSE(filterString.empty());
  EXPECT_FALSE(filter.printRetractAction().empty());
  option.setValue(ComplianceApproach(ComplianceApproach::PrescriptiveEnvelopeComponent).valueName());
  filter.setOption(option);
  EXPECT_NE(filterString,filter.print());
}

TEST_F(RulesetFixture,StandardsFilterClause_StandardsFilterAvailableObject) {
  StandardsFilterAvailableObject filter(ObjectType("Wall"));
  EXPECT_TRUE(filter.referencedFilters().empty());
  EXPECT_TRUE(filter.impliedFilters().empty());
  std::string filterString = filter.print();
  EXPECT_FALSE(filterString.empty());
  std::stringstream ss;
  ss << "?" << filter.factName() << "_ModelId";
  EXPECT_EQ(ss.str(),filter.modelId()); ss.str("");
  EXPECT_FALSE(filter.model());
  LOG(Info,"StandardsFilterAvailableObject on all objects of a type, from all existing models: " 
      << std::endl << filterString);

  StandardsFilterAvailableObject filter2(ObjectType("Wall"));
  EXPECT_TRUE(filter.equivalentData(filter2));

  StandardsRulesetModelType modelType(StandardsRulesetModelType::Original);
  StandardsFilterAvailableModel model(modelType);
  filter = StandardsFilterAvailableObject(model,ObjectType("Roof"));
  ASSERT_EQ(1u,filter.referencedFilters().size());
  EXPECT_TRUE(filter.impliedFilters().empty());
  EXPECT_TRUE(filter.referencedFilters()[0] == model);
  ss << "?" << filter.model()->factName() << "_Id";
  EXPECT_EQ(ss.str(),filter.modelId()); ss.str("");
  ss << filter.referencedFilters()[0].print() << filter.print();
  LOG(Info,"FilterAvailableObject linked to a StandardsFilterAvailableModel (both printed): " << std::endl
      << ss.str()); ss.str("");

  modelType = StandardsRulesetModelType(StandardsRulesetModelType::Budget);
  StandardsFilterAvailableModel model2(modelType);
  filter2 = StandardsFilterAvailableObject(model2,ObjectType("Roof"));
  EXPECT_FALSE(filter.equivalentData(filter2));
}
