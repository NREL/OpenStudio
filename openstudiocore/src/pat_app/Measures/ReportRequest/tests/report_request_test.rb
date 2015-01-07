require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'

require_relative '../measure.rb'

class ReportRequest_Test < MiniTest::Test

  # def setup
  # end

  # def teardown
  
  def test_number_of_arguments_and_argument_names
  
    # create an instance of the measure
    measure = ReportRequest.new
    
    # make an empty workspace
    workspace = OpenStudio::Workspace.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(workspace)
    assert_equal(1, arguments.size)
    assert_equal("zone_name", arguments[0].name)
  end
  
  def test_bad_argument_values
     
    # create an instance of the measure
    measure = ReportRequest.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # make an empty workspace
    workspace = OpenStudio::Workspace.new
    
    # check that there are no thermal zones
    assert_equal(0, workspace.getObjectsByType("Zone".to_IddObjectType).size)

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(arguments)
    
    # set argument values to bad value
    zone_name = arguments[0].clone
    assert(zone_name.setValue(""))
    argument_map["zone_name"] = zone_name

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result
    assert_equal("Fail", result.value.valueName)
    
    # check that there are still no thermal zones
    assert_equal(0, workspace.getObjectsByType("Zone".to_IddObjectType).size)
  end

  def test_good_argument_values
     
    # create an instance of the measure
    measure = ReportRequest.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # make an empty workspace
    workspace = OpenStudio::Workspace.new
    
    # check that there are no thermal zones
    assert_equal(0, workspace.getObjectsByType("Zone".to_IddObjectType).size)

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(arguments)
    
    # set argument values to bad value
    zone_name = arguments[0].clone
    assert(zone_name.setValue("New Zone"))
    argument_map["zone_name"] = zone_name

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result
    assert_equal("Success", result.value.valueName)
    
    # check that there is now 1 thermal zones
    assert_equal(1, workspace.getObjectsByType("Zone".to_IddObjectType).size)
    
    # check that zone is properly named
    zone = workspace.getObjectsByType("Zone".to_IddObjectType)[0]
    assert_equal("New Zone", zone.getString(0))
  end

end
