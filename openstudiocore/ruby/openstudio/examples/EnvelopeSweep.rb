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
#   Sweep an existing EnergyPlus simulation over a range of envelope 
#   parameters and optionally runs all simulations using the RunManager.
#
# == Usage
#
#  ruby EnvelopeSweep.rb ARGV[0] 
#
#  ARGV[0] - Path to EnergyPlus simulation directory for base file. 
#
#  ARGV[1] - Run EnergyPlus simulations or not, 'true' to run simulations, 
#            default is 'false'. 
#
#  ARGV[2] - Show RunManager GUI, 'true' to show, default is 'false'. 
#
# == Examples
#
#   ruby EnvelopeSweep.rb 'C:\path\to\energyplus\dir\' true
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'fileutils'

# provide path to EnergyPlus simulation directory including file 'in.idf'
epDir = OpenStudio::Path.new(ARGV[0]) 

# create analysis directory
analysisDir = epDir / OpenStudio::Path.new("EnvelopeSweep/")
FileUtils.rm_rf(analysisDir.to_s) if OpenStudio::exists(analysisDir)
FileUtils.mkdir_p(analysisDir.to_s)

# should we run EnergyPlus or not
run_energyplus = false
if ARGV[1] and /[tT]rue/.match(ARGV[1])
  run_energyplus = true
end

# should we show the GUI or not
show_gui = false
if ARGV[2] and /[tT]rue/.match(ARGV[2])
  show_gui = true
end

# find EnergyPlus
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
ep_parent_path = ep_path.parent_path();
idd_path = OpenStudio::Path.new(ep_hash[:energyplus_idd].to_s)
epw_path = OpenStudio::Path.new(epDir / OpenStudio::Path.new("in.epw"))

# configure logging
logFile = OpenStudio::FileLogSink.new(analysisDir / OpenStudio::Path.new("EnvelopeSweep.log"))
logFile.setLogLevel(0)
OpenStudio::Logger.instance.standardOutLogger.disable

# SEED MODEL

# load idf file
idfFilepath = epDir / OpenStudio::Path.new("in.idf")
oIdfFile = OpenStudio::IdfFile.load(idfFilepath,"EnergyPlus".to_IddFileType)
if oIdfFile.empty?
  raise "Unable to load " + idfFilepath.to_s + "."
end
base_idf = oIdfFile.get()
base_workspace = OpenStudio::Workspace.new(base_idf)

# reverse translate EnergyPlus simulation directory
reverse_translator = OpenStudio::EnergyPlus::ReverseTranslator.new()

# get model
puts "Translating " + idfFilepath.to_s + " to OSM."
seed_model = reverse_translator.translateWorkspace(base_workspace)

# set weather file
puts "Setting up the weather file."
epw_file = OpenStudio::EpwFile.new(epw_path)
OpenStudio::Model::WeatherFile::setWeatherFile(seed_model, epw_file)

# write seed model
seedPath = analysisDir / OpenStudio::Path.new("seed.osm")
File.open(seedPath.to_s, 'w') do |f|
  f.puts seed_model.to_s
end

seed = OpenStudio::FileReference.new(seedPath)

# PROBLEM
puts "Constructing the problem."

# path to scripts
perturbWallScript = OpenStudio::Path.new($OpenStudio_LibPath + "openstudio/examples/rubyscripts/PerturbWall.rb")
perturbWindowScript = OpenStudio::Path.new($OpenStudio_LibPath + "openstudio/examples/rubyscripts/PerturbWindow.rb")
postProcessScript = OpenStudio::Path.new($OpenStudio_LibPath + "openstudio/examples/rubyscripts/PostProcess.rb")

variables = OpenStudio::Analysis::VariableVector.new

# wall construction variable
measures1 = OpenStudio::Analysis::MeasureVector.new
measures1.push(OpenStudio::Analysis::NullMeasure.new)
# perturb the wall construction object
wallMeasure1 = OpenStudio::Analysis::RubyMeasure.new(perturbWallScript, 
                                                     "OSM".to_FileReferenceType, 
                                                     "OSM".to_FileReferenceType)
wallMeasure1.addArgument("insulation_thickness", "0.05")
wallMeasure1.addArgument("cost", "10.0")
measures1.push(wallMeasure1)
# perturb the wall construction object
wallMeasure2 = OpenStudio::Analysis::RubyMeasure.new(perturbWallScript, 
                                                     "OSM".to_FileReferenceType, 
                                                     "OSM".to_FileReferenceType)
