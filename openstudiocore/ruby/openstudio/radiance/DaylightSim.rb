######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
#  All rights reserved.
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

######################################################################
# == Synopsis
#
#   Take translated EnergyPlus building model and run Radiance daylight simulations
#
# == Usage
#
#  ruby DaylightSim.rb ARGV[0] (ARGV[2]) [options]
#
#  ARGV[0] - Path to OpenStudio Model
#
#  ARGV[1] - Path to EnergyPlus SQLite output.
#            EnergyPlus simulation must be pre-run with the SQLite output including tabular data
#            and the EnvelopeSummary table must be requested.  The "Exterior
#            Horizontal Illuminance From Sky", and "Exterior Beam Normal
#            Illuminance" output variables must be requested at hourly reporting
#            frequency.
#
#  ARGV[2] - Path to Radiance opts file, uses exec defaults if not provided.
#
# TODO: change to options parser, add control options
#   change code to read in model parameters
#   check to ensure valid model exists (i.e. ModelToRad.rb ran successfully)
#
# == Examples
#
#   ruby DaylightSim.rb 'C:\path\to\model.osm' 'C:\path\to\eplusout.sql' foo.opt --month 6,9,12 --day 21 
#
######################################################################

require 'openstudio'
require 'optparse'
require 'ostruct'
require 'fileutils'
require 'csv'
require 'tempfile'
require 'date'

class ParseOptions

  #
  # Return a structure describing the commandline options.
  #
  def self.parse(args)
    # The options specified on the command line will be collected in *options*.
    options = OpenStruct.new
    # We set some default values here.
    options.ts = "false"
    options.dc = "false"
    options.dcts = "false"
    options.x = "x"
    options.y = "false"
    options.dims = "400"
    options.verbose = ""

    opts = OptionParser.new do |opts|
      opts.banner = "Usage: DaylightSim.rb [options]"

      opts.separator ""
      opts.separator "Simulation variables:"

      # array option examples
      # Specify the month(s)
      opts.on("--month 06,09,12", Array, "Monthly argument(s)") do |simMonth|
        options.simMonth = simMonth
      end
      # Specify the day(s)
      opts.on("--day 21", Array, "Daily argument(s)") do |simDay|
        options.simDay = simDay
      end
      # Specify the hour(s)
      opts.on("--hour 08,12,16", Array, "Hourly argument(s)") do |simHour|
        options.simHour = simHour
      end
      # Specify the view(s)
      opts.on("--view front.vf,plan.vf", Array, "View argument(s)") do |simView|
        options.simView = simView
      end
      ## Specify the illuminance map(s)
      #opts.on("--map lobby.fld,room.fld", Array, "Illuminance Map argument(s)") do |simMap|
      #  options.simMap = simMap
      #end
      # Specify the space(s)
      opts.on("--spaces core,perimeter", Array, "Space argument(s)") do |simSpaces|
        options.simSpaces = simSpaces
      end
      # Specify the number of cores (for apps/OSen that use it)
      opts.on("--cores 8", "Number of cores to allocate") do |simCores|
        options.simCores = simCores
      end
      # Specify the maximum image dimension (pixels)
      opts.on("--dims 400", "Max image dimension") do |simDims|
        options.dims = simDims
      end
      # Boolean switch examples
      opts.separator ""
      opts.separator "Simulation type switches:"
      opts.on("--dc", "Generate daylight coefficients") do |dc|
        options.dc = dc
      end
      opts.on("--glare", "Boolean for requesting fisheye views of spaces for glare evaluation") do |glare|
        options.glare = glare
      end
      opts.on("--dcts", "Use daylight coefficients for timestep analysis") do |dcts|
        options.dcts = dcts
      end
      opts.on("--x", "Use single-phase method") do |x|
        options.x = x
        options.y = "false"
      end
      opts.on("--z", "Use three-phase DC method") do |z|
        options.x = ""
        options.z = true
      end
      opts.on("--ts", "Use Radiance classic for timestep analysis") do |ts|
        options.ts = ts
      end
      opts.separator ""
      opts.separator "Other options:"
      opts.on("-v", "--verbose", "Verbose (debug) mode") do |verbose|
        options.verbose = 'v'
      end

      # No argument, shows at tail.  This will print an options summary.
      opts.on_tail("-h", "--help", "Show this message") do
        puts opts
        exit
      end

    end

    opts.parse!(args)
    options
  end  # parse()

end  # class ParseOptions

modelPath = OpenStudio::Path.new(ARGV[0])
modelPath = OpenStudio::system_complete(modelPath)
outPath = modelPath.parent_path / OpenStudio::Path.new(modelPath.stem) / OpenStudio::Path.new("model") / OpenStudio::Path.new("radiance")

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

options = ParseOptions.parse(ARGV)


# Read simsettings from model export 2013.01.10 RPG 
# TODO: read settings directly from model
options.tregVars = ""
options.klemsDensity = ""
options.skyvecDensity = ""
options.dmx = ""
options.vmx = ""

File.open("#{outPath}/options/treg.opt", "r") do |file|
  tempIO = file.read
  tempSettings = tempIO.split(" ")
  options.klemsDensity = "#{tempSettings[0]} #{tempSettings[1]}"
  options.skyvecDensity = tempSettings[3].split(":")[1]
  options.tregVars = tempSettings[2..-1].join(" ")
end

File.open("#{outPath}/options/dmx.opt", "r") do |file|
  tempIO = file.read
  options.dmx = tempIO
end

File.open("#{outPath}/options/vmx.opt", "r") do |file|
  tempIO = file.read
  options.vmx = tempIO
end



if !which('perl')
  puts "Perl could not be found in path, exiting"
  exit 1
end

# set up MP option
coreCount = OpenStudio::System::numberOfProcessors
if not options.simCores.nil?
  if coreCount < options.simCores.to_i
    puts "WARN: system has #{coreCount} cores, but #{options.simCores} requested. will request #{coreCount}."
    simCores = coreCount
  else
    simCores = options.simCores
  end
elsif coreCount == 1
  simCores = 1
else
  simCores = coreCount - 1
end

