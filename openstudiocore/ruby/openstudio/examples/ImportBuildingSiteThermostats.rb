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
#   Load an idf, saves it as an osm, clones building, site, and thermostats and saves as new osm.
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
  raise "Unable to load " + idfFilepath.to_s + "."
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
osmPath = OpenStudio::Path.new(ARGV[0].gsub(".idf", "_RawImport.osm"))
model.toIdfFile().save(osmPath,true)

# create a new empty model
model_2 = OpenStudio::Model::Model.new

# get and clone building
building = model.getBuilding
building.clone(model_2)

# get and clone site shading
site = model.getSite
site.clone(model_2)

# get and clone thermostats
thermostats = model.getThermostatSetpointDualSetpoints
thermostats.each do |thermostat|
  thermostat.clone(model_2)
end

# save as osm
osmPath_2 = OpenStudio::Path.new(ARGV[0].gsub(".idf", "_NoHVAC.osm"))
model_2.toIdfFile().save(osmPath_2,true)

puts "Files have been generated"


