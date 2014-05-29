/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "AirTerminalSingleDuctUncontrolled_Impl.hpp"
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
#include "../utilities/time/Time.hpp"

namespace openstudio {

namespace model {

Schedule deckTempSchedule( Model & model )
{
  //Make a time stamp to use in multiple places
  Time osTime = Time(0,24,0,0);
  
  //Schedule Ruleset
  ScheduleRuleset deckTempSchedule = ScheduleRuleset(model);
  deckTempSchedule.setName("Deck_Temperature");

  //Winter Design Day
  ScheduleDay deckTempScheduleWinter = ScheduleDay(model);
  deckTempSchedule.setWinterDesignDaySchedule(deckTempScheduleWinter);
  deckTempSchedule.winterDesignDaySchedule().setName("Deck_Temperature_Winter_Design_Day");
  deckTempSchedule.winterDesignDaySchedule().addValue(osTime,12.8);

  //Summer Design Day
  ScheduleDay deckTempScheduleSummer = ScheduleDay(model);
  deckTempSchedule.setSummerDesignDaySchedule(deckTempScheduleSummer);
  deckTempSchedule.summerDesignDaySchedule().setName("Deck_Temperature_Summer_Design_Day");
  deckTempSchedule.summerDesignDaySchedule().addValue(osTime,12.8);  

  //All other days
  deckTempSchedule.defaultDaySchedule().setName("Deck_Temperature_Default");
  deckTempSchedule.defaultDaySchedule().addValue(osTime,12.8);

  return deckTempSchedule;
}

Schedule hotWaterTempSchedule( Model & model )
{
 //Make a time stamp to use in multiple places
  Time osTime = Time(0,24,0,0);
  
  //Schedule Ruleset
  ScheduleRuleset hotWaterTempSchedule = ScheduleRuleset(model);
  hotWaterTempSchedule.setName("Hot_Water_Temperature");

  //Winter Design Day
  ScheduleDay hotWaterTempScheduleWinter = ScheduleDay(model);
  hotWaterTempSchedule.setWinterDesignDaySchedule(hotWaterTempScheduleWinter);
  hotWaterTempSchedule.winterDesignDaySchedule().setName("Hot_Water_Temperature_Winter_Design_Day");
  hotWaterTempSchedule.winterDesignDaySchedule().addValue(osTime,67);

  //Summer Design Day
  ScheduleDay hotWaterTempScheduleSummer = ScheduleDay(model);
  hotWaterTempSchedule.setSummerDesignDaySchedule(hotWaterTempScheduleSummer);
  hotWaterTempSchedule.summerDesignDaySchedule().setName("Hot_Water_Temperature_Summer_Design_Day");
  hotWaterTempSchedule.summerDesignDaySchedule().addValue(osTime,67);  

  //All other days
  hotWaterTempSchedule.defaultDaySchedule().setName("Hot_Water_Temperature_Default");
  hotWaterTempSchedule.defaultDaySchedule().addValue(osTime,67);

  return hotWaterTempSchedule;
}

Schedule chilledWaterTempSchedule( Model & model )
{
 //Make a time stamp to use in multiple places
  Time osTime = Time(0,24,0,0);

  //Schedule Ruleset
  ScheduleRuleset chilledWaterSchedule = ScheduleRuleset(model);
  chilledWaterSchedule.setName("Chilled_Water_Temperature");

  //Winter Design Day
  ScheduleDay chilledWaterScheduleWinter = ScheduleDay(model);
  chilledWaterSchedule.setWinterDesignDaySchedule(chilledWaterScheduleWinter);
  chilledWaterSchedule.winterDesignDaySchedule().setName("Chilled_Water_Temperature_Winter_Design_Day");
  chilledWaterSchedule.winterDesignDaySchedule().addValue(osTime,6.7);

  //Summer Design Day
  ScheduleDay chilledWaterScheduleSummer = ScheduleDay(model);
  chilledWaterSchedule.setSummerDesignDaySchedule(chilledWaterScheduleSummer);
  chilledWaterSchedule.summerDesignDaySchedule().setName("Chilled_Water_Temperature_Summer_Design_Day");
  chilledWaterSchedule.summerDesignDaySchedule().addValue(osTime,6.7);  

  //All other days
  chilledWaterSchedule.defaultDaySchedule().setName("Chilled_Water_Temperature_Default");
  chilledWaterSchedule.defaultDaySchedule().addValue(osTime,6.7);

  return chilledWaterSchedule;
}

ZoneHVACPackagedTerminalAirConditioner addSystemType1(Model & model)
{
  Model tempModel;
  Schedule availabilitySchedule = model.alwaysOnDiscreteSchedule();
    
  model::FanConstantVolume fan(model,availabilitySchedule);
  fan.setPressureRise(500);

  model::CoilHeatingWater heatingCoil(model,availabilitySchedule);

  model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
  coolingCurveFofTemp.setCoefficient2x(0.009543347);
  coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
  coolingCurveFofTemp.setCoefficient4y(-0.011042676);
  coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.8);
  coolingCurveFofFlow.setCoefficient2x(0.2);
  coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
  coolingCurveFofFlow.setMinimumValueofx(0.5);
  coolingCurveFofFlow.setMaximumValueofx(1.5);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
  energyInputRatioFofTemp.setCoefficient2x(0.034885008);
  energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
  energyInputRatioFofTemp.setCoefficient4y(0.004977216);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.1552);
  energyInputRatioFofFlow.setCoefficient2x(-0.1808);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.0256);
  energyInputRatioFofFlow.setMinimumValueofx(0.5);
  energyInputRatioFofFlow.setMaximumValueofx(1.5);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.85);
  partLoadFraction.setCoefficient2x(0.15);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed( model,
                                                                   availabilitySchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction );


  model::ZoneHVACPackagedTerminalAirConditioner ptac( model,
                                                      availabilitySchedule, 
                                                      fan,
                                                      heatingCoil,
                                                      coolingCoil );

  return ptac;
}

