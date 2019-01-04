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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

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
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower,d.get());
  }
  else if( modelObject.isFanPoweratDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower,"Autosize");
  }


  // DesignWaterFlowRate

  if( (d = modelObject.designWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,d.get());
  }
  else if( modelObject.isDesignWaterFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,"Autosize");
  }

  // DesignSprayWaterFlowRate

  if( (d = modelObject.designSprayWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate,d.get());
  }


 // PerformanceInputMethod

  if( (s = modelObject.performanceInputMethod()) )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod,s.get());
  }

// OutdoorAirInletNodeName

  idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::OutdoorAirInletNodeName,"");


// StandardDesignCapacity

   if( (d = modelObject.standardDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity,d.get());
  }

  // UFactorTimesAreaValueatDesignAirFlowRate

  if( (d = modelObject.ufactorTimesAreaValueatDesignAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue,d.get());
  }
  else if( modelObject.isUfactorTimesAreaValueatDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue,"Autosize");
  }


  // UserSpecifiedDesignCapacity

  if( (d = modelObject.userSpecifiedDesignCapacity()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity,d.get());
  }


// DesignEnteringWaterTemperature

  if( (d = modelObject.designEnteringWaterTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,d.get());
  }

// DesignEnteringAirTemperature

  if( (d = modelObject.designEnteringAirTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,d.get());
  }

// DesignEnteringAirWetbulbTemperature
  if( (d = modelObject.designEnteringAirWetbulbTemperature()) )
  {
    idfObject.setDouble(openstudio::EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,d.get());
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

  //if( (s = modelObject.supplyWaterStorageTankName()) )
  //{
  //  idfObject.setString(openstudio::EvaporativeFluidCooler_SingleSpeedFields::SupplyWaterStorageTankName,s.get());
  //}

  return boost::optional<IdfObject>(idfObject);
}
} // energyplus

} // openstudio

