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
#   Runs an OpenStudio Model through a workflow defined by an
#   OpenStudio::Runmanager::WorkItemVector that has been serialized to 
#   the model's companion folder as a json file. 
#
#   This script assumes an extended OpenStudio Application file 
#   structure, that is:
#
#     myModel.osm
#     myModel/
#       files/
#       run/
#       scripts/
#       run.db
#       work_items.json
#
#   This script re-uses run.db and the myModel/run/ directory, so make 
#   a copy of your model first if you want to preserve that data. In 
#   addition, this script collects log messages in a file 
#   myModel/RunWorkItems.log.
#
# == Usage
#
#   ruby RunWorkItems.rb ARGV[0] 
#     
#     ARGV[0] - Path to .osm file. 
#
# == Examples
#
#   The contents of the following test show how to set up an OpenStudio 
#   Model for use with this script.
#
#   ruby test/RunWorkItems_Test.rb
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'

# non-argument parameters
log_level = 0 # Warn (Info = -1, Debug = -2, Trace = -3)

# get file paths
osm = ARGV[0]
model_path = OpenStudio::Path.new(osm)
raise "The path '" + model_path.to_s + "' does not exist." if not File.exists?(model_path.to_s)

model_folder_path = model_path.parent_path / OpenStudio::Path.new(model_path.stem)
json_path = model_folder_path / OpenStudio::Path.new("work_items.json")
raise "Expected file '" + json_path.to_s + "' does not exist." if not File.exists?(json_path.to_s)

# model_folder_path exists, so create log file 
log_file = OpenStudio::FileLogSink.new(model_folder_path / OpenStudio::Path.new("RunWorkItems.log"))
log_file.setLogLevel(log_level)
OpenStudio::Logger::instance.standardOutLogger.disable

# open model
translator = OpenStudio::OSVersion::VersionTranslator.new
model = translator.loadModel(model_path)
raise "Unable to load OpenStudio model from '" + model_path.to_s + "'." if model.empty?
model = model.get

# open work_items.json
work_items = OpenStudio::Runmanager::toVectorOfWorkItem(json_path)
raise "No work items to run." if work_items.empty?

# create RunManager
run_manager_path = model_folder_path / OpenStudio::Path.new("run.db")
run_manager = OpenStudio::Runmanager::RunManager.new(run_manager_path, true, false, false)

# create workflow
workflow = OpenStudio::Runmanager::Workflow.new(work_items)
params = OpenStudio::Runmanager::JobParams.new;
params.append("cleanoutfiles", "standard");
workflow.add(params)

# set up tools
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8, 1)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s).parent_path
tools = OpenStudio::Runmanager::ConfigOptions::makeTools(ep_path,
                                                         OpenStudio::Path.new,
                                                         OpenStudio::Path.new,
                                                         $OpenStudio_RubyExeDir,
                                                         OpenStudio::Path.new)
workflow.add(tools)

# run workflow
weather_file_path = OpenStudio::Path.new
url_search_paths = OpenStudio::URLSearchPathVector.new
run_directory = model_folder_path / OpenStudio::Path.new("run")
job = workflow.create(run_directory, model_path, weather_file_path, url_search_paths)
OpenStudio::Runmanager::JobFactory::optimizeJobTree(job)
run_manager.enqueue(job, false);
run_manager.waitForFinished