void addSystemType1(Model & model, std::vector<ThermalZone> zones)
{
  std::vector<model::ThermalZone> zonesToAddTo;

  for( const auto & zone : zones )
  {
    if( zone.model() == model )
    {
      zonesToAddTo.push_back(zone);
    }
  }

  // Hot Water Plant

  if( zonesToAddTo.size() > 0 )
  {
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

    SetpointManagerScheduled hotWaterSPM(model,_hotWaterSchedule);

    hotWaterSPM.addToNode(hotWaterOutletNode);

    for( auto & zone : zonesToAddTo )
    {
        model::ZoneHVACPackagedTerminalAirConditioner ptac = addSystemType1(model);

        ptac.addToThermalZone(zone);

        HVACComponent comp = ptac.heatingCoil();

        hotWaterPlant.addDemandBranchForComponent(comp);
    }
  }
}

ZoneHVACPackagedTerminalHeatPump addSystemType2(Model & model)
{
  Model tempModel;
  Schedule availabilitySchedule = model.alwaysOnDiscreteSchedule();

  model::FanConstantVolume fan(model,availabilitySchedule);
  fan.setPressureRise(300);

  model::CoilHeatingElectric supplementalHeatingCoil(model,availabilitySchedule);

  model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
  coolingCurveFofTemp.setCoefficient2x(0.009543347);
  coolingCurveFofTemp.setCoefficient3xPOW2(0.0018423);
  coolingCurveFofTemp.setCoefficient4y(-0.011042676);
  coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.718954);
  coolingCurveFofFlow.setCoefficient2x(0.435436);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.154193);
  coolingCurveFofFlow.setMinimumValueofx(0.75);
  coolingCurveFofFlow.setMaximumValueofx(1.25);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
  energyInputRatioFofTemp.setCoefficient2x(0.034885008);
  energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
  energyInputRatioFofTemp.setCoefficient4y(0.004977216);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.1552);
  energyInputRatioFofFlow.setCoefficient2x(-0.1808);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.0256);
  energyInputRatioFofFlow.setMinimumValueofx(0.5);
  energyInputRatioFofFlow.setMaximumValueofx(1.5);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.75);
  partLoadFraction.setCoefficient2x(0.25);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed( model,
                                                                   availabilitySchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction );

  CurveCubic totalHeatingCapacityFunctionofTemperatureCurve(model);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
  totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveCubic totalHeatingCapacityFunctionofFlowFractionCurve(model);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

  CurveCubic energyInputRatioFunctionofTemperatureCurve(model);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
  energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
  energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
  energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

  CurveQuadratic partLoadFractionCorrelationCurve(model);
  partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
  partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
  partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
  partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

  CoilHeatingDXSingleSpeed heatingCoil( model,
                                        availabilitySchedule,
                                        totalHeatingCapacityFunctionofTemperatureCurve,
                                        totalHeatingCapacityFunctionofFlowFractionCurve,
                                        energyInputRatioFunctionofTemperatureCurve,
                                        energyInputRatioFunctionofFlowFractionCurve,
                                        partLoadFractionCorrelationCurve ); 

  model::ZoneHVACPackagedTerminalHeatPump pthp( model,
                                                availabilitySchedule, 
                                                fan,
                                                heatingCoil,
                                                coolingCoil,
                                                supplementalHeatingCoil );

  return pthp;
}

