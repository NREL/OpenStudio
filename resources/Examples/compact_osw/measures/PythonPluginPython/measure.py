import os, sys
import openstudio
import typing
sys.path.append('C:/Python38/Lib/site-packages') # this should (needs to?) be same version as E+'s version
import jinja2
import tempfile


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
        return "TODO"

    def modeler_description(self):
        """
        human readable description of modeling approach
        """
        return "TODO"

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        """
        define what happens when the measure is run
        """
        args = openstudio.measure.OSArgumentVector()

        return args

    def run(self,
            model: openstudio.model.Model,
            runner: openstudio.measure.OSRunner,
            user_arguments: openstudio.measure.OSArgumentMap):
        """
        define what happens when the measure is run
        """
        super().run(model, runner, user_arguments)  # Do **NOT** remove this line

        if not(runner.validateUserArguments(self.arguments(model),
                                            user_arguments)):
            return False

        # Add a PythonPlugin:Variable (all OS SDK PythonPluginVariable objects are
        # translated to a single E+ PythonPlugin:Variables (extensible object))
        py_var = openstudio.model.PythonPluginVariable(model)
        py_var.setName('AverageBuildingTemp')

        # Add a PythonPlugin:OutputVariable for that variable
        py_out_var = openstudio.model.PythonPluginOutputVariable(py_var)
        py_out_var.setName('Averaged Building Temperature')
        py_out_var.setTypeofDatainVariable('Averaged')
        py_out_var.setUpdateFrequency('ZoneTimestep')
        py_out_var.setUnits('C')

        # Add a regular Output:Variable that references it
        out_var = openstudio.model.OutputVariable('PythonPlugin:OutputVariable', model)
        out_var.setKeyValue(py_out_var.nameString())
        out_var.setReportingFrequency('Timestep')

        # Add output variables for Zone Mean Air Temperature, so we can compare
        outputVariable = openstudio.model.OutputVariable('Zone Mean Air Temperature', model)
        outputVariable.setReportingFrequency('Timestep')

        # Trend Variable: while this is a fully functioning object, you're probably
        # best just using a storage variable on the Python side (eg: a list)
        py_trend_var = openstudio.model.PythonPluginTrendVariable(py_var)
        py_trend_var.setName('Running Averaged Building Temperature')
        n_timesteps = 24 * model.getTimestep().numberOfTimestepsPerHour()
        py_trend_var.setNumberofTimestepstobeLogged(n_timesteps)

        py_var2 = openstudio.model.PythonPluginVariable(model)
        py_var2.setName('RunningAverageBuildingTemp')

        py_out_trend_var = openstudio.model.PythonPluginOutputVariable(py_var2)
        py_out_trend_var.setName('Running Averaged Building Temperature')
        py_out_trend_var.setTypeofDatainVariable('Averaged')
        py_out_trend_var.setUpdateFrequency('ZoneTimestep')
        py_out_trend_var.setUnits('C')

        out_trend_var = openstudio.model.OutputVariable('PythonPlugin:OutputVariable', model)
        out_trend_var.setReportingFrequency('Timestep')

        pluginClassName = 'AverageZoneTemps'

        # get the python plugin program (jinja template)
        pluginTemplatePath = os.path.join(os.path.dirname(__file__), '../../files/python_plugin_program_jinja.py')
        f = open(pluginTemplatePath, 'r')
        in_py = f.read()

        dataPath = os.path.join(os.path.dirname(__file__), '../../files/python_plugin_program.csv')
        openstudio.model.ExternalFile.getExternalFile(model, dataPath)

        # configure plugin template with variable values
        env = jinja2.Environment()
        template = env.from_string(in_py)
        context = {
            'pluginClassName': pluginClassName,
            'zone_names': sorted([tz.nameString() for tz in model.getThermalZones() ]),
            'py_var': py_var,
            'py_trend_var': py_trend_var,
            'py_var2': py_var2
        }
        out_py = template.render(context)

        # Write it to a temporary directory so we don't pollute the current directory
        # ExternalFile will copy it
        temp_dir = tempfile.TemporaryDirectory()
        pluginPath = os.path.join(temp_dir.name, 'python_plugin_program_jinja.py')
        with open(pluginPath, 'w') as f:
            f.write(out_py)

        # create the external file object
        # os.chdir('../..')
        external_file = openstudio.model.ExternalFile.getExternalFile(model, pluginPath)
        external_file = external_file.get()

        # create the python plugin instance object
        python_plugin_instance = openstudio.model.PythonPluginInstance(external_file, pluginClassName)
        python_plugin_instance.setRunDuringWarmupDays(False)

        return True


# this allows the measure to be used by the application
PythonPluginPython().registerWithApplication()
