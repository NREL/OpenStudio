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
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatExchangerAirToAirSensibleAndLatent( HeatExchangerAirToAirSensibleAndLatent & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName
  Schedule sched = modelObject.availabilitySchedule();
  translateAndMapModelObject(sched);
  idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilityScheduleName,sched.name().get());

  // NominalSupplyAirFlowRate
  if( modelObject.isNominalSupplyAirFlowRateAutosized() )
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate,"Autosize");
  }
  else
  {
    d = modelObject.nominalSupplyAirFlowRate();
    if( d )
    {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate,d.get());
    }
  }

  // SensibleEffectivenessat100_HeatingAirFlow
  d = modelObject.sensibleEffectivenessat100HeatingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow,d.get());
  }

  // LatentEffectivenessat100_HeatingAirFlow
  d = modelObject.latentEffectivenessat100HeatingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow,d.get());
  }

  // SensibleEffectivenessat75_HeatingAirFlow
  d = modelObject.sensibleEffectivenessat75HeatingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow,d.get());
  }

  // LatentEffectivenessat75_HeatingAirFlow
  d = modelObject.latentEffectivenessat75HeatingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow,d.get());
  }

  // SensibleEffectivenessat100_CoolingAirFlow
  d = modelObject.sensibleEffectivenessat100CoolingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow,d.get());
  }

  // LatentEffectivenessat100_CoolingAirFlow
  d = modelObject.latentEffectivenessat100CoolingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow,d.get());
  }

  // SensibleEffectivenessat75_CoolingAirFlow
  d = modelObject.sensibleEffectivenessat75CoolingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow,d.get());
  }

  // LatentEffectivenessat75_CoolingAirFlow
  d = modelObject.latentEffectivenessat75CoolingAirFlow();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow,d.get());
  }

  // SupplyAirInletNodeName
  temp = modelObject.primaryAirInletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName,*s);
    }
  }

  // SupplyAirOutletNodeName
  temp = modelObject.primaryAirOutletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName,*s);
    }
  }

  // ExhaustAirInletNodeName
  temp = modelObject.secondaryAirInletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName,*s);
    }
  }

  // ExhaustAirOutletNodeName
  temp = modelObject.secondaryAirOutletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName,*s);
    }
  }

  // NominalElectricPower
  d = modelObject.nominalElectricPower();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower,*d);
  }

  // SupplyAirOutletTemperatureControl
  if( modelObject.supplyAirOutletTemperatureControl() )
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl,"Yes");
  }
  else
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl,"No");
  }

  // HeatExchangerType
  s = modelObject.heatExchangerType();
  if( s )
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType,*s);
  }

  // FrostControlType 
  s = modelObject.frostControlType();  
  if( s )
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType,*s);
  }

  // ThresholdTemperature
  d = modelObject.thresholdTemperature();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature,*d);
  }

  // InitialDefrostTimeFraction
  d = modelObject.initialDefrostTimeFraction();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction,*d);
  }

  // RateofDefrostTimeFractionIncrease
  d = modelObject.rateofDefrostTimeFractionIncrease();
  if( d )
  {
    idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease,*d);
  }

  // EconomizerLockout
  if( modelObject.economizerLockout() )
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout,"Yes");
  }
  else
  {
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout,"No");
  }

  return idfObject;
}

} // energyplus

} // openstudio