void addSystemType2(Model & model, std::vector<ThermalZone> zones)
{
  for( auto & zone : zones )
  {
    if( zone.model() == model )
    {
      ZoneHVACPackagedTerminalHeatPump pthp = addSystemType2(model);

      pthp.addToThermalZone(zone);
    }
  }
}

Loop addSystemType3(Model & model)
{
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
  sizingSystem.setMinimumSystemAirFlowRatio(1.0);
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

  FanConstantVolume fan = FanConstantVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingGas coilHeatingGas = CoilHeatingGas(model,_alwaysOnSchedule);

  CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coilCooling = CoilCoolingDXSingleSpeed( model,
                                                                   _alwaysOnSchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction );

  SetpointManagerSingleZoneReheat setpointMSZR(model);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  outdoorAirSystem.addToNode(supplyOutletNode);
  coilCooling.addToNode(supplyOutletNode);
  coilHeatingGas.addToNode(supplyOutletNode);
  fan.addToNode(supplyOutletNode);

  Node node1 = fan.outletModelObject()->cast<Node>();
  setpointMSZR.addToNode(node1);

  AirTerminalSingleDuctUncontrolled terminal(model,_alwaysOnSchedule); 

  airLoopHVAC.addBranchForHVACComponent(terminal);

  return airLoopHVAC;
}

Loop addSystemType4(Model & model)
{
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
  sizingSystem.setMinimumSystemAirFlowRatio(1.0);
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

  FanConstantVolume supplyFan(model,_alwaysOnSchedule);
  supplyFan.setPressureRise(500);

  CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.766956);
  coolingCurveFofTemp.setCoefficient2x(0.0107756);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.0000414703);
  coolingCurveFofTemp.setCoefficient4y(0.00134961);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.000261144);
  coolingCurveFofTemp.setCoefficient6xTIMESY(0.000457488);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.8);
  coolingCurveFofFlow.setCoefficient2x(0.2);
  coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
  coolingCurveFofFlow.setMinimumValueofx(0.5);
  coolingCurveFofFlow.setMaximumValueofx(1.5);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(0.297145);
  energyInputRatioFofTemp.setCoefficient2x(0.0430933);
  energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000748766);
  energyInputRatioFofTemp.setCoefficient4y(0.00597727);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.000482112);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000956448);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.156);
  energyInputRatioFofFlow.setCoefficient2x(-0.1816);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.0256);
  energyInputRatioFofFlow.setMinimumValueofx(0.5);
  energyInputRatioFofFlow.setMaximumValueofx(1.5);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.75);
  partLoadFraction.setCoefficient2x(0.25);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CoilCoolingDXSingleSpeed coolingCoil = CoilCoolingDXSingleSpeed( model,
                                                                   _alwaysOnSchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction );

  CurveCubic  totalHeatingCapacityFunctionofTemperatureCurve(model);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
  totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveCubic  totalHeatingCapacityFunctionofFlowFractionCurve(model);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

  CurveCubic  energyInputRatioFunctionofTemperatureCurve(model);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
  energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveQuadratic  energyInputRatioFunctionofFlowFractionCurve(model);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
  energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
  energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

  CurveQuadratic  partLoadFractionCorrelationCurve(model);
  partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
  partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
  partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
  partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

  CoilHeatingDXSingleSpeed heatingCoil( model,
                                        _alwaysOnSchedule,
                                        totalHeatingCapacityFunctionofTemperatureCurve,
                                        totalHeatingCapacityFunctionofFlowFractionCurve,
                                        energyInputRatioFunctionofTemperatureCurve,
                                        energyInputRatioFunctionofFlowFractionCurve,
                                        partLoadFractionCorrelationCurve ); 

  CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model,_alwaysOnSchedule);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);
  coolingCoil.addToNode(supplyOutletNode);
  heatingCoil.addToNode(supplyOutletNode);
  coilHeatingElectric.addToNode(supplyOutletNode);
  supplyFan.addToNode(supplyOutletNode);
  
  AirTerminalSingleDuctUncontrolled terminal(model,_alwaysOnSchedule); 

  airLoopHVAC.addBranchForHVACComponent(terminal);

  SetpointManagerSingleZoneReheat setpointMSZR(model);

  Node node1 = supplyFan.outletModelObject()->cast<Node>();
  setpointMSZR.addToNode(node1);

  return airLoopHVAC;
}

