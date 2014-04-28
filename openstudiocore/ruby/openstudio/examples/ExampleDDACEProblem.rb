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
#   Construct an optimization problem tailored for Model::exampleModel().
#   Save the problem and the exampleModel seed file to a specified 
#   location (osp file and folder containing example.osm). In the process
#   of creating the problem, calls EnergyPlusDataSetsComponentLibrary, 
#   placing the resulting local component library in the same location as
#   its own osp file.
#
# == Usage
#
#  ruby CreateExampleOptimizationProblem.rb ARGV[0] 
#
#  ARGV[0] - Path to ProjectDatabase osp file
#
#  ARGV[1] - number of samples
#
#  ARGV[2] - Run EnergyPlus simulations or not, 'true' to run simulations, 
#            default is 'false'. 
#
#  ARGV[3] - Show RunManager GUI, 'true' to show, default is 'false'. 
#
#  ARGV[4] - true/false for whether to delete existing files (default=true).
#            If false, this script will essentially do nothing if the osp file,
#            db file, and projectDir exist.
#
# == Examples
#
#   ruby ExampleDDACEProblem.rb './ExampleProblem.osp' #samples (true/false) (true/false) (true/false)
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'openstudio/analysis/find_dakota'
require 'fileutils'

include OpenStudio                      # to avoid the OpenStudio:: prefix

# options not exposed to command line
weatherFileName = "USA_CO_Golden-NREL.724666_TMY3.epw"

# extract script arguments
raise "At least one input argument is required." if (ARGV.size < 1)
ospPath = Path.new(ARGV[0])
samples = ARGV[1]
puts "LHS DDACE problem with " + samples + " samples"

# should we run EnergyPlus or not
run_energyplus = false
if ARGV[2] and /[tT]rue/.match(ARGV[2])
  run_energyplus = true
end

# should we show the GUI or not
show_gui = false
if ARGV[3] and /[tT]rue/.match(ARGV[3])
  show_gui = true
end

deleteExistingFiles = true
if ARGV[4] and /[fF]alse/.match(ARGV[4])
  deleteExistingFiles = false
end
warn "Ignoring extra input arguments." if (ARGV.size > 5)


# file locations
rmdbPath = toString(ospPath.stem) + "Run.db"
rmdbPath = ospPath.parent_path / Path.new(rmdbPath)
projectDir = ospPath.parent_path / Path.new(ospPath.stem)


# EnergyPlus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
weatherData_path = Path.new(ep_hash[:energyplus_weatherdata].to_s)


# Daktoa
dakota_hash = OpenStudio::Analysis::find_dakota(5,3,1)
if dakota_hash.nil?
  puts "Cannot run this script without Dakota. Please install Dakota (in a standard location) and try again."
  return
end

# create/open ProjectDatabase
if deleteExistingFiles
  FileUtils.rm(rmdbPath.to_s) if exists(rmdbPath)
  FileUtils.rm(ospPath.to_s) if exists(ospPath)
  FileUtils.rm_r(projectDir.to_s) if exists(projectDir)
end
exit if exists(rmdbPath) and exists(ospPath) and exists(projectDir)
runManager = Runmanager::RunManager.new(rmdbPath, false, false, show_gui)
database = Project::ProjectDatabase.new(ospPath, runManager)
FileUtils.mkdir(projectDir.to_s) if not exists(projectDir)

# save example.osm
exampleModelPath = projectDir / Path.new("example.osm")
Model::exampleModel.save(exampleModelPath,true)
exampleModelFileReferenceRecord = Project::FileReferenceRecord.new(
    FileReference.new(exampleModelPath),database)    
database.save

# define variables
variables = Analysis::VariableVector.new

  # variable 4: plug load density
  perturbScript = OpenStudio::Path.new($OpenStudio_LibPath + "openstudio/runmanager/rubyscripts/PerturbObject.rb")
  rubyMeasure = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  rubyMeasure.addArgument("inputPath","in.osm")
  rubyMeasure.addArgument("outputPath", "out.osm")
  rubyMeasure.addArgument("objectType", "OS:ElectricEquipment:Definition")
  rubyMeasure.addArgument("nameRegex", "Electric Equipment Definition [0-9]+") # Watts/Area
  rubyMeasure.addArgument("field", "4") # Watts per Space Floor Area
  continuousVariable = Analysis::RubyContinuousVariable.new(
      "Plug Load Density",
      OpenStudio::Ruleset::OSArgument::makeDoubleArgument("value"),
      rubyMeasure)
  continuousVariable.setMinimum(0.0)
  continuousVariable.setMaximum(100.0)
  variables.push(continuousVariable)

  # variable 5: lighting power density
  rubyMeasure = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  rubyMeasure.addArgument("inputPath","in.osm")
  rubyMeasure.addArgument("outputPath", "out.osm")
  rubyMeasure.addArgument("objectType", "OS:Lights:Definition")
  rubyMeasure.addArgument("nameRegex", ".*")
  rubyMeasure.addArgument("field", "4") # Watts per Space Floor Area
  continuousVariable = Analysis::RubyContinuousVariable.new(
      "Lighting Power Density",
      OpenStudio::Ruleset::OSArgument::makeDoubleArgument("value"),
      rubyMeasure)
  continuousVariable.setMinimum(0.0)
  continuousVariable.setMaximum(100.0)
  variables.push(continuousVariable)

# define simulation workflow
workflow = Runmanager::Workflow.new
workflow.addJob("ModelToIdf".to_JobType)
if run_energyplus
  workflow.addJob("EnergyPlus".to_JobType)
  workflow.addJob("OpenStudioPostProcess".to_JobType)
end

# add tools
workflow.add(Runmanager::ConfigOptions::makeTools(
    ep_path.parent_path,
    Path.new,
    Path.new,
    $OpenStudio_RubyExeDir,
    Path.new))

# define problem
problem = Analysis::Problem.new("Example LHS Problem", variables, workflow)

# set algorithm parameters
options = Analysis::DDACEAlgorithmOptions.new("lhs".to_DDACEAlgorithmType)
options.setSamples(samples.to_i)
algorithm = Analysis::DDACEAlgorithm.new(options)

seed = Project::getFileReferenceRecordByName(database, exampleModelPath.to_s).get.fileReference
model = Model::Model::load(seed.path).get
weatherFile = EpwFile.new(weatherData_path / Path.new(weatherFileName))
Model::WeatherFile::setWeatherFile(model,weatherFile)
model.save(seed.path,true)                                  # overwrite with weather data included


# create analysis
analysis = Analysis::Analysis.new("Example LHS Analysis",problem,algorithm,seed)


# run analysis
analysisDriver = AnalysisDriver::AnalysisDriver.new(database)
runOptions = AnalysisDriver::AnalysisRunOptions.new(projectDir,
                                                    OpenStudio::Path.new($OpenStudio_Dir),
                                                    OpenStudio::Path.new(dakota_hash[:dakota_exe].to_s))
runOptions.setQueueSize(8)
#test = runOptions.dakotaExePath
#puts test
currentAnalysis = analysisDriver.run(analysis,runOptions)
if show_gui
  runManager.showStatusDialog
end
analysisDriver.waitForFinished




