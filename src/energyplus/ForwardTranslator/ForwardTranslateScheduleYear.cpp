/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/ScheduleYear.hpp"
#include "../../model/ScheduleYear_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleYear(ScheduleYear& modelObject) {
    IdfObject scheduleYear = createRegisterAndNameIdfObject(openstudio::IddObjectType::Schedule_Year, modelObject);

    std::vector<ScheduleWeek> scheduleWeeks = modelObject.scheduleWeeks();
    std::vector<openstudio::Date> dates = modelObject.dates();

    unsigned N = scheduleWeeks.size();

    if (N != dates.size()) {
      LOG(Error,
          "Could not translate " << modelObject.briefDescription() << ", because the number of week schedules does not match the number of dates.");

      return boost::none;
    }

    boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits) {
      boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
      if (idfScheduleTypeLimits) {
        scheduleYear.setString(Schedule_YearFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
      }
    }

    openstudio::Date startDate(MonthOfYear::Jan, 1);

    scheduleYear.clearExtensibleGroups();

    for (unsigned i = 0; i < N; ++i) {
      IdfExtensibleGroup group = scheduleYear.pushExtensibleGroup();

      boost::optional<IdfObject> idfScheduleWeek = translateAndMapModelObject(scheduleWeeks[i]);
      if (idfScheduleWeek) {
        group.setString(Schedule_YearExtensibleFields::Schedule_WeekName, idfScheduleWeek->name().get());
      }

      group.setInt(Schedule_YearExtensibleFields::StartMonth, startDate.monthOfYear().value());
      group.setUnsigned(Schedule_YearExtensibleFields::StartDay, startDate.dayOfMonth());
      group.setInt(Schedule_YearExtensibleFields::EndMonth, dates[i].monthOfYear().value());
      group.setUnsigned(Schedule_YearExtensibleFields::EndDay, dates[i].dayOfMonth());

      startDate = dates[i] + openstudio::Time(1, 0, 0);
    }

    return scheduleYear;
  }

}  // namespace energyplus

}  // namespace openstudio
