######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
        options.z = ""
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

if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  perlpath = ""
  if OpenStudio::applicationIsRunningFromBuildDirectory()
    perlpath = OpenStudio::getApplicationRunDirectory().parent_path().parent_path() / 
    OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
  else
    perlpath = OpenStudio::getApplicationRunDirectory().parent_path() / 
    OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
  end
  puts "Adding path for local perl: " + perlpath.to_s
  ENV["PATH"] = ENV["PATH"] + ";" + perlpath.to_s
end

if !which('perl')
  puts "Perl could not be found in path, exiting"
  exit false
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
# disable MP for Yosemite, because:
# http://radiance-online.org/pipermail/radiance-dev/2014-November/001442.html
if /darwin/.match(RUBY_PLATFORM)
	ver = `defaults read loginwindow SystemVersionStampAsString`
	if ver.split(".")[1] == '10'
		puts "Radiance multiprocessing disabled for MacOS 10.10.x, sorry."
		simCores = 1
	end
end

if options.verbose == 'v'

  puts options
  puts "tregVars: #{options.tregVars}"
  puts "klemsDensity: #{options.klemsDensity}"
  puts "skyvecDensity: #{options.skyvecDensity}"
  puts "dmx: #{options.dmx}"
  puts "vmx: #{options.vmx}"

end

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
  #puts "start '#{s}'"
  result = system(s)
  #puts "end '#{s}'"
  return result
end

def writeTimeSeriesToSql(sqlfile, simDateTimes, illum, space_name, ts_name, ts_units)
  #puts "writing Radiance glare results database..."
  #puts DateTime.now.to_s + " Beginning timeseries write to sql"
  #puts DateTime.now.to_s + " Creating data vector"
  data = OpenStudio::Vector.new(illum.length)
  illum.length.times do |n|
    begin
      data[n] = illum[n].to_f;
    rescue Exception => e
      puts "Error inserting data: " + illum[n] + " inserting 0 instead";
      data[n] = 0;
    end
  end
  #puts DateTime.now.to_s + " Creating TimeSeries Object"
  illumTS = OpenStudio::TimeSeries.new(simDateTimes, data, ts_units);
  #puts DateTime.now.to_s + " Inserting into SQLFile"
  sqlfile.insertTimeSeriesData(
    "Average", "Zone", "Zone", space_name, ts_name, OpenStudio::ReportingFrequency.new("Hourly"),
    OpenStudio::OptionalString.new(),
    ts_units, illumTS);
  #puts DateTime.now.to_s + " Ending timeseries write to sql"
end