Loop addSystemType5(Model & model)
{
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

  FanVariableVolume fan = FanVariableVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingWater coilHeatingWater = CoilHeatingWater(model,_alwaysOnSchedule);

  CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CurveBiquadratic coolingLowSpdCurveFofTemp = CurveBiquadratic(model);
  coolingLowSpdCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingLowSpdCurveFofTemp.setCoefficient2x(0.04426);
  coolingLowSpdCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingLowSpdCurveFofTemp.setCoefficient4y(0.00333);
  coolingLowSpdCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingLowSpdCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingLowSpdCurveFofTemp.setMinimumValueofx(17.0);
  coolingLowSpdCurveFofTemp.setMaximumValueofx(22.0);
  coolingLowSpdCurveFofTemp.setMinimumValueofy(13.0);
  coolingLowSpdCurveFofTemp.setMaximumValueofy(46.0);

  CurveBiquadratic energyLowSpdInputRatioFofTemp = CurveBiquadratic(model);
  energyLowSpdInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyLowSpdInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyLowSpdInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyLowSpdInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyLowSpdInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyLowSpdInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyLowSpdInputRatioFofTemp.setMinimumValueofx(17.0);
  energyLowSpdInputRatioFofTemp.setMaximumValueofx(22.0);
  energyLowSpdInputRatioFofTemp.setMinimumValueofy(13.0);
  energyLowSpdInputRatioFofTemp.setMaximumValueofy(46.0);

  CoilCoolingDXTwoSpeed coilCooling = CoilCoolingDXTwoSpeed( model,
                                                                   _alwaysOnSchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction, 
                                                                   coolingLowSpdCurveFofTemp,
                                                                   energyLowSpdInputRatioFofTemp);

  coilCooling.setRatedLowSpeedSensibleHeatRatio(0.69);
  coilCooling.setBasinHeaterCapacity(10);
  coilCooling.setBasinHeaterSetpointTemperature(2.0);

  SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model,_deckTempSchedule);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

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

  SetpointManagerScheduled hotWaterSPM(model,_hotWaterSchedule);

  hotWaterSPM.addToNode(hotWaterOutletNode);

  // Terminal

  CoilHeatingWater waterReheatCoil(model,_alwaysOnSchedule);
  AirTerminalSingleDuctVAVReheat waterTerminal(model,_alwaysOnSchedule,waterReheatCoil);
  airLoopHVAC.addBranchForHVACComponent(waterTerminal);

  hotWaterPlant.addDemandBranchForComponent(waterReheatCoil);

  return airLoopHVAC;
}

