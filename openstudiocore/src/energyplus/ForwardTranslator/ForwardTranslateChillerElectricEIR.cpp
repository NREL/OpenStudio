/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ChillerElectricEIR.hpp>
#include <model/ChillerElectricEIR_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Chiller_Electric_EIR_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateChillerElectricEIR( ChillerElectricEIR & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Chiller_Electric_EIR);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // ReferenceCapacity

  if( modelObject.isReferenceCapacityAutosized() )
  {
    idfObject.setString(Chiller_Electric_EIRFields::ReferenceCapacity,"Autosize");
  }
  else if( (value = modelObject.referenceCapacity()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceCapacity,value.get()); 
  }

  // ReferenceCOP
  
  if( (value = modelObject.referenceCOP()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceCOP,value.get());
  }

  // ReferenceLeavingChilledWaterTemperature

  if( (value = modelObject.referenceLeavingChilledWaterTemperature()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature,value.get());
  }

  // ReferenceEnteringCondenserFluidTemperature

  if( (value = modelObject.referenceEnteringCondenserFluidTemperature()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature,value.get());
  }

  // ReferenceChilledWaterFlowRate

  if( modelObject.isReferenceChilledWaterFlowRateAutosized() )
  {
    idfObject.setString(Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.referenceChilledWaterFlowRate()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate,value.get());
  }

  // ReferenceCondenserFluidFlowRate

  if( modelObject.isReferenceCondenserFluidFlowRateAutosized() )
  {
    idfObject.setString(Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate,"Autosize");
  }
  else if( (value = modelObject.referenceCondenserFluidFlowRate()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate,value.get());
  }

  // MinimumPartLoadRatio

  if( (value = modelObject.minimumPartLoadRatio()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::MinimumPartLoadRatio,value.get());
  }

  // MaximumPartLoadRatio

  if( (value = modelObject.maximumPartLoadRatio()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::MaximumPartLoadRatio,value.get());
  }

  // OptimumPartLoadRatio

  if( (value = modelObject.optimumPartLoadRatio()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::OptimumPartLoadRatio,value.get());
  }

  // MinimumUnloadingRatio
  
  if( (value = modelObject.minimumUnloadingRatio()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::MinimumUnloadingRatio,value.get());
  }

  // ChilledWaterInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.supplyInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Chiller_Electric_EIRFields::ChilledWaterInletNodeName,node->name().get());
    }
  }

  // ChilledWaterOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.supplyOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Chiller_Electric_EIRFields::ChilledWaterOutletNodeName,node->name().get());
    }
  }

  // CondenserInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.demandInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Chiller_Electric_EIRFields::CondenserInletNodeName,node->name().get());
    }
  }

  // CondenserOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.demandOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Chiller_Electric_EIRFields::CondenserOutletNodeName,node->name().get());
    }
  }

  // CondenserType

  if( modelObject.demandInletModelObject() )
  {
    idfObject.setString(Chiller_Electric_EIRFields::CondenserType,"WaterCooled");
  }
  else
  {
    idfObject.setString(Chiller_Electric_EIRFields::CondenserType,"AirCooled");
  }

  // CondenserFanPowerRatio

  if( (value = modelObject.condenserFanPowerRatio()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::CondenserFanPowerRatio,value.get());
  }

  // CompressorMotorEfficiency
  // Changed to Fraction of Compressor Electric Consumption Rejected by Condenser in E+ version 8.0

  if( (value = modelObject.compressorMotorEfficiency()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser,value.get());
  }

  // LeavingChilledWaterLowerTemperatureLimit

  if( (value = modelObject.leavingChilledWaterLowerTemperatureLimit()) )
  {
    idfObject.setDouble(Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit,value.get());
  }

  // ChillerFlowMode

  if( (s = modelObject.chillerFlowMode()) )
  {
    idfObject.setString(Chiller_Electric_EIRFields::ChillerFlowMode,s.get());
  }

  // CoolingCapacityFunctionofTemperatureCurveName

  CurveBiquadratic curve = modelObject.coolingCapacityFunctionOfTemperature();

  boost::optional<IdfObject> _curve = translateAndMapModelObject(curve);

  if( _curve )
  {
    idfObject.setString(Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName,_curve->name().get());
  }

  // ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName

  curve = modelObject.electricInputToCoolingOutputRatioFunctionOfTemperature();

  _curve = translateAndMapModelObject(curve);

  if( _curve )
  {
    idfObject.setString(Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName,_curve->name().get());
  }

  // ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName

  CurveQuadratic quadcurve = modelObject.electricInputToCoolingOutputRatioFunctionOfPLR();

  _curve = translateAndMapModelObject(quadcurve);

  if( _curve )
  {
    idfObject.setString(Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName,_curve->name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

//OPENSTUDIO_ENUM( Chiller_Electric_EIRFields,
//  ((Name)(Name))
//  ((CoolingCapacityFunctionofTemperatureCurveName)(Cooling Capacity Function of Temperature Curve Name))
//  ((ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName)(Electric Input to Cooling Output Ratio Function of Temperature Curve Name))
//  ((ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName)(Electric Input to Cooling Output Ratio Function of Fan Coil Part Load Ratio Curve Name))
//  ((CondenserInletNodeName)(Condenser Inlet Node Name))
//  ((CondenserOutletNodeName)(Condenser Outlet Node Name))
//  ((DesignHeatRecoveryWaterFlowRate)(Design Heat Recovery Water Flow Rate))
//  ((HeatRecoveryInletNodeName)(Heat Recovery Inlet Node Name))
//  ((HeatRecoveryOutletNodeName)(Heat Recovery Outlet Node Name))
//  ((SizingFactor)(Sizing Factor))
//  ((BasinHeaterCapacity)(Basin Heater Capacity))
//  ((BasinHeaterSetpointTemperature)(Basin Heater Setpoint Temperature))
//  ((BasinHeaterOperatingScheduleName)(Basin Heater Operating Schedule Name))
//);

} // energyplus

} // openstudio

