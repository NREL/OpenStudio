require 'openstudio'

require 'openstudio/ruleset/ShowRunnerOutput'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'fileutils'

require 'test/unit'

class CalibrationReports_Test < Test::Unit::TestCase

  # paths to expected test files, includes osm and eplusout.sql
  def modelPath
    return "#{File.dirname(__FILE__)}/ExampleModel.osm"
  end

  def runDir
    return "#{File.dirname(__FILE__)}/ExampleModel/"
  end

  def sqlPath
    return "#{File.dirname(__FILE__)}/ExampleModel/ModelToIdf/EnergyPlusPreProcess-0/EnergyPlus-0/eplusout.sql"
  end

  def reportPath
    return "./report.html"
  end

  # create test files if they do not exist
  def setup

    if File.exist?(reportPath())
      FileUtils.rm(reportPath())
    end

    assert(File.exist?(modelPath()))

    assert(File.exist?(runDir()))

    if not File.exist?(sqlPath())
      puts "Running EnergyPlus"

      co = OpenStudio::Runmanager::ConfigOptions.new(true)
      co.findTools(false, true, false, true)

      wf = OpenStudio::Runmanager::Workflow.new("modeltoidf->energypluspreprocess->energyplus")
      wf.add(co.getTools())
      job = wf.create(OpenStudio::Path.new(runDir()), OpenStudio::Path.new(modelPath()))

      rm = OpenStudio::Runmanager::RunManager.new
      rm.enqueue(job, true)
      rm.waitForFinished
    end
  end

  # delete output files
  def teardown

    # comment this out if you don't want to rerun EnergyPlus each time
    if File.exist?(sqlPath())
      #FileUtils.rm(sqlPath())
    end

    # comment this out if you want to see the resulting report
    if File.exist?(reportPath())
      #FileUtils.rm(reportPath())
    end
  end

  # the actual test
  def test_CalibrationReports

    assert(File.exist?(modelPath()))
    assert(File.exist?(sqlPath()))

    # create an instance of the measure
    measure = CalibrationReports.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(0, arguments.size)

    # set up runner, this will happen automatically when measure is run in PAT
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(modelPath()))
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sqlPath()))

    # set argument values to good values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 0)
    assert(result.info.size == 0)

    assert(File.exist?(reportPath()))

    # get the last model and sql file

    model = runner.lastOpenStudioModel
    assert((not model.empty?))
    model = model.get

    sqlFile = runner.lastEnergyPlusSqlFile
    assert((not sqlFile.empty?))
    sqlFile = sqlFile.get

    model.setSqlFile(sqlFile)

    # must have a runPeriod
    runPeriod = model.runPeriod
    assert((not runPeriod.empty?))

    # must have a calendarYear
    yearDescription = model.yearDescription
    assert((not yearDescription.empty?))
    calendarYear = yearDescription.get.calendarYear
    assert((not calendarYear.empty?))

    # check for varying demand
    model.getUtilityBills.each do |utilityBill|
      if not utilityBill.peakDemandUnitConversionFactor.empty?
        hasVaryingDemand = false
        modelPeakDemand = 0.0
        count = 0
        utilityBill.billingPeriods.each do |billingPeriod|
          peakDemand = billingPeriod.modelPeakDemand
          if not peakDemand.empty?
            temp = peakDemand.get
            if count == 0
              modelPeakDemand = temp
            else
              if modelPeakDemand != temp
                hasVaryingDemand = true
                break
              end
            end
            count = count + 1
          end
        end
        if count > 1
          assert(hasVaryingDemand)
        end
      end
    end

  end
  

  def test_CalibrationReport_NoGas

    assert(File.exist?(modelPath()))
    assert(File.exist?(sqlPath()))

    # create an instance of the measure
    measure = CalibrationReports.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(0, arguments.size)
    
    # load model, remove gas bills, save to new file
    vt = OpenStudio::OSVersion::VersionTranslator.new
    model = vt.loadModel(modelPath())
    assert((not model.empty?))
    model = model.get
    utilityBills = model.getUtilityBills
    assert_equal(2, utilityBills.size())
    utilityBills.each do |utilityBill|
      if utilityBill.fuelType == "Gas".to_FuelType
        utilityBill.remove
      end
    end
    utilityBills = model.getUtilityBills
    assert_equal(1, utilityBills.size())
    
    newModelPath = modelPath().gsub("ExampleModel.osm", "ExampleModelNoGas.osm")
    model.save(OpenStudio::Path.new(newModelPath), true)

    # set up runner, this will happen automatically when measure is run in PAT
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(newModelPath))
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sqlPath))

    # set argument values to good values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 0)
    assert(result.info.size == 0)

    assert(File.exist?(reportPath()))

    # get the last model and sql file

    model = runner.lastOpenStudioModel
    assert((not model.empty?))
    model = model.get

    sqlFile = runner.lastEnergyPlusSqlFile
    assert((not sqlFile.empty?))
    sqlFile = sqlFile.get

    model.setSqlFile(sqlFile)

    # must have a runPeriod
    runPeriod = model.runPeriod
    assert((not runPeriod.empty?))

    # must have a calendarYear
    yearDescription = model.yearDescription
    assert((not yearDescription.empty?))
    calendarYear = yearDescription.get.calendarYear
    assert((not calendarYear.empty?))

    # check for varying demand
    model.getUtilityBills.each do |utilityBill|
      if not utilityBill.peakDemandUnitConversionFactor.empty?
        hasVaryingDemand = false
        modelPeakDemand = 0.0
        count = 0
        utilityBill.billingPeriods.each do |billingPeriod|
          peakDemand = billingPeriod.modelPeakDemand
          if not peakDemand.empty?
            temp = peakDemand.get
            if count == 0
              modelPeakDemand = temp
            else
              if modelPeakDemand != temp
                hasVaryingDemand = true
                break
              end
            end
            count = count + 1
          end
        end
        if count > 1
            assert(hasVaryingDemand)
        end
      end
    end

  end
  
  # the actual test
  def test_CalibrationReports_NoDemand

    assert(File.exist?(modelPath()))
    assert(File.exist?(sqlPath()))

    # create an instance of the measure
    measure = CalibrationReports.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # get arguments and test that they are what we are expecting
    arguments = measure.arguments()
    assert_equal(0, arguments.size)
    
    # load model, remove demand, save to new file
    vt = OpenStudio::OSVersion::VersionTranslator.new
    model = vt.loadModel(modelPath())
    assert((not model.empty?))
    model = model.get
    utilityBills = model.getUtilityBills
    assert_equal(2, utilityBills.size())
    utilityBills.each do |utilityBill|
      if utilityBill.fuelType == "Electricity".to_FuelType
        utilityBill.billingPeriods.each do |billingPeriod|
          billingPeriod.resetPeakDemand
        end
      end
    end
    utilityBills = model.getUtilityBills
    assert_equal(2, utilityBills.size())
    
    newModelPath = modelPath().gsub("ExampleModel.osm", "ExampleModelNoDemand.osm")
    model.save(OpenStudio::Path.new(newModelPath), true)
    
    # set up runner, this will happen automatically when measure is run in PAT
    runner.setLastOpenStudioModelPath(OpenStudio::Path.new(newModelPath))
    runner.setLastEnergyPlusSqlFilePath(OpenStudio::Path.new(sqlPath()))

    # set argument values to good values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 0)
    assert(result.info.size == 0)

    assert(File.exist?(reportPath()))

    # get the last model and sql file

    model = runner.lastOpenStudioModel
    assert((not model.empty?))
    model = model.get

    sqlFile = runner.lastEnergyPlusSqlFile
    assert((not sqlFile.empty?))
    sqlFile = sqlFile.get

    model.setSqlFile(sqlFile)

    # must have a runPeriod
    runPeriod = model.runPeriod
    assert((not runPeriod.empty?))

    # must have a calendarYear
    yearDescription = model.yearDescription
    assert((not yearDescription.empty?))
    calendarYear = yearDescription.get.calendarYear
    assert((not calendarYear.empty?))

    # check for no demand
    model.getUtilityBills.each do |utilityBill|
      utilityBill.billingPeriods.each do |billingPeriod|
        assert(billingPeriod.peakDemand.empty?)
      end
    end

  end
end
