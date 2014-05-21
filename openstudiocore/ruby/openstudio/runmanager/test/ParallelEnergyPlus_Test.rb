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

class ParallelEnergyPlus_Test < Test::Unit::TestCase

  def test_ParallelEnergyPlus1
  
    # configure logging
    logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("./ParallelEnergyPlus1.log"))
    logFile.setLogLevel(OpenStudio::Debug)
    OpenStudio::Logger.instance.standardOutLogger.setLogLevel(-2)
#    OpenStudio::Logger.instance.standardOutLogger.disable

    # General setup
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 

    # Get energyplus configuration
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    ep_path = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
    ep_parent_path = ep_path.parent_path();

    # Generate some reasonable output directory name
    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("parallelenergyplustest1")
    osm_path = outdir / OpenStudio::Path.new("in.osm") 
    OpenStudio::Model::exampleModel().save(osm_path, true);

    # Create a new workflow. We are going to take advantage of the string processor
    # constructor to help us kick start the first two parts 
    workflow = OpenStudio::Runmanager::Workflow.new("modeltoidf")

    # Append the parallelEnergyPlusJobTree with 2 splits
    workflow.addWorkflow(OpenStudio::Runmanager::Workflow.new(
      OpenStudio::Runmanager::JobFactory::createParallelEnergyPlusJobTree(7, 1)));

    # Set up the basic general tools needed. This takes care of EnergyPlus, XMLPreprocessor, Radiance, Ruby 
    workflow.add(
      OpenStudio::Runmanager::ConfigOptions::makeTools(ep_parent_path, 
                                                       OpenStudio::Path.new, 
                                                       OpenStudio::Path.new, 
                                                       $OpenStudio_RubyExeDir,
                                                       OpenStudio::Path.new)
    )

    # Create a runmanager
    run_manager = OpenStudio::Runmanager::RunManager.new(OpenStudio::tempDir() / OpenStudio::Path.new("ParallelEnergyPlusTest1.db"), true)

    puts outdir;

    # create the job tree
    jobtree = workflow.create(outdir, osm_path, epw_path)

    # run the job tree
    run_manager.enqueue(jobtree, true)
    
    # show status dialog
    #run_manager.showStatusDialog()

    # wait until done
    run_manager.waitForFinished()

    # get path of report
    puts jobtree.treeOutputFiles().getLastByFilename("eplusout.sql").fullPath.to_s
    
  end
end
