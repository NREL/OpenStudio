/**********************************************************************
 *  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/EvaporativeFluidCoolerSingleSpeed.hpp>
#include <model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEvaporativeFluidCoolerSingleSpeed( EvaporativeFluidCoolerSingleSpeed & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Create a new IddObjectType::Evaporative_FluidCoolerSingleSpeed
  IdfObject idfObject(IddObjectType::EvaporativeFluidCooler_SingleSpeed);

  m_idfObjects.push_back(idfObject);
 
 //Name  
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // WaterInletNodeName

  temp = modelObject.inletModelObject();
  if(temp)
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::WaterInletNodeName,temp->name().get());
  }

  // WaterOutletNodeName

  temp = modelObject.outletModelObject();
  if(temp)
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::WaterOutletNodeName,temp->name().get());
  }

  
  // DesignAirFlowRate 

  if( (d = modelObject.designAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate,d.get());
  }
  else if( modelObject.isDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate,"Autosize");
  }
  
   // FanPoweratDesignAirFlowRate
  
  if( (d = modelObject.fanPoweratDesignAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate,d.get());
  }
  else if( modelObject.isFanPoweratDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate,"Autosize");
  }

     
  // DesignWaterFlowRate 

  if( (d = modelObject.designWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,d.get());
  }
  else if( modelObject.isdesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,"Autosize");
  }

  // DesignSprayWaterFlowRate
  
  if( (d = modelObject.designSprayWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::designSprayWaterFlowRate,d.get());
  }

  
 // PerformanceInputMethod

  if( (s = modelObject.performanceInputMethod()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::performanceInputMethod,s.get());
  }
 
// OutdoorAirInletNodeName

  temp = modelObject.OutletModelObject();
  if(temp)
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::OutdoorAirInletNodeName,temp->name().get());
  }


// StandardDesignCapacity

   if( (d = modelObject.standardDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::standardDesignCapacity,d.get());
  }
 
  // UFactorTimesAreaValueatDesignAirFlowRate

  if( (d = modelObject.uFactorTimesAreaValueatDesignAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate,d.get());
  }
  else if( modelObject.isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::UFactorTimesAreaValueatDesignAirFlowRate,"Autosize");
  }


  // UserSpecifiedDesignCapacity

  if( (d = modelObject.userSpecifiedDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::userSpecifiedDesignCapacity,d.get());
  }

    
// DesignEnteringWaterTemperature
		
  if( (d = modelObject.designEnteringWaterTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::designEnteringWaterTemperature,d.get());
  }
	
// DesignEnteringAirTemperature
		
  if( (d = modelObject.designEnteringAirTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::designEnteringAirTemperature,d.get());
  }	
	
// DesignEnteringAirWetbulbTemperature
	if( (d = modelObject.designEnteringAirWetbulbTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::designEnteringAirWetbulbTemperature,d.get());
  }	

  // CapacityControl

  if( (s = modelObject.capacityControl()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::CapacityControl,s.get());
  }
  
   // SizingFactor

  if( (d = modelObject.sizingFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::SizingFactor,d.get());
  } 
 
  // EvaporationLossMode

  if( (s = modelObject.evaporationLossMode()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode,s.get());
  }

  // EvaporationLossFactor

  if( (d = modelObject.evaporationLossFactor()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor,d.get());
  }

  // DriftLossPercent

  if( (d = modelObject.driftLossPercent()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent,d.get());
  }

  // BlowdownCalculationMode

  if( (s = modelObject.blowdownCalculationMode()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode,s.get());
  }
  
  // BlowdownConcentrationRatio
  
  if( (d = modelObject.blowdownConcentrationRatio()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio,d.get());
  }

  // BlowdownMakeupWaterUsageScheduleName

  if( (temp = modelObject.blowdownMakeupWaterUsageSchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName,_schedule->name().get());
    }
  }

 // supplyWaterStorageTankName

  if( (s = modelObject.supplyWaterStorageTankName()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::supplyWaterStorageTankName,s.get());
  }

  return boost::optional<IdfObject>(idfObject);
}
} // energyplus

} // openstudio