Loop addSystemType6(Model & model)
{
  Model tempModel;
  Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

  Schedule _deckTempSchedule = deckTempSchedule(tempModel).clone(model).cast<Schedule>();

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  airLoopHVAC.setName("Packaged Rooftop VAV with PFP Boxes and Reheat");
  FanVariableVolume fan = FanVariableVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model,_alwaysOnSchedule);

  CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CurveBiquadratic coolingLowSpdCurveFofTemp = CurveBiquadratic(model);
  coolingLowSpdCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingLowSpdCurveFofTemp.setCoefficient2x(0.04426);
  coolingLowSpdCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingLowSpdCurveFofTemp.setCoefficient4y(0.00333);
  coolingLowSpdCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingLowSpdCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingLowSpdCurveFofTemp.setMinimumValueofx(17.0);
  coolingLowSpdCurveFofTemp.setMaximumValueofx(22.0);
  coolingLowSpdCurveFofTemp.setMinimumValueofy(13.0);
  coolingLowSpdCurveFofTemp.setMaximumValueofy(46.0);

  CurveBiquadratic energyLowSpdInputRatioFofTemp = CurveBiquadratic(model);
  energyLowSpdInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyLowSpdInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyLowSpdInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyLowSpdInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyLowSpdInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyLowSpdInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyLowSpdInputRatioFofTemp.setMinimumValueofx(17.0);
  energyLowSpdInputRatioFofTemp.setMaximumValueofx(22.0);
  energyLowSpdInputRatioFofTemp.setMinimumValueofy(13.0);
  energyLowSpdInputRatioFofTemp.setMaximumValueofy(46.0);

  CoilCoolingDXTwoSpeed coilCooling = CoilCoolingDXTwoSpeed( model,
                                                                   _alwaysOnSchedule,
                                                                   coolingCurveFofTemp,
                                                                   coolingCurveFofFlow,
                                                                   energyInputRatioFofTemp,
                                                                   energyInputRatioFofFlow,
                                                                   partLoadFraction, 
                                                                   coolingLowSpdCurveFofTemp,
                                                                   energyLowSpdInputRatioFofTemp);

  coilCooling.setRatedLowSpeedSensibleHeatRatio(0.69);
  coilCooling.setBasinHeaterCapacity(10);
  coilCooling.setBasinHeaterSetpointTemperature(2.0);

  SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model,_deckTempSchedule);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  outdoorAirSystem.addToNode(supplyOutletNode);
  coilCooling.addToNode(supplyOutletNode);
  fan.addToNode(supplyOutletNode);
  Node fanInletNode = fan.inletModelObject()->cast<Node>();
  coilHeatingElectric.addToNode(fanInletNode);

  Node node1 = fan.outletModelObject()->cast<Node>();
  deckTempSPM.addToNode(node1);

  CoilHeatingElectric reheatCoil(model,_alwaysOnSchedule);
  FanConstantVolume piuFan(model,_alwaysOnSchedule);
  piuFan.setPressureRise(300);
  AirTerminalSingleDuctParallelPIUReheat terminal(model,_alwaysOnSchedule,piuFan,reheatCoil);
  airLoopHVAC.addBranchForHVACComponent(terminal);

  return airLoopHVAC;
}

