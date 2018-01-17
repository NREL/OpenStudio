
#require 'openstudio'
require "C:/Projects/OpenStudio/build/OSCore-prefix/src/OSCore-build/ruby/Debug/openstudio"

#require 'test/unit'
require 'minitest/autorun'

  #load a model into OS & version translates, exiting and erroring if a problem is found
  def safe_load_model(model_path_string)  
    model_path = OpenStudio::Path.new(model_path_string)
    if OpenStudio::exists(model_path)
      versionTranslator = OpenStudio::OSVersion::VersionTranslator.new 
      model = versionTranslator.loadModel(model_path)
      if model.empty?
        puts "Version translation failed for #{model_path_string}"
        exit
      else
        model = model.get
      end
    else
      puts "#{model_path_string} couldn't be found"
      exit
    end
    return model
  end
  
#class TestEms < Test::Unit::TestCase
#class TestEms < MiniTest::Unit::TestCase
class TestEms < MiniTest::Test

  def test_ems
    #model = safe_load_model("#{Dir.pwd}/example.osm")
    model = OpenStudio::Model::Model.new
    ### Sensor ###

    # Create an output variable for OATdb
    output_var = "Site Outdoor Air Drybulb Temperature"
    output_var_oat = OpenStudio::Model::OutputVariable.new(output_var, model)

    # Create a sensor to sense the outdoor air temperature
    oat_sensor = OpenStudio::Model::EnergyManagementSystemSensor.new(model)
    oat_sensor_name = "OATdb Sensor"
    oat_sensor.setName(oat_sensor_name)
    oat_sensor.setOutputVariable(output_var_oat)

    # Assertions for sensor setters and getters
    assert_equal oat_sensor_name, oat_sensor.name.get.to_s 
    assert_equal output_var, oat_sensor.outputVariable.get.variableName

    meter = OpenStudio::Model::Meter.new(model)
    meter.setName("test meter")
    
    meter_sensor = OpenStudio::Model::EnergyManagementSystemSensor.new(model)
    meter_sensor_name = "meter Sensor"
    meter_sensor.setName(meter_sensor_name)
    meter_sensor.setOutputMeter(meter)
    
    assert_equal meter_sensor_name, meter_sensor.name.get.to_s 
    assert_equal "test meter", meter_sensor.outputMeter.get.variableName
    
    ### Actuator ###

    # Get the first fan from the example model
    #fan = model.getFanConstantVolumes[0]
    always_on = model.alwaysOnDiscreteSchedule
    fan = OpenStudio::Model::FanConstantVolume.new(model,always_on)

    # Create an actuator to set the fan pressure rise
    fan_actuator = OpenStudio::Model::EnergyManagementSystemActuator.new(fan)
    fan_actuator.setName("#{fan.name} Press Actuator")
    fan_press = "Fan Pressure Rise"
    fan_actuator.setActuatedComponentControlType(fan_press)
    fan_actuator.setActuatedComponentType("fan")
    fan_actuator.setActuatedComponent(fan)

    # Assertions for actuator setters and getters
    assert_equal(fan, fan_actuator.actuatedComponent)
    assert_equal(fan_press, fan_actuator.actuatedComponentControlType)
    assert_equal("fan", fan_actuator.actuatedComponentType)

    equip = OpenStudio::Model::ElectricEquipment.new(model)
    # Create an actuator to set the elec equip
    equip_actuator = OpenStudio::Model::EnergyManagementSystemActuator.new(equip)
    equip_actuator.setName("#{equip.name} power level actuator")
    equip_control = "Electric Power Level"
    equip_actuator.setActuatedComponentControlType(equip_control)
    equip_actuator.setActuatedComponentType("ElectricEquipment")

    ### Program ###

    # Create a program all at once
    fan_program_1 = OpenStudio::Model::EnergyManagementSystemProgram.new(model)
    fan_program_1.setName("#{fan.name} Pressure Rise Program by Line")
    fan_program_1_body = <<-EMS
      SET mult = #{oat_sensor.handle} / 15.0 !- This is nonsense
      SET #{fan_actuator.handle} = 250 * mult !- More nonsense
    EMS
    fan_program_1.setBody(fan_program_1_body)

    # Assertion for the number of lines
    assert_equal(2, fan_program_1.lines.get.size)
    # Assertion for the objects that are referenced
    assert_equal(2, fan_program_1.referencedObjects.size)
    # Assertion for the number of invalid objects
    assert_equal(0, fan_program_1.invalidReferencedObjects.size)
    # Delete the actuator
    fan_actuator.remove
    # Assertion for the new number of invalid objects
    assert_equal(1, fan_program_1.invalidReferencedObjects.size)
    
    # Create a third program from a vector of lines
    fan_program_2 = OpenStudio::Model::EnergyManagementSystemProgram.new(model)
    fan_program_2.setName("#{fan.name} Pressure Rise Program by Line")
    fan_program_2.addLine("SET mult = #{oat_sensor.handle} / 15.0 !- This is nonsense")
    fan_program_2.addLine("SET #{fan_actuator.handle} = 250 * mult !- More nonsense")
    
    # Assertion for the number of lines
    assert_equal(2, fan_program_2.lines.get.size)
    # Assertion for the objects that are referenced
    assert_equal(1, fan_program_2.referencedObjects.size)
    # Assertion for the new number of invalid objects
    assert_equal(1, fan_program_2.invalidReferencedObjects.size)

    # Create a third program from vector of lines
    fan_program_3 = OpenStudio::Model::EnergyManagementSystemProgram.new(model)
    fan_program_3.setName("#{fan.name} Pressure Rise Program by Vector of Lines")
    fan_program_3_lines = []
    fan_program_3_lines << "SET mult = #{oat_sensor.handle} / 15.0 !- This is nonsense"
    fan_program_3_lines << "SET #{fan_actuator.handle} = 250 * mult !- More nonsense"
    fan_program_3.setLines(fan_program_3_lines)

    # Assertion for the number of lines
    assert_equal(2, fan_program_3.lines.get.size)
    # Assertion for the objects that are referenced
    assert_equal(1, fan_program_3.referencedObjects.size)
    # Assertion for the new number of invalid objects
    assert_equal(1, fan_program_3.invalidReferencedObjects.size)
  end

end
