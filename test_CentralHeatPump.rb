# Requiring my custom build of OS
require '/Users/julien/Software/Others/OS-build/Products/ruby/openstudio'
require 'minitest/autorun'

class CentralHeatPump_Test < MiniTest::Unit::TestCase
  # def setup
  # end

  # def teardown

  def test_CentralHeatPumpBasic
    model = OpenStudio::Model::Model.new

    central_hp = OpenStudio::Model::CentralHeatPumpSystem.new(model)

    assert_equal("SmartMixing", central_hp.controlMethod)
    assert(!central_hp.setControlMethod("Prout"))
    assert(central_hp.setControlMethod("SmartMixing"))

    assert_equal(0, central_hp.ancillaryPower)
    central_hp.setAncillaryPower(1.1)
    assert_equal(1.1, central_hp.ancillaryPower)

    assert(central_hp.ancillaryOperationSchedule.empty?)
    assert(central_hp.setAncillaryOperationSchedule(model.alwaysOnDiscreteSchedule))
    assert(central_hp.ancillaryOperationSchedule)
    assert_equal(model.alwaysOnDiscreteSchedule, central_hp.ancillaryOperationSchedule.get)


    central_hp.modules


  end

  def test_CentralHeatPumpPlantLoops

    model = OpenStudio::Model::Model.new

    central_hp = OpenStudio::Model::CentralHeatPumpSystem.new(model)

    # PlantLoop = CoolingPlantLoop
    coolingPlantLoop = OpenStudio::Model::PlantLoop.new(model)
    assert(central_hp.addToNode(coolingPlantLoop.supplyInletNode))
    assert(central_hp.plantLoop)
    assert_equal(coolingPlantLoop, central_hp.plantLoop.get)
    assert(central_hp.coolingPlantLoop)
    assert_equal(coolingPlantLoop, central_hp.coolingPlantLoop.get)

    # secondaryPlantLoop = Source Plant Loop
    sourcePlantLoop = OpenStudio::Model::PlantLoop.new(model)
    assert(central_hp.addToNode(sourcePlantLoop.demandInletNode))
    assert(central_hp.secondaryPlantLoop)
    assert_equal(sourcePlantLoop, central_hp.secondaryPlantLoop.get)
    assert(central_hp.sourcePlantLoop)
    assert_equal(sourcePlantLoop, central_hp.sourcePlantLoop.get)

    heatingPlantLoop = OpenStudio::Model::PlantLoop.new(model)
    assert(central_hp.addToTertiaryNode(heatingPlantLoop.supplyInletNode))
    assert(central_hp.tertiaryPlantLoop)
    assert_equal(heatingPlantLoop, central_hp.tertiaryPlantLoop.get)
    assert(central_hp.heatingPlantLoop)
    assert_equal(heatingPlantLoop, central_hp.heatingPlantLoop.get)

  end


end
