require 'openstudio'

require 'openstudio/ruleset/ShowRunnerOutput'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'test/unit'

class EnergyPlusMeasure_Test < Test::Unit::TestCase

  
  def test_EnergyPlusMeasure
     
    # create an instance of the measure
    measure = EnergyPlusMeasure.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # make an empty model
    model = OpenStudio::Model::Model.new

    # forward translate OpenStudio Model to EnergyPlus Workspace
    ft = OpenStudio::EnergyPlus::ForwardTranslator.new
    workspace = ft.translateModel(model)
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(workspace)
    assert_equal(2, arguments.size)
    assert_equal("user_name", arguments[0].name)
    assert_equal("add_zone", arguments[1].name)
    assert((not arguments[0].hasDefaultValue))

    # set argument values to bad values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    user_name = arguments[0].clone
    assert(user_name.setValue(""))
    argument_map["user_name"] = user_name
    measure.run(workspace, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
       
    # set argument values to good values and run the measure on the workspace
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    user_name = arguments[0].clone
    assert(user_name.setValue("david"))
    argument_map["user_name"] = user_name
    add_space = arguments[1].clone
    assert(add_space.setValue(true))
    argument_map["add_zone"] = add_space
    measure.run(workspace, runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 1)
    assert(result.info.size == 2)
    
  end
  

end