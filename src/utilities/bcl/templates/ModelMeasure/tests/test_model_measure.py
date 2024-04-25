"""insert your copyright here."""

import sys
from pathlib import Path

import openstudio
import pytest

CURRENT_DIR_PATH = Path(__file__).parent.absolute()
sys.path.insert(0, str(CURRENT_DIR_PATH.parent))
from measure import ModelMeasureName
sys.path.pop(0)
del sys.modules['measure']


class TestModelMeasureName:
    """Py.test module for ModelMeasureName."""

    def test_number_of_arguments_and_argument_names(self):
        """Test that the arguments are what we expect."""
        # create an instance of the measure
        measure = ModelMeasureName()

        # make an empty model
        model = openstudio.model.Model()

        # get arguments and test that they are what we are expecting
        arguments = measure.arguments(model)
        assert arguments.size() == 1
        assert arguments[0].name() == "space_name"

    def test_bad_argument_values(self):
        """Test running the measure with inappropriate arguments, and that the measure reports failure."""
        # create an instance of the measure
        measure = ModelMeasureName()

        # create runner with empty OSW
        osw = openstudio.WorkflowJSON()
        runner = openstudio.measure.OSRunner(osw)

        # Make an empty model
        model = openstudio.model.Model()

        # get arguments
        arguments = measure.arguments(model)
        argument_map = openstudio.measure.convertOSArgumentVectorToMap(arguments)

        # create hash of argument values.
        # If the argument has a default that you want to use,
        # you don't need it in the dict
        args_dict = {}
        args_dict["space_name"] = ""

        # populate argument with specified hash value if specified
        for arg in arguments:
            temp_arg_var = arg.clone()
            if arg.name() in args_dict:
                assert temp_arg_var.setValue(args_dict[arg.name()])
                argument_map[arg.name()] = temp_arg_var

        # run the measure
        measure.run(model, runner, argument_map)
        result = runner.result()

        # show the output
        # show_output(result)
        print(f"results: {result}")

        # assert that it failed
        assert result.value().valueName() == "Fail"

    def test_good_argument_values(self):
        """Test running the measure with appropriate arguments.

        Asserts that the measure runs fine and with expected results.
        """
        # create an instance of the measure
        measure = ModelMeasureName()

        # create runner with empty OSW
        osw = openstudio.WorkflowJSON()
        runner = openstudio.measure.OSRunner(osw)

        # load the test model
        translator = openstudio.osversion.VersionTranslator()
        path = Path(__file__).parent.absolute() / "example_model.osm"
        model = translator.loadModel(path)
        assert model.is_initialized()
        model = model.get()

        # store the number of spaces in the seed model
        num_spaces_seed = len(model.getSpaces())

        # get arguments
        arguments = measure.arguments(model)
        argument_map = openstudio.measure.convertOSArgumentVectorToMap(arguments)

        # create hash of argument values.
        # If the argument has a default that you want to use,
        # you don't need it in the dict
        args_dict = {}
        args_dict["space_name"] = "New Space"
        # using defaults values from measure.py for other arguments

        # populate argument with specified hash value if specified
        for arg in arguments:
            temp_arg_var = arg.clone()
            if arg.name() in args_dict:
                assert temp_arg_var.setValue(args_dict[arg.name()])
                argument_map[arg.name()] = temp_arg_var

        # Ensure the model did not start with a space named like requested
        assert "New Space" not in [s.nameString() for s in model.getSpaces()]

        # run the measure
        measure.run(model, runner, argument_map)
        result = runner.result()

        # show the output
        # show_output(result)
        print(f"results: {result}")

        # assert that it ran correctly
        assert result.value().valueName() == "Success"
        assert len(result.stepWarnings()) == 0
        assert len(result.stepInfo()) == 1
        assert result.stepInfo()[0] == "Space New Space was added."

        # check that there is now 1 more space
        num_spaces_final = len(model.getSpaces())
        assert num_spaces_final == num_spaces_seed + 1
        assert "New Space" in [s.nameString() for s in model.getSpaces()]
        assert result.stepInitialCondition()
        assert result.stepInitialCondition().get() == "The building started with 4 spaces."

        assert result.stepFinalCondition()
        assert result.stepFinalCondition().get() == "The building finished with 5 spaces."

        # save the model to test output directory
        output_file_path = Path(__file__).parent.absolute() / "output" / "test_output.osm"
        model.save(output_file_path, True)


# This allows running openstudio CLI on this file (`openstudio test_measure.py`, maybe with extra args)
if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        pytest.main([__file__] + sys.argv[1:])
    else:
        pytest.main()
