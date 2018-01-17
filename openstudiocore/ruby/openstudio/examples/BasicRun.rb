########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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

