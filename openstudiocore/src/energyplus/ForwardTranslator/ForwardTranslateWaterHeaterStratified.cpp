/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/WaterHeater_Stratified_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateWaterHeaterStratified( WaterHeaterStratified & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<Schedule> schedule;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::WaterHeater_Stratified, modelObject);

  // End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::EndUseSubcategory, s.get());
  }

  // Tank Volume
  if( modelObject.isTankVolumeAutosized() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::TankVolume, "Autosize");
  }
  else
  {
    value = modelObject.tankVolume();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::TankVolume, value.get());
    }
  }

  // Tank Height
  if( modelObject.isTankHeightAutosized() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::TankHeight, "Autosize");
  }
  else
  {
    value = modelObject.tankHeight();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::TankHeight, value.get());
    }
  }

  // Tank Shape
  s = modelObject.tankShape();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::TankShape, s.get());
  }

  // Tank Perimeter
  value = modelObject.tankPerimeter();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::TankPerimeter, value.get());
  }

  // Maximum Temperature Limit
  value = modelObject.maximumTemperatureLimit();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::MaximumTemperatureLimit, value.get());
  }

  // Heater Priority Control
  s = modelObject.heaterPriorityControl();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::HeaterPriorityControl, s.get());
  }

  // Heater 1 Setpoint Temperature Schedule Name
  if( ( schedule = modelObject.heater1SetpointTemperatureSchedule() ) )
  {
    auto _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(WaterHeater_StratifiedFields::Heater1SetpointTemperatureScheduleName, _schedule->name().get());
    }
  }

  // Heater 1 Deadband Temperature Difference
  value = modelObject.heater1DeadbandTemperatureDifference();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Heater1DeadbandTemperatureDifference, value.get());
  }

  // Heater 1 Capacity
  if( modelObject.isHeater1CapacityAutosized() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::Heater1Capacity, "Autosize");
  }
  else
  {
    value = modelObject.heater1Capacity();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::Heater1Capacity, value.get());
    }
  }

  // Heater 1 Height
  value = modelObject.heater1Height();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Heater1Height, value.get());
  }

  // Heater 2 Setpoint Temperature Schedule Name
  if( ( schedule = modelObject.heater2SetpointTemperatureSchedule() ) )
  {
    auto _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(WaterHeater_StratifiedFields::Heater2SetpointTemperatureScheduleName, _schedule->name().get());
    }
  }

  // Heater 2 Deadband Temperature Difference
  value = modelObject.heater2DeadbandTemperatureDifference();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Heater2DeadbandTemperatureDifference, value.get());
  }

  // Heater 2 Capacity
  value = modelObject.heater2Capacity();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Heater2Capacity, value.get());
  }

  // Heater 2 Height
  value = modelObject.heater2Height();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Heater2Height, value.get());
  }

  // Heater Fuel Type
  s = modelObject.heaterFuelType();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::HeaterFuelType, s.get());
  }

  // Heater Thermal Efficiency
  value = modelObject.heaterThermalEfficiency();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::HeaterThermalEfficiency, value.get());
  }

  // Off Cycle Parasitic Fuel Consumption Rate
  value = modelObject.offCycleParasiticFuelConsumptionRate();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OffCycleParasiticFuelConsumptionRate, value.get());
  }

  // Off Cycle Parasitic Fuel Type
  s = modelObject.offCycleParasiticFuelType();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::OffCycleParasiticFuelType, s.get());
  }

  // Off Cycle Parasitic Heat Fraction to Tank
  value = modelObject.offCycleParasiticHeatFractiontoTank();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank, value.get());
  }

  // Off Cycle Parasitic Height
  value = modelObject.offCycleParasiticHeight();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OffCycleParasiticHeight, value.get());
  }

  // On Cycle Parasitic Fuel Consumption Rate
  value = modelObject.onCycleParasiticFuelConsumptionRate();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate, value.get());
  }

  // On Cycle Parasitic Fuel Type
  s = modelObject.onCycleParasiticFuelType();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::OnCycleParasiticFuelType, s.get());
  }

  // On Cycle Parasitic Heat Fraction to Tank
  value = modelObject.onCycleParasiticHeatFractiontoTank();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank, value.get());
  }

  // On Cycle Parasitic Height
  value = modelObject.onCycleParasiticHeight();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OnCycleParasiticHeight, value.get());
  }

  // Ambient Temperature Indicator
  s = modelObject.ambientTemperatureIndicator();
  if( s )
  {
    if( istringEqual(s.get(),"ThermalZone") )
    {
      idfObject.setString(WaterHeater_StratifiedFields::AmbientTemperatureIndicator,"Zone");
    }
    else
    {
      idfObject.setString(WaterHeater_StratifiedFields::AmbientTemperatureIndicator,s.get());
    }
  }

  // Ambient Temperature Schedule Name
  schedule = modelObject.ambientTemperatureSchedule();
  if( schedule )
  {
    auto _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(WaterHeater_StratifiedFields::AmbientTemperatureScheduleName,_schedule->name().get());
    }
  }

  // Ambient Temperature Zone Name
  if( auto zone = modelObject.ambientTemperatureThermalZone() )
  {
    auto _zone = translateAndMapModelObject(zone.get());

    if( _zone )
    {
      idfObject.setString(WaterHeater_StratifiedFields::AmbientTemperatureZoneName,_zone->name().get());
    }
  }

  // Ambient Temperature Outdoor Air Node Name
  s = modelObject.ambientTemperatureOutdoorAirNodeName();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName,s.get());
  }

  // Uniform Skin Loss Coefficient per Unit Area to Ambient Temperature
  value = modelObject.uniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature, value.get());
  }

  // Skin Loss Fraction to Zone
  value = modelObject.skinLossFractiontoZone();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::SkinLossFractiontoZone, value.get());
  }

  // Off Cycle Flue Loss Coefficient to Ambient Temperature
  value = modelObject.offCycleFlueLossCoefficienttoAmbientTemperature();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OffCycleFlueLossCoefficienttoAmbientTemperature, value.get());
  }

  // Off Cycle Flue Loss Fraction to Zone
  value = modelObject.offCycleFlueLossFractiontoZone();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::OffCycleFlueLossFractiontoZone, value.get());
  }

  // Peak Use Flow Rate
  value = modelObject.peakUseFlowRate();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::PeakUseFlowRate, value.get());
  }

  // Use Flow Rate Fraction Schedule Name
  schedule = modelObject.useFlowRateFractionSchedule();
  if( schedule )
  {
    auto _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(WaterHeater_StratifiedFields::UseFlowRateFractionScheduleName,_schedule->name().get());
    }
  }

  // Cold Water Supply Temperature Schedule Name
  schedule = modelObject.coldWaterSupplyTemperatureSchedule();
  if( schedule )
  {
    auto _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(WaterHeater_StratifiedFields::ColdWaterSupplyTemperatureScheduleName,_schedule->name().get());
    }
  }

  // Use Side Inlet Node Name
  if( auto mo = modelObject.supplyInletModelObject() )
  {
    if( auto node = mo->optionalCast<Node>() )
    {
      translateAndMapModelObject(node.get());

      idfObject.setString(WaterHeater_StratifiedFields::UseSideInletNodeName,node->name().get());
    }
  }

  // Use Side Outlet Node Name
  if( auto mo = modelObject.supplyOutletModelObject() )
  {
    if( auto node = mo->optionalCast<Node>() )
    {
      translateAndMapModelObject(node.get());

      idfObject.setString(WaterHeater_StratifiedFields::UseSideOutletNodeName,node->name().get());
    }
  }

  // Use Side Effectiveness
  value = modelObject.useSideEffectiveness();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::UseSideEffectiveness,value.get());
  }

  // Use Side Inlet Height
  value = modelObject.useSideInletHeight();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::UseSideInletHeight,value.get());
  }

  // Use Side Outlet Height
  if( modelObject.isUseSideOutletHeightAutocalculated() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::UseSideOutletHeight, "Autocalculate");
  }
  else
  {
    value = modelObject.useSideOutletHeight();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::UseSideOutletHeight, value.get());
    }
  }

  // Source Side Inlet Node Name
  if( auto mo = modelObject.demandInletModelObject() )
  {
    if( auto node = mo->optionalCast<Node>() )
    {
      translateAndMapModelObject(node.get());

      idfObject.setString(WaterHeater_StratifiedFields::SourceSideInletNodeName,node->name().get());
    }
  }

  // Source Side Outlet Node Name
  if( auto mo = modelObject.demandOutletModelObject() )
  {
    if( auto node = mo->optionalCast<Node>() )
    {
      translateAndMapModelObject(node.get());

      idfObject.setString(WaterHeater_StratifiedFields::SourceSideOutletNodeName,node->name().get());
    }
  }

  // Source Side Effectiveness
  value = modelObject.sourceSideEffectiveness();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::SourceSideEffectiveness,value.get());
  }

  // Source Side Inlet Height
  if( modelObject.isSourceSideInletHeightAutocalculated() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::SourceSideInletHeight, "Autocalculate");
  }
  else
  {
    value = modelObject.sourceSideInletHeight();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::SourceSideInletHeight, value.get());
    }
  }

  // Source Side Outlet Height
  value = modelObject.sourceSideOutletHeight();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::SourceSideOutletHeight,value.get());
  }
  
  // Inlet Mode
  s = modelObject.inletMode();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::InletMode, s.get());
  }

  // Use Side Design Flow Rate
  if( modelObject.isUseSideDesignFlowRateAutosized() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::UseSideDesignFlowRate,"Autosize");
  }
  else
  {
    value = modelObject.useSideDesignFlowRate();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::UseSideDesignFlowRate,value.get());
    }
  }

  // Source Side Design Flow Rate
  if( modelObject.isSourceSideDesignFlowRateAutosized() )
  {
      idfObject.setString(WaterHeater_StratifiedFields::SourceSideDesignFlowRate,"Autosize");
  }
  else
  {
    value = modelObject.sourceSideDesignFlowRate();

    if( value )
    {
      idfObject.setDouble(WaterHeater_StratifiedFields::SourceSideDesignFlowRate,value.get());
    }
  }
  // Indirect Water Heating Recovery Time
  value = modelObject.indirectWaterHeatingRecoveryTime();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::IndirectWaterHeatingRecoveryTime,value.get());
  }

  // Number of Nodes
  {
    auto num_nodes = modelObject.numberofNodes();
    idfObject.setInt(WaterHeater_StratifiedFields::NumberofNodes,num_nodes);
  }

  // Additional Destratification Conductivity
  value = modelObject.additionalDestratificationConductivity();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::AdditionalDestratificationConductivity,value.get());
  }

  // Node 1 Additional Loss Coefficient
  value = modelObject.node1AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient,value.get());
  }

  // Node 2 Additional Loss Coefficient
  value = modelObject.node2AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient,value.get());
  }

  // Node 3 Additional Loss Coefficient
  value = modelObject.node3AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient,value.get());
  }

  // Node 4 Additional Loss Coefficient
  value = modelObject.node4AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient,value.get());
  }

  // Node 5 Additional Loss Coefficient
  value = modelObject.node5AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient,value.get());
  }

  // Node 6 Additional Loss Coefficient
  value = modelObject.node6AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient,value.get());
  }

  // Node 7 Additional Loss Coefficient
  value = modelObject.node7AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient,value.get());
  }

  // Node 8 Additional Loss Coefficient
  value = modelObject.node8AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient,value.get());
  }

  // Node 9 Additional Loss Coefficient
  value = modelObject.node9AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient,value.get());
  }

  // Node 10 Additional Loss Coefficient
  value = modelObject.node10AdditionalLossCoefficient();
  if( value )
  {
    idfObject.setDouble(WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient,value.get());
  }

  // Source Side Flow Control Mode
  s = modelObject.sourceSideFlowControlMode();
  if( s )
  {
    idfObject.setString(WaterHeater_StratifiedFields::SourceSideFlowControlMode, s.get());
  }

  // Indirect Alternate Setpoint Temperature Schedule Name
  schedule = modelObject.indirectAlternateSetpointTemperatureSchedule();
  if( schedule )
  {
    auto _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(WaterHeater_StratifiedFields::IndirectAlternateSetpointTemperatureScheduleName,_schedule->name().get());
    }
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