def mergeSpaces(t_space_names_to_calculate, t_outPath)
  # write window group control points to merged file
  File.open("#{t_outPath}/numeric/window_controls.map", "w") do |f|

    windows = Dir.glob("#{t_outPath}/numeric/WG*.pts")
    windows.each do |wg|
      f.write IO.read(wg)
    end
  end

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
  FileUtils.mkdir_p("#{t_outPath}/output/ts") unless File.exists?("#{t_outPath}/output/ts")
 
  binDir = "#{t_outPath}/output/dc/merged_space/maps"
  FileUtils.mkdir_p("#{binDir}") unless File.exists?("#{binDir}")

  procsUsed = ""
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    puts "Radiance does not support multiple cores on Windows"
    procsUsed = ""
  else
    puts "Radiance using #{t_simCores} core(s)"
    procsUsed = "-n #{t_simCores}"
  end

  # "single phase" method
  if t_options.x == true
    rtrace_args = "#{t_options.vmx}"
    system("oconv \"#{t_outPath}/materials/materials.rad\" model.rad \
      \"#{t_outPath}/skies/dc_sky.rad\" > model_dc.oct")
    #compute illuminance map(s)
    puts "#{Time.now.getutc}: computing sky-to-point daylight coefficients (single phase)..."
    exec_statement("#{t_catCommand} \"#{t_outPath}/numeric/merged_space.map\" \
      | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{t_options.tregVars} \
      -o \"#{t_outPath}/output/dc/merged_space/maps/merged_space.dmx\" \
      -m skyglow model_dc.oct")
    puts "#{Time.now.getutc}: daylight coefficients computed, \
    stored in #{t_outPath}/output/dc/merged_space/maps"

  end # compute coefficients (single phase)

  # 3-phase method
  if t_options.z == true
  
  	haveWG0 = ""

    puts "#{Time.now.getutc}: computing daylight coefficients with 3-phase method..."

    mapFile=File.open("#{t_outPath}/numeric/merged_space.map","r")
    rfluxmtxDim = mapFile.readlines.size.to_s
    puts "total calculation points: #{rfluxmtxDim}"

    # compute daylight matrices
 
    system("oconv materials/materials.rad model.rad > model_dc.oct")
    windowMaps = File::open("#{t_outPath}/bsdf/mapping.rad")
    windowMaps.each do |row|
      next if row[0] == "#"
      wg=row.split(",")[0]
      
      # if WG0 (uncontrolled windows):
      if wg[2].to_i == 0
      
				# use more aggro simulation parameters because this is basically a view matrix
				rtrace_args = "#{t_options.vmx}"
				
				# make special WG0 octree
				system("oconv \"#{t_outPath}/materials/materials.rad\" \"#{t_outPath}/materials/materials_WG0.rad\" model.rad \
					\"#{t_outPath}/skies/dc_sky.rad\" > model_WG0.oct")
	
				# do daylight coefficients for uncontrolled windows
				puts "#{Time.now.getutc}: computing daylight/view matrix for window group #{wg}"
				system("#{t_catCommand} \"#{t_outPath}/numeric/merged_space.map\" \
					| rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{t_options.tregVars} -o \"#{t_outPath}/output/dc/WG0.vmx\" -m skyglow model_WG0.oct")

			else
				
				# use more chill sim parameters
				rtrace_args = "#{t_options.dmx}"
				
				# checking...
				puts "#{Time.now.getutc}: computing daylight matrix for window group #{wg}"
				
				# do daylight matrix(es) for controlled windows
				exec_statement("rfluxmtx -fa -v #{t_outPath}/scene/glazing/#{wg}.rad #{t_outPath}/skies/dc_sky.rad -i model_dc.oct > \
				#{t_outPath}/output/dc/#{wg}.dmx")
			
			end

    end

    # compute view matrices for all controlled window groups

		# use fine params
    rtrace_args = "#{t_options.vmx}" 

		# get the window groups, skipping WG0 if present
		wgInput = []
    Dir.glob("#{t_outPath}/scene/glazing/WG*.rad") {|file|
      next if file == "#{t_outPath}/scene/glazing/WG0.rad"
      wgInput << file
		}
    
    puts "#{Time.now.getutc}: computing view matri(ces) for controlled windows"
    exec_statement("#{t_catCommand} #{t_outPath}/materials/materials_vmx.rad #{wgInput.join(" ")} > receivers_vmx.rad")
    exec_statement("oconv #{t_outPath}/materials/materials.rad #{t_outPath}/scene/*.rad > model_vmx.oct")
    exec_statement("rfluxmtx #{rtrace_args} -ds .15 #{procsUsed} -faa -n #{t_simCores} -y #{rfluxmtxDim} -I -v - receivers_vmx.rad -i model_vmx.oct < \
      #{t_outPath}/numeric/merged_space.map")
    
    # compute daylight coefficient matrices for window group control points
		# use relaxed (dmx) params, TODO: have fine option for urban canyons/other obstructions
    rtrace_args = "#{t_options.dmx}"
    system("oconv \"#{t_outPath}/materials/materials.rad\" model.rad \
      \"#{t_outPath}/skies/dc_sky.rad\" > model_wc.oct")
    puts "#{Time.now.getutc}: computing DCs for window control points"
    exec_statement("#{t_catCommand} \"#{t_outPath}/numeric/window_controls.map\" \
      | rcontrib #{rtrace_args} #{procsUsed} -I+ -fo #{t_options.tregVars} \
      -o \"#{t_outPath}/output/dc/window_controls.vmx\" \
      -m skyglow model_wc.oct")

    puts "#{Time.now.getutc}: done (daylight coefficients)."

  end # compute coefficients (3-phase)

