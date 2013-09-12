require 'openstudio'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'test/unit'

class ReportingMeasure_Test < Test::Unit::TestCase
  
  def setup
    if not File.exist?(modelPath())
      puts "Creating example model"
      model = OpenStudio::Model::exampleModel()
      model.getRunPeriod.remove
      model.getSimulationControl.setRunSimulationforSizingPeriods(true)
      model.getSimulationControl.setRunSimulationforWeatherFileRunPeriods(false)
      model.save(OpenStudio::Path.new(modelPath()), true)
    end
    
    if not File.exist?(sqlPath())
      puts "Running EnergyPlus"
      
      co = OpenStudio::Runmanager::ConfigOptions.new(true)
      co.findTools(false, true, false, true)
      
      wf = OpenStudio::Runmanager::Workflow.new("modeltoidf->energyplus")
      wf.add(co.getTools())
      job = wf.create(OpenStudio::Path.new(runDir()), OpenStudio::Path.new(modelPath()))

      rm = OpenStudio::Runmanager::RunManager.new
      rm.enqueue(job, true)
      rm.waitForFinished
    end
  end

  # def teardown
  # end
  
  def modelPath
    return "#{File.dirname(__FILE__)}/ExampleModel.osm"
  end
  
  def runDir
    return "#{File.dirname(__FILE__)}/ExampleModel/"
  end
  
  def sqlPath
    return "#{File.dirname(__FILE__)}/ExampleModel/ModelToIdf/EnergyPlus-0/eplusout.sql"
  end
  
  def test_ReportingMeasure
     
    assert(File.exist?(modelPath()))
    assert(File.exist?(sqlPath()))
     
    # create an instance of the measure
    measure = ReportingMeasure.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(0, arguments.size)

    # set argument values to bad values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
    
    # set up runner
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(modelPath))    
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sqlPath))    
       
    # set argument values to good values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 0)
    assert(result.info.size == 2)
    
  end  

end