if options.verbose == 'v'

  puts options
  puts "tregVars: #{options.tregVars}"
  puts "klemsDensity: #{options.klemsDensity}"
  puts "skyvecDensity: #{options.skyvecDensity}"
  puts "dmx: #{options.dmx}"
  puts "vmx: #{options.vmx}"

end

# utility function
# make float.round() sorta work in ruby v1.8 like it does in v1.9, enough for our purposes
class Numeric
  def round_to_str( decimals=0 )
    if decimals >= 0
      "%.#{decimals}f" % self
    else
      factor = 10**-decimals
      ((self/factor).round * factor).to_s
    end
  end
end

# print statement and execute as system call
def exec_statement(s)
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    s = s.gsub("/", "\\")
  end
  puts "'#{s}'"
  result = system(s)
  puts
  return result
end

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
  puts DateTime.now.to_s + " Ending timeseries write to sql"
end

def mergeSpaces(t_space_names_to_calculate, t_outPath)
  File.open("#{t_outPath}/numeric/merged_space.map", "w") do |f|
    t_space_names_to_calculate.each do |space_name|

      f.write IO.read("#{t_outPath}/numeric/#{space_name}.map")

      if File.exists?("#{t_outPath}/numeric/#{space_name}.sns")        
        f.write IO.read("#{t_outPath}/numeric/#{space_name}.sns")
      end


      if File.exists?("#{t_outPath}/numeric/#{space_name}.glr")
        f.write IO.read("#{t_outPath}/numeric/#{space_name}.glr")
      end
    end
  end
end



def calculateDaylightCoeffecients(t_outPath, t_options, t_space_names_to_calculate, t_radMaps, t_opts_map, t_simCores, t_catCommand)
  perlPrefix = ""
  perlExtension = ""

  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    perlPrefix = "perl \"C:/Program Files (x86)/Radiance/bin/"

    raypath = ENV['RAYPATH']
    raypath.split(';').each do |path|
      candidate = "#{path}\\..\\bin\\"
      if File.exists?("#{candidate}genskyvec.pl")
        perlPrefix = "perl \"#{candidate}"
        break
      end
    end

    perlExtension = ".pl\""
  end

  Dir.chdir("#{t_outPath}")
  FileUtils.mkdir_p("#{t_outPath}/output/dc") unless File.exists?("#{t_outPath}/output/dc")
  # generate sky/ground model
  radEnvSphere = ""
  # create generic 1w sky/ground sphere
  radEnvSphere = "void glow skyglow\n0\n0\n4\n1 1 1 0\n\nskyglow source sky\n0\n0\n4\n0 0 1 360"
  File.open("#{t_outPath}/skies/dc.sky", "w") do |file|
    file << radEnvSphere
  end
  puts "wrote sky: #{t_outPath}/skies/dc.sky"

  binDir = "#{t_outPath}/output/dc/merged_space/maps"
  FileUtils.mkdir_p("#{binDir}") unless File.exists?("#{binDir}")

  procsUsed = ""
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    puts "rcontrib does not support multiple cores on Windows"
    procsUsed = ""
  else
    puts "rcontrib using #{t_simCores} core(s)"
    procsUsed = "-n #{t_simCores}"
  end

  # "single phase" method
  if t_options.x == true
    rtrace_args = "#{t_options.vmx}"
    system("oconv \"#{t_outPath}/materials/materials.rad\" model.rad \"#{t_outPath}/skies/dc.sky\" > model_dc.oct")
    system("oconv \"#{t_outPath}/skies/dc.sky\" > model_dc_skyonly.oct")
    #compute illuminance map(s)
    if t_options.verbose == 'v'
      puts "computing daylight coefficients..."
      puts "computing daylight matrix ONLY (single phase)."
    end

    # do map
    exec_statement("#{t_catCommand} \"#{t_outPath}/numeric/merged_space.map\" | rcontrib #{rtrace_args} #{procsUsed} \
      -I+ -fo #{t_options.tregVars} -o \"#{t_outPath}/output/dc/merged_space/maps/merged_space.dmx\" -m skyglow model_dc.oct")

    if t_options.verbose == 'v'
      puts "daylight coefficients computed, stored in #{t_outPath}/output/dc/merged_space/maps"
    end

    # do control point views, if requested
    # NOTE: this needs work. RPG 2012.02.18
    if t_options.glare == true
      t_space_names_to_calculate.each do |space_name|
        rtrace_args = "#{opts_img}"
        # TODO: add support for saving views from OpenStudio and for specifying views here
        binDir="#{t_outPath}/output/dc/#{space_name}/views"
        Dir.mkdir("#{binDir}") unless File.exists?("#{binDir}")

        if t_options.verbose == 'v'
          puts "computing daylight coefficients for #{space_name}"
        end
        view_def = ""
        dims = t_options.dims
        view_def ="-x #{dims} -y #{dims} -vf #{Dir.pwd}/numeric/#{space_name}.sns"
        if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
          vwrays_out = `vwrays -d #{view_def}`.strip
          exec_statement("vwrays -ff #{view_def} | rcontrib #{rtrace_args = "#{t_options.vmx}"} -V- -fo -ffc #{vwrays_out} \
            -f #{t_options.tregVars} -o \"#{binDir}/#{space_name}_treg%03d.hdr\" -m skyglow model_dc.oct")
        else
          exec_statement("vwrays -ff #{view_def} | rcontrib #{t_options.vmx} -n #{t_simCores} -V- -fo -ffc \
            $(vwrays -d #{view_def}) -f #{t_options.tregVars} -o \"#{binDir}/#{space_name}treg%03d.hdr\" -m skyglow model_dc.oct")
          # create "contact sheet" of DC images for reference/troubleshooting
          if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
            hdrs = Dir.glob("#{binDir}/*.hdr")
            # there is a limit on the length of the command line on Windows systems -- just one of the OS's great many shortcomings.
            hdrs = hdrs[0..12]
            exec_statement("pcompos -a 10 #{hdrs.join(' ')} | pfilt -x /6 -y /6 | ra_bmp -e +2 - \"#{binDir}/#{space_name}contact-sheet.bmp\"")
          else
            exec_statement("pcompos -a 10 #{binDir}/*.hdr | pfilt -x /6 -y /6 | ra_tiff -e +2 -z - \"#{binDir}/#{space_name}contact-sheet.tif\"")
          end
        end
      end
    end
  end

  # 3-phase method
  if t_options.z == true
    puts "using three phase method (have you modified your 'mapping.txt' file?)..."
    binPairs = Array.new
    matrixGroups = Array.new
    aziVectorX = ""
    aziVectorY = ""
    puts "Using 3-phase method"
    #do three phase method (views not supported yet) RPG 2012.02.18
    system("oconv \"#{t_outPath}/materials/materials.rad\" \"#{t_outPath}/materials/materials_vmx.rad\" model.rad \"#{t_outPath}/skies/dc.sky\" > \"#{t_outPath}/octrees/model_dc.oct\"")
    #read DC materials file
    windowMaps = File::open("#{t_outPath}/bsdf/mapping.rad")
    # get materials, compute vectors
    # plan (2D) only for now
      #t_space_names_to_calculate.each do |space_name|
      windowMaps.each do |row|
          #row[0..-1].each do |azi|
          matchVmx = /glaz_(.*?)_azi-(.*?)_tn-(.*).vmx/.match(row)
          space_name = matchVmx[1]
          aziVector = matchVmx[2]
          glazingTransmissivity = matchVmx[3]
          aziAngle = aziVector.to_f * (180 / Math::PI)
          # swap sin and cos so we get vectors that align with compass headings (which makes sense to this idiot -->(RPG).)
          aziVectorX = Math::sin(aziVector)
          aziVectorY = Math::cos(aziVector)
          viewVectorX = -aziVectorX
          viewVectorY = -aziVectorY
          # sanity checks
          puts "window azimuth: #{aziAngle}"
          puts "daylight vector (window to sky): #{aziVectorX.round_to_str(2)},#{aziVectorY.round_to_str(2)},0.00"
          puts "view vector (window to interior): #{viewVectorX.round_to_str(2)},#{viewVectorY.round_to_str(2)},0.00"
          binPairs << " -b 'kbin(#{viewVectorX.round_to_str(2)},#{viewVectorY.round_to_str(2)},0,0,0,1)' -m glaz_#{space_name}_azi-#{aziVector}_tn-#{glazingTransmissivity}"
          # compute daylight matri(ces)
          system("#{t_catCommand} \"#{t_outPath}/materials/materials_vmx.rad\" \"#{t_outPath}/scene/glazing/#{space_name}_glaz_#{aziVector}.rad\" > \"#{t_outPath}/window_temp.rad\"")

          exec_statement("#{perlPrefix}genklemsamp#{perlExtension} #{klemsDensity} -vd #{aziVectorX.round_to_str(2)} #{aziVectorY.round_to_str(2)} 0.00 \"#{t_outPath}/window_temp.rad\" \
            | rcontrib #{klemsDensity} #{tregVars} -m skyglow -fa \"#{t_outPath}/octrees/model_dc.oct\" > \
            \"#{t_outPath}/output/dc/merged_space/maps/glaz_#{space_name}_azi-#{aziVector}_tn-#{glazingTransmissivity}.dmx\"")
          #end
      end
      #end
    # compute view matri(ces)
    puts "computing view matri(ces) for merged_space.map..."
    if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM) #Windows commands
      exec_statement("#{t_catCommand} \"#{t_outPath}/numeric/merged_space.map\" | rcontrib #{rtrace_args} -I+ -fo #{klemsDensity} #{tregVars} \
        -o \"#{t_outPath}/output/dc/merged_space/maps/%s.vmx\" -m skyglow model_dc.oct")
    else #UNIX commands
      exec_statement("#{t_catCommand} \"#{t_outPath}/numeric/merged_space.map\" | rcontrib #{rtrace_args} -n #{t_simCores} -I+ -fa #{klemsDensity} #{tregVars} \
        -o \"#{t_outPath}/output/dc/merged_space/maps/%s.vmx\" #{binPairs} \"#{t_outPath}/octrees/model_dc.oct\"")
    end
  end
