/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <sdd/ReverseTranslator.hpp>
#include <sdd/ForwardTranslator.hpp>

#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/ControllerMechanicalVentilation.hpp>
#include <model/ControllerMechanicalVentilation_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanVariableVolume.hpp>
#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXTwoSpeed.hpp>
#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingDXSingleSpeed.hpp>
#include <model/ControllerWaterCoil.hpp>
#include <model/ControllerWaterCoil_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/ScheduleRuleset.hpp>
#include <model/ScheduleDay.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CoolingTowerSingleSpeed.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerWarmest.hpp>
#include <model/SetpointManagerOutdoorAirReset.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/ThermostatSetpointDualSetpoint.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/FanVariableVolume.hpp>
#include <model/FanVariableVolume_Impl.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/PumpConstantSpeed.hpp>
#include <model/PumpConstantSpeed_Impl.hpp>
#include <model/PumpVariableSpeed.hpp>
#include <model/PumpVariableSpeed_Impl.hpp>
#include <model/BoilerHotWater.hpp>
#include <model/BoilerHotWater_Impl.hpp>
#include <model/ChillerElectricEIR.hpp>
#include <model/ChillerElectricEIR_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <model/CoilCoolingWater.hpp>
#include <model/CoilCoolingWater_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACFourPipeFanCoil_Impl.hpp>
#include <model/ZoneHVACBaseboardConvectiveElectric.hpp>
#include <model/ZoneHVACBaseboardConvectiveElectric_Impl.hpp>
#include <model/ZoneHVACBaseboardConvectiveWater.hpp>
#include <model/ZoneHVACBaseboardConvectiveWater_Impl.hpp>
#include <model/CoilHeatingWaterBaseboard.hpp>
#include <model/CoilHeatingWaterBaseboard_Impl.hpp>
#include <model/WaterHeaterMixed.hpp>
#include <model/WaterHeaterMixed_Impl.hpp>
#include <model/Model.hpp>
#include <model/PortList.hpp>
#include <model/SizingPlant.hpp>
#include <model/SizingPlant_Impl.hpp>
#include <model/SizingSystem.hpp>
#include <model/SizingSystem_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/PipeAdiabatic.hpp>
#include <model/PipeAdiabatic_Impl.hpp>
#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>
#include <model/Mixer.hpp>
#include <model/Mixer_Impl.hpp>

#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/IPUnit.hpp>
#include <utilities/units/SIUnit.hpp>
#include <utilities/units/BTUUnit.hpp>
#include <utilities/units/CFMUnit.hpp>
#include <utilities/units/FahrenheitUnit.hpp>
#include <utilities/units/MPHUnit.hpp>
#include <utilities/units/WhUnit.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/units/UnitFactory.hpp>
#include <utilities/math/FloatCompare.hpp>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QStringList>
#include <cmath>

namespace openstudio {
namespace sdd {

const double cpWater = 4180.0;
const double densityWater = 1000.0;

model::Schedule ReverseTranslator::defaultDeckTempSchedule(openstudio::model::Model& model)
{
  if( ! m_defaultDeckTempSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Deck Temp");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),12.8);

    m_defaultDeckTempSchedule = scheduleRuleset;
  }

  return m_defaultDeckTempSchedule.get();
}

model::Schedule ReverseTranslator::alwaysOnSchedule(openstudio::model::Model& model)
{
  if( ! m_alwaysOnSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Always On");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),1.0);

    m_alwaysOnSchedule = scheduleRuleset;
  }

  return m_alwaysOnSchedule.get();
}

model::Schedule ReverseTranslator::hotWaterPlantSetpointSchedule(openstudio::model::Model& model)
{
  if( ! m_hotWaterPlantSetpointSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Hot Water Plant Setpoint");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),67.0);

    m_hotWaterPlantSetpointSchedule = scheduleRuleset;
  }

  return m_hotWaterPlantSetpointSchedule.get();
}

model::Schedule ReverseTranslator::chilledWaterPlantSetpointSchedule(openstudio::model::Model& model)
{
  if( ! m_chilledWaterPlantSetpointSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Chilled Water Plant Setpoint");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),6.7);

    m_chilledWaterPlantSetpointSchedule = scheduleRuleset;
  }

  return m_chilledWaterPlantSetpointSchedule.get();
}

model::Schedule ReverseTranslator::serviceHotWaterSetpointSchedule(openstudio::model::Model& model)
{
  if( ! m_serviceHotWaterSetpointSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("SHW Plant Setpoint");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),50.0);

    m_serviceHotWaterSetpointSchedule = scheduleRuleset;
  }

