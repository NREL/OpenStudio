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
co = OpenStudio::Runmanager::ConfigOptions.new
co.fastFindEnergyPlus()
co.fastFindRuby()
workflow.add(co.getTools)

# run workflow
weather_file_path = OpenStudio::Path.new
url_search_paths = OpenStudio::URLSearchPathVector.new
run_directory = model_folder_path / OpenStudio::Path.new("run")
job = workflow.create(run_directory, model_path, weather_file_path, url_search_paths)
OpenStudio::Runmanager::JobFactory::optimizeJobTree(job)
run_manager.enqueue(job, false);
run_manager.waitForFinished

