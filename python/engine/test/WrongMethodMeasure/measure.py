########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

import typing

import openstudio


class WrongMethodMeasure(openstudio.measure.ModelMeasure):
    def name(self):
        return "Wrong Method Measure"

    def modeler_description(self):
        return "The arguments method calls a non existing method on the model passed as argument"

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        model.nonExistingMethod()
        args = openstudio.measure.OSArgumentVector()

        return args

    def run(
        self,
        model: openstudio.model.Model,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """
        define what happens when the measure is run
        """
        super().run(model, runner, user_arguments)  # Do **NOT** remove this line

WrongMethodMeasure().registerWithApplication()
