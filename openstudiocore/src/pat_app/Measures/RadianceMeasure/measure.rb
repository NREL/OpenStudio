# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/measures/measure_writing_guide/

# design of this measure is at https://docs.google.com/document/d/16_TLRuhc4VFs2o0gRAp81hRObet7-s6fUEWo3HO7LpE/edit#

require_relative 'resources/RadiancePreprocess'

# start the measure
class RadianceMeasure < OpenStudio::Ruleset::ModelUserScript

  # human readable name
  def name
    return "Radiance Daylighting Measure"
  end

  # human readable description
  def description
    return "Does stuff"
  end

  # human readable description of modeling approach
  def modeler_description
    return "Things"
  end

  # define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    chs = OpenStudio::StringVector.new
    chs << 'Yes'
    chs << 'No'
    apply_schedules = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('apply_schedules', chs, true)
    apply_schedules.setDisplayName('Apply schedule')
    apply_schedules.setDefaultValue('Yes')
    apply_schedules.setDescription('Replace lighting and shading control schedules with schedules computed by radiance')
    args << apply_schedules
    
    chs = OpenStudio::StringVector.new
    chs << 'Yes'
    chs << 'No'
    write_sql = OpenStudio::Ruleset::OSArgument.makeStringArgument('write_sql', true)
    write_sql.setDisplayName('Write Radiance SqlFile')
    apply_schedules.setDefaultValue('Yes')
    write_sql.setDescription('Write Radiance results to a SqlFile format.')
    args << write_sql

    return args
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    OpenStudio::Logger::instance().standardOutLogger().enable()
    #OpenStudio::Logger::instance().standardOutLogger().setLogLevel(OpenStudio::Debug)

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # assign the user inputs to variables
    apply_schedules = runner.getStringArgumentValue('apply_schedules', user_arguments)
    apply_schedules = (apply_schedules == 'Yes')
    
    write_sql = runner.getStringArgumentValue('write_sql', user_arguments)
    write_sql = (write_sql == 'Yes')

		# Radiance model goes here
    out_dir = "./radiance"
    if !File.exists?(out_dir)
      FileUtils.mkdir_p(out_dir)
    end

    # Energyplus "pre-run" model goes here
    epout_dir = "./energyplus"
    if !File.exists?(epout_dir)
      FileUtils.mkdir_p(epout_dir)
    end

   	# save osm to prerun dir   	
    eplusin_file_path = OpenStudio::Path.new("eplusin.osm")
    model.save(eplusin_file_path,true)

		# queue the job to run the prerun model 

		# find EnergyPlus
		co = OpenStudio::Runmanager::ConfigOptions.new 
		co.fastFindEnergyPlus
 
		# make a workflow
		workflow = OpenStudio::Runmanager::Workflow.new("ModelToRadPreprocess->ModelToIdf->ExpandObjects->EnergyPlus")
		workflow.add(co.getTools)

		# try to minimize file path lengths
		workflow.addParam(OpenStudio::Runmanager::JobParam.new("flatoutdir"))

		# make a run manager

		runDir = OpenStudio::Path.new(epout_dir)
		runmanager_path = OpenStudio::Path.new("runmanager.db")
		runmanager = OpenStudio::Runmanager::RunManager.new(runmanager_path, true, true, false, false)

		OpenStudio::makeParentFolder(runDir, OpenStudio::Path.new(), true)
		puts "Creating workflow"
		
		jobtree = workflow.create(OpenStudio::system_complete(runDir), OpenStudio::system_complete(eplusin_file_path), OpenStudio::Path.new(runner.lastEpwFilePath.get))
		runmanager.enqueue(jobtree, true)
		#runmanager.getJobs.each {|job| job.setBasePath(resourcePath)} # DLM: need to be able to get this from runner
		puts "Running jobs in #{runDir}"
		runmanager.setPaused(false)
		runmanager.waitForFinished()

    # report initial condition of model
    daylightAnalysisSpaces = []
    spaces = model.getSpaces
    spaces.each do |sp|
    	if sp.illuminanceMaps.size > 0 
    		daylightAnalysisSpaces << sp
    	end
    end
    puts "Input building model contains #{daylightAnalysisSpaces.size} daylight analysis spaces."
    runner.registerInitialCondition("Input building model contains #{model.getSpaces.size} spaces.")

    # report final condition of model
    runner.registerFinalCondition("The building finished with #{model.getSpaces.size} spaces.")
    
    return true

		# get the sql file

  end #run

end

# register the measure to be used by the application
RadianceMeasure.new.registerWithApplication
