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

require 'openstudio'
require 'openstudio/analysis/find_dakota'
require 'fileutils'

require 'test/unit'

class TestWatcher < OpenStudio::AnalysisDriver::AnalysisDriverWatcher
  attr_accessor :numDataPointComplete, :numAnalysisComplete

  def initialize(analysisDriver)
    super(analysisDriver)
    @numDataPointComplete = 0
    @numAnalysisComplete = 0
  end
  
  def onDataPointComplete(dataPoint)
    super(dataPoint)
    puts "Derived class onDataPointComplete."
    dp = getDataPoint(dataPoint)
    @numDataPointComplete += 1
  end  
  
  def onAnalysisComplete(analysis)
    super(analysis)
    puts "Derived class onAnalysisComplete."    
    a = getAnalysis(analysis)
    @numAnalysisComplete += 1
  end
  
end

class AnalysisDriverWatcher_Test < Test::Unit::TestCase
  
  def test_AnalysisDriverWatcher
    OpenStudio::Logger.instance.standardOutLogger.setLogLevel(-2)
  
    projectDir = OpenStudio::Path.new("./AnalysisDriverWatcherTest")
    FileUtils.rm_r(projectDir.to_s) if OpenStudio::exists(projectDir)
    FileUtils.mkdir(projectDir.to_s)
    
    # save example.osm
    exampleModelPath = projectDir / OpenStudio::Path.new("example.osm")
    OpenStudio::Model::exampleModel.save(exampleModelPath,true)
    seed = OpenStudio::FileReference.new(exampleModelPath)
    
    # create univariate problem
    perturbObjectScript = OpenStudio::Path.new(
        $OpenStudio_LibPath + "openstudio/runmanager/rubyscripts/PerturbObject.rb")
    variables = OpenStudio::Analysis::VariableVector.new
    rubyMeasure = OpenStudio::Analysis::RubyMeasure.new(perturbObjectScript,
                                                        "OSM".to_FileReferenceType,
                                                        "OSM".to_FileReferenceType)
    rubyMeasure.addArgument("inputPath","in.osm");
    rubyMeasure.addArgument("outputPath","out.osm");
    rubyMeasure.addArgument("objectType","OS:Lights:Definition");
    rubyMeasure.addArgument("nameRegex", ".*");
    rubyMeasure.addArgument("field", "4"); # Watts per Space Floor Area
    continuousVariable = OpenStudio::Analysis::RubyContinuousVariable.new(
        "Lighting Power Density",
        OpenStudio::Ruleset::OSArgument::makeDoubleArgument("value"),
        rubyMeasure) 
    continuousVariable.setMinimum(6.0)
    continuousVariable.setMaximum(12.0)
    variables.push(continuousVariable)
    
    workflow = OpenStudio::Runmanager::Workflow.new
    workflow.addJob("ModelToIdf".to_JobType)
    workflow.add(OpenStudio::Runmanager::ConfigOptions::makeTools(
        OpenStudio::Path.new,
        OpenStudio::Path.new,
        OpenStudio::Path.new,
        $OpenStudio_RubyExeDir,
        OpenStudio::Path.new))
        
    problem = OpenStudio::Analysis::Problem.new("AnalysisDriverWatcherTest Problem",variables,workflow)
    
    # create algorithm
    algOptions = OpenStudio::Analysis::DDACEAlgorithmOptions.new("lhs".to_DDACEAlgorithmType)
    algOptions.setSamples(2)
    algorithm = OpenStudio::Analysis::DDACEAlgorithm.new(algOptions)
    
    # create analysis
    analysis = OpenStudio::Analysis::Analysis.new("AnalysisDriverWatcherTest Analysis",problem,algorithm,seed)
    
    # create driver and watcher
    rmPath = projectDir / OpenStudio::Path.new("rm.db")
    runmanager = OpenStudio::Runmanager::RunManager.new(rmPath,false,false,false)
    dbPath = projectDir / OpenStudio::Path.new("project.osp")
    database = OpenStudio::Project::ProjectDatabase.new(dbPath,runmanager)
    driver = OpenStudio::AnalysisDriver::AnalysisDriver.new(database)
    watcher = TestWatcher.new(driver)
    watcher.watch(analysis.uuid)
    
    # find dakota
    dakota_hash = OpenStudio::Analysis::find_dakota(5,3,1)
    if dakota_hash.nil?
      raise "Cannot run this script without Dakota. Please install Dakota (in a standard location) and try again."
    end
    
    # run analysis
    runOptions = OpenStudio::AnalysisDriver::AnalysisRunOptions.new(
                     OpenStudio::Path.new(projectDir),
                     OpenStudio::Path.new($OpenStudio_Dir),
                     OpenStudio::Path.new(dakota_hash[:dakota_exe].to_s))
    runOptions.setQueueSize(2)   
    driver.run(analysis,runOptions)
    driver.waitForFinished
    
    # test watcher
    assert_equal(2,watcher.numDataPointComplete)
    assert_equal(1,watcher.numAnalysisComplete)
    
  end
  
end

