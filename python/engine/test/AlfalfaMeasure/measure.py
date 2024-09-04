import typing

import openstudio


class AlfalfaMeasure(openstudio.measure.ModelMeasure):
    def name(self):
        return "Alfalfa Measure"

    def modeler_description(self):
        return "The method attempts to build an alfalfa json in the measure"


    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
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

        alfalfa = runner.alfalfa
        alfalfa.addConstant(10, "safe value")
        alfalfa.addMeter("Facility:Electricity", "Facility Electricity")
        alfalfa.addActuator("somehting", "another thing", "key", "example actuator")
        alfalfa.addOutputVariable("Whole Building", "Facility Total Purchased Electricity Energy", "output variable")
        alfalfa.addGlobalVariable("global_1", "global variable")
        super().run(model, runner, user_arguments)  # Do **NOT** remove this line

        return True

AlfalfaMeasure().registerWithApplication()