end

def execSimulation(t_cmd, t_verbose, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_outPath)
  if t_verbose == 'v'
    puts "simulation command: #{t_cmd}"
  end        
  tempIO = IO.popen(t_cmd)
  temp = tempIO.readlines.to_s
  tempIO.close

  values = []
  temp.split(/\n|,/).each do |val|
    values << [val.to_f, 0].max
  end

  if values.empty?
    puts "ERROR: simulation command generated no results: #{t_cmd}"
  end

  index = 0;

  splitvalues = Hash.new

  t_space_names_to_calculate.each do |space_name|
    space_size = t_spaceWidths[space_name] * t_spaceHeights[space_name]
    space = []
    illum = []
    glaresensors = nil

    if values.size > 0
      space = values.slice(index, space_size)
      index = index + space_size

      if File.exists?("#{t_outPath}/numeric/#{space_name}.sns")        
        illum = [values[index]]
        index = index + 1
      end

      if File.exists?("#{t_outPath}/numeric/#{space_name}.glr") and t_radGlareSensorViews[space_name]
        glareinput = values.slice(index, t_radGlareSensorViews[space_name].size)

        glaresensors = []
        glareinput.each do |val|

          #Note, this replaced the call to rcalc with dgpSimplified, faster to do it 
          #in here instead of calling out to rcalc for the number of vectors we are looking at per space
          adjustedval = [(0.0000622*val.to_f)+0.184, 0].max
#          puts "Orig val: #{val} adjusted #{adjustedval}"
          glaresensors << adjustedval
        end

        index = index + t_radGlareSensorViews[space_name].size
      end
    else
      puts "INFO: simulation command generated no results, stuffing with 0's"
      space = Array.new(space_size, 0)

      if File.exists?("#{t_outPath}/numeric/#{space_name}.sns")        
        illum = Array.new(1, 0)
      end

      if File.exists?("#{t_outPath}/numeric/#{space_name}.glr") and t_radGlareSensorViews[space_name]
        glaresensors = Array.new(t_radGlareSensorViews[space_name].size, 0)
      end
    end

    
    splitvalues[space_name] = [space, illum, glaresensors]
  end

  return splitvalues
