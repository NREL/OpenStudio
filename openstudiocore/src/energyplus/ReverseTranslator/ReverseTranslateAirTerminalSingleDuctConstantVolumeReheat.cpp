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

