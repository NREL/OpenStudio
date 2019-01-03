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
#include "../../model/Node.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>
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

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit & modelObject)
{
  //setup boost optionals to use to store get method returns
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<Node> node;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit,modelObject);

  // WatertoRefrigerantHXWaterInletNodeName
  if( auto node = modelObject.waterInletModelObject() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName,node->name().get());
  }

  // WatertoRefrigerantHXWaterOutletNodeName
  if( auto node = modelObject.waterOutletModelObject() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName,node->name().get());
  }

  // IndoorAirInletNodeName
  if( auto node = modelObject.airInletModelObject() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName,node->name().get());
  }

  // IndoorAirOutletNodeName
  if( auto node = modelObject.airOutletModelObject() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName,node->name().get());
  }

  // NominalSpeedLevel
  if( auto speedLevel = modelObject.nominalSpeedLevel() ) {
    idfObject.setInt(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel,speedLevel);
  }

  // RatedHeatingCapacityAtSelectedNominalSpeedLevel
  if( modelObject.isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedHeatingCapacityAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,value.get());
  }

  // RatedAirFlowRateAtSelectedNominalSpeedLevel
  if( modelObject.isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedAirFlowRateAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,value.get());
  }

  // RatedWaterFlowRateAtSelectedNominalSpeedLevel
  if( modelObject.isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedWaterFlowRateAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,value.get());
  }

  // EnergyPartLoadFractionCurveName
  {
    auto curve = modelObject.energyPartLoadFractionCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName,_curve->name().get());
    }
  }

  auto const speeds = modelObject.speeds();

  // NumberofSpeeds
  if( auto num = speeds.size() ) {
    idfObject.setInt(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::NumberofSpeeds,num);
  }

  for( auto const & speed : speeds ) {
    auto eg = idfObject.pushExtensibleGroup();

    // SpeedReferenceUnitGrossRatedHeatingCapacity
    if( (value = speed.referenceUnitGrossRatedHeatingCapacity()) ) {
      eg.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedHeatingCapacity,value.get());
    }

    // SpeedReferenceUnitGrossRatedHeatingCOP
    if( (value = speed.referenceUnitGrossRatedHeatingCOP()) ) {
      eg.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitGrossRatedHeatingCOP,value.get());
    }

    // SpeedReferenceUnitRatedAirFlow
    if( (value = speed.referenceUnitRatedAirFlow()) ) {
      eg.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitRatedAirFlow,value.get());
    }

    // SpeedReferenceUnitRatedWaterFlowRate
    if( (value = speed.referenceUnitRatedWaterFlowRate()) ) {
      eg.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate,value.get());
    }

    // SpeedHeatingCapacityFunctionofTemperatureCurveName
    {
      auto curve = speed.heatingCapacityFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

    // SpeedTotalHeatingCapacityFunctionofAirFlowFractionCurveName
    {
      auto curve = speed.totalHeatingCapacityFunctionofAirFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedTotalHeatingCapacityFunctionofAirFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedHeatingCapacityFunctionofWaterFlowFractionCurveName
    {
      auto curve = speed.heatingCapacityFunctionofWaterFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedHeatingCapacityFunctionofWaterFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofTemperatureCurveName
    {
      auto curve = speed.energyInputRatioFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName
    {
      auto curve = speed.energyInputRatioFunctionofAirFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofWaterFlowFractionCurveName
    {
      auto curve = speed.energyInputRatioFunctionofWaterFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedEnergyInputRatioFunctionofWaterFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions
    if( (value = speed.referenceUnitWasteHeatFractionofInputPowerAtRatedConditions()) ) {
      eg.setDouble(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions,value.get());
    }

    // SpeedWasteHeatFunctionofTemperatureCurveName
    {
      auto curve = speed.wasteHeatFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

  }

  return idfObject;
}

} // energyplus

} // openstudio

