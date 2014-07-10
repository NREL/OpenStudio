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

require 'minitest/autorun'

class ISOModel_Test < MiniTest::Unit::TestCase

  def test_ISOModel
  
    model = OpenStudio::Model::exampleModel
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 
    
    ft = OpenStudio::ISOModel::ISOModelForwardTranslator.new
    userModel = ft.translateModel(model)
    userModel.setWeatherFilePath(epw_path)
    simModel = userModel.toSimModel()
    results = simModel.simulate()

    monthlyResults = results.monthlyResults
    assert_equal(12, monthlyResults.size)
    
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
    assert(eui > 0)
    
    floorArea = OpenStudio::convert(userModel.floorArea, 'm^2', 'ft^2').get
    puts "Floor area: #{floorArea} ft^2"
  end
  
end

