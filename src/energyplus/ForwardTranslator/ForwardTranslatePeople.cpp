/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  boost::optional<IdfObject> ForwardTranslator::translatePeople(People& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::People);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    PeopleDefinition definition = modelObject.peopleDefinition();

    idfObject.setString(PeopleFields::Name, modelObject.name().get());

    boost::optional<Space> space = modelObject.space();
    boost::optional<SpaceType> spaceType = modelObject.spaceType();
    if (space) {
      boost::optional<ThermalZone> thermalZone = space->thermalZone();
      if (thermalZone) {
        idfObject.setString(PeopleFields::ZoneorZoneListName, thermalZone->name().get());
      }
    } else if (spaceType) {
      idfObject.setString(PeopleFields::ZoneorZoneListName, spaceType->name().get());
    }

    boost::optional<Schedule> schedule = modelObject.numberofPeopleSchedule();
    if (schedule) {
      idfObject.setString(PeopleFields::NumberofPeopleScheduleName, schedule->name().get());
    }

    idfObject.setString(PeopleFields::NumberofPeopleCalculationMethod, definition.numberofPeopleCalculationMethod());

    double multiplier = modelObject.multiplier();

    OptionalDouble d = definition.numberofPeople();
    if (d) {
      idfObject.setDouble(PeopleFields::NumberofPeople, (*d) * multiplier);
    }

    d = definition.peopleperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(PeopleFields::PeopleperZoneFloorArea, (*d) * multiplier);
    }

    d = definition.spaceFloorAreaperPerson();
    if (d) {
      idfObject.setDouble(PeopleFields::ZoneFloorAreaperPerson, (*d) * multiplier);
    }

    d = definition.fractionRadiant();
    if (d) {
      idfObject.setDouble(PeopleFields::FractionRadiant, *d);
    }

    d = definition.sensibleHeatFraction();
    if (d) {
      idfObject.setDouble(PeopleFields::SensibleHeatFraction, *d);
    }

    schedule = modelObject.activityLevelSchedule();
    if (schedule) {
      idfObject.setString(PeopleFields::ActivityLevelScheduleName, schedule->name().get());
    }

    if (!definition.isCarbonDioxideGenerationRateDefaulted()) {
      idfObject.setDouble(PeopleFields::CarbonDioxideGenerationRate, definition.carbonDioxideGenerationRate());
    }

    if (!definition.isEnableASHRAE55ComfortWarningsDefaulted()) {
      if (definition.enableASHRAE55ComfortWarnings()) {
        idfObject.setString(PeopleFields::EnableASHRAE55ComfortWarnings, "Yes");
      } else {
        idfObject.setString(PeopleFields::EnableASHRAE55ComfortWarnings, "No");
      }
    }

    if (!definition.isMeanRadiantTemperatureCalculationTypeDefaulted()) {
      idfObject.setString(PeopleFields::MeanRadiantTemperatureCalculationType, definition.meanRadiantTemperatureCalculationType());
    }

    // TODO: Surface Name/Angle Factor List Name

    schedule = modelObject.workEfficiencySchedule();
    if (schedule) {
      idfObject.setString(PeopleFields::WorkEfficiencyScheduleName, schedule->name().get());
    }

    schedule = modelObject.clothingInsulationSchedule();
    if (schedule) {
      idfObject.setString(PeopleFields::ClothingInsulationScheduleName, schedule->name().get());
    }

    schedule = modelObject.airVelocitySchedule();
    if (schedule) {
      idfObject.setString(PeopleFields::AirVelocityScheduleName, schedule->name().get());
    }

    for (int i = 0, n = definition.numThermalComfortModelTypes(); i < n; ++i) {
      OptionalString s = definition.getThermalComfortModelType(i);
      if (s) {
        idfObject.pushExtensibleGroup(StringVector(1u, *s));
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
