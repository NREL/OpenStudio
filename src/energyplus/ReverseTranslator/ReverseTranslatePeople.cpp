/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      d = workspaceObject.getDouble(openstudio::PeopleFields::PeopleperFloorArea);
      if (d) {
        definition.setPeopleperSpaceFloorArea(*d);
      } else {
        LOG(Error, "People/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Area/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::PeopleFields::FloorAreaperPerson);
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

    // As of 22.2.0, this is no longer possible to make this an extensible field
    // because E+ added 3 regular fields at the end (eg: Ankle Level Velocity Schedule Name)
    for (unsigned i = PeopleFields::ThermalComfortModel1Type, k = 0; i <= PeopleFields::ThermalComfortModel7Type; ++i) {
      s = workspaceObject.getString(i, false, true);
      if (s) {
        definition.setThermalComfortModelType(k++, *s);
      }
    }

    // create the instance
    People people(definition);

    s = workspaceObject.name();
    if (s) {
      people.setName(*s);
    }

    target = workspaceObject.getTarget(openstudio::PeopleFields::ZoneorZoneListorSpaceorSpaceListName);
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

    target = workspaceObject.getTarget(openstudio::PeopleFields::AnkleLevelAirVelocityScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          bool ok = people.setAnkleLevelAirVelocitySchedule(schedule);
          if (!ok) {
            LOG(Error, "Unable to set " << people.briefDescription() << "'s Ankle Level Air Velocity schedule to " << schedule.briefDescription()
                                        << ", likely because of a ScheduleTypeLimits conflict.");
          }
        }
      }
    }

    d = workspaceObject.getDouble(openstudio::PeopleFields::ColdStressTemperatureThreshold);
    if (d) {
      people.setColdStressTemperatureThreshold(*d);
    }

    d = workspaceObject.getDouble(openstudio::PeopleFields::HeatStressTemperatureThreshold);
    if (d) {
      people.setHeatStressTemperatureThreshold(*d);
    }

    return people;
  }

}  // namespace energyplus

}  // namespace openstudio
