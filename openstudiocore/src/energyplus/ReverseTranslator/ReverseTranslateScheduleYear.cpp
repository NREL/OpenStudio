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

#include "../ReverseTranslator.hpp"

#include "../../model/ScheduleYear.hpp"
#include "../../model/ScheduleYear_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Year_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/time/Date.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateScheduleYear(const WorkspaceObject & workspaceObject){
  if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Year){
    LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Year");
    return boost::none;
  }

  //create the schedule
  ScheduleYear scheduleYear(m_model);
  //translate name
  OptionalString s = workspaceObject.name();
  if (s){
    scheduleYear.setName(*s);
  }
  //translate schedule type limits name
  OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_YearFields::ScheduleTypeLimitsName);
  if (target){
    OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
    if (scheduleTypeLimits){
      scheduleYear.setPointer(OS_Schedule_YearFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
    }
  }
  //get extensible groups
  std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
  //loop over extensible groups
  unsigned n = extensibleGroups.size();
  for (unsigned i = 0; i < n; ++i){
    //read in extensible groups
    boost::optional<std::string> scheduleName = extensibleGroups[i].getString(Schedule_YearExtensibleFields::Schedule_WeekName);
    boost::optional<unsigned> startMonth = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::StartMonth);
    boost::optional<unsigned> startDay = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::StartDay);
    boost::optional<unsigned> endMonth = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::EndMonth);
    boost::optional<unsigned> endDay = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::EndDay);
    // check for errors, assume sorted groups
    if (i == 0){
      if ((*startMonth != 1) && (*startDay != 1)){
        LOG(Error, *scheduleName + " does not start on January 1. Object not translated.");
        return boost::none;
      }
    }else if (i == n - 1){
      if ((*endMonth != 12) && (*endDay != 31)){
        LOG(Error, *scheduleName + " does not end on December 31. Object not translated.");
        return boost::none;
      }
    }
    // reverse translate schedule week
    target = extensibleGroups[i].cast<WorkspaceExtensibleGroup>().getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
    if (target){
      OptionalModelObject scheduleWeekName = translateAndMapWorkspaceObject(*target);
      if (scheduleWeekName){
        scheduleYear.addScheduleWeek(Date(*endMonth,*endDay),scheduleWeekName->cast<ScheduleWeek>());
      }
    }
  }

  return scheduleYear;
}

} // energyplus

} // openstudio