end # def(calculateDaylightCoeffecients)


# t_cmds is actually the command pushed into "simulations"

def execSimulation(t_cmds, t_mapping, t_verbose, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_outPath)

  puts "#{Time.now.getutc}: Executing simulation"
  
  allValues = []
  values = []

  # read illuminance values from file
  values = []
  valuesFile = File.open("#{t_outPath}/output/ts/merged_space.ill")
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

        if File.exists?("#{t_outPath}/numeric/#{space_name}.sns")        
          if index >= values.size
            puts "index is #{index} but values.size is only #{values.size}"
          elsif hour >= values[index].size
            puts "hour is #{hour} but values.size[index] is only #{values[index].size}"
          end
          illum = [values[index][hour]]
          index = index + 1
        end

        if File.exists?("#{t_outPath}/numeric/#{space_name}.glr") and t_radGlareSensorViews[space_name]
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

        if File.exists?("#{t_outPath}/numeric/#{space_name}.sns")        
          illum = Array.new(1, 0)
        end

        if File.exists?("#{t_outPath}/numeric/#{space_name}.glr") and t_radGlareSensorViews[space_name]
          glaresensors = Array.new(t_radGlareSensorViews[space_name].size, 0)
        end
      end

      splitvalues[space_name] = [space, illum, glaresensors]
    end

    allhours[hour] = splitvalues;
  end

  puts "Returning annual results"
  return allhours;
end

