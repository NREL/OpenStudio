require 'openstudio'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'test/unit'

class UtilityMeasure_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_UtilityMeasure
     
    # create an instance of the measure
    measure = UtilityMeasure.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(2, arguments.size)
    assert_equal("user_name", arguments[0].name)
    assert_equal("say_hello", arguments[1].name)
    assert((not arguments[0].hasDefaultValue))

    # set argument values to bad values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    user_name = arguments[0].clone
    assert(user_name.setValue(""))
    argument_map["user_name"] = user_name
    measure.run(runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
       
    # set argument values to good values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    user_name = arguments[0].clone
    assert(user_name.setValue("david"))
    argument_map["user_name"] = user_name
    say_hello = arguments[1].clone
    assert(say_hello.setValue(true))
    argument_map["say_hello"] = say_hello
    measure.run(runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 1)
    assert(result.info.size == 2)
    
  end  

end
