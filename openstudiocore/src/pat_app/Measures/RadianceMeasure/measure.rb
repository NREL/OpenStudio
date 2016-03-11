# see the URL below for information on how to write OpenStudio measures:
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

# design document for this measure is at https://docs.google.com/document/d/16_TLRuhc4VFs2o0gRAp81hRObet7-s6fUEWo3HO7LpE/edit#

require 'fileutils'
require 'csv'
require 'tempfile'
require 'date'
require 'json'
require 'erb'
require 'matrix'
require 'open3'

class Array
  def average 
    fail "Cannot average 0 items" if self.size == 0
    sum = self.inject(:+)
    
    (sum / self.size).to_f
  end
end
  
# start the measure
class RadianceMeasure < OpenStudio::Ruleset::ModelUserScript

  # human readable name
  def name
    return 'Radiance Daylighting Measure'
  end

  # human readable description
  def description
    return 'This measure uses Radiance instead of EnergyPlus for daylighting calculations with OpenStudio.'
  end

  # human readable description of modeling approach
  def modeler_description
    return 'The OpenStudio model is converted to Radiance format. All spaces containing daylighting objects (illuminance map, daylighting control point, and optionally glare sensors) will have annual illuminance calculated using Radiance, and the OS model\'s lighting schedules can be overwritten with those based on daylight responsive lighting controls.'
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
    apply_schedules.setDescription('Update lighting load schedules with those computed by Radiance')
    args << apply_schedules
    
    chs = OpenStudio::StringVector.new
    chs << 'Default'
    chs << 'Min'
    chs << 'Max'
    use_cores = OpenStudio::Ruleset::OSArgument.makeChoiceArgument('use_cores', chs, true)
    use_cores.setDisplayName('Cores')
    use_cores.setDefaultValue('Default')    
    use_cores.setDescription('Number of CPU cores to use for Radiance jobs. Default is to use all but one core, NOTE: this option is ignored on Windows.')
    args << use_cores

    chs = OpenStudio::StringVector.new
    chs << 'Model'
    chs << 'Testing'
    rad_settings = OpenStudio::Ruleset::OSArgument.makeChoiceArgument('rad_settings', chs, true)
    rad_settings.setDisplayName('Radiance Settings')
    rad_settings.setDefaultValue('Model')
    rad_settings.setDescription('The measure reads Radiance simulation parameters from the "Model" by default. The "Testing" option can be used for testing your model with the Radiance workflow; it uses very crude parameters for a fast simulation but produces very inaccurate results.')
    args << rad_settings

    debug_mode = OpenStudio::Ruleset::OSArgument::makeBoolArgument('debug_mode', false)
    debug_mode.setDisplayName('Debug Mode')
    debug_mode.setDefaultValue('false')
    debug_mode.setDescription('Generate additional log messages, images for each window group, and save all window group output.')
    args << debug_mode

    cleanup_data = OpenStudio::Ruleset::OSArgument::makeBoolArgument('cleanup_data', false)
    cleanup_data.setDisplayName('Cleanup Data')
    cleanup_data.setDefaultValue('false')
    cleanup_data.setDescription('Delete Radiance input and (most) output data, post-simulation (lighting schedules are passed to OpenStudio model (and daylight metrics are passed to OpenStudio-server, if applicable)')
    args << cleanup_data

    return args

  end

  def read_illuminance_file(filename, runner)
    m = Matrix[]
    data_section = false
    header = []
    data = []
  
    print_statement("Reading '#{filename}'", runner)
    fail "Could not find illuminance file #{filename}" unless File.exist?(filename)
    File.read(filename).each_line do |line|
      data_section = true if line =~ /^\s?\d/  
      if data_section
        csv_line = CSV.parse_line(line.strip, {col_sep: " "})
    
        m = Matrix.rows(m.to_a << csv_line)
      else
        header << "#{line}"
      end
    end

    return m, header
  end


  # better OS detection
  # will call out Git Bash shells as Windows machines, optionally differentiates Mac versus *nix
  module OS
    def OS.windows
      (/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil
    end

    def OS.mac
     (/darwin/ =~ RUBY_PLATFORM) != nil
    end

    def OS.unix
      !OS.windows
    end

    def OS.linux
      OS.unix and not OS.mac
    end
  end


  # check for number of rmtxop processes
  def merge_count()
    if OS.windows
      # TODO: properly handle the Windows case
      # TODO: possibly add count as a user option
      return 1
    else 
      return `pgrep rmtxop`.split.size
    end
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    OpenStudio::Logger::instance().standardOutLogger().enable()
    
    # Enable debug-level log messages
    # OpenStudio::Logger::instance().standardOutLogger().setLogLevel(OpenStudio::Debug)

    # use the built-in error checking
    if !runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    # assign the user inputs to variables
    apply_schedules = runner.getStringArgumentValue('apply_schedules', user_arguments)    
    use_cores = runner.getStringArgumentValue('use_cores', user_arguments)
    rad_settings = runner.getStringArgumentValue('rad_settings', user_arguments)
    debug_mode = runner.getBoolArgumentValue('debug_mode',user_arguments)
    cleanup_data = runner.getBoolArgumentValue('cleanup_data',user_arguments)
    
    # Energyplus "pre-run" model dir
    epout_dir = 'eplus_preprocess'
    if !File.exist?(epout_dir)
      FileUtils.mkdir_p(epout_dir)
    end

    # Radiance model dir
    rad_dir = 'radiance'
    if !File.exist?(rad_dir)
      FileUtils.mkdir_p(rad_dir)
    end

    ## Radiance Utilities

    # print statement and execute as system call
    def exec_statement(s, runner)
      if OS.windows
        s = s.tr("/", "\\")
      end
      runner.registerInfo("#{s}")
      # additional puts for OSApp until v2.0...
      puts "[Radiance Measure #{Time.now.getutc}]: \$ #{s}"
      result = system(s)
      return result
    end

    # print statement for OS-Server and OSApp
    def print_statement(s, runner)
      #if /mswin/.match(RUBY_PLATFORM) || /mingw/.match(RUBY_PLATFORM)
      if OS.windows
        s = s.tr("/", "\\")
      end
      runner.registerInfo("#{s}")
      # additional puts for OSApp until v2.0...
      puts "[Radiance Measure #{Time.now.getutc}]: #{s}"
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
    sim_cores = '1'

    if use_cores == 'Max'
      sim_cores = coreCount
    elsif use_cores == 'Min'
      sim_cores = 1
    else
      sim_cores = coreCount - 1
    end
    if OS.windows #/mswin/.match(RUBY_PLATFORM) || /mingw/.match(RUBY_PLATFORM)
      print_statement("Radiance multiprocessing features are not supported on Windows.", runner)
      sim_cores = 1
    end
    print_statement("Using #{sim_cores} core(s) for Radiance jobs", runner)

    # help those poor Windows users out
    perlExtension = ""
    catCommand = "cat"
    osQuote = "\'"
    if OS.windows #/mswin/.match(RUBY_PLATFORM) || /mingw/.match(RUBY_PLATFORM)
      perlExtension = ".pl"
      catCommand = "type"
      osQuote = "\""
    end

    ## END Radiance Utilities

    print_statement("### DEBUG: running in debug mode", runner) if debug_mode

    # setup environment for Radiance and Perl
    co = OpenStudio::Runmanager::ConfigOptions.new(true);
    co.fastFindRadiance();
    radiancePath = co.getTools().getLastByName("rad").localBinPath.parent_path
    path = OpenStudio::Path.new(radiancePath).to_s
    raypath = (OpenStudio::Path.new(radiancePath).parent_path() / 
    OpenStudio::Path.new('lib')).to_s()   

    epw2weapath = (OpenStudio::Path.new(radiancePath) / OpenStudio::Path.new('epw2wea')).to_s

    programExtension = ""
    if OS.windows #/mswin/.match(RUBY_PLATFORM) || /mingw/.match(RUBY_PLATFORM)
      programExtension = ".exe"
      perlpath = ""
      if OpenStudio::applicationIsRunningFromBuildDirectory()
        perlpath = OpenStudio::getApplicationRunDirectory().parent_path().parent_path() / 
        OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
      else
        perlpath = OpenStudio::getApplicationRunDirectory().parent_path() / 
        OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
      end
      print_statement("Adding path for local perl: " + perlpath.to_s, runner)
      ENV["PATH"] = path + ";" + ENV["PATH"] + ";" + perlpath.to_s
      ENV["RAYPATH"] = path + ";" + raypath + ";."
    else
      ENV["PATH"] = path + ":" + ENV["PATH"]
      ENV["RAYPATH"] = path + ":" + raypath + ":."
    end
    
    # Radiance version detection and environment reportage                 
    # need to help Open3 on Windows (path sep issues)             
    returnDir = Dir.pwd
    Dir.chdir(path)
    ver = Open3.capture2("rcontrib#{programExtension} -version")
    # aaaannnddd, we're back...
    Dir.chdir(returnDir)
    print_statement("Radiance version info: #{ver[0]}", runner)
    print_statement("Radiance binary dir: #{path}", runner)
    print_statement("Radiance library dir: #{raypath}", runner)

    print_statement("Running on Windows (sorry)", runner) if OS.windows && debug_mode
    print_statement("Running on unix", runner) if OS.unix && debug_mode

    if Dir.glob(epw2weapath + programExtension).empty?
      runner.registerError("Cannot find epw2wea tool in radiance installation at '#{radiancePath}'. You may need to install a newer version of Radiance.")
      exit false
    end

    ENV["EPW2WEAPATH"] = epw2weapath + programExtension

 		if !which("perl")
 		 runner.registerError('Perl could not be found in path, exiting')
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
            alt_weath_path = File.expand_path(File.join(File.dirname(__FILE__), \
            "../../../resources"))
            alt_epw_path = File.expand_path(File.join(alt_weath_path, test.get.to_s))
            server_epw_path = File.expand_path(File.join(File.dirname(__FILE__), \
            "../../weather/#{File.basename(test.get.to_s)}"))
            if File.exist?(alt_epw_path)
              epw_path = OpenStudio::Path.new(alt_epw_path)
            elsif File.exist? server_epw_path
              epw_path = OpenStudio::Path.new(server_epw_path)
            else
              runner.registerError("Model has been assigned a weather file, but the file is not in \
              the specified location of '#{test.get}'. server_epw_path: #{server_epw_path}, test \
              basename: #{File.basename(test.get.to_s)}, test: #{test}")            
              return false
            end
          end
        else
          runner.registerError('Model has a weather file assigned, but the weather file path has \
          been deleted.')
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
    print_statement('Creating workflow', runner)

    jobtree = workflow.create(OpenStudio::system_complete(runDir), \
    OpenStudio::system_complete(modelPath), OpenStudio::Path.new(epw_path))
    runmanager.enqueue(jobtree, true)
    print_statement("Running jobs in #{runDir}", runner)
    runmanager.setPaused(false)
    runmanager.waitForFinished()
        
    if jobtree.treeErrors.succeeded
      print_statement('OpenStudio to Radiance translation complete', runner)
    
    else   
      jobtree.treeErrors.errors.each do |err|
        print_statement("ERROR: #{err}", runner)
      end
      print_statement("Model issue(s) caused EnergyPlus preprocess failure, aborting.", runner)
      abort()     

    end

    ##  Radiance crap

    modelPath = OpenStudio::system_complete(modelPath)
    radPath = modelPath.parent_path / OpenStudio::Path.new("radiance")
    
    windowControls = Dir.glob("scene/glazing/WG*.rad")
    
    # set up output dirs
    FileUtils.mkdir_p("#{radPath}/output/dc") unless File.exist?("#{radPath}/output/dc")
    FileUtils.mkdir_p("#{radPath}/output/ts") unless File.exist?("#{radPath}/output/ts")
    FileUtils.mkdir_p("#{radPath}/output/dc/merged_space/maps") unless \
    File.exist?("#{radPath}/output/dc/merged_space/maps")
    FileUtils.mkdir_p("#{radPath}/sql") unless File.exist?("#{radPath}/sql")
    FileUtils.mkdir_p("#{radPath}/wx") unless File.exist?("#{radPath}/wx")
    FileUtils.mkdir_p("#{radPath}/octrees") unless File.exist?("#{radPath}/octrees")
 
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
    if OS.windows
      procsUsed = ""
    else
      procsUsed = "-n #{sim_cores}"
    end

    # core functions

    def calculateDaylightCoeffecients(radPath, sim_cores, t_catCommand, options_tregVars,
                                      options_klemsDensity, options_skyvecDensity, options_dmx, 
                                      options_vmx, rad_settings, procsUsed, runner, debug_mode)

      # get calculation points array size (needed for rmtxop later)
      mapFile=File.open("numeric/merged_space.map","r")
      rfluxmtxDim = mapFile.readlines.size.to_s
      
      # sort out window groups, controls
      haveWG0 = ""
      haveWG1 = ""
      windowGroupCheck = File.open("bsdf/mapping.rad")
      windowGroupCheck.each do |row|

        next if row[0] == "#"
        wg=row.split(",")[0]
         
        if wg == "WG0"
          haveWG0 = "True"
        elsif wg == "WG1"
          haveWG1 = "True"
        end
    
      end  
      windowGroupCheck.close
      
      if 1000 < rfluxmtxDim.to_i && rfluxmtxDim.to_i < 2999
        print_statement("WARN: Model contains a large number of Radiance calculation points (#{rfluxmtxDim}), will produce large results files and potential memory issues.", runner)
      elsif 3000 < rfluxmtxDim.to_i
        print_statement("ERROR: Too many calculation points in model (#{rfluxmtxDim}). Consider reducing the number or resolution of illuminance maps in this model.", runner)
        exit false
      else
        print_statement("Passing #{rfluxmtxDim} calculation points to Radiance", runner)
      end

      # process individual window groups
      print_statement("Computing daylight coefficient matrices", runner)
      exec_statement("oconv materials/materials.rad model.rad > octrees/model_dc.oct", runner)

      windowMaps = File::open("bsdf/mapping.rad")
      
      windowMaps.each do |row|

        next if row[0] == "#"
        wg=row.split(",")[0]
        
        rad_command = "" 
                 
        if wg == "WG0" # window group zero (all uncontrolled windows) 

          print_statement("Computing view matrix for uncontrolled windows (WG0)", runner)

          # make WG0 octree (with shade-controlled window groups blacked out, if any)
          input_files = ""
          if haveWG1 == "True"
            input_files = "materials/materials.rad materials/materials_WG0.rad model.rad"
          else 
            input_files = "materials/materials.rad model.rad skies/dc_sky.rad"
          end
          
          # for the calc, include unit sky
          exec_statement("oconv #{input_files} skies/dc_sky.rad > octrees/model_WG0.oct", runner)
                    
          if debug_mode
            # for check images (insert sky later, in genImages())
            exec_statement("oconv #{input_files} > octrees/debug_model_WG0.oct", runner)
          end
          
          # use more aggro simulation parameters because this is basically a view matrix
          rtrace_args = "#{options_vmx}"
          
          ### foo (-faf)
          rad_command = "#{t_catCommand} numeric/merged_space.map | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} -faa -o output/dc/WG0.vmx -m skyglow octrees/model_WG0.oct"
          exec_statement(rad_command, runner)

        else # controlled window group

          print_statement("Processing shade-controlled window group '#{wg}'", runner)
                  
          if row.split(",")[4].rstrip == "SWITCHABLE" # has switchable glazing
          
            print_statement("Window Group '#{wg}' has switchable glazing control, calculating two view matrices", runner)
     
            # black out WG0 and all other WG shades
            # start with base materials, then black everything out
            base_mats = "materials/materials.rad materials/materials_blackout.rad"

            # do view matrices, one for each tint state
            rtrace_args = "#{options_vmx}"
            
            ["clear", "tinted"].each do |state|
              
              # for the calc
              exec_statement("oconv #{base_mats} materials/#{wg}_#{state}.mat model.rad skies/dc_sky.rad > octrees/model_#{wg}_#{state}.oct", runner)
              if debug_mode
                # for check images 
                exec_statement("oconv #{base_mats} materials/#{wg}_#{state}.mat model.rad > octrees/debug_model_#{wg}_#{state}.oct", runner)
              end
              print_statement("Computing view matrix for window group '#{wg}' in #{state} state", runner)
              ### foo (-faf)
              exec_statement("#{t_catCommand} \"numeric/merged_space.map\" | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{options_tregVars} -faa -o \"output/dc/#{wg}_#{state}.vmx\" -m skyglow octrees/model_#{wg}_#{state}.oct", runner)
            
            end

          else # has shades

            # use more chill sim parameters
            rtrace_args = "#{options_dmx}"
          
            # do daylight matrices for controlled windows
            print_statement("Computing daylight matrix for window group '#{wg}'", runner)
            
            if debug_mode
            
              # make octrees for debug images
              # load materials, then black out all materials, then add in scene geometry and glazing (no shades)
              input_files = "materials/materials.rad materials/materials_blackout.rad"
              # now reset window group glazing material and make an octree
              exec_statement("oconv #{input_files} materials/#{wg}.mat scene/*.rad scene/glazing/*.rad > octrees/debug_model_#{wg}.oct", runner)
              # now reset window group shade material to actual and make an octree
              exec_statement("oconv #{input_files} materials/#{wg}.mat materials/#{wg}_SHADE.mat scene/*.rad scene/glazing/*.rad scene/shades/#{wg}_SHADE.rad > octrees/debug_model_#{wg}_shade.oct", runner)
            
            end
            
            ### foo (-faa)
            rad_command = "rfluxmtx #{rtrace_args} -n #{sim_cores} -faa -v scene/shades/#{wg}_SHADE.rad skies/dc_sky.rad -i octrees/model_dc.oct > \"output/dc/#{wg}.dmx\""
            exec_statement(rad_command, runner)
          
          end

        end # calculate DMX
        
        
      end # individual window group processing
        
      # do remaining view matrices, if applicable
      
      shade_check = Dir.glob("scene/shades/WG*.rad")     
      if shade_check.length > 0
          
        # compute view matrices for shade controlled window groups all at once

        # use fine params   
        rtrace_args = "#{options_vmx}" 

        print_statement("Computing view matri(ces) for all remaining window groups", runner)

        # get the shaded window groups' shade polygons

        wgInput = []
        
        # get the SHADE polygons for sampling (NOT the GLAZING ones!)
        # this will automatically omit switchable glazing-controlled window groups. ;)
        
        Dir.glob("scene/shades/WG*.rad") {|file|
          wgInput << file
        }

        # make the receiver file
        exec_statement("#{t_catCommand} \"materials/materials_vmx.rad\" #{wgInput.join(" ")} > receivers_vmx.rad", runner)

        # make the octree
        scene_files = []
        Dir.glob("scene/*.rad").each {|f| scene_files << f}
        exec_statement("oconv materials/materials.rad #{scene_files.join(' ')} > octrees/model_vmx.oct", runner)
        

        # make rfluxmtx do all the work
        ### foo (-faf)
        rad_command = "rfluxmtx #{rtrace_args} -n #{sim_cores} -ds .15 -faa -y #{rfluxmtxDim} -I -v - receivers_vmx.rad -i octrees/model_vmx.oct < numeric/merged_space.map"
        exec_statement(rad_command, runner)
        
        FileUtils.rm('receivers_vmx.rad')
      
      end # VMX for controlled window groups
      

      if haveWG1 == "True"

        # compute daylight coefficient matrix for window group control points

        rtrace_args = "#{options_dmx}"
        exec_statement("oconv \"materials/materials.rad\" model.rad skies/dc_sky.rad > octrees/model_wc.oct", runner)
        print_statement("Computing DCs for window control points", runner)

        ### foo (keep this one as ASCII)
        rad_command = "#{t_catCommand} \"numeric/window_controls.map\" | rcontrib #{rtrace_args} #{procsUsed} -I+ -faa -fo #{options_tregVars} " + \
        "-o \"output/dc/window_controls.vmx\" -m skyglow octrees/model_wc.oct"
        exec_statement(rad_command, runner)
      
      end



    print_statement("Daylight coefficient matrices computed.", runner)
    
    end # calculateDaylightCoeffecients()

    # annual simulation dealio
    def runSimulation(t_space_names_to_calculate, t_sqlFile, t_simCores, t_options_skyvecDensity,
                      t_site_latitude, t_site_longitude, t_site_stdmeridian, t_radPath,
                      t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, runner, debug_mode)

      print_statement("Performing annual daylight simulation(s)", runner)

      rawValues = {}
      values = {}
      dcVectors = {}

      # sort out window groups, controls
      haveWG0 = "False"
      haveWG1 = "False"
      windowGroupCheck = File.open("bsdf/mapping.rad")
      windowGroupCheck.each do |row|

        next if row[0] == "#"
        wg=row.split(",")[0]
         
        if wg == "WG0"
          haveWG0 = "True"
        elsif wg == "WG1"
          haveWG1 = "True"
        end
    
      end  
      windowGroupCheck.close
       
      # Run the simulation 

      simulations = []

      rad_command = "gendaymtx -m #{t_options_skyvecDensity} \"wx/in.wea\" > annual-sky.mtx"
      exec_statement(rad_command, runner)

      windowMaps = File.open("bsdf/mapping.rad")
  
      # do annual sim for each window group and state
  
      windowMaps.each do |row|
        # skip header
        next if row[0] == "#"
        wg = row.split(",")[0]
    
        # do uncontrolled windows (WG0)
        if wg == "WG0"
        # if row.split(",")[2] == "n/a" || row.split(",")[2] == "AlwaysOff"
          # keep header, convert to illuminance, but no transpose
          ### foo (-ff)
          
          while merge_count() > 2
            puts "waiting in rmtxop queue..."
            sleep(5)
          end
          rad_command = "dctimestep output/dc/#{wg}.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}.ill"
          exec_statement(rad_command, runner)
          
        else

        # do all controlled window groups
        
          if row.split(",")[4].rstrip == "SWITCHABLE"
          
            # make single phase illuminance sched for each state
            states = ["clear", "tinted"]
            states.each_index do |i|
              while merge_count() > 2
                puts "waiting in rmtxop queue..."
                sleep(5)
              end
              print_statement("Calculating annual iluminance for window group '#{wg}', state: #{states.index(states[i])} (switchable glazing - #{states[i]})", runner)
              ### foo (-ff)
              exec_statement("dctimestep output/dc/#{wg}_#{states[i]}.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}_#{states.index(states[i])}.ill", runner)             
            end
          
          else
        
            wgXMLs = row.split(",")[4..-1]
            if wgXMLs.size > 2
              print_statement("WARN: Window Group #{wg} has #{wgXMLs.size} BSDFs (2 max supported by OpenStudio application).", runner)
            end

            wgXMLs.each_index do |i|
              #rad_command = "dctimestep output/dc/#{wg}.vmx bsdf/#{wgXMLs[i].strip} output/dc/#{wg}.dmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}_INDEX#{wgXMLs.index[i]}_#{wgXMLs[i].split[0]}.ill"
              while merge_count() > 2
                puts "waiting in rmtxop queue..."
                sleep(5)
              end
              print_statement("Calculating annual iluminance for window group '#{wg}', state: #{wgXMLs.index(wgXMLs[i])} (BSDF filename: '#{wgXMLs[i].split[0]}'):", runner)
              rad_command = "dctimestep output/dc/#{wg}.vmx bsdf/#{wgXMLs[i].strip} output/dc/#{wg}.dmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > output/ts/#{wg}_#{wgXMLs.index(wgXMLs[i])}.ill"
              ### orig ^^^
              ###rad_command = "dctimestep output/dc/#{wg}.vmx bsdf/#{wgXMLs[i].strip} output/dc/#{wg}.dmx annual-sky.mtx | rmtxop -ff -c 47.4 120 11.6 - > output/ts/#{wg}_#{wgXMLs.index(wgXMLs[i])}.ill"
              exec_statement(rad_command, runner)
              
            end
          end
        end
      end

      if haveWG1 == "True"

        # get annual values for window control sensors (note: convert to illuminance, no transpose, strip header)
        ### foo leave at -fa
        while merge_count() > 2
          puts "waiting in rmtxop queue..."
          sleep(5)
        end
        exec_statement("dctimestep output/dc/window_controls.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - | getinfo - > output/ts/window_controls.ill", runner)  
        print_statement("Blending window group results per shade control schedule", runner)
  
        # do that window group/state merge thing

        wg_index = 0
        
        print_statement("### DEBUG: getting window shade control(s) values", runner) 
        filename = "output/ts/window_controls.ill"          
        windowControls, _header = read_illuminance_file(filename, runner)
        print_statement("### DEBUG: windowControls matrix is #{windowControls.row_count} rows x #{windowControls.column_count} columns", runner)

        windowGroups = File.open("bsdf/mapping.rad")
        windowGroups.each do |wg|

          next if wg[0] == "#"              # skip header
          windowGroup = wg.split(",")[0]
          next if windowGroup == "WG0"      # skip unshaded windows

          wg_index += 1

          wgIllumFiles = Dir.glob("output/ts/#{windowGroup}_*.ill").sort

          shadeControlType = wg.split(",")[2].to_s
          shadeControlSetpointWatts = wg.split(",")[3].to_f
          shadeControlSetpoint = shadeControlSetpointWatts * 179 # Radiance's luminous efficacy factor
          wg_normal = wg.split(",")[1]
          wg_normal_x = wg_normal.split(" ")[0].to_f
          wg_normal_y = wg_normal.split(" ")[1].to_f
          wg_normal_z = wg_normal.split(" ")[2].to_f
   
          # DLM: hacktastic way to implement these options for now
          if shadeControlType == "AlwaysOn"
            shadeControlSetpoint = -1000
          elsif 
            shadeControlType == "AlwaysOff"
            shadeControlSetpoint = 10000000000
          end

          print_statement("Processing Window Group '#{windowGroup}', (exterior normal: '#{wg_normal_x * -1} #{wg_normal_y * -1} #{wg_normal_z * -1}', shade control setpoint: #{shadeControlSetpoint.round(0)} lux)", runner)

          ill0, header = read_illuminance_file(wgIllumFiles[0], runner)
          ill1, _header = read_illuminance_file(wgIllumFiles[1], runner)        
  
          wgMerge = Matrix.build(ill0.row_count, ill0.column_count) { 0 }
          print_statement("### DEBUG: wgmerge is #{wgMerge.row_count} rows x #{wgMerge.column_count} columns", runner)

          wgShadeSchedule = []
          print_statement("### DEBUG: window group = '#{wg.split(",")[0]}', window controls matrix index = '#{wg_index-1}'", runner)
          windowControls.row(wg_index-1).each_with_index do | illuminance, row_index|

            window_illuminance = illuminance.to_f

            if window_illuminance < shadeControlSetpoint
              print_statement("### DEBUG: E(#{windowGroup}) is #{window_illuminance.round(0)} lux at index #{row_index}: STATE=0 (up/clear)", runner) if row_index > 152 && row_index < 160 # print shade decisions for one day

              ill0.column(row_index).each_with_index do |value, column_index| 
                wgMerge.send(:[]=, column_index, row_index, value)
              end

              wgShadeSchedule << "#{row_index},#{window_illuminance.round(0)},#{shadeControlSetpoint.round(0)},0\n"
            else
              print_statement("### DEBUG: E(#{windowGroup}) is #{window_illuminance.round(0)} lux at index #{row_index}: STATE=1 (dn/tinted)", runner) if row_index > 152 && row_index < 160 # print shade decisions for one day

              ill1.column(row_index).each_with_index do |value, column_index| 
                wgMerge.send(:[]=, column_index, row_index, value.to_f)
              end

              wgShadeSchedule << "#{row_index},#{window_illuminance.round(0)},#{shadeControlSetpoint.round(0)},1\n"
            end
          end

          wgIllum = File.open("output/ts/m_#{windowGroup}.ill", "w")
          wgShade = File.open("output/ts/#{windowGroup}.shd", "w")
          header.each {|head| wgIllum.print "#{head}"}
          wgMerge.to_a.each {|array_ts| wgIllum.print " #{array_ts.join(" ")}\n"} # note leading space, for compatibility with default rfluxmtx output
          wgShadeSchedule.each {|sh| wgShade.print "#{sh}"}
          wgIllum.close
          wgShade.close
          FileUtils.rm Dir.glob('*.tmp')
  
        end

      end

      # make whole-building illuminance file

      print_statement("Creating whole-building daylight results file...", runner)

      # get the controlled window group results (m_*.ill), if any
      mergeWindows = Dir.glob("output/ts/m_*.ill")
      if mergeWindows.size > 0
        print_statement("Gathering shade-controlled window group results (#{mergeWindows.size} total)", runner)
      else
        print_statement("INFO: Model has 0 controlled window groups", runner)
      end

      # get the uncontrolled windows results, if any
      if File.exist?("output/ts/WG0.ill")
        mergeWindows.insert(0, "output/ts/WG0.ill")
      else
        print_statement("INFO: Model has no uncontrolled windows.", runner)
      end

      if mergeWindows.size == 0
        print_statement("ERROR: no illuminance results.", runner)
        exit false
      elsif mergeWindows.size == 1
        # go straight to final building results file format
        while merge_count() > 2
          puts "waiting in rmtxop queue..."
          sleep(5)
        end        
        print_statement("Finalizing output...", runner)
        exec_statement("rmtxop -fa #{mergeWindows[0]} -t | getinfo - > output/merged_space.ill", runner)
      else
        # make initial building results from first window group
        while merge_count() > 2
          puts "waiting in rmtxop queue..."
          sleep(5)
        end
        print_statement("Starting final building illumimance file with #{mergeWindows[0]}...", runner)
        exec_statement("rmtxop -fa #{mergeWindows[0]} -t > output/final_merge.tmp", runner)
        # add remaining groups, one at a time
        mergeWindows[1..-1].each do |merge|
          print_statement("adding #{merge}...", runner)
          temp_fname = rand(36**15).to_s(36)
          while merge_count() > 2
            puts "waiting in rmtxop queue..."
            sleep(5)
          end
          exec_statement("rmtxop -fa output/final_merge.tmp + #{merge} -t > #{temp_fname}", runner)
          FileUtils.mv temp_fname, 'output/final_merge.tmp'
        end
        # strip header
        while merge_count() > 2
          puts "waiting in rmtxop queue..."
          sleep(5)
        end
        print_statement("Finalizing output...", runner)
        exec_statement("rmtxop -fa output/final_merge.tmp -t | getinfo - > output/merged_space.ill", runner)
        FileUtils.rm 'output/final_merge.tmp'
        print_statement("Done.", runner)
      end

      ## window merge end

      rawValues = parseResults(simulations, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_radPath, runner, debug_mode)

      dcVectors = nil

      # for each environment period (design days, annual, or arbitrary) you will create a directory for results
      t_sqlFile.availableEnvPeriods.each do |envPeriod|

        # DLM: all of these might be available directly from the EpwFile after Jason DeGraw's work
        diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits = getTimeSeries(t_sqlFile, envPeriod)

        # check that we have all timeseries
        if (not diffHorizIllumAll) or (not dirNormIllumAll) or (not diffEfficacyAll) or (not dirNormEfficacyAll) or (not solarAltitudeAll) or (not solarAzimuthAll)
          runner.registerError('Missing required timeseries')
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
    def parseResults(t_cmds, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_radPath, runner, debug_mode)

      print_statement("Parsing daylighting results", runner)

      allValues = []
      values = []

      # read illuminance values from file
      values = []
      valuesFile = File.open("#{t_radPath}/output/merged_space.ill")
      valuesFile.each do |row|
        values << row.split(" ")
      end

      allhours = []

      # write out illuminance to individual space/map files
      8760.times do |hour|
        index = 0;
        splitvalues = {}

        t_space_names_to_calculate.each do |space_name|
          space_size = t_spaceWidths[space_name] * t_spaceHeights[space_name]
          space = []
          illum = []
          glaresensors ||= {} # TODO: you can probably remove this
          glaresensors[space_name] ||= {}
          

          if values.size > 0
            subspace = values.slice(index, space_size)
            index = index + space_size
            
            print_statement("### DEBUG: starting illuminance map for '#{space_name}'. space_size: #{space_size}, index is now at: #{index}, ", runner) if hour == 0 if debug_mode
            
            space = []
            subspace.each do |subspacevalue|
              space << subspacevalue[hour].to_f.round(1);
            end

            if File.exist?("#{t_radPath}/numeric/#{space_name}.sns")        
              if index >= values.size
                print_statement("Index is #{index} but values.size is only #{values.size}", runner)
              elsif hour >= values[index].size
                print_statement("Hour is #{hour} but values.size[index] is only #{values[index].size}", runner)
              end
              illum = [values[index][hour].to_f.round(1)]
              index = index + 1
              print_statement("### DEBUG: finished space map and daylight sensor values, and index is now: #{index}", runner) if hour == 0 && debug_mode
            end
            
            # get ALL glare sensors for space       
            if t_radGlareSensorViews[space_name] && t_radGlareSensorViews[space_name].keys.size > 0
              t_radGlareSensorViews[space_name].each do |sensor, views|
                sensor_index = t_radGlareSensorViews[space_name].keys.index(sensor)
                
                print_statement("### DEBUG: glare sensor '#{sensor}' has #{views.size} views", runner) if hour == 0 && debug_mode

                views['view_definitions'].each_index do |view_index|
                  print_statement("### DEBUG: index is #{index}; view_index is #{view_index}", runner) if hour == 0 && debug_mode         
                  t_radGlareSensorViews[space_name][sensor][hour] ||= {}
                  t_radGlareSensorViews[space_name][sensor][hour]["#{sensor_index}_#{view_index}"] ||= {}
                  view_values = values.slice(index, 1).first                     
                  
                  adjustedval = 0.00
                  if view_values[hour].to_f != 0.00
                    adjustedval = [(0.0000622*view_values[hour].to_f)+0.184, 0].max.round(2)
                  end
                  t_radGlareSensorViews[space_name][sensor][hour]["#{sensor_index}_#{view_index}"]['dgp'] = adjustedval.round(2)                  
                  t_radGlareSensorViews[space_name][sensor][hour]["#{sensor_index}_#{view_index}"]['raw'] = view_values[hour].to_f.round(2)                                   

                  index += 1                 
                end
              end
            end
          else
            print_statement("An error has occurred; no results for space '#{space_name}'.", runner)
            space = Array.new(space_size, 0)

            if File.exist?("#{t_radPath}/numeric/#{space_name}.sns")        
              illum = Array.new(1, 0)
            end
   
          end

          # make an array that will have all the views
          splitvalues[space_name] = [space, illum]
          # iterate over each sensor and combine the views together
          new_hash = {}
          
          if t_radGlareSensorViews[space_name]
            t_radGlareSensorViews[space_name].each do |sensor, v|
              new_hash[sensor] = v[hour]
            end
          end
          splitvalues[space_name] += [new_hash]
          
        end

        allhours[hour] = splitvalues;
      end
      
      allhours
      
      File.open('output/glare.json', 'w') { |f| f << JSON.pretty_generate(t_radGlareSensorViews)}
      File.open('output/radout.json', 'w') { |f| f << JSON.pretty_generate( { all_hours: allhours } )}

      print_statement("Returning annual results", runner)
      return allhours
    
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
          print_statement("Error inserting data: " + illum[n] + " inserting 0 instead", runner);
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
      t_radGlareSensorViews, t_simCores, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_outPath, t_building, t_values, t_dcVectors, runner)
      sqlOutPath = OpenStudio::Path.new("#{Dir.pwd}/output/radout.sql")
      if OpenStudio::exists(sqlOutPath)
        OpenStudio::remove(sqlOutPath)
      end

      # for each environment period (design days, annual, or arbitrary) you will create a directory for results
      t_sqlFile.availableEnvPeriods.each do |envPeriod|

        print_statement("envPeriod = '" + envPeriod.to_s + "'", runner)

        diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits = getTimeSeries(t_sqlFile, envPeriod)

        # check that we have all timeseries
        if (not diffHorizIllumAll) or (not dirNormIllumAll) or (not diffEfficacyAll) or (not dirNormEfficacyAll) or (not solarAltitudeAll) or (not solarAzimuthAll)
          runner.registerError("Missing required timeseries")
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

          print_statement("Processing space '#{space_name}'", runner)

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

            # Split up values by space

            illumValues, illumSensorValues, glareSensorValues = t_values[i][space_name]
            
            # Debug
            # File.open('glareSensorValues.out', 'w') { |f| f.write(glareSensorValues.to_s) }

            timeSeriesIllum[i] = tsDateTime.to_s.tr(" ",",") + "," + "#{dirNormIllum[i]},#{diffHorizIllum[i]}," + illumSensorValues.join(',') + "," + illumValues.join(',')

            # add glare sensor values           
            if t_radGlareSensorViews[space_name]
              if not glareSensorValues.nil?
                timeSeriesGlare[i] = tsDateTime.to_s.tr(" ",",")
                glareSensorValues.each_key do |key|
                  glare_values = glareSensorValues[key].map{ |_, v| v['dgp']}
                  timeSeriesGlare[i] += ",#{key},#{glare_values.average.round(2)},#{glare_values.min.round(2)},#{glare_values.max.round(2)},raw,#{glare_values.join(',')}"
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
          
          FileUtils.mkdir_p("#{Dir.pwd}/output/ts/#{space_name}/maps") unless File.exist?("#{Dir.pwd}/output/ts/#{space_name}/maps")
          f = File.open("#{Dir.pwd}/output/ts/#{space_name}/maps/#{space_name}_map.ill", "w")
          space = nil
          t_building.spaces.each do |s|
            this_name = s.name.get.tr(' ', '_').tr(':', '_')
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

            print_statement("Writing Radiance results file", runner)

            # illuminance to csv
            
            f.print "## OpenStudio Daylight Simulation Results file\n"
            f.print "## Header: xmin ymin z xmax ymin z xmax ymax z xspacing yspacing\n"
            f.print "## Data: month,day,time,directNormalIllumimance(external),diffuseHorizontalIlluminance(external),daylightSensorIlluminance,pointIlluminance [lux]\n"
            f.print "#{xmin} #{ymin} #{z} #{xmax} #{ymin} #{z} #{xmax} #{ymax} #{z} #{xSpacing} #{ySpacing}\n"
            timeSeriesIllum.each {|ts| f.print "#{ts}\n"}
            f.close

            # glare to csv 
            
            FileUtils.mkdir_p("#{Dir.pwd}/output/ts/#{space_name}/maps") unless File.exist?("#{Dir.pwd}/output/ts/#{space_name}/maps")
            f = File.open("#{Dir.pwd}/output/ts/#{space_name}/maps/#{space_name}.glr", "w")
            space = nil
            t_building.spaces.each do |s|
              this_name = s.name.get.tr(' ', '_').tr(':', '_')
              if this_name == space_name
                space = s
                break
              end
            end

            if t_radGlareSensorViews[space_name]
              f.print "## OpenStudio Daylight Simulation (glare) Results file\n"
              f.print "## Space name: '#{space_name}\n"
              f.print "## Data: month,day,time,sensor_name,DGPs(avg),DGPs(min),DGPs(max),raw,[raw values]...\n"
              timeSeriesGlare.each {|ts| f.print "#{ts}\n"}
              f.close
            end

            # all results to sql      

            print_statement("Writing Radiance results database", runner)

            writeTimeSeriesToSql(sqlOutFile, simDateTimes, dirNormIllum, space_name, "Direct Normal Illuminance", "lux")
            writeTimeSeriesToSql(sqlOutFile, simDateTimes, diffHorizIllum, space_name, "Global Horizontal Illuminance", "lux")
            writeTimeSeriesToSql(sqlOutFile, simDateTimes, daylightSensorIlluminance, space_name, "Daylight Sensor Illuminance", "lux")
            writeTimeSeriesToSql(sqlOutFile, simDateTimes, meanIlluminanceMap, space_name, "Mean Illuminance Map", "lux")

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

        sqlOutFile.createIndexes

      end
    end # annualSimulation()

    # makeSchedules()

    # write new lighting power schedules for the model
    
    def makeSchedules(model, sqlFile, runner)
    
      print_statement("Updating lighting load schedules", runner)
    
      # only run period in pre process job
      environmentName = "Run Period 1"

      # loop through each thermal zone
      model.getThermalZones.each do |thermalZone|

        spaces = thermalZone.spaces

        if spaces.empty?
          print_statement("ThermalZone '#{thermalZone.name}' has no spaces, skipping.", runner)
          next
        end
  
        # get people schedule for zone
        # TODO: require people for occupancy controls
        peopleTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone People Occupant Count", thermalZone.name.get.upcase)
  
        if peopleTimeseries.empty?
          print_statement("Cannot find timeseries 'Zone People Occupant Count' for ThermalZone '#{thermalZone.name}'.", runner)
        end
  
        # get lights schedule for zone
        lightsTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Zone Lights Electric Power", thermalZone.name.get.upcase)
  
        if lightsTimeseries.empty?
          newname = thermalZone.name.get.sub(/^OS:/, '');
          print_statement("Cannot find timeseries 'Zone Lights Electric Power' for ThermalZone '#{thermalZone.name}', skipping.", runner)
          next
        end
  
        lightsTimeseries = lightsTimeseries.get
   
        # get illuminance map
        illuminanceMap = thermalZone.illuminanceMap

        if illuminanceMap.empty?
          print_statement("Cannot find IlluminanceMap for ThermalZone '#{thermalZone.name}', skipping.", runner)
          next
        end

        illuminanceMap = illuminanceMap.get

        # get the space
        space = illuminanceMap.space
  
        if space.empty?
          print_statement("Cannot find Space for IlluminanceMap '#{illuminanceMap.name}' in ThermalZone '#{thermalZone.name}', skipping.", runner)
          next
        end

        space = space.get

        space_name = space.name.get.tr(' ', '_').tr(':', '_')

        radSqlPath = OpenStudio::Path.new("output/radout.sql")

        # load the illuminance map
        # assume this will be reported in 1 hour timesteps starting on 1/1
        averageIlluminances = []
        radSqlFile = OpenStudio::SqlFile.new(radSqlPath)

          # use the daylight sensor input
          spacename = space.name.get.tr(' ', '_').tr(':', '_')
          envPeriods = radSqlFile.availableEnvPeriods

          if envPeriods.size == 0
            print_statement("No available environment periods in radiance sql file, skipping", runner)
            next
          end

          daylightSensor = radSqlFile.timeSeries(envPeriods[0], "Hourly", "Daylight Sensor Illuminance", space_name)
    
          if daylightSensor.empty?
            print_statement("Daylight sensor data could not be loaded, skipping", runner)
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
        if !primaryDaylightingControl.empty?
          daylightSetpoint = primaryDaylightingControl.get.illuminanceSetpoint
        end

          secondaryDaylightingControl = thermalZone.secondaryDaylightingControl
          if !secondaryDaylightingControl.empty?
            if daylightSetpoint == 0.0
              daylightSetpoint = secondaryDaylightingControl.get.illuminanceSetpoint
            else
              print_statement("Ignoring secondary daylighting control in ThermalZone '#{thermalZone.name}'", runner)
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
          print_statement("Illuminance setpoint is not defined in Space '#{space.name}' or in ThermalZone '#{thermalZone.name}', skipping.", runner)
          next
        end
  
        print_statement("ThermalZone '#{thermalZone.name}' illuminance setpoint is: #{daylightSetpoint.round(0)} lux", runner)
  
        originalLightsValues = lightsTimeseries.values
        lightsValues = OpenStudio::Vector.new(averageIlluminances.size)
        averageIlluminances.each_index do |i|
          dimmingResponse = [(daylightSetpoint-averageIlluminances[i])/daylightSetpoint, 0].max
          lightsValues[i] = dimmingResponse*originalLightsValues[i]

        end
  
        # get max lighting power
        lightingLevel = OpenStudio::maximum(lightsValues)
  
        if lightingLevel <= 0.0
          print_statement("Thermal Zone '#{thermalZone.name}' lighting power is less than or equal to 0, skipping", runner)
          next
        end
  
        print_statement("Thermal Zone '#{thermalZone.name}' lighting power is: #{lightingLevel.round(0)} W", runner)
  
        # normalize lights values
        averageIlluminances.each_index do |i|
          lightsValues[i] = lightsValues[i]/lightingLevel
        end
  
        startDate = OpenStudio::Date.new(OpenStudio::MonthOfYear.new(1), 1)
        interval = OpenStudio::Time.new(0,1,0)
        timeseries = OpenStudio::TimeSeries.new(startDate, interval, lightsValues, "W")

        schedule = OpenStudio::Model::ScheduleInterval::fromTimeSeries(timeseries, model)
  
        if schedule.empty?
          print_statement("Could not create modified lighting schedule for Thermal Zone '#{thermalZone.name}', skipping", runner)
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


    def daylightMetrics(model, sqlFile, runner)
    
      # load the Radiance output data

      radoutPath = OpenStudio::Path.new("output/radout.sql")
      radoutPath = OpenStudio::system_complete(radoutPath)

      radoutFile = OpenStudio::SqlFile.new(radoutPath)
      if not sqlFile.connectionOpen
        print_statement("SqlFile #{sqlPath} connection is not open", runner)
        return false
      end 
    
      $METHOD = 1

      # get exterior illuminance timeseries from E+ run
      exteriorIlluminanceTimeseries = sqlFile.timeSeries("Run Period 1".upcase, "Hourly", "Site Exterior Horizontal Sky Illuminance")

      # summary report string
      summary_report = ""

      building_average_space = []
      building_average = ""
      building_average_space_cda = []
      building_average_cda = ""
      building_average_space_udi = []
      building_average_udi = ""

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
      
        space_name = space.name.get.tr(' ', '_').tr(':', '_')
  
        thermalZone = space.thermalZone
        next if thermalZone.empty?
        thermalZone = thermalZone.get
  
        map_name = "#{space_name} DAYLIGHT MAP"
        map_index = radoutFile.illuminanceMapIndex(map_name)
        next if map_index.empty?
  
        daylightSetpoint = nil
        primaryDaylightingControl = thermalZone.primaryDaylightingControl
        if primaryDaylightingControl.empty?
          print_statement("Thermal Zone \"#{thermalZone}\" has no primary daylighting control, skipping", runner)
          next
        else
          daylightSetpoint = primaryDaylightingControl.get.illuminanceSetpoint
        end
  
        print_statement("Calculating Daylight Metrics for Space '#{space_name}'", runner)
  
        da_daylit = []
        da_occupied = []
        da_daylit_occupied = []
        cda_daylit = []
        cda_occupied = []
        cda_daylit_occupied = []
        udi_daylit = []
        udi_occupied = []
        udi_daylit_occupied = []
        sda_credit = []

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
          sda_credit << nil

          # extract timestep and index
          hourly_report_index = hourly_report_index_date.first
          hourly_report_date = hourly_report_index_date.second

          # SDA credit hour? (using 8:00-17:00 as the qualifying range, to comply with the 10 hour/day, 3,650 annual hours expectation)
          sda_hour = false
          if hourly_report_date.to_s.split(' ')[1].split(':')[0].to_i >= 8 && hourly_report_date.to_s.split(' ')[1].split(':')[0].to_i <= 17
            sda_hour = true
          end
          
          # daylit hour?
          daylit_hour = false
          if not exteriorIlluminanceTimeseries.empty?
            val = exteriorIlluminanceTimeseries[0].value(hourly_report_date)
            if val > 0
              daylit_hour = true
            end
          end
    
          # occupied hour?
          occupied_hour = false
          if !peopleTimeseries.empty?
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

            # compute DA, conDA, UDI, and SDA
            num = map_values.size
            num_da = 0
            num_cda = 0
            num_udi = 0
            num_sda = 0
            map_values.each do |map_value|
              if map_value >= daylightSetpoint
                num_da += 1
                num_cda += 1
              end
              if map_value > 0 && map_value < daylightSetpoint  
                num_cda += map_value / daylightSetpoint 
              end
              if map_value >= 100 && map_value <= 3000
                num_udi += 1
              end
              if map_value >= 300 && sda_hour == true
                num_sda += 1
              end
            end
      
            da = num_da.to_f / num.to_f
            cda = num_cda.to_f / num.to_f
            udi = num_udi.to_f / num.to_f
            sda = num_sda.to_f / num.to_f

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
          
          if sda_hour
            sda_credit[-1] = sda
          end

        end

        # compute annual metrics for space

        #Daylight Autonomy
        da_daylit_sum = 0
        da_daylit_num = 0
        da_daylit.each do |da|
          if !da.nil?
            da_daylit_sum += da
            da_daylit_num += 1
          end
        end
        annual_da_daylit = da_daylit_sum.to_f / da_daylit_num.to_f
        summary_report += "#{space_name},DA(#{daylightSetpoint.round(0)}),Daylit Hours,#{annual_da_daylit.round(2)},#{da_daylit_sum.round(0)},#{da_daylit_num}\n"

        da_occupied_sum = 0
        da_occupied_num = 0
        da_occupied.each do |da|
          if !da.nil?
            da_occupied_sum += da
            da_occupied_num += 1
          end
        end
        annual_da_occupied = da_occupied_sum.to_f / da_occupied_num.to_f
        summary_report += "#{space_name},DA(#{daylightSetpoint.round(0)}),Occupied Hours,#{annual_da_occupied.round(2)},#{da_occupied_sum.round(0)},#{da_occupied_num}\n"

        da_daylit_occupied_sum = 0
        da_daylit_occupied_num = 0
        da_daylit_occupied.each do |da|
          if !da.nil?
            da_daylit_occupied_sum += da
            da_daylit_occupied_num += 1
          end
        end
        annual_da_daylit_occupied = da_daylit_occupied_sum.to_f / da_daylit_occupied_num.to_f
        summary_report += "#{space_name},DA(#{daylightSetpoint.round(0)}),Daylit and Occupied Hours,#{annual_da_daylit_occupied.round(2)},#{da_daylit_occupied_sum.round(0)},#{da_daylit_occupied_num}\n"

        # Continuous Daylight Autonomy
        cda_daylit_sum = 0
        cda_daylit_num = 0
        cda_daylit.each do |cda|
          if !cda.nil?
            cda_daylit_sum += cda
            cda_daylit_num += 1
          end
        end
        annual_cda_daylit = cda_daylit_sum.to_f / cda_daylit_num.to_f
        summary_report += "#{space_name},conDA(#{daylightSetpoint.round(0)}),Daylit Hours,#{annual_cda_daylit.round(2)},#{cda_daylit_sum.round(0)},#{cda_daylit_num}\n"

        cda_occupied_sum = 0
        cda_occupied_num = 0
        cda_occupied.each do |cda|
          if !cda.nil?
            cda_occupied_sum += cda
            cda_occupied_num += 1
          end
        end
        annual_cda_occupied = cda_occupied_sum.to_f / cda_occupied_num.to_f
        summary_report += "#{space_name},conDA(#{daylightSetpoint.round(0)}),Occupied Hours,#{annual_cda_occupied.round(2)},#{cda_occupied_sum.round(0)},#{cda_occupied_num}\n"

        cda_daylit_occupied_sum = 0
        cda_daylit_occupied_num = 0
        cda_daylit_occupied.each do |cda|
          if !cda.nil?
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
          if !udi.nil?
            udi_daylit_sum += udi
            udi_daylit_num += 1
          end
        end
        annual_udi_daylit = udi_daylit_sum.to_f / udi_daylit_num.to_f
        summary_report += "#{space_name},UDI(100-3000),Daylit Hours,#{annual_udi_daylit.round(2)},#{udi_daylit_sum.round(0)},#{udi_daylit_num}\n"

        udi_occupied_sum = 0
        udi_occupied_num = 0
        udi_occupied.each do |udi|
          if !udi.nil?
            udi_occupied_sum += udi
            udi_occupied_num += 1
          end
        end
        annual_udi_occupied = udi_occupied_sum.to_f / udi_occupied_num.to_f
        summary_report += "#{space_name},UDI(100-3000),Occupied Hours,#{annual_udi_occupied.round(2)},#{udi_occupied_sum.round(0)},#{udi_occupied_num}\n"

        udi_daylit_occupied_sum = 0
        udi_daylit_occupied_num = 0
        udi_daylit_occupied.each do |udi|
          if !udi.nil?
            udi_daylit_occupied_sum += udi
            udi_daylit_occupied_num += 1
          end
        end
        annual_udi_daylit_occupied = udi_daylit_occupied_sum.to_f / udi_daylit_occupied_num.to_f
        summary_report += "#{space_name},UDI(100-3000),Daylit and Occupied Hours,#{annual_udi_daylit_occupied.round(2)},#{cda_daylit_occupied_sum.round(0)},#{cda_daylit_occupied_num}\n"

        # Spatial Daylight Autonomy (FWIW)
        sda_sum = 0
        sda_num = 0
        sda_credit.each do |sda|
          if !sda.nil?
            sda_sum += sda
            sda_num += 1
          end
        end
        annual_sda = sda_sum.to_f / sda_num.to_f
        summary_report += "#{space_name},sDA(300),8AM-5PM (10 hours/day per IESNA LM-83-12),#{annual_sda.round(2)},#{sda_sum.round(0)},#{sda_num}\n"
        
        # Make building average metrics
        
        # DA
        building_average_space << annual_da_daylit_occupied
        # cDA
        building_average_space_cda << annual_cda_daylit_occupied
        # UDI
        building_average_space_udi << annual_udi_daylit_occupied

      end

      # DLM: can we make some more metrics that are area weighted rather than just space weighted?
      building_average_space_sum = 0.0
      building_average_space.each {|e| building_average_space_sum += e}
      building_average_space_cda_sum = 0.0
      building_average_space_cda.each {|e| building_average_space_cda_sum += e}
      building_average_space_udi_sum = 0.0
      building_average_space_udi.each {|e| building_average_space_udi_sum += e}

      # catch zero condition
      if building_average_space_sum == 0.0
        building_average = 0.0
        print_statement("Warning: Daylight Autonomy for building is zero, check daylighting control point(s) setup.", runner)
      else
        building_average = building_average_space_sum / building_average_space.length
        runner.registerValue("DA", building_average.round(2))
        building_average_cda = building_average_space_cda_sum / building_average_space_cda.length
        runner.registerValue("cDA", building_average_cda.round(2))
        building_average_udi = building_average_space_udi_sum / building_average_space_udi.length
        runner.registerValue("UDI", building_average_udi.round(2))        
      end

      File.open("output/daylight_metrics.csv", 'w') do |file|
        file.puts "# OpenStudio Daylight Metrics Report"
        file.puts "# Building average daylight metrics (daylit spaces): DA = #{building_average.round(2)} cDA = #{building_average_cda.round(2)} UDI = #{building_average_udi.round(2)}"
        file.puts "#[space_name],[metric(setpoint)],[input_hours_range],[metric_value],[hours_met],[input_hours]"       
        file.puts summary_report
      end

    end # daylightMetrics()

    def genImages(radPath, runner, site_latitude, site_longitude, site_meridian, catCommand, debug_mode)

      print_statement("Generating images", runner)
      
      # generate some equinox skies for renderings, so we have them. 
      ['09','12','15'].each do |hour|
        exec_statement("gensky 03 21 #{hour} -a #{site_latitude} -o #{site_longitude} -m #{site_meridian} +s > skies/render_sky_input", runner)
        File.open("skies/render_sky_skyfuncs", 'w') do |file|            
          # blue sky
          file.puts "skyfunc glow skyglow\n0\n0\n4 0.900 0.900 1.150 0\n\n"
          file.puts "skyglow source sky\n0\n0\n4 0 0 1 180\n\n"
          # brown ground
          file.puts "skyfunc glow groundglow\n0\n0\n4 1.400 0.900 0.600 0\n\n"
          file.puts "groundglow source ground\n0\n0\n4 0 0 -1 180\n\n"
        end   
        exec_statement("#{catCommand} skies/render_sky_input skies/render_sky_skyfuncs > skies/site_0321_#{hour}.sky", runner)
      end
      
      image_hour = "09" # preset hour
      # make octree
      rad_command = "oconv materials/materials.rad model.rad skies/site_0321_#{image_hour}.sky > octrees/images.oct"
      exec_statement(rad_command, runner)
      
      # do views
      
      # daylighting control views, unfiltered
      views_daylighting_control = Dir.glob('views/*_dc.vfh')
      views_daylighting_control.each do |dc|
        
        rad_command = "rpict -av .3 .3 .3 -ab 1 -vf #{dc} octrees/images.oct | ra_bmp - #{dc}_#{image_hour}.bmp"
        exec_statement(rad_command, runner)
        
        if debug_mode
        
          # do "debug" images (individual window groups)
          debug_images = Dir.glob('octrees/debug*.oct')
          debug_images.each do |debug|
            condition = debug.split("/")[1].split(".")[0]
            exec_statement("oconv -i #{debug} skies/site_0321_#{image_hour}.sky > octrees/debug_temp.oct", runner)       
            exec_statement("rpict -av .3 .3 .3 -ab 1 -vf #{dc} octrees/debug_temp.oct | ra_bmp - #{dc}_#{condition}_#{image_hour}_DEBUG.bmp", runner)
          end
      
        end # debug images
      
      end
      
      # glare sensor views are tonemapped
      views_glare_sensor = Dir.glob('views/*_gs.vfv')
      views_glare_sensor.each do |gv|
      
        rad_command = "rpict -av .3 .3 .3 -ab 1 -vf #{gv} octrees/images.oct > temp.hdr"
        exec_statement(rad_command, runner)
        rad_command = "pcond -h temp.hdr | ra_bmp - #{gv}.bmp"
        exec_statement(rad_command, runner)
        FileUtils.rm('temp.hdr')

      end
    
    end #genImages()

    ## ## ## ## ## ##

    # actually do the thing
    
    sqlOutFile = ""
    radoutFile = ""
    
    # settle in, it's gonna be a bumpy ride...
    Dir.chdir("#{radPath}")
    print_statement("Radiance working directory: '#{Dir.pwd}'", runner)

    epwFile = OpenStudio::OptionalEpwFile.new(OpenStudio::EpwFile.new(epw_path))

    sqlPath = OpenStudio::Path.new("sql/eplusout.sql")
    sqlPath = OpenStudio::system_complete(sqlPath)

    # load the sql file
    sqlFile = OpenStudio::SqlFile.new(sqlPath)
    if !sqlFile.connectionOpen
      runner.registerError("SqlFile #{sqlPath} connection is not open")
      return false
    end

    # set the sql file
    model.setSqlFile(sqlFile)
    if model.sqlFile.empty?
      runner.registerError("Model's SqlFile is not initialized")
      return false
    end

    # get the top level simulation object
    simulation = model.getSimulationControl

    sqlPath = OpenStudio::Path.new("sql/eplusout.sql")
    sqlPath = OpenStudio::system_complete(sqlPath)
                    
    # reduce/convert epw data to Daysim-style ".wea" input format
    exec_statement("epw2wea \"#{epw_path}\" wx/in.wea", runner)

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

    # create space geometry, hash of space name to file contents
    radSpaces = {}
    radSensors = {}
    radGlareSensorViews = {}
    radMaps = {}
    radMapHandles = {}
    radMapPoints = {}
    radViewPoints = {}
    radDaylightingControls = {}
    radDaylightingControlPoints = {}
    spaceWidths = {}
    spaceHeights = {}

    # loop through the model
    space_names = []

    building.spaces.each do |space|
      space_name = space.name.get.tr(' ', '_').tr(':', '_')
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
      print_statement("### DEBUG: there are #{space.glareSensors.size} glare sensors in this space ('#{space_name}')", runner) if debug_mode
      space.glareSensors.each do |sensor|
        tmp_sensor_name = sensor.name.get.tr(' ', '_').tr(':', '_')
        radGlareSensorViews[space_name] ||= {}
        radGlareSensorViews[space_name][tmp_sensor_name] ||= {}
        radGlareSensorViews[space_name][tmp_sensor_name]['view_definitions'] = OpenStudio::Radiance::RadianceForwardTranslator.getViewVectors(sensor)
        
        print_statement("### DEBUG: glare sensor '#{tmp_sensor_name}' has #{OpenStudio::Radiance::RadianceForwardTranslator::getViewVectors(sensor).size} views.", runner) if debug_mode      
      end
      
    end

    space_names_to_calculate = []

    # only do spaces with illuminance maps
    space_names_to_calculate = []
    space_names.each do |space_name|
      if !radMaps[space_name].nil?
        space_names_to_calculate << space_name
      end
    end

    # merge window group control points
    window_groups = Dir.glob('numeric/WG*.pts')
    if window_groups.size > 0
      File.open("numeric/window_controls.map", 'w') do |f|
        windows = Dir.glob("numeric/WG*.pts").sort
        windows.each do |wg|
          f.write IO.read(wg)
        end
      end
    end
    
    # merge calculation points
    File.open("numeric/merged_space.map", 'w') do |f|
      space_names_to_calculate.each do |space_name|
        f.write IO.read("numeric/#{space_name}.map")
        if File.exist?("numeric/#{space_name}.sns")
          f.write IO.read("numeric/#{space_name}.sns")
        end
        glare_sensors = Dir.glob("numeric/#{space_name}*.glr").sort
        if glare_sensors.size > 0
          glare_sensors.each do |sensor|
            print_statement("added glare sensor '#{sensor}' to calculation points", runner)
            f.write IO.read(sensor)
          end
        end
      end
    end

    print_statement("Warning: 'Cleanup data' option was selected; will delete ancilary Radiance data and all Radiance input files, post-simulation.",runner) if debug_mode && cleanup_data

    # get the daylight coefficient matrices
    calculateDaylightCoeffecients(radPath, sim_cores, catCommand, options_tregVars, 
    options_klemsDensity, options_skyvecDensity, options_dmx, options_vmx, rad_settings, procsUsed, runner, debug_mode)

    # make merged building-wide illuminance schedule(s)
    values, dcVectors = runSimulation(space_names_to_calculate, sqlFile, sim_cores, 
    options_skyvecDensity, site_latitude, site_longitude, site_meridian, radPath, spaceWidths,
    spaceHeights, radGlareSensorViews, runner, debug_mode)

    # make space-level illuminance schedules and radout.sql results database
    # hoping this is no longer necessary...
    annualSimulation(sqlFile, epwFile, space_names_to_calculate, radMaps, spaceWidths, spaceHeights,
      radMapPoints, radGlareSensorViews, sim_cores, site_latitude, site_longitude,
      site_meridian, radPath, building, values, dcVectors, runner)

    # make new lighting power schedules based on Radiance daylight data
    makeSchedules(model, sqlFile, runner)

    # compute daylight metrics for model
    daylightMetrics(model, sqlFile, runner)

    # remove illuminance map and daylighting controls from model, so they are not re-simulated in E+
    model.getThermalZones.each do |thermalZone|
      thermalZone.resetPrimaryDaylightingControl
      thermalZone.resetSecondaryDaylightingControl
      thermalZone.resetIlluminanceMap
    end

    # make check images 
    genImages(radPath, runner, site_latitude, site_longitude, site_meridian, catCommand, debug_mode)

    # cleanup
    FileUtils.rm('annual-sky.mtx')
    unless debug_mode
      rm_list = "output/ts/m_*.ill", "output/ts/window_controls.ill", "output/ts/WG*.ill", "octrees/*.oct", "output/ts/*.shd"
      FileUtils.rm Dir.glob(rm_list)
    end
    if cleanup_data
      print_statement("Deleting most Radiance I/O to preserve disk space...", runner)
      print_statement("WARN: deleting debug files because 'Cleanup data' option was selected!",runner) if debug_mode
      clean_list = "bsdf", "materials", "numeric", "octrees", "options", "scene", "skies", "sql", "views", "wx", "output/dc", "output/ts"
      FileUtils.rm_rf(clean_list)
    end

    # report initial condition of model
    daylightAnalysisSpaces = []
    spaces = model.getSpaces
    spaces.each do |sp|
      if sp.illuminanceMaps.size > 0
        daylightAnalysisSpaces << sp
      end
    end
    runner.registerInitialCondition("Input building model contains #{model.getSpaces.size} spaces.")

    # report final condition of model
    runner.registerFinalCondition("Measure ran Radiance on the #{daylightAnalysisSpaces.size} spaces containing daylighting objects.")

    return true
  end # run
end

# register the measure
RadianceMeasure.new.registerWithApplication
