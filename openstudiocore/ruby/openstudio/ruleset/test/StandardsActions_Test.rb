######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'openstudio'

require 'test/unit'

class StandardsActions_Test < Test::Unit::TestCase
  
  def test_cloneModel
    modelFilter = OpenStudio::Ruleset::StandardsFilterAvailableModel.new(
        "Original".to_StandardsRulesetModelType)
    action = OpenStudio::Ruleset::StandardsActionCloneModel.new(
        modelFilter,"Proposed".to_StandardsRulesetModelType)
    assert((modelFilter == action.modelToClone))
    assert(("Proposed".to_StandardsRulesetModelType == action.modelTypeOfClone))
    
    action.setModelTypeOfClone("Budget".to_StandardsRulesetModelType)
    assert(("Budget".to_StandardsRulesetModelType == action.modelTypeOfClone))
  end
  
  def test_ruleResult
    objectFilter = OpenStudio::Ruleset::StandardsFilterAvailableObject.new(
        "Roof".to_ObjectType)
    attributeFilter = OpenStudio::Ruleset::StandardsFilterObjectDoubleAttribute.new(
        objectFilter,
        "UFactor".to_DoubleAttribute,
        "LessThanOrEqualTo".to_RulesetNumericalPredicate,
        0.05,
        "W/m^2*K")
    action = OpenStudio::Ruleset::StandardsActionRuleSatisfied.new(attributeFilter)
    
    assert((attributeFilter == action.attributeTest))
    assert((objectFilter == action.attributeTest.object))
    
    action.attributeTest.to_StandardsFilterObjectDoubleAttribute.get.setPredicate(
        "GreaterThan".to_RulesetNumericalPredicate)
    assert((attributeFilter.predicate == "GreaterThan".to_RulesetNumericalPredicate))
    
    attributeFilterClone = attributeFilter.clone.to_StandardsFilterObjectDoubleAttribute.get
    attributeFilter.setPredicate("LessThanOrEqualTo".to_RulesetNumericalPredicate)
    assert((action.attributeTest.to_StandardsFilterObjectDoubleAttribute.get.predicate ==
        "LessThanOrEqualTo".to_RulesetNumericalPredicate))
    assert((attributeFilterClone.predicate == "GreaterThan".to_RulesetNumericalPredicate))
    
    failAction = OpenStudio::Ruleset::StandardsActionRuleViolated.new(attributeFilterClone)
    assert((not(failAction.attributeTest == action.attributeTest)))
    assert((failAction.attributeTest.object == action.attributeTest.object))
  end

  
end