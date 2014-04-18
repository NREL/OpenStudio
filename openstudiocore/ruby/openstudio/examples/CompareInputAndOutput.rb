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

######################################################################
# == Synopsis 
#
#   Demonstrates using the OpenStudio Building Model to compare input
#   data with (should be equivalent) output data.
#
# == Usage
#
#   ruby CompareInputAndOutput.rb 
#
# == Examples
#
#   ruby CompareInputAndOutput.rb
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'fileutils'

# energyplus simulation directory
runDir = OpenStudio::Path.new($OpenStudio_ResourcePath + "model/example/") 
OpenStudio::create_directory(OpenStudio::Path.new($OpenStudio_ResourcePath + "model"))
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

# make a building
building = model.getBuilding

# compare input calculated and sql file floor areas for each zone
grossArea = 0
building.thermalZones.each do |zone|

  name = zone.name.get

  # get floor area from zone calculation
  floorArea = zone.floorArea
  grossArea += floorArea

  # get floor area from sqlite database
  query = "SELECT FloorArea FROM zones WHERE ZoneName='#{name.upcase}'"
  sqlFloorArea = model.sqlFile.get.execAndReturnFirstDouble(query).get

  # calculated floor area should be same as that in sqlite database
  if (sqlFloorArea - floorArea).abs > 0.01
    puts "The input and output zone floor areas for zone '" + name + "' differ " +
         "by more than 1%. The model gives the floor area as #{floorArea}, but the " +
         "SQLite output file lists #{sqlFloorArea}."
    return false
  else
    puts "The input and output zone floor areas for zone '" + name + "' agree " +
           "to a 1% tolerance. The model gives the floor area as #{floorArea} and the " +
           "SQLite output file lists #{sqlFloorArea}."
  end
  
end

# get floor area from building calculation
buildingFloorAreaFromModel = building.floorArea

# check building area, will be less because plenum is not included
query = "SELECT Value FROM tabulardatawithstrings  WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' AND ReportForString='Entire Facility' AND TableName='Building Area' AND RowName='Total Building Area' AND ColumnName='Area' AND Units='m2'"
buildingFloorAreaFromSql = model.sqlFile.get.execAndReturnFirstDouble(query).get

# calculated floor area should be same as that in sqlite database
if (buildingFloorAreaFromSql - buildingFloorAreaFromModel).abs > 0.01
  puts "The input and output building floor areas differ by more than 1%. The model " +
       "gives the floor area as #{buildingFloorAreaFromModel}, but the SQLite " +
       "output file lists #{buildingFloorAreaFromSql}."
  return false
else
  puts "The input and output floor areas agree to a 1% tolerance. The model " +
       "gives the floor area as #{buildingFloorAreaFromModel} and the SQLite " +
       "output file lists #{buildingFloorAreaFromSql}."
end

