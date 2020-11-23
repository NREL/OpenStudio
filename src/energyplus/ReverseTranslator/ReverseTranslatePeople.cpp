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

#include "../ReverseTranslator.hpp"

#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/People_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translatePeople(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::People) {
      LOG(Error, "WorkspaceObject is not IddObjectType: People");
      return boost::none;
    }

    // create the definition
    openstudio::model::PeopleDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    s = workspaceObject.getString(openstudio::PeopleFields::NumberofPeopleCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("People", *s)) {
      d = workspaceObject.getDouble(openstudio::PeopleFields::NumberofPeople);
      if (d) {
        definition.setNumberofPeople(*d);
      } else {
        LOG(Error, "People value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("People/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::PeopleFields::PeopleperZoneFloorArea);
      if (d) {
        definition.setPeopleperSpaceFloorArea(*d);
      } else {
        LOG(Error, "People/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Area/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::PeopleFields::ZoneFloorAreaperPerson);
      if (d) {
        definition.setSpaceFloorAreaperPerson(*d);
      } else {
        LOG(Error, "Area/Person value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown NumberofPeopleCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::PeopleFields::FractionRadiant);
    if (d) {
      definition.setFractionRadiant(*d);
    }

    d = workspaceObject.getDouble(openstudio::PeopleFields::SensibleHeatFraction);
    if (d) {
      definition.setSensibleHeatFraction(*d);
    }

    d = workspaceObject.getDouble(openstudio::PeopleFields::CarbonDioxideGenerationRate);
    if (d) {
      definition.setCarbonDioxideGenerationRate(*d);
    }

    s = workspaceObject.getString(openstudio::PeopleFields::EnableASHRAE55ComfortWarnings);
    if (s) {
      if (istringEqual("Yes", *s)) {
        definition.setEnableASHRAE55ComfortWarnings(true);
      } else {
        definition.setEnableASHRAE55ComfortWarnings(false);
      }
    }

    s = workspaceObject.getString(openstudio::PeopleFields::MeanRadiantTemperatureCalculationType);
    if (s) {
      definition.setMeanRadiantTemperatureCalculationType(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::PeopleFields::SurfaceName_AngleFactorListName);
    if (target) {
      LOG(Error, "SurfaceName_AngleFactorListName not currently imported");
    }

    for (unsigned i = 0, n = workspaceObject.numExtensibleGroups(); i < n; ++i) {
      IdfExtensibleGroup eg = workspaceObject.getExtensibleGroup(i);
      OS_ASSERT(!eg.empty());
      s = eg.getString(openstudio::PeopleExtensibleFields::ThermalComfortModelType);
      if (s) {
        definition.setThermalComfortModelType(i, *s);
      }
    }

    // create the instance
    People people(definition);

    s = workspaceObject.name();
    if (s) {
      people.setName(*s);
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::ZoneorZoneListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          people.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          people.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::NumberofPeopleScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          bool ok = people.setNumberofPeopleSchedule(schedule);
          if (!ok) {
            LOG(Warn, "Unable to set " << people.briefDescription() << "'s Number of People schedule to " << schedule.briefDescription()
                                       << ", likely because of a ScheduleTypeLimits conflict.");
          }
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::ActivityLevelScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          bool ok = people.setActivityLevelSchedule(schedule);
          if (!ok) {
            LOG(Warn, "Unable to set " << people.briefDescription() << "'s Activity Level schedule to " << schedule.briefDescription()
                                       << ", likely because of a ScheduleTypeLimits conflict.");
          }
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::WorkEfficiencyScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          bool ok = people.setWorkEfficiencySchedule(schedule);
          if (!ok) {
            LOG(Warn, "Unable to set " << people.briefDescription() << "'s Work Efficiency schedule to " << schedule.briefDescription()
                                       << ", likely because of a ScheduleTypeLimits conflict.");
          }
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::ClothingInsulationScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          bool ok = people.setClothingInsulationSchedule(schedule);
          if (!ok) {
            LOG(Warn, "Unable to set " << people.briefDescription() << "'s Clothing Insulation schedule to " << schedule.briefDescription()
                                       << ", likely because of a ScheduleTypeLimits conflict.");
          }
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::AirVelocityScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          bool ok = people.setAirVelocitySchedule(schedule);
          if (!ok) {
            LOG(Error, "Unable to set " << people.briefDescription() << "'s Air Velocity schedule to " << schedule.briefDescription()
                                        << ", likely because of a ScheduleTypeLimits conflict.");
          }
        }
      }
    }

    return people;
  }

}  // namespace energyplus

}  // namespace openstudio