Loop addSystemType7(Model & model)
{
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

  FanVariableVolume fan = FanVariableVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingWater coilHeatingWater = CoilHeatingWater(model,_alwaysOnSchedule);

  CoilCoolingWater coilCoolingWater = CoilCoolingWater(model,_alwaysOnSchedule);

  SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model,_deckTempSchedule);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

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

  SetpointManagerScheduled hotWaterSPM(model,_hotWaterSchedule);

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
  ccFofT.setCoefficient1Constant(1.0215158);
  ccFofT.setCoefficient2x(0.037035864);
  ccFofT.setCoefficient3xPOW2(0.0002332476);
  ccFofT.setCoefficient4y(-0.003894048);
  ccFofT.setCoefficient5yPOW2(-6.52536e-005);
  ccFofT.setCoefficient6xTIMESY(-0.0002680452);
  ccFofT.setMinimumValueofx(5.0);
  ccFofT.setMaximumValueofx(10.0);
  ccFofT.setMinimumValueofy(24.0);
  ccFofT.setMaximumValueofy(35.0);

  CurveBiquadratic eirToCorfOfT(model);
  eirToCorfOfT.setCoefficient1Constant(0.70176857);
  eirToCorfOfT.setCoefficient2x(-0.00452016);
  eirToCorfOfT.setCoefficient3xPOW2(0.0005331096);
  eirToCorfOfT.setCoefficient4y(-0.005498208);
  eirToCorfOfT.setCoefficient5yPOW2(0.0005445792);
  eirToCorfOfT.setCoefficient6xTIMESY(-0.0007290324);
  eirToCorfOfT.setMinimumValueofx(5.0);
  eirToCorfOfT.setMaximumValueofx(10.0);
  eirToCorfOfT.setMinimumValueofy(24.0);
  eirToCorfOfT.setMaximumValueofy(35.0);

  CurveQuadratic eirToCorfOfPlr(model);
  eirToCorfOfPlr.setCoefficient1Constant(0.06369119);
  eirToCorfOfPlr.setCoefficient2x(0.58488832);
  eirToCorfOfPlr.setCoefficient3xPOW2(0.35280274);
  eirToCorfOfPlr.setMinimumValueofx(0.0);
  eirToCorfOfPlr.setMaximumValueofx(1.0);

  ChillerElectricEIR chiller(model,ccFofT,eirToCorfOfT,eirToCorfOfPlr);

  node = chilledWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
  chiller.addToNode(node);

  PipeAdiabatic pipe3(model);
  chilledWaterPlant.addSupplyBranchForComponent(pipe3);
  
  PipeAdiabatic pipe4(model);
  pipe4.addToNode(chilledWaterOutletNode);

  chilledWaterPlant.addDemandBranchForComponent(coilCoolingWater);

  SetpointManagerScheduled chilledWaterSPM(model,_chilledWaterSchedule);

  chilledWaterSPM.addToNode(chilledWaterOutletNode);

  CoilHeatingWater waterReheatCoil(model,_alwaysOnSchedule);
  AirTerminalSingleDuctVAVReheat waterTerminal(model,_alwaysOnSchedule,waterReheatCoil);
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

