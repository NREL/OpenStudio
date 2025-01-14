########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

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

  def test_swig_json_special_chars
    workflow = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(workflow)

    step1 = OpenStudio::MeasureStep.new('MeasureName1')
    step1.setArgument('Argument1', 100)

    workflow_step_result1 = OpenStudio::WorkflowStepResult.new
    workflow_step_result1.setMeasureName('measure_name_1')

    name = "name_(|)'*$£*µ%é"
    val = "val_(|)'*$£*µ%é"
    stepValue1 = OpenStudio::WorkflowStepValue.new(name, val)
    assert_equal(name, stepValue1.name)
    assert_equal(val, stepValue1.valueAsString)
    assert_equal(val, stepValue1.valueAsJSON)
    workflow_step_result1.addStepValue(stepValue1)

    workflow_step_result1.setStepResult('Success'.to_StepResult)
    workflow_step_result1.setCompletedAt(OpenStudio::DateTime.nowUTC)
    step1.setResult(workflow_step_result1)

    workflow.setWorkflowSteps([step1])

    # workflow.saveAs("OSRunner_getPastStepValues_special_chars.osw")

    h = runner.getPastStepValuesForMeasure('MeasureName1')
    assert_equal(1, h.size)
    assert_equal(name.to_sym, h.keys.first)
    assert_equal(val, h.values.first)
  end
end
