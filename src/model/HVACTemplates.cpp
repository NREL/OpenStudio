/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "HVACTemplates.hpp"
#include "Model.hpp"
#include "PipeAdiabatic.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "ChillerElectricEIR.hpp"
#include "ChillerElectricEIR_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "BoilerHotWater.hpp"
#include "BoilerHotWater_Impl.hpp"
#include "WaterHeaterMixed.hpp"
#include "WaterHeaterMixed_Impl.hpp"
#include "PumpVariableSpeed.hpp"
#include "PumpVariableSpeed_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "SizingPlant.hpp"
#include "SizingPlant_Impl.hpp"
#include "CoilCoolingDXSingleSpeed.hpp"
#include "CoilCoolingDXSingleSpeed_Impl.hpp"
#include "CoilCoolingDXTwoSpeed.hpp"
#include "CoilCoolingDXTwoSpeed_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CoilHeatingGas.hpp"
#include "CoilHeatingGas_Impl.hpp"
#include "CoilHeatingElectric.hpp"
#include "CoilHeatingElectric_Impl.hpp"
#include "CoilHeatingDXSingleSpeed.hpp"
#include "CoilHeatingDXSingleSpeed_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"
#include "SetpointManagerMixedAir_Impl.hpp"
#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "SetpointManagerScheduled.hpp"
#include "SetpointManagerScheduled_Impl.hpp"
#include "SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"
#include "SizingSystem.hpp"
#include "SizingSystem_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "CoolingTowerSingleSpeed.hpp"
#include "CoolingTowerSingleSpeed_Impl.hpp"
#include "PumpConstantSpeed.hpp"
#include "WaterUseEquipmentDefinition.hpp"
#include "WaterUseEquipment.hpp"
#include "WaterUseConnections.hpp"

#include "../utilities/time/Time.hpp"

namespace openstudio {

namespace model {

  Schedule makeSchedule(Model& model, std::string name = "Hot_Water_Temperature", double targetTemperature = 67.0) {
    //Make a time stamp to use in multiple places
    Time osTime = Time(0, 24, 0, 0);

    //Schedule Ruleset
    ScheduleRuleset TempSchedule = ScheduleRuleset(model);
    TempSchedule.setName(name);

    //Winter Design Day
    ScheduleDay TempScheduleWinter = ScheduleDay(model);
    TempSchedule.setWinterDesignDaySchedule(TempScheduleWinter);
    TempSchedule.winterDesignDaySchedule().setName(name + "_Winter_Design_Day");
    TempSchedule.winterDesignDaySchedule().addValue(osTime, targetTemperature);

    //Summer Design Day
    ScheduleDay TempScheduleSummer = ScheduleDay(model);
    TempSchedule.setSummerDesignDaySchedule(TempScheduleSummer);
    TempSchedule.summerDesignDaySchedule().setName(name + "_Summer_Design_Day");
    TempSchedule.summerDesignDaySchedule().addValue(osTime, targetTemperature);

    //All other days
    TempSchedule.defaultDaySchedule().setName(name + "_Default");
    TempSchedule.defaultDaySchedule().addValue(osTime, targetTemperature);

    return TempSchedule;
  }

  Schedule deckTempSchedule(Model& model) {
    return makeSchedule(model, "Deck_Temperature", 12.8);
  }

  Schedule hotWaterTempSchedule(Model& model) {
    return makeSchedule(model, "Hot_Water_Temperature", 67.0);
  }

  Schedule chilledWaterTempSchedule(Model& model) {
    return makeSchedule(model, "Chilled_Water_Temperature", 6.7);
  }

  ZoneHVACPackagedTerminalAirConditioner addSystemType1(Model& model) {
    Model tempModel;
    Schedule availabilitySchedule = model.alwaysOnDiscreteSchedule();

    model::FanConstantVolume fan(model, availabilitySchedule);
    fan.setPressureRise(500);

    model::CoilHeatingWater heatingCoil(model, availabilitySchedule);

    CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed(model);

    model::ZoneHVACPackagedTerminalAirConditioner ptac(model, availabilitySchedule, fan, heatingCoil, coolingCoil);

    return ptac;
  }

