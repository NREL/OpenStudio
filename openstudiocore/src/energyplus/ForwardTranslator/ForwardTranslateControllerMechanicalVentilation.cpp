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
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerMechanicalVentilation.hpp"
#include "../../model/ControllerMechanicalVentilation_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/Controller_MechanicalVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

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
  // If there is a ControllerOutdoorAir::minimumOutdoorAirSchedule 
  // then use that for the ControllerMechanicalVentilation::availabilitySchedule
  // Note that this scheme will not support fractions (schedule values above 0) because anything greater than 0 will
  // make the mechanical ventilation controller avaiable and thus taking precedence.
  bool useAvailabiltySchedule = true;

  // Find the associated oa controller
  auto oaControllers = modelObject.model().getConcreteModelObjects<ControllerOutdoorAir>();
  auto predicate = [&] (const ControllerOutdoorAir & oaController) {
    auto mechanicalVentilationController = oaController.controllerMechanicalVentilation();
    if( mechanicalVentilationController.handle() == modelObject.handle() ) {
      return true;
    }
    return false;
  };
  auto oaController = std::find_if(oaControllers.begin(),oaControllers.end(),predicate);
  if( oaController != oaControllers.end() ) {
    if( auto minOASchedule = oaController->minimumOutdoorAirSchedule() ) {
      auto _schedule = translateAndMapModelObject(minOASchedule.get());
      OS_ASSERT(_schedule);
      idfObject.setString(Controller_MechanicalVentilationFields::AvailabilityScheduleName,_schedule->name().get());
      useAvailabiltySchedule = false;
    }
  }

  if( useAvailabiltySchedule ) {
    Schedule availabilitySchedule = modelObject.availabilitySchedule();
    boost::optional<IdfObject> availabilityScheduleIdf = translateAndMapModelObject(availabilitySchedule);
    OS_ASSERT(availabilityScheduleIdf);
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
    if( istringEqual("ProportionalControl",s.get()) ) {
      idfObject.setString(openstudio::Controller_MechanicalVentilationFields::SystemOutdoorAirMethod,"ProportionalControlBasedonOccupancySchedule");
    } else {
      idfObject.setString(openstudio::Controller_MechanicalVentilationFields::SystemOutdoorAirMethod,s.get());
    }
  }

  m_idfObjects.push_back(idfObject);
  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

