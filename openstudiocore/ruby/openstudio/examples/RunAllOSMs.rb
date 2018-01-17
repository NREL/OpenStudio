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
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,5)
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
