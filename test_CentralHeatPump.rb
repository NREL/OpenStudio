# Requiring my custom build of OS
require '/Users/julien/Software/Others/OS-build/Products/ruby/openstudio'
require 'minitest/autorun'

class CentralHeatPump_Test < MiniTest::Unit::TestCase
  # def setup
  # end

  # def teardown

  def test_CentralHeatPumpSystem_Basic
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

  end

  def test_CentralHeatPumpySystemModule_Basic
    model = OpenStudio::Model::Model.new

    central_hp_module = OpenStudio::Model::CentralHeatPumpSystemModule.new(model)
    assert_equal(1, central_hp_module.numberofChillerHeaterModules)
    assert(central_hp_module.setNumberofChillerHeaterModules(3))
    assert_equal(3, central_hp_module.numberofChillerHeaterModules)

    assert_equal(model.alwaysOnDiscreteSchedule, central_hp_module.chillerHeaterModulesControlSchedule)
    sch = OpenStudio::Model::ScheduleRuleset.new(model)
    assert(central_hp_module.setChillerHeaterModulesControlSchedule(sch))
    assert_equal(sch, central_hp_module.chillerHeaterModulesControlSchedule)

    ch_heater = OpenStudio::Model::ChillerHeaterPerformanceElectricEIR.new(model)
    assert(central_hp_module.setChillerHeaterModulesPerformanceComponent(ch_heater))
    assert_equal(ch_heater, central_hp_module.chillerHeaterModulesPerformanceComponent)

  end

  def test_ChillerHeaterPerformanceElectricEIR_Basic
    model = OpenStudio::Model::Model.new

    ch_heater = OpenStudio::Model::ChillerHeaterPerformanceElectricEIR.new(model)

    # Blah, I should write this, but already did it in GTest and it's too long
    # and repetive

  end


  def test_CentralHeatPumpySystemModule_AddRemove
    model = OpenStudio::Model::Model.new

    central_hp = OpenStudio::Model::CentralHeatPumpSystem.new(model)

    assert_equal(0, central_hp.modules.size)
    central_hp_module = OpenStudio::Model::CentralHeatPumpSystemModule.new(model)
    assert(central_hp.addModule(central_hp_module))
    assert_equal(1, central_hp.modules.size)
    central_hp_module_2 = OpenStudio::Model::CentralHeatPumpSystemModule.new(model)
    assert(central_hp.addModule(central_hp_module_2))
    assert_equal(2, central_hp.modules.size)
    central_hp_module_3 = OpenStudio::Model::CentralHeatPumpSystemModule.new(model)
    assert(central_hp.addModule(central_hp_module_3))
    assert_equal(3, central_hp.modules.size)

    central_hp.removeModule(central_hp_module_2)
    assert_equal(2, central_hp.modules.size)

    central_hp.removeAllModules
    assert_equal(0, central_hp.modules.size)

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
