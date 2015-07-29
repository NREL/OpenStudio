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
		# TODO deprecate
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
			perlExtension = ".pl"
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
  	FileUtils.mkdir_p("#{radPath}/wx") unless File.exists?("#{radPath}/wx")
 
		# copy Radiance model up
		# TODO be smarter about this.
		FileUtils.copy_entry("#{epout_dir}/4-ModelToRad-0", rad_dir)
		FileUtils.cp("#{epout_dir}/3-EnergyPlus-0/eplusout.sql", "#{rad_dir}/sql")

		
#		def calculateDaylightCoeffecients(rad_dir, t_options, t_space_names_to_calculate, t_radMaps, t_opts_map, t_simCores, t_catCommand)
		def calculateDaylightCoeffecients(radPath, t_sim_cores, t_catCommand)


		# Read simulation settings from model export 
		# TODO: read settings directly from model
		options_tregVars = "-e MF:1 -f tregenza.cal -b tbin -bn Ntbins" 			## TESTING (reset to empty)
		options_klemsDensity = ""
		options_skyvecDensity = "1"
		options_dmx = "-ab 1 -ad 128 -as 56 -dj 1 -dp 1 -dt 0.1 -dc 0.1 -lw 0.1 " 	## TESTING (reset to empty)
		options_vmx = "-ab 1 -ad 128 -as 56 -dj 1 -dp 1 -dt 0.1 -dc 0.1 -lw 0.1"		## TESTING (reset to empty)
		
		puts "first call: #{options_skyvecDensity}"


