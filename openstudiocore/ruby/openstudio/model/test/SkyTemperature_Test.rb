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

class SkyTemperature_Test < MiniTest::Unit::TestCase

  def test_construction_and_parentage
  
    model = OpenStudio::Model::Model.new
    
    # no parent when no Site and parent not explicitly set
    skyTemperature = OpenStudio::Model::SkyTemperature.new(model)
    opo = skyTemperature.parent
    assert_equal(true,opo.empty?)
    
    site = model.getSite
    opo = skyTemperature.parent
    assert_equal(false,opo.empty?)
    skyTempParent = opo.get
    assert(skyTempParent == site)
    
    # make skyTemperature apply to a RunPeriod
    runPeriod = model.getRunPeriod
    assert(skyTemperature.setParent(runPeriod))
    assert_equal(0,site.children.size)
    assert_equal(1,runPeriod.children.size)
    
    # make new SkyTemperature that will be immediately parented by site
    globalSkyTemperature = OpenStudio::Model::SkyTemperature.new(model)
    opo = globalSkyTemperature.parent
    assert_equal(false,opo.empty?)
    globalSkyTempParent = opo.get
    assert(globalSkyTempParent == site)
    assert_equal(1,site.children.size)
    assert_equal(1,runPeriod.children.size)
  
  end
    
end
