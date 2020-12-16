# insert your copyright here

require 'openstudio'
require 'openstudio/measure/ShowRunnerOutput'
require 'minitest/autorun'
require_relative '../measure.rb'
require 'fileutils'

class PythonMeasureNameTest < Minitest::Test
  # def setup
  # end

  # def teardown
  # end

  def test_number_of_arguments_and_argument_names
    # create an instance of the measure
    measure = PythonMeasureName.new

    # make an empty model
    #model = OpenStudio::Model::Model.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(1, arguments.size)
    assert_equal('example_arg', arguments[0].name)
  end

  def test_bad_argument_values
    # create an instance of the measure
    measure = PythonMeasureName.new

    # create runner with empty OSW
    osw = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(osw)

    # load fake fmu
    model = OpenStudio::Model::Model.new
    #p = OpenStudio::Path.new("#{File.dirname(__FILE__)}/model_measure_test.fmu")
    #OpenStudio::path p = resourcesPath() / OpenStudio::toPath("utilities/Zip/test1.zip");
    #fmu = OpenStudio::ZipFile.new(p, false);

    # get arguments
    arguments = measure.arguments()
    argument_map = OpenStudio::Measure.convertOSArgumentVectorToMap(arguments)

    # create hash of argument values
    args_hash = {}
    args_hash['example_arg'] = ''

    # populate argument with specified hash value if specified
    arguments.each do |arg|
      temp_arg_var = arg.clone
      if args_hash.key?(arg.name)
        assert(temp_arg_var.setValue(args_hash[arg.name]))
      end
      argument_map[arg.name] = temp_arg_var
    end

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result

    # show the output
    show_output(result)

    # assert that it ran correctly
    assert_equal('Fail', result.value.valueName)
  end

  def test_good_argument_values
    # create an instance of the measure
    measure = PythonMeasureName.new

    # create runner with empty OSW
    osw = OpenStudio::WorkflowJSON.new
    runner = OpenStudio::Measure::OSRunner.new(osw)

    # load the test model
    #translator = OpenStudio::OSVersion::VersionTranslator.new
    #path = "#{File.dirname(__FILE__)}/example_model.osm"
    #model = translator.loadModel(path)
    #assert(!model.empty?)
    #model = model.get
    model = OpenStudio::Model::Model.new

    #p = OpenStudio::Path.new("#{File.dirname(__FILE__)}/model_measure_test.fmu")
    #OpenStudio::path p = resourcesPath() / OpenStudio::toPath("utilities/Zip/test1.zip");
    #fmu = OpenStudio::ZipFile.new(p, false);
    
    # store the number of spaces in the seed model
    #num_spaces_seed = model.getSpaces.size

    # get arguments
    arguments = measure.arguments()
    argument_map = OpenStudio::Measure.convertOSArgumentVectorToMap(arguments)

    # create hash of argument values.
    # If the argument has a default that you want to use, you don't need it in the hash
    args_hash = {}
    args_hash['example_arg'] = 'New Space'
    # using defaults values from measure.rb for other arguments

    # populate argument with specified hash value if specified
    arguments.each do |arg|
      temp_arg_var = arg.clone
      if args_hash.key?(arg.name)
        assert(temp_arg_var.setValue(args_hash[arg.name]))
      end
      argument_map[arg.name] = temp_arg_var
    end
    puts "run measure:"
    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result

    # show the output
    show_output(result)
    puts "results: #{result}"

    # assert that it ran correctly
    assert_equal('Success', result.value.valueName)
    assert(result.info.size == 1)
    assert(result.warnings.empty?)

    assert_equal(result.info[0].logMessage, "PYCALL test result = 0.0")

    # save the model to test output directory
    #output_file_path = "#{File.dirname(__FILE__)}//output/test_output.osm"
    #model.save(output_file_path, true)
  end
end
