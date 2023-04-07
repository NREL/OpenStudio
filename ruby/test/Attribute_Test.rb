########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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


