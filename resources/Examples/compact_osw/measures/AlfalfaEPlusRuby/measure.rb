# insert your copyright here

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

# start the measure
class AlfalfaEPlusRuby < OpenStudio::Measure::EnergyPlusMeasure
  # human readable name
  def name
    # Measure name should be the title case of the class name.
    return 'AlfalfaEPlusRuby'
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
  def arguments(workspace)
    args = OpenStudio::Measure::OSArgumentVector.new

    return args
  end

  # define what happens when the measure is run
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)  # Do **NOT** remove this line

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(workspace), user_arguments)
      return false
    end

    alfalfa = runner.alfalfa

    # Test Meters
    alfalfa.exposeMeter("Electricity:Facility", "Electricity Meter String:EPlus:Ruby")

    meter_object = OpenStudio::IdfObject.load("Output:Meter, Electricity:Facility;").get()
    alfalfa.exposeFromObject(meter_object, "Electricity Meter IDF:Eplus:Ruby")

    # Test Composite Point
    meter_component = OpenStudio::Alfalfa::AlfalfaMeter.new("Electricity:Facility")
    actuator_component = OpenStudio::Alfalfa::AlfalfaActuator.new("component_name", "component_type", "control_type")
    composite_point = OpenStudio::Alfalfa::AlfalfaPoint.new("Compound Point:Ruby")
    composite_point.setOutput(meter_component)
    composite_point.setInput(actuator_component)
    alfalfa.exposePoint(composite_point)

    # Test Expose from Component
    alfalfa.exposeFromComponent(actuator_component, "From Component Actuator")

    # Test Output Variables
    alfalfa.exposeOutputVariable("EMS", "my_var", "Output Variable String:EPlus:Ruby")

    ems_output_variable_object = OpenStudio::IdfObject.load("EnergyManagementSystem:OutputVariable,My Var,my_var,,ZoneTimestep,,;").get()
    alfalfa.exposeFromObject(ems_output_variable_object, "EMS Output Variable IDF:EPlus:Ruby")

    output_variable_object = OpenStudio::IdfObject.load("Output:Variable,EMS,my_var,timstep;").get()
    alfalfa.exposeFromObject(output_variable_object, "Output Variable IDF:EPlus:Ruby")

    # Test Global Variables
    alfalfa.exposeGlobalVariable("my_var", "Global Variable String:EPlus:Ruby")

    global_variable_object = OpenStudio::IdfObject.load("EnergyManagementSystem:GlobalVariable,my_var;").get()
    alfalfa.exposeFromObject(global_variable_object, "Global Variable IDF:EPlus:Ruby")

    # Test Actuators
    alfalfa.exposeActuator("component_name", "component_type", "control_type", "Actuator String:EPlus:Ruby")

    actuator_object = OpenStudio::IdfObject.load("EnergyManagementSystem:Actuator,MyActuator,component_name,component_type,control_type;").get()
    alfalfa.exposeFromObject(actuator_object, "Actuator IDF:EPlus:Ruby")

    return true
  end
end

# register the measure to be used by the application
AlfalfaEPlusRuby.new.registerWithApplication
