######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
#   Run all of the EnergyPlus example files, optionally provide an argument
#   to run only the first N.
#
# == Usage
#
#   ruby RunExampleFiles.rb ARGV[0] 
#
#   ARGV[0] - Maximum number of simulations to run
#
# == Examples
#
#   ruby RunExampleFiles.rb
#   ruby RunExampleFiles.rb 10
#
######################################################################

require 'openstudio'

# maximum simulations, may be nil
max_simulations = nil
if ARGV[0].to_i > 0
  max_simulations = ARGV[0].to_i
end

# find energyplus
co = OpenStudio::Runmanager::ConfigOptions.new
co.fastFindEnergyPlus()
weather_path = co.getDefaultEPWLocation
examples_path = weather_path / OpenStudio::Path.new("../ExampleFiles/")

# just find first weather file for now
weather_paths = Dir.glob(weather_path.to_s + "/*.epw")
raise "Unable to find weather files." if weather_paths.empty?
epw_path = OpenStudio::Path.new(weather_paths.first)

# make a run manager
run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::Path.new("runmanager-runexamplefiles.db"), true)

# make a job for each example file
Dir.glob(examples_path.to_s + "*.idf").each do |f|

  # we don't handle parametric preprocessor
  next if /1ZoneParameterAspect.idf/.match(f)
  
  idf_path = OpenStudio::Path.new(f)
  out_dir = OpenStudio::tempDir() / OpenStudio::Path.new("RunExampleFiles") / OpenStudio::Path.new(OpenStudio::Path.new(f).filename())
  
  puts "Running '#{idf_path}' in '#{out_dir}'"
  
  workflow = OpenStudio::Runmanager::Workflow.new("EnergyPlus")
  workflow.add(co.getTools)
  
  job = workflow.create(out_dir, idf_path, epw_path)
    
  run_manager.enqueue(job, false)
  
  # check if past max simulations
  break if max_simulations and run_manager.getJobs.size >= max_simulations
end

# start and wait for jobs to complete
run_manager.setPaused(false)
run_manager.waitForFinished
