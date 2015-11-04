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
#   For the given IDF and EPW files, ensures that a SQL file will be 
#   generated, runs the EnergyPlus simulation, places select output 
#   files in outputDirectory and launches ResultsViewer. If no 
#   outputDirectory is specified, places the output files in 
#   idfPathAndFileName's parent directory.
#
# == Usage
#
#   ruby basic_run.rb ARGV[0] ARGV[1] ARGV[2]
#     
#     ARGV[0] - idfPathAndFilename 
#     ARGV[1] - weatherPathAndFilename 
#     ARGV[2] - (optional) outputDirectory
#     ARGV[3] - bool - (optional) launchResultsViewer (defaults to true)
#
# == Examples
# 
#   From your EnergyPlus directory:
#
#   ruby BasicRun.rb ExampleFiles\5ZoneAirCooled.idf WeatherData\USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw
#
######################################################################

require 'openstudio'

require 'openstudio/utilities/IdfHelpers.rb'
require 'openstudio/runmanager/RunManagerHelpers.rb'

# configure logging
OpenStudio::Logger.instance.standardOutLogger.enable
OpenStudio::Logger.instance.standardOutLogger.setLogLevel(0)

if ARGV.size() < 1
  raise "An IDF file path and name is required to do a basic_run."
end
if ARGV.size() < 2
  raise "An EPW file path and name is required to do a basic_run."
end

# load idf file into workspace
idf = ARGV[0]
epw = ARGV[1]
puts "Loading Idf from path " + idf + "."
puts "Using EPW from path " + epw + "."
workspace = OpenStudio::Workspace::load(OpenStudio::Path.new(idf))
raise "Unable to load '" + idf + "' into an EnergyPlus Workspace." if workspace.empty?
workspace = workspace.get

# ensure sql file will be generated
puts "Placing output in Sql file."
workspace = ensureSqlOutput(workspace)

# run simulation and place output files in outputDirectory
outputDirectory = OpenStudio::Path.new(idf).parent_path() / OpenStudio::Path.new("rundir");
if (ARGV.size() >= 3 and (not ARGV[2].empty?))
  outputDirectory = OpenStudio::Path.new(ARGV[2])
end
puts "Running simulation."
sql_path = simulateOneWorkspace(workspace,epw,outputDirectory)
if sql_path.empty 
  raise "Simulation at " + outputDirectory.to_s + " did not complete successfully." 
end

launchResultsViewer = true
if ARGV.size >= 4 and ARGV[3].match(/(false|f|no|0)$/i)
  launchResultsViewer = false
end

# launch ResultsViewer
if (launchResultsViewer and sql_path and (not sql_path.empty()))
  config_path = OpenStudio::Path.new(Dir.getwd) / OpenStudio::Path.new("config.rb")
  results_viewer_path = OpenStudio::Path.new($OpenStudio_BinaryDir) / OpenStudio::Path.new("ResultsViewer")
  
  command_str = nil
  if RUBY_PLATFORM =~ /win32/
    command_str = results_viewer_path.to_s + " " + sql_path.to_s
  elsif RUBY_PLATFORM =~ /linux/
    command_str = results_viewer_path.to_s + " " + sql_path.to_s
  elsif RUBY_PLATFORM =~ /darwin/
    command_str = "open " + results_viewer_path.to_s + " --args " + sql_path.to_s
  end

  if command_str
    puts "Making system call: " + command_str
    system( command_str )
  end
  
end

