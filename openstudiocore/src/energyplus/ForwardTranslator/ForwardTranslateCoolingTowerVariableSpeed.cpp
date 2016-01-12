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

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

