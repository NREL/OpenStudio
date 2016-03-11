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

#include "../ReverseTranslator.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_NoReheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateAirTerminalSingleDuctVAVNoReheat( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::AirTerminal_SingleDuct_VAV_NoReheat )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: AirTerminal_SingleDuct_VAV_NoReheat");
     return boost::none;
  }

  boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName);
  boost::optional<Schedule> schedule;
  boost::optional<AirTerminalSingleDuctVAVNoReheat> airTerminal;

  if( wo )
  {
    boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
    if( mo )
    {
      if( ! (schedule = mo->optionalCast<Schedule>()) )
      {
        LOG(Error, workspaceObject.briefDescription() << " does not have an associated availability schedule");

        return boost::none;
      }
    }
  }

  if( schedule )
  {
    airTerminal = AirTerminalSingleDuctVAVNoReheat( m_model,schedule.get() );
  }

  if( airTerminal )
  {
    boost::optional<double> value;
    boost::optional<std::string> s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::Name);

    if( s )
    {
      airTerminal->setName(s.get());
    }

    // MaximumAirFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate);
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate);
    if( value )
    {
      airTerminal->setMaximumAirFlowRate(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      airTerminal->autosizeMaximumAirFlowRate();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      airTerminal->autosizeMaximumAirFlowRate();
    }

    // ZoneMinimumAirFlowInputMethod
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod);
    if( s )
    {
      airTerminal->setZoneMinimumAirFlowInputMethod(s.get());
    }

    // ConstantMinimumAirFlowFraction
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction);
    if( value )
    {
      airTerminal->setConstantMinimumAirFlowFraction(value.get());
    }

    // FixedMinimumAirFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate);
    if( value )
    {
      airTerminal->setFixedMinimumAirFlowRate(value.get());
    }

    boost::optional<WorkspaceObject> _schedule;

    // MinimumAirFlowFractionScheduleName
    _schedule = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowFractionScheduleName);
    if( _schedule )
    {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(_schedule.get());
      if( mo )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          airTerminal->setMinimumAirFlowFractionSchedule(schedule.get());
        }
      }
    }

    return airTerminal.get();
  }
  else
  {
    LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

    return boost::none;
  }
}

} // energyplus

} // openstudio

