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
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationCase_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/time/Time.hpp"

#include <utilities/idd/Refrigeration_Case_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
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

//CaseDefrostType
  s = modelObject.caseDefrostType();
  if (s) {
    object.setString(Refrigeration_CaseFields::CaseDefrostType,s.get());
  }

//DefrostCycleParameters
  boost::optional<int> durationofDefrostCycle = modelObject.durationofDefrostCycle();
  boost::optional<int> dripDownTime = modelObject.dripDownTime();
  std::vector<openstudio::Time> defrostStartTimes = modelObject.getImpl<model::detail::RefrigerationCase_Impl>()->defrostStartTimes();

  if( durationofDefrostCycle && dripDownTime && !defrostStartTimes.empty() ) {
    int defrostTimeHour = *durationofDefrostCycle / 60;
    int defrostTimeMin = *durationofDefrostCycle % 60;
    int dripDownTimeHour = *dripDownTime / 60;
    int dripDownTimeMin = *dripDownTime % 60;

    std::vector< std::pair<openstudio::Time, double> > defrostDefaultDay;
    std::vector< std::pair<openstudio::Time, double> > dripDownDefaultDay;
    for( auto _defrostStartTime = defrostStartTimes.begin();
       _defrostStartTime != defrostStartTimes.end();
       ++_defrostStartTime )
    {
      defrostDefaultDay.push_back(std::make_pair(*_defrostStartTime, 0)); // defrost off
      openstudio::Time defrostEndTime(0, _defrostStartTime->hours() + defrostTimeHour, _defrostStartTime->minutes() + defrostTimeMin);
      defrostDefaultDay.push_back(std::make_pair(defrostEndTime, 1)); // defrost on

      dripDownDefaultDay.push_back(std::make_pair(*_defrostStartTime, 0)); // drip down off
      openstudio::Time dripDownEndTime(0, _defrostStartTime->hours() + defrostTimeHour + dripDownTimeHour, _defrostStartTime->minutes() + defrostTimeMin + dripDownTimeMin);
      dripDownDefaultDay.push_back(std::make_pair(dripDownEndTime, 1)); // drip down on
    }

    if( (defrostStartTimes.front().hours() != 0 && defrostStartTimes.front().minutes() != 0) || defrostStartTimes.back().hours() < 24) {
      openstudio::Time defrostDayEnd(0, 24, 0);
      defrostDefaultDay.push_back(std::make_pair(defrostDayEnd, 0)); // defrost off
      dripDownDefaultDay.push_back(std::make_pair(defrostDayEnd, 0)); // drip down off
    }

    //CaseDefrostScheduleName
    std::string defrostName(modelObject.name().get() + " Defrost Schedule");
    boost::optional<IdfObject> defrostSchedule = this->createSimpleSchedule(defrostName, defrostDefaultDay);
    if( defrostSchedule ) {
      object.setString(Refrigeration_CaseFields::CaseDefrostScheduleName, defrostName);
    }

    //CaseDefrostDripDownScheduleName
    std::string dripDownName(modelObject.name().get() + " Defrost Drip Down Schedule");
    boost::optional<IdfObject> defrostDripDownSchedule = this->createSimpleSchedule(dripDownName, dripDownDefaultDay);
    if( defrostDripDownSchedule ) {
      object.setString(Refrigeration_CaseFields::CaseDefrostDripDownScheduleName, dripDownName);
    }
  } else {
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
  }

  //DefrostEnergyCorrectionCurveType
  {
    auto value = modelObject.defrostEnergyCorrectionCurveType();
    object.setString(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType,value);
  }

  //DefrostEnergyCorrectionCurveName
  if( auto defrostEnergyCorrectionCurve = modelObject.defrostEnergyCorrectionCurve() )
  {
    auto type = modelObject.caseDefrostType();
    // Only for these types or E+ will halt.
    if( istringEqual(type,"HotGasWithTemperatureTermination") ||
      istringEqual(type,"ElectricWithTemperatureTermination") ||
      istringEqual(type,"HotFluidWithTemperatureTermination") ) {
      boost::optional<IdfObject> _defrostEnergyCorrectionCurve = translateAndMapModelObject(defrostEnergyCorrectionCurve.get());

      if( _defrostEnergyCorrectionCurve && _defrostEnergyCorrectionCurve->name() )
      {
        object.setString(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName,_defrostEnergyCorrectionCurve->name().get());
      }
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

  boost::optional<double> ratedTotalCoolingCapacity;
  boost::optional<double> standardCaseFanPower;
  boost::optional<double> operatingCaseFanPower;
  boost::optional<double> standardCaseLightingPower;
  boost::optional<double> installedCaseLightingPower;
  boost::optional<double> caseAntiSweatHeaterPower;
  boost::optional<double> minimumAntiSweatHeaterPower;
  boost::optional<double> caseDefrostPower;

  std::string const unitType = modelObject.unitType();
  if(istringEqual("UnitLength", unitType)) {
    ratedTotalCoolingCapacity = modelObject.ratedTotalCoolingCapacityperUnitLength();
    standardCaseFanPower = modelObject.standardCaseFanPowerperUnitLength();
    operatingCaseFanPower = modelObject.operatingCaseFanPowerperUnitLength();
    standardCaseLightingPower = modelObject.standardCaseLightingPowerperUnitLength();
    installedCaseLightingPower = modelObject.installedCaseLightingPowerperUnitLength();
    caseAntiSweatHeaterPower = modelObject.caseAntiSweatHeaterPowerperUnitLength();
    minimumAntiSweatHeaterPower = modelObject.minimumAntiSweatHeaterPowerperUnitLength();
    caseDefrostPower = modelObject.caseDefrostPowerperUnitLength();
  }
  else { // NumberOfDoors
    boost::optional<int> numberOfDoors = modelObject.numberOfDoors();
    boost::optional<double> caseLength = modelObject.caseLength();
    if( !numberOfDoors ) {
      LOG(Error, "Missing required input 'NumberOfDoors' for Refrigeration:Case named '" << modelObject.name().get() << "'");
    }
    if( !caseLength ) {
      LOG(Error, "Missing required input 'CaseLength' for Refrigeration:Case named '" << modelObject.name().get() << "'");
    }
    double conversion = numberOfDoors.get() / caseLength.get();

    if( ( ratedTotalCoolingCapacity = modelObject.ratedTotalCoolingCapacityperDoor() ) ) {
      ratedTotalCoolingCapacity = ratedTotalCoolingCapacity.get() * conversion;
    }
    if( ( standardCaseFanPower = modelObject.standardCaseFanPowerperDoor() ) ) {
      standardCaseFanPower = standardCaseFanPower.get() * conversion;
    }
    if( ( operatingCaseFanPower = modelObject.operatingCaseFanPowerperDoor() ) ) {
      operatingCaseFanPower = operatingCaseFanPower.get() * conversion;
    }
    if( ( standardCaseLightingPower = modelObject.standardCaseLightingPowerperDoor() ) ) {
      standardCaseLightingPower = standardCaseLightingPower.get() * conversion;
    }
    if( ( installedCaseLightingPower = modelObject.installedCaseLightingPowerperDoor() ) ) {
      installedCaseLightingPower = installedCaseLightingPower.get() * conversion;
    }
    if( ( caseAntiSweatHeaterPower = modelObject.caseAntiSweatHeaterPowerperDoor() ) ) {
      caseAntiSweatHeaterPower = caseAntiSweatHeaterPower.get() * conversion;
    }
    if( ( minimumAntiSweatHeaterPower = modelObject.minimumAntiSweatHeaterPowerperDoor() ) ) {
      minimumAntiSweatHeaterPower = minimumAntiSweatHeaterPower.get() * conversion;
    }
    if( ( caseDefrostPower = modelObject.caseDefrostPowerperDoor() ) ) {
      caseDefrostPower = caseDefrostPower.get() * conversion;
    }
  }

//RatedTotalCoolingCapacityperUnitLength
  if (ratedTotalCoolingCapacity) {
    object.setDouble(Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength,ratedTotalCoolingCapacity.get());
  }

//StandardCaseFanPowerperUnitLength
  if (standardCaseFanPower) {
    object.setDouble(Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength,standardCaseFanPower.get());
  }

//OperatingCaseFanPowerperUnitLength
  if (operatingCaseFanPower) {
    object.setDouble(Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength,operatingCaseFanPower.get());
  }

//StandardCaseLightingPowerperUnitLength
  if (standardCaseLightingPower) {
    object.setDouble(Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength,standardCaseLightingPower.get());
  }

//InstalledCaseLightingPowerperUnitLength
  if (installedCaseLightingPower) {
    object.setDouble(Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength,installedCaseLightingPower.get());
  }

//CaseAntiSweatHeaterPowerperUnitLength
  if (caseAntiSweatHeaterPower) {
    object.setDouble(Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength,caseAntiSweatHeaterPower.get());
  }

//MinimumAntiSweatHeaterPowerperUnitLength
  if (minimumAntiSweatHeaterPower) {
    object.setDouble(Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength,minimumAntiSweatHeaterPower.get());
  }

//CaseDefrostPowerperUnitLength
  if (caseDefrostPower) {
    object.setDouble(Refrigeration_CaseFields::CaseDefrostPowerperUnitLength,caseDefrostPower.get());
  }

  return object;

}
}
}
