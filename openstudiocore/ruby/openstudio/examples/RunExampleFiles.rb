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
