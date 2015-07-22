# see the URL below for information on how to write OpenStudio measures:
# http://nrel.github.io/OpenStudio-user-documentation/measures/measure_writing_guide/

# design document for this measure is at https://docs.google.com/document/d/16_TLRuhc4VFs2o0gRAp81hRObet7-s6fUEWo3HO7LpE/edit#

require_relative 'resources/RadiancePreprocess'

# require 'fileutils'
# require 'csv'
# require 'tempfile'
# require 'date'

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
    write_sql.setDefaultValue('Yes')
    write_sql.setDescription('Write Radiance results to a SqlFile format.')
    args << write_sql

    chs = OpenStudio::StringVector.new
    chs << 'Min'
    chs << 'Max'
    chs << 'Default'
    use_cores = OpenStudio::Ruleset::OSArgument.makeStringArgument('use_cores', true)
    use_cores.setDisplayName('Cores')
    use_cores.setDefaultValue('Default')
    use_cores.setDescription('Number of CPU cores to use for Radiance jobs. Default is to use all but one core for Radiance')
    args << use_cores


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

    use_cores = runner.getStringArgumentValue('use_cores', user_arguments)
    use_cores = (use_cores == 'Default')


    # Energyplus "pre-run" model dir
    epout_dir = "eplus_preprocess"
    if !File.exists?(epout_dir)
      FileUtils.mkdir_p(epout_dir)
    end

    # Radiance model dir
    rad_dir = "radiance"
    if !File.exists?(rad_dir)
      FileUtils.mkdir_p(rad_dir)
    end

		## Radiance Utilities 

		# print statement and execute as system call
		def exec_statement(s)
			if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
				s = s.gsub("/", "\\")
			end
			#puts "start '#{s}'"
			result = system(s)
			#puts "end '#{s}'"
			return result
		end

		# UNIX-style which 
		def which(cmd) 
			exts = ENV['PATHEXT'] ? ENV['PATHEXT'].split(';') : [''] 
			ENV['PATH'].split(File::PATH_SEPARATOR).each do |path| 
				exts.each do |ext| 
					exe = "#{path}/#{cmd}#{ext}" 
					return exe if File.executable? exe 
				end
			end 
			return nil 
		end 
		
		# make float.round() sorta work in ruby v1.8 like it does in v1.9, enough for our purposes
		# TODO deprecate since we are on R2.0