  void addSystemType1(Model& model, std::vector<ThermalZone> zones) {
    std::vector<model::ThermalZone> zonesToAddTo;

    for (const auto& zone : zones) {
      if (zone.model() == model) {
        zonesToAddTo.push_back(zone);
      }
    }

    // Hot Water Plant

    if (zonesToAddTo.size() > 0) {
      PlantLoop hotWaterPlant(model);
      hotWaterPlant.setName("Hot Water Loop");
      SizingPlant sizingPlant = hotWaterPlant.sizingPlant();
      sizingPlant.setLoopType("Heating");
      sizingPlant.setDesignLoopExitTemperature(82.0);
      sizingPlant.setLoopDesignTemperatureDifference(11.0);

      Node hotWaterOutletNode = hotWaterPlant.supplyOutletNode();
      Node hotWaterInletNode = hotWaterPlant.supplyInletNode();

      PumpVariableSpeed pump = PumpVariableSpeed(model);

      BoilerHotWater boiler = BoilerHotWater(model);

      CurveBiquadratic boilerCurve(model);
      boilerCurve.setName("Boiler Efficiency");
      boilerCurve.setCoefficient1Constant(1.0);
      boilerCurve.setInputUnitTypeforX("Dimensionless");
      boilerCurve.setInputUnitTypeforY("Dimensionless");
      boilerCurve.setOutputUnitType("Dimensionless");

      boiler.setNormalizedBoilerEfficiencyCurve(boilerCurve);
      boiler.setEfficiencyCurveTemperatureEvaluationVariable("LeavingBoiler");

      pump.addToNode(hotWaterInletNode);

      Node node = hotWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
      boiler.addToNode(node);

      PipeAdiabatic pipe(model);
      hotWaterPlant.addSupplyBranchForComponent(pipe);

      PipeAdiabatic pipe2(model);
      pipe2.addToNode(hotWaterOutletNode);

      Schedule _hotWaterSchedule = hotWaterTempSchedule(model);

      SetpointManagerScheduled hotWaterSPM(model, _hotWaterSchedule);

      hotWaterSPM.addToNode(hotWaterOutletNode);

      for (auto& zone : zonesToAddTo) {
        model::ZoneHVACPackagedTerminalAirConditioner ptac = addSystemType1(model);

        ptac.addToThermalZone(zone);

        HVACComponent comp = ptac.heatingCoil();

        hotWaterPlant.addDemandBranchForComponent(comp);
      }
    }
  }

