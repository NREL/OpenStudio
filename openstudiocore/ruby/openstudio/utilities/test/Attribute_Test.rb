######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'openstudio'

require 'minitest/autorun'

class Attribute_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_attribute

    attribute = OpenStudio::Attribute.new("bool", true, "units")
    assert_equal("bool", attribute.name())
    assert_equal("Boolean".to_AttributeValueType.value, attribute.valueType().value)
    assert_equal(true, attribute.valueAsBoolean())
    assert((not attribute.units().empty?))
    assert_equal("units", attribute.units().get())
    
    attribute = OpenStudio::Attribute.new("int", 1)
    assert_equal("int", attribute.name())
    assert_equal("Integer".to_AttributeValueType.value, attribute.valueType().value)
    assert_equal(1, attribute.valueAsInteger())
    assert(attribute.units().empty?)
  
    # attribute = OpenStudio::Attribute.new("unsigned", 1)
    # assert_equal("unsigned", attribute.name())
    # assert_equal("Unsigned".to_AttributeValueType.value, attribute.valueType().value)
    # assert_equal(1, attribute.valueAsUnsigned())
    # assert(attribute.units().empty?)
  
    attribute = OpenStudio::Attribute.new("double", 1.5)
    assert_equal("double", attribute.name())
    assert_equal("Double".to_AttributeValueType.value, attribute.valueType().value)
    assert_equal(1.5, attribute.valueAsDouble())
    assert(attribute.units().empty?)
  
    attribute = OpenStudio::Attribute.new("string", "value")
    assert_equal("string", attribute.name())
    assert_equal("String".to_AttributeValueType.value, attribute.valueType().value)
    assert_equal("value", attribute.valueAsString())
    assert(attribute.units().empty?)
  
    attributes = OpenStudio::AttributeVector.new 
    attributes << OpenStudio::Attribute.new("bool", true)
    attributes << OpenStudio::Attribute.new("double", 1.5, 'J')
  
    attribute = OpenStudio::Attribute.new("vector", attributes)
    assert_equal("vector", attribute.name())
    assert_equal("AttributeVector".to_AttributeValueType.value, attribute.valueType().value)
    assert_equal(2, attribute.valueAsAttributeVector().size())
    assert_equal("Boolean".to_AttributeValueType.value, attribute.valueAsAttributeVector()[0].valueType().value)
    assert_equal("Double".to_AttributeValueType.value, attribute.valueAsAttributeVector()[1].valueType().value)
    assert(attribute.units().empty?)
  end
  

end


