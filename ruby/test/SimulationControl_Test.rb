########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class SimulationControl_Test < Minitest::Test

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