def runSimulation(t_space_names_to_calculate, t_sqlFile, t_options, t_simCores, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_outPath, \
t_spaceWidths, t_spaceHeights, t_radGlareSensorViews)

  puts "#{Time.now.getutc}: Calculating annual daylight values"

  rawValues = Hash.new
  values = Hash.new
  dcVectors = Hash.new

  perlPrefix = ""
  perlExtension = ""
  osQuote = "\'"


  # i can haz gendaymtx vintage? (gendaymtx >= v4.2.b adds header and -h option to suppress) - 2014.07.02 RPG 
	genDaymtxHdr = ""
  exec_statement("gendaymtx -h -m #{t_options.skyvecDensity} \"#{t_outPath / OpenStudio::Path.new("in.wea")}\" > \"#{t_outPath / OpenStudio::Path.new("daymtx_out.tmp")}\" ")
  if File.zero?("#{t_outPath / OpenStudio::Path.new("daymtx_out.tmp")}")
    genDaymtxHdr = ""
    if t_options.z == true
      puts "Old Radiance version detected, will not work with 3-phase method, quitting."
      exit false
    end
  end
  File.delete("#{t_outPath / OpenStudio::Path.new("daymtx_out.tmp")}")
  # we now haz =)

	# OS stuff that is probably redundant
	
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


  # Run the simulation 

  simulations = []

  exec_statement("gendaymtx #{genDaymtxHdr} -m #{t_options.skyvecDensity} \"#{t_outPath / OpenStudio::Path.new("in.wea")}\" > annual-sky.mtx")

  if t_options.z == true  # 3-phase

    windowMaps = File::open("#{t_outPath}/bsdf/mapping.rad")
    
    # do annual sim for each window group and state
    
    windowMaps.each do |row|
      # skip header
      next if row[0] == "#"
      wg = row.split(",")[0]
      
      # do uncontrolled windows (WG0)
      if wg[2].to_i == 0
        # keep header, convert to illuminance, but no transpose
        exec_statement("dctimestep #{t_outPath}/output/dc/#{wg}.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > #{t_outPath}/output/ts/#{wg}.ill") 
      
      else
      
      # do all controlled window groups
      
				wgXMLs = row.split(",")[4..-1]
				if wgXMLs.size > 2
					puts "WARN: Window Group #{wg} has #{wgXMLs.size.to_s} BSDFs (2 max supported by OpenStudio application)."
				end
			
				wgXMLs.each_index do |i|
					exec_statement("dctimestep #{t_outPath}/output/dc/#{wg}.vmx #{t_outPath}/bsdf/#{wgXMLs[i].strip} #{t_outPath}/output/dc/#{wg}.dmx \
					annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - > #{t_outPath}/output/ts/#{wg}_#{wgXMLs[i].split[0]}.ill")
 
				end
				
			end
      
    end
    
    # get annual values for window control sensors (note: convert to illuminance, no transpose, strip header)
    exec_statement("dctimestep #{t_outPath}/output/dc/window_controls.vmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - | getinfo - > #{t_outPath}/output/ts/window_controls.ill")

    # return the bsdf index for window group given by index at this hour
    # this is deprecated
    windowMapping = lambda { |index, hour| 
      data = dmx_mapping_data[index]
      # TODO remove this bit for shade controls
      return 0
    }
    
    puts "#{Time.now.getutc}: Calculated daylight illuminance for all window group states; merging results..."
    
    # do that window group/state merge thing
    
    windowGroups = File.open("#{t_outPath}/bsdf/mapping.rad")
		windowGroups.each do |wg|

			next if wg[0] == "#"
			windowGroup = wg.split(",")[0]
			next if windowGroup == "WG0"

			wgIllumFiles = Dir.glob("#{t_outPath}/output/ts/#{windowGroup}_*.ill")

			shadeControlSetpoint = wg.split(",")[3].to_f

			puts "#{Time.now.getutc}: Processing window group '#{windowGroup}', setpoint: #{shadeControlSetpoint} lux..."  

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
			
			windowControls = File.open("#{t_outPath}/output/ts/window_controls.ill", "r")

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
		system("rmtxop -fa #{addFiles} -t | getinfo - > #{t_outPath}/output/ts/merged_space.ill")

  	## window merge end
  	# 3-phase end   

  else # 2-phase 
    
    # should clean up these file extensions; really there's no daylight matrix in a 2-phase calc. =|
    # storing the "values" data in a file to support 3-phase workflow, no transpose operation
    
    exec_statement("dctimestep #{t_outPath}/output/dc/merged_space/maps/merged_space.dmx annual-sky.mtx | rmtxop -fa -c 47.4 120 11.6 - | \
    getinfo - > #{t_outPath}/output/ts/merged_space.ill")
    
  end # 2-phase end

  # TODO: rename execSimulation to parseResults or something that makes sense
  rawValues = execSimulation(simulations, windowMapping, t_options.verbose, t_space_names_to_calculate, t_spaceWidths, t_spaceHeights, t_radGlareSensorViews, t_outPath)

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
    buildSimulationTimes(t_sqlFile, envPeriod, t_options, diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll)

    simTimes.each_index do |i|
      datetime = simDateTimes[i]
      hours = ((datetime.date().dayOfYear() - 1) * 24) + datetime.time().hours()
      values[i] = rawValues[hours]
    end

  end

  return values, dcVectors;
  
end # end of runSimulation function

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


def annualSimulation(t_sqlFile, t_options, t_epwFile, t_space_names_to_calculate, t_radMaps, t_spaceWidths, t_spaceHeights, t_radMapPoints, \
	t_radGlareSensorViews, t_simCores, t_site_latitude, t_site_longitude, t_site_stdmeridian, t_outPath, t_building, t_values, t_dcVectors)
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
    buildSimulationTimes(t_sqlFile, envPeriod, t_options, diffHorizIllumAll, dirNormIllumAll, diffEfficacyAll, dirNormEfficacyAll, solarAltitudeAll, solarAzimuthAll)


    sqlOutFile = OpenStudio::SqlFile.new(sqlOutPath,
                                         t_epwFile.get(),
                                         OpenStudio::DateTime::now(),
                                         OpenStudio::Calendar.new(firstReportDateTime.date().year()));

    puts "#{Time.now.getutc}: removing indexes"
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

        if t_options.verbose == 'v'
          puts "Processing: #{space_name} (#{simTimes[i]})"
        end

        
        # these must be declared in the thread otherwise will get overwritten on each loop
        tsDateTime = simTimes[i]

        if t_options.glare == true
          puts "image based glare analysis temporarily disabled, sorry."
          #  system("gendaylit -ang #{tsSolarAlt} #{tsSolarAzi} -L #{tsDirectNormIllum} #{tsDiffuseHorIllum} \
          #  | #{perlPrefix}genskyvec#{perlExtension} -m 1 | dctimestep \"#{outPath}/output/dc/#{space_name}/views/#{space_name}treg%03d.hdr\" | pfilt -1 -x /2 -y /2 > \
          #  \"#{outPath}/output/dc/#{space_name}/views/#{tsDateTime.gsub(/[: ]/,'_')}.hdr\"")
        end


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

				puts "#{Time.now.getutc}: writing Radiance results file..."

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

    puts "#{Time.now.getutc}: creating indexes..."
    sqlOutFile.createIndexes
    puts "#{Time.now.getutc}: done writing Radiance results database."

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

