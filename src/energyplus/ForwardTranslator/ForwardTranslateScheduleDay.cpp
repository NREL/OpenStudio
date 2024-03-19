/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/time/Time.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Schedule_Day_Interval_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleDay(ScheduleDay& modelObject) {
    IdfObject scheduleDay = createRegisterAndNameIdfObject(openstudio::IddObjectType::Schedule_Day_Interval, modelObject);

    boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits) {
      boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
      if (idfScheduleTypeLimits) {
        scheduleDay.setString(Schedule_Day_IntervalFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
      }
    }

    scheduleDay.setString(Schedule_Day_IntervalFields::InterpolatetoTimestep, modelObject.interpolatetoTimestep());

    std::vector<double> values = modelObject.values();
    std::vector<openstudio::Time> times = modelObject.times();

    unsigned N = values.size();
    OS_ASSERT(N == times.size());

    scheduleDay.clearExtensibleGroups();

    for (unsigned i = 0; i < N; ++i) {
      IdfExtensibleGroup group = scheduleDay.pushExtensibleGroup();

      std::string hourPrefix;
      std::string minutePrefix;

      int hours = times[i].hours() + 24 * times[i].days();
      if (hours < 10) {
        hourPrefix = "0";
      }

      int minutes = times[i].minutes() + (int)floor((times[i].seconds() / 60.0) + 0.5);
      if (minutes < 10) {
        minutePrefix = "0";
      }

      std::stringstream ss;
      ss << hourPrefix << hours << ":" << minutePrefix << minutes;

      group.setString(Schedule_Day_IntervalExtensibleFields::Time, ss.str());
      group.setDouble(Schedule_Day_IntervalExtensibleFields::ValueUntilTime, values[i]);
    }

    return scheduleDay;
  }

}  // namespace energyplus

}  // namespace openstudio
