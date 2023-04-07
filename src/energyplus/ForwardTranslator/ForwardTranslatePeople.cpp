/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(PeopleFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.numberofPeopleSchedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(PeopleFields::NumberofPeopleScheduleName, idf_schedule_->nameString());
    }

    idfObject.setString(PeopleFields::NumberofPeopleCalculationMethod, definition.numberofPeopleCalculationMethod());

    double multiplier = modelObject.multiplier();

    OptionalDouble d = definition.numberofPeople();
    if (d) {
      idfObject.setDouble(PeopleFields::NumberofPeople, (*d) * multiplier);
    }

    d = definition.peopleperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(PeopleFields::PeopleperFloorArea, (*d) * multiplier);
    }

    d = definition.spaceFloorAreaperPerson();
    if (d) {
      idfObject.setDouble(PeopleFields::FloorAreaperPerson, (*d) * multiplier);
    }

    d = definition.fractionRadiant();
    if (d) {
      idfObject.setDouble(PeopleFields::FractionRadiant, *d);
    }

    d = definition.sensibleHeatFraction();
    if (d) {
      idfObject.setDouble(PeopleFields::SensibleHeatFraction, *d);
    }

    if (boost::optional<Schedule> schedule_ = modelObject.activityLevelSchedule()) {
      if (auto idf_schedule_ = translateAndMapModelObject(schedule_.get())) {
        idfObject.setString(PeopleFields::ActivityLevelScheduleName, idf_schedule_->nameString());
      }
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

    if (boost::optional<Schedule> schedule_ = modelObject.workEfficiencySchedule()) {
      if (auto idf_schedule_ = translateAndMapModelObject(schedule_.get())) {
        idfObject.setString(PeopleFields::WorkEfficiencyScheduleName, idf_schedule_->nameString());
      }
    }

    if (boost::optional<Schedule> schedule_ = modelObject.clothingInsulationSchedule()) {
      if (auto idf_schedule_ = translateAndMapModelObject(schedule_.get())) {
        idfObject.setString(PeopleFields::ClothingInsulationScheduleName, idf_schedule_->nameString());
      }
    }

    if (boost::optional<Schedule> schedule_ = modelObject.airVelocitySchedule()) {
      if (auto idf_schedule_ = translateAndMapModelObject(schedule_.get())) {
        idfObject.setString(PeopleFields::AirVelocityScheduleName, idf_schedule_->nameString());
      }
    }

    // As of 22.2.0, this is no longer possible to make this an extensible field
    // because E+ added 3 regular fields at the end (eg: Ankle Level Velocity Schedule Name)
    for (int i = 0, numComfortModelTypes = 0; i < definition.numThermalComfortModelTypes(); ++i) {
      OptionalString s = definition.getThermalComfortModelType(i);
      if (s) {
        ++numComfortModelTypes;
        if (numComfortModelTypes > 7) {
          LOG(Warn, "For " << definition.briefDescription() << ", only 7 Thermal Confort Model Types are supported by EnergyPlus, number "
                           << numComfortModelTypes << " [=" << *s << "] will be ignored.");
        } else {
          idfObject.setString(PeopleFields::ThermalComfortModel1Type + i, *s);
        }
      }
    }

    if (boost::optional<Schedule> schedule_ = modelObject.ankleLevelAirVelocitySchedule()) {
      if (auto idf_schedule_ = translateAndMapModelObject(schedule_.get())) {
        idfObject.setString(PeopleFields::AnkleLevelAirVelocityScheduleName, idf_schedule_->nameString());
      }
    }

    if (!modelObject.isColdStressTemperatureThresholdDefaulted()) {
      idfObject.setDouble(PeopleFields::ColdStressTemperatureThreshold, modelObject.coldStressTemperatureThreshold());
    }

    if (!modelObject.isHeatStressTemperatureThresholdDefaulted()) {
      idfObject.setDouble(PeopleFields::HeatStressTemperatureThreshold, modelObject.heatStressTemperatureThreshold());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