# UNCOMMENT WHEN DONE TESTING
# 			File.open("#{radPath}/options/treg.opt", "r") do |file|
# 				tempIO = file.read
# 				tempSettings = tempIO.split(" ")
# 				options_klemsDensity = "#{tempSettings[0]} #{tempSettings[1]}"
# 				options_skyvecDensity = tempSettings[3].split(":")[1]
# 				options_tregVars = tempSettings[2..-1].join(" ")
# 			end
# 
# 			File.open("#{radPath}/options/dmx.opt", "r") do |file|
# 				tempIO = file.read
# 				options_dmx = tempIO
# 			end
# 
# 			File.open("#{radPath}/options/vmx.opt", "r") do |file|
# 				tempIO = file.read
# 				options_vmx = tempIO
# 			end
# UNCOMMENT WHEN DONE TESTING

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
			mapFile=File.open("numeric/merged_space.map","r")
			rfluxmtxDim = mapFile.readlines.size.to_s

			puts "#{Time.now.getutc}: passing #{rfluxmtxDim} calculation points to Radiance"

			# compute daylight matrices

			puts "#{Time.now.getutc}: computing daylight coefficient matrices"

			system("oconv materials/materials.rad model.rad > model_dc.oct")
			windowMaps = File::open("bsdf/mapping.rad")
			windowMaps.each do |row|
				next if row[0] == "#"
				wg=row.split(",")[0]
				
				rad_command = ""
				
				if wg[2].to_i == 0	# uncontrolled windows
		
					# use more aggro simulation parameters because this is basically a view matrix
					rtrace_args = "#{options_vmx}"
			
					# make special WG0 octree (all shade controlled window groups blacked out)
					rad_command = "oconv materials/materials.rad materials/materials_WG0.rad model.rad skies/dc_sky.rad > model_WG0.oct"
					puts "#{Time.now.getutc}: #{rad_command}"
					system(rad_command)

					# do daylight coefficients for uncontrolled windows
					
					puts "#{Time.now.getutc}: computing daylight/view matrix for static window group (#{wg})..."
					
					#puts "procsUsed = #{procsUsed}"
					#puts "rtrace_args = #{rtrace_args}"
					#puts "options_tregVars = #{options_tregVars}"
					
					rad_command = "#{t_catCommand} numeric/merged_space.map | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} " + \
					"-o output/dc/WG0.vmx -m skyglow model_WG0.oct"
					puts "#{Time.now.getutc}: #{rad_command}"
					system(rad_command)

				else 	# window group has shade control 
			
					# use more chill sim parameters
					rtrace_args = "#{options_dmx}"
						
					# do daylight matrices for controlled windows
					puts "#{Time.now.getutc}: computing daylight matrix for window group #{wg}..."

					rad_command = "rfluxmtx #{rtrace_args} -fa -v scene/glazing/#{wg}.rad skies/dc_sky.rad -i model_dc.oct > output/dc/#{wg}.dmx"
					puts "#{Time.now.getutc}: #{rad_command}"
					system("rfluxmtx #{rtrace_args} -fa -v scene/shades/#{wg}_SHADE.rad skies/dc_sky.rad -i model_dc.oct > output/dc/#{wg}.dmx")
		
				end
				
 			end  # calculate DMX


			# compute view matrices for all controlled window groups

			# use fine params		
			rtrace_args = "#{options_vmx}" 

			puts "#{Time.now.getutc}: computing view matri(ces) for controlled windows"
			
			# get the shaded window groups' shade polygons
			
			wgInput = []
			# get the SHADE polygons for sampling (NOT the GLAZING ones!)
			Dir.glob("scene/shades/WG*.rad") {|file|
				wgInput << file
			}
	
			# make the receiver file
			exec_statement("#{t_catCommand} materials/materials_vmx.rad #{wgInput.join(" ")} > receivers_vmx.rad")
			
			# make the octree
			exec_statement("oconv materials/materials.rad scene/*.rad > model_vmx.oct")
			
			# make rfluxmtx do all the work
			rad_command = "rfluxmtx #{rtrace_args} -ds .15 -faa -y #{rfluxmtxDim} -I -v - receivers_vmx.rad -i model_vmx.oct < numeric/merged_space.map"
			exec_statement(rad_command)
	
			# compute daylight coefficient matrices for window group control points
			
			rtrace_args = "#{options_dmx}"
			system("oconv \"materials/materials.rad\" model.rad \
				\"skies/dc_sky.rad\" > model_wc.oct")
			puts "#{Time.now.getutc}: computing DCs for window control points"
			
			rad_command = "#{t_catCommand} numeric/window_controls.map | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} " + \
			"-o output/dc/window_controls.vmx -m skyglow model_wc.oct"
			system(rad_command)
			
			# end VMX

		puts "#{Time.now.getutc}: done (daylight coefficient matrices)."

		end # calculateDaylightCoeffecients()




		# annual simulation dealio
		def runSimulation(t_space_names_to_calculate, t_sqlFile, t_simCores, t_options_skyvecDensity, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_radPath, \
		t_spaceWidths, t_spaceHeights, t_radGlareSensorViews)

			puts "#{Time.now.getutc}: Calculating annual daylight values for all window groups and shade states"

			rawValues = Hash.new
			values = Hash.new
			dcVectors = Hash.new

			# i can haz gendaymtx vintage? (gendaymtx >= v4.2.b adds header and -h option to suppress) - 2014.07.02 RPG 
# 			genDaymtxHdr = ""
# 			exec_statement("gendaymtx -h -m #{t_options.skyvecDensity} \"#{t_outPath / OpenStudio::Path.new("in.wea")}\" > \"#{t_outPath / OpenStudio::Path.new("daymtx_out.tmp")}\" ")
# 			if File.zero?("#{t_outPath / OpenStudio::Path.new("daymtx_out.tmp")}")
# 				genDaymtxHdr = ""
# 				if t_options.z == true
# 					puts "Old Radiance version detected, will not work with 3-phase method, quitting."
# 					exit false
# 				end
# 			end
# 			File.delete("#{t_outPath / OpenStudio::Path.new("daymtx_out.tmp")}")
# 			# we now haz =)
			
			# Run the simulation 

			simulations = []

