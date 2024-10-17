"""insert your copyright here.

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/
"""

import typing

import openstudio


class AlfalfaModelPython(openstudio.measure.ModelMeasure):
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
        return "AlfalfaModelPython"

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

        return args

    def run(
        self,
        model: openstudio.model.Model,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """Defines what happens when the measure is run."""
        super().run(model, runner, user_arguments)  # Do **NOT** remove this line

        if not (runner.validateUserArguments(self.arguments(model), user_arguments)):
            return False

        alfalfa = runner.alfalfa()

        # Test Meters
        alfalfa.exposeMeter("Electricity:Facility", "Electricity Meter String:Model:Python")

        meter_object = openstudio.model.OutputMeter(model)
        meter_object.setFuelType(openstudio.FuelType("Electricity"))
        meter_object.setInstallLocationType(openstudio.InstallLocationType("Facility"))
        alfalfa.exposeFromObject(meter_object, "Electricity Meter OSM:Model:Python")

        # Test Output Variables
        alfalfa.exposeOutputVariable("EMS", "my_var", "Output Variable String:Model:Python")

        ems_output_variable_object = openstudio.model.EnergyManagementSystemOutputVariable(model, "my_var")
        alfalfa.exposeFromObject(ems_output_variable_object, "EMS Output Variable OSM:Model:Python")

        output_variable_object = openstudio.model.OutputVariable("my_var", model)
        output_variable_object.setKeyValue("EMS")
        alfalfa.exposeFromObject(output_variable_object, "Output Variable OSM:Model:Python")

        # Test Global Variables
        alfalfa.exposeGlobalVariable("my_var", "Global Variable String:Model:Python")

        global_variable_object = openstudio.model.EnergyManagementSystemGlobalVariable(model, "my_var")
        alfalfa.exposeFromObject(global_variable_object, "Global Variable OSM:Model:Python")

        # Test Actuators
        alfalfa.exposeActuator("component_name", "componen_type", "control_type", "Actuator String:Model:Python")

        actuated_zone = openstudio.model.ThermalZone(model)
        actuated_zone.setName("component_name")
        actuator_object = openstudio.model.EnergyManagementSystemActuator(actuated_zone, "component_type", "control_type")
        alfalfa.exposeFromObject(actuator_object, "Actuator OSM:Model:Python")

        return True


# register the measure to be used by the application
AlfalfaModelPython().registerWithApplication()