weaPath = nil
smxPath = nil

# reduce/convert epw data to Daysim-style ".wea" input format
if (!epwFile.empty?)
  epwFilePath = epwFile.get().path()
  weaPath = outPath / OpenStudio::Path.new("in.wea")

  epw2weapath = OpenStudio::Path.new(ENV['EPW2WEAPATH'])

  puts "Executing epw2wea: #{epw2weapath}"
  exec_statement("\"#{epw2weapath.to_s}\" \"#{epwFilePath.to_s}\" \"#{weaPath.to_s}\"")
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
  options.simSpaces.each do |space_name|
    space_names_to_calculate << space_name.gsub(' ', '_').gsub(':', '_')
  end

else
  space_names_to_calculate = space_names
end

# only do spaces with illuminance maps
filtered_space_names_to_calculate = Array.new
space_names_to_calculate.each do |space_name|
  if not radMaps[space_name].nil?
    filtered_space_names_to_calculate << space_name
  end
end

space_names_to_calculate = filtered_space_names_to_calculate
puts "#{Time.now.getutc}: calculating #{space_names_to_calculate}"

# run radiance
# check for Radiance installation

puts "#{Time.now.getutc}: checking for radiance"

if not system("rtrace -version")
  puts "#{Time.now.getutc}: Cannot find required Radiance executables in the current path"
  exit false  # Chevy Nova
else
  puts "#{Time.now.getutc}: radiance... ok."
end

# setup some directories
# FileUtils.mkdir("#{outPath}/skies/") unless File.exists?("#{outPath}/skies/")
FileUtils.mkdir("#{outPath}/octrees/") unless File.exists?("#{outPath}/octrees/")
FileUtils.mkdir("#{outPath}/output/") unless File.exists?("#{outPath}/output/")

# look for options file argument, otherwise read in from model object
opts_map = ""
opts_img = ""
if not ARGV[2].nil?
  opts = '@' + ARGV[1]
else
  puts "#{Time.now.getutc}: no options file provided, reading from model"
  opts_map = "@#{outPath}/options/maps.opt"
  opts_img = "@#{outPath}/options/images.opt"
  opts_vmx = "@#{outPath}/options/vmx.opt"
  opts_dmx = "@#{outPath}/options/dmx.opt"
end


# Move to base Radiance dir

Dir.chdir("#{outPath}")
puts "\nRunning radiance in directory '#{Dir.pwd}'"

# Merge illuminance map points, daylight control points, and glare sensor views to a single input file for Radiance programs

mergeSpaces(space_names_to_calculate, outPath)


# Compute daylight coefficients for model

if options.dc == true
  calculateDaylightCoeffecients(outPath, options, space_names_to_calculate, radMaps, opts_map, simCores, catCommand)
end


# Annual simulation (run genskyvec, dctimestep, for timestep results from DCs)

if options.dcts == true
  values, dcVectors = runSimulation(space_names_to_calculate, sqlFile, options, simCores, site_latitude, site_longitude, site_stdmeridian, outPath, spaceWidths, spaceHeights, radGlareSensorViews)
  annualSimulation(sqlFile, options, epwFile, space_names_to_calculate, radMaps, spaceWidths, spaceHeights, radMapPoints, radGlareSensorViews, simCores, site_latitude, site_longitude, site_stdmeridian, outPath, building, values, dcVectors)
end
