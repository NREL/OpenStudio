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
#   Run osm files in a directory structure falling out from ARGV[0]. 
#   The osm files should have WeatherFile objects, with paths relative
#   to ARGV[1], the weather file directory.
#
# == Usage
#
#   ruby RunAllOSMs.rb ARGV[0] ARGV[1] ARGV[2]
#   ruby RunAllOSMs.rb ARGV[0] ARGV[1] ARGV[2] ARGV[3]
#     
#     ARGV[0] - Path to folder containing osm files.
#     ARGV[1] - Path to folder containing weather files.
#     ARGV[2] - Output directory.
#     ARGV[3] - Number of simulations to run.
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'

osmDir = OpenStudio::Path.new(ARGV[0])
weatherFileDir = OpenStudio::system_complete(OpenStudio::Path.new(ARGV[1]))
outputDir = OpenStudio::Path.new(ARGV[2])

nSim = OpenStudio::OptionalInt.new
nSim = OpenStudio::OptionalInt.new(ARGV[3].to_i) if ARGV.size > 3

OpenStudio::create_directory(outputDir)
runManagerDBPath = outputDir / OpenStudio::Path.new("RunManager.db")
puts "Creating RunManager database at " + runManagerDBPath.to_s + "."
OpenStudio::remove(runManagerDBPath) if (OpenStudio::exists(runManagerDBPath))
runManager = OpenStudio::Runmanager::RunManager.new(runManagerDBPath,true)

# find energyplus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)

tools = OpenStudio::Runmanager::ConfigOptions::makeTools(ep_path.parent_path(),
                                                         OpenStudio::Path.new,
                                                         OpenStudio::Path.new,
                                                         OpenStudio::Path.new,
                                                         OpenStudio::Path.new)

filenames = Dir.glob(osmDir.to_s + "./**/*.osm")

n = 0
filenames.each { |filename|
  break if (not nSim.empty?) && (nSim.get <= n)

  # copy osm file
  relativeFilePath = OpenStudio::relativePath(OpenStudio::Path.new(filename),osmDir)
  puts "Queuing simulation job for " + relativeFilePath.to_s + "."
  
  originalOsmPath = osmDir / relativeFilePath
  outputOsmPath = outputDir / relativeFilePath 
  puts "Copying '" + originalOsmPath.to_s + "' to '" + outputOsmPath.to_s + "'."
  OpenStudio::makeParentFolder(outputOsmPath,OpenStudio::Path.new,true)
  OpenStudio::copy_file(originalOsmPath,outputOsmPath)

  # create workflow
  workflow = OpenStudio::Runmanager::Workflow.new("modeltoidf->energyplus->openstudiopostprocess")
  workflow.setInputFiles(outputOsmPath,weatherFileDir)
  workflow.add(tools)
  
  # create and queue job
  jobDirectory = outputOsmPath.parent_path() / OpenStudio::Path.new(outputOsmPath.stem()) / OpenStudio::Path.new("/")
  puts "Job directory will be '" + jobDirectory.to_s + "'."
  job = workflow.create(jobDirectory)
  runManager.enqueue(job, true)
  
  n = n + 1
}

# wait for finished
runManager.showStatusDialog
runManager.waitForFinished

runManager.getJobs.each { |job| 

  if not job.errors.succeeded   
    puts "The job in '" + job.outdir.to_s + "' did not finish successfully."  
  elsif not job.errors.warnings.empty?
    puts "The job in '" + job.outdir.to_s + "' has warnings."
  end
  
  job.errors.errors.each { |err|
    puts "ERROR: " + err
  }
  
  # job.errors.warnings.each { |warn|
  #   puts "WARN: " + warn
  # }
  
}