  return m_serviceHotWaterSetpointSchedule.get();
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateAirSystem(
                                                  const QDomElement& airSystemElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  double value;

  if( ! istringEqual(airSystemElement.tagName().toStdString(),"AirSys") )
  {
    return result;
  }

  // Name
  QDomElement nameElement = airSystemElement.firstChildElement("Name");

  // Air System
  model::AirLoopHVAC airLoopHVAC(model);
  airLoopHVAC.setName(nameElement.text().toStdString());
  result = airLoopHVAC;
  model::Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  model::Node supplyInletNode = airLoopHVAC.supplyInletNode();

  QDomElement airSystemTypeElement = airSystemElement.firstChildElement("Type");

  QDomElement airSystemClgCtrlElement = airSystemElement.firstChildElement("ClgCtrl");

  QDomElement airHndlrAvailSchElement = airSystemElement.firstChildElement("AvailSchRef");

  // Availability Schedule
  boost::optional<model::Schedule> availabilitySchedule; 
  if( ! airHndlrAvailSchElement.isNull() )
  {
      availabilitySchedule = model.getModelObjectByName<model::Schedule>(airHndlrAvailSchElement.text().toStdString());
  }

  if( availabilitySchedule )
  {
    airLoopHVAC.setAvailabilitySchedule(availabilitySchedule.get());
  }

  QDomElement nightCycleFanCtrlElement = airSystemElement.firstChildElement("NightCycleFanCtrl");
  
  if( istringEqual(nightCycleFanCtrlElement.text().toStdString(),"CycleOnCallAnyZone") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAny");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().toStdString(),"CycleOnCallPrimaryZone") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAny");

    LOG(Warn,airLoopHVAC.name().get() << ": CycleOnAnyZoneFansOnly is not supported, using CycleOnAny.");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().toStdString(),"CycleZoneFansOnly") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAnyZoneFansOnly");
  }


  // Air Segments
  QDomNodeList airSegmentElements = airSystemElement.elementsByTagName("AirSeg");

  for (int i = 0; i < airSegmentElements.count(); i++)
  {
    QDomElement airSegmentElement = airSegmentElements.at(i).toElement();

    QDomElement airSegmentTypeElement = airSegmentElement.firstChildElement("Type");

    // Supply Segments
    if(istringEqual(airSegmentTypeElement.text().toStdString(),"Supply"))
    {
      QDomNodeList airSegmentChildElements = airSegmentElement.childNodes();

      for (int j = airSegmentChildElements.count() - 1; j >= 0 ; j--)
      {
        QDomElement airSegmentChildElement = airSegmentChildElements.at(j).toElement();
        
        // CoilCooling
        if( istringEqual(airSegmentChildElement.tagName().toStdString(),"CoilClg") )
        {
          if( boost::optional<model::ModelObject> mo = 
                translateCoilCooling(airSegmentChildElement,doc,model) )
          {
            OS_ASSERT(mo);

            model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();

            hvacComponent.addToNode(supplyInletNode);

            if( ! autosize() )
            {
              if( boost::optional<model::CoilCoolingWater> coilCoolingWater = hvacComponent.optionalCast<model::CoilCoolingWater>() )
              {
                boost::optional<model::ControllerWaterCoil> controller = coilCoolingWater->controllerWaterCoil();

                OS_ASSERT(controller);

                controller->setControllerConvergenceTolerance(0.1);

                boost::optional<double> maxFlow = coilCoolingWater->designWaterFlowRate();

                OS_ASSERT(maxFlow);

                controller->setMaximumActuatedFlow(maxFlow.get() * 1.25);
              }
            }
          }
        }
        // CoilHeating
        else if( istringEqual(airSegmentChildElement.tagName().toStdString(),"CoilHtg") &&
                 ! ( istringEqual(airSystemTypeElement.text().toStdString(),"SZVAVAC") ||
                     istringEqual(airSystemTypeElement.text().toStdString(),"SZVAVHP") 
                   )
               )
        {
          boost::optional<model::ModelObject> mo = translateCoilHeating(airSegmentChildElement,doc,model);

          OS_ASSERT(mo);

          model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();

          hvacComponent.addToNode(supplyInletNode);

          if( boost::optional<model::CoilHeatingWater> coilHeatingWater = hvacComponent.optionalCast<model::CoilHeatingWater>() )
          {
            boost::optional<model::ControllerWaterCoil> controller = coilHeatingWater->controllerWaterCoil();

            OS_ASSERT(controller);

            controller->setControllerConvergenceTolerance(0.1);

            if( ! autosize() )
            {
              boost::optional<double> capacity = coilHeatingWater->ratedCapacity();

              boost::optional<double> inletTemp = coilHeatingWater->ratedInletWaterTemperature();

              boost::optional<double> outletTemp = coilHeatingWater->ratedOutletWaterTemperature();

              OS_ASSERT(capacity);

              OS_ASSERT(inletTemp);

              OS_ASSERT(outletTemp);

              double density = 1000.0;

              double cp = 4180.0;

              double maxFlow = capacity.get() / ( density * cp * std::abs(inletTemp.get() - outletTemp.get()));

              controller->setMaximumActuatedFlow(maxFlow * 1.25);

              controller->setMinimumActuatedFlow(maxFlow * 1.25 * 0.05);
            }
            else
            {
              controller->setMinimumActuatedFlow(0.000001);
            }
          }
        }
        // Fan
        else if( istringEqual(airSegmentChildElement.tagName().toStdString(),"Fan") )
        {
          if( boost::optional<model::ModelObject> mo = 
                translateFan(airSegmentChildElement,doc,model) )
          {
            if( boost::optional<model::HVACComponent> hvacComponent = 
                  mo->optionalCast<model::HVACComponent>() )
            {
              hvacComponent->addToNode(supplyInletNode);

              if( availabilitySchedule )
              {
                if( boost::optional<model::FanConstantVolume> fan = hvacComponent->optionalCast<model::FanConstantVolume>() )
                {
                  if( ! fan->isMaximumFlowRateAutosized() )
                  {
                    airLoopHVAC.setDesignSupplyAirFlowRate(fan->maximumFlowRate().get());
                  }
                }
                else if( boost::optional<model::FanVariableVolume> fan = hvacComponent->optionalCast<model::FanVariableVolume>() )
                {
                  if( ! fan->isMaximumFlowRateAutosized() )
                  {
                    airLoopHVAC.setDesignSupplyAirFlowRate(fan->maximumFlowRate().get());
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // OACtrl
  QDomElement airSystemOACtrlElement = airSystemElement.firstChildElement("OACtrl");

  if( ! airSystemOACtrlElement.isNull() )
  {
    boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem();
    if( ! oaSystem )
    {
      model::ControllerOutdoorAir oaController(model);

      model::AirLoopHVACOutdoorAirSystem newOASystem(model,oaController);

      newOASystem.addToNode(supplyInletNode);

      // MaxOARat
      QDomElement maxOARatElement = airSystemOACtrlElement.firstChildElement("MaxOARat");
      bool ok;
      double maxOARat = maxOARatElement.text().toDouble(&ok);
      if( ok && maxOARat > 0.0 && maxOARat <= 1.0 )
      {
        model::ScheduleRuleset maxOARatSchedule(model);
        maxOARatSchedule.setName(oaController.name().get() + " Max OA Schedule");
        model::ScheduleDay scheduleDay = maxOARatSchedule.defaultDaySchedule();
        scheduleDay.addValue(Time(1.0),maxOARat);
        oaController.setMaximumFractionofOutdoorAirSchedule(maxOARatSchedule);
      }

      // EconoCtrlMthd
      QDomElement econoCtrlMthdElement = airSystemOACtrlElement.firstChildElement("EconoCtrlMthd");

      if(istringEqual(econoCtrlMthdElement.text().toStdString(),"FixedDryBulb"))
      {
        oaController.setEconomizerControlType("FixedDryBulb");
      }
      else if(istringEqual(econoCtrlMthdElement.text().toStdString(),"DifferentialDryBulb"))
      {
        oaController.setEconomizerControlType("DifferentialDryBulb");
      }
      else if(istringEqual(econoCtrlMthdElement.text().toStdString(),"DifferentialEnthalpy"))
      {
        oaController.setEconomizerControlType("DifferentialEnthalpy");
      }
      else if(istringEqual(econoCtrlMthdElement.text().toStdString(),"DifferentialDryBulbAndEnthalpy"))
      {
        oaController.setEconomizerControlType("DifferentialEnthalpy");
      }

      // EconoIntegration
      QDomElement econoIntegrationElement = airSystemOACtrlElement.firstChildElement("EconoIntegration");
      if( istringEqual(econoIntegrationElement.text().toStdString(),"Integrated") )
      {
        oaController.setLockoutType("NoLockout");
      }
      else if( istringEqual(econoIntegrationElement.text().toStdString(),"NonIntegrated") )
      {
        oaController.setLockoutType("LockoutWithCompressor");
      }

      // EconoHiTempLockout
      QDomElement econoHiTempLockoutElement = airSystemOACtrlElement.firstChildElement("EconoHiTempLockout");

      value = econoHiTempLockoutElement.text().toDouble(&ok);

      if( ok ) 
      {
        Quantity valueIP(value,createFahrenheitTemperature());
        OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::Celsius));
        OS_ASSERT(valueSI);
        oaController.setEconomizerMaximumLimitDryBulbTemperature(valueSI->value());
      }

      // EconolowTempLockout
      QDomElement econoLowTempLockoutElement = airSystemOACtrlElement.firstChildElement("EconoLowTempLockout");

      value = econoLowTempLockoutElement.text().toDouble(&ok);

      if( ok ) 
      {
        Quantity valueIP(value,createFahrenheitTemperature());
        OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::Celsius));
        OS_ASSERT(valueSI);
        oaController.setEconomizerMinimumLimitDryBulbTemperature(valueSI->value());
      }
    }
  }

  // Add Setpoint managers
  std::vector<model::ModelObject> supplyNodes = airLoopHVAC.supplyComponents(IddObjectType::OS_Node);

  // Mixed air setpoint managers
  for( std::vector<model::ModelObject>::iterator it = supplyNodes.begin();
       it != supplyNodes.end();
       it++ )
  {
    if( *it != airLoopHVAC.supplyInletNode() &&
        *it != airLoopHVAC.supplyOutletNode() )
    {
      model::Node node = it->cast<model::Node>();

      model::SetpointManagerMixedAir spm(model);

      node.addSetpointManager(spm);
    }
  }

  QDomElement clgCtrlElement = airSystemElement.firstChildElement("ClgCtrl");

  QDomElement clgSupAirTempElement = airSystemElement.firstChildElement("ClgSupAirTemp");

  QDomElement htgSupAirTempElement = airSystemElement.firstChildElement("HtgSupAirTemp");

  bool ok;

  if( istringEqual(clgCtrlElement.text().toStdString(),"Fixed") )
  {
    if( istringEqual(airSystemTypeElement.text().toStdString(),"SZAC") ||
        istringEqual(airSystemTypeElement.text().toStdString(),"SZHP")
      )
    {
      model::SetpointManagerSingleZoneReheat spm(model);

      supplyOutletNode.addSetpointManager(spm);

      value = clgSupAirTempElement.text().toDouble(&ok);

      if( ok )
      {
        airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(unitToUnit(value,"F","C").get());
      }
      else
      {
        airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(12.8);
      }

      value = htgSupAirTempElement.text().toDouble(&ok);

      if( ok )
      {
        airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(unitToUnit(value,"F","C").get());
      }
      else
      {
        airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(40.0);
      }
    }
    else
    {
      model::ScheduleRuleset schedule(model);

      schedule.setName(airLoopHVAC.name().get() + " Supply Air Temp Schedule");

      value = clgSupAirTempElement.text().toDouble(&ok);

      if( ok )
      {
        model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

        value = unitToUnit(value,"F","C").get();

        scheduleDay.addValue(Time(1.0),value);

        airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(value);

        airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(value);
      }
      else
      {
        model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

        scheduleDay.addValue(Time(1.0),12.8);

        airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(12.8);

        airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(12.8);
      }

      model::SetpointManagerScheduled spm(model,schedule);

      supplyOutletNode.addSetpointManager(spm);
    }
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"WarmestReset") )
  {
    model::SetpointManagerWarmest spm(model);

    supplyOutletNode.addSetpointManagerWarmest(spm);

    QDomElement clRstSupHiElement = airSystemElement.firstChildElement("ClRstSupHiElement");

    value = clRstSupHiElement.text().toDouble(&ok);

    if( ok )
    {
      value = unitToUnit(value,"F","C").get();

      spm.setMaximumSetpointTemperature(value);

      airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(value);
    }
    else
    {
      value = 15.6;

      spm.setMaximumSetpointTemperature(value);

      airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(value);
    }

    QDomElement clRstSupLowElement = airSystemElement.firstChildElement("ClRstSupLowElement");

    value = clRstSupLowElement.text().toDouble(&ok);

    if( ok )
    {
      value = unitToUnit(value,"F","C").get();

      spm.setMinimumSetpointTemperature(value);

      airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(value);
    }
    else
    {
      value = 12.2;

      spm.setMinimumSetpointTemperature(value);

      airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(value);
    }
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"Scheduled") )
  {
    QDomElement clgSetPtSchRefElement = airSystemElement.firstChildElement("ClgSetPtSchRef");

    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(clgSetPtSchRefElement.text().toStdString());

    if( ! schedule )
    {
      model::ScheduleRuleset schedule(model);

      schedule.setName(airLoopHVAC.name().get() + " Supply Air Temp Schedule");

      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),12.8);
    }

    model::SetpointManagerScheduled spm(model,schedule.get());

    supplyOutletNode.addSetpointManager(spm);

    airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(12.8);

    airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(12.8);
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"OutsideAirReset") )
  {
    model::SetpointManagerOutdoorAirReset spm(model);

    supplyOutletNode.addSetpointManager(spm);

    QDomElement clRstSupHiElement = airSystemElement.firstChildElement("ClRstSupHiElement");

    value = clRstSupHiElement.text().toDouble(&ok);

    if( ok )
    {
      value = unitToUnit(value,"F","C").get();

      spm.setSetpointatOutdoorHighTemperature(value);

      airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(value);
    }
    else
    {
      value = 10.0;

      spm.setSetpointatOutdoorHighTemperature(value);

      airLoopHVAC.sizingSystem().setCentralCoolingDesignSupplyAirTemperature(value);
    }

    QDomElement clRstSupLowElement = airSystemElement.firstChildElement("ClRstSupLowElement");

    value = clRstSupLowElement.text().toDouble(&ok);

    if( ok )
    {
      value = unitToUnit(value,"F","C").get();

      spm.setSetpointatOutdoorLowTemperature(value);

      airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(value);
    }
    else
    {
      value = 22.0;

      spm.setSetpointatOutdoorLowTemperature(value);

      airLoopHVAC.sizingSystem().setCentralHeatingDesignSupplyAirTemperature(value);
    }

    QDomElement clRstOutdrHiElement = airSystemElement.firstChildElement("ClRstOutdrHiElement");

    value = clRstOutdrHiElement.text().toDouble(&ok);

    if( ok )
    {
      spm.setOutdoorHighTemperature(unitToUnit(value,"F","C").get());
    }

    QDomElement clRstOutdrLowElement = airSystemElement.firstChildElement("ClRstOutdrLowElement");

    value = clRstOutdrLowElement.text().toDouble(&ok);

    if( ok )
    {
      spm.setOutdoorLowTemperature(unitToUnit(value,"F","C").get());
    }
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilHeating(
  const QDomElement& heatingCoilElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(heatingCoilElement.tagName().toStdString(),"CoilHtg") )
  {
    return result;
  }

  // Type
  QDomElement coilHeatingTypeElement = heatingCoilElement.firstChildElement("Type");
  QDomElement nameElement = heatingCoilElement.firstChildElement("Name");
  QDomElement capTotRtdElement = heatingCoilElement.firstChildElement("CapTotGrossRtd");

  // CapTotGrossRtd
  boost::optional<double> capTotGrossRtd;
  
  if( ! autosize() )
  {
    value = capTotRtdElement.text().toDouble(&ok);

    if( ok ) 
    {
      Quantity valueIP(value,createBTUPower());
      OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(valueSI);
      capTotGrossRtd = valueSI->value();
    }
  }

  // Furnace
  if( istringEqual(coilHeatingTypeElement.text().toStdString(),"Furnace") )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingGas coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    // FurnHIR_fPLRCrvRef
    boost::optional<model::Curve> hirCurve;
    QDomElement hirCurveElement = 
      heatingCoilElement.firstChildElement("FurnHIR_fPLRCrvRef");
    hirCurve = 
      model.getModelObjectByName<model::Curve>(
        hirCurveElement.text().toStdString());
    if( hirCurve )
    {
      coil.setPartLoadFractionCorrelationCurve(hirCurve.get());
    }

    // FurnThrmlEff
    QDomElement furnThrmlEffElement = heatingCoilElement.firstChildElement("FurnThrmlEff");
    value = furnThrmlEffElement.text().toDouble(&ok);
    if( ok )
    {
      coil.setGasBurnerEfficiency(value);
    }

    // FurnPilotFuelInp
    QDomElement furnPilotFuelInpElement = heatingCoilElement.firstChildElement("FurnPilotFuelInp");
    value = furnPilotFuelInpElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"Btu/h","W").get();
      coil.setParasiticGasLoad(value);
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().toStdString(),"HotWater") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilHeatingWater coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    if( capTotGrossRtd )
    {
      coil.setPerformanceInputMethod("NominalCapacity");

      coil.setRatedCapacity(capTotGrossRtd.get());

      coil.setRatedInletWaterTemperature(82.2);

      coil.setRatedInletAirTemperature(16.6);

      coil.setRatedOutletWaterTemperature(54.4);

      coil.setRatedOutletAirTemperature(32.2);
    }

    QDomElement fluidSegInRefElement = heatingCoilElement.firstChildElement("FluidSegInRef");

    if( boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement.text(),doc,model) )
    {
      plant->addDemandBranchForComponent(coil);
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().toStdString(),"Resistance") )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingElectric coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().toStdString(),"HeatPump") )
  {
    // HtPumpCap_fTempCrvRef
    boost::optional<model::Curve> totalHeatingCapacityFunctionofTemperatureCurve;
    QDomElement totalHeatingCapacityFunctionofTemperatureCurveElement = 
      heatingCoilElement.firstChildElement("HtPumpCap_fTempCrvRef");
    totalHeatingCapacityFunctionofTemperatureCurve = 
      model.getModelObjectByName<model::Curve>(
        totalHeatingCapacityFunctionofTemperatureCurveElement.text().toStdString());

    if( ! totalHeatingCapacityFunctionofTemperatureCurve )
    {
      model::CurveCubic _totalHeatingCapacityFunctionofTemperatureCurve(model);
      _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
      _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
      _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
      _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
      _totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      _totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      totalHeatingCapacityFunctionofTemperatureCurve = _totalHeatingCapacityFunctionofTemperatureCurve;
    }

    // HtPumpCap_fFlowCrvRef
    boost::optional<model::Curve> totalHeatingCapacityFunctionofFlowFractionCurve;
    QDomElement totalHeatingCapacityFunctionofFlowFractionCurveElement = 
      heatingCoilElement.firstChildElement("HtPumpCap_fFlowCrvRef");
    totalHeatingCapacityFunctionofFlowFractionCurve = 
      model.getModelObjectByName<model::Curve>(
        totalHeatingCapacityFunctionofFlowFractionCurveElement.text().toStdString());

    if( ! totalHeatingCapacityFunctionofFlowFractionCurve )
    {
      model::CurveCubic _totalHeatingCapacityFunctionofFlowFractionCurve(model);
      _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
      _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
      _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
      _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
      _totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
      _totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

      totalHeatingCapacityFunctionofFlowFractionCurve = _totalHeatingCapacityFunctionofFlowFractionCurve;
    }

    // HtPumpEIR_fTempCrvRef 
    boost::optional<model::Curve> energyInputRatioFunctionofTemperatureCurve;
    QDomElement energyInputRatioFunctionofTemperatureCurveElement = 
      heatingCoilElement.firstChildElement("HtPumpEIR_fTempCrvRef");
    energyInputRatioFunctionofTemperatureCurve = 
      model.getModelObjectByName<model::Curve>(
        energyInputRatioFunctionofTemperatureCurveElement.text().toStdString());

    if( ! energyInputRatioFunctionofTemperatureCurve )
    {
      model::CurveCubic _energyInputRatioFunctionofTemperatureCurve(model);
      _energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
      _energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
      _energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
      _energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
      _energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      _energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      energyInputRatioFunctionofTemperatureCurve = _energyInputRatioFunctionofTemperatureCurve;
    }

    // HtPumpEIR_fFlowCrvRef
    boost::optional<model::Curve> energyInputRatioFunctionofFlowFractionCurve;
    QDomElement energyInputRatioFunctionofFlowFractionCurveElement = 
      heatingCoilElement.firstChildElement("HtPumpEIR_fFlowCrvRef");
    energyInputRatioFunctionofFlowFractionCurve = 
      model.getModelObjectByName<model::Curve>(
        energyInputRatioFunctionofFlowFractionCurveElement.text().toStdString());

    if( ! energyInputRatioFunctionofFlowFractionCurve )
    {
      model::CurveQuadratic _energyInputRatioFunctionofFlowFractionCurve(model);
      _energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
      _energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
      _energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
      _energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
      _energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

      energyInputRatioFunctionofFlowFractionCurve = _energyInputRatioFunctionofFlowFractionCurve;
    }

    // HtPumpEIR_fPLFCrvRef 
    boost::optional<model::Curve> partLoadFractionCorrelationCurve;
    QDomElement partLoadFractionCorrelationCurveElement = 
      heatingCoilElement.firstChildElement("HtPumpEIR_fPLFCrvRef");
    partLoadFractionCorrelationCurve = 
      model.getModelObjectByName<model::Curve>(
        partLoadFractionCorrelationCurveElement.text().toStdString());

    if( ! partLoadFractionCorrelationCurve )
    {
      model::CurveQuadratic _partLoadFractionCorrelationCurve(model);
      _partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
      _partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
      _partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
      _partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
      _partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

      partLoadFractionCorrelationCurve = _partLoadFractionCorrelationCurve;
    }

    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilHeatingDXSingleSpeed heatingCoil( model,
                                                 schedule,
                                                 totalHeatingCapacityFunctionofTemperatureCurve.get(),
                                                 totalHeatingCapacityFunctionofFlowFractionCurve.get(),
                                                 energyInputRatioFunctionofTemperatureCurve.get(),
                                                 energyInputRatioFunctionofFlowFractionCurve.get(),
                                                 partLoadFractionCorrelationCurve.get() ); 

    // HtPumpEIR
    QDomElement htPumpEIRElement = heatingCoilElement.firstChildElement("HtPumpEIR");
    value = htPumpEIRElement.text().toDouble(&ok);
    if( ok ) 
    { 
      heatingCoil.setRatedCOP(1.0 / value);
    } 

    // HtPumpCprsrLockoutTemp
    QDomElement htPumpCprsrLockoutTempElement = heatingCoilElement.firstChildElement("HtPumpCprsrLockoutTemp");
    value = htPumpCprsrLockoutTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();
      heatingCoil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(value);
    }

    // HtPumpCrankcaseHtCap
    QDomElement htPumpCrankcaseHtCapElement = heatingCoilElement.firstChildElement("HtPumpCrankcaseHtCap");
    value = htPumpCrankcaseHtCapElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"Btu/h","W").get();
      heatingCoil.setCrankcaseHeaterCapacity(value);
    }

    // HtPumpCrankcaseCtrlTemp
    QDomElement htPumpCrankcaseCtrlTempElement = heatingCoilElement.firstChildElement("HtPumpCrankcaseCtrlTemp");
    value = htPumpCrankcaseCtrlTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();
      heatingCoil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(value);
    }

    // HtPumpDefHtSrc
    QDomElement htPumpDefHtSrcElement = heatingCoilElement.firstChildElement("HtPumpDefHtSrc");
    if( istringEqual(htPumpDefHtSrcElement.text().toStdString(),"Electric") )
    {
      heatingCoil.setDefrostStrategy("Resistive");
    }
    else if( istringEqual(htPumpDefHtSrcElement.text().toStdString(),"HotGas") )
    {
      heatingCoil.setDefrostStrategy("ReverseCycle");
    }

    // HtPumpDefHtCap
    QDomElement htPumpDefHtCapElement = heatingCoilElement.firstChildElement("HtPumpDefHtCap");
    value = htPumpDefHtCapElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"Btu/h","W").get();
      heatingCoil.setResistiveDefrostHeaterCapacity(value);
    }

    // HtPumpDefCtrl 
    QDomElement htPumpDefCtrlElement = heatingCoilElement.firstChildElement("HtPumpDefCtrl");
    if( istringEqual(htPumpDefCtrlElement.text().toStdString(),"OnDemand") )
    {
      heatingCoil.setDefrostControl("OnDemand");
    }
    else if( istringEqual(htPumpDefCtrlElement.text().toStdString(),"TimedCycle") )
    {
      heatingCoil.setDefrostControl("Timed");
    }

    // HtPumpDefCtrlTemp
    QDomElement htPumpDefCtrlTempElement = heatingCoilElement.firstChildElement("HtPumpDefCtrlTemp");
    value = htPumpDefCtrlTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();
      heatingCoil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(value);
    }

    result = heatingCoil;
  }
  else
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingElectric coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    result = coil;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFan(
  const QDomElement& fanElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(fanElement.tagName().toStdString(),"Fan") )
  {
    return result;
  }

  // FanControlMethod
  QDomElement fanControlMethodElement = fanElement.firstChildElement("CtrlMthd");

  // Name
  QDomElement nameElement = fanElement.firstChildElement("Name");

  // OverallEff
  QDomElement overallEffElement = fanElement.firstChildElement("OverallEff");

  // MtrfEff 
  QDomElement mtrEffElement = fanElement.firstChildElement("MtrEff");

  // FlowCap
  QDomElement flowCapElement = fanElement.firstChildElement("FlowCap");

  // TotStaticPress
  QDomElement totStaticPressElement = fanElement.firstChildElement("TotStaticPress"); 

  // MtrPos
  QDomElement mtrPosElement = fanElement.firstChildElement("MtrPos");
  bool motorInAirstream = false;
  if( istringEqual(mtrPosElement.text().toStdString(),"InAirStream") )
  {
    motorInAirstream = true;
  }

  boost::optional<double> flowCap;

  if( ! autosize() )
  {
    value = flowCapElement.text().toDouble(&ok);

    if( ok ) 
    {
      Quantity flowRateIP(value,createCFMVolumetricFlowrate());
      OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(flowRateSI);
      flowCap = flowRateSI->value();
    }
  }

  // ConstantVolume
  if( istringEqual(fanControlMethodElement.text().toStdString(),"ConstantVolume") )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::FanConstantVolume fan(model,schedule);

    fan.setName(nameElement.text().toStdString());

    // OverallEff
    value = overallEffElement.text().toDouble(&ok);
    if( ok )
    {
      fan.setFanEfficiency(value);
    }

    // MtrEff
    value = mtrEffElement.text().toDouble(&ok);
    if( ok )
    {
      fan.setMotorEfficiency(value);
    }

    // FlowCap
    if( flowCap )
    {
      fan.setMaximumFlowRate(flowCap.get());
    }

    // TotStaticPress
    value = totStaticPressElement.text().toDouble(&ok);
    if( ok )
    {
      // Convert in WC to Pa
      fan.setPressureRise(value * 249.0889 );
    }

    // MtrPos
    if( motorInAirstream )
    {
      fan.setMotorInAirstreamFraction(1.0);
    }
    else
    {
      fan.setMotorInAirstreamFraction(0.0);
    }

    result = fan;
  }
  // Variable Volume
  else if( istringEqual(fanControlMethodElement.text().toStdString(),"VariableSpeedDrive") ) 
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::FanVariableVolume fan(model,schedule);

    fan.setName(nameElement.text().toStdString());

    value = overallEffElement.text().toDouble(&ok);
    if( ok )
    {
      fan.setFanEfficiency(value);
    }

    value = mtrEffElement.text().toDouble(&ok);
    if( ok )
    {
      fan.setMotorEfficiency(value);
    }

    // FlowCap
    if( flowCap )
    {
      fan.setMaximumFlowRate(flowCap.get());
    }

    // TotStaticPress
    value = totStaticPressElement.text().toDouble(&ok);
    if( ok )
    {
      // Convert in WC to Pa
      fan.setPressureRise(value * 249.0889 );
    }

    // Pwr_fPLRCrvRef
    QDomElement pwr_fPLRCrvElement = fanElement.firstChildElement("Pwr_fPLRCrvRef");
    boost::optional<model::Curve> pwr_fPLRCrv;
    pwr_fPLRCrv = model.getModelObjectByName<model::Curve>(pwr_fPLRCrvElement.text().toStdString());
    if( pwr_fPLRCrv )
    {
      if( boost::optional<model::CurveCubic> curveCubic = pwr_fPLRCrv->optionalCast<model::CurveCubic>() )
      {
        fan.setFanPowerCoefficient1(curveCubic->coefficient1Constant());
        fan.setFanPowerCoefficient2(curveCubic->coefficient2x());
        fan.setFanPowerCoefficient3(curveCubic->coefficient3xPOW2());
        fan.setFanPowerCoefficient4(curveCubic->coefficient4xPOW3());
        fan.setFanPowerCoefficient5(0.0);
      }
      else
      {
        LOG(Warn,"Fan: " << fan.name().get() << " references an unsupported curve type.");
      }
    }

    // MtrPos
    if( motorInAirstream )
    {
      fan.setMotorInAirstreamFraction(1.0);
    }
    else
    {
      fan.setMotorInAirstreamFraction(0.0);
    }

    result = fan;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilCooling(
  const QDomElement& coolingCoilElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(coolingCoilElement.tagName().toStdString(),"CoilClg") )
  {
    return result;
  }

  // Go up and look for a containning air system element
  
  QDomElement airSysElement = coolingCoilElement.parentNode().parentNode().toElement();

  QDomElement airSystemTypeElement;

  if( ! airSysElement.isNull() )
  {
    airSystemTypeElement = airSysElement.firstChildElement("Type");
  }

  // Look for a sibling fan to figure out what the flow capacity should be

  QDomElement flowCapElement;

  QDomElement airSegElement = coolingCoilElement.parentNode().toElement();

  if( ! airSegElement.isNull() )
  {
    QDomElement fanElement = airSegElement.firstChildElement("Fan");

    if( ! fanElement.isNull() )
    {
      flowCapElement = fanElement.firstChildElement("FlowCap");
    }
  }

  // CapTotRtd
  QDomElement capTotRtdElement = coolingCoilElement.firstChildElement("CapTotGrossRtd");

  // CapSensRtd
  QDomElement capSensRtdElement = coolingCoilElement.firstChildElement("CapSensGrossRtd");

  // Type
  QDomElement coilCoolingTypeElement = coolingCoilElement.firstChildElement("Type");

  // DirectExpansion
  if( istringEqual(coilCoolingTypeElement.text().toStdString(),"DirectExpansion") )
  {
    // Name

    QDomElement nameElement = coolingCoilElement.firstChildElement("Name");

    // NumClgStages

    QDomElement numClgStagesElement = coolingCoilElement.firstChildElement("NumClgStages");

    if( numClgStagesElement.text().toInt() == 1 || numClgStagesElement.isNull() )
    {
      model::Schedule schedule = model.alwaysOnDiscreteSchedule();

      // Cap_fTempCrvRef

      boost::optional<model::Curve> coolingCurveFofTemp;
      QDomElement cap_fTempCrvRefElement = coolingCoilElement.firstChildElement("Cap_fTempCrvRef");
      coolingCurveFofTemp = model.getModelObjectByName<model::Curve>(cap_fTempCrvRefElement.text().toStdString());
      if( ! coolingCurveFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _coolingCurveFofTemp(model);
        _coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
        _coolingCurveFofTemp.setCoefficient2x(0.009543347);
        _coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
        _coolingCurveFofTemp.setCoefficient4y(-0.011042676);
        _coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
        _coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
        _coolingCurveFofTemp.setMinimumValueofx(17.0);
        _coolingCurveFofTemp.setMaximumValueofx(22.0);
        _coolingCurveFofTemp.setMinimumValueofy(13.0);
        _coolingCurveFofTemp.setMaximumValueofy(46.0);
        coolingCurveFofTemp = _coolingCurveFofTemp;
      }

      // Cap_fFlowCrvRef
      
      boost::optional<model::Curve> coolingCurveFofFlow;
      QDomElement cap_fFlowCrvRefElement = coolingCoilElement.firstChildElement("Cap_fFlowCrvRef");
      coolingCurveFofFlow = model.getModelObjectByName<model::Curve>(cap_fFlowCrvRefElement.text().toStdString());
      if( ! coolingCurveFofFlow )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fFlowCrvRef");

        // Provide default curves
        model::CurveQuadratic _coolingCurveFofFlow(model);
        _coolingCurveFofFlow.setCoefficient1Constant(0.8);
        _coolingCurveFofFlow.setCoefficient2x(0.2);
        _coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
        _coolingCurveFofFlow.setMinimumValueofx(0.5);
        _coolingCurveFofFlow.setMaximumValueofx(1.5);
        coolingCurveFofFlow = _coolingCurveFofFlow;
      }


      // DXEIR_fTempCrvRef

      boost::optional<model::Curve> energyInputRatioFofTemp;
      QDomElement dxEIR_fTempCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fTempCrvRef");
      energyInputRatioFofTemp = model.getModelObjectByName<model::Curve>(dxEIR_fTempCrvRefElement.text().toStdString());
      if( ! energyInputRatioFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _energyInputRatioFofTemp(model);
        _energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
        _energyInputRatioFofTemp.setCoefficient2x(0.034885008);
        _energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
        _energyInputRatioFofTemp.setCoefficient4y(0.004977216);
        _energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
        _energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
        _energyInputRatioFofTemp.setMinimumValueofx(17.0);
        _energyInputRatioFofTemp.setMaximumValueofx(22.0);
        _energyInputRatioFofTemp.setMinimumValueofy(13.0);
        _energyInputRatioFofTemp.setMaximumValueofy(46.0);
        energyInputRatioFofTemp = _energyInputRatioFofTemp;
      }

      // DX EIR function of Flow

      boost::optional<model::Curve> energyInputRatioFofFlow;
      QDomElement dxEIR_fFlowCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fFlowCrvRef");
      energyInputRatioFofFlow = model.getModelObjectByName<model::Curve>(dxEIR_fFlowCrvRefElement.text().toStdString());
      if( ! energyInputRatioFofFlow )
      {
        model::CurveQuadratic _energyInputRatioFofFlow(model);
        _energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
        _energyInputRatioFofFlow.setCoefficient2x(-0.32953);
        _energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
        _energyInputRatioFofFlow.setMinimumValueofx(0.75918);
        _energyInputRatioFofFlow.setMaximumValueofx(1.13877);
        energyInputRatioFofFlow = _energyInputRatioFofFlow;
      }

      // DXEIR_fPLRCrvRef

      boost::optional<model::Curve> partLoadFraction;
      QDomElement dxEIR_fPLFCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fPLFCrvRef");
      partLoadFraction = model.getModelObjectByName<model::Curve>(dxEIR_fPLFCrvRefElement.text().toStdString());
      if( ! partLoadFraction )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fPLFCrvRef");

        // Provide default curves
        model::CurveQuadratic _partLoadFraction(model);
        _partLoadFraction.setCoefficient1Constant(0.85);
        _partLoadFraction.setCoefficient2x(0.15);
        _partLoadFraction.setCoefficient3xPOW2(0.0);
        _partLoadFraction.setMinimumValueofx(0.0);
        _partLoadFraction.setMaximumValueofx(1.0);
        partLoadFraction = _partLoadFraction;
      }

      model::CoilCoolingDXSingleSpeed coilCooling( model,
                                                   schedule,
                                                   coolingCurveFofTemp.get(),
                                                   coolingCurveFofFlow.get(),
                                                   energyInputRatioFofTemp.get(),
                                                   energyInputRatioFofFlow.get(),
                                                   partLoadFraction.get() );

      coilCooling.setName(nameElement.text().toStdString());

      // FlowCap
      if( ! autosize() )
      {
        value = flowCapElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity flowRateIP(value,createCFMVolumetricFlowrate());
          OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(flowRateSI);
          coilCooling.setRatedAirFlowRate(flowRateSI->value());
        }
      }

      boost::optional<double> totalCapacity;
      boost::optional<double> sensibleCapacity;

      // CapTotRtd
      if( ! autosize() )
      {
        value = capTotRtdElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity valueIP(value,createBTUPower());
          OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(valueSI);
          coilCooling.setRatedTotalCoolingCapacity(valueSI->value());

          totalCapacity = value;
        }
      }

      // CapSensRtd
      if( ! autosize() )
      {
        value = capSensRtdElement.text().toDouble(&ok);

        if( ok )
        {
          sensibleCapacity = value;
        }
      }

      if( totalCapacity && sensibleCapacity )
      {
        coilCooling.setRatedSensibleHeatRatio( sensibleCapacity.get() / totalCapacity.get() );
      }

      // DXEIR
      QDomElement dxEIRElement = coolingCoilElement.firstChildElement("DXEIR");
      value = dxEIRElement.text().toDouble(&ok);
      if( ok )
      {
        coilCooling.setRatedCOP( 1.0 / value);
      }

      // DXCrankcaseHtCap
      QDomElement dxCrankcaseHtCapElement = coolingCoilElement.firstChildElement("DXCrankcaseHtCap");
      value = dxCrankcaseHtCapElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"Btu/h","W").get();
        coilCooling.setCrankcaseHeaterCapacity(value);
      }

      // DXCrankcaseCtrlTemp
      QDomElement dxCrankcaseCtrlTempElement = coolingCoilElement.firstChildElement("DXCrankcaseCtrlTemp");
      value = dxCrankcaseCtrlTempElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        coilCooling.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(value);
      }

      result = coilCooling;
    }
    else if( numClgStagesElement.text().toInt() == 2 || numClgStagesElement.isNull() )
    {
      model::Schedule schedule = alwaysOnSchedule(model);

      // Cap_fTempCrvRef

      boost::optional<model::Curve> coolingCurveFofTemp;
      QDomElement cap_fTempCrvRefElement = coolingCoilElement.firstChildElement("Cap_fTempCrvRef");
      coolingCurveFofTemp = model.getModelObjectByName<model::Curve>(cap_fTempCrvRefElement.text().toStdString());
      if( ! coolingCurveFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _coolingCurveFofTemp(model);
        _coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
        _coolingCurveFofTemp.setCoefficient2x(0.009543347);
        _coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
        _coolingCurveFofTemp.setCoefficient4y(-0.011042676);
        _coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
        _coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
        _coolingCurveFofTemp.setMinimumValueofx(17.0);
        _coolingCurveFofTemp.setMaximumValueofx(22.0);
        _coolingCurveFofTemp.setMinimumValueofy(13.0);
        _coolingCurveFofTemp.setMaximumValueofy(46.0);
        coolingCurveFofTemp = _coolingCurveFofTemp;
      }

      // Cap_fFlowCrvRef
      
      boost::optional<model::Curve> coolingCurveFofFlow;
      QDomElement cap_fFlowCrvRefElement = coolingCoilElement.firstChildElement("Cap_fFlowCrvRef");
      coolingCurveFofFlow = model.getModelObjectByName<model::Curve>(cap_fFlowCrvRefElement.text().toStdString());
      if( ! coolingCurveFofFlow )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fFlowCrvRef");

        // Provide default curves
        model::CurveQuadratic _coolingCurveFofFlow(model);
        _coolingCurveFofFlow.setCoefficient1Constant(0.8);
        _coolingCurveFofFlow.setCoefficient2x(0.2);
        _coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
        _coolingCurveFofFlow.setMinimumValueofx(0.5);
        _coolingCurveFofFlow.setMaximumValueofx(1.5);
        coolingCurveFofFlow = _coolingCurveFofFlow;
      }


      // DXEIR_fTempCrvRef

      boost::optional<model::Curve> energyInputRatioFofTemp;
      QDomElement dxEIR_fTempCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fTempCrvRef");
      energyInputRatioFofTemp = model.getModelObjectByName<model::Curve>(dxEIR_fTempCrvRefElement.text().toStdString());
      if( ! energyInputRatioFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _energyInputRatioFofTemp(model);
        _energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
        _energyInputRatioFofTemp.setCoefficient2x(0.034885008);
        _energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
        _energyInputRatioFofTemp.setCoefficient4y(0.004977216);
        _energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
        _energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
        _energyInputRatioFofTemp.setMinimumValueofx(17.0);
        _energyInputRatioFofTemp.setMaximumValueofx(22.0);
        _energyInputRatioFofTemp.setMinimumValueofy(13.0);
        _energyInputRatioFofTemp.setMaximumValueofy(46.0);
        energyInputRatioFofTemp = _energyInputRatioFofTemp;
      }

      // DX EIR function of Flow

      boost::optional<model::Curve> energyInputRatioFofFlow;
      QDomElement dxEIR_fFlowCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fFlowCrvRef");
      energyInputRatioFofFlow = model.getModelObjectByName<model::Curve>(dxEIR_fFlowCrvRefElement.text().toStdString());
      if( ! energyInputRatioFofFlow )
      {
        model::CurveQuadratic _energyInputRatioFofFlow(model);
        _energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
        _energyInputRatioFofFlow.setCoefficient2x(-0.32953);
        _energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
        _energyInputRatioFofFlow.setMinimumValueofx(0.75918);
        _energyInputRatioFofFlow.setMaximumValueofx(1.13877);
        energyInputRatioFofFlow = _energyInputRatioFofFlow;
      }

      // DXEIR_fPLRCrvRef

      boost::optional<model::Curve> partLoadFraction;
      QDomElement dxEIR_fPLFCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fPLFCrvRef");
      partLoadFraction = model.getModelObjectByName<model::Curve>(dxEIR_fPLFCrvRefElement.text().toStdString());
      if( ! partLoadFraction )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fPLFCrvRef");

        // Provide default curves
        model::CurveQuadratic _partLoadFraction(model);
        _partLoadFraction.setCoefficient1Constant(0.85);
        _partLoadFraction.setCoefficient2x(0.15);
        _partLoadFraction.setCoefficient3xPOW2(0.0);
        _partLoadFraction.setMinimumValueofx(0.0);
        _partLoadFraction.setMaximumValueofx(1.0);
        partLoadFraction = _partLoadFraction;
      }
  
      model::CoilCoolingDXTwoSpeed coilCooling( model,
                                              schedule,
                                              coolingCurveFofTemp.get(),
                                              coolingCurveFofFlow.get(),
                                              energyInputRatioFofTemp.get(),
                                              energyInputRatioFofFlow.get(),
                                              partLoadFraction.get(), 
                                              coolingCurveFofTemp->clone(model).cast<model::Curve>(),
                                              energyInputRatioFofTemp->clone(model).cast<model::Curve>());

      coilCooling.setName(nameElement.text().toStdString());

      boost::optional<double> totalCapacity;
      boost::optional<double> sensibleCapacity;
      boost::optional<double> lowSpeedCapacityFraction;

      // CapTotRtdStageFrac
      QDomNodeList capTotRtdStageFracElements = coolingCoilElement.elementsByTagName("CapTotRtdStageFrac");
      for( int j = 0; j < capTotRtdStageFracElements.count(); j++ )
      {
        QDomElement capTotRtdStageFracElement = capTotRtdStageFracElements.at(j).toElement();
        QString indexAttribute = capTotRtdStageFracElement.attribute("index");
        if( istringEqual(indexAttribute.toStdString(),"0") )
        {
          double fraction = capTotRtdStageFracElement.text().toDouble(&ok);
          if( ok )
          {
            lowSpeedCapacityFraction = fraction;
          }

          break;
        }
      }

      // FlowCap
      if( ! autosize() )
      {
        value = flowCapElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity flowRateIP(value,createCFMVolumetricFlowrate());
          OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(flowRateSI);
          coilCooling.setRatedHighSpeedAirFlowRate(flowRateSI->value());
          if( lowSpeedCapacityFraction )
          {
            coilCooling.setRatedLowSpeedAirFlowRate(flowRateSI->value() * lowSpeedCapacityFraction.get());
          }
        }
      }

      // CapTotRtd
      if( ! autosize() )
      {
        value = capTotRtdElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity valueIP(value,createBTUPower());
          OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(valueSI);
          coilCooling.setRatedHighSpeedTotalCoolingCapacity(valueSI->value());
          if( lowSpeedCapacityFraction )
          {
            coilCooling.setRatedLowSpeedTotalCoolingCapacity(valueSI->value() * lowSpeedCapacityFraction.get());
          }

          totalCapacity = value;
        }
      }

      // CapSensRtd
      if( ! autosize() )
      {
        value = capSensRtdElement.text().toDouble(&ok);

        if( ok )
        {
          sensibleCapacity = value;
        }
      }

      if( totalCapacity && sensibleCapacity )
      {
        coilCooling.setRatedHighSpeedSensibleHeatRatio( sensibleCapacity.get() / totalCapacity.get() );

        // CapSensRtdStageFrac
        if( lowSpeedCapacityFraction )
        {
          QDomNodeList capSensRtdStageFracElements = coolingCoilElement.elementsByTagName("CapSensRtdStageFrac");
          for( int j = 0; j < capSensRtdStageFracElements.count(); j++ )
          {
            QDomElement capSensRtdStageFracElement = capSensRtdStageFracElements.at(j).toElement();
            QString indexAttribute = capSensRtdStageFracElement.attribute("index");
            if( istringEqual(indexAttribute.toStdString(),"0") )
            {
              double fraction = capSensRtdStageFracElement.text().toDouble(&ok);
              if( ok )
              {
                double lowSHR = (sensibleCapacity.get() * fraction) / (totalCapacity.get() * lowSpeedCapacityFraction.get());
                coilCooling.setRatedLowSpeedSensibleHeatRatio(lowSHR);
              }
              break;
            }
          }
        }
      }

      // DXEIR
      
      QDomElement dxEIRElement = coolingCoilElement.firstChildElement("DXEIR");

      value = dxEIRElement.text().toDouble(&ok);

      if( ok )
      {
        double cop = 1.0 / value;

        coilCooling.setRatedHighSpeedCOP(cop);

        coilCooling.setRatedLowSpeedCOP(cop);
      }

      result = coilCooling;
    }
    else if( numClgStagesElement.text().toInt() > 2 || numClgStagesElement.isNull() )
    {
      LOG(Error, "DX cooling coils with more than two stages are not yet supported. See coil named " << nameElement.text().toStdString());
    }
  }
  // ChilledWater
  if( istringEqual(coilCoolingTypeElement.text().toStdString(),"ChilledWater") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilCoolingWater coilCooling(model,schedule);

    // Name

    QDomElement nameElement = coolingCoilElement.firstChildElement("Name");

    coilCooling.setName(nameElement.text().toStdString());


    if( ! autosize() )
    {
      // Design Water Volum Flow Rate of Coil

      QDomElement fluidFlowRtDsgnElement = coolingCoilElement.firstChildElement("FluidFlowRtDsgn");

      coilCooling.setDesignWaterFlowRate(fluidFlowRtDsgnElement.text().toDouble() * 0.00006309);

      coilCooling.setDesignInletWaterTemperature(7.22); // From Sizing Plant

      coilCooling.setDesignInletAirTemperature(25.0); // Assumption

      coilCooling.setDesignOutletAirTemperature(12.8); // From Sizing System

      coilCooling.setDesignInletAirHumidityRatio(0.012); // Assumption

      coilCooling.setDesignOutletAirHumidityRatio(0.0085); // From Sizing System

      // FlowCap

      double value = flowCapElement.text().toDouble();

      Quantity flowRateIP(value,createCFMVolumetricFlowrate());
      OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(flowRateSI);
      coilCooling.setDesignAirFlowRate(flowRateSI->value());
    }

    // Plant

    QDomElement fluidSegNameElement = coolingCoilElement.firstChildElement("FluidSegInRef");

    if( boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegNameElement.text(),doc,model) )
    {
      plant->addDemandBranchForComponent(coilCooling);
    }

    result = coilCooling;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateThermalZone(
  const QDomElement& thermalZoneElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model )
{
  UnitSystem siSys(UnitSystem::SI);
  UnitSystem whSys(UnitSystem::Wh);

  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(thermalZoneElement.tagName().toStdString(),"ThrmlZn") )
  {
    return result;
  }

  boost::optional<model::ThermalZone> optionalThermalZone;
  boost::optional<model::ThermostatSetpointDualSetpoint> optionalThermostat;

  // Name
  QDomElement nameElement = thermalZoneElement.firstChildElement("Name");
  std::string name = nameElement.text().toStdString();
  optionalThermalZone = model.getModelObjectByName<model::ThermalZone>(name);

  if( ! optionalThermalZone )
  {
    return result;
  }

  model::ThermalZone thermalZone = optionalThermalZone.get();
  result = thermalZone;

  // Type
  QDomElement typeElement = thermalZoneElement.firstChildElement("Type");
  if(!typeElement.isNull()){
    if (typeElement.text() == "Conditioned"){
      optionalThermostat = model::ThermostatSetpointDualSetpoint(model);
      optionalThermostat->setName(name + " Thermostat");
      thermalZone.setThermostatSetpointDualSetpoint(*optionalThermostat);
    }else if (typeElement.text() == "Unconditioned"){
      // no thermostat
    }else if (typeElement.text() == "Plenum"){
      // no thermostat
    }else{
      LOG(Error, "Unknown thermal zone type '" << toString(typeElement.text()) << "'");
    }
  }

  // Ventilation
  QDomElement ventRtElement = thermalZoneElement.firstChildElement("DsgnVentRt");
  if (!ventRtElement.isNull() && (ventRtElement.text().toDouble() > 0))
  {
    openstudio::Quantity ventRateIP(ventRtElement.text().toDouble(), openstudio::createUnit("cfm",UnitSystem::BTU).get());
    OptionalQuantity ventRateSI = QuantityConverter::instance().convert(ventRateIP, siSys);
    OS_ASSERT(ventRateSI);
    OS_ASSERT(ventRateSI->units() == SIUnit(SIExpnt(0,3,-1)));

    // DsgnVentRt is CFM, so divide it up evenly among all attached spaces.
    std::vector<model::Space> spaces = thermalZone.spaces();

    for( std::vector<model::Space>::iterator it = spaces.begin();
         it != spaces.end();
         it++ )
    {
      openstudio::model::DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);
      designSpecificationOutdoorAir.setName(name + " Design Specification Outdoor Air");
      designSpecificationOutdoorAir.setOutdoorAirFlowRate(ventRateSI->value() / spaces.size());
      it->setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
    }
  }

  // Mult
  QDomElement multElement = thermalZoneElement.firstChildElement("Mult");
  if(!multElement.isNull()){
    thermalZone.setMultiplier(multElement.text().toInt());
  }

  // Optional AirLoopHVAC
  boost::optional<model::AirLoopHVAC> airLoopHVAC;

  // VentSysRef
  QDomElement ventSysRefElement = thermalZoneElement.firstChildElement("VentSysRef");

  // PrimaryAirConditioningSystemReference
  QDomElement primAirCondSysRefElement = thermalZoneElement.firstChildElement("PrimAirCondgSysRef");

  QDomElement znSysElement = findZnSysElement(primAirCondSysRefElement.text(),doc);

  if( ! znSysElement.isNull() )
  {
    boost::optional<model::ModelObject> mo = translateZnSys(znSysElement,doc,model);

    if( mo )
    {
      boost::optional<model::ZoneHVACComponent> zoneHVACComponent = mo->optionalCast<model::ZoneHVACComponent>();

      if( zoneHVACComponent )
      {
        zoneHVACComponent->addToThermalZone(thermalZone);

        // If not the ventilation system we lock down the oa system of the zone equipment
        if( primAirCondSysRefElement.text() != ventSysRefElement.text() )
        {
          if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> ptac = 
              zoneHVACComponent->optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>() )
          {
            ptac->setOutdoorAirFlowRateDuringHeatingOperation(0.0);
            ptac->setOutdoorAirFlowRateDuringCoolingOperation(0.0);
            ptac->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.0);
          }
          else if( boost::optional<model::ZoneHVACPackagedTerminalHeatPump> pthp = 
              zoneHVACComponent->optionalCast<model::ZoneHVACPackagedTerminalHeatPump>() )
          {
            pthp->setOutdoorAirFlowRateDuringHeatingOperation(0.0);
            pthp->setOutdoorAirFlowRateDuringCoolingOperation(0.0);
            pthp->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.0);
          }
          else if( boost::optional<model::ZoneHVACFourPipeFanCoil> fanCoil = 
              zoneHVACComponent->optionalCast<model::ZoneHVACFourPipeFanCoil>() )
          {
            fanCoil->setMaximumOutdoorAirFlowRate(0.0);
          }
        }
      }
    }
  }
  else
  {
    airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(primAirCondSysRefElement.text().toStdString());

    if( airLoopHVAC && ! thermalZone.airLoopHVAC() )
    {
      airLoopHVAC->addBranchForZone(thermalZone);
    }
  }

  // The ventSysRef is currently assumed to be an air system or zone equipment that is already listed as
  // primAirCondSysRefElement.  This section is intended to catch DOAS air systems.
  if( ! ventSysRefElement.isNull() && ventSysRefElement.text() != primAirCondSysRefElement.text() )
  {
    airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(primAirCondSysRefElement.text().toStdString());

    if( airLoopHVAC && ! thermalZone.airLoopHVAC() )
    {
      airLoopHVAC->addBranchForZone(thermalZone);
    }
  }

  // DCV?
  QDomElement ventCtrlMthdElement = thermalZoneElement.firstChildElement("VentCtrlMthd");
  if( istringEqual(ventCtrlMthdElement.text().toStdString(),"OccupantSensors") ||
      istringEqual(ventCtrlMthdElement.text().toStdString(),"CO2Sensors") )
  {
    if( airLoopHVAC )
    {
      if( boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem() )
      {
        oaSystem->getControllerOutdoorAir().controllerMechanicalVentilation().setDemandControlledVentilation(true);
      }
    }
  }

  // ClgTstatSchRef
  QDomElement clgTstatSchRefElement = thermalZoneElement.firstChildElement("ClgTstatSchRef");
  if (!clgTstatSchRefElement.isNull()){
    std::string scheduleName = escapeName(clgTstatSchRefElement.text());
    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
    if (schedule){
      if (optionalThermostat){
        optionalThermostat->setCoolingSchedule(*schedule);
      }else{
        LOG(Error, "Cannot assign cooling schedule to unconditioned thermal zone '" << name << "'");
      }
    }else{
      LOG(Error, "Could not find schedule '" << scheduleName << "'");
    }
  }

  // HtgTstatSchRef
  QDomElement htgTstatSchRefElement = thermalZoneElement.firstChildElement("HtgTstatSchRef");
  if (!htgTstatSchRefElement.isNull()){
    std::string scheduleName = escapeName(htgTstatSchRefElement.text());
    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
    if (schedule){
      if (optionalThermostat){
        optionalThermostat->setHeatingSchedule(*schedule);
      }else{
        LOG(Error, "Cannot assign heating schedule to unconditioned thermal zone '" << name << "'");
      }
    }else{
      LOG(Error, "Could not find schedule '" << scheduleName << "'");
    }
  }

  // ThermalZoneVentilationSystem
  QDomElement thermalZoneVentilationSystemElement = thermalZoneElement.firstChildElement("VentSysRef");

  if( thermalZoneVentilationSystemElement.text() != primAirCondSysRefElement.text() )
  {
    airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(thermalZoneVentilationSystemElement.text().toStdString());

    if( airLoopHVAC && ! thermalZone.airLoopHVAC() )
    {
      airLoopHVAC->addBranchForZone(thermalZone);
    }

    // If the model has requested ventilation from the system,
    // make sure the system has an OA system even if it wasn't asked for.
    if( airLoopHVAC )
    {
      boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem();
      if( ! oaSystem )
      {
        model::ControllerOutdoorAir oaController(model);

        model::AirLoopHVACOutdoorAirSystem newOASystem(model,oaController);

        model::Node supplyInletNode = airLoopHVAC->supplyInletNode(); 

        newOASystem.addToNode(supplyInletNode);
      }
    }
  }

  airLoopHVAC = thermalZone.airLoopHVAC();

  QDomElement airSystemElement;

  // Add an air terminal to serve the zone
  if( airLoopHVAC )
  {
    bool terminalFound = false;

    QDomElement buildingElement = thermalZoneElement.parentNode().toElement();

    QDomNodeList airSystemElements = buildingElement.elementsByTagName("AirSys");

    for( int i = 0; i < airSystemElements.count(); i++ )
    {
      airSystemElement = airSystemElements.at(i).toElement();

      QDomElement nameElement = airSystemElement.firstChildElement("Name");

      if (nameElement.isNull())
      {
        continue;
      }

      if( istringEqual( airLoopHVAC->name().get(),nameElement.text().toStdString()) )
      {
        QDomNodeList terminalElements = airSystemElement.elementsByTagName("TrmlUnit");

        for( int j = 0; j < terminalElements.count(); j++ )
        {
          QDomElement terminalElement = terminalElements.at(j).toElement();

          QDomElement zoneServedElement = terminalElement.firstChildElement("ZnServedRef");

          if(istringEqual(thermalZone.name().get(),zoneServedElement.text().toStdString()))
          {
            boost::optional<model::ModelObject> terminalModelObject = translateTrmlUnit(terminalElement,doc,model);

            if( terminalModelObject )
            {
              if( boost::optional<model::HVACComponent> terminalHVACComponent = 
                    terminalModelObject->optionalCast<model::HVACComponent>() )
              {
                 model::Node airInletNode = thermalZone.inletPortList().airLoopHVACModelObject()->cast<model::Node>();

                 model::ModelObject nodeInletObject = airInletNode.inletModelObject().get();

                 if( nodeInletObject != airLoopHVAC->zoneSplitter() )
                 {
                    nodeInletObject.remove();
                 }

                 airInletNode = thermalZone.inletPortList().airLoopHVACModelObject()->cast<model::Node>();

                 terminalHVACComponent->addToNode(airInletNode);

                 //if( boost::optional<model::AirTerminalSingleDuctVAVReheat> terminalVAV = 
                 //      terminalModelObject->optionalCast<model::AirTerminalSingleDuctVAVReheat>() )
                 //{
                 //  if( airLoopHVAC->supplyComponents(model::CoilCoolingDXSingleSpeed::iddObjectType()).size() > 0 );
                 //  {
                 //    // TODO limit the turn down to something in these situations.
                 //    //terminalVAV->setConstantMinimumAirFlowFraction(0.5);
                 //  }
                 //}

                 terminalFound = true;

                 break;
              }
            }
          }
        }
      }

      if( terminalFound ) { break; }
    }
  }

  // Connect setpoint manager single zone reheat if required 
  if( airLoopHVAC )
  {
    model::Node supplyOutletNode = airLoopHVAC->supplyOutletNode();

    boost::optional<model::SetpointManagerSingleZoneReheat> spm; 

    spm = supplyOutletNode.getSetpointManagerSingleZoneReheat();

    // Only set the control zone if there is a SetpointManagerSingleZoneReheat on the supply outlet node
    if( spm && ! airSystemElement.isNull() )
    {
      QDomElement ctrlZnRefElement = airSystemElement.firstChildElement("CtrlZnRef");

      if( istringEqual(ctrlZnRefElement.text().toStdString(),thermalZone.name().get()) )
      {
        spm->setControlZone(thermalZone);
      }
    }
  }


  //<ThermalZone>
  //  <Name>Office Zone G2</Name>
  //  <ThermalZoneType>Conditioned</ThermalZoneType>
  //  <ThermalZoneCoolingSystem>Office Sys G2</ThermalZoneCoolingSystem>
  //  <ThermalZoneHeatingSystem>Office Sys G2</ThermalZoneHeatingSystem>
  //  <ThermalZoneVentilationSystem>Office Sys G2</ThermalZoneVentilationSystem>
  //  <ThermalZoneVentControlMethod>FixedVentilation</ThermalZoneVentControlMethod>
  //  <ThermalZoneVentRateDesign>50</ThermalZoneVentRateDesign>
  //  <ThermalZoneVentRateMin>50</ThermalZoneVentRateMin>
  //  <ThermalZoneExhaustSystem>Office Sys G2</ThermalZoneExhaustSystem>
  //  <ThermalZoneThrottlingRange>2</ThermalZoneThrottlingRange>
  //</ThermalZone>

  return result;
}

