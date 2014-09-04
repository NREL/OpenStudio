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

class ClimateZones_Test < MiniTest::Unit::TestCase
  
  def test_basic
    
    model = OpenStudio::Model::Model.new
    
    climateZones = model.getClimateZones
    
    assert_equal(1,climateZones.numClimateZones)
    individualZones = climateZones.climateZones
    assert_equal(1,individualZones.size)
    defaultZone = individualZones[0]
    assert_equal("ASHRAE",defaultZone.institution)
    assert_equal("ANSI/ASHRAE Standard 169",defaultZone.documentName)
    assert_equal(OpenStudio::Model::ClimateZones.ashraeDefaultYear,defaultZone.year)
    assert_equal("",defaultZone.value)
    
    assert(defaultZone.setValue("3C"))
    
    newZone = climateZones.appendClimateZone(OpenStudio::Model::ClimateZones.cecInstitutionName,
                                                    "12")
    assert_equal(false,newZone.empty)
    assert_equal(2,climateZones.numClimateZones)
    
  end
  
end
