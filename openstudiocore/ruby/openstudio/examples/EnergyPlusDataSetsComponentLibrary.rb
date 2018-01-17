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
co = OpenStudio::Runmanager::ConfigOptions.new
co.fastFindEnergyPlus()
weather_path = co.getDefaultEPWLocation
datasets_path = weather_path / OpenStudio::Path.new("../DataSets/")

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

