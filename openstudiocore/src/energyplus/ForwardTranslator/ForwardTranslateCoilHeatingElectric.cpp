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
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingElectric_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingElectric( CoilHeatingElectric & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Coil_Heating_Electric);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(Coil_Heating_ElectricFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Efficiency

  if( (value = modelObject.efficiency()) )
  {
    idfObject.setDouble(Coil_Heating_ElectricFields::Efficiency,value.get());
  }

  // Nominal Capacity 
  
  if( modelObject.isNominalCapacityAutosized() )
  {
    idfObject.setString(Coil_Heating_ElectricFields::NominalCapacity,"Autosize");
  }
  else if( (value = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(Coil_Heating_ElectricFields::NominalCapacity,value.get());
  }

  // Air Inlet Node Name
 
  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_ElectricFields::AirInletNodeName,node->name().get());
    }
  }

  // Air Outlet Node Name
 
  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_ElectricFields::AirOutletNodeName,node->name().get());
    }
  }

  // Temperature Setpoint Node Name 
 
  if( boost::optional<Node> node = modelObject.temperatureSetpointNode() )
  {
    idfObject.setString(Coil_Heating_ElectricFields::TemperatureSetpointNodeName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

