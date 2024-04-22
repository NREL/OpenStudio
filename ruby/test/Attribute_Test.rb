########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Attribute_Test < Minitest::Test

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


