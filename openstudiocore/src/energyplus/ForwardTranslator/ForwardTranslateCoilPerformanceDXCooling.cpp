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
#include "../../model/CoilPerformanceDXCooling.hpp"
#include "../../model/CoilPerformanceDXCooling_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilPerformance_DX_Cooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilPerformanceDXCooling( 
    CoilPerformanceDXCooling & modelObject)
{
  IdfObject idfObject(IddObjectType::CoilPerformance_DX_Cooling);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // GrossRatedTotalCoolingCapacity
  if( modelObject.isGrossRatedTotalCoolingCapacityAutosized() ) {
    idfObject.setString(CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity,"Autosize");
  } else if( auto value = modelObject.grossRatedTotalCoolingCapacity() ) {
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity,value.get());
  }

  // GrossRatedSensibleHeatRatio
  if( modelObject.isGrossRatedSensibleHeatRatioAutosized() ) {
    idfObject.setString(CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio,"Autosize");
  } else if( auto value = modelObject.grossRatedSensibleHeatRatio() ) {
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio,value.get());
  }

  // GrossRatedCoolingCOP
  {
    auto value = modelObject.grossRatedCoolingCOP();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::GrossRatedCoolingCOP,value);
  }

  // RatedAirFlowRate
  if( modelObject.isRatedAirFlowRateAutosized() ) {
    idfObject.setString(CoilPerformance_DX_CoolingFields::RatedAirFlowRate,"Autosize");
  } else if( auto value = modelObject.ratedAirFlowRate() ) {
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::RatedAirFlowRate,value.get());
  }

  // FractionofAirFlowBypassedAroundCoil
  {
    auto value = modelObject.fractionofAirFlowBypassedAroundCoil();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::FractionofAirFlowBypassedAroundCoil,value);
  }

  // TotalCoolingCapacityFunctionofTemperatureCurveName
  {
    auto curve = modelObject.totalCoolingCapacityFunctionofTemperatureCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  // TotalCoolingCapacityFunctionofFlowFractionCurveName
  {
    auto curve = modelObject.totalCoolingCapacityFunctionofFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofFlowFractionCurveName,idf->name().get());
    }
  }

  // EnergyInputRatioFunctionofTemperatureCurve
  {
    auto curve = modelObject.energyInputRatioFunctionofTemperatureCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  // EnergyInputRatioFunctionofFlowFractionCurve
  {
    auto curve = modelObject.energyInputRatioFunctionofFlowFractionCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofFlowFractionCurveName,idf->name().get());
    }
  }

  // PartLoadFractionCorrelationCurveName
  {
    auto curve = modelObject.partLoadFractionCorrelationCurve();
    if( auto idf = translateAndMapModelObject(curve) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::PartLoadFractionCorrelationCurveName,idf->name().get());
    }
  }

  // NominalTimeforCondensateRemovaltoBegin
  {
    auto value = modelObject.nominalTimeforCondensateRemovaltoBegin();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::NominalTimeforCondensateRemovaltoBegin,value);
  }

  // RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity
  {
    auto value = modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,value);
  }

  // MaximumCyclingRate
  {
    auto value = modelObject.maximumCyclingRate();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::MaximumCyclingRate,value);
  }

  // LatentCapacityTimeConstant
  {
    auto value = modelObject.latentCapacityTimeConstant();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::LatentCapacityTimeConstant,value);
  }
  
  // CondenserAirInletNode
  if( auto node = modelObject.condenserAirInletNode() ) {
    idfObject.setString(CoilPerformance_DX_CoolingFields::CondenserAirInletNodeName,node->name().get());
  }

  // CondenserType
  {
    auto value = modelObject.condenserType();
    idfObject.setString(CoilPerformance_DX_CoolingFields::CondenserType,value);
  }

  // EvaporativeCondenserEffectiveness
  {
    auto value = modelObject.evaporativeCondenserEffectiveness();
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::EvaporativeCondenserEffectiveness,value);
  }

  // EvaporativeCondenserAirFlowRate
  if( modelObject.isEvaporativeCondenserAirFlowRateAutosized() ) {
    idfObject.setString(CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate,"Autosize");
  } else if( auto value = modelObject.evaporativeCondenserAirFlowRate() ) {
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate,value.get());
  }

  // EvaporativeCondenserPumpRatedPowerConsumption
  if( modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() ) {
    idfObject.setString(CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  } else if( auto value = modelObject.evaporativeCondenserPumpRatedPowerConsumption() ) {
    idfObject.setDouble(CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption,value.get());
  }

  // SensibleHeatRatioFunctionofTemperatureCurveName
  if( auto curve = modelObject.sensibleHeatRatioFunctionofTemperatureCurve() ) {
    if( auto idf = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofTemperatureCurveName,idf->name().get());
    }
  }

  // SensibleHeatRatioFunctionofFlowFractionCurveName
  if( auto curve = modelObject.sensibleHeatRatioFunctionofFlowFractionCurve() ) {
    if( auto idf = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofFlowFractionCurveName,idf->name().get());
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
