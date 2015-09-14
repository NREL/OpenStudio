# see the URL below for information on how to write OpenStudio measures:
# http://nrel.github.io/OpenStudio-user-documentation/measures/measure_writing_guide/

# design document for this measure is at https://docs.google.com/document/d/16_TLRuhc4VFs2o0gRAp81hRObet7-s6fUEWo3HO7LpE/edit#

require 'fileutils'
require 'csv'
require 'tempfile'
require 'date'

# start the measure
class RadianceMeasure < OpenStudio::Ruleset::ModelUserScript

  # human readable name
  def name
    return "Radiance Daylighting Measure"
  end

  # human readable description
  def description
    return "This measure uses Radiance instead of EnergyPlus for daylighting calculations with OpenStudio."
  end

  # human readable description of modeling approach
  def modeler_description
    return "The OpenStudio model is converted to Radiance format. All spaces containing daylighting objects (illuminance map, daylighting control point, and optionally glare sensors) will have annual illuminance calculated using Radiance, and the OS mole's lighting schedules can be overwritten with those based on daylight responsive lighting controls."
  end

  # define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    chs = OpenStudio::StringVector.new
    chs << 'Yes'
    chs << 'No'
    apply_schedules = OpenStudio::Ruleset::OSArgument::makeChoiceArgument('apply_schedules', chs, true)
    apply_schedules.setDisplayName('Apply schedules')
    apply_schedules.setDefaultValue('Yes')
    apply_schedules.setDescription('Replace lighting and shading control schedules with schedules computed by Radiance')
    args << apply_schedules
    
    chs = OpenStudio::StringVector.new
    chs << 'Yes'
    chs << 'No'
    write_sql = OpenStudio::Ruleset::OSArgument.makeChoiceArgument('write_sql', chs, true)
    write_sql.setDisplayName('Write Radiance SqlFile')
    write_sql.setDefaultValue('Yes')
    write_sql.setDescription('Write Radiance results to a SqlFile format')
    args << write_sql

    chs = OpenStudio::StringVector.new
    chs << 'Default'
    chs << 'Min'
    chs << 'Max'
    use_cores = OpenStudio::Ruleset::OSArgument.makeChoiceArgument('use_cores', chs, true)
    use_cores.setDisplayName('Cores')
    use_cores.setDefaultValue('Default')
    use_cores.setDescription('Number of CPU cores to use for Radiance jobs. Default is to use all but one core')
    args << use_cores

    chs = OpenStudio::StringVector.new
    chs << 'Model'
    chs << 'Testing'
    rad_settings = OpenStudio::Ruleset::OSArgument.makeChoiceArgument('rad_settings', chs, true)
    rad_settings.setDisplayName('Radiance Settings')
    rad_settings.setDefaultValue('Model')
    rad_settings.setDescription('Radiance simulation parameters. Default is to get these from the model; the Testing option is for testing the Radiance workflow only, as it uses very crude parameters for a fast simulation but very inaccurate results.')
    args << rad_settings


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
    
    write_sql = runner.getStringArgumentValue('write_sql', user_arguments)

    if apply_schedules == "Yes" && write_sql == "No"
      puts "lighting power schedules requested, will write Radiance sql results file"
      write_sql = "Yes"
    end

    use_cores = runner.getStringArgumentValue('use_cores', user_arguments)

    rad_settings = runner.getStringArgumentValue('rad_settings', user_arguments)


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
      puts "Executing: #{s}"
      result = system(s)
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
    #ENV["EPW2WEAPATH"] = epw2weapath + programExtension

    if !which('perl')
      puts "Perl could not be found in path, exiting"
      exit false
    end
    
    # get the epw file
    # TODO align with long-winded thread from 2015.07.28
    
    epw_path = nil
    
    # try runner first
    if runner.lastEpwFilePath.is_initialized
      test = runner.lastEpwFilePath.get.to_s
      if File.exist?(test)
        epw_path = test
      end
    end
        
    # try model second
    if !epw_path
      if model.weatherFile.is_initialized
        test = model.weatherFile.get.path
        if test.is_initialized
          # have a file name from the model
          if File.exist?(test.get.to_s)
            epw_path = test.get
          else
            # If this is an always-run Measure, need to check for file in different path
            alt_weath_path = File.expand_path(File.join(File.dirname(__FILE__), "../../../resources"))
            alt_epw_path = File.expand_path(File.join(alt_weath_path, test.get.to_s))
            if File.exist?(alt_epw_path)
              epw_path = OpenStudio::Path.new(alt_epw_path)
            else
              runner.registerError("Model has been assigned a weather file, but the file is not in the specified location of '#{test.get}'.")
              return false
            end
          end
        else
          runner.registerError("Model has a weather file assigned, but the weather file path has been deleted.")
          return false
        end
      else
        runner.registerError('Model has not been assigned a weather file.')
        return false
      end
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
    
    # remove the E+ run dir so we don't confuse users 
    FileUtils.rm_rf(epout_dir)

    # Set Radiance simulation settings
    # TODO: read settings directly from model
    
    options_tregVars = ""
    options_dmx = ""
    options_vmx = ""
    if rad_settings == "Testing"
      options_tregVars = "-e MF:1 -f tregenza.cal -b tbin -bn Ntbins"
      options_dmx = "-ab 1 -ad 128 -as 56 -dj 1 -dp 1 -dt 0.1 -dc 0.1 -lw 0.1 " 
      options_vmx = "-ab 1 -ad 128 -as 56 -dj 1 -dp 1 -dt 0.1 -dc 0.1 -lw 0.1"
    end
    options_klemsDensity = ""
    options_skyvecDensity = "1"

    if rad_settings == "Model"
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

    end

    # configure multiprocessing 
    procsUsed = ""
    if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
      puts "Radiance does not support multiple cores on Windows"
      procsUsed = ""
    else
      puts "Radiance using #{sim_cores} core(s)"
      procsUsed = "-n #{sim_cores}"
    end



    # core functions

    def calculateDaylightCoeffecients(radPath, sim_cores, t_catCommand, options_tregVars, options_klemsDensity, options_skyvecDensity, options_dmx, options_vmx, rad_settings, procsUsed)



      haveWG0 = ""

      # get calculation points array size (needed for rmtxop later)
      mapFile=File.open("numeric/merged_space.map","r")
      rfluxmtxDim = mapFile.readlines.size.to_s

      puts "#{Time.now.getutc}: passing #{rfluxmtxDim} calculation points to Radiance"

      # compute daylight matrices

      puts "#{Time.now.getutc}: computing daylight coefficient matrices"

      exec_statement("oconv materials/materials.rad model.rad > model_dc.oct")
      windowMaps = File::open("bsdf/mapping.rad")
      windowMaps.each do |row|
        next if row[0] == "#"
        wg=row.split(",")[0]
        
        rad_command = ""
                
        if row.split(",")[2] == 'n/a' || row.split(",")[2] == 'AlwaysOff' # uncontrolled windows
        
          # use more aggro simulation parameters because this is basically a view matrix
          rtrace_args = "#{options_vmx}"
      
          # make special WG0 octree (all shade controlled window groups blacked out)  
          rad_command = "oconv \"materials/materials.rad\" \"materials/materials_WG0.rad\" model.rad \"skies/dc_sky.rad\" > model_WG0.oct"
          puts "#{Time.now.getutc}: #{rad_command}"
          exec_statement(rad_command)

          # do daylight coefficients for uncontrolled windows
          
          puts "#{Time.now.getutc}: computing daylight/view matrix for static window group (#{wg})..."
                    
          rad_command = "#{t_catCommand} \"numeric/merged_space.map\" | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} " + \
          "-o \"output/dc/WG0.vmx\" -m skyglow model_WG0.oct"
          puts "#{Time.now.getutc}: #{rad_command}"
          exec_statement(rad_command)

        else  # window group has shade control 
    
          # use more chill sim parameters
          rtrace_args = "#{options_dmx}"
            
          # do daylight matrices for controlled windows
          puts "#{Time.now.getutc}: computing daylight matrix for window group #{wg}..."

          rad_command = "rfluxmtx #{rtrace_args} -n #{sim_cores} -fa -v \"scene/shades/#{wg}_SHADE.rad\" \"skies/dc_sky.rad\" -i model_dc.oct > \"output/dc/#{wg}.dmx\""
          puts "#{Time.now.getutc}: #{rad_command}"
          exec_statement(rad_command)
    
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
      exec_statement("#{t_catCommand} \"materials/materials_vmx.rad\" #{wgInput.join(" ")} > receivers_vmx.rad")
      
      # make the octree
      scene_files = []
      Dir.glob("scene/*.rad").each {|f| scene_files << f}
      exec_statement("oconv materials/materials.rad #{scene_files.join(' ')} > model_vmx.oct")
      
      # make rfluxmtx do all the work
      rad_command = "rfluxmtx #{rtrace_args} -n #{sim_cores} -ds .15 -faa -y #{rfluxmtxDim} -I -v - receivers_vmx.rad -i model_vmx.oct < numeric/merged_space.map"
      exec_statement(rad_command)
  
      # compute daylight coefficient matrices for window group control points
      
      rtrace_args = "#{options_dmx}"
      exec_statement("oconv \"materials/materials.rad\" model.rad \
        \"skies/dc_sky.rad\" > model_wc.oct")
      puts "#{Time.now.getutc}: computing DCs for window control points"
      
      rad_command = "#{t_catCommand} \"numeric/window_controls.map\" | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} " + \
      "-o \"output/dc/window_controls.vmx\" -m skyglow model_wc.oct"
      exec_statement(rad_command)
      
      # end VMX

    puts "#{Time.now.getutc}: done (daylight coefficient matrices)."

    end # calculateDaylightCoeffecients()



    # annual simulation dealio
    def runSimulation(t_space_names_to_calculate, t_sqlFile, t_simCores, t_options_skyvecDensity, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_radPath, \
    t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, runner, write_sql)

      puts "#{Time.now.getutc}: Calculating annual daylight values for all window groups and shade states"

      rawValues = Hash.new
      values = Hash.new
      dcVectors = Hash.new

              
      
      # Run the simulation 

      simulations = []

      exec_statement("gendaymtx -m #{t_options_skyvecDensity} \"wx/in.wea\" > annual-sky.mtx")

      windowMaps = File::open("bsdf/mapping.rad")
  
      # do annual sim for each window group and state
  
      windowMaps.each do |row|
        # skip header
        next if row[0] == "#"
        wg = row.split(",")[0]
    
        # do uncontrolled windows (WG0)
        if row.split(",")[2] == "n/a" || row.split(",")[2] == "AlwaysOff"
          # keep header, convert to illuminance, but no transpose
          exec_statement("dctimestep \"#{t_radPath}/output/dc/#{wg}.vmx\" annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > \"#{t_radPath}/output/ts/#{wg}.ill\"") 
    
        else
    
        # do all controlled window groups
    
          wgXMLs = row.split(",")[4..-1]
          if wgXMLs.size > 2
            puts "WARN: Window Group #{wg} has #{wgXMLs.size.to_s} BSDFs (2 max supported by OpenStudio application)."
          end
    
          wgXMLs.each_index do |i|
            rad_command = "dctimestep output/dc/#{wg}.vmx bsdf/#{wgXMLs[i].strip} output/dc/#{wg}.dmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}_#{wgXMLs[i].split[0]}.ill"
            runner.registerInfo("#{Time.now.getutc}: #{rad_command}")
            exec_statement(rad_command)
          end
      
        end
    
      end
  
      # get annual values for window control sensors (note: convert to illuminance, no transpose, strip header)
      exec_statement("dctimestep output/dc/window_controls.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - | getinfo - > output/ts/window_controls.ill")
  
      puts "#{Time.now.getutc}: Merging results:"
  
      # do that window group/state merge thing
  
      windowGroups = File.open("bsdf/mapping.rad")
      windowGroups.each do |wg|

        next if wg[0] == "#"
        windowGroup = wg.split(",")[0]
        next if windowGroup == "WG0"

        wgIllumFiles = Dir.glob("output/ts/#{windowGroup}_*.ill")

        # possible values are "AlwaysOn", "AlwaysOff", "OnIfScheduleAllows", "OnIfHighSolarOnWindow"
        shadeControlType = wg.split(",")[2].to_s

        # DLM: we are not allowing scheduled setpoints for now, but when we do this would have to change

        # RPG: setpoint is coming from OS control, in watts, so...
        shadeControlSetpointWatts = wg.split(",")[3].to_f
      
        shadeControlSetpoint = shadeControlSetpointWatts * 179 # Radiance's luminous efficacy factor
                
        # DLM: hacktastic way to implement these options for now
        if shadeControlType == "AlwaysOn"
          shadeControlSetpoint = -1000
        elsif 
          shadeControlType == "AlwaysOff"
          shadeControlSetpoint = 100000000
        end

        
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
    
        windowControls = File.open("output/ts/window_controls.ill", "r")

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
      exec_statement("rmtxop -fa #{addFiles} -t | getinfo - > \"output/ts/merged_space.ill\"")

      

      ## window merge end


      rawValues = parseResults(simulations, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_radPath, write_sql)

      dcVectors = nil

      # for each environment period (design days, annual, or arbitrary) you will create a directory for results
      t_sqlFile.availableEnvPeriods.each do |envPeriod|

        # DLM: all of these might be available directly from the EpwFile after Jason DeGraw's work
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
    def parseResults(t_cmds, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_radPath, write_sql)

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
            puts "WARN: an error has occurred; no results for space '#{space_name}'."
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

          simTimes << "#{dateTime.date.monthOfYear.value} #{dateTime.date.dayOfMonth} #{dateTime.time}"
          simDateTimes << dateTime
          diffHorizIllum << t_diffHorizIllumAll[i]
          dirNormIllum << t_dirNormIllumAll[i]
          diffEfficacy << t_diffEfficacyAll[i]
          dirNormEfficacy << t_dirNormEfficacyAll[i]
          solarAltitude << t_solarAltitudeAll[i]
          solarAzimuth << t_solarAzimuthAll[i]
        end
      end

      return simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime

    end # buildSimulationTimes()



    def writeTimeSeriesToSql(sqlfile, simDateTimes, illum, space_name, ts_name, ts_units)
      data = OpenStudio::Vector.new(illum.length)
      illum.length.times do |n|
        begin
          data[n] = illum[n].to_f;
        rescue Exception => e
          puts "Error inserting data: " + illum[n] + " inserting 0 instead";
          data[n] = 0;
        end
      end

      illumTS = OpenStudio::TimeSeries.new(simDateTimes, data, ts_units);
      sqlfile.insertTimeSeriesData(
        "Average", "Zone", "Zone", space_name, ts_name, OpenStudio::ReportingFrequency.new("Hourly"),
        OpenStudio::OptionalString.new(),
        ts_units, illumTS);

    end # writeTimeSeriesToSql()



    def annualSimulation(t_sqlFile, t_epwFile, t_space_names_to_calculate, t_radMaps, t_spaceWidths, t_spaceHeights, t_radMapPoints, \
      t_radGlareSensorViews, t_simCores, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_outPath, t_building, t_values, t_dcVectors, write_sql)
      sqlOutPath = OpenStudio::Path.new("#{Dir.pwd}/output/radout.sql")
      if OpenStudio::exists(sqlOutPath)
        OpenStudio::remove(sqlOutPath)
      end

      # for each environment period (design days, annual, or arbitrary) you will create a directory for results
      t_sqlFile.availableEnvPeriods.each do |envPeriod|

        puts "envPeriod = '" + envPeriod.to_s + "'"

        diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits = getTimeSeries(t_sqlFile, envPeriod)

        # check that we have all timeseries
        if (not diffHorizIllumAll) or (not dirNormIllumAll) or (not diffEfficacyAll) or (not dirNormEfficacyAll) or (not solarAltitudeAll) or (not solarAzimuthAll)
          puts "Missing required timeseries"
          exit false
        end

        # make timeseries
        simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime = \
        buildSimulationTimes(t_sqlFile, envPeriod, diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll)


        sqlOutFile = OpenStudio::SqlFile.new(sqlOutPath,
                                             t_epwFile.get(),
                                             OpenStudio::DateTime::now(),
                                             OpenStudio::Calendar.new(firstReportDateTime.date().year()));

        sqlOutFile.removeIndexes

        t_space_names_to_calculate.each do |space_name|
          illuminanceMatrixMaps = OpenStudio::MatrixVector.new();
          daylightSensorIlluminance = []
          meanIlluminanceMap = []
          minDGP = []
          meanDGP = []
          maxDGP = []

          puts "#{Time.now.getutc}: Processing Space: #{space_name}"

          timeSeriesIllum =[]
          timeSeriesGlare =[]

          simTimes.each_index do |i|
            spaceWidth = t_spaceWidths[space_name]
            spaceHeight = t_spaceHeights[space_name]

            illuminanceMatrixMaps << OpenStudio::Matrix.new(spaceWidth, spaceHeight, 0)
            daylightSensorIlluminance << 0
            meanIlluminanceMap << 0
            minDGP << 0
            meanDGP << 0
            maxDGP << 0

        
            # these must be declared in the thread otherwise will get overwritten on each loop
            tsDateTime = simTimes[i]

            # <someday> (image based glare analysis)
              #  system("gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} \
              #  | #{perlPrefix}genskyvec#{perlExtension} -m 1 | dctimestep \"#{outPath}/output/dc/#{space_name}/views/#{space_name}treg%03d.hdr\" | pfilt -1 -x /2 -y /2 > \
              #  \"#{outPath}/output/dc/#{space_name}/views/#{tsDateTime.gsub(/[: ]/,'_')}.hdr\"")
            # </someday>

            # Split up values by space

            illumValues, illumSensorValues, glareSensorValues = t_values[i][space_name]

            timeSeriesIllum[i] = tsDateTime.to_s.gsub(" ",",") + "," + "#{dirNormIllum[i]},#{diffHorizIllum[i]}," + illumSensorValues.join(',') + "," + illumValues.join(',')

            # add glare sensor values
            if t_radGlareSensorViews[space_name]
              if not glareSensorValues.nil?
                timeSeriesGlare[i] = tsDateTime.to_s.gsub(" ",",") + "," + glareSensorValues.join(',')

                if not glareSensorValues.empty?
                  sumDGP = 0
                  glareSensorValues.each do |val| 
                    sumDGP += val
                  end
                  minDGP[i] = glareSensorValues.min
                  meanDGP[i] = sumDGP / glareSensorValues.size.to_f
                  maxDGP[i] = glareSensorValues.max
                end
              end
            end

            m = OpenStudio::Matrix.new(spaceWidth, spaceHeight, 0)

            if not illumSensorValues.empty?
              daylightSensorIlluminance[i] = illumSensorValues[0]
            end

            n = 0
            sumIllumMap = 0
            illumValues.each do |val|
              x = (n%spaceWidth).to_i;
              y = (n/spaceWidth).to_i;
              sumIllumMap += val.to_f
              m[x, y] = val.to_f
              n = n + 1
            end

            illuminanceMatrixMaps[i] = m

            if n != 0
              meanIlluminanceMap[i] = sumIllumMap / n.to_f
            end

          end

          # Write results
          
          FileUtils.mkdir_p("#{Dir.pwd}/output/ts/#{space_name}/maps") unless File.exists?("#{Dir.pwd}/output/ts/#{space_name}/maps")
          f = File.open("#{Dir.pwd}/output/ts/#{space_name}/maps/#{space_name}_map.ill", "w")
          space = nil
          t_building.spaces.each do |s|
            this_name = s.name.get.gsub(' ', '_').gsub(':', '_')
            if this_name == space_name
              space = s
              break
            end
          end

          illuminanceMaps = space.illuminanceMaps

          # todo: use all if not empty
          if not illuminanceMaps.empty?

            map = illuminanceMaps[0]

            xmin = map.originXCoordinate
            xmax = xmin + map.xLength
            nx = map.numberofXGridPoints
            ymin = map.originYCoordinate
            ymax = ymin + map.yLength
            ny = map.numberofYGridPoints
            z = map.originZCoordinate

            xSpacing = (xmax-xmin)/nx
            ySpacing = (ymax-ymin)/ny

            puts "#{Time.now.getutc}: writing Radiance results file..."

            # illuminance to csv
            
            f.print "## OpenStudio Daylight Simulation Results file\n"
            f.print "## Header: xmin ymin z xmax ymin z xmax ymax z xspacing yspacing\n"
            f.print "## Data: month,day,time,directNormalIllumimance(external),diffuseHorizontalIlluminance(external),daylightSensorIlluminance,pointIlluminance [lux]\n"
            f.print "#{xmin} #{ymin} #{z} #{xmax} #{ymin} #{z} #{xmax} #{ymax} #{z} #{xSpacing} #{ySpacing}\n"
            timeSeriesIllum.each {|ts| f.print "#{ts}\n"}
            f.close

            # glare to csv 
            
            FileUtils.mkdir_p("#{Dir.pwd}/output/ts/#{space_name}/maps") unless File.exists?("#{Dir.pwd}/output/ts/#{space_name}/maps")
            f = File.open("#{Dir.pwd}/output/ts/#{space_name}/maps/#{space_name}_map.glr", "w")
            space = nil
            t_building.spaces.each do |s|
              this_name = s.name.get.gsub(' ', '_').gsub(':', '_')
              if this_name == space_name
                space = s
                break
              end
            end

            if t_radGlareSensorViews[space_name]
              f.print "## OpenStudio Daylight Simulation (glare) Results file\n"
              f.print "## Data: month,day,time,DGPSimplified values\n"
              timeSeriesGlare.each {|ts| f.print "#{ts}\n"}
              f.close
            end

            # all results to sql
            
            if write_sql == "Yes"
              puts "#{Time.now.getutc}: writing Radiance results database..."
              writeTimeSeriesToSql(sqlOutFile, simDateTimes, dirNormIllum, space_name, "Direct Normal Illuminance", "lux")
              writeTimeSeriesToSql(sqlOutFile, simDateTimes, diffHorizIllum, space_name, "Global Horizontal Illuminance", "lux")
              writeTimeSeriesToSql(sqlOutFile, simDateTimes, daylightSensorIlluminance, space_name, "Daylight Sensor Illuminance", "lux")
              writeTimeSeriesToSql(sqlOutFile, simDateTimes, meanIlluminanceMap, space_name, "Mean Illuminance Map", "lux")

              if t_radGlareSensorViews[space_name]
                writeTimeSeriesToSql(sqlOutFile, simDateTimes, minDGP, space_name, "Minimum Simplified Daylight Glare Probability", "")
                writeTimeSeriesToSql(sqlOutFile, simDateTimes, meanDGP, space_name, "Mean Simplified Daylight Glare Probability", "")
                writeTimeSeriesToSql(sqlOutFile, simDateTimes, maxDGP, space_name, "Maximum Simplified Daylight Glare Probability", "")
              end

              # I really have no idea how to populate these fields
              sqlOutFile.insertZone(space_name,
                                    0,
                                    0,0,0,
                                    0,0,0,
                                    0,
                                    0,
                                    0,
                                    0, 0,
                                    0, 0,
                                    0, 0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    true)

              xs = OpenStudio::DoubleVector.new()

              nx.times do |n|
                xs << xmin + (n * xSpacing)
              end

              ys = OpenStudio::DoubleVector.new()

              ny.times do |n|
                ys << ymin + (n * ySpacing)
              end

              sqlOutFile.insertIlluminanceMap(space_name, space_name + " DAYLIGHT MAP", t_epwFile.get().wmoNumber(),
                                              simDateTimes, xs, ys, map.originZCoordinate, 
                                              illuminanceMatrixMaps)
          
            end
          end
        end

        sqlOutFile.createIndexes

      end
    end # annualSimulation()


    # makeSchedules() 
    # write new lighting power schedules for the model
    
    def makeSchedules(model, sqlFile)
    
      # only run period in pre process job
      environmentName = "Run Period 1"

      # loop through each thermal zone
      model.getThermalZones.each do |thermalZone|

        spaces = thermalZone.spaces

        if spaces.empty?
          puts "ThermalZone '#{thermalZone.name}' has no spaces, skipping."
          next
        end
  
        # get people schedule for zone
        # TODO: require people for occupancy controls
        peopleTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone People Occupant Count", thermalZone.name.get.upcase)
  
        if peopleTimeseries.empty?
          puts "Cannot find timeseries 'Zone People Occupant Count' for ThermalZone '#{thermalZone.name}'."
        end
  
        # get lights schedule for zone
        lightsTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone Lights Electric Power", thermalZone.name.get.upcase)
  
        if lightsTimeseries.empty?
          newname = thermalZone.name.get.sub(/^OS:/, '');
          puts "Cannot find timeseries 'Zone Lights Electric Power' for ThermalZone '#{thermalZone.name}', skipping."
          next
        end
  
        lightsTimeseries = lightsTimeseries.get
   
        # get illuminance map
        illuminanceMap = thermalZone.illuminanceMap

        if illuminanceMap.empty?
          puts "Cannot find IlluminanceMap for ThermalZone '#{thermalZone.name}', skipping."
          next
        end

        illuminanceMap = illuminanceMap.get

        # get the space
        space = illuminanceMap.space
  
        if space.empty?
          puts "Cannot find Space for IlluminanceMap '#{illuminanceMap.name}' in ThermalZone '#{thermalZone.name}', skipping."
          next
        end
  
        space = space.get
  
        space_name = space.name.get.gsub(' ', '_').gsub(':', '_')

        radSqlPath = OpenStudio::Path.new("output/radout.sql")

        # load the illuminance map
        # assume this will be reported in 1 hour timesteps starting on 1/1
        averageIlluminances = []
        radSqlFile = OpenStudio::SqlFile.new(radSqlPath)

          # use the daylight sensor input
          spacename = space.name.get.gsub(' ', '_').gsub(':', '_')
          envPeriods = radSqlFile.availableEnvPeriods

          if envPeriods.size == 0
            puts "No available environment periods in radiance sql file, skipping"
            next
          end

          daylightSensor = radSqlFile.timeSeries(envPeriods[0], "Hourly", "Daylight Sensor Illuminance", space_name)
    
          if daylightSensor.empty?
            puts "Daylight sensor data could not be loaded, skipping"
            next
          end

          values = daylightSensor.get.values

          values.length.times do |i|
            val = values[i];

            if val < 0
              val = 0
            end
            averageIlluminances << val
          #end
         end

        daylightSetpoint = 0.0

        primaryDaylightingControl = thermalZone.primaryDaylightingControl
        if not primaryDaylightingControl.empty?
          daylightSetpoint = primaryDaylightingControl.get.illuminanceSetpoint
        end

          secondaryDaylightingControl = thermalZone.secondaryDaylightingControl
          if not secondaryDaylightingControl.empty?
            if daylightSetpoint == 0.0
              daylightSetpoint = secondaryDaylightingControl.get.illuminanceSetpoint
            else
              "Ignoring secondary daylighting control in ThermalZone '#{thermalZone.name}'"
          end
        end
  
        if daylightSetpoint == 0.0
          space.daylightingControls.each do |i|
            daylightSetpoint = i.illuminanceSetpoint
            if daylightSetpoint != 0.0
              break
            end
          end
        end
  
        if daylightSetpoint == 0.0
          puts "Illuminance setpoint is not defined in Space '#{space.name}' or in ThermalZone '#{thermalZone.name}', skipping."
          next
        end
  
        puts "ThermalZone '#{thermalZone.name}' illuminance setpoint is: #{daylightSetpoint.round(0)} lux"
  
        originalLightsValues = lightsTimeseries.values
        lightsValues = OpenStudio::Vector.new(averageIlluminances.size)
        averageIlluminances.each_index do |i|
          dimmingResponse = [(daylightSetpoint-averageIlluminances[i])/daylightSetpoint, 0].max
          #puts "#{daylightSetpoint}, #{averageIlluminances[i]}, #{dimmingResponse}"
          lightsValues[i] = dimmingResponse*originalLightsValues[i]

      #    is_occupied = false
      #    if not peopleTimeseries.empty?
      #      is_occupied = peopleTimeseries.get.values[i] > 0
      #    end
      #    
      #    daValues[i] = is_occupied*(averageIlluminances[i] > daylightSetpoint)
        end
  
        # get max lighting power
        lightingLevel = OpenStudio::maximum(lightsValues)
  
        if lightingLevel <= 0.0
          puts "ThermalZone '#{thermalZone.name}' lighting level is less than or equal to 0, skipping"
          next
        end
  
        puts "ThermalZone '#{thermalZone.name}' lighting level is: #{lightingLevel.round(0)} W"
  
        # normalize lights values
        averageIlluminances.each_index do |i|
          lightsValues[i] = lightsValues[i]/lightingLevel
        end
  
        startDate = OpenStudio::Date.new(OpenStudio::MonthOfYear.new(1), 1)
        interval = OpenStudio::Time.new(0,1,0)
        timeseries = OpenStudio::TimeSeries.new(startDate, interval, lightsValues, "W")

        schedule = OpenStudio::Model::ScheduleInterval::fromTimeSeries(timeseries, model)
  
        if schedule.empty?
          puts "Could not create modified lighting schedule for ThermalZone '#{thermalZone.name}', skipping"
          next
        end
  
        schedule = schedule.get
  
        schedule.setName(thermalZone.name.get + " Lights Schedule")
  
        # remove all lights in this zone
        spaces.each do |space|
          space.hardApplySpaceType(true)
          space.lights.each do |light|
            light.remove
          end
          space.luminaires.each do |luminaire|
            luminaire.remove
          end
        end
  
        # add a new lights object to first space in this zone and set schedule
        lightsDefinition = OpenStudio::Model::LightsDefinition.new(model)
        lightsDefinition.setLightingLevel(lightingLevel)
  
        lights = OpenStudio::Model::Lights.new(lightsDefinition)
        lights.setSchedule(schedule)
        lights.setSpace(spaces[0])
  
      end
  
    end # makeSchedules()


    def daylightMetrics(model, sqlFile)
    
      # load the Radiance output data

      radoutPath = OpenStudio::Path.new("output/radout.sql")
      radoutPath = OpenStudio::system_complete(radoutPath)

      radoutFile = OpenStudio::SqlFile.new(radoutPath)
      if not sqlFile.connectionOpen
        puts "SqlFile #{sqlPath} connection is not open"
        return false
      end 
    
      $METHOD = 1

      # get exterior illuminance timeseries from E+ run
      exteriorIlluminanceTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Site Exterior Horizontal Sky Illuminance")

      # summary report string
      summary_report = ""

      building_average_space = []
      building_average = ""

      # loop through all the spaces
      # building = model.getBuilding
      #building.spaces.each do |space|

      daylightAnalysisSpaces = []
      spaces = model.getSpaces
      spaces.each do |sp|
        if sp.illuminanceMaps.size > 0 
          daylightAnalysisSpaces << sp
        end
      end

      daylightAnalysisSpaces.each do |space|
      
        space_name = space.name.get.gsub(' ', '_').gsub(':', '_')
  
        thermalZone = space.thermalZone
        next if thermalZone.empty?
        thermalZone = thermalZone.get
  
        map_name = "#{space_name} DAYLIGHT MAP"
        map_index = radoutFile.illuminanceMapIndex(map_name)
        next if map_index.empty?
  
        daylightSetpoint = nil
        primaryDaylightingControl = thermalZone.primaryDaylightingControl
        if primaryDaylightingControl.empty?
          puts "ThermalZone \"#{thermalZone.to_s}\" has no primary daylighting control, skipping"
          next
        else
          daylightSetpoint = primaryDaylightingControl.get.illuminanceSetpoint
        end
  
        puts "Calculating Daylight Metrics for Space #{space_name}"
  
        da_daylit = []
        da_occupied = []
        da_daylit_occupied = []
        cda_daylit = []
        cda_occupied = []
        cda_daylit_occupied = []
        udi_daylit = []
        udi_occupied = []
        udi_daylit_occupied = []
  
  
        # get people timeseries from E+ run for this zone
        peopleTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone People Occupant Count", thermalZone.name.get.upcase)
  
        # loop over all timesteps, return type is std::vector< std::pair<int, DateTime> > 
        hourly_report_indices_dates = radoutFile.illuminanceMapHourlyReportIndicesDates(map_name)
        hourly_report_indices_dates.each do |hourly_report_index_date| 
    
          # initialize metrics to nil for timestep
          da_daylit << nil
          da_occupied << nil
          da_daylit_occupied << nil
          cda_daylit << nil
          cda_occupied << nil
          cda_daylit_occupied << nil
          udi_daylit << nil
          udi_occupied << nil
          udi_daylit_occupied << nil
  
          # extract timestep and index
          hourly_report_index = hourly_report_index_date.first
          hourly_report_date = hourly_report_index_date.second
    
          # see if this is a daylit hour
          daylit_hour = false
          if not exteriorIlluminanceTimeseries.empty?
            val = exteriorIlluminanceTimeseries[0].value(hourly_report_date)
            if val > 0
              daylit_hour = true
            end
          end
    
          # see if this is an occupied hour
          occupied_hour = false
          if not peopleTimeseries.empty?
            val = peopleTimeseries.get.value(hourly_report_date)
            if val > 0
              occupied_hour = true
            end
          end
  
          da = 0
          if $METHOD == 0
    
            # get map values
            map_values = radoutFile.illuminanceMap(hourly_report_index)
      
            # compute number of map points with illuminance greater than setpoint
            size1 = map_values.size1
            size2 = map_values.size2
            num = size1*size2
            num_da = 0
            for i in (0...size1)
              for j in (0...size2)
                map_value = map_values[i,j]
                if map_value >= daylightSetpoint
                  num_da += 1
                end
              end
            end
      
            da = num_da.to_f / num.to_f
      
          elsif $METHOD == 1
    
            x = OpenStudio::DoubleVector.new
            y = OpenStudio::DoubleVector.new
            map_values = OpenStudio::DoubleVector.new
      
            radoutFile.illuminanceMap(hourly_report_index, x, y, map_values)
      
            # compute DA, conDA, and UDI
            num = map_values.size
            num_da = 0
            num_cda = 0
            num_udi = 0
            map_values.each do |map_value|
              if map_value >= daylightSetpoint
                num_da += 1
                num_cda += 1
              end
              if map_value > 0 and map_value < daylightSetpoint  
                num_cda += map_value / daylightSetpoint 
              end
              if map_value > 100 and map_value < 3000
                num_udi += 1
              end
            end
      
            da = num_da.to_f / num.to_f
            cda = num_cda.to_f / num.to_f
            udi = num_udi.to_f / num.to_f

          end

          # assign to timeseries
          if daylit_hour
            da_daylit[-1] = da
            cda_daylit[-1] = cda
            udi_daylit[-1] = udi
          end
    
          if occupied_hour
            da_occupied[-1] = da
            cda_occupied[-1] = cda
            udi_occupied[-1] = udi
          end
    
          if daylit_hour and occupied_hour
            da_daylit_occupied[-1] = da
            cda_daylit_occupied[-1] = cda
            udi_daylit_occupied[-1] = udi
          end

        end
  
        # compute annual metrics for space
  
        #Daylight Autonomy
        da_daylit_sum = 0
        da_daylit_num = 0
        da_daylit.each do |da|
          if not da.nil?
            da_daylit_sum += da
            da_daylit_num += 1
          end
        end
        annual_da_daylit = da_daylit_sum.to_f / da_daylit_num.to_f
        summary_report += "#{space_name},DA(#{daylightSetpoint.round(0)}),Daylit Hours,#{annual_da_daylit.round(2)},#{da_daylit_sum.round(0)},#{da_daylit_num}\n"
  
        da_occupied_sum = 0
        da_occupied_num = 0
        da_occupied.each do |da|
          if not da.nil?
            da_occupied_sum += da
            da_occupied_num += 1
          end
        end
        annual_da_occupied = da_occupied_sum.to_f / da_occupied_num.to_f
        summary_report += "#{space_name},DA(#{daylightSetpoint.round(0)}),Occupied Hours,#{annual_da_occupied.round(2)},#{da_occupied_sum.round(0)},#{da_occupied_num}\n"
  
        da_daylit_occupied_sum = 0
        da_daylit_occupied_num = 0
        da_daylit_occupied.each do |da|
          if not da.nil?
            da_daylit_occupied_sum += da
            da_daylit_occupied_num += 1
          end
        end
        annual_da_daylit_occupied = da_daylit_occupied_sum.to_f / da_daylit_occupied_num.to_f
        summary_report += "#{space_name},DA(#{daylightSetpoint.round(0)}),Daylit and Occupied Hours,#{annual_da_daylit_occupied.round(2)},#{da_daylit_occupied_sum.round(0)},#{da_daylit_occupied_num}\n"
  
        #Continuous Daylight Autonomy
        cda_daylit_sum = 0
        cda_daylit_num = 0
        cda_daylit.each do |cda|
          if not cda.nil?
            cda_daylit_sum += cda
            cda_daylit_num += 1
          end
        end
        annual_cda_daylit = cda_daylit_sum.to_f / cda_daylit_num.to_f
        summary_report += "#{space_name},conDA(#{daylightSetpoint.round(0)}),Daylit Hours,#{annual_cda_daylit.round(2)},#{cda_daylit_sum.round(0)},#{cda_daylit_num}\n"
  
        cda_occupied_sum = 0
        cda_occupied_num = 0
        cda_occupied.each do |cda|
          if not cda.nil?
            cda_occupied_sum += cda
            cda_occupied_num += 1
          end
        end
        annual_cda_occupied = cda_occupied_sum.to_f / cda_occupied_num.to_f
        summary_report += "#{space_name},conDA(#{daylightSetpoint.round(0)}),#{annual_cda_occupied.round(2)},Occupied Hours#{cda_occupied_sum.round(0)},#{cda_occupied_num}\n"
  
        cda_daylit_occupied_sum = 0
        cda_daylit_occupied_num = 0
        cda_daylit_occupied.each do |cda|
          if not cda.nil?
            cda_daylit_occupied_sum += cda
            cda_daylit_occupied_num += 1
          end
        end
        annual_cda_daylit_occupied = cda_daylit_occupied_sum.to_f / cda_daylit_occupied_num.to_f
        summary_report += "#{space_name},conDA(#{daylightSetpoint.round(0)}),Daylit and Occupied Hours,#{annual_cda_daylit_occupied.round(2)},#{cda_daylit_occupied_sum.round(0)},#{cda_daylit_occupied_num}\n"

       # Useful Daylight Illuminance
        udi_daylit_sum = 0
        udi_daylit_num = 0
        udi_daylit.each do |udi|
          if not udi.nil?
            udi_daylit_sum += udi
            udi_daylit_num += 1
          end
        end
        annual_udi_daylit = udi_daylit_sum.to_f / udi_daylit_num.to_f
        summary_report += "#{space_name},UDI(100-3000),Daylit Hours,#{annual_udi_daylit.round(2)},#{udi_daylit_sum.round(0)},#{udi_daylit_num}\n"
  
        udi_occupied_sum = 0
        udi_occupied_num = 0
        udi_occupied.each do |udi|
          if not udi.nil?
            udi_occupied_sum += udi
            udi_occupied_num += 1
          end
        end
        annual_udi_occupied = udi_occupied_sum.to_f / udi_occupied_num.to_f
        summary_report += "#{space_name},UDI(100-3000),Occupied Hours,#{annual_udi_occupied.round(2)},#{udi_occupied_sum.round(0)},#{udi_occupied_num}\n"
  
        udi_daylit_occupied_sum = 0
        udi_daylit_occupied_num = 0
        udi_daylit_occupied.each do |udi|
          if not udi.nil?
            udi_daylit_occupied_sum += udi
            udi_daylit_occupied_num += 1
          end
        end
        annual_udi_daylit_occupied = udi_daylit_occupied_sum.to_f / cda_daylit_occupied_num.to_f
        summary_report += "#{space_name},UDI(100-3000),Daylit and Occupied Hours,#{annual_udi_daylit_occupied.round(2)},#{cda_daylit_occupied_sum.round(0)},#{cda_daylit_occupied_num}\n"

        # now replace nil with 0 in each timeseries to radout.sql for plotting
  
        building_average_space << annual_da_daylit_occupied
  
      end

      # DLM: can we make some more metrics that are area weighted rather than just space weighted?
      building_average_space_sum = 0.0
      building_average_space.each {|e| building_average_space_sum += e}

      # catch zero condition
      if building_average_space_sum == 0.0
        building_average = 0.0
        puts "warning: Daylight Autonomy for building is zero, check daylighting control point(s) setup."
      else
        building_average = building_average_space_sum / building_average_space.length
      end

      File.open("DaylightingMetrics.csv", 'w') do |file|
        file.puts "# OpenStudio Daylight Metrics Report"
        file.puts "# Average daylight autonomy for building daylit spaces: #{building_average.round(2)}"
        file.puts "# Space data format: [space_name] [metric(setpoint)] [input_hours_range] [metric_value] [hours_met] [input_hours]"       
        file.puts summary_report
      end

    end #daylightMetrics()



    ## ## ## ## ## ##



    # actually do the thing
    
    sqlOutFile = ""
    radoutFile = ""
    
    # settle in, it's gonna be a bumpy ride...
    Dir.chdir("#{radPath}")
    puts "\nRunning radiance in directory: '#{Dir.pwd}'"
    
    epwFile = OpenStudio::OptionalEpwFile.new(OpenStudio::EpwFile.new(epw_path))
        
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

    #epw2weapath = OpenStudio::Path.new(ENV['EPW2WEAPATH'])

    #puts "Executing epw2wea: #{epw2weapath}"
    #exec_statement("#{epw2weapath.to_s} #{epw_path.to_s} wx/in.wea")
    exec_statement("epw2wea \"#{epw_path.to_s}\" wx/in.wea")

    # get site info from .wea file
    # TODO may want to get this from properly set OS.site object
    site = model.getSite
    
    site_name = site.name.to_s
    site_latitude = site.latitude
    site_longitude = site.longitude
    site_meridian = site.timeZone.to_f * 15

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
    calculateDaylightCoeffecients(radPath, sim_cores, catCommand, options_tregVars, options_klemsDensity, options_skyvecDensity, options_dmx, \
    options_vmx, rad_settings, procsUsed)

    # make merged building-wide illuminance schedule(s)
    values, dcVectors = runSimulation(space_names_to_calculate, sqlFile, sim_cores, options_skyvecDensity, site_latitude, site_longitude, \
    site_meridian, radPath, spaceWidths, spaceHeights, radGlareSensorViews, runner, write_sql)
    

    # make space-level illuminance schedules and radout.sql results database
    # hoping this is no longer necessary...
    annualSimulation(sqlFile, epwFile, space_names_to_calculate, radMaps, spaceWidths, spaceHeights, radMapPoints, radGlareSensorViews, \
    sim_cores, site_latitude, site_longitude, site_meridian, radPath, building, values, dcVectors, write_sql)


    # make new lighting power schedules based on Radiance daylight data
    makeSchedules(model, sqlFile)


    # compute daylight metrics for model
    daylightMetrics(model, sqlFile)


    # remove illuminance map and daylighting controls from model, so they are not re-simulated in E+
    model.getThermalZones.each do |thermalZone|
      thermalZone.resetPrimaryDaylightingControl
      thermalZone.resetSecondaryDaylightingControl
      thermalZone.resetIlluminanceMap
    end
    
    
    # report initial condition of model
    daylightAnalysisSpaces = []
    spaces = model.getSpaces
    spaces.each do |sp|
      if sp.illuminanceMaps.size > 0 
        daylightAnalysisSpaces << sp
      end
    end
    puts ""
    runner.registerInitialCondition("Input building model contains #{model.getSpaces.size} spaces.")

    # report final condition of model
    runner.registerFinalCondition("Measure ran Radiance on the #{daylightAnalysisSpaces.size} spaces containing daylighting objects.")
    
    return true

  end #run

end

# register the measure to be used by the application
RadianceMeasure.new.registerWithApplication
