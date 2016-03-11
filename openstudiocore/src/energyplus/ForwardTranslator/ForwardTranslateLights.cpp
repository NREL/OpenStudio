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
#include "../../model/Lights.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Space.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/LifeCycleCost.hpp"
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateLights( Lights & modelObject )
{
  // create, register, and name object
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Lights,
                                                       modelObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  LightsDefinition definition = modelObject.lightsDefinition();

  OptionalIdfObject relatedIdfObject;

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      relatedIdfObject = translateAndMapModelObject(*thermalZone);
      OS_ASSERT(relatedIdfObject);
      idfObject.setString(LightsFields::ZoneorZoneListName,
                          relatedIdfObject->name().get());
    }
  }else if(spaceType){
    relatedIdfObject = translateAndMapModelObject(*spaceType);
    OS_ASSERT(relatedIdfObject);
    idfObject.setString(LightsFields::ZoneorZoneListName,
                        relatedIdfObject->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    relatedIdfObject = translateAndMapModelObject(*schedule);
    OS_ASSERT(relatedIdfObject);
    idfObject.setString(LightsFields::ScheduleName, relatedIdfObject->name().get());
  }

  idfObject.setString(LightsFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

  double multiplier = modelObject.multiplier();

  boost::optional<double> d = definition.lightingLevel();
  if (d){
    idfObject.setDouble(LightsFields::LightingLevel, (*d)*multiplier);
  }

  d = definition.wattsperSpaceFloorArea();
  if (d){
    idfObject.setDouble(LightsFields::WattsperZoneFloorArea, (*d)*multiplier);
  }

  d = definition.wattsperPerson();
  if (d){
    idfObject.setDouble(LightsFields::WattsperPerson, (*d)*multiplier);
  }

  if (!definition.isReturnAirFractionDefaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFraction, definition.returnAirFraction());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(LightsFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionVisibleDefaulted()){
    idfObject.setDouble(LightsFields::FractionVisible, definition.fractionVisible());
  }

  if (!modelObject.isFractionReplaceableDefaulted()){
    idfObject.setDouble(LightsFields::FractionReplaceable, modelObject.fractionReplaceable());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(LightsFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  if (!definition.isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted()){
    if (definition.returnAirFractionCalculatedfromPlenumTemperature()){
      idfObject.setString(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature,"Yes");
    }else{
      idfObject.setString(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature,"No");
    }
  }

  if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,
                        definition.returnAirFractionFunctionofPlenumTemperatureCoefficient1());
  }

  if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,
                        definition.returnAirFractionFunctionofPlenumTemperatureCoefficient2());
  }

  return idfObject;
}

} // energyplus

} // openstudio