# 		class Numeric
# 			def round_to_str( decimals=0 )
# 				if decimals >= 0
# 					"%.#{decimals}f" % self
# 				else
# 					factor = 10**-decimals
# 					((self/factor).round * factor).to_s
# 				end
# 			end
# 		end

		# set up MP option
		coreCount = OpenStudio::System::numberOfProcessors
		sim_cores = "1"

		if use_cores == "Max"
			sim_cores = coreCount
		elsif use_cores == "Min"
			sim_cores = 1
		else
			sim_cores = coreCount - 1
		end
		# disable MP for Yosemite, because:
		# http://radiance-online.org/pipermail/radiance-dev/2014-November/001442.html
		if /darwin/.match(RUBY_PLATFORM)
			ver = `defaults read loginwindow SystemVersionStampAsString`
			if ver.split(".")[1] == '10'
				puts "Radiance multiprocessing disabled for MacOS 10.10.x, sorry."
				sim_cores = 1
			end
		end
		puts "Using #{sim_cores} cores for Radiance jobs"

		# help those poor Windows users out
		perlExtension = ""
		catCommand = "cat"
		osQuote = "\'"
		if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
			catCommand = "type"
			osQuote = "\""
		end

		## END Radiance Utilities
		

		# setup path to Radiance binaries
		co = OpenStudio::Runmanager::ConfigOptions.new(true);
		co.fastFindRadiance();
		radiancePath = co.getTools().getLastByName("rad").localBinPath.parent_path

		path = OpenStudio::Path.new(radiancePath).to_s
		raypath = (OpenStudio::Path.new(radiancePath).parent_path() / 
		OpenStudio::Path.new('lib')).to_s()

		epw2weapath = (OpenStudio::Path.new(radiancePath) / OpenStudio::Path.new('epw2wea')).to_s


		ENV["EPW2WEAPATH"] = epw2weapath
		programExtension = ''
		if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
			programExtension = ".exe"
			perlpath = ""
			if OpenStudio::applicationIsRunningFromBuildDirectory()
				perlpath = OpenStudio::getApplicationRunDirectory().parent_path().parent_path() / 
				OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
			else
				perlpath = OpenStudio::getApplicationRunDirectory().parent_path() / 
				OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
			end
			puts "Adding path for local perl: " + perlpath.to_s
			ENV["PATH"] = ENV["PATH"] + ";" + path + ";" + perlpath.to_s
			ENV["RAYPATH"] = path + ";" + raypath + ";."
		else
			ENV["PATH"] = ENV["PATH"] + ":" + path
			ENV["RAYPATH"] = path + ":" + raypath + ":."
		end

		if Dir.glob(epw2weapath + programExtension).empty?
			puts "(test) Cannot find epw2wea tool in radiance installation at '#{radiancePath}'. \
			You may need to install a newer version of Radiance."
			exit false
		end
		ENV["EPW2WEAPATH"] = epw2weapath

		if !which('perl')
			puts "Perl could not be found in path, exiting"
			exit false
		end

		
		## ModelToRad Workflow

   	# save osm for input to eplus pre-process
    modelPath = OpenStudio::Path.new("eplusin.osm")
    model.save(modelPath,true)

		# find EnergyPlus
		co = OpenStudio::Runmanager::ConfigOptions.new 
		co.fastFindEnergyPlus
 
		# make a workflow (EnergyPlus "pre-run" to get constructions and weather)
		workflow = OpenStudio::Runmanager::Workflow.new("ModelToRadPreprocess->ModelToIdf->ExpandObjects->EnergyPlus")
		workflow.add(co.getTools)

		# add model-to-rad workflow
		modelToRad = OpenStudio::Runmanager::Workflow.new("ModelToRad")
		workflow.addWorkflow(modelToRad)

		# minimize file path lengths
		workflow.addParam(OpenStudio::Runmanager::JobParam.new("flatoutdir"))
		
		# make the run manager
		runDir = OpenStudio::Path.new(epout_dir)
		runmanager_path = OpenStudio::Path.new("runmanager.db")
		runmanager = OpenStudio::Runmanager::RunManager.new(runmanager_path, true, true, false, false)

		OpenStudio::makeParentFolder(runDir, OpenStudio::Path.new(), true)
		puts "Creating workflow"
		
		jobtree = workflow.create(OpenStudio::system_complete(runDir), OpenStudio::system_complete(modelPath), OpenStudio::Path.new(runner.lastEpwFilePath.get))
		runmanager.enqueue(jobtree, true)
		#runmanager.getJobs.each {|job| job.setBasePath(resourcePath)} # DLM: need to be able to get this from runner
		puts "Running jobs in #{runDir}"
		runmanager.setPaused(false)
		runmanager.waitForFinished()


		##  Radiance crap

		modelPath = OpenStudio::system_complete(modelPath)
		radPath = modelPath.parent_path / OpenStudio::Path.new("radiance")
		
		# set up output dirs
		FileUtils.mkdir_p("#{radPath}/output/dc") unless File.exists?("#{radPath}/output/dc")
		FileUtils.mkdir_p("#{radPath}/output/ts") unless File.exists?("#{radPath}/output/ts")
		FileUtils.mkdir_p("#{radPath}/output/dc/merged_space/maps") unless File.exists?("#{radPath}/output/dc/merged_space/maps")
 		FileUtils.mkdir_p("#{radPath}/sql") unless File.exists?("#{radPath}/sql")
 
		# copy Radiance model up
		# TODO be smarter about this.
		FileUtils.copy_entry("#{epout_dir}/4-ModelToRad-0", rad_dir)
		FileUtils.cp("#{epout_dir}/3-EnergyPlus-0/eplusout.sql", "#{rad_dir}/sql")

		