boost::optional<model::ModelObject> ReverseTranslator::translateTrmlUnit(const QDomElement& trmlUnitElement, 
                                                                         const QDomDocument& doc, 
                                                                         openstudio::model::Model& model)
{
  boost::optional<openstudio::model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(trmlUnitElement.tagName().toStdString(),"TrmlUnit") )
  {
    return result;
  }

  // Name
  QDomElement nameElement = trmlUnitElement.firstChildElement("Name");

  // Type
  QDomElement typeElement = trmlUnitElement.firstChildElement("Type");

  // PriAirFlow
  QDomElement priAirFlowElement = trmlUnitElement.firstChildElement("PriAirFlowMax");

  QDomElement priAirFlowMinElement = trmlUnitElement.firstChildElement("PriAirFlowMin");

  boost::optional<double> primaryAirFlow;
  boost::optional<double> primaryAirFlowMin;

  if( ! autosize() )
  {
    value = priAirFlowElement.text().toDouble(&ok);

    if( ok ) 
    {
      primaryAirFlow = unitToUnit(value,"cfm","m^3/s").get();
    }

    value = priAirFlowMinElement.text().toDouble(&ok);

    if( ok )
    {
      primaryAirFlowMin = unitToUnit(value,"cfm","m^3/s").get();
    }
  }

  QDomElement airSysElement = trmlUnitElement.parentNode().toElement();

  QDomElement airSystemTypeElement;

  if( ! airSysElement.isNull() )
  {
    airSystemTypeElement = airSysElement.firstChildElement("Type");
  }

  if( istringEqual("VAVReheatBox",typeElement.text().toStdString()) ||
      istringEqual("SZVAVAC",airSystemTypeElement.text().toStdString()) || 
      istringEqual("SZVAVHP",airSystemTypeElement.text().toStdString())
    )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    QDomElement coilElement = trmlUnitElement.firstChildElement("CoilHtg");

    boost::optional<model::HVACComponent> coil;

    if( ! coilElement.isNull() )
    {
      boost::optional<model::ModelObject> mo = translateCoilHeating(coilElement,doc,model);

      OS_ASSERT(mo);

      coil = mo->cast<model::HVACComponent>();
    }
    else if( istringEqual("SZVAVAC",airSystemTypeElement.text().toStdString()) || 
             istringEqual("SZVAVHP",airSystemTypeElement.text().toStdString()) )
    {
      if( ! airSysElement.isNull() )
      {
        // Air Segments
        QDomNodeList airSegmentElements = airSysElement.elementsByTagName("AirSeg");

        for (int i = 0; i < airSegmentElements.count(); i++)
        {
          QDomElement airSegmentElement = airSegmentElements.at(i).toElement();

          QDomElement airSegmentTypeElement = airSegmentElement.firstChildElement("Type");

          // Supply Segments
          if(istringEqual(airSegmentTypeElement.text().toStdString(),"Supply"))
          {
            QDomNodeList airSegmentChildElements = airSegmentElement.childNodes();

            for (int j = airSegmentChildElements.count() - 1; j >= 0 ; j--)
            {
              QDomElement airSegmentChildElement = airSegmentChildElements.at(j).toElement();

              if( istringEqual(airSegmentChildElement.tagName().toStdString(),"CoilHtg") )
              {
                boost::optional<model::ModelObject> mo = translateCoilHeating(airSegmentChildElement,doc,model);

                OS_ASSERT(mo);

                coil = mo->cast<model::HVACComponent>();
              }
            }
          } 
        }
      }
    }
    else // If no coil is specified in the SDD, create an electric coil
    {
      coil = model::CoilHeatingElectric(model,schedule);
    }

    OS_ASSERT(coil);

    model::AirTerminalSingleDuctVAVReheat terminal(model,schedule,coil.get());

    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    if( primaryAirFlowMin )
    {
      terminal.setZoneMinimumAirFlowMethod("FixedFlowRate");
      terminal.setFixedMinimumAirFlowRate(primaryAirFlowMin.get());
    }

    // ReheatCtrlMthd
    QDomElement reheatCtrlMthdElement = trmlUnitElement.firstChildElement("ReheatCtrlMthd");
    if( istringEqual(reheatCtrlMthdElement.text().toStdString(),"DualMaximum") )
    {
      terminal.setDamperHeatingAction("Reverse");

      QDomElement htgAirFlowMaxElement = trmlUnitElement.firstChildElement("HtgAirFlowMax");
      value = htgAirFlowMaxElement.text().toDouble(&ok);
      if( ok && primaryAirFlow )
      {
         value = unitToUnit(value,"cfm","m^3/s").get();
         double fraction = value / primaryAirFlow.get();
         terminal.setMaximumFlowFractionDuringReheat(fraction);
      }
    }
    else
    {
      terminal.setDamperHeatingAction("Normal");
    }

    result = terminal;
  }
  else if( istringEqual("SeriesFanBox",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingElectric coil(model,schedule);

    model::FanConstantVolume fan(model,schedule);

    model::AirTerminalSingleDuctParallelPIUReheat terminal(model,schedule,fan,coil);

    if( primaryAirFlow )
    {
      terminal.setMaximumPrimaryAirFlowRate(primaryAirFlow.get());
    }

    if( primaryAirFlow && primaryAirFlowMin )
    {
      terminal.setMinimumPrimaryAirFlowFraction(primaryAirFlowMin.get() / primaryAirFlow.get());
    }

    result = terminal;
  }
  else if( istringEqual("Uncontrolled",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::AirTerminalSingleDuctUncontrolled terminal(model,schedule);

    // PriAirFlow
    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    result = terminal;
  }
  else
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::AirTerminalSingleDuctUncontrolled terminal(model,schedule);

    result = terminal;
  }

  if( result )
  {
    result->setName(nameElement.text().toStdString());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFluidSys(
                                                  const QDomElement& fluidSysElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  double value;

  bool ok;

  if( ! istringEqual(fluidSysElement.tagName().toStdString(),"FluidSys") )
  {
    return result;
  }

  QDomElement nameElement = fluidSysElement.firstChildElement("Name");

  if( boost::optional<model::PlantLoop> plant = model.getModelObjectByName<model::PlantLoop>(nameElement.text().toStdString()) )
  {
    return plant.get();
  }

  model::PlantLoop plantLoop(model);

  model::Node supplyInletNode = plantLoop.supplyInletNode();
  model::Node supplyOutletNode = plantLoop.supplyOutletNode();

  bool bypass = false;

  // Name

  plantLoop.setName(nameElement.text().toStdString());

  model::SizingPlant sizingPlant = plantLoop.sizingPlant();

  // Type

  QDomElement typeElement = fluidSysElement.firstChildElement("Type");

  if( istringEqual(typeElement.text().toStdString(),"HotWater") )
  {
    model::Schedule schedule = hotWaterPlantSetpointSchedule(model);

    model::SetpointManagerScheduled spm(model,schedule);

    spm.addToNode(supplyOutletNode);

    sizingPlant.setLoopType("Heating");

    sizingPlant.setDesignLoopExitTemperature(82.0);

    sizingPlant.setLoopDesignTemperatureDifference(11.0);
  }
  else
  {
    QDomElement tempCtrlElement = fluidSysElement.firstChildElement("TempCtrl");

    // Fixed
    if( istringEqual(tempCtrlElement.text().toStdString(),"Fixed") )
    {
      QDomElement fixedSupTempElement = fluidSysElement.firstChildElement("FixedSupTemp");

      double fixedSupTemp = 21.1;

      value = fixedSupTempElement.text().toDouble(&ok);

      if( ok )
      {
        fixedSupTemp = unitToUnit(value,"F","C").get();
      }
      else
      {
        QDomElement dsgnSupWtrTempElement = fluidSysElement.firstChildElement("DsgnSupWtrTemp");
        value = dsgnSupWtrTempElement.text().toDouble(&ok);
        if( ok )
        {
          fixedSupTemp = unitToUnit(value,"F","C").get();

          LOG(Warn,plantLoop.name().get() << " Using DsgnSupWtrTemp for fixed supply temperature");
        }
        else
        {
          LOG(Error,plantLoop.name().get() << " Control type is fixed, but a valid temperature was not provided");
        }
      }

      model::ScheduleRuleset schedule(model);

      schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");

      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),fixedSupTemp);

      model::SetpointManagerScheduled spm(model,schedule);

      supplyOutletNode.addSetpointManager(spm);
    }
    else if( istringEqual(tempCtrlElement.text().toStdString(),"Scheduled") )
    {
      QDomElement tempSetPtSchRefElement = fluidSysElement.firstChildElement("TempSetPtSchRef");

      boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(tempSetPtSchRefElement.text().toStdString());

      if( ! schedule )
      {
        LOG(Error,plantLoop.name().get() << " Control type is scheduled, but a valid schedule could not be found.");

        model::ScheduleRuleset schedule(model);

        schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");

        model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

        scheduleDay.addValue(Time(1.0),21.1);
      }

      model::SetpointManagerScheduled spm(model,schedule.get());

      supplyOutletNode.addSetpointManager(spm);
    }
    else if( istringEqual(tempCtrlElement.text().toStdString(),"OutsideAirReset") )
    {
      model::SetpointManagerOutdoorAirReset spm(model);

      supplyOutletNode.addSetpointManager(spm);

      // RstSupHi
      QDomElement rstSupHiElement = fluidSysElement.firstChildElement("RstSupHi");
      value = rstSupHiElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        spm.setSetpointatOutdoorHighTemperature(value);
      }

      // RstSupLo
      QDomElement rstSupLoElement = fluidSysElement.firstChildElement("RstSupLo");
      value = rstSupLoElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        spm.setSetpointatOutdoorLowTemperature(value);
      }

      // RstOutDrHi
      QDomElement rstOutDrHiElement = fluidSysElement.firstChildElement("RstSupHi");
      value = rstOutDrHiElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        spm.setOutdoorHighTemperature(value);
      }

      // RstOutDrLo
      QDomElement rstOutDrLoElement = fluidSysElement.firstChildElement("RstSupLo");
      value = rstOutDrLoElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        spm.setOutdoorLowTemperature(value);
      }
    }
    else if( istringEqual(tempCtrlElement.text().toStdString(),"LoadReset") )
    {
      QDomElement dsgnSupWtrTempElement = fluidSysElement.firstChildElement("DsgnSupWtrTemp");
      value = dsgnSupWtrTempElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();

        model::ScheduleRuleset schedule(model);

        schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");

        model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

        scheduleDay.addValue(Time(1.0),value);

        model::SetpointManagerScheduled spm(model,schedule);

        supplyOutletNode.addSetpointManager(spm);

        LOG(Warn,plantLoop.name().get() << " Using DsgnSupWtrTemp for LoadReset temperature control.  This control scheme is not fully implemented.");
      }
      else
      {
        LOG(Error,plantLoop.name().get() << " no setpoint for LoadReset temperature control.");
      }
    }
    else
    {
      LOG(Error,plantLoop.name().get() << " does not have a setpoint.");
    }
  }

  // DsgnSupWtrTemp
  QDomElement dsgnSupWtrTempElement = fluidSysElement.firstChildElement("DsgnSupWtrTemp");
  value = dsgnSupWtrTempElement.text().toDouble(&ok);
  if( ok )
  {
    value = unitToUnit(value,"F","C").get();

    sizingPlant.setDesignLoopExitTemperature(value);
  }

  // DsgnSupWtrDelT 
  QDomElement dsgnSupWtrDelTElement = fluidSysElement.firstChildElement("DsgnSupWtrDelT");
  value = dsgnSupWtrDelTElement.text().toDouble(&ok);
  if( ok )
  {
    value = value / 1.8;
    sizingPlant.setLoopDesignTemperatureDifference(value);
  }

  if( istringEqual(typeElement.text().toStdString(),"HotWater") )
  {
    sizingPlant.setLoopType("Heating");
  }
  else if( istringEqual(typeElement.text().toStdString(),"ChilledWater") )
  {
    sizingPlant.setLoopType("Cooling");
  }
  else if( istringEqual(typeElement.text().toStdString(),"CondenserWater") )
  {
    sizingPlant.setLoopType("Condenser");
  }

  // Boilers

  QDomNodeList boilerElements = fluidSysElement.elementsByTagName("Blr");

  for (int i = 0; i < boilerElements.count(); i++)
  {
    QDomElement boilerElement = boilerElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateBoiler(boilerElement,doc,model) )
    {
      model::BoilerHotWater boiler = mo->cast<model::BoilerHotWater>();

      plantLoop.addSupplyBranchForComponent(boiler);

      QDomElement pumpElement = boilerElement.firstChildElement("Pump"); 

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo2 = translatePump(pumpElement,doc,model);

        if( mo2 )
        {
          model::Node inletNode = boiler.inletModelObject()->cast<model::Node>();

          if( boost::optional<model::PumpVariableSpeed> pump = mo2->optionalCast<model::PumpVariableSpeed>() )
          {
            pump->addToNode(inletNode);

            LOG(Warn,"Variable speed branch pumps are unsupported");
          }
          else if( boost::optional<model::PumpConstantSpeed> pump = mo2->optionalCast<model::PumpConstantSpeed>() )
          {
            pump->addToNode(inletNode);
          }
        }
      }
    }
  }

  // Chillers

  QDomNodeList chillerElements = fluidSysElement.elementsByTagName("Chlr");

  for (int i = 0; i < chillerElements.count(); i++)
  {
    QDomElement chillerElement = chillerElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateChiller(chillerElement,doc,model) )
    {
      model::ChillerElectricEIR chiller = mo->cast<model::ChillerElectricEIR>();

      plantLoop.addSupplyBranchForComponent(chiller);

      QDomElement pumpElement = chillerElement.firstChildElement("Pump"); 

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo2 = translatePump(pumpElement,doc,model);

        if( mo2 )
        {
          model::Node inletNode = chiller.supplyInletModelObject()->cast<model::Node>();

          if( boost::optional<model::PumpVariableSpeed> pump = mo2->optionalCast<model::PumpVariableSpeed>() )
          {
            pump->addToNode(inletNode);

            LOG(Warn,"Variable speed branch pumps are unsupported");
          }
          else if( boost::optional<model::PumpConstantSpeed> pump = mo2->optionalCast<model::PumpConstantSpeed>() )
          {
            pump->addToNode(inletNode);
          }
        }
      }

      QDomElement evapHasBypassElement = chillerElement.firstChildElement("EvapHasBypass");

      if( evapHasBypassElement.text() == "1" )
      {
        bypass = true;
      }
    }
  }

  // HtRej

  QDomNodeList htRejElements = fluidSysElement.elementsByTagName("HtRej");

  for(int i = 0; i < htRejElements.count(); i++)
  {
    QDomElement htRejElement = htRejElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateHtRej(htRejElement,doc,model) )
    {
      plantLoop.addSupplyBranchForComponent(mo->cast<model::HVACComponent>());
    }
  }

  // Water Heaters

  QDomNodeList wtrHtrElements = fluidSysElement.elementsByTagName("WtrHtr");

  for (int i = 0; i < wtrHtrElements.count(); i++)
  {
    QDomElement wtrHtrElement = wtrHtrElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateWtrHtr(wtrHtrElement,doc,model) )
    {
      plantLoop.addSupplyBranchForComponent(mo->cast<model::HVACComponent>());
    }
  }

  // Add a default bypass

  if(bypass)
  {
    model::PipeAdiabatic pipe(model);

    plantLoop.addSupplyBranchForComponent(pipe);
  }

  // Add a default hot water heater for servicehotwater systems

  if( typeElement.text().toLower() == "servicehotwater" )
  {
    boost::optional<model::WaterHeaterMixed> waterHeater;

    boost::optional<model::Schedule> setpointSchedule;

    std::vector<model::ModelObject> waterHeaters = plantLoop.supplyComponents(model::WaterHeaterMixed::iddObjectType());

    if( waterHeaters.size() > 0 )
    {
      waterHeater = waterHeaters.front().cast<model::WaterHeaterMixed>();
    }

    if( waterHeater )
    {
      setpointSchedule = waterHeater->setpointTemperatureSchedule();
    }

    if( setpointSchedule )
    {
      model::SetpointManagerScheduled spm(model,setpointSchedule.get());

      spm.addToNode(supplyOutletNode);
    }

    if( waterHeaters.size() > 0 )
    {
      model::PumpVariableSpeed pumpVariableSpeed(model);

      model::Node supplyInletNode = plantLoop.supplyInletNode();

      pumpVariableSpeed.addToNode(supplyInletNode);

      pumpVariableSpeed.setRatedPumpHead(0.0);
    }
  }


  QDomNodeList fluidSegElements = fluidSysElement.elementsByTagName("FluidSeg");

  for (int i = 0; i < fluidSegElements.count(); i++)
  {
    QDomElement fluidSegElement = fluidSegElements.at(i).toElement();

    QDomElement typeElement = fluidSegElement.firstChildElement("Type");

    // Translate Secondary Supply
    
    if( typeElement.text().toLower() == "secondarysupply" )
    {
      QDomElement pumpElement = fluidSegElement.firstChildElement("Pump");

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo = translatePump(pumpElement,doc,model);

        if( mo )
        {
          model::Node demandInletNode = plantLoop.demandInletNode();

          mo->cast<model::HVACComponent>().addToNode(demandInletNode);

          plantLoop.setCommonPipeSimulation("CommonPipe");
        }
      }
    }
    
    // Translate Primary Supply
    
    if( typeElement.text().toLower() == "primaryreturn" || 
        typeElement.text().toLower() == "primarysupply"
      )
    {
      QDomElement pumpElement = fluidSegElement.firstChildElement("Pump");

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo = translatePump(pumpElement,doc,model);

        if( mo )
        {
          model::Node supplyInletNode = plantLoop.supplyInletNode();

          mo->cast<model::HVACComponent>().addToNode(supplyInletNode);
        }
      }
    }
  }

  // At this time undo any hard sized CondenserWater system pumps.
  // We don't have full specifications for tower sizing.

  if( istringEqual(typeElement.text().toStdString(),"CondenserWater") )
  {
    std::vector<model::ModelObject> constantPumps;
    constantPumps = plantLoop.supplyComponents(model::PumpConstantSpeed::iddObjectType());

    for(std::vector<model::ModelObject>::iterator it = constantPumps.begin();
        it != constantPumps.end();
        it++)
    {
      it->cast<model::PumpConstantSpeed>().autosizeRatedFlowRate();
      it->cast<model::PumpConstantSpeed>().autosizeRatedPowerConsumption();
    }

    std::vector<model::ModelObject> variablePumps;
    variablePumps = plantLoop.supplyComponents(model::PumpVariableSpeed::iddObjectType());

    for(std::vector<model::ModelObject>::iterator it = variablePumps.begin();
        it != variablePumps.end();
        it++)
    {
      it->cast<model::PumpVariableSpeed>().autosizeRatedFlowRate();
      it->cast<model::PumpVariableSpeed>().autosizeRatedPowerConsumption();
    }
  }

  // Translate PlantLoop::MaximumLoopFlowRate
  if( ! autosize() && (! istringEqual(typeElement.text().toStdString(),"CondenserWater")) )
  {
    std::vector<model::ModelObject> constantPumps;
    constantPumps = plantLoop.supplyComponents(plantLoop.supplyInletNode(),
                                       plantLoop.supplySplitter(),
                                       model::PumpConstantSpeed::iddObjectType());

    std::vector<model::ModelObject> variablePumps;
    variablePumps = plantLoop.supplyComponents(plantLoop.supplyInletNode(),
                                       plantLoop.supplySplitter(),
                                       model::PumpVariableSpeed::iddObjectType());
    if( constantPumps.size() > 0 )
    {
      model::PumpConstantSpeed pump = constantPumps.front().cast<model::PumpConstantSpeed>();
      if( boost::optional<double> value = pump.ratedFlowRate() )
      {
        plantLoop.setMaximumLoopFlowRate(value.get());
      }
    }
    else if( variablePumps.size() > 0 )
    {
      model::PumpVariableSpeed pump = variablePumps.front().cast<model::PumpVariableSpeed>();
      if( boost::optional<double> value = pump.ratedFlowRate() )
      {
        plantLoop.setMaximumLoopFlowRate(value.get());
      }
    }
    else
    {
      constantPumps = plantLoop.supplyComponents(plantLoop.supplySplitter(),
                                         plantLoop.supplyMixer(),
                                         model::PumpConstantSpeed::iddObjectType());
      if( constantPumps.size() > 0 )
      {
        double flowRate = 0.0;
        for( std::vector<model::ModelObject>::iterator it = constantPumps.begin();
             it != constantPumps.end();
             it++ )
        {
          if( boost::optional<double> ratedFlowRate = it->cast<model::PumpConstantSpeed>().ratedFlowRate() )
          {
            flowRate = flowRate + ratedFlowRate.get();
          }
        }
        plantLoop.setMaximumLoopFlowRate(flowRate);
      }
    }
  }

  // AvailSchRef 
  //QDomElement availSchRefElement = fluidSysElement.firstChildElement("AvailSchRef");
  //boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(availSchRefElement.text().toStdString());
  //if( schedule )
  //{
  //  std::vector<model::ModelObject> constantPumps;
  //  constantPumps = plantLoop.supplyComponents(model::PumpConstantSpeed::iddObjectType());
  //  for( std::vector<model::ModelObject>::iterator it = constantPumps.begin();
  //       it != constantPumps.end();
  //       it++ )
  //  {
  //    it->cast<model::PumpConstantSpeed>().setPumpFlowRateSchedule(schedule.get());
  //  }
  //}

  return plantLoop;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translatePump(
                                                  const QDomElement& pumpElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  double value;

  bool ok;

  if( ! istringEqual(pumpElement.tagName().toStdString(),"Pump") )
  {
    return result;
  }

  QDomElement spdCtrlElement = pumpElement.firstChildElement("SpdCtrl");
  
  if( spdCtrlElement.text().toLower() == "constantspeed" )
  {
    model::PumpConstantSpeed pump(model);

    pump.setRatedPumpHead(149453.0);

    if( ! autosize() )
    {
      boost::optional<double> flowCap;
      boost::optional<double> pwr;

      QDomElement flowCapElement = pumpElement.firstChildElement("FlowCap");
      value = flowCapElement.text().toDouble(&ok);
      if( ok )
      {
        flowCap = unitToUnit(value, "gal/min", "m^3/s").get();
      }

      QDomElement pwrElement = pumpElement.firstChildElement("Pwr");
      value = pwrElement.text().toDouble(&ok);
      if( ok )
      {
        // kW to W
        pwr = value * 1000.0;
      }

      if( flowCap && pwr )
      {
        if( equal(flowCap.get(),0.0) || equal(pwr.get(),0.0)  )
        {
          LOG(Warn,pump.name().get() << " has 0 capacity specified.");
        }
        else
        {
          pump.setRatedFlowRate(flowCap.get());
          pump.setRatedPowerConsumption(pwr.get());
          pump.setRatedPumpHead(0.80 * pwr.get() / flowCap.get());
        }
      }
    }

    result = pump;
  }
  else
  {
    model::PumpVariableSpeed pump(model);

    pump.setRatedPumpHead(149453.0);

    if( ! autosize() )
    {
      boost::optional<double> flowCap;
      boost::optional<double> flowMin;
      boost::optional<double> pwr;

      QDomElement flowCapElement = pumpElement.firstChildElement("FlowCap");
      value = flowCapElement.text().toDouble(&ok);
      if( ok )
      {
        flowCap = unitToUnit(value, "gal/min", "m^3/s");
      }

      QDomElement flowMinElement = pumpElement.firstChildElement("FlowMin");
      value = flowCapElement.text().toDouble(&ok);
      if( ok )
      {
        flowMin = unitToUnit(value, "gal/min", "m^3/s");
      }

      QDomElement pwrElement = pumpElement.firstChildElement("Pwr");
      value = pwrElement.text().toDouble(&ok);
      if( ok )
      {
        // kW to W
        pwr = value * 1000.0;
      }

      if( flowMin && flowCap && pwr )
      {
        if( equal(flowCap.get(),0.0) || equal(pwr.get(),0.0)  )
        {
          LOG(Warn,pump.name().get() << " has 0 capacity specified.");
        }
        else
        {
          pump.setRatedFlowRate(flowCap.get());
          pump.setMinimumFlowRate(flowMin.get());
          pump.setRatedPowerConsumption(pwr.get());
          pump.setRatedPumpHead(0.80 * pwr.get() / flowCap.get());
        }
      }
    }

    result = pump;
  }

  if( result )
  {
    QDomElement nameElement = pumpElement.firstChildElement("Name");

    result->setName(nameElement.text().toStdString());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateBoiler(
                                                  const QDomElement& boilerElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(boilerElement.tagName().toStdString(),"Blr") )
  {
    return result;
  }

  model::BoilerHotWater boiler(model);

  // Name

  QDomElement nameElement = boilerElement.firstChildElement("Name");

  boiler.setName(nameElement.text().toStdString());

  // ThrmlEff

  QDomElement thrmlEffElement = boilerElement.firstChildElement("ThrmlEff");

  boiler.setNominalThermalEfficiency(thrmlEffElement.text().toDouble());

  // HIR_fPLRCrvRef

  boost::optional<model::Curve> hirfPLRCrv;
  QDomElement hirfPLRCrvRefElement = boilerElement.firstChildElement("HIR_fPLRCrvRef");
  hirfPLRCrv = model.getModelObjectByName<model::Curve>(hirfPLRCrvRefElement.text().toStdString());
  if( hirfPLRCrv )
  {
    boiler.setNormalizedBoilerEfficiencyCurve(hirfPLRCrv.get());
  }

  // FuelSrc

  QDomElement fuelSrcElement = boilerElement.firstChildElement("FuelSrc");
  QString fuelSrc = fuelSrcElement.text().toLower();

  if( fuelSrc == "electric" )
  {
    boiler.setFuelType("Electricity");
  }
  else if ( fuelSrc == "gas" )
  {
    boiler.setFuelType("NaturalGas");
  }
  else if( fuelSrc == "oil" )
  {
    boiler.setFuelType("FuelOil#2");
  }

  // ParasiticLd
  
  QDomElement parasiticLdElement = boilerElement.firstChildElement("ParasiticLd");

  bool ok;

  double parasiticLd = parasiticLdElement.text().toDouble(&ok);

  if( ok )
  {
    boiler.setParasiticElectricLoad(unitToUnit(parasiticLd,"Btu/h","W").get());
  }

  double value;

  if( ! autosize() )
  {
    // CapRtd
    boost::optional<double> capRtd;
    QDomElement capRtdElement = boilerElement.firstChildElement("CapRtd");
    value = capRtdElement.text().toDouble(&ok);
    if( ok )
    {
      capRtd = unitToUnit(value,"Btu/h","W");
    }

    // EntTempDsgn
    boost::optional<double> entTempDsgn;
    QDomElement entTempDsgnElement = boilerElement.firstChildElement("EntTempDsgn");
    value = entTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      entTempDsgn = unitToUnit(value,"F","C");
    }

    // LvgTempDsgn
    boost::optional<double> lvgTempDsgn;
    QDomElement lvgTempDsgnElement = boilerElement.firstChildElement("LvgTempDsgn");
    value = lvgTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      lvgTempDsgn = unitToUnit(value,"F","C");
    }

    if( capRtd && entTempDsgn && lvgTempDsgn )
    {
      boiler.setNominalCapacity(capRtd.get());

      double flow = capRtd.get() / ( cpWater * densityWater * (lvgTempDsgn.get() - entTempDsgn.get()));

      boiler.setDesignWaterFlowRate(flow);
    }
  }

  return boiler;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHtRej(
                                                  const QDomElement& htRejElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(htRejElement.tagName().toStdString(),"HtRej") )
  {
    return result;
  }

  model::CoolingTowerSingleSpeed tower(model);

  // Name

  QDomElement nameElement = htRejElement.firstChildElement("Name");
  
  tower.setName(nameElement.text().toStdString());

  return tower;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateChiller(
                                                  const QDomElement& chillerElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(chillerElement.tagName().toStdString(),"Chlr") )
  {
    return result;
  }

  QDomElement nameElement = chillerElement.firstChildElement("Name");

  // Cap_fTempCrvRef

  boost::optional<model::CurveBiquadratic> cap_fTempCrv;
  QDomElement cap_fTempCrvElement = chillerElement.firstChildElement("Cap_fTempCrvRef");
  cap_fTempCrv = model.getModelObjectByName<model::CurveBiquadratic>(cap_fTempCrvElement.text().toStdString());
  if( ! cap_fTempCrv )
  {
    LOG(Error,"Coil: " << nameElement.text().toStdString() << " Broken Cap_fTempCrv");

    cap_fTempCrv = model::CurveBiquadratic(model);
    cap_fTempCrv->setCoefficient1Constant(1.0215158);
    cap_fTempCrv->setCoefficient2x(0.037035864);
    cap_fTempCrv->setCoefficient3xPOW2(0.0002332476);
    cap_fTempCrv->setCoefficient4y(-0.003894048);
    cap_fTempCrv->setCoefficient5yPOW2(-6.52536e-005);
    cap_fTempCrv->setCoefficient6xTIMESY(-0.0002680452);
    cap_fTempCrv->setMinimumValueofx(5.0);
    cap_fTempCrv->setMaximumValueofx(10.0);
    cap_fTempCrv->setMinimumValueofy(24.0);
    cap_fTempCrv->setMaximumValueofy(35.0);
  }

  // EIR_fTempCrvRef

  boost::optional<model::CurveBiquadratic> eir_fTempCrv;
  QDomElement eir_fTempCrvElement = chillerElement.firstChildElement("EIR_fTempCrvRef");
  eir_fTempCrv = model.getModelObjectByName<model::CurveBiquadratic>(eir_fTempCrvElement.text().toStdString());
  if( ! eir_fTempCrv )
  {
    LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken EIR_fTempCrvRef");

    eir_fTempCrv = model::CurveBiquadratic(model);
    eir_fTempCrv->setCoefficient1Constant(0.70176857);
    eir_fTempCrv->setCoefficient2x(-0.00452016);
    eir_fTempCrv->setCoefficient3xPOW2(0.0005331096);
    eir_fTempCrv->setCoefficient4y(-0.005498208);
    eir_fTempCrv->setCoefficient5yPOW2(0.0005445792);
    eir_fTempCrv->setCoefficient6xTIMESY(-0.0007290324);
    eir_fTempCrv->setMinimumValueofx(5.0);
    eir_fTempCrv->setMaximumValueofx(10.0);
    eir_fTempCrv->setMinimumValueofy(24.0);
    eir_fTempCrv->setMaximumValueofy(35.0);
  }

  // EIR_fPLRCrvRef

  boost::optional<model::CurveQuadratic> eir_fPLRCrv;
  QDomElement eir_fPLRCrvElement = chillerElement.firstChildElement("EIR_fPLRCrvRef");
  eir_fPLRCrv = model.getModelObjectByName<model::CurveQuadratic>(eir_fPLRCrvElement.text().toStdString());
  if( ! eir_fPLRCrv )
  {
    LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken EIR_fPLRCrvRef");

    eir_fPLRCrv = model::CurveQuadratic(model);
    eir_fPLRCrv->setCoefficient1Constant(0.06369119);
    eir_fPLRCrv->setCoefficient2x(0.58488832);
    eir_fPLRCrv->setCoefficient3xPOW2(0.35280274);
    eir_fPLRCrv->setMinimumValueofx(0.0);
    eir_fPLRCrv->setMaximumValueofx(1.0);
  }

  model::ChillerElectricEIR chiller(model,cap_fTempCrv.get(),eir_fTempCrv.get(),eir_fPLRCrv.get());

  // Name

  chiller.setName(nameElement.text().toStdString());

  // CndsrInRef

  QDomElement cndsrInRefElement = chillerElement.firstChildElement("CndsrFluidSegInRef");

  boost::optional<model::PlantLoop> condenserSystem = loopForSupplySegment(cndsrInRefElement.text(),doc,model);

  if( condenserSystem )
  {
    condenserSystem->addDemandBranchForComponent(chiller);
  }

  double value;
  bool ok;

  // COP
  boost::optional<double> cop;
  QDomElement copElement = chillerElement.firstChildElement("COP");
  value = copElement.text().toDouble(&ok);
  if( ok )
  {
    chiller.setReferenceCOP(value);
    cop = value;
  }

  // PartLdRatMin
  QDomElement partLdRatMinElement = chillerElement.firstChildElement("PartLdRatMin");
  value = partLdRatMinElement.text().toDouble(&ok);
  if( ok )
  {
    chiller.setMinimumPartLoadRatio(value);
  }

  // UnldRatMin
  QDomElement unldRatMinElement = chillerElement.firstChildElement("UnldRatMin");
  value = unldRatMinElement.text().toDouble(&ok);
  if( ok )
  {
    chiller.setMinimumUnloadingRatio(value);
  }

  if( ! autosize() )
  {
    // CapRtd
    boost::optional<double> capRtd;
    QDomElement capRtdElement = chillerElement.firstChildElement("CapRtd");
    value = capRtdElement.text().toDouble(&ok);
    if( ok )
    {
      capRtd = unitToUnit(value,"Btu/h","W");
    }

    // EntTempDsgn
    boost::optional<double> entTempDsgn;
    QDomElement entTempDsgnElement = chillerElement.firstChildElement("EntTempDsgn");
    value = entTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      entTempDsgn = unitToUnit(value,"F","C");
    }

    // LvgTempDsgn
    boost::optional<double> lvgTempDsgn;
    QDomElement lvgTempDsgnElement = chillerElement.firstChildElement("LvgTempDsgn");
    value = lvgTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      lvgTempDsgn = unitToUnit(value,"F","C");
    }

    if( capRtd && entTempDsgn && lvgTempDsgn && cop )
    {
      chiller.setReferenceCapacity(capRtd.get());

      double flow = capRtd.get() / ( cpWater * densityWater * (entTempDsgn.get() - lvgTempDsgn.get()));

      chiller.setReferenceChilledWaterFlowRate(flow);

      double condFlow = capRtd.get() * (1.0 + 1.0 / cop.get()) / ( cpWater * densityWater * (entTempDsgn.get() - lvgTempDsgn.get()));

      chiller.setReferenceCondenserFluidFlowRate(condFlow);
    }
  }

  return chiller;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWtrHtr(
                                                  const QDomElement& element, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;
  bool ok;

  if( ! istringEqual(element.tagName().toStdString(),"WtrHtr") )
  {
    return result;
  }

  model::WaterHeaterMixed waterHeaterMixed(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  waterHeaterMixed.setName(nameElement.text().toStdString());

  // ThrmlEff

  QDomElement thrmlEffElement = element.firstChildElement("ThrmlEff");
  
  double thrmlEff = thrmlEffElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setHeaterThermalEfficiency(thrmlEff);
  }

  // Ambient water temperature indicator

  waterHeaterMixed.setAmbientTemperatureIndicator("Schedule");

  model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

  scheduleRuleset.setName(QString(nameElement.text() + " Ambient Temperature").toStdString());

  model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

  scheduleDay.addValue(Time(1.0),20.0);

  waterHeaterMixed.setAmbientTemperatureSchedule(scheduleRuleset);

  // WtrHtrStorCap

  QDomElement wtrHtrStorCapElement = element.firstChildElement("WtrHtrStorCap");

  double wtrHtrStorCap = wtrHtrStorCapElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setTankVolume(unitToUnit(wtrHtrStorCap,"gal","m^3").get());
  }
  //else
  //{
  //  waterHeaterMixed.autosizeTankVolume();
  //}

  // WtrHtrMaxCap
  
  QDomElement wtrHtrMaxCapElement = element.firstChildElement("WtrHtrMaxCap");

  double wtrHtrMaxCap = wtrHtrMaxCapElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setHeaterMaximumCapacity(unitToUnit(wtrHtrMaxCap,"Btu/h","W").get());
  }
  //else
  //{
  //  waterHeaterMixed.autosizeHeaterMaximumCapacity();
  //}

  // MinCap

  QDomElement minCapElement = element.firstChildElement("MinCap");

  double minCap = minCapElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setHeaterMinimumCapacity(unitToUnit(minCap,"Btu/h","W").get());
  }

  model::Schedule setpointTempSchedule = serviceHotWaterSetpointSchedule(model);

  waterHeaterMixed.setSetpointTemperatureSchedule(setpointTempSchedule);

  return waterHeaterMixed;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateZnSys(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"ZnSys") )
  {
    return boost::none;
  }

  // Name
  std::string name = element.firstChildElement("Name").text().toStdString();

  // Type
  std::string type = element.firstChildElement("Type").text().toStdString();

  // Availability Schedule
  QDomElement scheduleElement = element.firstChildElement("AvailSchRef");

  boost::optional<model::Schedule> schedule;

  if( ! scheduleElement.isNull() )
  {
    schedule = model.getModelObjectByName<model::Schedule>(scheduleElement.text().toStdString()); 
  }

  if( ! schedule )
  {
    schedule = model.alwaysOnDiscreteSchedule();
  }

  // FlowCap

  QDomElement fanElement = element.firstChildElement("Fan"); 

  QDomElement flowCapElement = fanElement.firstChildElement("FlowCap");

  boost::optional<double> flowCap;

  if( ! autosize() )
  {
    bool ok;

    double value = flowCapElement.text().toDouble(&ok);

    if( ok ) 
    {
      Quantity flowRateIP(value,createCFMVolumetricFlowrate());
      OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(flowRateSI);
      flowCap = flowRateSI->value();
    }
  }

  if( istringEqual(type,"PTAC") )
  {
    boost::optional<openstudio::model::ModelObject> mo;

    // Fan
    
    QDomElement fanElement = element.firstChildElement("Fan"); 

    mo = translateFan(fanElement,doc,model);    

    OS_ASSERT(mo);

    model::HVACComponent fan = mo->cast<model::HVACComponent>();
 
    // Heating Coil

    QDomElement heatingCoilElement = element.firstChildElement("CoilHtg"); 
    
    mo = translateCoilHeating(heatingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent heatingCoil = mo->cast<model::HVACComponent>();

    // Cooling Coil

    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");

    mo = translateCoilCooling(coolingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    // PTAC

    model::ZoneHVACPackagedTerminalAirConditioner ptac(model,schedule.get(),fan,heatingCoil,coolingCoil);

    if( flowCap )
    {
      ptac.setSupplyAirFlowRateDuringCoolingOperation(flowCap.get());
      ptac.setSupplyAirFlowRateDuringHeatingOperation(flowCap.get());
      ptac.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(flowCap.get());
    }

    // Name

    ptac.setName(name);

    return ptac;
  }
  else if( istringEqual(type,"PTHP") )
  {
    boost::optional<openstudio::model::ModelObject> mo;

    QDomElement htPumpSuppTempElement;

    // Fan
    
    QDomElement fanElement = element.firstChildElement("Fan"); 

    mo = translateFan(fanElement,doc,model);    

    OS_ASSERT(mo);

    model::HVACComponent fan = mo->cast<model::HVACComponent>();
 
    // Heating Coil

    boost::optional<model::HVACComponent> heatingCoil;

    boost::optional<model::HVACComponent> suppHeatingCoil;

    QDomNodeList heatingCoilElements = element.elementsByTagName("CoilHtg"); 

    for(int i = 0; i < heatingCoilElements.count(); i++)
    {
      QDomElement heatingCoilElement = heatingCoilElements.at(i).toElement();

      QDomElement typeElement = heatingCoilElement.firstChildElement("Type");

      mo = translateCoilHeating(heatingCoilElement,doc,model);

      OS_ASSERT(mo);

      if(istringEqual(typeElement.text().toStdString(),"HeatPump"))
      {
        heatingCoil = mo->cast<model::HVACComponent>();

        htPumpSuppTempElement = heatingCoilElement.firstChildElement("HtPumpSuppTemp");
      }
      else
      {
        suppHeatingCoil = mo->cast<model::HVACComponent>();
      }

      if( i > 1 )
      {
        LOG(Warn,name << " has unused heating coils defined.");
      }
    }

    // Cooling Coil

    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");

    mo = translateCoilCooling(coolingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    // Default Supplemental Heating Coil if one is not provided
    
    if( ! suppHeatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();

      suppHeatingCoil = model::CoilHeatingElectric(model,alwaysOnDiscreteSchedule);

      LOG(Warn,name << " creating a default supplementary heating coil");
    }

    // Default Heating Coil if one is not provided
    
    if( ! heatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();

      model::CurveCubic totalHeatingCapacityFunctionofTemperatureCurve(model);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
      totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      model::CurveCubic totalHeatingCapacityFunctionofFlowFractionCurve(model);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
      totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
      totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

      model::CurveCubic energyInputRatioFunctionofTemperatureCurve(model);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
      energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      model::CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
      energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
      energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

      model::CurveQuadratic partLoadFractionCorrelationCurve(model);
      partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
      partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
      partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
      partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
      partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

      suppHeatingCoil = model::CoilHeatingDXSingleSpeed( model,
                               alwaysOnDiscreteSchedule,
                               totalHeatingCapacityFunctionofTemperatureCurve,
                               totalHeatingCapacityFunctionofFlowFractionCurve,
                               energyInputRatioFunctionofTemperatureCurve,
                               energyInputRatioFunctionofFlowFractionCurve,
                               partLoadFractionCorrelationCurve ); 

      LOG(Warn,name << " creating a default heating coil");
    }

    // PTHP

    model::ZoneHVACPackagedTerminalHeatPump pthp(model,
                                                 schedule.get(),
                                                 fan,
                                                 heatingCoil.get(),
                                                 coolingCoil,
                                                 suppHeatingCoil.get());

    if( flowCap )
    {
      pthp.setSupplyAirFlowRateDuringCoolingOperation(flowCap.get());
      pthp.setSupplyAirFlowRateDuringHeatingOperation(flowCap.get());
      pthp.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(flowCap.get());
    }

    // Name

    pthp.setName(name);

    // HtPumpSuppTemp

    bool ok;

    double value = htPumpSuppTempElement.text().toDouble(&ok);

    if( ok )
    {
      value = unitToUnit(value,"F","C").get();

      pthp.setMaximumSupplyAirTemperaturefromSupplementalHeater(value);
    }

    return pthp;
  }
  else if( istringEqual(type,"Baseboard") )
  {
    // Heating Coil
    QDomElement heatingCoilElement = element.firstChildElement("CoilHtg"); 
    
    // Heating Coil Type
    std::string htgCoilType = heatingCoilElement.firstChildElement("Type").text().toStdString();

    if ( istringEqual(htgCoilType,"Resistance") )
    {
      // ZoneHVAC Baseboard Convective Electric
      model::ZoneHVACBaseboardConvectiveElectric baseboard(model);
      
      // Nominal Capacity
      boost::optional<double> nomHtgCap;
      
      if( ! autosize() )
      {
        bool ok;

        double value = heatingCoilElement.firstChildElement("CapTotGrossRtd").text().toDouble(&ok);

        if( ok ) 
        {
          nomHtgCap = unitToUnit(value, "Btu/hr", "W");
        }
      }
     
      //set the nominal capacity of the baseboard unit
      if ( nomHtgCap )
      {
        baseboard.setNominalCapacity(nomHtgCap.get());
      }

      // Name

      baseboard.setName(name);

      return baseboard;
    }
    else if (istringEqual(htgCoilType, "HotWater") )
    {
      // Coil Heating Water Baseboard (special coil type for baseboard only)
      model::CoilHeatingWaterBaseboard coil(model);

      // Autosize the max flow rate and the u-factor times area value
      coil.autosizeMaximumWaterFlowRate();
      coil.autosizeUFactorTimesAreaValue();

      // Maximum Water Flow Rate (units are gpm in the sdd file)
      boost::optional<double> dsnHtgFlowRt;

      if( ! autosize() )
      {
        bool ok;

        double value = heatingCoilElement.firstChildElement("FluidFlowRtDsgn").text().toDouble(&ok);

        if( ok ) 
        {
          dsnHtgFlowRt = unitToUnit(value, "gal/min", "m^3/s");
        }
      }
     
      if( dsnHtgFlowRt )
      {
        coil.setMaximumWaterFlowRate(*dsnHtgFlowRt);
      }

      //Hook Coil to PlantLoop
      QDomElement fluidSegInRefElement = heatingCoilElement.firstChildElement("FluidSegInRef");

      if( boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement.text(),doc,model) )
      {
        plant->addDemandBranchForComponent(coil);
      }

      // ZoneHVAC Baseboard Convective Electric
      model::ZoneHVACBaseboardConvectiveWater baseboard(model, schedule.get(), coil);

      // Name

      baseboard.setName(name);

      return baseboard;
    }

    return boost::none;
  }
  else if( istringEqual(type,"FPFC") )
  {
    boost::optional<openstudio::model::ModelObject> mo;

    // Fan
    
    QDomElement fanElement = element.firstChildElement("Fan"); 

    mo = translateFan(fanElement,doc,model);    

    OS_ASSERT(mo);

    model::HVACComponent fan = mo->cast<model::HVACComponent>();
 
    // Heating Coil

    QDomElement heatingCoilElement = element.firstChildElement("CoilHtg"); 
    
    mo = translateCoilHeating(heatingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent heatingCoil = mo->cast<model::HVACComponent>();

    // get the heating coil's max flow rate (units are gpm in the sdd file)
    QDomElement dsnHtgFlowRtElement = heatingCoilElement.firstChildElement("FluidFlowRtDsgn");

    boost::optional<double> dsnHtgFlowRt;

    if( ! autosize() )
    {
      bool ok;

      double value = dsnHtgFlowRtElement.text().toDouble(&ok);

      if( ok ) 
      {
        dsnHtgFlowRt = unitToUnit(value, "gal/min", "m^3/s");
      }
    }
    
    // Cooling Coil

    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");

    mo = translateCoilCooling(coolingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    // get the cooling coil's max flow rate (units are gpm in the sdd file)
    QDomElement dsnClgFlowRtElement = coolingCoilElement.firstChildElement("FluidFlowRtDsgn");

    boost::optional<double> dsnClgFlowRt;

    if( ! autosize() )
    {
      bool ok;

      double value = dsnClgFlowRtElement.text().toDouble(&ok);

      if( ok ) 
      {
        dsnClgFlowRt = unitToUnit(value, "gal/min", "m^3/s");
      }
    }
    
    // FPFC (Four Pipe Fan Coil)
    model::ZoneHVACFourPipeFanCoil fpfc(model,schedule.get(),fan,coolingCoil,heatingCoil);

    // pull the max air flow rate from the fan and set in the fpfc
    if( flowCap )
    {
      fpfc.setMaximumSupplyAirFlowRate(flowCap.get());
    }

    // zero out the max OA flowrate
    fpfc.setMaximumOutdoorAirFlowRate(0.0);


    //set the max heating water flow rate
    if( dsnHtgFlowRt )
    {
      fpfc.setMaximumHotWaterFlowRate(*dsnHtgFlowRt);
    }

    //set the max cooling water flow rate
    if( dsnClgFlowRt )
    {
      fpfc.setMaximumColdWaterFlowRate(dsnClgFlowRt.get());
    }

    // Name

    fpfc.setName(name);

    return fpfc;

  }

  return boost::none;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvDblQuad(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"CrvDblQuad") )
  {
    return boost::none;
  }

  model::CurveBiquadratic curve(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  curve.setName(nameElement.text().toStdString());

  // Coef1 

  QDomElement coef1Element = element.firstChildElement("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().toDouble());

  // Coef2

  QDomElement coef2Element = element.firstChildElement("Coef2");

  curve.setCoefficient2x(coef2Element.text().toDouble());

  // Coef3

  QDomElement coef3Element = element.firstChildElement("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().toDouble());

  // Coef4

  QDomElement coef4Element = element.firstChildElement("Coef4");

  curve.setCoefficient4y(coef4Element.text().toDouble());

  // Coef5

  QDomElement coef5Element = element.firstChildElement("Coef5");

  curve.setCoefficient5yPOW2(coef5Element.text().toDouble());

  // Coef6

  QDomElement coef6Element = element.firstChildElement("Coef6");

  curve.setCoefficient6xTIMESY(coef6Element.text().toDouble());

  // MinVar1
  
  QDomElement minVar1Element = element.firstChildElement("MinVar1");

  bool ok;

  double value = minVar1Element.text().toDouble(&ok); 

  if(ok)
  {
    curve.setMinimumValueofx(value);
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Minimum Limit");
  }

  // MaxVar1
  
  QDomElement maxVar1Element = element.firstChildElement("MaxVar1");

  value = maxVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofx(value);
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Maximum Limit");
  }

  // MinVar2
  
  QDomElement minVar2Element = element.firstChildElement("MinVar2");

  value = minVar2Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMinimumValueofy(value);
  }
  else
  {
    curve.setMinimumValueofy(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing Y Minimum Limit");
  }

  // MaxVar2
  
  QDomElement maxVar2Element = element.firstChildElement("MaxVar2");

  value = maxVar2Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofy(value);
  }
  else
  {
    curve.setMaximumValueofy(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing Y Maximum Limit");
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvCubic(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"CrvCubic") )
  {
    return boost::none;
  }

  model::CurveCubic curve(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  curve.setName(nameElement.text().toStdString());

  // Coef1

  QDomElement coef1Element = element.firstChildElement("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().toDouble());

  // Coef2

  QDomElement coef2Element = element.firstChildElement("Coef2");

  curve.setCoefficient2x(coef2Element.text().toDouble());

  // Coef3

  QDomElement coef3Element = element.firstChildElement("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().toDouble());

  // Coef4

  QDomElement coef4Element = element.firstChildElement("Coef4");

  curve.setCoefficient4xPOW3(coef4Element.text().toDouble());

  // MinVar1
  
  QDomElement minVar1Element = element.firstChildElement("MinVar1");

  bool ok;

  double value = minVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMinimumValueofx(value);
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Minimum Limit");
  }

  // MaxVar1
  
  QDomElement maxVar1Element = element.firstChildElement("MaxVar1");

  value = maxVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofx(value);
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Maximum Limit");
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvQuad(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"CrvQuad") )
  {
    return boost::none;
  }

  model::CurveQuadratic curve(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  curve.setName(nameElement.text().toStdString());

  // Coef1

  QDomElement coef1Element = element.firstChildElement("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().toDouble());

  // Coef2

  QDomElement coef2Element = element.firstChildElement("Coef2");

  curve.setCoefficient2x(coef2Element.text().toDouble());

  // Coef3

  QDomElement coef3Element = element.firstChildElement("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().toDouble());

  // MinVar1
  
  QDomElement minVar1Element = element.firstChildElement("MinVar1");

  bool ok;

  double value = minVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMinimumValueofx(value);
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Minimum Limit");
  }

  // MaxVar1
  
  QDomElement maxVar1Element = element.firstChildElement("MaxVar1");

  value = maxVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofx(value);
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Maximum Limit");
  }

  return curve;
}

QDomElement ReverseTranslator::findZnSysElement(const QString & znSysName,const QDomDocument & doc)
{
  QDomNodeList znSysElements = doc.documentElement().firstChildElement("Proj").elementsByTagName("ZnSys");

  for (int i = 0; i < znSysElements.count(); i++)
  {
    QDomElement znSysElement = znSysElements.at(i).toElement();

    QDomElement znSysNameElement = znSysElement.firstChildElement("Name");

    if( znSysNameElement.text() == znSysName )
    {
      return znSysElement;
    }
  }

  return QDomElement();
}

} // sdd
} // openstudio

