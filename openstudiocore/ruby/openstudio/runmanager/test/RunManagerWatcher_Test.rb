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

require 'openstudio'
require 'minitest/autorun'

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

class RunManagerWatcher_Test < MiniTest::Unit::TestCase


  def test_RunManagerWatcher

    # configure logging
    logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("./ParallelEnergyPlus1.log"))
    logFile.setLogLevel(OpenStudio::Debug)
    OpenStudio::Logger.instance.standardOutLogger.setLogLevel(-2)
    #    OpenStudio::Logger.instance.standardOutLogger.disable

    dir = OpenStudio::Path.new($OpenStudio_ResourcePath) / OpenStudio::Path.new("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade")
    osm = OpenStudio::Path.new($OpenStudio_ResourcePath) / OpenStudio::Path.new("/runmanager/SimpleModel.osm")
    epw = OpenStudio::Path.new($OpenStudio_ResourcePath) / OpenStudio::Path.new("/runmanager/USA_CO_Golden-NREL.724666_TMY3.epw")

    measure = OpenStudio::BCLMeasure::load(dir)

    wf = OpenStudio::Runmanager::Workflow.new()

    wwr = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("wwr")
    wwr.setValue(0.1)

    sillHeight = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("sillHeight")
    sillHeight.setValue(0.2)

    facade = OpenStudio::Ruleset::OSArgument::makeStringArgument("facade")
    facade.setValue("North")

    args  = OpenStudio::Ruleset::OSArgumentVector.new()
    args << wwr
    args << sillHeight
    args << facade

    # Add job one
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new(measure.get(), args)
    rubyjobbuilder.setIncludeDir(OpenStudio::getOpenStudioRubyIncludePath())
    wf.addJob(rubyjobbuilder.toWorkItem())

    # add job two
    args2  = OpenStudio::Ruleset::OSArgumentVector.new()
    args2 << wwr
    sillHeight.setValue(0.3)
    args2 << sillHeight
    facade.setValue("East")
    args2 << facade
    rubyjobbuilder2 = OpenStudio::Runmanager::RubyJobBuilder.new(measure.get(), args2)
    rubyjobbuilder2.setIncludeDir(OpenStudio::getOpenStudioRubyIncludePath())
    wf.addJob(rubyjobbuilder2.toWorkItem())


    wf.addJob(OpenStudio::Runmanager::JobType.new("ModelToIdf"))
    wf.addJob(OpenStudio::Runmanager::JobType.new("EnergyPlus"))

    co = OpenStudio::Runmanager::ConfigOptions.new
    co.fastFindEnergyPlus()
    co.fastFindRuby()

    wf.add(co.getTools)
    wf.addParam(OpenStudio::Runmanager::JobParam.new("flatoutdir"))

    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("runmanagerwatchertest")
    jobtree = wf.create(outdir, osm, epw)
    OpenStudio::Runmanager::JobFactory::optimizeJobTree(jobtree)

    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("runmanagerwatchertest.db"), true)
    watcher = RunManagerWatcherImpl.new(run_manager)

    puts outdir

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
    assert(counts[999] == 1)

  end
end
