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
#  ARGV[1] - true/false for whether to delete existing files (default=true).
#            If false, this script will essentially do nothing if the osp file,
#            db file, and projectDir exist.
#
# == Examples
#
#   ruby CreateExampleOptimizationProblem.rb './ExampleOptimizationProblem.osp'
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'openstudio/model/user_scripts/SummaryTable_Objects_Attributes_Relationships.rb'
require 'openstudio/ruleset/NonInteractiveUserScriptRunner.rb'

require 'fileutils'

include OpenStudio                      # to avoid the OpenStudio:: prefix


# extract script arguments
raise "At least one input argument is required." if (ARGV.size < 1)
ospPath = Path.new(ARGV[0])
deleteExistingFiles = true
if ARGV[1] and /[fF]alse/.match(ARGV[1])
  deleteExistingFiles = false
end
warn "Ignoring extra input arguments." if (ARGV.size > 2)


# file locations
rmdbPath = toString(ospPath.stem) + "Run.db"
rmdbPath = ospPath.parent_path / Path.new(rmdbPath)
projectDir = ospPath.parent_path / Path.new(ospPath.stem)


# EnergyPlus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)


# create/open ProjectDatabase
if deleteExistingFiles
  FileUtils.rm(rmdbPath.to_s) if exists(rmdbPath)
  FileUtils.rm(ospPath.to_s) if exists(ospPath)
  FileUtils.rm_r(projectDir.to_s) if exists(projectDir)
end
exit if exists(rmdbPath) and exists(ospPath) and exists(projectDir)
runManager = Runmanager::RunManager.new(rmdbPath,false,false,false)
database = Project::ProjectDatabase.new(ospPath,runManager)
FileUtils.mkdir(projectDir.to_s) if not exists(projectDir)

# set up logging
Logger::instance.standardOutLogger.disable
logFile = FileLogSink.new(projectDir / Path.new("project.log"))
logFile.setLogLevel(-2)

# save example.osm
exampleModelPath = projectDir / Path.new("example.osm")
puts "Saving model to " + exampleModelPath.to_s
Model::exampleModel.save(exampleModelPath,true)
exampleModelFileReferenceRecord = Project::FileReferenceRecord.new(
    FileReference.new(exampleModelPath),database)    
database.save


# load model (to make sure save worked properly)
model = Model::Model.load(exampleModelFileReferenceRecord.path)
raise "Unable to load example model." if model.empty?
model = model.get


# create and save summary table
summaryTablePath = projectDir / Path.new("ExampleModelSummary.csv")
puts "Creating attribute and relationship summary table at " + summaryTablePath.to_s + "."
tableScript = SummaryTableObjectsAttributesRelationships.new
args = tableScript.arguments(model)
# next three lines should be one, but swig bindings seem to need the breakout
csvArg = args[0]; 
csvArg.setValue(summaryTablePath);
args[0] = csvArg
scriptRunner = NonInteractiveUserScriptRunner.new
argMap = scriptRunner.getUserInput(args)
tableScript.run(model,scriptRunner,argMap)
summaryTableFileReferenceRecord = Project::FileReferenceRecord.new(FileReference.new(summaryTablePath),database)
database.save

# create problem

# create component library
componentLibraryPath = projectDir.parent_path / Path.new("LocalComponentLibrary.osp")
puts "Creating component library at " + componentLibraryPath.to_s + "."
systemCallString = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/EnergyPlusDataSetsComponentLibrary.rb' "
systemCallString << "'" << componentLibraryPath.to_s << "'" 
if (ARGV.size > 1) 
  systemCallString << " " << ARGV[1]
end
puts "Making system call: " + systemCallString
system(systemCallString)

# open component library
componentLibraryRmdbPath = toString(componentLibraryPath.stem) + "Run.db"
componentLibraryRmdbPath = componentLibraryPath.parent_path / Path.new(componentLibraryRmdbPath)
componentLibraryRunManager = Runmanager::RunManager.new(componentLibraryRmdbPath,false,false,false)
componentLibrary = Project::ProjectDatabase.new(componentLibraryPath,componentLibraryRunManager)

