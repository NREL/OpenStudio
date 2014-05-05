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

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'test/unit'

class RunManagerWatcherImpl < OpenStudio::Runmanager::RunManagerWatcher
  def initialize(runmanager)
    super
    @m_finishedCounts=[]
  end

  def finishedCounts
    return @m_finishedCounts
  end

  def jobFinishedDetails(t_jobId, t_jobType, t_lastRun, t_errors, t_outputFiles, t_inputParams, t_isMergedJob, t_mergedIntoJobId)
    puts "JobFinished: #{t_jobId} #{t_jobType.valueName} #{t_lastRun} #{t_errors.succeeded} #{t_outputFiles.files.size} #{t_outputFiles.files.at(0).fullPath} #{t_inputParams.params.size} #{t_isMergedJob} #{t_mergedIntoJobId}"

    if not @m_finishedCounts[t_jobType.value]
      @m_finishedCounts[t_jobType.value] = 1
    else
      @m_finishedCounts[t_jobType.value] += 1
    end
  end

  def treeFinished(t_job)
    puts "Tree Finished"
    if not @m_finishedCounts[999]
      @m_finishedCounts[999] = 1
    else
      @m_finishedCounts[999] += 1
    end
  end
end

class RunManagerWatcher_Test < Test::Unit::TestCase


  def test_RunManagerWatcher

    # configure logging
    logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("./ParallelEnergyPlus1.log"))
    logFile.setLogLevel(OpenStudio::Debug)
    OpenStudio::Logger.instance.standardOutLogger.setLogLevel(-2)
    #    OpenStudio::Logger.instance.standardOutLogger.disable

    dir = OpenStudio::Path.new($OpenStudio_ResourcePath) / OpenStudio::Path.new("/utilities/BCL/Measures/SetWindowToWallRatioByFacade");
    osm = OpenStudio::Path.new($OpenStudio_ResourcePath) / OpenStudio::Path.new("/runmanager/SimpleModel.osm");
    epw = OpenStudio::Path.new($OpenStudio_ResourcePath) / OpenStudio::Path.new("/runmanager/USA_CO_Golden-NREL.724666_TMY3.epw");

    measure = OpenStudio::BCLMeasure::load(dir);

    wf = OpenStudio::Runmanager::Workflow.new();

    wwr = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("wwr");
    wwr.setValue(0.1)

    sillHeight = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("sillHeight");
    sillHeight.setValue(0.2)

    facade = OpenStudio::Ruleset::OSArgument::makeStringArgument("facade");
    facade.setValue("North");

    args  = OpenStudio::Ruleset::OSArgumentVector.new();
    args << wwr
    args << sillHeight
    args << facade

    # Add job one
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new(measure.get(), args);
    rubyjobbuilder.setIncludeDir(OpenStudio::getOpenStudioRubyIncludePath());
    wf.addJob(rubyjobbuilder.toWorkItem());

    # add job two
    args2  = OpenStudio::Ruleset::OSArgumentVector.new();
    args2 << wwr
    sillHeight.setValue(0.3);
    args2 << sillHeight
    facade.setValue("East");
    args2 << facade
    rubyjobbuilder2 = OpenStudio::Runmanager::RubyJobBuilder.new(measure.get(), args2);
    rubyjobbuilder2.setIncludeDir(OpenStudio::getOpenStudioRubyIncludePath());
    wf.addJob(rubyjobbuilder2.toWorkItem());


    wf.addJob(OpenStudio::Runmanager::JobType.new("ModelToIdf"));
    wf.addJob(OpenStudio::Runmanager::JobType.new("EnergyPlus"));

    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
    ep_parent_path = ep_path.parent_path();

    tools = OpenStudio::Runmanager::ConfigOptions::makeTools(ep_parent_path, OpenStudio::Path.new(), OpenStudio::Path.new(), 
                                                 $OpenStudio_RubyExeDir, OpenStudio::Path.new(),
                                                 OpenStudio::Path.new(),OpenStudio::Path.new(),OpenStudio::Path.new(),OpenStudio::Path.new(),OpenStudio::Path.new())

    wf.add(tools);
    wf.addParam(OpenStudio::Runmanager::JobParam.new("flatoutdir"));

    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("runmanagerwatchertest")
    jobtree = wf.create(outdir, osm, epw);
    OpenStudio::Runmanager::JobFactory::optimizeJobTree(jobtree)

    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("runmanagerwatchertest.db"), true)
    watcher = RunManagerWatcherImpl.new(run_manager)

    puts outdir;

    # run the job tree
    run_manager.enqueue(jobtree, true)

    # show status dialog
    #run_manager.showStatusDialog()

    # wait until done
    run_manager.waitForFinished()

    # get path of report
    puts jobtree.treeOutputFiles().getLastByFilename("eplusout.sql").fullPath.to_s

    counts = watcher.finishedCounts

    assert(counts[OpenStudio::Runmanager::JobType.new("UserScript").value] == 2)
    assert(counts[OpenStudio::Runmanager::JobType.new("ModelToIdf").value] == 1)
    assert(counts[OpenStudio::Runmanager::JobType.new("EnergyPlus").value] == 1)

    # check to make sure exactly ONE *tree* finished
    assert(counts[999] == 1);

  end
end
