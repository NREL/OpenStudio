require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'
require 'fileutils'
require_relative '../measure.rb'

class ModelMeasureTest < MiniTest::Test

  def test_number_of_arguments_and_argument_names
    # create an instance of the measure
    measure = ModelMeasure.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    assert_equal(2, arguments.size)
    assert_equal("user_name", arguments[0].name)
    assert_equal("add_space", arguments[1].name)
  end
  
  def test_bad_argument_values   
    # create an instance of the measure
    measure = ModelMeasure.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(arguments)

    # set argument values to bad value
    user_name = arguments[0].clone
    assert(user_name.setValue(""))
    argument_map["user_name"] = user_name

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
  end
   
  def test_existing_model   
    # create an instance of the measure
    measure = ModelMeasure.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/TestModel.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get
 
    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(arguments)

    # set argument values that are not defaulted
    user_name = arguments[0].clone
    assert(user_name.setValue("david"))
    argument_map["user_name"] = user_name
    
    # run the measure including log of output messages shown in GUI
    measure.run(model, runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 1)
    assert(result.info.size == 2)    
    
    # save the resulting model
    output_dir = File.expand_path('output', File.dirname(__FILE__))
    FileUtils.mkdir output_dir unless Dir.exist? output_dir
    output_file_path = OpenStudio::Path.new("#{output_dir}/TestModel_SpaceAdded.osm")
    model.save(output_file_path,true)    
  end
end