#		def calculateDaylightCoeffecients(rad_dir, t_options, t_space_names_to_calculate, t_radMaps, t_opts_map, t_simCores, t_catCommand)
		def calculateDaylightCoeffecients(radPath, t_sim_cores, t_catCommand)


			# Read simulation settings from model export 
			# TODO: read settings directly from model
			options_tregVars = ""
			options_klemsDensity = ""
			options_skyvecDensity = ""
			options_dmx = ""
			options_vmx = ""

			File.open("#{radPath}/options/treg.opt", "r") do |file|
				tempIO = file.read
				tempSettings = tempIO.split(" ")
				options_klemsDensity = "#{tempSettings[0]} #{tempSettings[1]}"
				options_skyvecDensity = tempSettings[3].split(":")[1]
				options_tregVars = tempSettings[2..-1].join(" ")
			end

			File.open("#{radPath}/options/dmx.opt", "r") do |file|
				tempIO = file.read
				options_dmx = tempIO
			end

			File.open("#{radPath}/options/vmx.opt", "r") do |file|
				tempIO = file.read
				options_vmx = tempIO
			end

			## END read simulation settings

			# configure multiprocessing 
			procsUsed = ""
			if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
				puts "Radiance does not support multiple cores on Windows"
				procsUsed = ""
			else
				puts "Radiance using #{t_sim_cores} core(s)"
				procsUsed = "-n #{t_sim_cores}"
			end

			haveWG0 = ""

			# get calculation points array size (needed for rmtxop later)
			mapFile=File.open("#{radPath}/numeric/merged_space.map","r")
			rfluxmtxDim = mapFile.readlines.size.to_s

			puts "#{Time.now.getutc}: passing #{rfluxmtxDim} calculation points to Radiance"

			# compute daylight matrices

			puts "#{Time.now.getutc}: computing daylight coefficient matrices"

			system("oconv materials/materials.rad model.rad > model_dc.oct")
			windowMaps = File::open("#{radPath}/bsdf/mapping.rad")
			windowMaps.each do |row|
				next if row[0] == "#"
				wg=row.split(",")[0]
				
				if wg[2].to_i == 0	# uncontrolled windows
		
					# use more aggro simulation parameters because this is basically a view matrix
					rtrace_args = "#{options_vmx}"
			
					# make special WG0 octree (all shade control window groups blacked out)
					exec_statement("oconv \"#{radPath}/materials/materials.rad\" \"#{radPath}/materials/materials_WG0.rad\" model.rad \
						\"#{radPath}/skies/dc_sky.rad\" > model_WG0.oct")

					# do daylight coefficients for uncontrolled windows
					puts "#{Time.now.getutc}: computing daylight/view matrix for window group #{wg}"
					exec_statement("#{t_catCommand} \"#{radPath}/numeric/merged_space.map\" \
						| rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} -o \"#{radPath}/output/dc/WG0.vmx\" -m skyglow model_WG0.oct")

				else 	# window group has shade control 
			
					# use more chill sim parameters
					rtrace_args = "#{options_dmx}"
						
					# do daylight matrices for controlled windows
					puts "#{Time.now.getutc}: computing daylight matrix for window group #{wg}"

					exec_statement("rfluxmtx #{rtrace_args} #{procsUsed} -fa -v #{radPath}/scene/glazing/#{wg}.rad #{radPath}/skies/dc_sky.rad -i model_dc.oct > \
					#{radPath}/output/dc/#{wg}.dmx")
		
				end
				
 			end  # calculate DMX