#			exec_statement("gendaymtx #{genDaymtxHdr} -m #{t_options_skyvecDensity} in.wea > annual-sky.mtx")
			exec_statement("gendaymtx -m #{t_options_skyvecDensity} wx/in.wea > annual-sky.mtx")


			windowMaps = File::open("bsdf/mapping.rad")
	
			# do annual sim for each window group and state
	
			windowMaps.each do |row|
				# skip header
				next if row[0] == "#"
				wg = row.split(",")[0]
		
				# do uncontrolled windows (WG0)
				if wg[2].to_i == 0
					# keep header, convert to illuminance, but no transpose
					exec_statement("dctimestep output/dc/#{wg}.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}.ill") 
		
				else
		
				# do all controlled window groups
		
					wgXMLs = row.split(",")[4..-1]
					if wgXMLs.size > 2
						puts "WARN: Window Group #{wg} has #{wgXMLs.size.to_s} BSDFs (2 max supported by OpenStudio application)."
					end
		
					wgXMLs.each_index do |i|
						exec_statement("dctimestep output/dc/#{wg}.vmx bsdf/#{wgXMLs[i].strip} output/dc/#{wg}.dmx \
						annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}_#{wgXMLs[i].split[0]}.ill")

					end
			
				end
		
			end
	
			# get annual values for window control sensors (note: convert to illuminance, no transpose, strip header)
			exec_statement("dctimestep #{t_radPath}/output/dc/window_controls.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - | getinfo - > #{t_radPath}/output/ts/window_controls.ill")

			# return the bsdf index for window group given by index at this hour
			# this is deprecated
			windowMapping = lambda { |index, hour| 
				data = dmx_mapping_data[index]
				# TODO remove this bit for shade controls
				return 0
			}
	
			puts "#{Time.now.getutc}: Merging results:"
	
			# do that window group/state merge thing
	
			windowGroups = File.open("#{t_radPath}/bsdf/mapping.rad")
			windowGroups.each do |wg|

				next if wg[0] == "#"
				windowGroup = wg.split(",")[0]
				next if windowGroup == "WG0"

				wgIllumFiles = Dir.glob("#{t_radPath}/output/ts/#{windowGroup}_*.ill")

				shadeControlSetpoint = wg.split(",")[3].to_f

				puts "#{Time.now.getutc}: Processing window group '#{windowGroup}', setpoint: #{shadeControlSetpoint} lux"  

				# separate header from data; so, so ugly. 
				header = []
				ill0 = []
				ill1 = []

				wgIllum_0 = File.open("#{wgIllumFiles[0]}").each_line do |line|
					if line.chomp! =~ /^\s?\d/
						ill0 << "#{line}\n"
					else 
						header << "#{line}\n"
					end

				end

				wgIllum_1 = File.open("#{wgIllumFiles[1]}").each_line do |line|
					if line.chomp! =~ /^\s?\d/
						ill1 << "#{line}\n"
					else 
						next
					end

				end

				# get the window control point illuminances (should be headerless file)
		
				windowControls = File.open("#{t_radPath}/output/ts/window_controls.ill", "r")

				windowControls.each do |row|
	
					data = row.split(" ")

					wgMerge = []
					wgShadeSched = []

					# simple, window illuminance-based shade control

					data.each_index do |i|

						if data[i].to_f < shadeControlSetpoint
							wgMerge << ill0[i]
							wgShadeSched << "0\n"
						else
							wgMerge << ill1[i]
							wgShadeSched << "1\n"
						end
	
					end


					# you need to file these files, yo.
			
					wgIllum = File.open("m_#{windowGroup}.ill", "w")
					wgShade = File.open("#{windowGroup}.shd", "w")
					header.each {|head| wgIllum.print "#{head}"}
					wgMerge.each {|ts| wgIllum.print "#{ts}"}
					wgShadeSched.each {|sh| wgShade.print "#{sh}"}
					wgIllum.close
					wgShade.close
					FileUtils.rm Dir.glob('*.tmp')

				end

			end

			# make whole-building illuminance file

			addFiles = ""

			# there may not be a WG0...
	
			if File.exist?('output/ts/WG0.ill')
				addFiles << "output/ts/WG0.ill "
			end

			# merge uncontrolled windows (WG0.ill) with blended controlled window groups (m_*.ill) 
	
			mergedWindows = Dir.glob("m_*.ill")

			mergedWindows.each do |file|
				addFiles << "+ #{file} "
			end
			system("rmtxop -fa #{addFiles} -t | getinfo - > output/ts/merged_space.ill")

			## window merge end


			rawValues = parseResults(simulations, windowMapping, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_radPath)

			dcVectors = nil

			# for each environment period (design days, annual, or arbitrary) you will create a directory for results
			t_sqlFile.availableEnvPeriods.each do |envPeriod|

				diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits = getTimeSeries(t_sqlFile, envPeriod)

				# check that we have all timeseries
				if (not diffHorizIllumAll) or (not dirNormIllumAll) or (not diffEfficacyAll) or (not dirNormEfficacyAll) or (not solarAltitudeAll) or (not solarAzimuthAll)
					puts "Missing required timeseries"
					exit false
				end

				simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime = \
				buildSimulationTimes(t_sqlFile, envPeriod, diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll)

				simTimes.each_index do |i|
					datetime = simDateTimes[i]
					hours = ((datetime.date().dayOfYear() - 1) * 24) + datetime.time().hours()
					values[i] = rawValues[hours]
				end

			end

			return values, dcVectors;
	
		end # runSimulation()


		# function renamed from execSimulation() to parseResults()
		def parseResults(t_cmds, t_mapping, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_radPath)

			puts "#{Time.now.getutc}: Executing simulation"
	
			allValues = []
			values = []

			# read illuminance values from file
			values = []
			valuesFile = File.open("#{t_radPath}/output/ts/merged_space.ill")
			valuesFile.each do |row|
				values << row.split(" ")
			end

			puts "#{Time.now.getutc}: writing output"

			allhours = []

			# write out illuminance to individual space/map files
			8760.times do |hour|
				index = 0;
				splitvalues = Hash.new

				t_space_names_to_calculate.each do |space_name|
					space_size = t_spaceWidths[space_name] * t_spaceHeights[space_name]
					space = []
					illum = []
					glaresensors = nil

					if values.size > 0
						subspace = values.slice(index, space_size)
						index = index + space_size

						space = []
						subspace.each do |subspacevalue|
							space << subspacevalue[hour];
						end

						if File.exists?("#{t_radPath}/numeric/#{space_name}.sns")        
							if index >= values.size
								puts "index is #{index} but values.size is only #{values.size}"
							elsif hour >= values[index].size
								puts "hour is #{hour} but values.size[index] is only #{values[index].size}"
							end
							illum = [values[index][hour]]
							index = index + 1
						end

						if File.exists?("#{t_radPath}/numeric/#{space_name}.glr") and t_radGlareSensorViews[space_name]
							glareinput = values.slice(index, t_radGlareSensorViews[space_name].size)

							glaresensors = []
							glareinput.each do |val|

								#Note, this replaced the call to rcalc with dgpSimplified, faster to do it 
								#in here instead of calling out to rcalc for the number of vectors we are looking at per space
								adjustedval = [(0.0000622*val[hour].to_f)+0.184, 0].max
								#          puts "Orig val: #{val} adjusted #{adjustedval}"
								glaresensors << adjustedval
							end

							index = index + t_radGlareSensorViews[space_name].size
						end
					else
						puts "INFO: simulation command generated no results, stuffing with 0's"
						space = Array.new(space_size, 0)

						if File.exists?("#{t_radPath}/numeric/#{space_name}.sns")        
							illum = Array.new(1, 0)
						end

						if File.exists?("#{t_radPath}/numeric/#{space_name}.glr") and t_radGlareSensorViews[space_name]
							glaresensors = Array.new(t_radGlareSensorViews[space_name].size, 0)
						end
					end

					splitvalues[space_name] = [space, illum, glaresensors]
				end

				allhours[hour] = splitvalues;
			end

			puts "Returning annual results"
			return allhours;
		
		end # parseResults()



		def getTimeSeries(t_sqlFile, t_envPeriod)
			diffHorizIllumAll = []; dirNormIllumAll = [];
			diffEfficacyAll = []; dirNormEfficacyAll = [];
			solarAltitudeAll = []; solarAzimuthAll = [];
			diffHorizUnits = nil; dirNormUnits = nil

			# get the solar data
			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Exterior Horizontal Sky Illuminance").each do |timeseries|
				diffHorizIllumAll = timeseries.values
				diffHorizUnits = timeseries.units if not diffHorizUnits
			end
			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Exterior Beam Normal Illuminance").each do |timeseries|
				dirNormIllumAll = timeseries.values
				dirNormUnits = timeseries.units if not dirNormUnits
			end
			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Sky Diffuse Solar Radiation Luminous Efficacy").each do |timeseries|
				diffEfficacyAll = timeseries.values
				diffEfficacyUnits = timeseries.units if not diffEfficacyUnits
			end
			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Beam Solar Radiation Luminous Efficacy").each do |timeseries|
				dirNormEfficacyAll = timeseries.values
				dirNormEfficacyUnits = timeseries.units if not dirNormEfficacyUnits
			end
			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Solar Altitude Angle").each do |timeseries|
				solarAltitudeAll = timeseries.values
				solarAltitudeUnits = timeseries.units if not solarAltitudeUnits
			end
			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Solar Azimuth Angle").each do |timeseries|
				solarAzimuthAll = timeseries.values
				solarAzimuthUnits = timeseries.units if not solarAzimuthUnits
			end

			return diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits

		end # getTimeSeries()

		def buildSimulationTimes(t_sqlFile, t_envPeriod, t_diffHorizIllumAll, t_dirNormIllumAll, t_diffEfficacyAll, t_dirNormEfficacyAll, t_solarAltitudeAll, t_solarAzimuthAll)

			# we want simulation at these indices only
			simDateTimes = OpenStudio::DateTimeVector.new();
			simTimes = []
			diffHorizIllum = []
			dirNormIllum = []
			diffEfficacy = []
			dirNormEfficacy = []
			solarAltitude = []
			solarAzimuth = []
			firstReportDateTime = nil

			t_sqlFile.timeSeries(t_envPeriod, "Hourly", "Site Exterior Horizontal Sky Illuminance").each do |timeseries|
				firstReportDateTime = timeseries.firstReportDateTime
				daysFromFirstReport = timeseries.daysFromFirstReport
				(0...daysFromFirstReport.size).each do |i|
					dateTime = firstReportDateTime + OpenStudio::Time.new(daysFromFirstReport[i]) #   - 0.5/24.0 subtract 1/2 hr to center of interval
					if (dateTime.time().seconds() == 59) 
						# rounding error, let's help
						dateTime = dateTime + OpenStudio::Time.new(0,0,0,1);
					end

					if (dateTime.time().seconds() == 1) 
						# rounding error, let's help
						dateTime = dateTime - OpenStudio::Time.new(0,0,0,1);
					end

