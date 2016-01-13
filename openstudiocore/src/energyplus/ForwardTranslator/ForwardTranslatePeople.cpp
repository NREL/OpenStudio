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
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/People_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePeople( People & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::People);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  PeopleDefinition definition = modelObject.peopleDefinition();

  idfObject.setString(PeopleFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(PeopleFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(PeopleFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.numberofPeopleSchedule();
  if (schedule){
    idfObject.setString(PeopleFields::NumberofPeopleScheduleName, schedule->name().get());
  }

  idfObject.setString(PeopleFields::NumberofPeopleCalculationMethod, definition.numberofPeopleCalculationMethod());

  double multiplier = modelObject.multiplier();

  OptionalDouble d = definition.numberofPeople();
  if (d){
    idfObject.setDouble(PeopleFields::NumberofPeople, (*d)*multiplier);
  }

  d = definition.peopleperSpaceFloorArea();
  if (d){
    idfObject.setDouble(PeopleFields::PeopleperZoneFloorArea, (*d)*multiplier);
  }

  d = definition.spaceFloorAreaperPerson();
  if (d){
    idfObject.setDouble(PeopleFields::ZoneFloorAreaperPerson, (*d)*multiplier);
  }

  d = definition.fractionRadiant();
  if (d){
    idfObject.setDouble(PeopleFields::FractionRadiant, *d);
  }

  d = definition.sensibleHeatFraction();
  if (d){
    idfObject.setDouble(PeopleFields::SensibleHeatFraction, *d);
  }

  schedule = modelObject.activityLevelSchedule();
  if (schedule){
    idfObject.setString(PeopleFields::ActivityLevelScheduleName, schedule->name().get());
  }

  if (!definition.isCarbonDioxideGenerationRateDefaulted()){
    idfObject.setDouble(PeopleFields::CarbonDioxideGenerationRate, definition.carbonDioxideGenerationRate());
  }

  if (!definition.isEnableASHRAE55ComfortWarningsDefaulted()){
    if (definition.enableASHRAE55ComfortWarnings()){
      idfObject.setString(PeopleFields::EnableASHRAE55ComfortWarnings, "Yes");
    }else{
       idfObject.setString(PeopleFields::EnableASHRAE55ComfortWarnings, "No");
    }
  }

  if (!definition.isMeanRadiantTemperatureCalculationTypeDefaulted()){
    idfObject.setString(PeopleFields::MeanRadiantTemperatureCalculationType, definition.meanRadiantTemperatureCalculationType());
  }

  // TODO: Surface Name/Angle Factor List Name

  schedule = modelObject.workEfficiencySchedule();
  if (schedule){
    idfObject.setString(PeopleFields::WorkEfficiencyScheduleName, schedule->name().get());
  }

  schedule = modelObject.clothingInsulationSchedule();
  if (schedule){
    idfObject.setString(PeopleFields::ClothingInsulationScheduleName, schedule->name().get());
  }

  schedule = modelObject.airVelocitySchedule();
  if (schedule){
    idfObject.setString(PeopleFields::AirVelocityScheduleName, schedule->name().get());
  }

  for (int i = 0, n = definition.numThermalComfortModelTypes(); i < n; ++i) {
    OptionalString s = definition.getThermalComfortModelType(i);
    if (s) {
      idfObject.pushExtensibleGroup(StringVector(1u,*s));
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