  ZoneHVACPackagedTerminalHeatPump addSystemType2(Model& model) {
    Model tempModel;
    Schedule availabilitySchedule = model.alwaysOnDiscreteSchedule();

    model::FanConstantVolume fan(model, availabilitySchedule);
    fan.setPressureRise(300);

    model::CoilHeatingElectric supplementalHeatingCoil(model);

    CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed(model);

    CoilHeatingDXSingleSpeed heatingCoil(model);

    model::ZoneHVACPackagedTerminalHeatPump pthp(model, availabilitySchedule, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

    return pthp;
  }

  void addSystemType2(Model& model, std::vector<ThermalZone> zones) {
    for (auto& zone : zones) {
      if (zone.model() == model) {
        ZoneHVACPackagedTerminalHeatPump pthp = addSystemType2(model);

        pthp.addToThermalZone(zone);
      }
    }
  }

  Loop addSystemType3(Model& model) {
    Model tempModel;
    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("Packaged Rooftop Air Conditioner");
    // when an airloophvac is contructed, its constructor automatically creates a sizing:system object
    // the default sizing:system contstructor makes a system:sizing object appropriate for a multizone VAV system
    // this systems is a constant volume system with no VAV terminals, and needs different default settings

    // get the sizing:system object associated with the airloophvac
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

    //set the default parameters correctly for a constant volume system with no VAV terminals
    sizingSystem.setTypeofLoadtoSizeOn("Sensible");
    sizingSystem.autosizeDesignOutdoorAirFlowRate();
    sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(1.0);
    sizingSystem.setPreheatDesignTemperature(7.0);
    sizingSystem.setPreheatDesignHumidityRatio(0.008);
    sizingSystem.setPrecoolDesignTemperature(12.8);
    sizingSystem.setPrecoolDesignHumidityRatio(0.008);
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(12.8);
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(40.0);
    sizingSystem.setSizingOption("NonCoincident");
    sizingSystem.setAllOutdoorAirinCooling("No");
    sizingSystem.setAllOutdoorAirinHeating("No");
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(0.0085);
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(0.0080);
    sizingSystem.setCoolingDesignAirFlowMethod("DesignDay");
    sizingSystem.setCoolingDesignAirFlowRate(0.0);
    sizingSystem.setHeatingDesignAirFlowMethod("DesignDay");
    sizingSystem.setHeatingDesignAirFlowRate(0.0);
    sizingSystem.setSystemOutdoorAirMethod("ZoneSum");

    FanConstantVolume fan = FanConstantVolume(model);
    fan.setPressureRise(500);

    CoilHeatingGas coilHeatingGas = CoilHeatingGas(model, _alwaysOnSchedule);

    CoilCoolingDXSingleSpeed coilCooling = CoilCoolingDXSingleSpeed(model);

    SetpointManagerSingleZoneReheat setpointMSZR(model);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilCooling.addToNode(supplyOutletNode);
    coilHeatingGas.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    setpointMSZR.addToNode(node1);

    AirTerminalSingleDuctConstantVolumeNoReheat terminal(model, _alwaysOnSchedule);

    airLoopHVAC.addBranchForHVACComponent(terminal);

    return airLoopHVAC;
  }

  Loop addSystemType4(Model& model) {
    Model tempModel;

    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    AirLoopHVAC airLoopHVAC(model);
    airLoopHVAC.setName("Packaged Rooftop Heat Pump");
    // when an airloophvac is contructed, its constructor automatically creates a sizing:system object
    // the default sizing:system contstructor makes a system:sizing object appropriate for a multizone VAV system
    // this systems is a constant volume system with no VAV terminals, and needs different default settings

    // get the sizing:system object associated with the airloophvac
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

    //set the default parameters correctly for a constant volume system with no VAV terminals
    sizingSystem.setTypeofLoadtoSizeOn("Sensible");
    sizingSystem.autosizeDesignOutdoorAirFlowRate();
    sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(1.0);
    sizingSystem.setPreheatDesignTemperature(7.0);
    sizingSystem.setPreheatDesignHumidityRatio(0.008);
    sizingSystem.setPrecoolDesignTemperature(12.8);
    sizingSystem.setPrecoolDesignHumidityRatio(0.008);
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(12.8);
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(40.0);
    sizingSystem.setSizingOption("NonCoincident");
    sizingSystem.setAllOutdoorAirinCooling("No");
    sizingSystem.setAllOutdoorAirinHeating("No");
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(0.0085);
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(0.0080);
    sizingSystem.setCoolingDesignAirFlowMethod("DesignDay");
    sizingSystem.setCoolingDesignAirFlowRate(0.0);
    sizingSystem.setHeatingDesignAirFlowMethod("DesignDay");
    sizingSystem.setHeatingDesignAirFlowRate(0.0);
    sizingSystem.setSystemOutdoorAirMethod("ZoneSum");

    FanConstantVolume supplyFan(model, _alwaysOnSchedule);
    supplyFan.setPressureRise(500);

    CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed(model);

    CoilHeatingDXSingleSpeed heatingCoil(model);

    CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
    outdoorAirSystem.addToNode(supplyOutletNode);
    coolingCoil.addToNode(supplyOutletNode);
    heatingCoil.addToNode(supplyOutletNode);
    coilHeatingElectric.addToNode(supplyOutletNode);
    supplyFan.addToNode(supplyOutletNode);

    AirTerminalSingleDuctConstantVolumeNoReheat terminal(model, _alwaysOnSchedule);

    airLoopHVAC.addBranchForHVACComponent(terminal);

    SetpointManagerSingleZoneReheat setpointMSZR(model);

    Node node1 = supplyFan.outletModelObject()->cast<Node>();
    setpointMSZR.addToNode(node1);

    return airLoopHVAC;
  }

  Loop addSystemType5(Model& model) {
    Model tempModel;

    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    Schedule _deckTempSchedule = deckTempSchedule(tempModel).clone(model).cast<Schedule>();

    Schedule _hotWaterSchedule = hotWaterTempSchedule(tempModel).clone(model).cast<Schedule>();

    Schedule _chilledWaterSchedule = chilledWaterTempSchedule(tempModel).clone(model).cast<Schedule>();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("Packaged Rooftop VAV with Reheat");
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(12.8);
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(12.8);

    FanVariableVolume fan = FanVariableVolume(model);
    fan.setPressureRise(500);

    CoilHeatingWater coilHeatingWater = CoilHeatingWater(model);

    CoilCoolingDXTwoSpeed coilCooling = CoilCoolingDXTwoSpeed(model);

    SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model, _deckTempSchedule);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilCooling.addToNode(supplyOutletNode);
    coilHeatingWater.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    deckTempSPM.addToNode(node1);

    // Hot Water Plant

    PlantLoop hotWaterPlant(model);
    hotWaterPlant.setName("Hot Water Loop");
    SizingPlant sizingPlant = hotWaterPlant.sizingPlant();
    sizingPlant.setLoopType("Heating");
    sizingPlant.setDesignLoopExitTemperature(82.0);
    sizingPlant.setLoopDesignTemperatureDifference(11.0);

    Node hotWaterOutletNode = hotWaterPlant.supplyOutletNode();
    Node hotWaterInletNode = hotWaterPlant.supplyInletNode();

    PumpVariableSpeed pump = PumpVariableSpeed(model);

    BoilerHotWater boiler = BoilerHotWater(model);

    CurveBiquadratic boilerCurve(model);
    boilerCurve.setName("Boiler Efficiency");
    boilerCurve.setCoefficient1Constant(1.0);
    boilerCurve.setInputUnitTypeforX("Dimensionless");
    boilerCurve.setInputUnitTypeforY("Dimensionless");
    boilerCurve.setOutputUnitType("Dimensionless");

    boiler.setNormalizedBoilerEfficiencyCurve(boilerCurve);
    boiler.setEfficiencyCurveTemperatureEvaluationVariable("LeavingBoiler");

    pump.addToNode(hotWaterInletNode);

    Node node = hotWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
    boiler.addToNode(node);

    PipeAdiabatic pipe(model);
    hotWaterPlant.addSupplyBranchForComponent(pipe);

    PipeAdiabatic pipe2(model);
    pipe2.addToNode(hotWaterOutletNode);

    hotWaterPlant.addDemandBranchForComponent(coilHeatingWater);

    SetpointManagerScheduled hotWaterSPM(model, _hotWaterSchedule);

    hotWaterSPM.addToNode(hotWaterOutletNode);

    // Terminal

    CoilHeatingWater waterReheatCoil(model, _alwaysOnSchedule);
    AirTerminalSingleDuctVAVReheat waterTerminal(model, _alwaysOnSchedule, waterReheatCoil);
    airLoopHVAC.addBranchForHVACComponent(waterTerminal);

    hotWaterPlant.addDemandBranchForComponent(waterReheatCoil);

    return airLoopHVAC;
  }

  Loop addSystemType6(Model& model) {
    Model tempModel;
    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    Schedule _deckTempSchedule = deckTempSchedule(tempModel).clone(model).cast<Schedule>();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("Packaged Rooftop VAV with PFP Boxes and Reheat");
    FanVariableVolume fan = FanVariableVolume(model);
    fan.setPressureRise(500);

    CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model);

