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
#   Load an idf, write an idf, convert to OpenStudio model, save to osm and read from osm.
#
# == Usage
#
#   ruby Serialization.rb ARGV[0] 
# 
#   ARGV[0] - Path to EnergyPlus Idf file
#
# == Examples
#
#   ruby Serialization.rb 'C:\path\to\energyplus\dir\in.idf'
#
######################################################################

require 'openstudio'

# read in idf
idfPath = OpenStudio::Path.new(ARGV[0])
oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType)
if oIdfFile.empty?
  raise "Unable to load " + idfPath.to_s + "."
end 
idfFile = oIdfFile.get()

# save idf
File.open(idfPath.replace_extension('newidf').to_s, 'w') do |file|
  file << idfFile.to_s
end

# convert to workspace
workspace = OpenStudio::Workspace.new(idfFile)

# reverse translate EnergyPlus file
translator = OpenStudio::EnergyPlus::ReverseTranslator.new()

# convert to model
model = translator.translateWorkspace(workspace)

# save as osm
osmPath = idfPath.replace_extension('osm')
model.toIdfFile().save(osmPath,true)

# load from osm
oIdfFile = OpenStudio::IdfFile.load(osmPath)
if not oIdfFile
  raise "Loading Model from osm failed."
end
idfFile = oIdfFile.get()
model = OpenStudio::Model::Model.new(idfFile)
