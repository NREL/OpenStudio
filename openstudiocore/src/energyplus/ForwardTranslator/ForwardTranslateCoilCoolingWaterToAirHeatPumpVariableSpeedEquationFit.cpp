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
#include "../../model/Node.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>
#include "../../model/Curve.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit( CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit & modelObject )
{
  //setup boost optionals to use to store get method returns
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<Node> node;
  
  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit,modelObject);

  // WatertoRefrigerantHXWaterInletNodeName
  if( auto node = modelObject.waterInletModelObject() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName,node->name().get());
  }

  // WatertoRefrigerantHXWaterOutletNodeName
  if( auto node = modelObject.waterOutletModelObject() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName,node->name().get());
  }

  // IndoorAirInletNodeName
  if( auto node = modelObject.airInletModelObject() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName,node->name().get());
  }

  // IndoorAirOutletNodeName
  if( auto node = modelObject.airOutletModelObject() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName,node->name().get());
  }

  // NominalSpeedLevel
  if( auto speedLevel = modelObject.nominalSpeedLevel() ) {
    idfObject.setInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel,speedLevel);
  }

  // GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel
  if( modelObject.isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel,value.get());
  }

  // RatedAirFlowRateAtSelectedNominalSpeedLevel
  if( modelObject.isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedAirFlowRateAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,value.get());
  }

  // RatedWaterFlowRateAtSelectedNominalSpeedLevel
  if( modelObject.isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedWaterFlowRateAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,value.get());
  }

  // NominalTimeforCondensatetoBeginLeavingtheCoil
  if( (value = modelObject.nominalTimeforCondensatetoBeginLeavingtheCoil()) ) {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalTimeforCondensatetoBeginLeavingtheCoil,value.get());
  }

  // InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity
  if( (value = modelObject.initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity()) ) {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity,value.get());
  }

  // FlagforUsingHotGasReheat0or1
  if( modelObject.useHotGasReheat() ) {
    idfObject.setInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::FlagforUsingHotGasReheat0or1,1);
  } else {
    idfObject.setInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::FlagforUsingHotGasReheat0or1,0);
  }

  // EnergyPartLoadFractionCurveName
  { 
    auto curve = modelObject.energyPartLoadFractionCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName,_curve->name().get());
    }
  }

  auto const speeds = modelObject.speeds();

  // NumberofSpeeds
  if( auto num = speeds.size() ) {
    idfObject.setInt(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NumberofSpeeds,num);
  }

  for( auto const & speed : speeds ) {
    auto eg = idfObject.pushExtensibleGroup();

    // SpeedReferenceUnitGrossRatedTotalCoolingCapacity
    if( (value = speed.referenceUnitGrossRatedTotalCoolingCapacity()) ) {
      eg.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedTotalCoolingCapacity,value.get());
    }

    // SpeedReferenceUnitGrossRatedSensibleHeatRatio
    if( (value = speed.referenceUnitGrossRatedSensibleHeatRatio()) ) {
      eg.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedSensibleHeatRatio,value.get());
    }

    // SpeedReferenceUnitGrossRatedCoolingCOP
    if( (value = speed.referenceUnitGrossRatedCoolingCOP()) ) {
      eg.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedCoolingCOP,value.get());
    }

    // SpeedReferenceUnitRatedAirFlowRate
    if( (value = speed.referenceUnitRatedAirFlowRate()) ) {
      eg.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitRatedAirFlowRate,value.get());
    }

    // SpeedReferenceUnitRatedWaterFlowRate
    if( (value = speed.referenceUnitRatedWaterFlowRate()) ) {
      eg.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate,value.get());
    }

    // SpeedTotalCoolingCapacityFunctionofTemperatureCurveName
    { 
      auto curve = speed.totalCoolingCapacityFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

    // SpeedTotalCoolingCapacityFunctionofAirFlowFractionCurveName
    { 
      auto curve = speed.totalCoolingCapacityFunctionofAirFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalCoolingCapacityFunctionofAirFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedTotalCoolingCapacityFunctionofWaterFlowFractionCurveName
    { 
      auto curve = speed.totalCoolingCapacityFunctionofWaterFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalCoolingCapacityFunctionofWaterFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofTemperatureCurveName
    { 
      auto curve = speed.energyInputRatioFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName
    { 
      auto curve = speed.energyInputRatioFunctionofAirFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofWaterFlowFractionCurveName
    { 
      auto curve = speed.energyInputRatioFunctionofWaterFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofWaterFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions
    if( (value = speed.referenceUnitWasteHeatFractionofInputPowerAtRatedConditions()) ) {
      eg.setDouble(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions,value.get());
    }

    // SpeedWasteHeatFunctionofTemperatureCurveName
    { 
      auto curve = speed.wasteHeatFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName,_curve->name().get());
      }
    }
  
  }

  return idfObject;
}

} // energyplus

} // openstudio