end


def execTimeSteps(t_cmd, t_dcTimeStepCommands, t_windowAzimuths, t_solarAzimuth, t_bsdfs, t_i)
  system(t_cmd)

  illum = 0

  dcVector = []

  t_dcTimeStepCommands.each_index do |dc_i|

    windowAzi = t_windowAzimuths[dc_i].to_f * (180 / Math::PI) #gimme degrees, yo!
    winVec = windowAzi + 180

    bsdf_index = 1 # or 1 (shade control)
    # shade calc
    if t_solarAzimuth[t_i] > (winVec + 90) or t_solarAzimuth[t_i] < (winVec - 90)
      bsdf_index = 0
    end

    bsdf = t_bsdfs[dc_i][bsdf_index]

    puts "solar azimuth: #{t_solarAzimuth[t_i]}, window azimuth: #{winVec}; using BSDF: #{bsdf}"

    dctimestep_command = t_dcTimeStepCommands[dc_i].gsub('_BSDF_FILENAME_',bsdf)
    dctimestep_command
    tempIO = IO.popen(dctimestep_command)
    result = tempIO.readlines.to_s
    tempIO.close

    n = 0
    result.split("\n").each do |line|
      illumTempParts = line.split(' ')
      illum = 179*((illumTempParts[0].to_f * 0.265) + (illumTempParts[1].to_f * 0.67) + (illumTempParts[2].to_f * 0.065))
      #$1=179*($1*0.265+$2*0.67+$3*0.065)
      if not dcVector[n]
        dcVector[n] = 0
      end
      dcVector[n] = dcVector[n] + illum

      n += 1
    end

  end

  return dcVector
end




