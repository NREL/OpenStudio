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
require 'fileutils'

require 'test/unit'

class TimeDependentValuation_Test < Test::Unit::TestCase
  
  def test_TimeDependentValuation_Construction
  
    tdvPath = OpenStudio::Path.new($OpenStudio_ResourcePath + "utilities/Filetypes/TDV_2008_kBtu_CZ13.csv")
    oTdvFile = OpenStudio::TimeDependentValuationFile::load(tdvPath)
    assert((not oTdvFile.empty?))
    tdvFile = oTdvFile.get
    
    model = OpenStudio::Model::Model.new
    
    tdv = OpenStudio::Model::TimeDependentValuation::setTimeDependentValuation(model,tdvFile)
    
    assert((not tdv.file.empty?))
    assert(tdv.site.empty?)
    assert((not tdv.path.empty?))
    assert_equal(tdvPath.to_s, tdv.path.get.to_s)
    assert((tdv.activeBuildingSector == "Commercial".to_BuildingSector))
    assert_equal(3,tdv.availableFuelTypes.size)
    tdv.makeUrlRelative
    assert_equal("TDV_2008_kBtu_CZ13.csv",tdv.path.get.to_s)
        
  end
  
  def test_TimeDependentValuation_Usage
  
    OpenStudio::Logger::instance.standardOutLogger.enable
    OpenStudio::Logger::instance.standardOutLogger.setLogLevel(-2)
  
    # energyplus simulation directory
    runDir = OpenStudio::Path.new($OpenStudio_ResourcePath + "model/example/") 
    OpenStudio::create_directory(runDir)

    # find energyplus
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    ep_exe = OpenStudio::Path.new(ep_hash[:energyplus_exe].to_s)
    weatherFilePath = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s) / 
        OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")

    # set up RunManager
    runDbPath = runDir / OpenStudio::Path.new("run.db")
    if (OpenStudio::exists(runDbPath))
      FileUtils.remove(runDbPath.to_s)
    end
    runManager = OpenStudio::Runmanager::RunManager.new(runDbPath,false,false,false)

    # get building model and set up simulation job
    model = OpenStudio::Model::exampleModel
    modelPath = runDir / OpenStudio::Path.new("in.osm")
    model.save(modelPath,true)
    workflow = OpenStudio::Runmanager::Workflow.new
    workflow.addJob("ModelToIdf".to_JobType)
    workflow.addJob("EnergyPlus".to_JobType)
    nullPath = OpenStudio::Path.new
    workflow.add(OpenStudio::Runmanager::ConfigOptions::makeTools(
        ep_exe.parent_path,nullPath,nullPath,nullPath,nullPath))
    job = workflow.create(runDir,modelPath,weatherFilePath)

    # run simulation and get sql file
    runManager.enqueue(job,false)
    runManager.waitForFinished
    sqlFilePath = job.treeAllFiles.getLastByExtension("sql").fullPath
    sqlFile = OpenStudio::SqlFile.new(sqlFilePath)
    model.setSqlFile(sqlFile)
    if not model.setSqlFile(sqlFile)
      puts "Translation from EnergyPlus to Model failed"
      return false
    end
    
    # load TDV file
    tdvPath = OpenStudio::Path.new($OpenStudio_ResourcePath + "utilities/Filetypes/TDV_2008_kBtu_CZ13.csv")
    oTdvFile = OpenStudio::TimeDependentValuationFile::load(tdvPath)
    assert((not oTdvFile.empty?))
    tdvFile = oTdvFile.get
    tdv = OpenStudio::Model::TimeDependentValuation::setTimeDependentValuation(model,tdvFile)
    
    # TDV getters
    od = tdv.energyTimeDependentValuation
    assert((not od.empty?))
    energyTDV = od.get
    assert((energyTDV > 0.0))
    
    od = tdv.getCostTimeDependentValuation("Electricity".to_FuelType)
    assert((not od.empty?))
    elecCostTDV = od.get
    assert((elecCostTDV > 0.0))
    
    # Facility attributes
    facility = model.getFacility
    
    assert((not facility.getAttribute("totalEnergyTimeDependentValuation").empty?))
    attribute = facility.getAttribute("totalEnergyTimeDependentValuation").get
    assert((attribute.valueType == "Double".to_AttributeValueType))
    assert((attribute.valueAsDouble > 0.0))
    assert_equal(energyTDV,attribute.valueAsDouble)
    
    assert((not facility.getAttribute("fossilFuelCostTimeDependentValuation").empty?))
    attribute = facility.getAttribute("fossilFuelCostTimeDependentValuation").get
    assert((attribute.valueType == "Double".to_AttributeValueType))
    assert((attribute.valueAsDouble > 0.0))
  
  end
  
end
