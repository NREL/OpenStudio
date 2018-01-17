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
#   Retrieve the example optimization problem created by 
#   CreateExampleOptimizationProblem.rb, then solve it with sequential 
#   search. Demonstrates the addition of a user custom point, and is 
#   set up to initially run only the baseline, a custom point, and the
#   first real search iteration.
#
# == Usage
#
#  ruby RunExampleOptimizationProblem.rb ARGV[0]
#
#  ARGV[0] - Path to ProjectDatabase osp file for 
#            CreateExampleOptimizationProblem.rb (Will be created if 
#            does not exist.) 
#
#  ARGV[1] - Path to ProjectDatabase osp file for this analysis. (Will 
#            be created if it does not exist.)
#
#  ARGV[2] - true/false for whether to reconstruct example problem 
#            (default=true)
#
#  ARGV[3] - true/false for whether to run analysis from scratch 
#            (default=true)
#
# == Examples
#
#   ruby RunExampleOptimizationProblem.rb './ExampleOptimizationProblem.osp' './ExampleSequentialSearch_Curve1.osp' false false
#
######################################################################

require 'openstudio'
require 'fileutils'

include OpenStudio                      # to avoid the OpenStudio:: prefix


# options not exposed to command line
show_gui = false
weatherFileName = "USA_CO_Golden-NREL.724666_TMY3.epw"
indexOfObjectiveFunctionToMinimizeFirst = 1            # 0 (cooling energy) or 1 (heating energy)
maxIter = OptionalDouble.new(2)


# extract script arguments
raise "At least one input argument is required." if (ARGV.size < 1)
exampleProblemOspPath = Path.new(ARGV[0])
ospPath = Path.new(ARGV[1])
reconstructProblem = true
if ARGV[2] and /[fF]alse/.match(ARGV[2])
  reconstructProblem = false
end
deleteAnalysis = true
if ARGV[3] and /[fF]alse/.match(ARGV[3])
  deleteAnalysis = false
end
warn "Ignoring extra input arguments." if (ARGV.size > 4)


# file locations
exampleProblemRmdbPath = toString(exampleProblemOspPath.stem) + "Run.db"
exampleProblemRmdbPath = exampleProblemOspPath.parent_path / Path.new(exampleProblemRmdbPath)
problemDir = ospPath.parent_path / Path.new(exampleProblemOspPath.stem)
projectDir = ospPath.parent_path / Path.new(ospPath.stem)
rmdbPath = toString(ospPath.stem) + "Run.db"
rmdbPath = ospPath.parent_path / Path.new(rmdbPath)

# EnergyPlus WeatherData
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,5)
weatherData_path = Path.new(ep_hash[:energyplus_weatherdata].to_s)


# create/open ProjectDatabase
if deleteAnalysis
  FileUtils.rm(rmdbPath.to_s) if exists(rmdbPath)
  FileUtils.rm(ospPath.to_s) if exists(ospPath)
  FileUtils.rm_r(projectDir.to_s) if exists(projectDir)
end
runManager = Runmanager::RunManager.new(rmdbPath,false,false,show_gui)
database = Project::ProjectDatabase.new(ospPath,runManager)
FileUtils.mkdir(projectDir.to_s) if not exists(projectDir)


# get problem
if reconstructProblem or not exists(exampleProblemOspPath)
  systemCallString = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/CreateExampleOptimizationProblem.rb' "
  systemCallString << "'" << exampleProblemOspPath.to_s << "'" 
  puts "Making system call: " + systemCallString
  system(systemCallString)
end
exampleProblemRunManager = Runmanager::RunManager.new(exampleProblemRmdbPath,false,false,false)
exampleProblemDatabase = Project::ProjectDatabase.new(exampleProblemOspPath,exampleProblemRunManager)
problemRecords = Project::ProblemRecord::getProblemRecords(exampleProblemDatabase)
raise "Unexpected data in example opimization problem database." if problemRecords.empty? or problemRecords.size > 1
problem = problemRecords[0].problem.to_OptimizationProblem.get


# set algorithm parameters
options = Analysis::SequentialSearchOptions.new(indexOfObjectiveFunctionToMinimizeFirst)
if not maxIter.empty?
  options.setMaxIter(maxIter.get.to_i)
end
algorithm = Analysis::SequentialSearch.new(options)


# get seed file and set weather file
exampleModelPath = problemDir / Path.new("example.osm")
seed = Project::getFileReferenceRecordByName(exampleProblemDatabase, exampleModelPath.to_s).get.fileReference
model = Model::Model::load(seed.path).get
weatherFile = EpwFile.new(weatherData_path / Path.new(weatherFileName))
Model::WeatherFile::setWeatherFile(model,weatherFile)
model.save(seed.path,true)                                  # overwrite with weather data included


analysis = Analysis::OptionalAnalysis.new
if not deleteAnalysis
  # first try to retrieve from database
  analysisRecords = Project::AnalysisRecord::getAnalysisRecords(database)
  if not analysisRecords.empty?
    analysis = analysisRecords[0].analysis
    if maxIter.empty?
      analysis.algorithm.get.options.clearMaxIter
    else 
      analysis.algorithm.get.options.setMaxIter(maxIter.get.to_i)
    end
    analysis = Analysis::OptionalAnalysis.new(analysis)
  end  
end

if analysis.empty?

  # create analysis
  analysis = Analysis::Analysis.new(toString(ospPath.stem),problem,algorithm,seed)
  
  # add custom point
  variableValues = QVariantVector.new
  problem.variables.each { |var|
    variableValues.push(QVariant.new(var.to_DiscreteVariable.get.numPerturbations(false) - 1))
  }
  dataPoint = problem.createDataPoint(variableValues).get
  analysis.addDataPoint(dataPoint)
  
else
  
  analysis = analysis.get
  
end


# run analysis
analysisDriver = AnalysisDriver::AnalysisDriver.new(database)
runOptions = AnalysisDriver::AnalysisRunOptions.new(projectDir,
                                                    OpenStudio::Path.new($OpenStudio_Dir))
currentAnalysis = analysisDriver.run(analysis,runOptions)
if show_gui
  runManager.showStatusDialog
end
analysisDriver.waitForFinished

tablePath = projectDir / Path.new("SequentialSearchSummary.csv")
analysis = currentAnalysis.analysis
table = analysis.algorithm.get.to_SequentialSearch.get.getSummaryTable(analysis)
table.save(tablePath,true)
tableRecord = Project::FileReferenceRecord.new(FileReference.new(tablePath),database)
analysisRecord = Project::AnalysisRecord.new(analysis,database) # save curve and pareto tags
database.save

