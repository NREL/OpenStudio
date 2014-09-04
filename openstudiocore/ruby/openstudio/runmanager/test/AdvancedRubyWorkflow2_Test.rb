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
require 'minitest/autorun'

class AdvancedRubyWorkflow_Test < MiniTest::Unit::TestCase

  def test_Workflow2
  
    # General setup
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 

    # Generate some reasonable output directory name
    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("rubyscriptexample2"); 
    osm_path = outdir / OpenStudio::Path.new("in.osm") 
    OpenStudio::Model::exampleModel().save(osm_path, true);


    # Create a new workflow. 
    workflow = OpenStudio::Runmanager::Workflow.new();

    # Create a new ruby job
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new();
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PerturbObject.rb")
    puts "Script file set to: " + rubyscriptfile.to_s();
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                                OpenStudio::Runmanager::FileSource.new("All"),
                                ".*\\.osm",
                                "in.osm")
    rubyjobbuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir))
    rubyjobbuilder.addScriptParameter("objectType", "Material")
    rubyjobbuilder.addScriptParameter("nameRegex", "Roof Membrane")
    rubyjobbuilder.addScriptParameter("field", "2")
    rubyjobbuilder.addScriptParameter("value", "0.02")

    # Add it to the workflow
    workflow.addJob(rubyjobbuilder.toWorkItem())

    # Set up the basic general tools needed. This takes care of EnergyPlus, XMLPreprocessor, Radiance, Ruby 
    workflow.add(
      OpenStudio::Runmanager::ConfigOptions::makeTools(OpenStudio::Path.new, 
                                                       OpenStudio::Path.new, 
                                                       OpenStudio::Path.new, 
                                                       $OpenStudio_RubyExeDir,
                                                       OpenStudio::Path.new)
    )

    # Create a second ruby job
    rubyjobbuilder = OpenStudio::Runmanager::RubyJobBuilder.new();
    rubyscriptfile = OpenStudio::Path.new(File.expand_path(File.dirname(__FILE__))) / OpenStudio::Path.new("../rubyscripts/PerturbObject.rb")
    puts "Script file set to: " + rubyscriptfile.to_s();
    rubyjobbuilder.setScriptFile(rubyscriptfile)
    rubyjobbuilder.addInputFile(OpenStudio::Runmanager::FileSelection.new("last"),
                                OpenStudio::Runmanager::FileSource.new("All"),
                                ".*\\.osm",
                                "in.osm")
    rubyjobbuilder.setIncludeDir(OpenStudio::Path.new($OpenStudio_Dir))
    rubyjobbuilder.addScriptParameter("objectType", "Material")
    rubyjobbuilder.addScriptParameter("nameRegex", "Mass NonRes Wall Insulation")
    rubyjobbuilder.addScriptParameter("field", "2")
    rubyjobbuilder.addScriptParameter("value", "0.10")

    # Add it to the workflow
    workflow.addJob(rubyjobbuilder.toWorkItem())

    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("RubyRunManagerJobTest2.db"), true)

    # Create the workflow
    jobtree = workflow.create(outdir, osm_path, epw_path)

    # Queue it up
    run_manager.enqueue(jobtree, true);

    # Run it out
    run_manager.waitForFinished()

    # get path of report
    puts jobtree.treeOutputFiles().getLastByFilename("out.osm").fullPath.to_s
  
  end
end




