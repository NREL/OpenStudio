######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

class ElectricLoadCenterDistribution_Test < MiniTest::Unit::TestCase

  def test_ElectricLoadCenterDistribution_Photovoltaics
  
    model = OpenStudio::Model::Model.new
    
    elcd = OpenStudio::Model::ElectricLoadCenterDistribution.new(model)
    assert_equal(elcd.generators.size, 0)
    assert(elcd.inverter.empty?)

    simple_panel = OpenStudio::Model::GeneratorPhotovoltaic::simple(model)
    assert(simple_panel.electricLoadCenterDistribution.empty?)
    
    elcd.addGenerator(simple_panel)
    assert(!simple_panel.electricLoadCenterDistribution.empty?)
    assert_equal(elcd.generators.size, 1)
    assert(elcd.inverter.empty?)
    
    eod_panel = OpenStudio::Model::GeneratorPhotovoltaic::equivalentOneDiode(model)
    assert(eod_panel.electricLoadCenterDistribution.empty?)
    
    elcd.addGenerator(eod_panel)
    assert(!eod_panel.electricLoadCenterDistribution.empty?)
    assert_equal(elcd.generators.size, 2)
    assert(elcd.inverter.empty?)
    
    inverter = OpenStudio::Model::ElectricLoadCenterInverterSimple.new(model)
    assert(inverter.electricLoadCenterDistribution.empty?)
    
    elcd.setInverter(inverter)
    assert(!inverter.electricLoadCenterDistribution.empty?)
    assert(!elcd.inverter.empty?)

  end
    
end
