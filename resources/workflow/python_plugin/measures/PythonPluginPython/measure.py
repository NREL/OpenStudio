import tempfile
import typing
from pathlib import Path

import jinja2
import openstudio

RESOURCES_DIR = Path(__file__).parent / "resources"


class PythonPluginPython(openstudio.measure.ModelMeasure):
    def name(self):
        """
        Return the human readable name.
        Measure name should be the title case of the class name.
        """
        return "Python Plugin Python."

    def description(self):
        """
        human readable description
        """
        return "Generates an EnergyPlus PythonPlugin python file"

    def modeler_description(self):
        """
        human readable description of modeling approach
        """
        return "Uses a Jinja template to render the PythonPlugin"

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        """
        define what happens when the measure is run
        """
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

        if not (runner.validateUserArguments(self.arguments(model), user_arguments)):
            return False

        # Add a PythonPlugin:Variable (all OS SDK PythonPluginVariable objects are
        # translated to a single E+ PythonPlugin:Variables (extensible object))
        py_var = openstudio.model.PythonPluginVariable(model)
        py_var.setName("PythonAverageBuildingTemp")

        # Add a PythonPlugin:OutputVariable for that variable
        py_out_var = openstudio.model.PythonPluginOutputVariable(py_var)
        py_out_var.setName("Python Averaged Building Temperature")
        py_out_var.setTypeofDatainVariable("Averaged")
        py_out_var.setUpdateFrequency("ZoneTimestep")
        py_out_var.setUnits("C")

        # Add a regular Output:Variable that references it
        out_var = openstudio.model.OutputVariable("PythonPlugin:OutputVariable", model)
        out_var.setKeyValue(py_out_var.nameString())
        out_var.setReportingFrequency("Timestep")

        # Add output variables for Zone Mean Air Temperature, so we can compare
        outputVariable = openstudio.model.OutputVariable("Zone Mean Air Temperature", model)
        outputVariable.setReportingFrequency("Timestep")

        # Trend Variable: while this is a fully functioning object, you're probably
        # best just using a storage variable on the Python side (eg: a list)
        py_trend_var = openstudio.model.PythonPluginTrendVariable(py_var)
        py_trend_var.setName("Python Running Averaged Building Temperature")
        n_timesteps = 24 * model.getTimestep().numberOfTimestepsPerHour()
        py_trend_var.setNumberofTimestepstobeLogged(n_timesteps)

        py_var2 = openstudio.model.PythonPluginVariable(model)
        py_var2.setName("Python RunningAverageBuildingTemp")

        py_out_trend_var = openstudio.model.PythonPluginOutputVariable(py_var2)
        py_out_trend_var.setName("Python Running Averaged Building Temperature")
        py_out_trend_var.setTypeofDatainVariable("Averaged")
        py_out_trend_var.setUpdateFrequency("ZoneTimestep")
        py_out_trend_var.setUnits("C")

        out_trend_var = openstudio.model.OutputVariable("PythonPlugin:OutputVariable", model)
        out_trend_var.setReportingFrequency("Timestep")

        pluginClassName = "PythonAverageZoneTemps"

        # get the python plugin program (jinja template)
        pluginTemplatePath = RESOURCES_DIR / "python_plugin_program.py.jinja"
        in_py = pluginTemplatePath.read_text()

        dataPath = RESOURCES_DIR / "python_plugin_program.csv"
        openstudio.model.ExternalFile.getExternalFile(model, str(dataPath))

        # configure plugin template with variable values
        env = jinja2.Environment()
        template = env.from_string(in_py)
        context = {
            "pluginClassName": pluginClassName,
            "zone_names": sorted([tz.nameString() for tz in model.getThermalZones()]),
            "py_var": py_var,
            "py_trend_var": py_trend_var,
            "py_var2": py_var2,
        }
        out_py = template.render(context)

        # Write it to a temporary directory so we don't pollute the current directory
        # ExternalFile will copy it
        with tempfile.TemporaryDirectory() as temp_dir:
            pluginPath = Path(temp_dir) / "python_plugin_program_jinja.py"
            pluginPath.write_text(out_py)

            copyFile = True
            external_file = openstudio.model.ExternalFile.getExternalFile(model, str(pluginPath), copyFile)
            external_file = external_file.get()

        # create the python plugin instance object
        python_plugin_instance = openstudio.model.PythonPluginInstance(external_file, pluginClassName)
        python_plugin_instance.setRunDuringWarmupDays(False)

        return True


# this allows the measure to be used by the application
PythonPluginPython().registerWithApplication()
