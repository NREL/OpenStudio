######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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
#   Converts an osm to iso model and runs simulation
#
# == Usage
#
#  ruby RunISOModel.rb ARGV[0] ARGV[1]
#
#  ARGV[0] - Path to osm
#  ARGV[1] - Path to epw
#
# == Examples
#
#   ruby RunISOModel.rb 'C:\path\to\model\in.osm' 'C:\path\to\model\in\files\in.epw' 
#
######################################################################

require 'openstudio'

osm = OpenStudio::Path.new(ARGV[0])
epw_path = OpenStudio::Path.new(ARGV[1])

model = OpenStudio::Model::Model::load(osm)
if model.empty?
  puts "Cannot load model from '#{ARGV[0]}'"
  return
end
model = model.get

epwFile = OpenStudio::EpwFile::load(epw_path)
if epwFile.empty?
  puts "Cannot load epwFile from '#{ARGV[1]}'"
  return
end
epwFile = epwFile.get

ft = OpenStudio::ISOModel::ISOModelForwardTranslator.new
userModel = ft.translateModel(model)
userModel.setWeatherFilePath(epw_path)
simModel = userModel.toSimModel()
results = simModel.simulate()

monthlyResults = results.monthlyResults

fuelTypes = OpenStudio::EndUses.fuelTypes
puts "Month, #{fuelTypes.join(',')} kBtu/ft^2"

month = 1
monthlyResults.each do |monthlyResult|
  endUses = []
  fuelTypes.each do |fuelType| 
    endUse = OpenStudio::convert(monthlyResult.getEndUseByFuelType(fuelType), 'kWh/m^2', 'kBtu/ft^2').get
    endUses << endUse
  end
  puts "#{OpenStudio::MonthOfYear.new(month).valueName}, #{endUses.join(',')}"
  month += 1
end
eui = OpenStudio::convert(results.totalEnergyUse, 'kWh/m^2', 'kBtu/ft^2').get
puts "Total energy use intensity: #{eui} kBtu/ft^2"

floorArea = OpenStudio::convert(userModel.floorArea, 'm^2', 'ft^2').get
puts "Floor area: #{floorArea} ft^2"
