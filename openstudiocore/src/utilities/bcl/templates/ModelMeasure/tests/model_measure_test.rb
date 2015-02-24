require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'

require_relative '../measure.rb'

require 'fileutils'

class ModelMeasureTest < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_number_of_arguments_and_argument_names
    # create an instance of the measure
    measure = ModelMeasure.new

    # make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    assert_equal(1, arguments.size)
    assert_equal("space_name", arguments[0].name)
  end

  def test_bad_argument_values   
    # create an instance of the measure
    measure = ModelMeasure.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # make an empty model
    model = OpenStudio::Model::Model.new

    # check that there are no spaces
    assert_equal(0, model.getSpaces.size)

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset.convertOSArgumentVectorToMap(arguments)

    # set argument values to bad value
    space_name = arguments[0].clone
    assert(space_name.setValue(""))
    argument_map["space_name"] = space_name

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result
    assert_equal("Fail", result.value.valueName)

    # check that there are still no spaces
    assert_equal(0, model.getSpaces.size)
  end

  def test_good_argument_values
    # create an instance of the measure
    measure = ModelMeasure.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # make an empty model
    model = OpenStudio::Model::Model.new
    
    # check that there are no spaces
    assert_equal(0, model.getSpaces.size)

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset.convertOSArgumentVectorToMap(arguments)

    # set argument values to good values
    space_name = arguments[0].clone
    assert(space_name.setValue("New Space"))
    argument_map["space_name"] = space_name

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result
    assert_equal("Success", result.value.valueName)

    # check that there is now 1 space
    assert_equal(1, model.getSpaces.size)

    # check that space is properly named
    space = model.getSpaces[0]
    assert_equal("New Space", space.name.get)
  end

end
