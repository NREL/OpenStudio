require 'openstudio'
require 'openstudio/measure/ShowRunnerOutput'
require 'minitest/autorun'

require_relative '../measure.rb'

class EnergyPlusMeasureName_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown

  def test_number_of_arguments_and_argument_names

    # create an instance of the measure
    measure = EnergyPlusMeasureName.new

    # make an empty workspace
    workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel, "EnergyPlus".to_IddFileType)

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(workspace)
    assert_equal(1, arguments.size)
    assert_equal("zone_name", arguments[0].name)
  end

  def test_bad_argument_values

    # create an instance of the measure
    measure = EnergyPlusMeasureName.new

    # create runner with empty OSW
    osw = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(osw)

    # make an empty workspace
    workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel, "EnergyPlus".to_IddFileType)

    # check that there are no thermal zones
    assert_equal(0, workspace.getObjectsByType("Zone".to_IddObjectType).size)

    # get arguments
    arguments = measure.arguments(workspace)
    argument_map = OpenStudio::Measure.convertOSArgumentVectorToMap(arguments)

    # set argument values to bad value
    zone_name = arguments[0].clone
    assert(zone_name.setValue(""))
    argument_map["zone_name"] = zone_name

    # run the measure
    measure.run(workspace, runner, argument_map)
    result = runner.result
    assert_equal("Fail", result.value.valueName)

    # check that there are still no thermal zones
    assert_equal(0, workspace.getObjectsByType("Zone".to_IddObjectType).size)
  end

  def test_good_argument_values

    # create an instance of the measure
    measure = EnergyPlusMeasureName.new

    # create runner with empty OSW
    osw = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(osw)

    # make an empty workspace
    workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel, "EnergyPlus".to_IddFileType)

    # check that there are no thermal zones
    assert_equal(0, workspace.getObjectsByType("Zone".to_IddObjectType).size)

    # get arguments
    arguments = measure.arguments(workspace)
    argument_map = OpenStudio::Measure.convertOSArgumentVectorToMap(arguments)

    # set argument values to good values
    zone_name = arguments[0].clone
    assert(zone_name.setValue("New Zone"))
    argument_map["zone_name"] = zone_name

    # run the measure
    measure.run(workspace, runner, argument_map)
    result = runner.result
    assert_equal("Success", result.value.valueName)

    # check that there is now 1 thermal zones
    assert_equal(1, workspace.getObjectsByType("Zone".to_IddObjectType).size)

    # check that zone is properly named
    zone = workspace.getObjectsByType("Zone".to_IddObjectType)[0]
    assert(!zone.getString(0).empty?)
    assert_equal("New Zone", zone.getString(0).get)

    # save the workspace to output directory
    output_file_path = OpenStudio::Path.new(File.dirname(__FILE__) + "/output/test_output.idf")
    workspace.save(output_file_path,true)
  end

end
