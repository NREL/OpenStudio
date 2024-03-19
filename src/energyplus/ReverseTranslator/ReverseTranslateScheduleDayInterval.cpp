/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/time/Time.hpp"

#include <utilities/idd/Schedule_Day_Interval_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Day_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/core/Exception.hpp"

#include <boost/regex.hpp>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleDayInterval(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Day_Interval) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Day:Interval");
      return boost::none;
    }

    // create the schedule
    ScheduleDay scheduleDay(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      scheduleDay.setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_Day_IntervalFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleDay.setPointer(OS_Schedule_DayFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }

    s = workspaceObject.getString(Schedule_Day_IntervalFields::InterpolatetoTimestep);
    if (s) {
      scheduleDay.setInterpolatetoTimestep(*s);
    }

    //get extensible groups
    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
    //loop over extensible groups
    boost::regex timeRegex("(\\d?\\d:\\d\\d)");
    boost::smatch m;
    unsigned n = extensibleGroups.size();
    for (unsigned i = 0; i < n; ++i) {
      //read in extensible groups
      boost::optional<std::string> timeString = extensibleGroups[i].getString(Schedule_Day_IntervalExtensibleFields::Time);
      boost::optional<double> valueUntilTime = extensibleGroups[i].getDouble(Schedule_Day_IntervalExtensibleFields::ValueUntilTime);
      if (timeString && valueUntilTime) {
        // Time string may be prefixed with "Until: ". Extract time in HH:MM format.
        if (boost::regex_search(*timeString, m, timeRegex)) {
          timeString = std::string(m[1].first, m[1].second);
        }
        try {
          openstudio::Time time(*timeString);
          scheduleDay.addValue(time, *valueUntilTime);
        } catch (std::exception& e) {
          LOG(Warn, "Could not add value (" << *timeString << ", " << *valueUntilTime << ") to ScheduleDay being created from "
                                            << workspaceObject.briefDescription() << ", because " << e.what() << ".");
        }
      } else {
        LOG(Warn, "Encountered extensible group with incomplete or improperly formatted data in "
                    << workspaceObject.briefDescription() << ". Therefore, a corresponding value is not "
                    << "being added to the ScheduleDay object under construction.");
      }
    }

    return scheduleDay;
  }

}  // namespace energyplus

}  // namespace openstudio
