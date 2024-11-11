"""insert your copyright here.

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/
"""

import openstudio


class AlfalfaEPlusPython(openstudio.measure.EnergyPlusMeasure):
    """An EnergyPlusMeasure."""

    def name(self):
        """Returns the human readable name.

        Measure name should be the title case of the class name.
        The measure name is the first contact a user has with the measure;
        it is also shared throughout the measure workflow, visible in the OpenStudio Application,
        PAT, Server Management Consoles, and in output reports.
        As such, measure names should clearly describe the measure's function,
        while remaining general in nature
        """
        return "AlfalfaEPlusPython"

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

    def arguments(self, workspace: openstudio.Workspace):
        """Prepares user arguments for the measure.

        Measure arguments define which -- if any -- input parameters the user may set before running the measure.
        """
        args = openstudio.measure.OSArgumentVector()

        return args

    def run(
        self,
        workspace: openstudio.Workspace,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """Defines what happens when the measure is run."""
        super().run(workspace, runner, user_arguments)  # Do **NOT** remove this line

        if not (runner.validateUserArguments(self.arguments(workspace), user_arguments)):
            return False

        alfalfa = runner.alfalfa()

        # Test Meters
        alfalfa.exposeMeter("Electricity:Facility", "Electricity Meter String:EPlus:Python")

        meter_object = openstudio.IdfObject.load("Output:Meter, Electricity:Facility;").get()
        alfalfa.exposeFromObject(meter_object, "Electricity Meter IDF:Eplus:Python")

        # Test Composite Point
        meter_component = openstudio.alfalfa.AlfalfaMeter("Electricity:Facility")
        actuator_component = openstudio.alfalfa.AlfalfaActuator("component_name", "component_type", "control_type")
        composite_point = openstudio.alfalfa.AlfalfaPoint("Compound Point:Ruby")
        composite_point.setOutput(meter_component)
        composite_point.setInput(actuator_component)
        alfalfa.exposePoint(composite_point)

        # Test Expose from Component
        alfalfa.exposeFromComponent(actuator_component, "From Component Actuator")

        # Test Output Variables
        alfalfa.exposeOutputVariable("EMS", "my_var", "Output Variable String:EPlus:Python")

        ems_output_variable_object = openstudio.IdfObject.load("EnergyManagementSystem:OutputVariable,My Var,my_var,,ZoneTimestep,,;").get()
        alfalfa.exposeFromObject(ems_output_variable_object, "EMS Output Variable IDF:EPlus:Python")

        output_variable_object = openstudio.IdfObject.load("Output:Variable,EMS,my_var,timstep;").get()
        alfalfa.exposeFromObject(output_variable_object, "Output Variable IDF:EPlus:Python")

        # Test Global Variables
        alfalfa.exposeGlobalVariable("my_var", "Global Variable String:EPlus:Python")

        global_variable_object = openstudio.IdfObject.load("EnergyManagementSystem:GlobalVariable,my_var;").get()
        alfalfa.exposeFromObject(global_variable_object, "Global Variable IDF:EPlus:Python")

        # Test Actuators
        alfalfa.exposeActuator("component_name", "componen_type", "control_type", "Actuator String:EPlus:Python")

        actuator_object = openstudio.IdfObject.load("EnergyManagementSystem:Actuator,MyActuator,component_name,component_type,control_type;").get()
        alfalfa.exposeFromObject(actuator_object, "Actuator IDF:EPlus:Python")

        return True


# register the measure to be used by the application
AlfalfaEPlusPython().registerWithApplication()
