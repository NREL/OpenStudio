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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneControlContaminantController.hpp"
#include "../../model/ZoneControlContaminantController_Impl.hpp"
#include <utilities/idd/ZoneControl_ContaminantController_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneControlContaminantController( ZoneControlContaminantController& modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::ZoneControl_ContaminantController);

  m_idfObjects.push_back(idfObject);

  {
    auto s = modelObject.name();
    idfObject.setName(s.get());
  }

  if( auto tz = modelObject.getImpl<model::detail::ZoneControlContaminantController_Impl>()->controlledZone() ) {
    idfObject.setString(ZoneControl_ContaminantControllerFields::ControlledZoneName,tz->name().get());
  }

  if( auto s = modelObject.carbonDioxideControlAvailabilitySchedule() ) {
    idfObject.setString(ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName,s->name().get());
  }

  if( auto s = modelObject.carbonDioxideSetpointSchedule() ) {
    idfObject.setString(ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName,s->name().get());
  }

  if( auto s = modelObject.minimumCarbonDioxideConcentrationSchedule() ) {
    idfObject.setString(ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName,s->name().get());
  }

  if( auto s = modelObject.genericContaminantControlAvailabilitySchedule() ) {
    idfObject.setString(ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName,s->name().get());
  }

  if( auto s = modelObject.genericContaminantSetpointSchedule() ) {
    idfObject.setString(ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName,s->name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