    CoilCoolingDXTwoSpeed coilCooling = CoilCoolingDXTwoSpeed(model);

    SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model, _deckTempSchedule);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilCooling.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);
    Node fanInletNode = fan.inletModelObject()->cast<Node>();
    coilHeatingElectric.addToNode(fanInletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    deckTempSPM.addToNode(node1);

    CoilHeatingElectric reheatCoil(model);
    FanConstantVolume piuFan(model, _alwaysOnSchedule);
    piuFan.setPressureRise(300);
    AirTerminalSingleDuctParallelPIUReheat terminal(model, _alwaysOnSchedule, piuFan, reheatCoil);
    airLoopHVAC.addBranchForHVACComponent(terminal);

    return airLoopHVAC;
  }

  Loop addSystemType7(Model& model) {
    Model tempModel;

    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    Schedule _deckTempSchedule = deckTempSchedule(tempModel).clone(model).cast<Schedule>();

    Schedule _hotWaterSchedule = hotWaterTempSchedule(tempModel).clone(model).cast<Schedule>();

    Schedule _chilledWaterSchedule = chilledWaterTempSchedule(tempModel).clone(model).cast<Schedule>();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("VAV with Reheat");
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(12.8);
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(12.8);

    FanVariableVolume fan = FanVariableVolume(model);
    fan.setPressureRise(500);

    CoilHeatingWater coilHeatingWater = CoilHeatingWater(model);

    CoilCoolingWater coilCoolingWater = CoilCoolingWater(model);

    SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model, _deckTempSchedule);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilCoolingWater.addToNode(supplyOutletNode);
    coilHeatingWater.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    deckTempSPM.addToNode(node1);

    // Hot Water Plant

    PlantLoop hotWaterPlant(model);
    hotWaterPlant.setName("Hot Water Loop");
    SizingPlant sizingPlant = hotWaterPlant.sizingPlant();
    sizingPlant.setLoopType("Heating");
    sizingPlant.setDesignLoopExitTemperature(82.0);
    sizingPlant.setLoopDesignTemperatureDifference(11.0);

    Node hotWaterOutletNode = hotWaterPlant.supplyOutletNode();
    Node hotWaterInletNode = hotWaterPlant.supplyInletNode();

    Node hotWaterDemandOutletNode = hotWaterPlant.demandOutletNode();
    Node hotWaterDemandInletNode = hotWaterPlant.demandInletNode();

    PumpVariableSpeed pump = PumpVariableSpeed(model);

    BoilerHotWater boiler = BoilerHotWater(model);

    CurveBiquadratic boilerCurve(model);
    boilerCurve.setName("Boiler Efficiency");
    boilerCurve.setCoefficient1Constant(1.0);
    boilerCurve.setInputUnitTypeforX("Dimensionless");
    boilerCurve.setInputUnitTypeforY("Dimensionless");
    boilerCurve.setOutputUnitType("Dimensionless");

    boiler.setNormalizedBoilerEfficiencyCurve(boilerCurve);
    boiler.setEfficiencyCurveTemperatureEvaluationVariable("LeavingBoiler");

    pump.addToNode(hotWaterInletNode);

    Node node = hotWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
    boiler.addToNode(node);

    PipeAdiabatic pipe(model);
    hotWaterPlant.addSupplyBranchForComponent(pipe);

    PipeAdiabatic hotWaterBypass(model);
    hotWaterPlant.addDemandBranchForComponent(hotWaterBypass);

    hotWaterPlant.addDemandBranchForComponent(coilHeatingWater);

    PipeAdiabatic hotWaterDemandInlet(model);
    PipeAdiabatic hotWaterDemandOutlet(model);

    hotWaterDemandOutlet.addToNode(hotWaterDemandOutletNode);
    hotWaterDemandInlet.addToNode(hotWaterDemandInletNode);

    PipeAdiabatic pipe2(model);
    pipe2.addToNode(hotWaterOutletNode);

    SetpointManagerScheduled hotWaterSPM(model, _hotWaterSchedule);

    hotWaterSPM.addToNode(hotWaterOutletNode);

    // Chilled Water Plant

    PlantLoop chilledWaterPlant(model);
    chilledWaterPlant.setName("Chilled Water Loop");
    SizingPlant chilledWaterSizing = chilledWaterPlant.sizingPlant();
    chilledWaterSizing.setLoopType("Cooling");
    chilledWaterSizing.setDesignLoopExitTemperature(7.22);
    chilledWaterSizing.setLoopDesignTemperatureDifference(6.67);

    Node chilledWaterOutletNode = chilledWaterPlant.supplyOutletNode();
    Node chilledWaterInletNode = chilledWaterPlant.supplyInletNode();

    Node chilledWaterDemandOutletNode = chilledWaterPlant.demandOutletNode();
    Node chilledWaterDemandInletNode = chilledWaterPlant.demandInletNode();

    PumpVariableSpeed pump2 = PumpVariableSpeed(model);

    pump2.addToNode(chilledWaterInletNode);

    CurveBiquadratic ccFofT(model);
    ccFofT.setCoefficient1Constant(0.258);
    ccFofT.setCoefficient2x(0.0389);
    ccFofT.setCoefficient3xPOW2(-0.000217);
    ccFofT.setCoefficient4y(0.0469);
    ccFofT.setCoefficient5yPOW2(-0.000943);
    ccFofT.setCoefficient6xTIMESY(-0.000343);
    ccFofT.setMinimumValueofx(5);
    ccFofT.setMaximumValueofx(10);
    ccFofT.setMinimumValueofy(24);
    ccFofT.setMaximumValueofy(35);

    CurveBiquadratic eiToCORFofT(model);
    eiToCORFofT.setCoefficient1Constant(0.934);
    eiToCORFofT.setCoefficient2x(-0.0582);
    eiToCORFofT.setCoefficient3xPOW2(0.0045);
    eiToCORFofT.setCoefficient4y(0.00243);
    eiToCORFofT.setCoefficient5yPOW2(0.000486);
    eiToCORFofT.setCoefficient6xTIMESY(-0.00122);
    eiToCORFofT.setMinimumValueofx(5);
    eiToCORFofT.setMaximumValueofx(10);
    eiToCORFofT.setMinimumValueofy(24);
    eiToCORFofT.setMaximumValueofy(35);

    CurveQuadratic eiToCORFofPLR(model);
    eiToCORFofPLR.setCoefficient1Constant(0.222903);
    eiToCORFofPLR.setCoefficient2x(0.313387);
    eiToCORFofPLR.setCoefficient3xPOW2(0.46371);
    eiToCORFofPLR.setMinimumValueofx(0);
    eiToCORFofPLR.setMaximumValueofx(1);

    ChillerElectricEIR chiller(model, ccFofT, eiToCORFofT, eiToCORFofPLR);

    node = chilledWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
    chiller.addToNode(node);

    PipeAdiabatic pipe3(model);
    chilledWaterPlant.addSupplyBranchForComponent(pipe3);

    PipeAdiabatic pipe4(model);
    pipe4.addToNode(chilledWaterOutletNode);

    chilledWaterPlant.addDemandBranchForComponent(coilCoolingWater);

    SetpointManagerScheduled chilledWaterSPM(model, _chilledWaterSchedule);

    chilledWaterSPM.addToNode(chilledWaterOutletNode);

    CoilHeatingWater waterReheatCoil(model, _alwaysOnSchedule);
    AirTerminalSingleDuctVAVReheat waterTerminal(model, _alwaysOnSchedule, waterReheatCoil);
    airLoopHVAC.addBranchForHVACComponent(waterTerminal);

    PipeAdiabatic chilledWaterDemandBypass(model);
    chilledWaterPlant.addDemandBranchForComponent(chilledWaterDemandBypass);

    hotWaterPlant.addDemandBranchForComponent(waterReheatCoil);

    PipeAdiabatic chilledWaterDemandInlet(model);
    chilledWaterDemandInlet.addToNode(chilledWaterDemandInletNode);

    PipeAdiabatic chilledWaterDemandOutlet(model);
    chilledWaterDemandOutlet.addToNode(chilledWaterDemandOutletNode);

    // Condenser System

    PlantLoop condenserSystem(model);
    condenserSystem.setName("Condenser Water Loop");
    SizingPlant condenserSizing = condenserSystem.sizingPlant();
    condenserSizing.setLoopType("Condenser");
    condenserSizing.setDesignLoopExitTemperature(29.4);
    condenserSizing.setLoopDesignTemperatureDifference(5.6);

    CoolingTowerSingleSpeed tower(model);
    condenserSystem.addSupplyBranchForComponent(tower);

    PipeAdiabatic condenserSupplyBypass(model);
    condenserSystem.addSupplyBranchForComponent(condenserSupplyBypass);

    Node condenserSystemDemandOutletNode = condenserSystem.demandOutletNode();
    Node condenserSystemDemandInletNode = condenserSystem.demandInletNode();
    Node condenserSystemSupplyOutletNode = condenserSystem.supplyOutletNode();
    Node condenserSystemSupplyInletNode = condenserSystem.supplyInletNode();

    PipeAdiabatic condenserSupplyOutlet(model);

    condenserSupplyOutlet.addToNode(condenserSystemSupplyOutletNode);

    PumpVariableSpeed pump3 = PumpVariableSpeed(model);

    pump3.addToNode(condenserSystemSupplyInletNode);

    condenserSystem.addDemandBranchForComponent(chiller);

    PipeAdiabatic condenserDemandBypass(model);

    condenserSystem.addDemandBranchForComponent(condenserDemandBypass);

    PipeAdiabatic condenserDemandInlet(model);

    condenserDemandInlet.addToNode(condenserSystemDemandInletNode);

    PipeAdiabatic condenserDemandOutlet(model);

    condenserDemandOutlet.addToNode(condenserSystemDemandOutletNode);

    SetpointManagerFollowOutdoorAirTemperature spm(model);

    spm.addToNode(condenserSystemSupplyOutletNode);

    return airLoopHVAC;
  }

  Loop addSystemType8(Model& model) {
    Model tempModel;
    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    Schedule _chilledWaterSchedule = chilledWaterTempSchedule(tempModel).clone(model).cast<Schedule>();

    Schedule _deckTempSchedule = deckTempSchedule(tempModel).clone(model).cast<Schedule>();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("VAV with PFP Boxes and Reheat");
    FanVariableVolume fan = FanVariableVolume(model);
    fan.setPressureRise(500);

    CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model);

    CoilCoolingWater coilCoolingWater = CoilCoolingWater(model);

    SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model, _deckTempSchedule);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilCoolingWater.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);
    Node fanInletNode = fan.inletModelObject()->cast<Node>();
    coilHeatingElectric.addToNode(fanInletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    deckTempSPM.addToNode(node1);

    // Chilled Water Plant

    PlantLoop chilledWaterPlant(model);
    chilledWaterPlant.setName("Chilled Water Loop");
    SizingPlant sizingPlant = chilledWaterPlant.sizingPlant();
    sizingPlant.setLoopType("Cooling");
    sizingPlant.setDesignLoopExitTemperature(7.22);
    sizingPlant.setLoopDesignTemperatureDifference(6.67);

    Node chilledWaterOutletNode = chilledWaterPlant.supplyOutletNode();
    Node chilledWaterInletNode = chilledWaterPlant.supplyInletNode();

    PumpVariableSpeed pump2 = PumpVariableSpeed(model);

    pump2.addToNode(chilledWaterInletNode);

    ChillerElectricEIR chiller(model);

    Node node = chilledWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
    chiller.addToNode(node);

    chilledWaterPlant.addDemandBranchForComponent(coilCoolingWater);

    PipeAdiabatic pipe(model);
    chilledWaterPlant.addSupplyBranchForComponent(pipe);

    PipeAdiabatic pipe2(model);
    pipe2.addToNode(chilledWaterOutletNode);

    SetpointManagerScheduled chilledWaterSPM(model, _chilledWaterSchedule);

    chilledWaterSPM.addToNode(chilledWaterOutletNode);

    CoilHeatingElectric reheatCoil(model);
    FanConstantVolume piuFan(model, _alwaysOnSchedule);
    piuFan.setPressureRise(300);
    AirTerminalSingleDuctParallelPIUReheat terminal(model, _alwaysOnSchedule, piuFan, reheatCoil);
    airLoopHVAC.addBranchForHVACComponent(terminal);

    return airLoopHVAC;
  }

  Loop addSystemType9(Model& model) {
    Model tempModel;
    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("Gas Fired Furnace");
    // when an airloophvac is contructed, its constructor automatically creates a sizing:system object
    // the default sizing:system contstructor makes a system:sizing object appropriate for a multizone VAV system
    // this systems is a constant volume system with no VAV terminals, and needs different default settings

    // get the sizing:system object associated with the airloophvac
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

    //set the default parameters correctly for a constant volume system with no VAV terminals
    sizingSystem.setTypeofLoadtoSizeOn("Sensible");
    sizingSystem.autosizeDesignOutdoorAirFlowRate();
    sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(1.0);
    sizingSystem.setPreheatDesignTemperature(7.0);
    sizingSystem.setPreheatDesignHumidityRatio(0.008);
    sizingSystem.setPrecoolDesignTemperature(12.8);
    sizingSystem.setPrecoolDesignHumidityRatio(0.008);
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(12.8);
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(40.0);
    sizingSystem.setSizingOption("NonCoincident");
    sizingSystem.setAllOutdoorAirinCooling("No");
    sizingSystem.setAllOutdoorAirinHeating("No");
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(0.0085);
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(0.0080);
    sizingSystem.setCoolingDesignAirFlowMethod("DesignDay");
    sizingSystem.setCoolingDesignAirFlowRate(0.0);
    sizingSystem.setHeatingDesignAirFlowMethod("DesignDay");
    sizingSystem.setHeatingDesignAirFlowRate(0.0);
    sizingSystem.setSystemOutdoorAirMethod("ZoneSum");

    FanConstantVolume fan = FanConstantVolume(model);
    fan.setPressureRise(500);

    CoilHeatingGas coilHeatingGas = CoilHeatingGas(model, _alwaysOnSchedule);

    SetpointManagerSingleZoneReheat setpointMSZR(model);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilHeatingGas.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    setpointMSZR.addToNode(node1);

    AirTerminalSingleDuctConstantVolumeNoReheat terminal(model, _alwaysOnSchedule);

    airLoopHVAC.addBranchForHVACComponent(terminal);

    return airLoopHVAC;
  }

  Loop addSystemType10(Model& model) {
    Model tempModel;
    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
    airLoopHVAC.setName("Electric Furnace");
    // when an airloophvac is contructed, its constructor automatically creates a sizing:system object
    // the default sizing:system contstructor makes a system:sizing object appropriate for a multizone VAV system
    // this systems is a constant volume system with no VAV terminals, and needs different default settings

    // get the sizing:system object associated with the airloophvac
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

    //set the default parameters correctly for a constant volume system with no VAV terminals
    sizingSystem.setTypeofLoadtoSizeOn("Sensible");
    sizingSystem.autosizeDesignOutdoorAirFlowRate();
    sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(1.0);
    sizingSystem.setPreheatDesignTemperature(7.0);
    sizingSystem.setPreheatDesignHumidityRatio(0.008);
    sizingSystem.setPrecoolDesignTemperature(12.8);
    sizingSystem.setPrecoolDesignHumidityRatio(0.008);
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(12.8);
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(40.0);
    sizingSystem.setSizingOption("NonCoincident");
    sizingSystem.setAllOutdoorAirinCooling("No");
    sizingSystem.setAllOutdoorAirinHeating("No");
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(0.0085);
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(0.0080);
    sizingSystem.setCoolingDesignAirFlowMethod("DesignDay");
    sizingSystem.setCoolingDesignAirFlowRate(0.0);
    sizingSystem.setHeatingDesignAirFlowMethod("DesignDay");
    sizingSystem.setHeatingDesignAirFlowRate(0.0);
    sizingSystem.setSystemOutdoorAirMethod("ZoneSum");

    FanConstantVolume fan = FanConstantVolume(model);
    fan.setPressureRise(500);

    CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model);

    SetpointManagerSingleZoneReheat setpointMSZR(model);

    ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model, controllerOutdoorAir);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    outdoorAirSystem.addToNode(supplyOutletNode);
    coilHeatingElectric.addToNode(supplyOutletNode);
    fan.addToNode(supplyOutletNode);

    Node node1 = fan.outletModelObject()->cast<Node>();
    setpointMSZR.addToNode(node1);

    AirTerminalSingleDuctConstantVolumeNoReheat terminal(model, _alwaysOnSchedule);

    airLoopHVAC.addBranchForHVACComponent(terminal);

    return airLoopHVAC;
  }

  Loop addSHWLoop(Model& model) {

    Model tempModel;
    // Make a schedule at 140°F
    Schedule shwSchedule = makeSchedule(tempModel, "SHW_Temperature_140F", 60).clone(model).cast<Schedule>();

    Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

    // SHW Plant
    PlantLoop shwPlant(model);
    shwPlant.setName("Service Water Loop");

    // Initialize nodes
    Node shwOutletNode = shwPlant.supplyOutletNode();
    Node shwInletNode = shwPlant.supplyInletNode();
    Node shwDemandOutletNode = shwPlant.demandOutletNode();
    Node shwDemandInletNode = shwPlant.demandInletNode();

    // Set temperatures
    shwPlant.setMaximumLoopTemperature(60);
    shwPlant.setMinimumLoopTemperature(10);
    SizingPlant sizingPlant = shwPlant.sizingPlant();
    sizingPlant.setLoopType("Heating");
    // Supply at 60°C (140°F), delta is 5°K (9°F)
    sizingPlant.setDesignLoopExitTemperature(60);
    sizingPlant.setLoopDesignTemperatureDifference(5.0);

    SetpointManagerScheduled shwSPM(model, shwSchedule);
    shwSPM.addToNode(shwOutletNode);

    // Pump: assume 10 ft of head, just from friction (recirculation)
    PumpConstantSpeed swh_pump(model);
    swh_pump.setRatedPumpHead(29891);
    swh_pump.setPumpControlType("Intermittent");
    swh_pump.addToNode(shwInletNode);

    // WaterHeater
    // Mostly taken from prototype input for HighRiseApartment
    WaterHeaterMixed wh(model);
    // Connect it to loop
    shwPlant.addSupplyBranchForComponent(wh);
    // 600 gal, 140F cut-out, delta of 5K(9F) means cut-in is 131°F
    wh.setName("600 gal Water Heater - 600 kBtu/hr");
    wh.setTankVolume(2.2712470703819108);
    wh.setSetpointTemperatureSchedule(shwSchedule);
    wh.setDeadbandTemperatureDifference(5);
    // 600 kBTU/h
    wh.setHeaterMaximumCapacity(175842);
    // Max temp is 180F
    wh.setMaximumTemperatureLimit(82.22222);
    wh.setHeaterControlType("Cycle");
    wh.setOffCycleParasiticHeatFractiontoTank(0.8);
    wh.setIndirectWaterHeatingRecoveryTime(1.5);  // 1.5hrs

    // Assume gas
    wh.setHeaterFuelType("NaturalGas");
    // Result from running the prototype building for 90.1-2013 HighRiseApartment
    wh.setHeaterThermalEfficiency(0.80154340529419);

    // Set it to Ambient schedule, at 70°F
    Schedule ambientSchedule = makeSchedule(tempModel, "Water Heater Ambient Temp Schedule - 70F", 21.11).clone(model).cast<Schedule>();
    wh.setAmbientTemperatureSchedule(ambientSchedule);
    wh.setAmbientTemperatureIndicator("Schedule");

    // 8742 Btu/hr (from prototype)
    wh.setOffCycleParasiticFuelConsumptionRate(2562);
    wh.setOnCycleParasiticFuelConsumptionRate(2562);
    wh.setOffCycleParasiticFuelType("NaturalGas");
    wh.setOnCycleParasiticFuelType("NaturalGas");
    wh.setOffCycleLossCoefficienttoAmbientTemperature(6.0);
    wh.setOnCycleLossCoefficienttoAmbientTemperature(6.0);
    wh.setEndUseSubcategory("Service Hot Water Heating");

    // Make a typical DHW Demand schedule
    ScheduleRuleset dhw_flow_sch(model);
    dhw_flow_sch.setName("ESTAR MFHR DHW Fraction Schedule");

    ScheduleDay dhw_sch_day = dhw_flow_sch.defaultDaySchedule();
    dhw_sch_day.setName("ESTAR MFHR DHW Schedule Day");

    // dhw_flow_sch.setScheduleTypeLimits(frac);

    // map of schedule info:
    // Key is the hour until
    // Value is the value
    //
    std::map<int, double> schInfo = {
      std::pair<int, double>(6, 0.05),  std::pair<int, double>(7, 0.3),   std::pair<int, double>(8, 0.5),   std::pair<int, double>(9, 0.4),
      std::pair<int, double>(11, 0.3),  std::pair<int, double>(12, 0.35), std::pair<int, double>(13, 0.40), std::pair<int, double>(15, 0.35),
      std::pair<int, double>(17, 0.30), std::pair<int, double>(19, 0.5),  std::pair<int, double>(20, 0.40), std::pair<int, double>(21, 0.35),
      std::pair<int, double>(22, 0.45), std::pair<int, double>(23, 0.3),  std::pair<int, double>(24, 0.05),
    };

    std::map<int, double>::iterator it;

    double dhw_daily_frac = 0;
    int lastHour = 0;
    for (it = schInfo.begin(); it != schInfo.end(); it++) {
      int hour = it->first;
      double value = it->second;
      Time t = Time(0, hour, 0, 0);
      dhw_sch_day.addValue(t, value);
      dhw_daily_frac += (hour - lastHour) * it->second;
      lastHour = hour;
    }

    // Assume occupancy type = "Medium" => 25 GPM / person
    // Assume 100 apartments, with 200 occupants, Medium Occupancy, and some standard GPMs (1.5 kitchen, 1.0 bathroom, 1.2 showerheads)
    int num_occupants = 200;
    int gpm_per_person_per_day = 25;
    int baseline_occupant_gal_per_day = num_occupants * gpm_per_person_per_day;
    // One kitchen, one bath per apartment
    int num_kitchen_faucets = 100;
    int num_bathroom_faucets = 100;
    double proposed_gpm_kitchen = 1.5;
    double proposed_gpm_bathroom = 1.0;
    double proposed_gpm_showerhead = 1.2;

    // Calculate the weighted average gpm for faucets
    double proposed_gpm_faucet_weighted =
      (num_kitchen_faucets * proposed_gpm_kitchen + num_bathroom_faucets * proposed_gpm_bathroom) / (num_kitchen_faucets + num_bathroom_faucets);

    double proposed_occupant_gal_per_day =
      baseline_occupant_gal_per_day * (0.36 + (0.54 * proposed_gpm_showerhead + 0.1 * proposed_gpm_faucet_weighted) / 2.5);

    // No dishwashers nor laundry
    double total_gal_per_day = proposed_occupant_gal_per_day;

    // Divide by total daily fraction
    double dhw_gpm = total_gal_per_day / (60 * dhw_daily_frac);

    // Add a sample service hot water connection
    WaterUseEquipmentDefinition water_use_equip_def(model);
    water_use_equip_def.setName("SWH Definition");
    water_use_equip_def.setName("Whole Building Water Equipment Definition - 100 units 200 occupants");
    water_use_equip_def.setPeakFlowRate(dhw_gpm * 6.309019639949753e-05);

    // Set Fraction sensible and latent

    //Make a time stamp to use in multiple places
    Time osTime = Time(0, 24, 0, 0);

    //Schedule Ruleset
    ScheduleRuleset fracSchedule = ScheduleRuleset(model);
    fracSchedule.setName("Fraction Sensible - 0.2");
    fracSchedule.defaultDaySchedule().setName("Fraction Sensible DefaultDay - 0.2");
    fracSchedule.defaultDaySchedule().addValue(osTime, 0.2);
    water_use_equip_def.setSensibleFractionSchedule(fracSchedule);

    fracSchedule = ScheduleRuleset(model);
    fracSchedule.setName("Fraction Sensible - 0.05");
    fracSchedule.defaultDaySchedule().setName("Fraction Latent DefaultDay - 0.05");
    fracSchedule.defaultDaySchedule().addValue(osTime, 0.05);
    water_use_equip_def.setLatentFractionSchedule(fracSchedule);

    // TODO: make 120F?
    water_use_equip_def.setTargetTemperatureSchedule(shwSchedule);

    WaterUseEquipment water_use_equip(water_use_equip_def);
    water_use_equip.setFlowRateFractionSchedule(dhw_flow_sch);
    water_use_equip.setName("ESTAR MFHR Whole Building Water Use Equipment");

    // add water use connection
    WaterUseConnections water_use_connection(model);
    water_use_connection.addWaterUseEquipment(water_use_equip);
    water_use_connection.setName("ESTAR MFHR DHW Water Use Connection");

    shwPlant.addDemandBranchForComponent(water_use_connection);

    /*
 *  PipeAdiabatic shwBypass(model);
 *  shwPlant.addDemandBranchForComponent(shwBypass);
 *
 *  shwPlant.addDemandBranchForComponent(coilHeatingWater);
 *
 *  PipeAdiabatic shwDemandInlet(model);
 *  PipeAdiabatic shwDemandOutlet(model);
 *
 *  shwDemandOutlet.addToNode(shwDemandOutletNode);
 *  shwDemandInlet.addToNode(shwDemandInletNode);
 *
 *  PipeAdiabatic pipe2(model);
 *  pipe2.addToNode(shwOutletNode);
 *
 */

    return shwPlant;
  }

}  // namespace model

}  // namespace openstudio
