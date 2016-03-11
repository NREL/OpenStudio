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

