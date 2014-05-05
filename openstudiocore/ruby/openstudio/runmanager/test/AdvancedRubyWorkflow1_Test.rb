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

  def test_Workflow1

    # General setup
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 

    # Get energyplus configuration
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
    ep_parent_path = ep_path.parent_path();

    # Generate some reasonable output directory name
    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("rubyscriptexample1")
    osm_path = outdir / OpenStudio::Path.new("in.osm") 
    OpenStudio::Model::exampleModel().save(osm_path, true);

    # Create a new workflow. We are going to take advantage of the string processor
    # constructor to help us kick start the first two parts 
    workflow = OpenStudio::Runmanager::Workflow.new("modeltoidf")
    #workflow = OpenStudio::Runmanager::Workflow.new("modeltoidf->energyplus")

    # Create a new ruby job
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new()
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PostProcess.rb")
    puts "Script file set to: " + rubyscriptfile.to_s()
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    #rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
    #                            OpenStudio::Runmanager::FileSource.new("All"),
    #                            "eplusout\\.sql",
    #                            "eplusout.sql")
    rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                                OpenStudio::Runmanager::FileSource.new("All"),
                                ".*\\.osm",
                                "in.osm")
    rubyjobbuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir)) 

    # Add it to the workflow
    workflow.addJob(rubyjobbuilder.toWorkItem())

    # Set up the basic general tools needed. This takes care of EnergyPlus, XMLPreprocessor, Radiance, Ruby 
    workflow.add(
      OpenStudio::Runmanager::ConfigOptions::makeTools(ep_parent_path, 
                                                       OpenStudio::Path.new, 
                                                       OpenStudio::Path.new, 
                                                       $OpenStudio_RubyExeDir,
                                                       OpenStudio::Path.new)
    )

    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("RubyRunManagerJobTest1.db"), true)

    # create the job tree
    jobtree = workflow.create(outdir, osm_path, epw_path)

    # run the job tree
    run_manager.enqueue(jobtree, true)

    # wait until done
    run_manager.waitForFinished()

    # get path of report
    puts jobtree.treeOutputFiles().getLastByFilename("report.xml").fullPath.to_s
    
  end
end