def runSimulation(t_space_names_to_calculate, t_sqlFile, t_options, t_simCores, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_outPath, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews)

  values = Hash.new
  dcVectors = Hash.new

  perlPrefix = ""
  perlExtension = ""
  osQuote = "\'"


  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    perlPrefix = "perl \"C:/Program Files (x86)/Radiance/bin/"

    raypath = ENV['RAYPATH']
    raypath.split(';').each do |path|
      candidate = "#{path}\\..\\bin\\"
      if File.exists?("#{candidate}genskyvec.pl")
        perlPrefix = "perl \"#{candidate}"
        break
      end
    end

    perlExtension = ".pl\""
    osQuote = "\""
  end

  vLambda = "#{osQuote}$1=179*($1*0.265+$2*0.67+$3*0.065)#{osQuote}".strip

  # for each environment period (design days, annual, or arbitrary) you will create a directory for results
  t_sqlFile.availableEnvPeriods.each do |envPeriod|

    puts "envPeriod = " + envPeriod.to_s
    diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits = getTimeSeries(t_sqlFile, envPeriod)


    # check that we have all timeseries
    if (not diffHorizIllumAll) or (not dirNormIllumAll) or (not diffEfficacyAll) or (not dirNormEfficacyAll) or (not solarAltitudeAll) or (not solarAzimuthAll)
      puts "Missing required timeseries"
      exit
    end

    simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime = buildSimulationTimes(t_sqlFile, envPeriod, t_options, diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll)


    if not t_options.simMonth.nil? and not t_options.simDay.nil?
      puts "doing user-specified months/days using EPW data"
    else
      puts "Performing annual simulation"
    end

    # build up dctimestep command(s) for annual 3-phase sim
    windowAzimuths = nil
    dcTimeStepCommands = nil
    bsdfs = nil
    if t_options.z == true

      windowAzimuths = Array.new
      dcTimeStepCommands = Array.new
      bsdfs = Array.new

      # store window group matrix pairs, bsdf options, in array(s)

      File.open("#{t_outPath}/bsdf/mapping.rad", 'r') do |row|
        while line = row.gets

          mapAttrs = line.to_s.split(",")

          windowAzimuths << mapAttrs[0].split('_')[1].to_f
          dcTimeStepCommands << "dctimestep #{mapAttrs[0]} \"#{t_outPath}/bsdf/_BSDF_FILENAME_\" #{mapAttrs[3]} \"#{t_outPath}/timestep.vec\""
          bsdfs << [mapAttrs[1], mapAttrs[2]]

        end

      end

    end #dctimestep command(s)


    threads = []

    simTimes.each_index do |i|
      puts "Radiance (rcontrib) calculating: #{simTimes[i]}"

      # check if sun is up
      if (solarAltitude[i] < 0)
        if t_options.verbose == 'v'
          puts "#{simTimes[i]}, solar altitude is below horizon, skipping simulation."
        end
        next 
      end

      # start a new thread for this simulation
      threads << Thread.new {

        # these must be declared in the thread otherwise will get overwritten on each loop
        tsDateTime = simTimes[i]
        tsSolarAlt = solarAltitude[i]
        # gensky and gendaylit interpret azimuth in degrees WEST of SOUTH. Read it. Know it. Live it. (Whoa.) 
        tsSolarAzi = (solarAzimuth[i] - 180.0)
        tsDirectNormIllum = dirNormIllum[i]
        tsDiffuseHorIllum = diffHorizIllum[i]

        # trying to prevent gendaylit errors
        if tsDirectNormIllum < 1.0
          tsDirectNormIllum = 0.001
        end
        if tsDiffuseHorIllum < 1.0
          tsDiffuseHorIllum = 0.001
        end

        # do a test run of gendaylit at this timestep

        gendaylit_command = "gendaylit #{tsDateTime} -a #{t_site_latitude} -o #{t_site_longitude} -m #{t_site_stdmeridian} -L #{dirNormIllum[i]} #{diffHorizIllum[i]} 2>&1"
        gendaylit_command = "gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} 2>&1"
        tempIO = IO.popen(gendaylit_command)
        gendaylit_output = tempIO.readlines.to_s
        tempIO.close

        tempSky = true
        if /[wW]arning/.match(gendaylit_output) or /[eE]rror/.match(gendaylit_output) or /valid/.match(gendaylit_output) or /[cC]heck/.match(gendaylit_output) or /skyclearness/.match(gendaylit_output)
          tempSky = false
        end

        if tempSky == true

          # gendaylit was ok

          #changing glare options 20120816 rpg
          if t_options.glare == true
            puts "image based glare analysis temporarily disabled, sorry."
            #  system("gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} \
            #  | #{perlPrefix}genskyvec#{perlExtension} -m 1 | dctimestep #{outPath}/output/dc/#{space_name}/views/#{space_name}treg%03d.hdr | pfilt -1 -x /2 -y /2 > \
            #  #{outPath}/output/dc/#{space_name}/views/#{tsDateTime.gsub(/[: ]/,'_')}.hdr")
          end

          if t_options.x == true

            values[i] = execSimulation("gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} | #{perlPrefix}genskyvec#{perlExtension} -m #{t_options.skyvecDensity} | dctimestep ./output/dc/merged_space/maps/merged_space.dmx | rcalc -e #{vLambda}", t_options.verbose, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_outPath)

          end

          if t_options.z == true
            Dir.chdir("#{t_outPath}/output/dc/merged_space/maps/")
            dcVectors[i] = execTimeSteps("gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} | #{perlPrefix}genskyvec#{perlExtension} -m #{options.skyvecDensity} -c 1 1 1 > \"#{t_outPath}/timestep.vec\"", dcTimeStepCommands, windowAzimuths, solarAzimuth, bsdfs, i)

          end

          Dir.chdir("#{t_outPath}/")

        else  #use gensky
          tsHour = tsDateTime.to_s.split(":")[0]
          tsMin = tsDateTime.to_s.split(":")[1]
          tsSec = tsDateTime.to_s.split(":")[2]
          illToIrradDirect = dirNormIllum[i] / diffEfficacy[i]
          illToIrradDiffuse = diffHorizIllum[i] / dirNormEfficacy[i]

          if t_options.x == true
            # reformat time/irradiance vars for gensky input
            if t_options.glare == true
              puts "image based glare analysis temporarily disabled, sorry."
              #  system("gensky #{tsHour}:#{tsMin} -a #{site_latitude} -o #{site_longitude} -m #{site_stdmeridian} -R #{illToIrradDirect} -B #{illToIrradDiffuse} \
              #  | #{perlPrefix}genskyvec#{perlExtension} -m 1 | dctimestep #{outPath}/output/ts/#{space_name}/views/#{space_name}treg%03d.hdr | pfilt -1 -x /2 -y /2 > \
              #  #{Dir.pwd}/output/dc/#{space_name}/views/#{tsDateTime.gsub(/[: ]/,'_')}.hdr")
            end

            values[i] = execSimulation("gensky #{tsHour}:#{tsMin} -a #{t_site_latitude} -o #{t_site_longitude} -m #{t_site_stdmeridian} -R #{illToIrradDirect} -B #{illToIrradDiffuse} | #{perlPrefix}genskyvec#{perlExtension} -m #{t_options.patches}  | dctimestep \"#{Dir.pwd}/output/dc/merged_space/maps/merged_space.dmx\" | rcalc -e #{vLambda} 2>&1", t_options.verbose, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_outPath)

          end

          if t_options.z == true
            # reformat time/irradiance vars for gensky input
            Dir.chdir("#{t_outPath}/output/dc/merged_space/maps/")

            dcVectors[i] = execTimeSteps("gensky #{tsHour}:#{tsMin} -a #{t_site_latitude} -o #{t_site_longitude} -m #{t_site_stdmeridian} -R #{illToIrradDirect} -B #{illToIrradDiffuse} | #{perlPrefix}genskyvec#{perlExtension} -m #{t_options.patches}  -c 1 1 1 > \"#{t_outPath}/timestep.vec\"", dcTimeStepCommands, windowAzimuths, solarAzimuth, bsdfs, i)


            Dir.chdir("#{t_outPath}/")
          end
        end

      }

      # join threads when get to simCores
      if threads.size == t_simCores
        threads.each do |thread| 
          if not thread.join(200) # give limit in seconds to join
            puts "Error, thread did not complete successfully"
          end
        end
        threads = []
      end

    end

    # join any remaining threads
    threads.each do |thread| 
      if not thread.join(200) # give limit in seconds to join
        puts "Error, thread did not complete successfully"
      end
    end
    threads = []

  end

  return values, dcVectors;

end


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

end


def buildSimulationTimes(t_sqlFile, t_envPeriod, t_options, t_diffHorizIllumAll, t_dirNormIllumAll, t_diffEfficacyAll, t_dirNormEfficacyAll, t_solarAltitudeAll, t_solarAzimuthAll)

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


      simulate_this_time = true
      curMonth = "#{dateTime.date.monthOfYear.value}"
      if t_options.simMonth.nil?
        #puts "INFO: no months specified, simulating all months"
      else
        if not t_options.simMonth.include?(curMonth)
          simulate_this_time = false
        end
      end
      curDay = "#{dateTime.date.dayOfMonth}"
      if t_options.simDay.nil?
        #puts "INFO: no days specified, simulating all days"
      else
        if not t_options.simDay.include?(curDay)
          simulate_this_time = false
        end
      end
      if simulate_this_time
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
  end

  return simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime

end


