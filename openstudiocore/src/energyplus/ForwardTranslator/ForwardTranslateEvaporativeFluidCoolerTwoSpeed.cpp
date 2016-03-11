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
#include "../../model/EvaporativeFluidCoolerTwoSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerTwoSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeFluidCooler_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEvaporativeFluidCoolerTwoSpeed( EvaporativeFluidCoolerTwoSpeed & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;
 
  //Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EvaporativeFluidCooler_TwoSpeed, modelObject);

  // WaterInletNodeName
  temp = modelObject.inletModelObject();
  if(temp) {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::WaterInletNodeName,temp->name().get());
  }

  // WaterOutletNodeName
  temp = modelObject.outletModelObject();
  if(temp) {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::WaterOutletNodeName,temp->name().get());
  }

  // HighFanSpeedAirFlowRate
  if( modelObject.isHighFanSpeedAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate,"Autosize");
  } 
  else if( (d = modelObject.highFanSpeedAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate,d.get());
  }

  // HighFanSpeedFanPower
  if( modelObject.isHighFanSpeedFanPowerAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower,"Autosize");
  } 
  else if( (d = modelObject.highFanSpeedFanPower()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower,d.get());
  }

  // LowFanSpeedAirFlowRate
  if( modelObject.isLowFanSpeedAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate,"Autocalculate");
  } 
  else if( (d = modelObject.lowFanSpeedAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate,d.get());
  }

  // LowFanSpeedAirFlowRateSizingFactor
  if( (d = modelObject.lowFanSpeedAirFlowRateSizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor,d.get());
  }

  // LowFanSpeedFanPower
  if( modelObject.isLowFanSpeedFanPowerAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower,"Autocalculate");
  } 
  else if( (d = modelObject.lowFanSpeedFanPower()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower,d.get());
  }

  // LowFanSpeedFanPowerSizingFactor
  if( (d = modelObject.lowFanSpeedFanPowerSizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor,d.get());
  }

  // DesignSprayWaterFlowRate
  if( (d = modelObject.designSprayWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignSprayWaterFlowRate,d.get());
  }

  // PerformanceInputMethod
  if( (s = modelObject.performanceInputMethod()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::PerformanceInputMethod,s.get());
  }

  // OutdoorAirInletNodeName
  idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::OutdoorAirInletNodeName,"");

  // HeatRejectionCapacityandNominalCapacitySizingRatio
  if( (d = modelObject.heatRejectionCapacityandNominalCapacitySizingRatio()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio,d.get());
  }

  // HighSpeedStandardDesignCapacity
  if( (d = modelObject.highSpeedStandardDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighSpeedStandardDesignCapacity,d.get());
  }

  // LowSpeedStandardDesignCapacity
  if( modelObject.isLowSpeedStandardDesignCapacityAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity,"Autosize");
  } 
  else if( (d = modelObject.lowSpeedStandardDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity,d.get());
  }

  // LowSpeedStandardCapacitySizingFactor
  if( (d = modelObject.lowSpeedStandardCapacitySizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardCapacitySizingFactor,d.get());
  }

  // HighFanSpeedUfactorTimesAreaValue
  if( modelObject.isHighFanSpeedUfactorTimesAreaValueAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue,"Autosize");
  } 
  else if( (d = modelObject.highFanSpeedUfactorTimesAreaValue()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue,d.get());
  }

  // LowFanSpeedUfactorTimesAreaValue
  if( modelObject.isLowFanSpeedUfactorTimesAreaValueAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue,"Autocalculate");
  } 
  else if( (d = modelObject.lowFanSpeedUfactorTimesAreaValue()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue,d.get());
  }

  // LowFanSpeedUFactorTimesAreaSizingFactor
  if( (d = modelObject.lowFanSpeedUFactorTimesAreaSizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor,d.get());
  }

  // DesignWaterFlowRate
  if( modelObject.isDesignWaterFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate,"Autosize");
  } 
  else if( (d = modelObject.designWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate,d.get());
  }

  // HighSpeedUserSpecifiedDesignCapacity
  if( (d = modelObject.highSpeedUserSpecifiedDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighSpeedUserSpecifiedDesignCapacity,d.get());
  }

  // LowSpeedUserSpecifiedDesignCapacity
  if( modelObject.isLowSpeedUserSpecifiedDesignCapacityAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity,"Autocalculate");
  } 
  else if( (d = modelObject.lowSpeedUserSpecifiedDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity,d.get());
  }

  // LowSpeedUserSpecifiedDesignCapacitySizingFactor
  if( (d = modelObject.lowSpeedUserSpecifiedDesignCapacitySizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacitySizingFactor,d.get());
  }

  // DesignEnteringWaterTemperature
  if( (d = modelObject.designEnteringWaterTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature,d.get());
  }

  // DesignEnteringAirTemperature
  if( (d = modelObject.designEnteringAirTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirTemperature,d.get());
  }

  // DesignEnteringAirWetbulbTemperature
  if( (d = modelObject.designEnteringAirWetbulbTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature,d.get());
  }

  // HighSpeedSizingFactor
  if( (d = modelObject.highSpeedSizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighSpeedSizingFactor,d.get());
  }

  // EvaporationLossMode
  if( (s = modelObject.evaporationLossMode()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossMode,s.get());
  }

  // EvaporationLossFactor
  if( (d = modelObject.evaporationLossFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossFactor,d.get());
  }

  // DriftLossPercent
  if( (d = modelObject.driftLossPercent()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DriftLossPercent,d.get());
  }

  // BlowdownCalculationMode
  if( (s = modelObject.blowdownCalculationMode()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::BlowdownCalculationMode,s.get());
  }

  // BlowdownConcentrationRatio
  if( (d = modelObject.blowdownConcentrationRatio()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::BlowdownConcentrationRatio,d.get());
  }

  // BlowdownMakeupWaterUsageScheduleName
  if( (temp = modelObject.blowdownMakeupWaterUsageSchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName,_schedule->name().get());
    }
  }

  // SupplyWaterStorageTankName

  return idfObject;
}
} // energyplus

} // openstudio

