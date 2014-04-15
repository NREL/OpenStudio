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
#   Create a local component library out of data in the EnergyPlus 
#   DataSets folder.
#
# == Usage
#
#  ruby EnergyPlusDataSetsComponentLibrary.rb ARGV[0] 
#
#  ARGV[0] - Path to ProjectDatabase osp file
#
#  ARGV[1] - true/false for whether to delete existing files (default=true).
#            If false, this script will essentially do nothing if the osp file
#            and projectDir exist.
#            
#
# == Examples
#
#   ruby EnergyPlusDataSetsComponentLibrary.rb './ComponentLibrary.osp'
#
######################################################################

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'
require 'fileutils'

include OpenStudio                      # to avoid the OpenStudio:: prefix


# extract script arguments
raise "At least one input argument is required." if (ARGV.size < 1)
ospPath = Path.new(ARGV[0])
deleteExistingFiles = true
if ARGV[1] and /[fF]alse/.match(ARGV[1])
  deleteExistingFiles = false
end
warn "Ignoring extra input arguments." if (ARGV.size > 2)


# file locations
rmdbPath = toString(ospPath.stem) + "Run.db"
rmdbPath = ospPath.parent_path / Path.new(rmdbPath)
projectDir = ospPath.parent_path / Path.new(ospPath.stem)


# create/open ProjectDatabase
if deleteExistingFiles
  FileUtils.rm(rmdbPath.to_s) if exists(rmdbPath)
  FileUtils.rm(ospPath.to_s) if exists(ospPath)
  FileUtils.rm_r(projectDir.to_s) if exists(projectDir)
end
exit if exists(ospPath) and exists(projectDir)
runManager = Runmanager::RunManager.new(rmdbPath,false,false,false)
database = Project::ProjectDatabase.new(ospPath,runManager)
FileUtils.mkdir(projectDir.to_s) if not exists(projectDir)


# find EnergyPlus DataSets
ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
datasets_path = Path.new(ep_hash[:energyplus_datasets].to_s)


# create Window construction components
windowDir = projectDir / Path.new("WindowConstructions")
FileUtils.mkdir(windowDir.to_s) if not exists(windowDir)
# load idf objects
workspace = Workspace.new("Draft".to_StrictnessLevel,"EnergyPlus".to_IddFileType)
# tempPath = datasets_path / Path.new("WindowBlindMaterials.idf")
# idfFile = IdfFile.load(tempPath).get
# workspace.addObjects(idfFile.objects)
tempPath = datasets_path / Path.new("WindowGasMaterials.idf")
idfFile = IdfFile.load(tempPath).get
workspace.addObjects(idfFile.objects)
tempPath = datasets_path / Path.new("WindowGlassMaterials.idf")
idfFile = IdfFile.load(tempPath).get
workspace.addObjects(idfFile.objects)
# tempPath = datasets_path / Path.new("WindowScreenMaterials.idf")
# idfFile = IdfFile.load(tempPath).get
# workspace.addObjects(idfFile.objects)
# tempPath = datasets_path / Path.new("WindowShadeMaterials.idf")
# idfFile = IdfFile.load(tempPath).get
# workspace.addObjects(idfFile.objects)
tempPath = datasets_path / Path.new("WindowConstructs.idf")
idfFile = IdfFile.load(tempPath).get
workspace.addObjects(idfFile.objects)
# translate to model
model = EnergyPlus::ReverseTranslator.new.translateWorkspace(workspace)
# loop over constructions and make components
didStartTransaction = database.startTransaction
Model::getConstructionBases(model).each { |construction|
  
  componentPath = windowDir / Path.new(construction.name.get.gsub(/\//,"_") + ".osc")
  if (not exists(componentPath)) or overwriteExistingFilesAndRecords
    component = construction.createComponent
    component.componentData.setName(toString(componentPath.stem))
    component.save(componentPath,true)
    fileReference = OpenStudio::FileReference.new(componentPath)
    componentRecord = Project::FileReferenceRecord.new(fileReference,database)
    componentRecord.setName(toString(componentPath.stem))
  end

}
database.save
database.commitTransaction if didStartTransaction

