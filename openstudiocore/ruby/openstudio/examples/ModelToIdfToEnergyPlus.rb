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
#   Run a specific osm file, passing it through ModelToIdf and EnergyPlusJob
#   using runmanager to run the simulation
#
# == Usage
#
#   ruby ModelToIdfToEnergyPlus.rb ARGV[0] 
#
#   ARGV[0] - osm job to run
#
# == Examples
#
#   ruby RunExampleFiles.rb mytest.osm
#
######################################################################

require 'openstudio'

osm_path = OpenStudio::Path.new(ARGV[0])

# find energyplus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,5)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
idd_path = OpenStudio::Path.new(ep_hash[:energyplus_idd].to_s)
ep_dir = File.dirname(ep_path.to_s)

expandobjects_path = ''
if (/mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM))
  expandobjects_path = ep_dir + '/ExpandObjects.exe'
else
  expandobjects_path = ep_dir + '/expandobjects'
end

readvars_path = ""
if (/mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM))
  readvars_path = ep_dir + '/PostProcess/ReadVarsESO.exe'
else
  readvars_path = ep_dir + '/readvars'
end

# just find first weather file for now
weatherDir = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s)
weatherGlobSearch = weatherDir / OpenStudio::Path.new("*.epw")
weatherFilePath = Dir.glob(weatherGlobSearch.to_s).first
epw_path = OpenStudio::Path.new(weatherFilePath)

# make a run manager
run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::Path.new("runmanager.db"))

parentpath = OpenStudio::Path.new();

# develop an intelligent output directory location
if osm_path.has_parent_path
  parentpath = OpenStudio::Path.new(osm_path.parent_path().filename())
end

outdir = OpenStudio::tempDir() / OpenStudio::Path.new("ModelToIdfExample") / (osm_path.stem().to_s == "in"? parentpath : OpenStudio::Path.new(osm_path.stem()))

# our out dir is now something like:
# /tmp/ModelToIdfExample/InputFile
#
# Our overall created directory tree will be something like:
# /tmp/ModelToIdfExample/InputFile/
#                                  ModelToIdf/
#                                             in.osm
#                                             in.idf
#                                             in.epw
#                                             EnergyPlusJob/
#                                                           in.idf
#                                                           in.epw
#                                                           Energy+.idd
#                                                           eplusout.*

# Show the jobs status as it processes
run_manager.showStatusDialog()

# create the "set of tools" needed. 
tools = OpenStudio::Runmanager::Tools.new()
tools.append(OpenStudio::Runmanager::ToolInfo.new("energyplus", OpenStudio::Runmanager::ToolVersion.new(7,2), ep_path))
tools.append(OpenStudio::Runmanager::ToolInfo.new("expandobjects", OpenStudio::Runmanager::ToolVersion.new(), OpenStudio::Path.new(expandobjects_path)))
tools.append(OpenStudio::Runmanager::ToolInfo.new("readvars", OpenStudio::Runmanager::ToolVersion.new(), OpenStudio::Path.new(readvars_path)))

# Create the modelToIdf job and give it the osm path and the output directory
modeltoidfjob = OpenStudio::Runmanager::JobFactory::createModelToIdfJob(osm_path, outdir)

# expandbjects job
expandobjectsjob = OpenStudio::Runmanager::JobFactory::createExpandObjectsJob(tools, OpenStudio::Runmanager::JobParams.new(), OpenStudio::Runmanager::Files.new())

# now, create an EnergyPlusJob, passing it the set of tools
# It does not need any files or params, because it is getting those (in.idf and outdir, respectively)
# from its parent job
eplusjob = OpenStudio::Runmanager::JobFactory::createEnergyPlusJob(tools, OpenStudio::Runmanager::JobParams.new(), OpenStudio::Runmanager::Files.new())
      
# make the readvars job, could take an arbitrary list of rvi files to use
rvi_files = OpenStudio::Runmanager::Files.new()
readvarsjob = OpenStudio::Runmanager::JobFactory::createReadVarsJob(tools, OpenStudio::Runmanager::JobParams.new(), rvi_files)
      
# now that both jobs are created, establish the parent/child relationship
modeltoidfjob.addChild(expandobjectsjob)
expandobjectsjob.addChild(eplusjob)
eplusjob.addChild(readvarsjob)

# Queue up the parent, which also queues any children
run_manager.enqueue(modeltoidfjob, true)

# wait for jobs to complete
while run_manager.workPending()
  sleep 0.1
  OpenStudio::Application::instance().processEvents()
end


run_manager.hideStatusDialog()

# now let's see what output files we generated
# maybe there should be another way to do this, say to ask the parent for all output files generated by the children

puts "Output files generated by child: "

files = eplusjob.outputFiles()

files.each do |f|
  puts f.fullPath.to_s
end


