# insert your copyright here

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

# start the measure
class AlfalfaModelRuby < OpenStudio::Measure::ModelMeasure
  # human readable name
  def name
    # Measure name should be the title case of the class name.
    return 'AlfalfaModelRuby'
  end

  # human readable description
  def description
    return 'DESCRIPTION_TEXT'
  end

  # human readable description of modeling approach
  def modeler_description
    return 'MODELER_DESCRIPTION_TEXT'
  end

  # define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  # define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)  # Do **NOT** remove this line

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    alfalfa = runner.alfalfa()

    # Test Meters
    alfalfa.exposeMeter("Electricity:Facility", "Electricity Meter String:Model:Ruby")

    meter_object = OpenStudio::Model::OutputMeter.new(model)
    meter_object.setFuelType(OpenStudio::FuelType.new("Electricity"))
    meter_object.setInstallLocationType(OpenStudio::InstallLocationType.new("Facility"))
    alfalfa.exposeFromObject(meter_object, "Electricity Meter OSM:Model:Ruby")

    # Test Output Variables
    alfalfa.exposeOutputVariable("EMS", "my_var_1", "Output Variable String:Model:Ruby")

    ems_output_variable_object = OpenStudio::Model::EnergyManagementSystemOutputVariable.new(model, "my_var_1")
    alfalfa.exposeFromObject(ems_output_variable_object, "EMS Output Variable OSM:Model:Ruby")

    output_variable_object = OpenStudio::Model::OutputVariable.new("my_var_1", model)
    output_variable_object.setKeyValue("EMS")
    alfalfa.exposeFromObject(output_variable_object, "Output Variable OSM:Model:Ruby")

    # Test Global Variables
    alfalfa.exposeGlobalVariable("my_var_1", "Global Variable String:Model:Ruby")

    global_variable_object = OpenStudio::Model::EnergyManagementSystemGlobalVariable.new(model, "my_var_1")
    alfalfa.exposeFromObject(global_variable_object, "Global Variable OSM:Model:Ruby")

    # Test Actuators
    alfalfa.exposeActuator("component_name", "componen_type", "control_type", "Actuator String:Model:Ruby")

    actuated_zone = OpenStudio::Model::ThermalZone.new(model)
    actuated_zone.setName("component_name")
    actuator_object = OpenStudio::Model::EnergyManagementSystemActuator.new(actuated_zone, "component_type", "control_type")
    alfalfa.exposeFromObject(actuator_object, "Actuator OSM:Model:Ruby")

    return true
  end
end

# register the measure to be used by the application
AlfalfaModelRuby.new.registerWithApplication