def annualSimulation(t_sqlFile, t_options, t_epwFile, t_space_names_to_calculate, t_radMaps, t_spaceWidths, t_spaceHeights, t_radMapPoints, t_radGlareSensorViews, t_simCores, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_outPath, t_building, t_values, t_dcVectors)
  sqlOutPath = OpenStudio::Path.new("#{Dir.pwd}/output/radout.sql")
  if OpenStudio::exists(sqlOutPath)
    OpenStudio::remove(sqlOutPath)
  end


  # for each environment period (design days, annual, or arbitrary) you will create a directory for results
  t_sqlFile.availableEnvPeriods.each do |envPeriod|

    puts "envPeriod = " + envPeriod.to_s

    diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll, diffHorizUnits, dirNormUnits = getTimeSeries(t_sqlFile, envPeriod)

    # check that we have all timeseries
    if (not diffHorizIllumAll) or (not dirNormIllumAll) or (not diffEfficacyAll) or (not dirNormEfficacyAll) or (not solarAltitudeAll) or (not solarAzimuthAll)
      puts "Missing required timeseries"
      exit
    end

    simDateTimes, simTimes, diffHorizIllum, dirNormIllum, diffEfficacy, dirNormEfficacy, solarAltitude, solarAzimuth, firstReportDateTime = buildSimulationTimes(t_sqlFile, envPeriod, t_options, diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll)


    sqlOutFile = OpenStudio::SqlFile.new(sqlOutPath,
                                         t_epwFile.get(),
                                         OpenStudio::DateTime::now(),
                                         OpenStudio::Calendar.new(firstReportDateTime.date().year()));


    t_space_names_to_calculate.each do |space_name|
      illuminanceMatrixMaps = OpenStudio::MatrixVector.new();
      daylightSensorIlluminance = []
      meanIlluminanceMap = []
      minDGP = []
      meanDGP = []
      maxDGP = []

      puts "Processing Space: #{space_name}"
      timeSeriesIllum =[]
      timeSeriesGlare =[]
      if not t_options.simMonth.nil? and not t_options.simDay.nil?
        puts "doing user-specified months/days using EPW data"
      else
        puts "Performing annual simulation"
      end


      threads = []


      simTimes.each_index do |i|
        spaceWidth = t_spaceWidths[space_name]
        spaceHeight = t_spaceHeights[space_name]

        illuminanceMatrixMaps << OpenStudio::Matrix.new(spaceWidth, spaceHeight, 0)
        daylightSensorIlluminance << 0
        meanIlluminanceMap << 0
        minDGP << 0
        meanDGP << 0
        maxDGP << 0

        if t_options.verbose == 'v'
          puts "Processing: #{space_name} (#{simTimes[i]})"
        end

        # check if sun is up
        if (solarAltitude[i] < 0)
          if t_options.verbose == 'v'
            puts "#{simTimes[i]}, solar altitude is below horizon, skipping simulation."
          end
          timeSeriesIllum[i] = simTimes[i].to_s.gsub(" ",",") + "," + "#{dirNormIllum[i]}" +  "," "#{diffHorizIllum[i]}" + "," + ("0.0," * (t_radMapPoints[space_name].size + 1))

          # if space also has a glare sensor
          if t_radGlareSensorViews[space_name]
            timeSeriesGlare[i] = simTimes[i].to_s.gsub(" ",",") + "," + ("0.0," * (t_radGlareSensorViews[space_name].size))
          end

          next
        end

        # start a new thread for this simulation
        threads << Thread.new {

          # these must be declared in the thread otherwise will get overwritten on each loop
          tsDateTime = simTimes[i]

          #changing glare options 20120816 rpg
          if t_options.glare == true
            puts "image based glare analysis temporarily disabled, sorry."
            #  system("gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} \
            #  | #{perlPrefix}genskyvec#{perlExtension} -m 1 | dctimestep #{outPath}/output/dc/#{space_name}/views/#{space_name}treg%03d.hdr | pfilt -1 -x /2 -y /2 > \
            #  #{outPath}/output/dc/#{space_name}/views/#{tsDateTime.gsub(/[: ]/,'_')}.hdr")
          end
          if t_options.x == true

            illumValues, illumSensorValues, glareSensorValues = t_values[i][space_name]

            timeSeriesIllum[i] = tsDateTime.to_s.gsub(" ",",") + "," + "#{dirNormIllum[i]},#{diffHorizIllum[i]}," + illumSensorValues.join(',') + "," + illumValues.join(',')

            #glare
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

            #puts "Daylight sensor: #{daylightSensorIlluminance[i]} lux"
            n = 0
            sumIllumMap = 0
            illumValues.each do |val|
              x = (n%spaceWidth).to_i;
              y = (n/spaceWidth).to_i;
              #puts "Setting value (" + x.to_s + ", " + y.to_s + ") to " + val.to_f.to_s
              sumIllumMap += val.to_f
              m[x, y] = val.to_f
              n = n + 1
            end

            illuminanceMatrixMaps[i] = m

            if n != 0
              meanIlluminanceMap[i] = sumIllumMap / n.to_f
            end
          end

          if t_options.z == true
            dcVector = t_dcVectors[i]

            m = OpenStudio::Matrix.new(spaceWidth, spaceHeight, 0);
            sumIllumMap = 0
            numIllumMap = 0
            dcVector.length.times do |n| 
              if (n == 0)
                daylightSensorIlluminance[i] = dcVector[0].to_f
              else
                x = ((n-1)%spaceWidth).to_i
                y = ((n-1)/spaceWidth).to_i

                sumIllumMap += dcVector[n].to_f
                numIllumMap += 1

                #puts "Setting value (" + x.to_s + ", " + y.to_s + ") to " + dcVector[n].to_f.to_s
                m[x,y] = dcVector[n].to_f
              end
            end
            illuminanceMatrixMaps[i] = m;

            if numIllumMap != 0
              meanIlluminanceMap[i] = sumIllumMap / numIllumMap.to_f
            end

            timeSeriesIllum[i] = tsDateTime.to_s.gsub(" ",",") + "," + "#{dirNormIllum[i]},#{diffHorizIllum[i]}," + dcVector.join(',')

          end
          Dir.chdir("#{t_outPath}/")

        }

        # join threads when get to simCores
        if threads.size == t_simCores
          threads.each do |thread| 
            if not thread.join(200) # give limit in seconds to join
              puts "Error, thread did not complete successfully"
            end
          end
          threads = []
        end

      end

      # join any remaining threads
      threads.each do |thread| 
        if not thread.join(200) # give limit in seconds to join
          puts "Error, thread did not complete successfully"
        end
      end
      threads = []


      #Print illuminance results to dat file
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

        f.print "## OpenStudio Daylight Simulation Results file\n"
        f.print "## Header: xmin ymin z xmax ymin z xmax ymax z xspacing yspacing\n"
        f.print "## Data: month,day,time,directNormalIllumimance(external),diffuseHorizontalIlluminance(external),daylightSensorIlluminance,pointIlluminance [lux]\n"
        f.print "#{xmin} #{ymin} #{z} #{xmax} #{ymin} #{z} #{xmax} #{ymax} #{z} #{xSpacing} #{ySpacing}\n"
        timeSeriesIllum.each {|ts| f.print "#{ts}\n"}
        f.close

        #Print glare results to dat file
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

        puts "writing Radiance results database..."
        writeTimeSeriesToSql(sqlOutFile, simDateTimes, dirNormIllum, space_name, "Direct Normal Illuminance", "lux")
        writeTimeSeriesToSql(sqlOutFile, simDateTimes, diffHorizIllum, space_name, "Global Horizontal Illuminance", "lux")
        writeTimeSeriesToSql(sqlOutFile, simDateTimes, daylightSensorIlluminance, space_name, "Daylight Sensor Illuminance", "lux")
        writeTimeSeriesToSql(sqlOutFile, simDateTimes, meanIlluminanceMap, space_name, "Mean Illuminance Map", "lux")
        puts "Done writing illuminance results to database..."

        if t_radGlareSensorViews[space_name]
          writeTimeSeriesToSql(sqlOutFile, simDateTimes, minDGP, space_name, "Minimum Simplified Daylight Glare Probability", "")
          writeTimeSeriesToSql(sqlOutFile, simDateTimes, meanDGP, space_name, "Mean Simplified Daylight Glare Probability", "")
          writeTimeSeriesToSql(sqlOutFile, simDateTimes, maxDGP, space_name, "Maximum Simplified Daylight Glare Probability", "")
        end
        puts "Done writing glare results to database..."

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
end




