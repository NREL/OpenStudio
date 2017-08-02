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
    #

    ch_heaterClone = ch_heater.clone(model)
    ch_heaterClone = ch_heaterClone.to_ChillerHeaterPerformanceElectricEIR.get

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

    # tertiaryPlantLoop = Hot Water loop
    heatingPlantLoop = OpenStudio::Model::PlantLoop.new(model)
    assert(central_hp.addToTertiaryNode(heatingPlantLoop.supplyInletNode))
    assert(central_hp.tertiaryPlantLoop)
    assert_equal(heatingPlantLoop, central_hp.tertiaryPlantLoop.get)
    assert(central_hp.heatingPlantLoop)
    assert_equal(heatingPlantLoop, central_hp.heatingPlantLoop.get)

  end

  def test_clone_in_same_model

    model = OpenStudio::Model::Model.new

    central_hp = OpenStudio::Model::CentralHeatPumpSystem.new(model)

    # PlantLoop = CoolingPlantLoop
    coolingPlantLoop = OpenStudio::Model::PlantLoop.new(model)
    central_hp.addToNode(coolingPlantLoop.supplyInletNode)
    # secondaryPlantLoop = Source Plant Loop
    sourcePlantLoop = OpenStudio::Model::PlantLoop.new(model)
    central_hp.addToNode(sourcePlantLoop.demandInletNode)
    # tertiaryPlantLoop = Hot Water loop
    heatingPlantLoop = OpenStudio::Model::PlantLoop.new(model)
    central_hp.addToTertiaryNode(heatingPlantLoop.supplyInletNode)

    central_hpClone = central_hp.clone(model)
    central_hpClone = central_hpClone.to_CentralHeatPumpSystem.get
    assert_equal(2, model.getCentralHeatPumpSystems.size)

    # Make sure it's isn't connected to the same loops
    assert(central_hpClone.coolingPlantLoop.empty?)
    assert(central_hpClone.sourcePlantLoop.empty?)
    assert(central_hpClone.heatingPlantLoop.empty?)


    mod = OpenStudio::Model::CentralHeatPumpSystemModule.new(model)
    assert_equal(1, model.getCentralHeatPumpSystemModules.size)
    assert_equal(1, model.getChillerHeaterPerformanceElectricEIRs.size)
    assert(central_hp.addModule(mod))
    assert_equal(1, central_hp.modules.size)
    assert_equal(0, central_hpClone.modules.size)

    # Clone again, this time the Modules and chiller will be cloned too
    central_hpClone1 = central_hp.clone(model)
    central_hpClone1 = central_hpClone1.to_CentralHeatPumpSystem.get
    assert_equal(3, model.getCentralHeatPumpSystems.size)
    assert_equal(2, model.getCentralHeatPumpSystemModules.size)
    assert_equal(2, model.getChillerHeaterPerformanceElectricEIRs.size)
    assert_equal(1, central_hpClone1.modules.size)
    modClone = central_hpClone1.modules[0]
    refute_equal(mod, modClone)

    chiller = mod.chillerHeaterModulesPerformanceComponent
    chillerClone = modClone.chillerHeaterModulesPerformanceComponent
    refute_equal(chiller, chillerClone)

  end

  def test_clone_in_other_model

    model = OpenStudio::Model::Model.new

    central_hp = OpenStudio::Model::CentralHeatPumpSystem.new(model)
    mod = OpenStudio::Model::CentralHeatPumpSystemModule.new(model)
    central_hp.addModule(mod)

    # Clone in other model
    model2 = OpenStudio::Model::Model.new
    # TODO: This throws warnings for curve objects having multiple parents
    # https://github.com/NREL/OpenStudio/blob/72e4961a2d6547dcb40be48f722345b5691e3a74/openstudiocore/src/model/Curve.cpp#L59
    central_hpClone1 = central_hp.clone(model2)
    central_hpClone1 = central_hpClone1.to_CentralHeatPumpSystem.get

    assert_equal(1, model2.getCentralHeatPumpSystems.size)

    assert_equal(1, model2.getCentralHeatPumpSystemModules.size)
    assert_equal(1, central_hpClone1.modules.size)

    assert_equal(1, model2.getChillerHeaterPerformanceElectricEIRs.size)

  end


end
