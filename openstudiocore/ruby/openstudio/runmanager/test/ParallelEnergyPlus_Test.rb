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

class ParallelEnergyPlus_Test < MiniTest::Unit::TestCase

  def test_ParallelEnergyPlus1
  
    # configure logging
    logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("./ParallelEnergyPlus1.log"))
    logFile.setLogLevel(OpenStudio::Debug)
    OpenStudio::Logger.instance.standardOutLogger.setLogLevel(-2)
#    OpenStudio::Logger.instance.standardOutLogger.disable

    # General setup
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 

    # Get energyplus configuration
    co = OpenStudio::Runmanager::ConfigOptions.new
    co.fastFindEnergyPlus()
    co.fastFindRuby()

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
    workflow.add(co.getTools)

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
