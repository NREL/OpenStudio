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
    epout_dir = "./eplus_preprocess"
    if !File.exists?(epout_dir)
      FileUtils.mkdir_p(epout_dir)
    end

    # Radiance model dir
    rad_dir = "./radiance"
    if !File.exists?(rad_dir)
      FileUtils.mkdir_p(rad_dir)
    end

		
		# Radiance utility stuff 
		
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

		# END (Radiance utility stuff)
		

   	# save osm 	
    eplusin_file_path = OpenStudio::Path.new("eplusin.osm")
    model.save(eplusin_file_path,true)

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


		# make the Radiance model
		
		# make the run manager
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


		# copy Radiance model up
		# TODO be smarter about this. Is Mo
		FileUtils.copy_entry("#{epout_dir}/4-ModelToRad-0", rad_dir)

	
		# set up output dirs
		Dir.chdir("#{rad_dir}")
		FileUtils.mkdir_p("output/dc") unless File.exists?("output/dc")
		FileUtils.mkdir_p("output/ts") unless File.exists?("output/ts")
		FileUtils.mkdir_p("output/dc/merged_space/maps") unless File.exists?("output/dc/merged_space/maps")
 
# 		def calculateDaylightCoeffecients(rad_dir, t_options, t_space_names_to_calculate, t_radMaps, t_opts_map, t_simCores, t_catCommand)
# 
# 			haveWG0 = ""
# 
# 			puts "#{Time.now.getutc}: computing daylight coefficients with 3-phase method..."
# 
# 			mapFile=File.open("#{rad_dir}/numeric/merged_space.map","r")
# 			rfluxmtxDim = mapFile.readlines.size.to_s
# 			puts "total calculation points: #{rfluxmtxDim}"
# 
# 			# compute daylight matrices
# 
# 			system("oconv materials/materials.rad model.rad > model_dc.oct")
# 			windowMaps = File::open("#{rad_dir}/bsdf/mapping.rad")
# 			windowMaps.each do |row|
# 				next if row[0] == "#"
# 				wg=row.split(",")[0]
# 		
# 				
# 				if wg[2].to_i == 0	# uncontrolled windows
# 		
# 					# use more aggro simulation parameters because this is basically a view matrix
# 					rtrace_args = "#{t_options.vmx}"
# 			
# 					# make special WG0 octree (all shade control window groups blacked out)
# 					system("oconv \"#{rad_dir}/materials/materials.rad\" \"#{rad_dir}/materials/materials_WG0.rad\" model.rad \
# 						\"#{rad_dir}/skies/dc_sky.rad\" > model_WG0.oct")
# 
# 					# do daylight coefficients for uncontrolled windows
# 					puts "#{Time.now.getutc}: computing daylight/view matrix for window group #{wg}"
# 					system("#{t_catCommand} \"#{rad_dir}/numeric/merged_space.map\" \
# 						| rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{t_options.tregVars} -o \"#{rad_dir}/output/dc/WG0.vmx\" -m skyglow model_WG0.oct")
# 
# 				else 	# window group has shade control 
# 			
# 					# use more chill sim parameters
# 					rtrace_args = "#{t_options.dmx}"
# 			
# 					# checking...
# 					puts "#{Time.now.getutc}: computing daylight matrix for window group #{wg}"
# 			
# 					# do daylight matrices for controlled windows
# 					exec_statement("rfluxmtx -fa -v #{rad_dir}/scene/glazing/#{wg}.rad #{rad_dir}/skies/dc_sky.rad -i model_dc.oct > \
# 					#{rad_dir}/output/dc/#{wg}.dmx")
# 		
# 				end
# 				
# 			end  # calculate DMX
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
# 
# 		puts "#{Time.now.getutc}: done (daylight coefficients)."
# 
# 		end # def(calculateDaylightCoeffecients)


############ ^^^^^^^^ new ^^^^^^^^ #############


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
