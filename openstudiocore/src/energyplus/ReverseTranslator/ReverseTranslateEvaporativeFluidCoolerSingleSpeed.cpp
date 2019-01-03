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
#include "../../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateEvaporativeFluidCoolerSingleSpeed( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::EvaporativeFluidCooler_SingleSpeed )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: EvaporativeFluidCooler_SingleSpeed");
     return boost::none;
  }

  boost::optional<EvaporativeFluidCoolerSingleSpeed> evapCooler;

  evapCooler = EvaporativeFluidCoolerSingleSpeed( m_model );

  if( evapCooler )
  {
    boost::optional<double> value;
    boost::optional<std::string> s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::Name);

    if( s )
    {
      evapCooler->setName(s.get());
    }

    // DesignAirFlowRate
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate);
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate);
    if( value )
    {
      evapCooler->setDesignAirFlowRate(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      evapCooler->autosizeDesignAirFlowRate();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      evapCooler->autosizeDesignAirFlowRate();
    }

    // FanPoweratDesignAirFlowRate
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower);
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower);
    if( value )
    {
      evapCooler->setFanPoweratDesignAirFlowRate(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      evapCooler->autosizeFanPoweratDesignAirFlowRate();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      evapCooler->autosizeFanPoweratDesignAirFlowRate();
    }

    // DesignSprayWaterFlowRate
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate);
    if( value )
    {
      evapCooler->setDesignSprayWaterFlowRate(value.get());
    }

    // PerformanceInputMethod
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod);
    if( s )
    {
      evapCooler->setPerformanceInputMethod(s.get());
    }

    // StandardDesignCapacity
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity);
    if( value )
    {
      evapCooler->setStandardDesignCapacity(value.get());
    }

    // UfactorTimesAreaValueatDesignAirFlowRate
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue);
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue);
    if( value )
    {
      evapCooler->setUfactorTimesAreaValueatDesignAirFlowRate(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      evapCooler->autosizeUfactorTimesAreaValueatDesignAirFlowRate();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      evapCooler->autosizeUfactorTimesAreaValueatDesignAirFlowRate();
    }

    // DesignWaterFlowRate
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate);
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate);
    if( value )
    {
      evapCooler->setDesignWaterFlowRate(value.get());
    }
    else if( s && istringEqual(s.get(),"Autosize") )
    {
      evapCooler->autosizeDesignWaterFlowRate();
    }
    else if( s && istringEqual(s.get(),"Autocalculate") )
    {
      evapCooler->autosizeDesignWaterFlowRate();
    }

    // UserSpecifiedDesignCapacity
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity);
    if( value )
    {
      evapCooler->setUserSpecifiedDesignCapacity(value.get());
    }

    // DesignEnteringWaterTemperature
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature);
    if( value )
    {
      evapCooler->setDesignEnteringWaterTemperature(value.get());
    }

    // DesignEnteringAirTemperature
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature);
    if( value )
    {
      evapCooler->setDesignEnteringAirTemperature(value.get());
    }

    // DesignEnteringAirWetbulbTemperature
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature);
    if( value )
    {
      evapCooler->setDesignEnteringAirWetbulbTemperature(value.get());
    }

    // CapacityControl
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::CapacityControl);
    if( s )
    {
      evapCooler->setCapacityControl(s.get());
    }

    // SizingFactor
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::SizingFactor);
    if( value )
    {
      evapCooler->setSizingFactor(value.get());
    }

    // EvaporationLossMode
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode);
    if( s )
    {
      evapCooler->setEvaporationLossMode(s.get());
    }

    // EvaporationLossFactor
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor);
    if( value )
    {
      evapCooler->setEvaporationLossFactor(value.get());
    }

    // DriftLossPercent
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent);
    if( value )
    {
      evapCooler->setDriftLossPercent(value.get());
    }

    // BlowdownCalculationMode
    s = workspaceObject.getString(EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode);
    if( s )
    {
      evapCooler->setBlowdownCalculationMode(s.get());
    }

    // BlowdownConcentrationRatio
    value = workspaceObject.getDouble(EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio);
    if( value )
    {
      evapCooler->setBlowdownConcentrationRatio(value.get());
    }

    boost::optional<WorkspaceObject> _schedule;

    // BlowdownMakeupWaterUsageScheduleName
    _schedule = workspaceObject.getTarget(EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName);
    if( _schedule )
    {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(_schedule.get());
      if( mo )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          evapCooler->setBlowdownMakeupWaterUsageSchedule(schedule.get());
        }
      }
    }

    return evapCooler.get();
  }
  else
  {
    LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

    return boost::none;
  }
}

} // energyplus

} // openstudio

