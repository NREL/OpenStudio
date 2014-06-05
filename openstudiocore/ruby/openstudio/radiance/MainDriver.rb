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
#  ruby MainDriver.rb ARGV[0] (ARGV[1])
#
#  ARGV[0] - Path to OpenStudio Model
#
#  ARGV[1] - Optional path to EnergyPlus SQLite output, if not given will run EnergyPlus. 
#            EnergyPlus simulation must be pre-run with the SQLite output including tabular data 
#            and the EnvelopeSummary table must be requested.  The "Exterior 
#            Horizontal Illuminance From Sky", and "Exterior Beam Normal 
#            Illuminance" output variables must be requested at hourly reporting 
#            frequency.
#
# == Examples
#
#   ruby MainDriver.rb 'C:\path\to\model.osm' 
#   ruby MainDriver.rb 'C:\path\to\model.osm' 'C:\path\to\eplusout.sql' 
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
      opts.banner = "\nUsage: MainDriver.rb [ARGV[0] (ARGV[1]) [options]]\n\nARGV[0] - Path to OpenStudio Model\nARGV[1] - Optional path to EnergyPlus SQLite output, if not given will run EnergyPlus."
      opts.separator ""
      opts.separator "Optional variables:"        
      opts.on("-v", "--verbose", "Verbose (debug) mode") do |verbose|
        options.verbose = 'v'
      end
      opts.separator "Other options:"
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
  puts "'#{s}'"
  result = system(s)
  puts 
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
  puts "No OpenStudio model provided, quitting. Try 'MainDriver.rb -h' for options."
  exit
end

sqlPath = ""
if ARGV[1]
  if File::file?(ARGV[1]) or File::directory?(ARGV[1])
    sqlPath = File.expand_path(ARGV[1])
  else
    puts "Invalid sql file given '#{ARGV[1]}'"
    exit
  end
end

dirname = File.dirname(__FILE__)

load_paths = ""
$LOAD_PATH.each {|load_path| load_paths += "-I '#{load_path}' "}

# execute ModelToRad
result = exec_statement("ruby #{load_paths} #{dirname}/ModelToRad.rb #{modelPath} #{sqlPath}")
if not result
  puts "failed to run ModelToRad"
  exit
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
    exit
  end
end

# execute DaylightSim to create daylighting coefficients
result = exec_statement("ruby #{load_paths} #{dirname}/DaylightSim.rb #{modelPath} #{sqlPath} --dc --x")
if not result
  puts "failed to run DaylightSim"
  exit
end

# execute DaylightSim to run annual simulation
result = exec_statement("ruby #{load_paths} #{dirname}/DaylightSim.rb #{modelPath} #{sqlPath} --dcts --x")
if not result
  puts "failed to run DaylightSim"
  exit
end

# execute MakeSchedules
result = exec_statement("ruby #{load_paths} #{dirname}/MakeSchedules.rb #{modelPath} #{sqlPath} --keep")
if not result
  puts "failed to run MakeSchedules"
  exit
end
