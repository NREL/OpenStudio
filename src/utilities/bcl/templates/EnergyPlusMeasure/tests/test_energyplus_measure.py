"""Insert your copyright here."""

import sys
from pathlib import Path

import openstudio
import pytest

CURRENT_DIR_PATH = Path(__file__).parent.absolute()
sys.path.insert(0, str(CURRENT_DIR_PATH.parent))
from measure import EnergyPlusMeasureName
sys.path.pop(0)
del sys.modules['measure']


class TestEnergyPlusMeasureName:
    """Py.test module for EnergyPlusMeasureName."""

    def test_number_of_arguments_and_argument_names(self):
        """Test that the arguments are what we expect."""
        # create an instance of the measure
        measure = EnergyPlusMeasureName()

        # make an empty workspace
        workspace = openstudio.Workspace(openstudio.StrictnessLevel("Draft"), openstudio.IddFileType("EnergyPlus"))

        # get arguments and test that they are what we are expecting
        arguments = measure.arguments(workspace)
        assert arguments.size() == 1
        assert arguments[0].name() == "zone_name"
        assert arguments[0].required()
        assert arguments[0].type().valueName() == "String"

    def test_bad_argument_values(self):
        """Test running the measure with inappropriate arguments, and that the measure reports failure."""
        # create an instance of the measure
        measure = EnergyPlusMeasureName()

        # create runner with empty OSW
        osw = openstudio.WorkflowJSON()
        runner = openstudio.measure.OSRunner(osw)

        # make an empty workspace
        workspace = openstudio.Workspace(openstudio.StrictnessLevel("Draft"), openstudio.IddFileType("EnergyPlus"))

        # get arguments
        arguments = measure.arguments(workspace)
        argument_map = openstudio.measure.convertOSArgumentVectorToMap(arguments)

        # set argument values to bad value
        zone_name = arguments[0].clone()
        assert zone_name.setValue("")
        argument_map["zone_name"] = zone_name

        # run the measure
        measure.run(workspace, runner, argument_map)
        result = runner.result()
        assert result.value().valueName() == "Fail"

        # check that there are still no thermal zones
        assert len(workspace.getObjectsByType("Zone")) == 0

    def test_good_argument_values(self):
        """Test running the measure with appropriate arguments.

        Asserts that the measure runs fine and with expected results.
        """
        # create an instance of the measure
        measure = EnergyPlusMeasureName()

        # create runner with empty OSW
        osw = openstudio.WorkflowJSON()
        runner = openstudio.measure.OSRunner(osw)

        # make an empty workspace
        workspace = openstudio.Workspace(openstudio.StrictnessLevel("Draft"), openstudio.IddFileType("EnergyPlus"))

        # check that there are no thermal zones
        assert len(workspace.getObjectsByType("Zone")) == 0

        # get arguments
        arguments = measure.arguments(workspace)
        argument_map = openstudio.measure.convertOSArgumentVectorToMap(arguments)

        # set argument values to good values
        # create hash of argument values.
        # If the argument has a default that you want to use,
        # you don't need it in the dict
        args_dict = {}
        args_dict["zone_name"] = "New Zone"
        # using defaults values from measure.py for other arguments

        # populate argument with specified hash value if specified
        for arg in arguments:
            temp_arg_var = arg.clone()
            if arg.name() in args_dict:
                assert temp_arg_var.setValue(args_dict[arg.name()])
                argument_map[arg.name()] = temp_arg_var

        # run the measure
        measure.run(workspace, runner, argument_map)
        result = runner.result()

        # show the output
        print(f"results: {result}")

        # assert that it ran correctly
        assert result.value().valueName() == "Success"

        # check that there is now 1 thermal zones
        assert len(workspace.getObjectsByType("Zone")) == 1

        # check that zone is properly named
        zone = workspace.getObjectsByType("Zone")[0]
        assert zone.getString(0).is_initialized()
        assert zone.getString(0).get() == "New Zone"

        # save the workspace to test output directory
        output_file_path = Path(__file__).parent.absolute() / "output" / "test_output.idf"
        workspace.save(output_file_path, True)


# This allows running openstudio CLI on this file (`openstudio test_measure.py`, maybe with extra args)
if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        pytest.main([__file__] + sys.argv[1:])
    else:
        pytest.main()
