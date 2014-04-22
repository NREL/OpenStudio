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
require 'openstudio/energyplus/find_energyplus'

# maximum simulations, may be nil
max_simulations = nil
if ARGV[0].to_i > 0
  max_simulations = ARGV[0].to_i
end

# find energyplus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
idd_path = OpenStudio::Path.new(ep_hash[:energyplus_idd].to_s)
weather_path = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s)

# just find first weather file for now
weather_paths = Dir.glob(weather_path.to_s + "/*.epw")
raise "Unable to find weather files." if weather_paths.empty?
epw_path = OpenStudio::Path.new(weather_paths.first)

# make a run manager
run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::Path.new("runmanager-runexamplefiles.db"), true)

# make a job for each example file
Dir.glob(ep_hash[:energyplus_examplefiles] + "*.idf").each do |f|

  # we don't handle parametric preprocessor
  next if /1ZoneParameterAspect.idf/.match(f)
  
  puts "Running '#{f}'"

  idf_path = OpenStudio::Path.new(f)
  job = OpenStudio::Runmanager::JobFactory::createEnergyPlusJob(OpenStudio::Runmanager::ToolInfo.new(ep_path), idd_path, idf_path, epw_path, OpenStudio::tempDir()/OpenStudio::Path.new("RunExampleFiles")/OpenStudio::Path.new(OpenStudio::Path.new(f).filename()) )
  run_manager.enqueue(job, false)
  
  # check if past max simulations
  break if max_simulations and run_manager.getJobs.size >= max_simulations
end

# wait for jobs to complete
while run_manager.workPending()
  sleep 1
  OpenStudio::Application::instance().processEvents()
end