# help those poor Windows users out
perlExtension = ""
catCommand = "cat"
osQuote = "\'"
if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  catCommand = "type"
  osQuote = "\""
end


modelPath = OpenStudio::Path.new(ARGV[0])
modelPath = OpenStudio::system_complete(modelPath)
outPath = modelPath.parent_path / OpenStudio::Path.new(modelPath.stem) / OpenStudio::Path.new("model") /OpenStudio::Path.new("radiance")
osmPath = modelPath.parent_path / OpenStudio::Path.new(modelPath.stem)
sqlPath = OpenStudio::Path.new(ARGV[1])
sqlPath = OpenStudio::system_complete(sqlPath)

puts "outPath= #{outPath}"
puts "modelPath=#{modelPath}"
puts "sqlPath=#{sqlPath}"

# get model
idf = OpenStudio::IdfFile.load(modelPath)
model = OpenStudio::Model::Model.new(idf.get)

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

puts "Getting weather file"
epwFile = nil
if (!weatherFile.empty?)
  puts "Weather file is not empty"

  p = weatherFile.get().path()
  if (!p.empty?)
    puts "Path to weather file is: " + p.get().to_s + " using osmpath: " + osmPath.to_s
  end

  epwFile = weatherFile.get().file(osmPath)

  if (!epwFile.empty?)
    puts "epwFile is not empty"
  end
else
  puts "weather file object is empty"
end

if (weatherFile.empty? || epwFile.empty? || !File.exists?(epwFile.get.to_s))
  puts "EPW From model not found"
  possibleEpw = modelPath.parent_path() / OpenStudio::Path.new("in.epw");

  if (File.exists?(possibleEpw.to_s))
    puts "EPW not found, but found one here: " + possibleEpw.to_s
    epwFile = OpenStudio::OptionalEpwFile.new(OpenStudio::EpwFile.new(possibleEpw))
  end
end

site_name = site.getString(1, true).get
site_latitude = site.getString(2, true).get
site_longitude = site.getString(3, true).get
site_timezone = site.getString(4, true).get
site_elevation = site.getString(5, true).get

site_stdmeridian = site_timezone.to_f * 15

puts "city: #{site_name}"
puts "latitude: #{site_latitude}"
puts "longitude: #{site_longitude}"
puts "elevation: #{site_elevation} meters"
puts "timezone: GMT #{site_timezone}"

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
if not options.simSpaces.nil?
  puts "calculating user-specified spaces:"
  options.simSpaces.each do |space_name|
    space_names_to_calculate << space_name.gsub(' ', '_').gsub(':', '_')
  end
else
  space_names_to_calculate = space_names
  puts "no spaces specified, calculating all spaces (could take a while):"
end

filtered_space_names_to_calculate = Array.new
space_names_to_calculate.each do |space_name|
  if not radMaps[space_name].nil?
    filtered_space_names_to_calculate << space_name
  end
end

space_names_to_calculate = filtered_space_names_to_calculate

# run radiance
# check for Radiance installation
if not system("rtrace -version")
  puts "Cannot find required Radiance executables in the current path"
  exit
else
  puts "Radiance... OK."
end

# setup some directories
FileUtils.mkdir("#{outPath}/skies/") unless File.exists?("#{outPath}/skies/")
FileUtils.mkdir("#{outPath}/octrees/") unless File.exists?("#{outPath}/octrees/")
FileUtils.mkdir("#{outPath}/output/") unless File.exists?("#{outPath}/output/")

# look for options file argument, otherwise read in from model object
opts_map = ""
opts_img = ""
if not ARGV[2].nil?
  opts = '@' + ARGV[1]
else
  puts "no options file provided, reading from model"
  opts_map = "@#{outPath}/options/maps.opt"
  opts_img = "@#{outPath}/options/images.opt"
  opts_vmx = "@#{outPath}/options/vmx.opt"
  opts_dmx = "@#{outPath}/options/dmx.opt"
end

# move to base radiance dir
Dir.chdir("#{outPath}")
puts "\nRunning radiance in directory '#{Dir.pwd}'"


mergeSpaces(space_names_to_calculate, outPath)


#
# calculate daylight coefficients, if selected (single phase)
#
if options.dc == true
  calculateDaylightCoeffecients(outPath, options, space_names_to_calculate, radMaps, opts_map, simCores, catCommand)
end

