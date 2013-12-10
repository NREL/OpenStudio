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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/CoilHeatingDesuperheater.hpp>

#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDesuperheater(CoilHeatingDesuperheater & modelObject)
{
  OptionalModelObject omo;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_Desuperheater, modelObject);

// Availability Schedule Name
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      object.setString(Coil_Heating_DesuperheaterFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

// Heat Recovery Efficiency
  d = modelObject.heatReclaimRecoveryEfficiency();
  if (d) {
      object.setDouble(Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency,d.get());
  }

// Air Inlet Node Name
  omo = modelObject.inletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      object.setString(Coil_Heating_DesuperheaterFields::AirInletNodeName,*s);
    }
  }

// Air Outlet Node Name
  omo = modelObject.outletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      object.setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName,*s);
    }
  }

// Heating Source Name
  boost::optional<ModelObject> heatingSource = modelObject.heatingSource();

  if ( heatingSource ) {
      boost::optional<IdfObject> _heatingSource = translateAndMapModelObject(heatingSource.get());

      if ( _heatingSource && _heatingSource->name() )
      {
          object.setString(Coil_Heating_DesuperheaterFields::HeatingSourceName,_heatingSource->name().get());
      }
  }

// Temperature Setpoint Node Name
  object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName,"");

// Parasitic Electric Load
  d = modelObject.parasiticElectricLoad();
  if (d) {
      object.setDouble(Coil_Heating_DesuperheaterFields::ParasiticElectricLoad,d.get());
  }

  return object;

}
}
}