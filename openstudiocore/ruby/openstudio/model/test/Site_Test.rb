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

class Site_Test < MiniTest::Unit::TestCase
  
  def test_construct_all_children
    
    model = OpenStudio::Model::Model.new
    
    site = model.getSite
    numSiteChildren = 0
    assert_equal(numSiteChildren,site.children.size)
    
    climateZones = model.getClimateZones
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)

    designDay = OpenStudio::Model::DesignDay.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)
    
    skyTemperature = OpenStudio::Model::SkyTemperature.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)
    
    timeDependentValuation = model.getTimeDependentValuation
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)
    
    weatherFile = model.getWeatherFile
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)
    
    weatherFileConditionType = OpenStudio::Model::WeatherFileConditionType.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)
    
    weatherFileDays = OpenStudio::Model::WeatherFileDays.new(model)
    numSiteChildren = numSiteChildren + 1
    assert_equal(numSiteChildren,site.children.size)
    
  end
  
  def test_get_optional_site
    
    model = OpenStudio::Model::Model.new  
    site = model.getOptionalSite
    assert(site.empty?)

  end
 
end
