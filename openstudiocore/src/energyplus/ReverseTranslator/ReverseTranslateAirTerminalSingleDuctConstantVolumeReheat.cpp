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
#include "../../model/AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingElectric_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_Reheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateAirTerminalSingleDuctConstantVolumeReheat( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::AirTerminal_SingleDuct_ConstantVolume_Reheat )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: AirTerminal_SingleDuct_ConstantVolume_Reheat");
     return boost::none;
  }

  boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName);
  boost::optional<Schedule> schedule;
  boost::optional<HVACComponent> coil;
  boost::optional<AirTerminalSingleDuctConstantVolumeReheat> airTerminal;

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

  wo = workspaceObject.getTarget(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName);
  if( wo )
  {
    boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
    if( mo )
    {
      if( ! coil )
      {
        //TODO: Maybe try to cast this to different types depending on ReheatCoilType
        coil = mo->optionalCast<CoilHeatingElectric>();
      }
    }
  }

  if( schedule && coil )
  {
    airTerminal = AirTerminalSingleDuctConstantVolumeReheat( m_model,schedule.get(),coil.get() );
  }

  if( airTerminal )
  {
    boost::optional<double> value;
    boost::optional<std::string> s = workspaceObject.getString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::Name);

    if( s )
    {
      airTerminal->setName(s.get());
    }

    // MaximumAirFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate);
    if( value )
    {
      airTerminal->setMaximumAirFlowRate(value.get());
    }
    else
    {
      s = workspaceObject.getString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate);
      if( s && istringEqual(s.get(),"Autosize") )
      {
        airTerminal->autosizeMaximumAirFlowRate();
      }
      else if( s && istringEqual(s.get(),"Autocalculate") )
      {
        airTerminal->autosizeMaximumAirFlowRate();
      }
    }
  
    // MaximumHotWaterorSteamFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate);
    if( value )
    {
      airTerminal->setMaximumHotWaterorSteamFlowRate(value.get());
    }
    else
    {
      s = workspaceObject.getString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate);
      if( s && istringEqual(s.get(),"Autosize") )
      {
        airTerminal->autosizeMaximumHotWaterorSteamFlowRate();
      }
      else if( s && istringEqual(s.get(),"Autocalculate") )
      {
        airTerminal->autosizeMaximumHotWaterorSteamFlowRate();
      }
    }

    // MinimumHotWaterorSteamFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate);
    if( value )
    {
      airTerminal->setMinimumHotWaterorSteamFlowRate(value.get());
    }

    // ConvergenceTolerance
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance);
    if( value )
    {
      airTerminal->setConvergenceTolerance(value.get());
    }

    // MaximumReheatAirTemperature
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature);
    if( value )
    {
      airTerminal->setMaximumReheatAirTemperature(value.get());
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

