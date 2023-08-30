/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/time/Time.hpp"

#include <utilities/idd/Schedule_Day_Hourly_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Day_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleDayHourly(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Day_Hourly) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Day:Hourly");
      return boost::none;
    }

    // create the schedule
    ScheduleDay scheduleDay(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      scheduleDay.setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_Day_HourlyFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleDay.setPointer(OS_Schedule_DayFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }

    for (unsigned i = 0; i < 24; ++i) {
      OptionalDouble d = workspaceObject.getDouble(Schedule_Day_HourlyFields::Hour1 + i, true);
      if (d) {
        scheduleDay.addValue(openstudio::Time(0, i + 1, 0, 0), *d);
      }
    }

    return scheduleDay;
  }

}  // namespace energyplus

}  // namespace openstudio
