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
#include "../../model/CoolingTowerSingleSpeed.hpp"
#include "../../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/CoolingTower_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoolingTowerSingleSpeed( CoolingTowerSingleSpeed & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Create a new IddObjectType::Fan_ConstantVolume
  IdfObject idfObject(IddObjectType::CoolingTower_SingleSpeed);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // WaterInletNodeName

  temp = modelObject.inletModelObject();
  if(temp)
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::WaterInletNodeName,temp->name().get());
  }

  // WaterOutletNodeName

  temp = modelObject.outletModelObject();
  if(temp)
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::WaterOutletNodeName,temp->name().get());
  }

  // DesignWaterFlowRate

  if( istringEqual(modelObject.performanceInputMethod(),"NominalCapacity") )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignWaterFlowRate,"");
  }
  else
  {
    if( (d = modelObject.designWaterFlowRate()) )
    {
      idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignWaterFlowRate,d.get());
    }
    else if( modelObject.isDesignAirFlowRateAutosized() )
    {
      idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignWaterFlowRate,"Autosize");
    }
  }

  // DesignAirFlowRate

  if( (d = modelObject.designAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignAirFlowRate,d.get());
  }
  else if( modelObject.isDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignAirFlowRate,"Autosize");
  }

  // FanPoweratDesignAirFlowRate

  if( (d = modelObject.fanPoweratDesignAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignFanPower,d.get());
  }
  else if( modelObject.isFanPoweratDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignFanPower,"Autosize");
  }

  // UFactorTimesAreaValueatDesignAirFlowRate

  if( (d = modelObject.uFactorTimesAreaValueatDesignAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignUFactorTimesAreaValue,d.get());
  }
  else if( modelObject.isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignUFactorTimesAreaValue,"Autosize");
  }

  // AirFlowRateinFreeConvectionRegime

  if( (d = modelObject.airFlowRateinFreeConvectionRegime()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionAirFlowRate,d.get());
  }
  else if( modelObject.isAirFlowRateinFreeConvectionRegimeAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionAirFlowRate,"Autosize");
  }

  // UFactorTimesAreaValueatFreeConvectionAirFlowRate

  if( (d = modelObject.uFactorTimesAreaValueatFreeConvectionAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionUFactorTimesAreaValue,d.get());
  }
  else if( modelObject.isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionUFactorTimesAreaValue,"Autosize");
  }

  // PerformanceInputMethod

  if( (s = modelObject.performanceInputMethod()) )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::PerformanceInputMethod,s.get());
  }

  // NominalCapacity

  if( (d = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::NominalCapacity,d.get());
  }

  // FreeConvectionCapacity

  if( (d = modelObject.freeConvectionCapacity()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionCapacity,d.get());
  }

  // BasinHeaterCapacity

  if( (d = modelObject.basinHeaterCapacity()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::BasinHeaterCapacity,d.get());
  }

  // BasinHeaterSetpointTemperature

  if( (d = modelObject.basinHeaterSetpointTemperature()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::BasinHeaterSetpointTemperature,d.get());
  }

  // BasinHeaterOperatingSchedule

  if( (temp = modelObject.basinHeaterOperatingSchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::BasinHeaterOperatingScheduleName,_schedule->name().get());
    }
  }

  // EvaporationLossMode

  if( (s = modelObject.evaporationLossMode()) )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::EvaporationLossMode,s.get());
  }

  // EvaporationLossFactor

  if( (d = modelObject.evaporationLossFactor()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::EvaporationLossFactor,d.get());
  }

  // DriftLossPercent

  if( (d = modelObject.driftLossPercent()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DriftLossPercent,d.get());
  }

  // BlowdownCalculationMode

  if( (s = modelObject.blowdownCalculationMode()) )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::BlowdownCalculationMode,s.get());
  }

  // BlowdownConcentrationRatio

  if( (d = modelObject.blowdownConcentrationRatio()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::BlowdownConcentrationRatio,d.get());
  }

  // BlowdownMakeupWaterUsageScheduleName

  if( (temp = modelObject.blowdownMakeupWaterUsageSchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName,_schedule->name().get());
    }
  }

  // CapacityControl

  if( (s = modelObject.capacityControl()) )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::CapacityControl,s.get());
  }

  // NumberofCells

  if( int n = modelObject.numberofCells() )
  {
    idfObject.setUnsigned(openstudio::CoolingTower_SingleSpeedFields::NumberofCells,n);
  }

  // CellControl

  if( (s = modelObject.cellControl()) )
  {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::CellControl,s.get());
  }

  // CellMinimumWaterFlowRateFraction

  if( (d = modelObject.cellMinimumWaterFlowRateFraction()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::CellMinimumWaterFlowRateFraction,d.get());
  }

  // CellMaximumWaterFlowRateFraction

  if( (d = modelObject.cellMaximumWaterFlowRateFraction()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::CellMaximumWaterFlowRateFraction,d.get());
  }

  // SizingFactor

  if( (d = modelObject.sizingFactor()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::SizingFactor,d.get());
  } else {
    // DLM: apply default value of 1, was added to IDD but did not fix previous files
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::SizingFactor,1);
  }

  if( (d = modelObject.freeConvectionAirFlowRateSizingFactor()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionAirFlowRateSizingFactor,d.get());
  }

  if( (d = modelObject.freeConvectionUFactorTimesAreaValueSizingFactor()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor,d.get());
  }

  if( (d = modelObject.heatRejectionCapacityAndNominalCapacitySizingRatio()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio,d.get());
  }

  if( (d = modelObject.freeConvectionNominalCapacitySizingFactor()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::FreeConvectionNominalCapacitySizingFactor,d.get());
  }

  if( (d = modelObject.designInletAirDryBulbTemperature()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignInletAirDryBulbTemperature,d.get());
  }

  if( (d = modelObject.designInletAirWetBulbTemperature()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignInletAirWetBulbTemperature,d.get());
  }

  if( modelObject.isDesignApproachTemperatureAutosized() ) {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignApproachTemperature,"Autosize");
  } else if( (d = modelObject.designApproachTemperature()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignApproachTemperature,d.get());
  }

  if( modelObject.isDesignRangeTemperatureAutosized() ) {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::DesignRangeTemperature,"Autosize");
  } else if( (d = modelObject.designApproachTemperature()) ) {
    idfObject.setDouble(openstudio::CoolingTower_SingleSpeedFields::DesignRangeTemperature,d.get());
  }

  if( (s = modelObject.endUseSubcategory()) ) {
    idfObject.setString(openstudio::CoolingTower_SingleSpeedFields::EndUseSubcategory,s.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

