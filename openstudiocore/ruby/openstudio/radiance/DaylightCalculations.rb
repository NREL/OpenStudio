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
#   Translate an OpenStudio Building Model to Radiance Simulation format,
#   compute daylighting coefficients, perform annual daylight simulation using
#   daylighting coefficients, compute dimming response based on annual simulation,
#   create modified model with lighting schedules to implement dimming response.
#
# == Usage
#
#  ruby DaylightCalculations.rb ARGV[0] (ARGV[1])
#
#  ARGV[0] - Path to OpenStudio Model
#
#  ARGV[1] - Path to radiance installation
#
#  ARGV[2] - Optional path to EnergyPlus SQLite output, if not given will run EnergyPlus. 
#            EnergyPlus simulation must be pre-run with the SQLite output including tabular data 
#            and the EnvelopeSummary table must be requested.  The "Exterior 
#            Horizontal Illuminance From Sky", and "Exterior Beam Normal 
#            Illuminance" output variables must be requested at hourly reporting 
#            frequency.
#
# == Examples
#
#   ruby DaylightCalculations.rb 'C:\path\to\model.osm' 'c:\Program Files (x86)\Radiance\bin'
#   ruby DaylightCalculations.rb 'C:\path\to\model.osm' 'c:\Program Files (x86)\Radiance\bin' 'C:\path\to\eplusout.sql' 
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'fileutils'

require 'optparse'
require 'ostruct'
  
class ParseOptions

  #
  # Return a structure describing the commandline options.
  #
  def self.parse(args)
    # The options specified on the command line will be collected in *options*.
    options = OpenStruct.new
    # We set some default values here.
    options.verbose = ''

    opts = OptionParser.new do |opts|
      opts.banner = "\nUsage: DaylightCalculations.rb [ARGV[0] ARGV[1] (ARGV[2]) [options]]\n\nARGV[0] - Path to OpenStudio Model\nARGV[1] - Path to radiance installation to use\nARGV[1] - Optional path to EnergyPlus SQLite output, if not given will run EnergyPlus."
      opts.separator ""
      opts.separator "Optional variables:"        
      opts.on("-v", "--verbose", "Verbose (debug) mode") do |verbose|
        options.verbose = 'v'
      end
      opts.separator "Other options:"
      # No argument, shows at tail.  This will print an options summary.
      opts.on_tail("-h", "--help", "Show this message") do
        puts opts
        exit false
      end
    end
    opts.parse!(args)
    options
  end  # parse()
  
end  # class ParseOptions

options = ParseOptions.parse(ARGV)
if options.verbose == 'v'
  puts options
end


#
# utility functions
#
# print statement and execute as system call
def exec_statement(s)
  if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
    s = s.gsub("/", "\\")
  end
  puts "#{Time.now.getutc}: '#{s}'"
  result = system(s)
  puts "#{Time.now.getutc}: completed '#{s}'"
  return result
end

#
# extract arguments
#

modelPath = ""
if ARGV[0]
  if File::file?(ARGV[0])
    modelPath = File.expand_path(ARGV[0])
  end
end

if modelPath.empty?
  puts "No OpenStudio model provided, quitting. Try 'DaylightCalculations.rb -h' for options."
  exit false
end

radiancePath = ""

if ARGV[1]
  radiancePath = ARGV[1]
end


if radiancePath.empty?
  puts "No radiance path provided, quitting."
  exit false
end


sqlPath = ""
if ARGV[2]
  if File::file?(ARGV[2]) or File::directory?(ARGV[2])
    sqlPath = File.expand_path(ARGV[2])
  else
    puts "Invalid sql file given '#{ARGV[2]}'"
    exit false
  end
end


path = OpenStudio::Path.new(radiancePath).to_s
raypath = (OpenStudio::Path.new(radiancePath).parent_path() / OpenStudio::Path.new("lib")).to_s()

if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  perlpath = ""
  if OpenStudio::applicationIsRunningFromBuildDirectory()
    perlpath = OpenStudio::getApplicationRunDirectory().parent_path().parent_path() / OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
  else
    perlpath = OpenStudio::getApplicationRunDirectory().parent_path() / OpenStudio::Path.new("strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin")
  end
  puts "Adding path for local perl: " + perlpath.to_s
  ENV["PATH"] = ENV["PATH"] + ";" + path + ";" + perlpath.to_s
  ENV["RAYPATH"] = path + ";" + raypath
else
  ENV["PATH"] = ENV["PATH"] + ":" + path
  ENV["RAYPATH"] = path + ":" + raypath
end


puts "Setting RAYPATH: " + raypath + " " + path
puts "Appending PATH: " + path

dirname = File.dirname(__FILE__)

load_paths = ""
$LOAD_PATH.each {|load_path| load_paths += "-I '#{load_path}' "}

# execute ModelToRad
result = exec_statement("ruby #{load_paths} '#{dirname}/ModelToRad.rb' '#{modelPath}' '#{sqlPath}'")
if not result
  puts "failed to run ModelToRad"
  exit false
end

# if no sqlPath given, glob under the modelPath to find it
if sqlPath.empty?
  model_dirname = File.dirname(modelPath)
  model_basename = File.basename(modelPath, ".osm")
  files = Dir.glob("#{model_dirname}/#{model_basename}/model/radiance/**/eplusout.sql")
  if files.size == 1
    sqlPath = files[0]
  end
  if sqlPath.empty?
    puts "could not find sqlPath after running ModelToRad"
    exit false
  end
end

# execute DaylightSim to create daylighting coefficients
result = exec_statement("ruby #{load_paths} '#{dirname}/DaylightSim-Simple.rb' '#{modelPath}' '#{sqlPath}' --dc --x")
if not result
  puts "failed to run DaylightSim for daylight coefficients"
  exit false
end

# execute DaylightSim to run annual simulation
result = exec_statement("ruby #{load_paths} '#{dirname}/DaylightSim-Simple.rb' '#{modelPath}' '#{sqlPath}' --dcts --x")
if not result
  puts "failed to run DaylightSim for annual daylight simulation"
  exit false
end

# execute MakeSchedules
result = exec_statement("ruby #{load_paths} '#{dirname}/MakeSchedules.rb' '#{modelPath}' '#{sqlPath}' --keep")
if not result
  puts "failed to run MakeSchedules"
  exit false
end

FileUtils.copy("in/model/radiance/out.osm", "out.osm");
FileUtils.copy("in/model/radiance/output/radout.sql", "radout.sql");

# execute DaylightMetrics
result = exec_statement("ruby #{load_paths} '#{dirname}/DaylightMetrics.rb' '#{modelPath}' '#{sqlPath}' radout.sql")
if not result
  puts "failed to run DaylightMetrics"
  exit false
end

files = Dir.glob('*') - ['in/files']
FileUtils.mkdir_p 'out/files'
FileUtils.cp_r files, 'out/files'

weather = Dir.glob('in/files/*.epw')
FileUtils.cp weather, 'out/files/'
