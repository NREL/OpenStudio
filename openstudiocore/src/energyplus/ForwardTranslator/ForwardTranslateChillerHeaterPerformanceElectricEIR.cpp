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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/ChillerHeaterPerformance_Electric_EIR_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateChillerHeaterPerformanceElectricEIR( ChillerHeaterPerformanceElectricEIR & modelObject )
{
  OptionalString s;
  OptionalDouble value;
  OptionalModelObject temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ChillerHeaterPerformance_Electric_EIR, modelObject);

  // ReferenceCoolingModeEvaporatorCapacity
  if( modelObject.isReferenceCoolingModeEvaporatorCapacityAutosized() ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity,"Autosize");
  }
  else if( (value = modelObject.referenceCoolingModeEvaporatorCapacity()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity,value.get()); 
  }

  // ReferenceCoolingModeCOP
  if( (value = modelObject.referenceCoolingModeCOP()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeCOP,value.get()); 
  }

  // ReferenceCoolingModeLeavingChilledWaterTemperature
  if( (value = modelObject.referenceCoolingModeLeavingChilledWaterTemperature()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingChilledWaterTemperature,value.get()); 
  }

  // ReferenceCoolingModeEnteringCondenserFluidTemperature
  if( (value = modelObject.referenceCoolingModeEnteringCondenserFluidTemperature()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEnteringCondenserFluidTemperature,value.get()); 
  }

  // ReferenceCoolingModeLeavingCondenserWaterTemperature
  if( (value = modelObject.referenceCoolingModeLeavingCondenserWaterTemperature()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingCondenserWaterTemperature,value.get()); 
  }

  // ReferenceHeatingModeCoolingCapacityRatio
  if( (value = modelObject.referenceHeatingModeCoolingCapacityRatio()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingCapacityRatio,value.get()); 
  }

  // ReferenceHeatingModeCoolingPowerInputRatio
  if( (value = modelObject.referenceHeatingModeCoolingPowerInputRatio()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingPowerInputRatio,value.get()); 
  }

  // ReferenceHeatingModeLeavingChilledWaterTemperature
  if( (value = modelObject.referenceHeatingModeLeavingChilledWaterTemperature()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingChilledWaterTemperature,value.get()); 
  }

  // ReferenceHeatingModeLeavingCondenserWaterTemperature
  if( (value = modelObject.referenceHeatingModeLeavingCondenserWaterTemperature()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingCondenserWaterTemperature,value.get()); 
  }

  // ReferenceHeatingModeEnteringCondenserFluidTemperature
  if( (value = modelObject.referenceHeatingModeEnteringCondenserFluidTemperature()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeEnteringCondenserFluidTemperature,value.get()); 
  }

  // HeatingModeEnteringChilledWaterTemperatureLowLimit
  if( (value = modelObject.heatingModeEnteringChilledWaterTemperatureLowLimit()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::HeatingModeEnteringChilledWaterTemperatureLowLimit,value.get()); 
  }

  // ChilledWaterFlowModeType
  if( (s = modelObject.chilledWaterFlowModeType()) ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType,s.get()); 
  }

  // DesignChilledWaterFlowRate
  if( modelObject.isDesignChilledWaterFlowRateAutosized() ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.designChilledWaterFlowRate()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate,value.get()); 
  }

  // DesignCondenserWaterFlowRate
  if( modelObject.isDesignCondenserWaterFlowRateAutosized() ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.designCondenserWaterFlowRate()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate,value.get()); 
  }

  // DesignHotWaterFlowRate
  if( (value = modelObject.designHotWaterFlowRate()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::DesignHotWaterFlowRate,value.get()); 
  }

  // CompressorMotorEfficiency
  if( (value = modelObject.compressorMotorEfficiency()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::CompressorMotorEfficiency,value.get()); 
  }

  // CondenserType
  if( (s = modelObject.condenserType()) ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::CondenserType,s.get()); 
  }

  // CoolingModeTemperatureCurveCondenserWaterIndependentVariable
  if( (s = modelObject.coolingModeTemperatureCurveCondenserWaterIndependentVariable()) ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable,s.get()); 
  }

  // CoolingModeCoolingCapacityFunctionofTemperatureCurveName
  { 
    auto curve = modelObject.coolingModeCoolingCapacityFunctionofTemperatureCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName
  { 
    auto curve = modelObject.coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName
  { 
    auto curve = modelObject.coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName,_curve->name().get());
    }
  }

  // CoolingModeCoolingCapacityOptimumPartLoadRatio
  if( (value = modelObject.coolingModeCoolingCapacityOptimumPartLoadRatio()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityOptimumPartLoadRatio,value.get()); 
  }

  // HeatingModeTemperatureCurveCondenserWaterIndependentVariable
  if( (s = modelObject.heatingModeTemperatureCurveCondenserWaterIndependentVariable()) ) {
    idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable,s.get()); 
  }

  // HeatingModeCoolingCapacityFunctionofTemperatureCurveName
  { 
    auto curve = modelObject.heatingModeCoolingCapacityFunctionofTemperatureCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName
  { 
    auto curve = modelObject.heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName
  { 
    auto curve = modelObject.heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName,_curve->name().get());
    }
  }

  // HeatingModeCoolingCapacityOptimumPartLoadRatio
  if( (value = modelObject.heatingModeCoolingCapacityOptimumPartLoadRatio()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityOptimumPartLoadRatio,value.get()); 
  }

  // SizingFactor
  if( (value = modelObject.sizingFactor()) ) {
    idfObject.setDouble(ChillerHeaterPerformance_Electric_EIRFields::SizingFactor,value.get()); 
  }

  return idfObject;
}

} // energyplus

} // openstudio

