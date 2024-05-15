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