# define variables
puts "Setting up problem variables."
variables = Analysis::VariableVector.new
measures = Analysis::MeasureVector.new

  # variable 1: exterior window construction
  measures.clear
  measures.push(Analysis::NullMeasure.new)
  # create measures from osc files
  componentNames = []
  componentNames.push("Dbl Clr 6mm_13mm Arg")
  componentNames.push("Dbl LoE (e3=.1) Clr 3mm_6mm Air")
  componentNames.push("Trp LoE (e2=e5=.1) Clr 3mm_13mm Arg")
  componentNames.push("Quadruple LoE Films (88) 3mm_8mm Krypton")
  # add component data to model
  componentNames.each { |componentName|
    componentRecord = Project::getFileReferenceRecordByName(componentLibrary,componentName).get
    componentPath = componentRecord.path
    component = OpenStudio::Model::Component::load(componentPath).get
    model.insertComponent(component)
  }
  # save model
  model.save(exampleModelPath,true)
  # download measure from BCL
  remoteBCL = OpenStudio::RemoteBCL.new
  bclMeasure = remoteBCL.getMeasure("e9cc8132-fe83-4cd6-a168-8c7637b04e12").get
  # get measure arguments for model
  args = OpenStudio::Ruleset::getArguments(bclMeasure,
                                           OpenStudio::Model::OptionalModel.new(model))
  runner = OpenStudio::Ruleset::OSRunner.new
  argMap = runner.getUserInput(args)
  # create measure instances based on componentNames
  componentNames.each { |componentName|
    rubyMeasure = OpenStudio::Analysis::RubyMeasure.new(bclMeasure)
    arg = argMap["construction"]
    arg.setValue(componentName)
    rubyMeasure.setArgument(arg)
    measures.push(rubyMeasure)
  }
  variables.push(Analysis::MeasureGroup.new("Exterior Window Construction",measures))

  # variable 2: exterior wall construction

  # variable 3: exterior roof construction

  # variable 4: plug load density
  measures.clear
  perturbScript = OpenStudio::Path.new($OpenStudio_LibPath + "openstudio/runmanager/rubyscripts/PerturbObject.rb")
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:ElectricEquipment:Definition")
  template.addArgument("nameRegex", "Electric Equipment Definition [0-9]+") # Watts/Area
  template.addArgument("field", "4") # Watts per Space Floor Area
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "5.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","4.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","3.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Plug Load Density",measures))

  # variable 5: lighting power density
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:Lights:Definition")
  template.addArgument("nameRegex", ".*")
  template.addArgument("field", "4") # Watts per Space Floor Area
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "10.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","8.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Lighting Power Density",measures))

  # variable 6: building rotation
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:Building")
  template.addArgument("nameRegex", ".*")
  template.addArgument("field", "3") # North Axis
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "0.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","90.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","180.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","270.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Building Rotation",measures))

  # variable 7: number of printers
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:ElectricEquipment")
  template.addArgument("nameRegex", "Printer")
  template.addArgument("field", "5") # Multiplier
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "2.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","1.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Number of Printers",measures))

  # variable 8: daylighting fraction
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:Lights")
  template.addArgument("nameRegex", ".*")
  template.addArgument("field", "5") # Fraction Replacable
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "0.5")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","1.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Daylighting Fraction",measures))

  # variable 9: overhangs
  measures.clear
  measures.push(Analysis::NullMeasure.new)
  overhangScript = Path.new($OpenStudio_LibPath + "openstudio/examples/rubyscripts/AddOverhang.rb")
  measure = Analysis::RubyMeasure.new(overhangScript, 
                                      "OSM".to_FileReferenceType, 
                                      "OSM".to_FileReferenceType)
  measure.addArgument("projectionfactor","0.5")
  measure.addArgument("offsetfraction","0.1")
  measures.push(measure)
  measure = Analysis::RubyMeasure.new(overhangScript, 
                                      "OSM".to_FileReferenceType, 
                                      "OSM".to_FileReferenceType)
  measure.addArgument("projectionfactor","1.0")
  measure.addArgument("offsetfraction","0.1")
  measures.push(measure)
  measure = Analysis::RubyMeasure.new(overhangScript, 
                                      "OSM".to_FileReferenceType, 
                                      "OSM".to_FileReferenceType)
  measure.addArgument("projectionfactor","1.5")
  measure.addArgument("offsetfraction","0.1")
  measures.push(measure)    
  variables.push(Analysis::MeasureGroup.new("Overhangs",measures))

  # variable 10: evaporative cooler effectiveness
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                      "OSM".to_FileReferenceType,
                                                      "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:EvaporativeCooler:Direct:ResearchSpecial")
  template.addArgument("nameRegex", ".*")
  template.addArgument("field", "3") # Cooler Effectiveness
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "0.6")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","0.8")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","0.9")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Evaporative Cooler Effectiveness",measures))

  # variable 11: fan characteristics
  measures.clear
  measures.push(Analysis::NullMeasure.new)

  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                   "OSM".to_FileReferenceType,
                                                   "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:Fan:ConstantVolume")
  template.addArgument("nameRegex", ".*")
  # option package 1
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("field", "3") # Fan Efficiency
  rubyMeasure.addArgument("value", "0.8")
  rubyMeasure.addArgument("field", "4") # Pressure Rise
  rubyMeasure.addArgument("value", "225.0")
  rubyMeasure.addArgument("field", "6") # Motor Efficiency
  rubyMeasure.addArgument("value", "0.92")
  measures.push(rubyMeasure)
  # option package 2
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("field", "3") # Fan Efficiency
  rubyMeasure.addArgument("value", "0.9")
  rubyMeasure.addArgument("field", "4") # Pressure Rise
  rubyMeasure.addArgument("value", "200.0")
  rubyMeasure.addArgument("field", "6") # Motor Efficiency
  rubyMeasure.addArgument("value", "0.96")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Fan Performance",measures))

  # variable 12: min supply air temperature
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                   "OSM".to_FileReferenceType,
                                                   "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:SetpointManager:SingleZone:Reheat")
  template.addArgument("nameRegex", ".*")
  template.addArgument("field", "2") # Minimum Supply Air Temperature
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "15.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","10.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","5.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Minimum Supply Air Temperature",measures))

  # variable 13: max supply air temperature
  measures.clear
  template = OpenStudio::Analysis::RubyMeasure.new(perturbScript,
                                                   "OSM".to_FileReferenceType,
                                                   "OSM".to_FileReferenceType)
  template.addArgument("inputPath","in.osm")
  template.addArgument("outputPath", "out.osm")
  template.addArgument("objectType", "OS:SetpointManager:SingleZone:Reheat")
  template.addArgument("nameRegex", ".*")
  template.addArgument("field", "3") # Maximum Supply Air Temperature
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value", "18.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","22.0")
  measures.push(rubyMeasure)
  rubyMeasure = template.clone.to_RubyMeasure.get
  rubyMeasure.addArgument("value","26.0")
  measures.push(rubyMeasure)
  variables.push(OpenStudio::Analysis::MeasureGroup.new("Maximum Supply Air Temperature",measures))

  # variable 14: heating setpoints

  # variable 15: cooling setpoints