# 
# 
# 			# compute view matrices for all controlled window groups
# 
# 			# use fine params
# 			
# 			rtrace_args = "#{t_options.vmx}" 
# 
# 			# get the window groups, skipping WG0 if present
# 			
# 			wgInput = []
# 			Dir.glob("#{rad_dir}/scene/glazing/WG*.rad") {|file|
# 				next if file == "#{rad_dir}/scene/glazing/WG0.rad"
# 				wgInput << file
# 			}
# 	
# 			puts "#{Time.now.getutc}: computing view matri(ces) for controlled windows"
# 			exec_statement("#{t_catCommand} #{rad_dir}/materials/materials_vmx.rad #{wgInput.join(" ")} > receivers_vmx.rad")
# 			exec_statement("oconv #{rad_dir}/materials/materials.rad #{rad_dir}/scene/*.rad > model_vmx.oct")
# 			exec_statement("rfluxmtx #{rtrace_args} -ds .15 #{procsUsed} -faa -n #{t_simCores} -y #{rfluxmtxDim} -I -v - receivers_vmx.rad -i model_vmx.oct < \
# 				#{rad_dir}/numeric/merged_space.map")
# 	
# 			# compute daylight coefficient matrices for window group control points
# 			
# 			rtrace_args = "#{t_options.dmx}"
# 			system("oconv \"#{rad_dir}/materials/materials.rad\" model.rad \
# 				\"#{rad_dir}/skies/dc_sky.rad\" > model_wc.oct")
# 			puts "#{Time.now.getutc}: computing DCs for window control points"
# 			exec_statement("#{t_catCommand} \"#{rad_dir}/numeric/window_controls.map\" \
# 				| rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{t_options.tregVars} \
# 				-o \"#{rad_dir}/output/dc/window_controls.vmx\" \
# 				-m skyglow model_wc.oct")
# 			
# 			# end VMX

		puts "#{Time.now.getutc}: done (daylight coefficients)."

		end # def(calculateDaylightCoeffecients)


		# actually do the thing
		
		# settle in, it's gonna be a bumpy ride...
		Dir.chdir("#{radPath}")
		puts "\nRunning radiance in directory: '#{Dir.pwd}'"

		sqlPath = OpenStudio::Path.new("#{radPath}/sql/eplusout.sql")
		sqlPath = OpenStudio::system_complete(sqlPath)
		
		# load the sql file
		sqlFile = OpenStudio::SqlFile.new(sqlPath)
		if not sqlFile.connectionOpen
			puts "SqlFile #{sqlPath} connection is not open"
			return false
		end	

		# set the sql file
		model.setSqlFile(sqlFile)
		if model.sqlFile.empty?
			puts "Model's SqlFile is not initialized"
			return false
		end

		# get the top level simulation object
		simulation = model.getSimulationControl

		# get site information
		site = model.getSite()
		weatherFile = site.weatherFile();
				
		# settle in, it's gonna be a bumpy ride...
		Dir.chdir("#{radPath}")

		sqlPath = OpenStudio::Path.new("#{radPath}/sql/eplusout.sql")
		sqlPath = OpenStudio::system_complete(sqlPath)
		
		# load the sql file
		sqlFile = OpenStudio::SqlFile.new(sqlPath)
		if not sqlFile.connectionOpen
			puts "SqlFile #{sqlPath} connection is not open"
			return false
		end	

		# set the sql file
		model.setSqlFile(sqlFile)
		if model.sqlFile.empty?
			puts "Model's SqlFile is not initialized"
			return false
		end

		# get the top level simulation object
		simulation = model.getSimulationControl

		# get site information
		site = model.getSite()
		weatherFile = site.weatherFile();
				
		
	# wait for Dan \/ (where's the epw file??)
	# #
	# 		puts "Getting weather file"
	# 		epwFile = nil
	# 		if (!weatherFile.empty?)
	# 			puts "Weather file is not empty"
	# 
	# 			p = weatherFile.get().path()
	# 			if (!p.empty?)
	# 				puts "Path to weather file is: " + p.get().to_s + " using osmpath: " + osmPath.to_s
	# 			end
	# 
	# 			epwFile = weatherFile.get().file(osmPath)
	# 
	# 			if (!epwFile.empty?)
	# 				puts "epwFile is not empty"
	# 			end
	# 		else
	# 			puts "weather file object is empty"
	# 		end
	# 
	# 		if (weatherFile.empty? || epwFile.empty? || !File.exists?(epwFile.get.to_s))
	# 			puts "EPW From model not found"
	# 			possibleEpw = modelPath.parent_path() / OpenStudio::Path.new("in.epw");
	# 
	# 			if (File.exists?(possibleEpw.to_s))
	# 				puts "EPW not found, but found one here: " + possibleEpw.to_s
	# 				epwFile = OpenStudio::OptionalEpwFile.new(OpenStudio::EpwFile.new(possibleEpw))
	# 			end
	# 		end
	# 
	# 		weaPath = nil
	# 		smxPath = nil
	#
	# 		reduce/convert epw data to Daysim-style ".wea" input format
	# 		if (!epwFile.empty?)
	# 			epwFilePath = epwFile.get().path()
	# 			weaPath = outPath / OpenStudio::Path.new("in.wea")
	# 
	# 			epw2weapath = OpenStudio::Path.new(ENV['EPW2WEAPATH'])
	# 
	# 			puts "Executing epw2wea: #{epw2weapath}"
	# 			exec_statement("\"#{epw2weapath.to_s}\" \"#{epwFilePath.to_s}\" \"#{weaPath.to_s}\"")
	# 		end
	# 
	# 		site_name = site.getString(1, true).get
	# 		site_latitude = site.getString(2, true).get
	# 		site_longitude = site.getString(3, true).get
	# 		site_timezone = site.getString(4, true).get
	# 		site_elevation = site.getString(5, true).get
	# 
	# 		site_stdmeridian = site_timezone.to_f * 15
	# 
	# 		puts "city: #{site_name}"
	# 		puts "latitude: #{site_latitude}"
	# 		puts "longitude: #{site_longitude}"
	# 		puts "elevation: #{site_elevation} meters"
	# 		puts "timezone: GMT #{site_timezone}"

	#  wait for Dan /\

		# get the facility and building
		facility = model.getFacility
		building = model.getBuilding
		building_transformation = building.transformation

		# create materials library for model, shared for all spaces
		radMaterials = Array.new

		# create space geometry, hash of space name to file contents
		radSpaces = Hash.new
		radSensors = Hash.new
		radGlareSensors = Hash.new
		radGlareSensorViews = Hash.new
		radMaps = Hash.new
		radMapHandles = Hash.new
		radMapPoints = Hash.new
		radViewPoints = Hash.new
		radDaylightingControls = Hash.new
		radDaylightingControlPoints = Hash.new
		spaceWidths = Hash.new
		spaceHeights = Hash.new

		# loop through the model
		space_names = Array.new

		building.spaces.each do |space|
			space_name = space.name.get.gsub(' ', '_').gsub(':', '_')
			space_names << space_name

			space_transformation = space.transformation

			# get output illuminance map points
			space.illuminanceMaps.each do |map|
				radMaps[space_name] = ""
				radMapHandles[space_name] = map.handle
				radMapPoints[space_name] = OpenStudio::Radiance::RadianceForwardTranslator::getReferencePoints(map)
				spaceWidths[space_name] = map.numberofXGridPoints
				spaceHeights[space_name] = map.numberofYGridPoints
			end

			# get daylighting control points
			space.daylightingControls.each do |control|
				radDaylightingControls[space_name] = ""
				radDaylightingControlPoints[space_name] = OpenStudio::Radiance::RadianceForwardTranslator::getReferencePoint(control)
			end

			# get glare sensors
			space.glareSensors.each do |sensor|
				radGlareSensors[space_name] = ""
				radGlareSensorViews[space_name] = OpenStudio::Radiance::RadianceForwardTranslator::getViewVectors(sensor)
			end
		end

		space_names_to_calculate = Array.new


		# only do spaces with illuminance maps
		space_names_to_calculate = Array.new
		space_names.each do |space_name|
			if not radMaps[space_name].nil?
				space_names_to_calculate << space_name
			end
		end

		# merge window group control points
		File.open("#{radPath}/numeric/window_controls.map", "w") do |f|

			windows = Dir.glob("#{radPath}/numeric/WG*.pts")
			windows.each do |wg|
				f.write IO.read(wg)
			end
		end

		# merge calculation points
		File.open("#{radPath}/numeric/merged_space.map", "w") do |f|
			space_names_to_calculate.each do |space_name|

				f.write IO.read("#{radPath}/numeric/#{space_name}.map")

				if File.exists?("#{radPath}/numeric/#{space_name}.sns")        
					f.write IO.read("#{radPath}/numeric/#{space_name}.sns")
				end

				if File.exists?("#{radPath}/numeric/#{space_name}.glr")
					f.write IO.read("#{radPath}/numeric/#{space_name}.glr")

				end
			end
		end

		calculateDaylightCoeffecients(radPath, sim_cores, catCommand)

		#values, dcVectors = runSimulation(space_names_to_calculate, sqlFile, options, simCores, site_latitude, site_longitude, site_stdmeridian, outPath, spaceWidths, spaceHeights, radGlareSensorViews)
  	#annualSimulation(sqlFile, options, epwFile, space_names_to_calculate, radMaps, spaceWidths, spaceHeights, radMapPoints, radGlareSensorViews, simCores, site_latitude, site_longitude, site_stdmeridian, outPath, building, values, dcVectors)


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

  end #run

end

# register the measure to be used by the application
RadianceMeasure.new.registerWithApplication
