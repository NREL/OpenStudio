/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include <model/EvaporativeFluidCoolerSingleSpeed.hpp>
#include <model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>
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