# define objective functions
puts "Setting up problem objective functions."
objectiveFunctions = Analysis::FunctionVector.new
objectiveVariable = Analysis::OutputAttributeContinuousVariable.new("Cooling Electricity Use","EndUses.Electricity.Cooling.General")
objective = Analysis::LinearFunction.new(objectiveVariable.name(),Analysis::VariableVector.new(1,objectiveVariable))
objectiveFunctions.push(objective)
objectiveVariable = Analysis::OutputAttributeContinuousVariable.new("Heating Gas Use","EndUses.Gas.Heating.General")
objective = Analysis::LinearFunction.new(objectiveVariable.name(),Analysis::VariableVector.new(1,objectiveVariable))
objectiveFunctions.push(objective)

# define simulation workflow
puts "Defining the simulation workflow."
workflow = Runmanager::Workflow.new
workflow.addJob("ModelToIdf".to_JobType)
workflow.addJob("EnergyPlus".to_JobType)
workflow.addJob("OpenStudioPostProcess".to_JobType)
workflow.add(Runmanager::ConfigOptions::makeTools(
    ep_path.parent_path,
    Path.new,
    Path.new,
    $OpenStudio_RubyExeDir,
    Path.new))

# define problem
puts "Creating and saving the optimization problem."
optimizationProblem = Analysis::OptimizationProblem.new("Example Optimization Problem",
                                                        objectiveFunctions,
                                                        variables,
                                                        workflow)

# save in database
optimizationProblemRecord = Project::OptimizationProblemRecord.new(optimizationProblem,database)
database.save
