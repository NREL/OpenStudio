########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

import typing

import openstudio


class BadMeasure(openstudio.measure.ModelMeasure):
    def name(self):
        return "Bad Measure"

    def modeler_description(self):
        return "The arguments method calls another_method which does a raise ValueError"

    def another_method(self):
        raise ValueError("oops")

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        self.another_method()
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

BadMeasure().registerWithApplication()
