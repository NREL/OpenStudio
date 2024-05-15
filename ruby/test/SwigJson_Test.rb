# frozen_string_literal: true

########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'minitest/autorun'

class SwigJSONTest < Minitest::Test
  def test_osargument_value_as_json
    boolArgument = OpenStudio::Measure::OSArgument.makeBoolArgument('bool')
    valNotAssigned = boolArgument.valueAsJSON
    assert valNotAssigned.nil?
    boolArgument.setValue(true)
    val = boolArgument.valueAsJSON
    assert val.is_a?(TrueClass) || val.is_a?(FalseClass)
    assert val

    doubleArgument = OpenStudio::Measure::OSArgument.makeDoubleArgument('double')
    valNotAssigned = doubleArgument.valueAsJSON
    assert valNotAssigned.nil?
    doubleArgument.setValue(10.0)
    val = doubleArgument.valueAsJSON
    assert_kind_of(Float, val)
    assert_equal(10.0, val)

    integerArgument = OpenStudio::Measure::OSArgument.makeIntegerArgument('integer')
    valNotAssigned = integerArgument.valueAsJSON
    assert valNotAssigned.nil?
    integerArgument.setValue(10)
    val = integerArgument.valueAsJSON
    assert_kind_of(Integer, val)
    assert_equal(10, val)

    stringArgument = OpenStudio::Measure::OSArgument.makeStringArgument('string')
    valNotAssigned = stringArgument.valueAsJSON
    assert valNotAssigned.nil?
    stringArgument.setValue('hello')
    val = stringArgument.valueAsJSON
    assert_kind_of(String, val)
    assert_equal('hello', val)

    choices_to_display_values_map = {
      'handle1' => 'choice1',
      'handle2' => 'choice2'
    }

    choiceArgument = OpenStudio::Measure::OSArgument.makeChoiceArgument('choice', choices_to_display_values_map)
    valNotAssigned = choiceArgument.valueAsJSON
    assert valNotAssigned.nil?
    choiceArgument.setValue('handle1')
    val = choiceArgument.valueAsJSON
    assert_kind_of(String, val)
    assert_equal('handle1', val)
  end
end
