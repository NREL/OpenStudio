######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,0)
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
perturbations = Analysis::DiscretePerturbationVector.new

  # variable 1: exterior window construction
  perturbations.clear
  perturbations.push(Analysis::NullPerturbation.new)
  # create basic rule
  ruleTemplate = Ruleset::ModelRule.new("Template")
  filters = Ruleset::FilterClauseVector.new
  filter = Ruleset::ModelObjectFilterType.new("OS:DefaultSubSurfaceConstructions".to_IddObjectType)
  filter = Ruleset::ModelObjectFilterStringAttribute.new("name",
                                                         "IEquals".to_RulesetStringPredicate,
                                                         "Exterior SubSurface Constructions")
  filters.push(filter)
  ruleTemplate.add(filters)
  # create perturbations by referencing osc files
  componentNames = []
  componentNames.push("Dbl Clr 6mm_13mm Arg")
  componentNames.push("Dbl LoE (e3=.1) Clr 3mm_6mm Air")
  componentNames.push("Trp LoE (e2=e5=.1) Clr 3mm_13mm Arg")
  componentNames.push("Quadruple LoE Films (88) 3mm_8mm Krypton")
  componentNames.each { |componentName|
    componentRecord = Project::getFileReferenceRecordByName(componentLibrary,componentName).get
    componentPath = componentRecord.path
    rule = Ruleset::ModelRule.new("Set Exterior Window Construction to '" + componentName + "'")
    rule.add(ruleTemplate.filters)
    action = Ruleset::ModelObjectActionSetRelationship.new("fixedWindowConstruction",componentPath)
    rule.add(action)
    ruleset = Ruleset::ModelRuleset.new(rule.name())
    ruleset.add(rule)
    perturbations.push(Analysis::ModelRulesetPerturbation.new(ruleset))
  }
  variables.push(Analysis::DiscreteVariable.new("Exterior Window Construction",perturbations))

  # variable 2: exterior wall construction

  # variable 3: exterior roof construction

  # variable 4: plug load density
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:ElectricEquipment:Definition".to_IddObjectType))
  filters.push(Ruleset::ModelObjectFilterStringAttribute.new("designLevelCalculationMethod",
                                                             "IEquals".to_RulesetStringPredicate,
                                                             "Watts/Area"))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Plug Load Density",
                                                                    filters,
                                                                    "wattsperSpaceFloorArea")
  values = []
  values.push(5.0) # baseline because first
  values.push(4.0)
  values.push(3.0)
  variables.push(continuousVariable.discretize(values))

  # variable 5: lighting power density
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:Lights:Definition".to_IddObjectType))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Lighting Power Density",
                                                                    filters,
                                                                    "wattsperSpaceFloorArea") 
  values = []
  values.push(10.0) # baseline because first
  values.push(8.0)
  variables.push(continuousVariable.discretize(values))

  # variable 6: building rotation
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:Building".to_IddObjectType))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Building Rotation",
                                                                    filters,
                                                                    "northAxis")    
  continuousVariable.setMinimum(0.0)
  continuousVariable.setMaximum(270.0)
  continuousVariable.setIncrement(90.0)
  variables.push(continuousVariable.discretize)

  # variable 7: number of printers
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:ElectricEquipment".to_IddObjectType))
  filters.push(Ruleset::ModelObjectFilterStringAttribute.new("name",
                                                             "IEquals".to_RulesetStringPredicate,
                                                             "Printer"))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Number of Printers",
                                                                    filters,
                                                                    "multiplier")
  values = []
  values.push(2.0) # baseline because first
  values.push(1.0)
  variables.push(continuousVariable.discretize(values))

  # variable 8: daylighting fraction
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:Lights".to_IddObjectType))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Daylighting Fraction",
                                                                    filters,
                                                                    "fractionReplacable")
  continuousVariable.setMinimum(0.5)
  continuousVariable.setMaximum(1.0)
  continuousVariable.setIncrement(0.5)
  variables.push(continuousVariable.discretize)

  # variable 9: overhangs
  perturbations.clear
  perturbations.push(Analysis::NullPerturbation.new)
  overhangScript = Path.new($OpenStudio_LibPath + "openstudio/examples/rubyscripts/AddOverhang.rb")
  perturbation = Analysis::RubyPerturbation.new(overhangScript, 
                                                "OSM".to_FileReferenceType, 
                                                "OSM".to_FileReferenceType)
  perturbation.addArgument("projectionfactor","0.5")
  perturbation.addArgument("offsetfraction","0.1")
  perturbations.push(perturbation)
  perturbation = Analysis::RubyPerturbation.new(overhangScript, 
                                                "OSM".to_FileReferenceType, 
                                                "OSM".to_FileReferenceType)
  perturbation.addArgument("projectionfactor","1.0")
  perturbation.addArgument("offsetfraction","0.1")
  perturbations.push(perturbation)
  perturbation = Analysis::RubyPerturbation.new(overhangScript, 
                                                "OSM".to_FileReferenceType, 
                                                "OSM".to_FileReferenceType)
  perturbation.addArgument("projectionfactor","1.5")
  perturbation.addArgument("offsetfraction","0.1")
  perturbations.push(perturbation)    
  variables.push(Analysis::DiscreteVariable.new("Overhangs",perturbations))

  # variable 10: evaporative cooler effectiveness
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:EvaporativeCooler:Direct:ResearchSpecial".to_IddObjectType))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Evaporative Cooler Effectiveness",
                                                                    filters,
                                                                    "coolerEffectiveness")
  values = []
  values.push(0.6) # baseline because first
  values.push(0.8)
  values.push(0.9)
  variables.push(continuousVariable.discretize(values))

  # variable 11: fan characteristics
  perturbations.clear
  perturbations.push(Analysis::NullPerturbation.new)
  # option package 1
  rule = Ruleset::ModelRule.new("Better Fan")
  filters = Ruleset::FilterClauseVector.new
  filter = Ruleset::ModelObjectFilterType.new("OS:Fan:ConstantVolume".to_IddObjectType)
  filters.push(filter)
  rule.add(filters)
  action = Ruleset::ModelObjectActionSetAttribute.new("fanEfficiency",0.8)
  rule.add(action)
  action = Ruleset::ModelObjectActionSetAttribute.new("pressureRise",225.0)
  rule.add(action)
  action = Ruleset::ModelObjectActionSetAttribute.new("motorEfficiency",0.92)
  rule.add(action)    
  ruleset = Ruleset::ModelRuleset.new(rule.name())
  ruleset.add(rule)
  perturbations.push(Analysis::ModelRulesetPerturbation.new(ruleset))
  # option package 2
  rule = Ruleset::ModelRule.new("Best Fan")
  filters = Ruleset::FilterClauseVector.new
  filter = Ruleset::ModelObjectFilterType.new("OS:Fan:ConstantVolume".to_IddObjectType)
  filters.push(filter)
  rule.add(filters)
  action = Ruleset::ModelObjectActionSetAttribute.new("fanEfficiency",0.9)
  rule.add(action)
  action = Ruleset::ModelObjectActionSetAttribute.new("pressureRise",200.0)
  rule.add(action)
  action = Ruleset::ModelObjectActionSetAttribute.new("motorEfficiency",0.96)
  rule.add(action)    
  ruleset = Ruleset::ModelRuleset.new(rule.name())
  ruleset.add(rule)
  perturbations.push(Analysis::ModelRulesetPerturbation.new(ruleset))
  variables.push(Analysis::DiscreteVariable.new("Fan Performance",perturbations))    

  # variable 12: min supply air temperature
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:SetpointManager:SingleZone:Reheat".to_IddObjectType))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Minimum Supply Air Temperature",
                                                                    filters,
                                                                    "minimumSupplyAirTemperature")
  values = []
  values.push(15.0) # baseline because first
  values.push(10.0)
  values.push(5.0)
  variables.push(continuousVariable.discretize(values))    

  # variable 13: max supply air temperature
  filters = Ruleset::ModelObjectFilterClauseVector.new
  filters.push(Ruleset::ModelObjectFilterType.new("OS:SetpointManager:SingleZone:Reheat".to_IddObjectType))
  continuousVariable = Analysis::ModelRulesetContinuousVariable.new("Maximum Supply Air Temperature",
                                                                    filters,
                                                                    "maximumSupplyAirTemperature")
  continuousVariable.setMinimum(18.0)
  continuousVariable.setMaximum(26.0)
  continuousVariable.setIncrement(4.0)
  variables.push(continuousVariable.discretize)

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
