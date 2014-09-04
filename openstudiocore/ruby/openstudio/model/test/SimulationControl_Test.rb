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

class SimulationControl_Test < MiniTest::Unit::TestCase

  def test_construct_through_child
  
    model = OpenStudio::Model::Model.new
    
    timestep = model.getTimestep
    oParent = timestep.parent
    assert_equal(false,oParent.empty?)
    parent = oParent.get
    oSimulation = parent.to_SimulationControl
    assert_equal(false,oSimulation.empty?)
    assert_equal(1,oSimulation.get.children.size)
  
  end
  
  def test_construct_all_children
    
    model = OpenStudio::Model::Model.new
    
    simulation = model.getSimulationControl
    numSimulationChildren = 0
    assert_equal(numSimulationChildren,simulation.children.size)
    
    convergenceLimits = model.getConvergenceLimits
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    heatBalanceAlgorithm = model.getHeatBalanceAlgorithm
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    insideSurfaceConvectionAlgorithm = model.getInsideSurfaceConvectionAlgorithm
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    outsideSurfaceConvectionAlgorithm = model.getOutsideSurfaceConvectionAlgorithm
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    runPeriod = model.getRunPeriod
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    shadowCalculation = model.getShadowCalculation
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    timestep = model.getTimestep
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    zoneAirContaminantBalance = model.getZoneAirContaminantBalance
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    zoneAirHeatBalanceAlgorithm = model.getZoneAirHeatBalanceAlgorithm
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
    zoneCapacitanceMultiplierResearchSpecial = model.getZoneCapacitanceMultiplierResearchSpecial
    numSimulationChildren = numSimulationChildren + 1
    assert_equal(numSimulationChildren,simulation.children.size)
    
  end
  
end
