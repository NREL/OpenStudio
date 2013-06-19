/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.
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
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/ControllerMechanicalVentilation.hpp>
#include <model/ControllerMechanicalVentilation_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/ControllerOutdoorAir_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/Controller_MechanicalVentilation_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateControllerMechanicalVentilation( ControllerMechanicalVentilation& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::Controller_MechanicalVentilation);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // Availability Schedule
  Schedule availabilitySchedule = modelObject.availabilitySchedule();
  boost::optional<IdfObject> availabilityScheduleIdf = translateAndMapModelObject(availabilitySchedule);
  if( availabilityScheduleIdf )
  {
    idfObject.setString(openstudio::Controller_MechanicalVentilationFields::AvailabilityScheduleName,availabilityScheduleIdf->name().get());
  }

  // Demand Controlled Ventilation
  if( modelObject.demandControlledVentilation() )
  {
    idfObject.setString(openstudio::Controller_MechanicalVentilationFields::DemandControlledVentilation,"Yes"); 
  }
  else
  {
    idfObject.setString(openstudio::Controller_MechanicalVentilationFields::DemandControlledVentilation,"No"); 
  }

  // System Outdoor Air Method
  s = modelObject.systemOutdoorAirMethod();
  if( s )
  {
    idfObject.setString(openstudio::Controller_MechanicalVentilationFields::SystemOutdoorAirMethod,s.get());
  }

  m_idfObjects.push_back(idfObject);
  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