Loop addSystemType8(Model & model)
{
  Model tempModel;
  Schedule _alwaysOnSchedule = model.alwaysOnDiscreteSchedule();

  Schedule _chilledWaterSchedule = chilledWaterTempSchedule(tempModel).clone(model).cast<Schedule>();

  Schedule _deckTempSchedule = deckTempSchedule(tempModel).clone(model).cast<Schedule>();

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  airLoopHVAC.setName("VAV with PFP Boxes and Reheat");
  FanVariableVolume fan = FanVariableVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model,_alwaysOnSchedule);

  CoilCoolingWater coilCoolingWater = CoilCoolingWater(model,_alwaysOnSchedule);

  SetpointManagerScheduled deckTempSPM = SetpointManagerScheduled(model,_deckTempSchedule);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

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

  CurveBiquadratic ccFofT(model);
  ccFofT.setCoefficient1Constant(1.0215158);
  ccFofT.setCoefficient2x(0.037035864);
  ccFofT.setCoefficient3xPOW2(0.0002332476);
  ccFofT.setCoefficient4y(-0.003894048);
  ccFofT.setCoefficient5yPOW2(-6.52536e-005);
  ccFofT.setCoefficient6xTIMESY(-0.0002680452);
  ccFofT.setMinimumValueofx(5.0);
  ccFofT.setMaximumValueofx(10.0);
  ccFofT.setMinimumValueofy(24.0);
  ccFofT.setMaximumValueofy(35.0);

  CurveBiquadratic eirToCorfOfT(model);
  eirToCorfOfT.setCoefficient1Constant(0.70176857);
  eirToCorfOfT.setCoefficient2x(-0.00452016);
  eirToCorfOfT.setCoefficient3xPOW2(0.0005331096);
  eirToCorfOfT.setCoefficient4y(-0.005498208);
  eirToCorfOfT.setCoefficient5yPOW2(0.0005445792);
  eirToCorfOfT.setCoefficient6xTIMESY(-0.0007290324);
  eirToCorfOfT.setMinimumValueofx(5.0);
  eirToCorfOfT.setMaximumValueofx(10.0);
  eirToCorfOfT.setMinimumValueofy(24.0);
  eirToCorfOfT.setMaximumValueofy(35.0);

  CurveQuadratic eirToCorfOfPlr(model);
  eirToCorfOfPlr.setCoefficient1Constant(0.06369119);
  eirToCorfOfPlr.setCoefficient2x(0.58488832);
  eirToCorfOfPlr.setCoefficient3xPOW2(0.35280274);
  eirToCorfOfPlr.setMinimumValueofx(0.0);
  eirToCorfOfPlr.setMaximumValueofx(1.0);

  ChillerElectricEIR chiller(model,ccFofT,eirToCorfOfT,eirToCorfOfPlr);

  Node node = chilledWaterPlant.supplySplitter().lastOutletModelObject()->cast<Node>();
  chiller.addToNode(node);

  chilledWaterPlant.addDemandBranchForComponent(coilCoolingWater);

  PipeAdiabatic pipe(model);
  chilledWaterPlant.addSupplyBranchForComponent(pipe);

  PipeAdiabatic pipe2(model);
  pipe2.addToNode(chilledWaterOutletNode);

  SetpointManagerScheduled chilledWaterSPM(model,_chilledWaterSchedule);

  chilledWaterSPM.addToNode(chilledWaterOutletNode);

  CoilHeatingElectric reheatCoil(model,_alwaysOnSchedule);
  FanConstantVolume piuFan(model,_alwaysOnSchedule);
  piuFan.setPressureRise(300);
  AirTerminalSingleDuctParallelPIUReheat terminal(model,_alwaysOnSchedule,piuFan,reheatCoil);
  airLoopHVAC.addBranchForHVACComponent(terminal);

  return airLoopHVAC;
}

Loop addSystemType9(Model & model)
{
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
  sizingSystem.setMinimumSystemAirFlowRatio(1.0);
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

  FanConstantVolume fan = FanConstantVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingGas coilHeatingGas = CoilHeatingGas(model,_alwaysOnSchedule);

  SetpointManagerSingleZoneReheat setpointMSZR(model);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  outdoorAirSystem.addToNode(supplyOutletNode);
  coilHeatingGas.addToNode(supplyOutletNode);
  fan.addToNode(supplyOutletNode);

  Node node1 = fan.outletModelObject()->cast<Node>();
  setpointMSZR.addToNode(node1);

  AirTerminalSingleDuctUncontrolled terminal(model,_alwaysOnSchedule); 

  airLoopHVAC.addBranchForHVACComponent(terminal);

  return airLoopHVAC;
}

Loop addSystemType10(Model & model)
{
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
  sizingSystem.setMinimumSystemAirFlowRatio(1.0);
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

  FanConstantVolume fan = FanConstantVolume(model,_alwaysOnSchedule);
  fan.setPressureRise(500);

  CoilHeatingElectric coilHeatingElectric = CoilHeatingElectric(model,_alwaysOnSchedule);

  SetpointManagerSingleZoneReheat setpointMSZR(model);

  ControllerOutdoorAir controllerOutdoorAir = ControllerOutdoorAir(model);

  AirLoopHVACOutdoorAirSystem outdoorAirSystem = AirLoopHVACOutdoorAirSystem(model,controllerOutdoorAir);

  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  outdoorAirSystem.addToNode(supplyOutletNode);
  coilHeatingElectric.addToNode(supplyOutletNode);
  fan.addToNode(supplyOutletNode);

  Node node1 = fan.outletModelObject()->cast<Node>();
  setpointMSZR.addToNode(node1);

  AirTerminalSingleDuctUncontrolled terminal(model,_alwaysOnSchedule); 

  airLoopHVAC.addBranchForHVACComponent(terminal);

  return airLoopHVAC;
}

} // model

} // openstudio

