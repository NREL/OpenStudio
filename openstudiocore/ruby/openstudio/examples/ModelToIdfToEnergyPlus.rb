######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

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
require 'openstudio/energyplus/find_energyplus'

osm_path = OpenStudio::Path.new(ARGV[0])

# find energyplus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
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


