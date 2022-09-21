"""insert your copyright here.

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/
"""

import openstudio
import typing


class ModelMeasureName(openstudio.measure.ModelMeasure):
    """A ModelMeasure."""

    def name(self):
        """Returns the human readable name.

        Measure name should be the title case of the class name.
        The measure name is the first contact a user has with the measure;
        it is also shared throughout the measure workflow, visible in the OpenStudio Application,
        PAT, Server Management Consoles, and in output reports.
        As such, measure names should clearly describe the measure's function,
        while remaining general in nature
        """
        return "NAME_TEXT"

    def description(self):
        """Human readable description.

        The measure description is intended for a general audience and should not assume
        that the reader is familiar with the design and construction practices suggested by the measure.
        """
        return "DESCRIPTION_TEXT"

    def modeler_description(self):
        """Human readable description of modeling approach.

        The modeler description is intended for the energy modeler using the measure.
        It should explain the measure's intent, and include any requirements about
        how the baseline model must be set up, major assumptions made by the measure,
        and relevant citations or references to applicable modeling resources
        """
        return "MODELER_DESCRIPTION_TEXT"

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        """Prepares user arguments for the measure.

        Measure arguments define which -- if any -- input parameters the user may set before running the measure.
        """
        args = openstudio.measure.OSArgumentVector()

        example_arg = openstudio.measure.OSArgument.makeStringArgument("space_name", True)
        example_arg.setDisplayName("New space name")
        example_arg.setDescription("This name will be used as the name of the new space.")
        args.append(example_arg)

        return args

    def run(
        self,
        model: openstudio.model.Model,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """Defines what happens when the measure is run."""
        super().run(model, runner, user_arguments)

        if not (runner.validateUserArguments(self.arguments(model), user_arguments)):
            return False

        # assign the user inputs to variables
        space_name = runner.getStringArgumentValue("space_name", user_arguments)

        # check the example_arg for reasonableness
        if not space_name:
            runner.registerError("Empty space name was entered.")
            return False

        # report initial condition of model
        runner.registerInitialCondition(f"The building started with {len(model.getSpaces())} spaces.")

        # add a new space to the model
        new_space = openstudio.model.Space(model)
        new_space.setName(space_name)

        # echo the new space's name back to the user
        runner.registerInfo(f"Space {new_space.nameString()} was added.")

        # report final condition of model
        runner.registerFinalCondition(f"The building finished with {len(model.getSpaces())} spaces.")

        return True


# register the measure to be used by the application
ModelMeasureName().registerWithApplication()
