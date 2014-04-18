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

class AdvancedRubyWorkflow_Test < Test::Unit::TestCase

  def test_Workflow3

    # General setup
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 

    # Get energyplus configuration
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
    ep_parent_path = ep_path.parent_path();

    # Generate some reasonable output directory name
    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("rubyscriptexample3"); 
    osm_path = outdir / OpenStudio::Path.new("in.osm") 
    OpenStudio::Model::exampleModel().save(osm_path, true);


    ## MAIN JOB


    # Start with a dummy script job.
    # The goal of this job is simply to be a holder for our additional jobs.
    # We are using the Workflow object as a convenience to help us put together
    # the tools and such
    mainworkflow = OpenStudio::Runmanager::Workflow.new()
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new();
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/Dummy.rb")
    puts "Script file set to: " + rubyscriptfile.to_s();
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    mainworkflow.addJob(rubyjobbuilder.toWorkItem())


    ## MAIN CHILD 1

    # Now we are going to create our two child jobs.
    # These jobs each modify an OSM and run a simulation
    # We'll currently have to create them one at a time, we don't have a good way of making one and reusing it yet
    child1 = OpenStudio::Runmanager::Workflow.new()
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new();
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PerturbObject.rb")
    puts "Script file set to: " + rubyscriptfile.to_s();
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
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new();
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PerturbObject.rb")
    puts "Script file set to: " + rubyscriptfile.to_s();
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
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new();
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PostProcessMultiple.rb")
    puts "Script file set to: " + rubyscriptfile.to_s();
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
    rollup.addJob(rubyjobbuilder.toWorkItem());

    # Add the tools to the main workflow
    mainworkflow.add(
      OpenStudio::Runmanager::ConfigOptions::makeTools(ep_parent_path, 
                                                       OpenStudio::Path.new, 
                                                       OpenStudio::Path.new, 
                                                       $OpenStudio_RubyExeDir,
                                                       OpenStudio::Path.new)
    )


    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("RubyRunManagerJobTest3.db"), true)


    # Create the main job 
    jobtree = mainworkflow.create(outdir, osm_path, epw_path);

    # Add the first child
    jobtree.addChild(child1.create());

    # Add the second child
    jobtree.addChild(child2.create());

    # Add the roll up job
    jobtree.setFinishedJob(rollup.create());

    # Run the whole thing
    run_manager.enqueue(jobtree, true);

    run_manager.waitForFinished()

    puts jobtree.treeOutputFiles().getLastByFilename("report.xml").fullPath.to_s

  end
end
