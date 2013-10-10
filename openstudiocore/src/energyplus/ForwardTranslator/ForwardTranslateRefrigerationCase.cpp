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
#include <model/RefrigerationCase.hpp>
#include <model/CurveCubic.hpp>
#include <model/ThermalZone.hpp>
#include <model/Schedule.hpp>

#include <utilities/idd/Refrigeration_Case_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCase( RefrigerationCase & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Case, modelObject);

// AvailabilityScheduleName
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      object.setString(Refrigeration_CaseFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

//Zone Name
  boost::optional<ThermalZone> thermalZone = modelObject.thermalZone();

  if( thermalZone )
  {
    boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone.get());

    if( _thermalZone && _thermalZone->name() )
    {
      object.setString(Refrigeration_CaseFields::ZoneName,_thermalZone->name().get());
    }
  }

//RatedAmbientTemperature
  d = modelObject.ratedAmbientTemperature();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::RatedAmbientTemperature,d.get());
  }

//RatedAmbientRelativeHumidity
  d = modelObject.ratedAmbientRelativeHumidity();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::RatedAmbientRelativeHumidity,d.get());
  }

//RatedTotalCoolingCapacityperUnitLength
  d = modelObject.ratedTotalCoolingCapacityperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength,d.get());
  }

//RatedLatentHeatRatio
  d = modelObject.ratedLatentHeatRatio();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::RatedLatentHeatRatio,d.get());
  }

//RatedRuntimeFraction
  d = modelObject.ratedRuntimeFraction();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::RatedRuntimeFraction,d.get());
  }

//CaseLength
  d = modelObject.caseLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::CaseLength,d.get());
  }

//CaseOperatingTemperature
  d = modelObject.caseOperatingTemperature();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::CaseOperatingTemperature,d.get());
  }

//LatentCaseCreditCurveType
  s = modelObject.latentCaseCreditCurveType();
  if (s) {
    object.setString(Refrigeration_CaseFields::LatentCaseCreditCurveType,s.get());
  }

//LatentCaseCreditCurveName
  boost::optional<CurveCubic> latentCaseCreditCurve = modelObject.latentCaseCreditCurve();

  if( latentCaseCreditCurve )
  {
    boost::optional<IdfObject> _latentCaseCreditCurve = translateAndMapModelObject(latentCaseCreditCurve.get());

    if( _latentCaseCreditCurve && _latentCaseCreditCurve->name() )
    {
      object.setString(Refrigeration_CaseFields::LatentCaseCreditCurveName,_latentCaseCreditCurve->name().get());
    }
  }

//StandardCaseFanPowerperUnitLength
  d = modelObject.standardCaseFanPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength,d.get());
  }

//OperatingCaseFanPowerperUnitLength
  d = modelObject.operatingCaseFanPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength,d.get());
  }

//StandardCaseLightingPowerperUnitLength
  d = modelObject.standardCaseLightingPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength,d.get());
  }

//InstalledCaseLightingPowerperUnitLength
  d = modelObject.installedCaseLightingPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength,d.get());
  }

//CaseLightingScheduleName
  boost::optional<Schedule> caseLightingSchedule = modelObject.caseLightingSchedule();

  if( caseLightingSchedule )
  {
    boost::optional<IdfObject> _caseLightingSchedule = translateAndMapModelObject(caseLightingSchedule.get());

    if( _caseLightingSchedule && _caseLightingSchedule->name() )
    {
      object.setString(Refrigeration_CaseFields::CaseLightingScheduleName,_caseLightingSchedule->name().get());
    }
  }

//FractionofLightingEnergytoCase
  d = modelObject.fractionofLightingEnergytoCase();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::FractionofLightingEnergytoCase,d.get());
  }

//CaseAntiSweatHeaterPowerperUnitLength
  d = modelObject.caseAntiSweatHeaterPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength,d.get());
  }

//MinimumAntiSweatHeaterPowerperUnitLength
  d = modelObject.minimumAntiSweatHeaterPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength,d.get());
  }

//AntiSweatHeaterControlType
  s = modelObject.antiSweatHeaterControlType();
  if (s) {
    object.setString(Refrigeration_CaseFields::AntiSweatHeaterControlType,s.get());
  }

//HumidityatZeroAntiSweatHeaterEnergy
  d = modelObject.humidityatZeroAntiSweatHeaterEnergy();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy,d.get());
  }

