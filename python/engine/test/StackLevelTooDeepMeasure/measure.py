########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

import typing

import openstudio

def s(x):
    return s(x)

class StackLevelTooDeepMeasure(openstudio.measure.ModelMeasure):
    def name(self):
        return "Stack Level Too Deep Measure"

    def modeler_description(self):
        return "The arguments method calls an infinitely recursive function"

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        s(10)
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

StackLevelTooDeepMeasure().registerWithApplication()
