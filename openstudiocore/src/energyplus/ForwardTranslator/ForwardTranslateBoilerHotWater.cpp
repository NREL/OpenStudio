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
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/BoilerHotWater.hpp>
#include <model/BoilerHotWater_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Boiler_HotWater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateBoilerHotWater( BoilerHotWater & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Boiler_HotWater);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // FuelType

  if( (s = modelObject.fuelType()) )
  {
    idfObject.setString(Boiler_HotWaterFields::FuelType,s.get());
  }

  // NominalCapacity

  if( modelObject.isNominalCapacityAutosized() )
  {
    idfObject.setString(Boiler_HotWaterFields::NominalCapacity,"Autosize");
  }
  else if( (value = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::NominalCapacity,value.get());
  }

  // NominalThermalEfficiency
  
  if( (value = modelObject.nominalThermalEfficiency()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::NominalThermalEfficiency,value.get());
  }

  // EfficiencyCurveTemperatureEvaluationVariable

  if ( (s = modelObject.efficiencyCurveTemperatureEvaluationVariable()) )
  {
    idfObject.setString(Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable,s.get());
  }

  // Normalized Boiler Efficiency Curve

  if (OptionalCurve curve = modelObject.normalizedBoilerEfficiencyCurve()) 
  {
    if( boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get()) )
    {
      idfObject.setString(Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName,_curve->name().get());
    }
  }

  // DesignWaterOutletTemperature

  if( (value = modelObject.designWaterOutletTemperature()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::DesignWaterOutletTemperature,value.get());
  }

  // DesignWaterFlowRate

  if( modelObject.isDesignWaterFlowRateAutosized() )
  {
    idfObject.setString(Boiler_HotWaterFields::DesignWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.designWaterFlowRate()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::DesignWaterFlowRate,value.get());
  }

  // MinimumPartLoadRatio

  if( (value = modelObject.minimumPartLoadRatio()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::MinimumPartLoadRatio,value.get());
  }

  // MaximumPartLoadRatio

  if( (value = modelObject.maximumPartLoadRatio()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::MaximumPartLoadRatio,value.get());
  }

  // OptimumPartLoadRatio

  if( (value = modelObject.optimumPartLoadRatio()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::OptimumPartLoadRatio,value.get());
  }

  // BoilerWaterInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Boiler_HotWaterFields::BoilerWaterInletNodeName,node->name().get());
    }
  }

  // BoilerWaterOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Boiler_HotWaterFields::BoilerWaterOutletNodeName,node->name().get());
    }
  }

  // WaterOutletUpperTemperatureLimit

  if( (value = modelObject.waterOutletUpperTemperatureLimit()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit,value.get());
  }

  // BoilerFlowMode

  if( (s = modelObject.boilerFlowMode()) )
  {
    idfObject.setString(Boiler_HotWaterFields::BoilerFlowMode,s.get());
  } 

  // ParasiticElectricLoad

  if( (value = modelObject.parasiticElectricLoad()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::ParasiticElectricLoad,value.get());
  }

  // SizingFactor

  if( (value = modelObject.sizingFactor()) )
  {
    idfObject.setDouble(Boiler_HotWaterFields::SizingFactor,value.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

//OPENSTUDIO_ENUM( Boiler_HotWaterFields,
//  ((NormalizedBoilerEfficiencyCurveName)(Normalized Boiler Efficiency Curve Name))

} // energyplus

} // openstudio

