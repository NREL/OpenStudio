/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateAirTerminalSingleDuctVAVReheat( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::AirTerminal_SingleDuct_VAV_Reheat )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: AirTerminal_SingleDuct_VAV_Reheat");
     return boost::none;
  }

  boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName);
  boost::optional<Schedule> schedule;
  boost::optional<HVACComponent> coil;
  boost::optional<AirTerminalSingleDuctVAVReheat> airTerminal;

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

  wo = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName);
  if( wo )
  {
    boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
    if( mo )
    {
      if( ! coil )
      {
        coil = mo->optionalCast<CoilHeatingGas>();
      }
    }
  }

  if( schedule && coil )
  {
    airTerminal = AirTerminalSingleDuctVAVReheat( m_model,schedule.get(),coil.get() );
  }

  if( airTerminal )
  {
    boost::optional<double> value;
    boost::optional<std::string> s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::Name);

    if( s )
    {
      airTerminal->setName(s.get());
    }

    // MaximumAirFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
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
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod);
    if( s )
    {
      airTerminal->setZoneMinimumAirFlowMethod(s.get());
    }

    // ConstantMinimumAirFlowFraction
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction);
    if( value )
    {
      airTerminal->setConstantMinimumAirFlowFraction(value.get());
    } else {
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction);
      if( s && (istringEqual(s.get(),"Autosize") || istringEqual(s.get(),"Autocalculate") ) ) {
        airTerminal->autosizeMaximumAirFlowRate();
      }
    }

    // FixedMinimumAirFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate);
    if( value )
    {
      airTerminal->setFixedMinimumAirFlowRate(value.get());
    } else {
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate);
      if( s && (istringEqual(s.get(),"Autosize") || istringEqual(s.get(),"Autocalculate") ) ) {
        airTerminal->autosizeFixedMinimumAirFlowRate();
      }
    }

    boost::optional<WorkspaceObject> _schedule;

    // MinimumAirFlowFractionScheduleName
    _schedule = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName);
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

    // MaximumHotWaterorSteamFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
    if( value )
    {
      airTerminal->setMaximumHotWaterOrSteamFlowRate(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      airTerminal->autosizeMaximumHotWaterOrSteamFlowRate();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      airTerminal->autosizeMaximumHotWaterOrSteamFlowRate();
    }

    // MinimumHotWaterorSteamFlowRate
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate);
    if( value )
    {
      airTerminal->setMinimumHotWaterOrStreamFlowRate(value.get());
    }

    // ConvergenceTolerance
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance);
    if( value )
    {
      airTerminal->setConvergenceTolerance(value.get());
    }

    // DamperHeatingAction
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction);
    if( s )
    {
      airTerminal->setDamperHeatingAction(s.get());
    }

    // MaximumFlowPerZoneFloorAreaDuringReheat
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
    if( value )
    {
      airTerminal->setMaximumFlowPerZoneFloorAreaDuringReheat(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      airTerminal->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      airTerminal->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
    }

    // MaximumFlowFractionDuringReheat
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
    s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
    if( value )
    {
      airTerminal->setMaximumFlowFractionDuringReheat(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      airTerminal->autosizeMaximumFlowFractionDuringReheat();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      airTerminal->autosizeMaximumFlowFractionDuringReheat();
    }

    // MaximumReheatAirTemperature
    value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature);
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

