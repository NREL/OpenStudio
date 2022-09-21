# insert your copyright here

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

import openstudio
import typing

class ModelMeasureName(openstudio.measure.ModelMeasure):
    """
    My Measure
    """

    def name(self):
        """
        Return the human readable name.
        Measure name should be the title case of the class name.
        """
        return "ModelMeasureName"

    def description(self):
        """
        human readable description
        """
        return "DESCRIPTION_TEXT"

    def modeler_description(self):
        """
        human readable description of modeling approach
        """
        return "MODELER_DESCRIPTION_TEXT"

    def arguments(self, model: typing.Optional[openstudio.model.Model]=None):
        """
        define what happens when the measure is run
        """
        args = openstudio.measure.OSArgumentVector()

        example_arg = openstudio.measure.OSArgument.makeStringArgument('space_name', True)
        example_arg.setDisplayName('New space name')
        example_arg.setDescription('This name will be used as the name of the new space.')
        args.append(example_arg)

        return args

    def run(self,
            model: openstudio.model.Model,
            runner: openstudio.measure.OSRunner,
            user_arguments: openstudio.measure.OSArgumentMap):
        """
        define what happens when the measure is run
        """
        super().run(model, runner, user_arguments)

        if not(runner.validateUserArguments(self.arguments(model),
                                            user_arguments)):
            return False

        # assign the user inputs to variables
        space_name = runner.getStringArgumentValue('space_name',
                                                   user_arguments)

        # check the example_arg for reasonableness
        if not space_name:
            runner.registerError('Empty space name was entered.')
            return False

        # report initial condition of model
        runner.registerInitialCondition(
            f"The building started with {len(model.getSpaces())} spaces."
        )

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