wallMeasure2.addArgument("insulation_thickness", "0.10")
wallMeasure2.addArgument("cost", "13.0")
measures1.push(wallMeasure2)
variable1 = OpenStudio::Analysis::MeasureGroup.new("Wall Construction", measures1)
variables.push(variable1)

# window construction variable
measures2 = OpenStudio::Analysis::MeasureVector.new
measures2.push(OpenStudio::Analysis::NullMeasure.new)
# perturb the window construction object
windowMeasure1 = OpenStudio::Analysis::RubyMeasure.new(perturbWindowScript, 
                                                       "OSM".to_FileReferenceType, 
                                                       "OSM".to_FileReferenceType)
windowMeasure1.addArgument("uvalue", "0.77")
windowMeasure1.addArgument("cost", "30.0")
measures2.push(windowMeasure1)
# perturb the window construction object
windowMeasure2 = OpenStudio::Analysis::RubyMeasure.new(perturbWindowScript, 
                                                       "OSM".to_FileReferenceType, 
                                                       "OSM".to_FileReferenceType)
windowMeasure2.addArgument("uvalue", "0.47")
windowMeasure2.addArgument("cost", "40.0")
measures2.push(windowMeasure2)
variable2 = OpenStudio::Analysis::MeasureGroup.new("Window Construction", measures2)
variables.push(variable2)

# configure workflow
rmWorkflow = OpenStudio::Runmanager::Workflow.new
rmWorkflow.addJob(OpenStudio::Runmanager::JobType.new("modeltoidf"))
if run_energyplus
  rmWorkflow.addJob(OpenStudio::Runmanager::JobType.new("energyplus"))
end

# post process
rubyJobBuilder = OpenStudio::Runmanager::RubyJobBuilder.new
rubyJobBuilder.setScriptFile(postProcessScript)
rubyJobBuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                            OpenStudio::Runmanager::FileSource.new("All"),
                            ".*\\.osm",
                            "in.osm")
if run_energyplus
  rubyJobBuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                              OpenStudio::Runmanager::FileSource.new("All"),
                              "eplusout.sql",
                              "eplusout.sql")
end                            
rubyJobBuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir))
rmWorkflow.addJob(rubyJobBuilder.toWorkItem())

# define the problem
problem = OpenStudio::Analysis::Problem.new("Envelope Problem", variables, rmWorkflow)


# ALGORITHM
puts "Setting the algorithm."
options = OpenStudio::Analysis::DesignOfExperimentsOptions.new("FullFactorial".to_DesignOfExperimentsType)
algorithm = OpenStudio::Analysis::DesignOfExperiments.new(options)


# ANALYSIS
analysis = OpenStudio::Analysis::Analysis.new("Mesh Analysis", problem, algorithm, seed)


# RUN ANALYSIS

# create the run manager
runPath = analysisDir / OpenStudio::Path.new("run.db")
FileUtils.rm(runPath.to_s) if OpenStudio::exists(runPath)
runManager = OpenStudio::Runmanager::RunManager.new(runPath, true, false, show_gui)

# create the project database
projectPath = analysisDir / OpenStudio::Path.new("project.osp")
FileUtils.rm(projectPath.to_s) if OpenStudio::exists(projectPath)
database = OpenStudio::Project::ProjectDatabase.new(projectPath, runManager)

# run generates all the models and runs them
puts "Running the analysis."
analysisDriver = OpenStudio::AnalysisDriver::AnalysisDriver.new(database)
runOptions = OpenStudio::AnalysisDriver::AnalysisRunOptions.new(analysisDir,
                                                                OpenStudio::Path.new($OpenStudio_Dir))
# add tools 
runOptions.setRunManagerTools(OpenStudio::Runmanager::ConfigOptions::makeTools(
    ep_parent_path, 
    OpenStudio::Path.new, 
    OpenStudio::Path.new, 
    $OpenStudio_RubyExeDir,
    OpenStudio::Path.new))
currentAnalysis = analysisDriver.run(analysis,runOptions)
if show_gui
  runManager.showStatusDialog()
end
analysisDriver.waitForFinished()

# find the baseline
puts "Post-processing the results."
analysisRecord = OpenStudio::Project::AnalysisRecord::getAnalysisRecords(
    analysisDriver.database)[0]
testVariableValues = OpenStudio::QVariantVector.new
testVariableValues.push(OpenStudio::QVariant.new(0))
testVariableValues.push(OpenStudio::QVariant.new(0))
testDataPoints = analysisRecord.getDataPointRecords(testVariableValues);
raise "Unexpected number of baseline points." if not (testDataPoints.size == 1)

