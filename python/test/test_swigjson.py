########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

from pathlib import Path

import openstudio


def test_osargument_value_as_json():
    boolArgument = openstudio.measure.OSArgument.makeBoolArgument("bool")
    valNotAssigned = boolArgument.valueAsJSON()
    assert valNotAssigned is None
    boolArgument.setValue(True)
    val = boolArgument.valueAsJSON()
    assert isinstance(val, bool)
    assert val

    doubleArgument = openstudio.measure.OSArgument.makeDoubleArgument("double")
    valNotAssigned = doubleArgument.valueAsJSON()
    assert valNotAssigned is None
    doubleArgument.setValue(10.0)
    val = doubleArgument.valueAsJSON()
    assert isinstance(val, float)
    assert val == 10.0

    integerArgument = openstudio.measure.OSArgument.makeIntegerArgument("integer")
    valNotAssigned = integerArgument.valueAsJSON()
    assert valNotAssigned is None
    integerArgument.setValue(10)
    val = integerArgument.valueAsJSON()
    assert isinstance(val, int)
    assert val == 10

    stringArgument = openstudio.measure.OSArgument.makeStringArgument("string")
    valNotAssigned = stringArgument.valueAsJSON()
    assert valNotAssigned is None
    stringArgument.setValue("hello")
    val = stringArgument.valueAsJSON()
    assert isinstance(val, str)
    assert val == "hello"

    choices_to_display_values_map = {
        "handle1": "choice1",
        "handle2": "choice2",
    }
    choiceArgument = openstudio.measure.OSArgument.makeChoiceArgument("choice", choices_to_display_values_map)
    valNotAssigned = choiceArgument.valueAsJSON()
    assert valNotAssigned is None
    choiceArgument.setValue("handle1")
    val = choiceArgument.valueAsJSON()
    assert isinstance(val, str)
    assert val == "handle1"


def test_swig_json_special_chars():
    workflow = openstudio.WorkflowJSON()
    runner = openstudio.measure.OSRunner(workflow)

    step1 = openstudio.MeasureStep("MeasureName1")
    step1.setArgument("Argument1", 100)

    workflow_step_result1 = openstudio.WorkflowStepResult()
    workflow_step_result1.setMeasureName("measure_name_1")

    name = "name_(|)'*$£*µ%é"
    val = "val_(|)'*$£*µ%é"
    stepValue1 = openstudio.WorkflowStepValue(name, val)
    assert name == stepValue1.name()
    assert val == stepValue1.valueAsString()
    assert val == stepValue1.valueAsJSON()

    workflow_step_result1.addStepValue(stepValue1)

    workflow_step_result1.setStepResult(openstudio.StepResult("Success"))
    workflow_step_result1.setCompletedAt(openstudio.DateTime.nowUTC())
    step1.setResult(workflow_step_result1)

    workflow.setWorkflowSteps([step1])

    d = runner.getPastStepValuesForMeasure("MeasureName1")
    assert len(d) == 1
    k, v = next(iter(d.items()))
    assert k == name
    assert v == val