//CaseHeight
  d = modelObject.caseHeight();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::CaseHeight,d.get());
  }

//FractionofAntiSweatHeaterEnergytoCase
  d = modelObject.fractionofAntiSweatHeaterEnergytoCase();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase,d.get());
  }

//CaseDefrostPowerperUnitLength
  d = modelObject.caseDefrostPowerperUnitLength();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::CaseDefrostPowerperUnitLength,d.get());
  }

//CaseDefrostType
  s = modelObject.caseDefrostType();
  if (s) {
    object.setString(Refrigeration_CaseFields::CaseDefrostType,s.get());
  }

//CaseDefrostScheduleName
  boost::optional<Schedule> caseDefrostSchedule = modelObject.caseDefrostSchedule();

  if( caseDefrostSchedule )
  {
    boost::optional<IdfObject> _caseDefrostSchedule = translateAndMapModelObject(caseDefrostSchedule.get());

    if( _caseDefrostSchedule && _caseDefrostSchedule->name() )
    {
      object.setString(Refrigeration_CaseFields::CaseDefrostScheduleName,_caseDefrostSchedule->name().get());
    }
  }

//CaseDefrostDripDownScheduleName
  boost::optional<Schedule> caseDefrostDripDownSchedule = modelObject.caseDefrostDripDownSchedule();

  if( caseDefrostDripDownSchedule )
  {
    boost::optional<IdfObject> _caseDefrostDripDownSchedule = translateAndMapModelObject(caseDefrostDripDownSchedule.get());

    if( _caseDefrostDripDownSchedule && _caseDefrostDripDownSchedule->name() )
    {
      object.setString(Refrigeration_CaseFields::CaseDefrostDripDownScheduleName,_caseDefrostDripDownSchedule->name().get());
    }
  }

//DefrostEnergyCorrectionCurveType
  s = modelObject.defrostEnergyCorrectionCurveType();
  if (s) {
    object.setString(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType,s.get());
  }

//DefrostEnergyCorrectionCurveName
  boost::optional<CurveCubic> defrostEnergyCorrectionCurve = modelObject.defrostEnergyCorrectionCurve();

  if( defrostEnergyCorrectionCurve )
  {
    boost::optional<IdfObject> _defrostEnergyCorrectionCurve = translateAndMapModelObject(defrostEnergyCorrectionCurve.get());

    if( _defrostEnergyCorrectionCurve && _defrostEnergyCorrectionCurve->name() )
    {
      object.setString(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName,_defrostEnergyCorrectionCurve->name().get());
    }
  }

//UnderCaseHVACReturnAirFraction
  d = modelObject.underCaseHVACReturnAirFraction();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction,d.get());
  }

//RefrigeratedCaseRestockingScheduleName
  boost::optional<Schedule> refrigeratedCaseRestockingSchedule = modelObject.refrigeratedCaseRestockingSchedule();

  if( refrigeratedCaseRestockingSchedule )
  {
    boost::optional<IdfObject> _refrigeratedCaseRestockingSchedule = translateAndMapModelObject(refrigeratedCaseRestockingSchedule.get());

    if( _refrigeratedCaseRestockingSchedule && _refrigeratedCaseRestockingSchedule->name() )
    {
      object.setString(Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName,_refrigeratedCaseRestockingSchedule->name().get());
    }
  }

//CaseCreditFractionScheduleName
  boost::optional<Schedule> caseCreditFractionSchedule = modelObject.caseCreditFractionSchedule();

  if( caseCreditFractionSchedule )
  {
    boost::optional<IdfObject> _caseCreditFractionSchedule = translateAndMapModelObject(caseCreditFractionSchedule.get());

    if( _caseCreditFractionSchedule && _caseCreditFractionSchedule->name() )
    {
      object.setString(Refrigeration_CaseFields::CaseCreditFractionScheduleName,_caseCreditFractionSchedule->name().get());
    }
  }

//DesignEvaporatorTemperatureorBrineInletTemperature
  d = modelObject.designEvaporatorTemperatureorBrineInletTemperature();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::DesignEvaporatorTemperatureorBrineInletTemperature,d.get());
  }

//AverageRefrigerantChargeInventory
  d = modelObject.averageRefrigerantChargeInventory();
  if (d) {
    object.setDouble(Refrigeration_CaseFields::AverageRefrigerantChargeInventory,d.get());
  }

  return object;

}
}
}