table = []
row = ["wallInsulationThickness"]
baselineTotalSiteEnergy = nil
baselineAnnualTotalUtilityCost = nil
baselineCostEstimate = nil
if run_energyplus
  xmlOutputDataRecord = testDataPoints[0].xmlOutputDataRecord.get
  baselineTotalSiteEnergy = xmlOutputDataRecord.getAttributeRecord("totalSiteEnergy").get.attributeValueAsDouble
  baselineTotalSourceEnergy = xmlOutputDataRecord.getAttributeRecord("totalSourceEnergy").get.attributeValueAsDouble
  
  # ETH@20110926 Utility Rates not supported in ReverseTranslator 0.5.0
  # baselineAnnualTotalUtilityCost = xmlOutputDataRecord.getAttributeRecord("annualTotalUtilityCost").get.attributeValueAsDouble
  # baselineCostEstimate = xmlOutputDataRecord.getAttributeRecord("costEstimate").get.attributeValueAsDouble
  
  row.concat(["windowUValue", "totalSiteEnergy (GJ)", "totalSourceEnergy (GJ)"])
  # row.concat(["annualTotalUtilityCost ($)", "deltaAnnualTotalUtilityCost ($)"]
  # row.concat(["costEstimate ($)", "deltaCostEstimate ($)"])
  row.concat(["percentSavings (%)"])
  # row.concat(["simplePayback (yr)", "5 Year TLCC ($)"])
   
  attribute = xmlOutputDataRecord.getAttributeRecord("EndUses").get.attribute
  endUses = OpenStudio::EndUses::fromAttribute(attribute).get
  OpenStudio::EndUses::fuelTypes.each do |fuelType|
    use = endUses.getEndUseByFuelType(fuelType)
    units = OpenStudio::EndUses::getUnitsForFuelType(fuelType)
    puts "Baseline used #{use} #{units} of #{fuelType.valueName}"
  end
end

table << row

# loop over all models
analysisRecord.dataPointRecords.each do |dataPointRecord|

  row = []
  
  xmlOutputDataRecord = dataPointRecord.xmlOutputDataRecords[0]

  wallInsulationThickness = xmlOutputDataRecord.getAttributeRecord("wallInsulationThickness").get.attributeValueAsDouble
  row << wallInsulationThickness.to_s
  
  if run_energyplus
  
    # SqlFile needed to retrieve baseline u-value
    windowUValue = xmlOutputDataRecord.getAttributeRecord("windowUValue").get.attributeValueAsDouble
    row << windowUValue.to_s
  
    totalSiteEnergy = xmlOutputDataRecord.getAttributeRecord("totalSiteEnergy").get.attributeValueAsDouble
    row << totalSiteEnergy.to_s 

    totalSourceEnergy = xmlOutputDataRecord.getAttributeRecord("totalSourceEnergy").get.attributeValueAsDouble
    row << totalSourceEnergy.to_s

    # annualTotalUtilityCost = xmlOutputDataRecord.getAttributeRecord("annualTotalUtilityCost").get.attributeValueAsDouble
    # row << annualTotalUtilityCost.to_s

    # deltaAnnualTotalUtilityCost = annualTotalUtilityCost - baselineAnnualTotalUtilityCost
    # row << deltaAnnualTotalUtilityCost.to_s

    # costEstimate = xmlOutputDataRecord.getAttributeRecord("costEstimate").get.attributeValueAsDouble
    # row << costEstimate.to_s

    # deltaCostEstimate = costEstimate - baselineCostEstimate
    # row << deltaCostEstimate.to_s

    percentSavings = 100.0 * (baselineTotalSiteEnergy - totalSiteEnergy) / baselineTotalSiteEnergy
    row << percentSavings.to_s

    # simplePayback = -deltaCostEstimate/deltaAnnualTotalUtilityCost
    # if deltaAnnualTotalUtilityCost == 0
    #   row << "NA"
    # else
    #   row << simplePayback.to_s
    # end

    economics = OpenStudio::Economics.new
    monthlyRate = 0.06/12
    numMonths = 5*12
    upvFactor = economics.getUPVFactor(monthlyRate, numMonths)

    # tlcc = costEstimate + upvFactor*annualTotalUtilityCost
    # row << tlcc.to_s
  end
    
  table << row
end

# print out the table
csvPath = analysisDir / OpenStudio::Path.new("EnvelopeSweep.csv")
File.open(csvPath.to_s, 'w') do |f|
  table.each do |row|
    f.puts row.join(',')
  end
end



















