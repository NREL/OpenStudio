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
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/CoilSystem_Heating_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXVariableSpeedWithoutUnitary( CoilHeatingDXVariableSpeed & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_DX_VariableSpeed, modelObject);

  // IndoorAirInletNodeName
  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName,node->name().get());
  }

  // IndoorAirOutletNodeName
  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName,node->name().get());
  }

  // NominalSpeedLevel
  if( auto speedLevel = modelObject.nominalSpeedLevel() ) {
    idfObject.setInt(Coil_Heating_DX_VariableSpeedFields::NominalSpeedLevel,speedLevel);
  }

  // RatedHeatingCapacityAtSelectedNominalSpeedLevel
  if( modelObject.isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedHeatingCapacityAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,value.get());
  }

  // RatedAirFlowRateAtSelectedNominalSpeedLevel
  if( modelObject.isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,"AutoSize");
  } else if( (value = modelObject.ratedAirFlowRateAtSelectedNominalSpeedLevel()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,value.get());
  }

  // EnergyPartLoadFractionCurveName
  {
    auto curve = modelObject.energyPartLoadFractionCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName,_curve->name().get());
    }
  }

  // DefrostEnergyInputRatioFunctionofTemperatureCurveName
  if( auto curve = modelObject.defrostEnergyInputRatioFunctionofTemperatureCurve() ) {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName,_curve->name().get());
    }
  }

  // MinimumOutdoorDryBulbTemperatureforCompressorOperation
  if( (value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,value.get());
  }

  // OutdoorDryBulbTemperaturetoTurnOnCompressor
  if( (value = modelObject.outdoorDryBulbTemperaturetoTurnOnCompressor()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforDefrostOperation
  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforDefrostOperation()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,value.get());
  }

  // CrankcaseHeaterCapacity
  if( (value = modelObject.crankcaseHeaterCapacity()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacity,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,value.get());
  }

  // DefrostStrategy
  if( (s = modelObject.defrostStrategy()) ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::DefrostStrategy,s.get());
  }

  // DefrostControl
  if( (s = modelObject.defrostControl()) ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::DefrostControl,s.get());
  }

  // DefrostTimePeriodFraction
  if( (value = modelObject.defrostTimePeriodFraction()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::DefrostTimePeriodFraction,value.get());
  }

  // ResistiveDefrostHeaterCapacity
  if( modelObject.isResistiveDefrostHeaterCapacityAutosized() ) {
    idfObject.setString(Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity,"AutoSize");
  } else if( (value = modelObject.resistiveDefrostHeaterCapacity()) ) {
    idfObject.setDouble(Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity,value.get());
  }

  auto const speeds = modelObject.speeds();

  // NumberofSpeeds
  if( auto num = speeds.size() ) {
    idfObject.setInt(Coil_Heating_DX_VariableSpeedFields::NumberofSpeeds,num);
  }

  for( auto const & speed : speeds ) {
    auto eg = idfObject.pushExtensibleGroup();

    // SpeedReferenceUnitGrossRatedHeatingCapacity
    if( (value = speed.referenceUnitGrossRatedHeatingCapacity()) ) {
      eg.setDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedHeatingCapacity,value.get());
    }

    // SpeedReferenceUnitGrossRatedHeatingCOP
    if( (value = speed.referenceUnitGrossRatedHeatingCOP()) ) {
      eg.setDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedHeatingCOP,value.get());
    }

    // SpeedReferenceUnitRatedAirFlowRate
    if( (value = speed.referenceUnitRatedAirFlowRate()) ) {
      eg.setDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate,value.get());
    }

    // SpeedHeatingCapacityFunctionofTemperatureCurveName
    {
      auto curve = speed.heatingCapacityFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

    // SpeedTotalHeatingCapacityFunctionofAirFlowFractionCurveName
    {
      auto curve = speed.totalHeatingCapacityFunctionofAirFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedTotalHeatingCapacityFunctionofAirFlowFractionCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofTemperatureCurveName
    {
      auto curve = speed.energyInputRatioFunctionofTemperatureCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName,_curve->name().get());
      }
    }

    // SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName
    {
      auto curve = speed.energyInputRatioFunctionofAirFlowFractionCurve();
      if( auto _curve = translateAndMapModelObject(curve) ) {
        eg.setString(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName,_curve->name().get());
      }
    }

  }

  return idfObject;
}

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXVariableSpeed( CoilHeatingDXVariableSpeed& modelObject )
{
  IdfObject coilSystemHeatingDXIdf(IddObjectType::CoilSystem_Heating_DX);

  m_idfObjects.push_back(coilSystemHeatingDXIdf);

  boost::optional<IdfObject> idfObject = translateCoilHeatingDXVariableSpeedWithoutUnitary(modelObject);

  if( ! idfObject ) { return boost::none; }

  if( auto s = modelObject.name() )
  {
    coilSystemHeatingDXIdf.setString(CoilSystem_Heating_DXFields::HeatingCoilObjectType,idfObject->iddObject().name());
    coilSystemHeatingDXIdf.setString(CoilSystem_Heating_DXFields::HeatingCoilName,s.get());
    coilSystemHeatingDXIdf.setName(s.get() + " CoilSystem");
  }

  return coilSystemHeatingDXIdf;
}

} // energyplus

} // openstudio

