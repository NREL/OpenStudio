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

class StandardsFilters_Test < Test::Unit::TestCase
  
  def test_availableModel
    filter = OpenStudio::Ruleset::StandardsFilterAvailableModel.new(
        "Original".to_StandardsRulesetModelType)
    modelType = filter.modelType
    assert((not modelType.empty?))
    modelType = modelType.get
    assert((modelType == "Original".to_StandardsRulesetModelType))
  end
  
  def test_availableObject
    filter = OpenStudio::Ruleset::StandardsFilterAvailableObject.new(
        "Building".to_ObjectType)
    objectType = filter.objectType
    assert((objectType == "Building".to_ObjectType))
    
    modelFilter = OpenStudio::Ruleset::StandardsFilterAvailableModel.new(
        "Proposed".to_StandardsRulesetModelType)
    filter = OpenStudio::Ruleset::StandardsFilterAvailableObject.new(
        modelFilter,"Building".to_ObjectType)
    copyOfModelFilter = filter.model
    assert((not copyOfModelFilter.empty?))
    copyOfModelFilter = copyOfModelFilter.get
    assert((copyOfModelFilter == modelFilter))
  end
  
  def test_objectAttributes
    # boolean
    
    
    # double
    objectFilter = OpenStudio::Ruleset::StandardsFilterAvailableObject.new(
        "Wall".to_ObjectType)
    filter = OpenStudio::Ruleset::StandardsFilterObjectDoubleAttribute.new(
        objectFilter,
        "UFactor".to_DoubleAttribute,
        "LessThanOrEqualTo".to_RulesetNumericalPredicate,
        0.01,
        "W/m^2*K")
    assert((filter.object == objectFilter))
    assert_equal("UFactor",filter.attribute.valueName)
    assert_equal("LessThanOrEqualTo",filter.predicate.valueName)
    assert_equal(0.01,filter.value)
    assert(filter.setValue(0.02))
    assert_equal(0.02,filter.value)
    
    # enum
    objectFilter = OpenStudio::Ruleset::StandardsFilterAvailableObject.new(
        "Wall".to_ObjectType)
    filter = OpenStudio::Ruleset::StandardsFilterObjectEnumAttribute.new(
        objectFilter,
        "PartitionThermalType".to_EnumAttribute,
        "Equals".to_RulesetStringPredicate,
        "Exterior")
    assert((filter.object == objectFilter))
    assert_equal("Exterior",filter.value)
    
    # integer
    
  end
  
end
