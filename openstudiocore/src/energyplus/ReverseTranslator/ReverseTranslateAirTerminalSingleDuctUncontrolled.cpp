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

#include <energyplus/ReverseTranslator.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateAirTerminalSingleDuctUncontrolled( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::AirTerminal_SingleDuct_Uncontrolled )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: AirTerminal_SingleDuct_Uncontrolled");
     return boost::none;
  }

  boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(AirTerminal_SingleDuct_UncontrolledFields::AvailabilityScheduleName);
  boost::optional<Schedule> schedule;
  boost::optional<AirTerminalSingleDuctUncontrolled> airTerminal;
  boost::optional<double> value;

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
    airTerminal = AirTerminalSingleDuctUncontrolled(m_model,schedule.get());
  }

  if( airTerminal )
  {
    boost::optional<std::string> s = workspaceObject.getString(AirTerminal_SingleDuct_UncontrolledFields::Name);
    if( s )
    {
      airTerminal->setName(s.get());
    }

    s = workspaceObject.getString(AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate);
    if( s && istringEqual(s.get(),"AutoSize") )
    {
      airTerminal->autosizeMaximumAirFlowRate();
    }
    else if( (value = workspaceObject.getDouble(AirTerminal_SingleDuct_UncontrolledFields::MaximumAirFlowRate)) )
    {
      airTerminal->setMaximumAirFlowRate(value.get());
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

