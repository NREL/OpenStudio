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

class ModelRulesetContinuousVariable_Test < Test::Unit::TestCase
  
  def test_ModelRulesetContinuousVariable_FiltersIncludeModelObjectFilterRelationship
  
    OpenStudio::Logger.instance.standardOutLogger.setLogLevel(-3)

    filters = OpenStudio::Ruleset::ModelObjectFilterClauseVector.new
    filters.push(OpenStudio::Ruleset::ModelObjectFilterType.new("OS:Construction".to_IddObjectType))
    filters.push(OpenStudio::Ruleset::ModelObjectFilterStringAttribute.new(
                     "name","IEquals".to_RulesetStringPredicate,"Mass Non-res Ext Wall"))
    filters.push(OpenStudio::Ruleset::ModelObjectFilterRelationship.new("insulation"))
    
    continuousVariable = OpenStudio::Analysis::ModelRulesetContinuousVariable.new(
                           "Wall Insulation R-Value",filters,"thermalResistance")

    # primarily testing for segfault
    ruleset = continuousVariable.getModelRuleset(OpenStudio::OptionalDouble.new(1.0))
    assert_equal(1,ruleset.rules.size)
    rule = ruleset.rules[0]
    assert_equal(3,rule.filters.size)
    assert_equal(1,rule.actions.size)
  end
  
end
