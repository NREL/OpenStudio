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

class AdvancedRubyWorkflow_Test < MiniTest::Unit::TestCase

  def test_Workflow3

    # General setup
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 

    # Get energyplus configuration
    co = OpenStudio::Runmanager::ConfigOptions.new(true)
    co.fastFindEnergyPlus()
    co.fastFindRuby()

    # Generate some reasonable output directory name
    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("rubyscriptexample3"); 
    osm_path = outdir / OpenStudio::Path.new("in.osm") 
    OpenStudio::Model::exampleModel().save(osm_path, true)


    ## MAIN JOB

    # Start with a dummy script job.
    # The goal of this job is simply to be a holder for our additional jobs.
    # We are using the Workflow object as a convenience to help us put together
    # the tools and such
    mainworkflow = OpenStudio::Runmanager::Workflow.new()
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new()
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/Dummy.rb")
    puts "Script file set to: " + rubyscriptfile.to_s()
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    mainworkflow.addJob(rubyjobbuilder.toWorkItem())


    ## MAIN CHILD 1

    # Now we are going to create our two child jobs.
    # These jobs each modify an OSM and run a simulation
    # We'll currently have to create them one at a time, we don't have a good way of making one and reusing it yet
    child1 = OpenStudio::Runmanager::Workflow.new()
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new()
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PerturbObject.rb")
    puts "Script file set to: " + rubyscriptfile.to_s()
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                                OpenStudio::Runmanager::FileSource.new("All"),
                                ".*\\.osm",
                                "in.osm")
    rubyjobbuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir))
    rubyjobbuilder.addScriptParameter("objectType", "RunPeriod")
    rubyjobbuilder.addScriptParameter("nameRegex", ".*")
    rubyjobbuilder.addScriptParameter("field", "1")
    rubyjobbuilder.addScriptParameter("value", "1")
    rubyjobbuilder.addScriptParameter("field", "2")
    rubyjobbuilder.addScriptParameter("value", "1")
    rubyjobbuilder.addScriptParameter("field", "3")
    rubyjobbuilder.addScriptParameter("value", "1")
    rubyjobbuilder.addScriptParameter("field", "4")
    rubyjobbuilder.addScriptParameter("value", "31")

    # Add it to the workflow
    child1.addJob(rubyjobbuilder.toWorkItem())
    child1.addJob(OpenStudio::Runmanager::JobType.new("modeltoidf"))
    #child1.addJob(OpenStudio::Runmanager::JobType.new("energyplus"))


    ## MAIN CHILD 2

    # Second child
    #
    child2 = OpenStudio::Runmanager::Workflow.new()
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new()
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PerturbObject.rb")
    puts "Script file set to: " + rubyscriptfile.to_s()
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                                OpenStudio::Runmanager::FileSource.new("All"),
                                ".*\\.osm",
                                "in.osm")
    rubyjobbuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir))
    rubyjobbuilder.addScriptParameter("objectType", "RunPeriod")
    rubyjobbuilder.addScriptParameter("nameRegex", ".*")
    rubyjobbuilder.addScriptParameter("field", "2")
    rubyjobbuilder.addScriptParameter("value", "1")
    rubyjobbuilder.addScriptParameter("field", "2")
    rubyjobbuilder.addScriptParameter("value", "1")
    rubyjobbuilder.addScriptParameter("field", "3")
    rubyjobbuilder.addScriptParameter("value", "2")
    rubyjobbuilder.addScriptParameter("field", "4")
    rubyjobbuilder.addScriptParameter("value", "28")

    # Add it to the workflow
    child2.addJob(rubyjobbuilder.toWorkItem())
    child2.addJob(OpenStudio::Runmanager::JobType.new("modeltoidf"))
    #child2.addJob(OpenStudio::Runmanager::JobType.new("energyplus"))

    # ROLL UP JOB
    #

    rollup = OpenStudio::Runmanager::Workflow.new()
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new()
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PostProcessMultiple.rb")
    puts "Script file set to: " + rubyscriptfile.to_s()
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    rubyjobbuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir))
    rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("All"),
                                OpenStudio::Runmanager::FileSource.new("All"),
                                ".*\\.osm",
                                "in.osm")
    #rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("All"),
    #                            OpenStudio::Runmanager::FileSource.new("All"),
    #                            ".*\\.sql",
    #                            "")
    rollup.addJob(rubyjobbuilder.toWorkItem())

    # Add the tools to the main workflow
    mainworkflow.add(co.getTools)
    
    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("RubyRunManagerJobTest3.db"), true)

    # Create the main job 
    jobtree = mainworkflow.create(outdir, osm_path, epw_path)

    # Add the first child
    jobtree.addChild(child1.create())

    # Add the second child
    jobtree.addChild(child2.create())

    # Add the roll up job
    jobtree.setFinishedJob(rollup.create())

    # Run the whole thing
    run_manager.enqueue(jobtree, true)

    run_manager.waitForFinished()

    puts jobtree.treeOutputFiles().getLastByFilename("report.xml").fullPath.to_s

  end
end
