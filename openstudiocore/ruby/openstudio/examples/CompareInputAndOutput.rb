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
require 'fileutils'

# energyplus simulation directory
runDir = OpenStudio::Path.new($OpenStudio_ResourcePath + "model/example/") 
OpenStudio::create_directory(OpenStudio::Path.new($OpenStudio_ResourcePath + "model"))
OpenStudio::create_directory(runDir)

# find energyplus
co = OpenStudio::Runmanager::ConfigOptions.new
co.fastFindEnergyPlus()
weatherFilePath = OpenStudio::Path.new(co.getDefaultEPWLocation.to_s) / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    
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
workflow.add(co.getTools)
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

