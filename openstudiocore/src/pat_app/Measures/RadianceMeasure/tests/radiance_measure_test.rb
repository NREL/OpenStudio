require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'
require 'minitest/autorun'
require_relative '../measure.rb'
require 'fileutils'

class RadianceMeasureTest < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_number_of_arguments_and_argument_names
    # create an instance of the measure
    measure = RadianceMeasure.new

    # make an empty model
    model = OpenStudio::Model::Model.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    assert_equal(4, arguments.size)
    assert_equal("apply_schedules", arguments[0].name)
    assert_equal("write_sql", arguments[1].name)
    assert_equal("use_cores", arguments[2].name)
    assert_equal("rad_settings", arguments[3].name)
  end

  def test_measure
    # create an instance of the measure
    measure = RadianceMeasure.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # set up runner, this will happen automatically when measure is run in PAT
    
    runner.setLastEpwFilePath(File.dirname(__FILE__) + "/USA_CO_Golden-NREL.724666_TMY3.epw")  
    
    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/test_model.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))

    model = model.get

		weather_file = runner.lastEpwFilePath.get
    epw_file = OpenStudio::EpwFile.new(weather_file)
    OpenStudio::Model::WeatherFile.setWeatherFile(model, epw_file).get
    
    site = model.getSite
    site.setName("Test Site")
    site.setLatitude(epw_file.latitude)
    site.setLongitude(epw_file.longitude)
    site.setTimeZone(epw_file.timeZone)
    
    # store the number of spaces in the seed model
    num_spaces_seed = model.getSpaces.size

    # get arguments
    arguments = measure.arguments(model)
    argument_map = OpenStudio::Ruleset.convertOSArgumentVectorToMap(arguments)

    # create hash of argument values.
    # If the argument has a default that you want to use, you don't need it in the hash
    args_hash = {}
    args_hash["apply_schedules"] = "Yes"
    args_hash["write_sql"] = "No"
    args_hash["use_cores"] = "Max"
    args_hash["rad_settings"] = "Testing"

    # using defaults values from measure.rb for other arguments

    # populate argument with specified hash value if specified
    arguments.each do |arg|
      temp_arg_var = arg.clone
      if args_hash[arg.name]
      	puts "setting #{arg.name} = #{args_hash[arg.name]}"
        assert(temp_arg_var.setValue(args_hash[arg.name]))
      end
      argument_map[arg.name] = temp_arg_var
    end
        
    # move to output dir
    current_dir = Dir.pwd 
    out_dir = File.dirname(__FILE__) + "/output"
    if !File.exists?(out_dir)
      FileUtils.mkdir_p(out_dir)
    end
    Dir.chdir(out_dir)

    # run the measure
    measure.run(model, runner, argument_map)
    result = runner.result

    # show the output

    show_output(result)

    # assert that it ran correctly
    assert_equal("Success", result.value.valueName)
    assert(result.warnings.size == 0)

    # save the model to test output directory
    output_file_path = OpenStudio::Path.new("./test_output.osm")
    model.save(output_file_path,true)
    
    Dir.chdir(current_dir)
    
  end

end