#Annual simulation
#run genskyvec, dctimestep, for timestep results from DCs. Defaults to all spaces, can specify spaces/months/days at command line
#TODO: check results and verify format of grids
if options.dcts == true
  values, dcVectors = runSimulation(space_names_to_calculate, sqlFile, options, simCores, site_latitude, site_longitude, site_stdmeridian, outPath, spaceWidths, spaceHeights, radGlareSensorViews)
  annualSimulation(sqlFile, options, epwFile, space_names_to_calculate, radMaps, spaceWidths, spaceHeights, radMapPoints, radGlareSensorViews, simCores, site_latitude, site_longitude, site_stdmeridian, outPath, building, values, dcVectors)
end

# do "radiance classic" timestep simulation(s), if selected
if options.ts == true
  Dir.mkdir("#{Dir.pwd}/output/designday") unless File.exists?("#{Dir.pwd}/output/designday")
  # make base octree
  exec_statement("oconv -f ./materials/materials.rad ./model.rad > model.oct")
  # generate timestep sky models
  radEnvSphere = ""
  radEnvSphere = "\nskyfunc glow skyglow\n0\n0\n4 1 1 1 0\nskyglow source sky\n0\n0\n4 0 0 1 180\nskyfunc glow groundglow\n0\n0\n4 1 1 1 0\ngroundglow source ground\n0\n0\n4 0 0 -1 180"
  options.simMonth.each do |sim_month|
    # loop over days
    options.simDay.each do |sim_day|
      #loop over hours
      options.simHour.each do |sim_hour|
        # loop over sky types
        ['CIE12', 'CIE01'].each do |sky_type|
          # NOTE: CIE01 = CIE standard overcast sky, CIE12 = CIE standard clear sky (with sun)
          gensky_type = ""
          if sky_type == "CIE01"
            gensky_type = "-c"
          elsif sky_type == "CIE12"
            gensky_type = "+s"
          end
          File.open("./skies/#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.sky", "w") do |file|
            file << "!gensky #{sim_month} #{sim_day} #{sim_hour} #{gensky_type} -a #{site_latitude} -o #{site_longitude} -m #{site_stdmeridian} \n#{radEnvSphere}"
            puts "wrote sky: ./skies/#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.sky"
          end
        end
      end
    end
  end
  # loop over spaces
  space_names_to_calculate.each do |space_name|
    # loop over months
    options.simMonth.each do |sim_month|
      # loop over days
      options.simDay.each do |sim_day|
        #loop over hours
        options.simHour.each do |sim_hour|
          # loop over sky types
          ['CIE12', 'CIE01'].each do |sky_type|
            # if space has an illuminance map, do calcs
            if not radMaps[space_name].nil?
              exec_statement("oconv -w -i ./model.oct #{Dir.pwd}/skies/#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.sky > timestep.oct")
              rtrace_args = "#{opts_map} -faa -w -h -ov -I -dv- timestep.oct"
              rcalc_eq = "$1=($1*.265+$2*.67+$3*.065)*179"
              if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
                exec_statement("type .\\numeric\\#{space_name}.map | rtrace #{rtrace_args} | rcalc -e \"#{rcalc_eq}\" > .\\output\\designday\\#{space_name}_#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.ill")
              elsif
                exec_statement("#{catCommand} ./numeric/#{space_name}.map | rtrace #{rtrace_args} | rcalc -e '#{rcalc_eq}' > ./output/designday/#{space_name}_#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.ill")
              end
              # create flood plots
              # open the illuminance file
              File.open("./output/designday/#{space_name}_#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.ill", 'r') do |file|
                # get the output illuminance map
                handle = radMapHandles[space_name]
                map = model.getObject(handle)
                if map.empty?
                  puts "OutputIlluminanceMap not found"
                  next
                end
                map = map.get

                xmin = map.getDouble(4, true).get
                xmax = map.getDouble(5, true).get
                nx = map.getUnsigned(6, true).get
                ymin = map.getDouble(7, true).get
                ymax = map.getDouble(8, true).get
                ny = map.getUnsigned(9, true).get

                x = OpenStudio::linspace(xmin, xmax, nx)
                y = OpenStudio::linspace(ymin, ymax, ny)
                puts "xmin=#{xmin}, xmax=#{xmax}, nx=#{nx}, ymin=#{ymin}, ymax=#{ymax}, ny=#{ny}"

                # each line has illuminance value in lux
                fcs = []
                while line=file.gets
                  fcs << line.strip.to_f
                end

                data = OpenStudio::Matrix.new(nx,ny)
                i = 0
                j = 0
                fcs.each_index do |index|
                  data[i,j] = fcs[index]
                  i += 1
                  if i >= nx
                    i = 0
                    j += 1
                  end
                end

                fp = OpenStudio::FloodPlot::create().get
                fp.floodPlotData(OpenStudio::MatrixFloodPlotData::create(x, y, data))
                fp.plotTitle("Illuminance [lux]")
                fp.bottomAxisTitle("x [m]")
                fp.leftAxisTitle("y [m]")
                fp.generateImage(OpenStudio::Path.new("./output/designday/#{space_name}_#{sim_month}_#{sim_day}_#{sim_hour}_#{sky_type}.ill.png"))
              end

              # add rendering calls here
              # TODO:
              # will need to define X/Y image size
              # place to store views (store with design day output, or separate?)
              # read in views
              # maybe just do each space/view for noon with some low parameters for now?
              ['N', 'S'].each do |view_dir|
                exec_statement("rpict #{rtrace_args} -vf #{Dir.pwd}/views/#{space_name}_#{view_dir}.vf timestep.oct >  #{Dir.pwd}/output/designday/#{space_name}_#{view_dir}.hdr")

                # image conversion from .hdr
                if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
                  exec_statement("ra_bmp #{Dir.pwd}/output/designday/#{space_name}_#{view_dir}.hdr #{Dir.pwd}/output/designday/#{space_name}_#{view_dir}.bmp")
                else
                  exec_statement("ra_tiff #{Dir.pwd}/output/designday/#{space_name}_#{view_dir}.hdr #{Dir.pwd}/output/designday/#{space_name}_#{view_dir}.tif")
                end
              end
            end
          end
        end
      end
    end
  end
end
