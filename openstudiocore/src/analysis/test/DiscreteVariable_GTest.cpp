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
#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/ModelRulesetPerturbation.hpp>
#include <analysis/UncertaintyDescription.hpp>

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>

#include <utilities/core/FileReference.hpp>

#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, DiscreteVariable_Constructors) {
  DiscretePerturbationVector perturbations;

  // At most one null perturbation is allowed.
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(NullPerturbation());
  DiscreteVariable variable("Variable",perturbations);
  EXPECT_EQ(1u,variable.numPerturbations(false));

  // deserialization constructor
  UUID uuid = createUUID();
  UUID versionUUID = createUUID();
  perturbations = variable.perturbations(false);
  variable = DiscreteVariable(uuid,versionUUID,"Variable","","",boost::none,perturbations);
  EXPECT_EQ("Variable",variable.name());
  EXPECT_TRUE(variable.uuid() == uuid);
  EXPECT_TRUE(variable.versionUUID() == versionUUID);
  EXPECT_TRUE(variable.perturbations(false) == perturbations);

  // Inconsistent file types in perturbations (should throw)
  perturbations.clear();
  openstudio::path rubyScriptPath = toPath(rubyLibDir()) / 
                                    toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(RubyPerturbation(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::IDF));
  EXPECT_THROW(DiscreteVariable("Variable 2",perturbations),std::exception);

  // Inconsistent file types in perturbations (should throw)
  perturbations.clear();
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(RubyPerturbation(rubyScriptPath,
                                           FileReferenceType::IDF,
                                           FileReferenceType::IDF));
  perturbations.push_back(RubyPerturbation(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  EXPECT_THROW(DiscreteVariable("Variable",perturbations),std::exception);

  // Inconsistent file types in perturbations (should throw)
  ruleset::ModelRule rule("Rule");
  rule.add(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_Lights_Definition)));
  rule.add(ruleset::ModelObjectActionSetAttribute("wattsperSpaceFloorArea",8.0));
  ruleset::ModelRuleset ruleset("Ruleset");
  ruleset.add(rule);
  perturbations[2] = ModelRulesetPerturbation(ruleset);
  EXPECT_THROW(DiscreteVariable("Variable",perturbations),std::exception);
}

TEST_F(AnalysisFixture, DiscreteVariable_DeselectPerturbations) {
  DiscretePerturbationVector perturbations;

  // null, ruby, ruby, model ruleset
  perturbations.push_back(NullPerturbation());
  openstudio::path rubyScriptPath = toPath(rubyLibDir()) / 
                                    toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  perturbations.push_back(RubyPerturbation(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  perturbations.push_back(RubyPerturbation(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  ruleset::ModelRule rule("Rule");
  rule.add(ruleset::ModelObjectFilterType(IddObjectType(IddObjectType::OS_Lights_Definition)));
  rule.add(ruleset::ModelObjectActionSetAttribute("wattsperSpaceFloorArea",8.0));
  ruleset::ModelRuleset ruleset("Ruleset");
  ruleset.add(rule);
  perturbations.push_back(ModelRulesetPerturbation(ruleset));

  DiscreteVariable variable("Variable",perturbations);
  EXPECT_EQ(4u,variable.numPerturbations(true));
  EXPECT_EQ(4u,variable.numPerturbations(false));

  perturbations[1].setIsSelected(false);
  EXPECT_EQ(3u,variable.numPerturbations(true));
  ASSERT_EQ(3u,variable.perturbations(true).size());
  EXPECT_TRUE(variable.perturbations(true)[1] == perturbations[2]);
  EXPECT_TRUE(variable.perturbations(true)[2] == perturbations[3]);
  EXPECT_EQ(4u,variable.numPerturbations(false));

  perturbations[0].setIsSelected(false);
  perturbations[2].setIsSelected(false);
  perturbations[3].setIsSelected(false);
  EXPECT_EQ(0u,variable.numPerturbations(true));
  EXPECT_EQ(4u,variable.numPerturbations(false));

  perturbations[3].setIsSelected(true);
  EXPECT_EQ(1u,variable.numPerturbations(true));
  ASSERT_EQ(1u,variable.perturbations(true).size());
  EXPECT_TRUE(variable.perturbations(true)[0] == perturbations[3]);
  EXPECT_EQ(4u,variable.numPerturbations(false));
}
