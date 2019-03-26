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
#include "../../model/CoolingTowerVariableSpeed.hpp"
#include "../../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/CoolingTower_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoolingTowerVariableSpeed( CoolingTowerVariableSpeed & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::CoolingTower_VariableSpeed);

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
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::WaterInletNodeName,temp->name().get());
  }

  // WaterOutletNodeName

  temp = modelObject.outletModelObject();
  if(temp)
  {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::WaterOutletNodeName,temp->name().get());
  }

  // ModelType

  if( (s = modelObject.modelType()) )
  {
    idfObject.setString(CoolingTower_VariableSpeedFields::ModelType,s.get());
  }

  // ModelCoefficient

  if( boost::optional<ModelObject> mo = modelObject.modelCoefficient() )
  {
    if( boost::optional<IdfObject> _mo = translateAndMapModelObject(mo.get()) )
    {
      idfObject.setString(CoolingTower_VariableSpeedFields::ModelCoefficientName,_mo->name().get());
    }
  }

  // DesignInletAirWetBulbTemperature

  if( (d = modelObject.designInletAirWetBulbTemperature()) )
  {
    idfObject.setDouble(CoolingTower_VariableSpeedFields::DesignInletAirWetBulbTemperature,d.get());
  }

  // DesignApproachTemperature

  if( (d = modelObject.designApproachTemperature()) )
  {
    idfObject.setDouble(CoolingTower_VariableSpeedFields::DesignApproachTemperature,d.get());
  }

  // DesignRangeTemperature

  if( (d = modelObject.designRangeTemperature()) )
  {
    idfObject.setDouble(CoolingTower_VariableSpeedFields::DesignRangeTemperature,d.get());
  }

  // DesignWaterFlowRate

  if( (d = modelObject.designWaterFlowRate()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::DesignWaterFlowRate,d.get());
  }
  else if( modelObject.isDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::DesignWaterFlowRate,"Autosize");
  }

  // DesignAirFlowRate

  if( (d = modelObject.designAirFlowRate()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::DesignAirFlowRate,d.get());
  }
  else if( modelObject.isDesignAirFlowRateAutosized() )
  {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::DesignAirFlowRate,"Autosize");
  }

  // DesignFanPower

  if( (d = modelObject.designFanPower()) )
  {
    idfObject.setDouble(CoolingTower_VariableSpeedFields::DesignFanPower,d.get());
  }
  else if( modelObject.isDesignFanPowerAutosized() )
  {
    idfObject.setString(CoolingTower_VariableSpeedFields::DesignFanPower,"Autosize");
  }

  // FanPowerRatioFunctionofAirFlowRateRatioCurve

  if( boost::optional<CurveCubic> curve = modelObject.fanPowerRatioFunctionofAirFlowRateRatioCurve() )
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(CoolingTower_VariableSpeedFields::FanPowerRatioFunctionofAirFlowRateRatioCurveName,_curve->name().get());
    }
  }

  // MinimumAirFlowRateRatio

  if( (d = modelObject.minimumAirFlowRateRatio()) )
  {
    idfObject.setDouble(CoolingTower_VariableSpeedFields::MinimumAirFlowRateRatio,d.get());
  }

  // FractionofTowerCapacityinFreeConvectionRegime

  if( (d = modelObject.fractionofTowerCapacityinFreeConvectionRegime()) )
  {
    idfObject.setDouble(CoolingTower_VariableSpeedFields::FractionofTowerCapacityinFreeConvectionRegime,d.get());
  }

  // BasinHeaterCapacity

  if( (d = modelObject.basinHeaterCapacity()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::BasinHeaterCapacity,d.get());
  }

  // BasinHeaterSetpointTemperature

  if( (d = modelObject.basinHeaterSetpointTemperature()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::BasinHeaterSetpointTemperature,d.get());
  }

  // BasinHeaterOperatingSchedule

  if( (temp = modelObject.basinHeaterOperatingSchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::BasinHeaterOperatingScheduleName,_schedule->name().get());
    }
  }

  // EvaporationLossMode

  if( (s = modelObject.evaporationLossMode()) )
  {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::EvaporationLossMode,s.get());
  }

  // EvaporationLossFactor

  if( (d = modelObject.evaporationLossFactor()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::EvaporationLossFactor,d.get());
  }

  // DriftLossPercent

  if( (d = modelObject.driftLossPercent()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::DriftLossPercent,d.get());
  }

  // BlowdownCalculationMode

  if( (s = modelObject.blowdownCalculationMode()) )
  {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::BlowdownCalculationMode,s.get());
  }

  // BlowdownConcentrationRatio

  if( (d = modelObject.blowdownConcentrationRatio()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::BlowdownConcentrationRatio,d.get());
  }

  // BlowdownMakeupWaterUsageScheduleName

  if( (temp = modelObject.blowdownMakeupWaterUsageSchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::BlowdownMakeupWaterUsageScheduleName,_schedule->name().get());
    }
  }

  // NumberofCells

  if( boost::optional<int> n = modelObject.numberofCells() )
  {
    idfObject.setUnsigned(openstudio::CoolingTower_VariableSpeedFields::NumberofCells,n.get());
  }

  // CellControl

  if( (s = modelObject.cellControl()) )
  {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::CellControl,s.get());
  }

  // CellMinimumWaterFlowRateFraction

  if( (d = modelObject.cellMinimumWaterFlowRateFraction()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::CellMinimumWaterFlowRateFraction,d.get());
  }

  // CellMaximumWaterFlowRateFraction

  if( (d = modelObject.cellMaximumWaterFlowRateFraction()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::CellMaximumWaterFlowRateFraction,d.get());
  }

  // SizingFactor

  if( (d = modelObject.sizingFactor()) )
  {
    idfObject.setDouble(openstudio::CoolingTower_VariableSpeedFields::SizingFactor,d.get());
  }

  if( (s = modelObject.endUseSubcategory()) ) {
    idfObject.setString(openstudio::CoolingTower_VariableSpeedFields::EndUseSubcategory,s.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