# removing month/day select 
# 					simulate_this_time = true
# 					curMonth = "#{dateTime.date.monthOfYear.value}"
# 					if t_options.simMonth.nil?
# 						#puts "INFO: no months specified, simulating all months"
# 					else
# 						if not t_options.simMonth.include?(curMonth)
# 							simulate_this_time = false
# 						end
# 					end
# 					curDay = "#{dateTime.date.dayOfMonth}"
# 					if t_options.simDay.nil?
# 						#puts "INFO: no days specified, simulating all days"
# 					else
# 						if not t_options.simDay.include?(curDay)
# 							simulate_this_time = false
# 						end
# 					end
# 					if simulate_this_time
					simTimes << "#{dateTime.date.monthOfYear.value} #{dateTime.date.dayOfMonth} #{dateTime.time}"
					simDateTimes << dateTime
					diffHorizIllum << t_diffHorizIllumAll[i]
					dirNormIllum << t_dirNormIllumAll[i]
					diffEfficacy << t_diffEfficacyAll[i]
					dirNormEfficacy << t_dirNormEfficacyAll[i]
					solarAltitude << t_solarAltitudeAll[i]
					solarAzimuth << t_solarAzimuthAll[i]
#					end
				end
			end

			return simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime

		end


		# actually do the thing
		
		# settle in, it's gonna be a bumpy ride...
		Dir.chdir("#{radPath}")
		puts "\nRunning radiance in directory: '#{Dir.pwd}'"

		# get the epw file
		# TODO align with long-winded thread from 2015.07.28
		
		epw_path = nil
		if model.weatherFile.is_initialized
			epw_path = model.weatherFile.get.path
			if epw_path.is_initialized
				if File.exist?(epw_path.get.to_s)
					epw_path = epw_path.get
				else
					# If this is an always-run Measure, need to check a different path
					alt_weath_path = File.expand_path(File.join(File.dirname(__FILE__), "../../../resources"))
					alt_epw_path = File.expand_path(File.join(alt_weath_path, epw_path.get.to_s))
					if File.exist?(alt_epw_path)
						epw_path = OpenStudio::Path.new(alt_epw_path)
					else
						OpenStudio::logFree(OpenStudio::Error, "openstudio.model.Model", "Model has been assigned a weather file, but the file is not in the specified location of '#{epw_path.get}'.")
						return false
					end
				end
			else
				OpenStudio::logFree(OpenStudio::Error, "openstudio.model.Model", "Model has a weather file assigned, but the weather file path has been deleted.")
				return false
			end
		else
			OpenStudio::logFree(OpenStudio::Error, 'openstudio.model.Model', 'Model has not been assigned a weather file.')
			return false

	  end

		sqlPath = OpenStudio::Path.new("sql/eplusout.sql")
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

		sqlPath = OpenStudio::Path.new("sql/eplusout.sql")
		sqlPath = OpenStudio::system_complete(sqlPath)
										
		# reduce/convert epw data to Daysim-style ".wea" input format

		epw2weapath = OpenStudio::Path.new(ENV['EPW2WEAPATH'])

		puts "Executing epw2wea: #{epw2weapath}"
		exec_statement("\"#{epw2weapath.to_s}\" \"#{epw_path.to_s}\" \"wx/in.wea\"")
		

		# get site info from .wea file
		# TODO may want to get this from properly set OS.site object
		site = model.getSite
		
		site_name = site.name.to_s
		site_latitude = site.latitude
		site_longitude = site.longitude
		site_meridian = site.timeZone.to_f * 15

		puts "city: #{site_name}"
		puts "latitude: #{site_latitude}"
		puts "longitude: #{site_longitude}"
		puts "time zone: GMT #{site.timeZone}"
		puts "std meridian: #{site_meridian}"

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
		File.open("numeric/window_controls.map", "w") do |f|

			windows = Dir.glob("numeric/WG*.pts")
			windows.each do |wg|
				f.write IO.read(wg)
			end
		end

		# merge calculation points
		File.open("numeric/merged_space.map", "w") do |f|
			space_names_to_calculate.each do |space_name|

				f.write IO.read("numeric/#{space_name}.map")

				if File.exists?("numeric/#{space_name}.sns")        
					f.write IO.read("numeric/#{space_name}.sns")
				end

				if File.exists?("numeric/#{space_name}.glr")
					f.write IO.read("numeric/#{space_name}.glr")

				end
			end
		end

		# get the daylight coefficient matrices
		calculateDaylightCoeffecients(radPath, sim_cores, catCommand)

		# TODO remove this breakpoint!
		options_skyvecDensity = "1"
		puts "last call: #{options_skyvecDensity}"

		# calculate input for annualSimulation()
		values, dcVectors = runSimulation(space_names_to_calculate, sqlFile, sim_cores, options_skyvecDensity, site_latitude, site_longitude, site_meridian, radPath, spaceWidths, spaceHeights, radGlareSensorViews)
		
		# make merged building-wide illuminance schedule
  	#annualSimulation(sqlFile, options, epw_path, space_names_to_calculate, radMaps, spaceWidths, spaceHeights, radMapPoints, radGlareSensorViews, sim_cores, site_latitude, site_longitude, site_meridian, radPath, building, values, dcVectors)

		# execute MakeSchedules
		# result = exec_statement("ruby #{load_paths} '#{dirname}/MakeSchedules.rb' '#{modelPath}' '#{sqlPath}' --keep")
		# if not result
		#   puts "failed to run MakeSchedules"
		#   exit false
		# end
		# 
		# FileUtils.copy("in/model/radiance/out.osm", "out.osm");
		# FileUtils.copy("in/model/radiance/output/radout.sql", "radout.sql");
		# 
		# # execute DaylightMetrics
		# result = exec_statement("ruby #{load_paths} '#{dirname}/DaylightMetrics.rb' '#{modelPath}' '#{sqlPath}' radout.sql")
		# if not result
		#   puts "failed to run DaylightMetrics"
		#   exit false
		# end
		
		

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
