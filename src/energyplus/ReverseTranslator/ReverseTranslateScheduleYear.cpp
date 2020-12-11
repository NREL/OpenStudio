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

  OptionalModelObject ReverseTranslator::translateScheduleYear(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Year) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Year");
      return boost::none;
    }

    //create the schedule
    ScheduleYear scheduleYear(m_model);
    //translate name
    OptionalString s = workspaceObject.name();
    if (s) {
      scheduleYear.setName(*s);
    }
    //translate schedule type limits name
    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_YearFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleYear.setPointer(OS_Schedule_YearFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }
    //get extensible groups
    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
    //loop over extensible groups
    unsigned n = extensibleGroups.size();
    for (unsigned i = 0; i < n; ++i) {
      //read in extensible groups
      boost::optional<std::string> scheduleName = extensibleGroups[i].getString(Schedule_YearExtensibleFields::Schedule_WeekName);
      boost::optional<unsigned> startMonth = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::StartMonth);
      boost::optional<unsigned> startDay = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::StartDay);
      boost::optional<unsigned> endMonth = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::EndMonth);
      boost::optional<unsigned> endDay = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::EndDay);
      // check for errors, assume sorted groups
      if (i == 0) {
        if ((*startMonth != 1) && (*startDay != 1)) {
          LOG(Error, *scheduleName + " does not start on January 1. Object not translated.");
          return boost::none;
        }
      } else if (i == n - 1) {
        if ((*endMonth != 12) && (*endDay != 31)) {
          LOG(Error, *scheduleName + " does not end on December 31. Object not translated.");
          return boost::none;
        }
      }
      // reverse translate schedule week
      target = extensibleGroups[i].cast<WorkspaceExtensibleGroup>().getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
      if (target) {
        OptionalModelObject scheduleWeekName = translateAndMapWorkspaceObject(*target);
        if (scheduleWeekName) {
          scheduleYear.addScheduleWeek(Date(*endMonth, *endDay), scheduleWeekName->cast<ScheduleWeek>());
        }
      }
    }

    return scheduleYear;
  }

}  // namespace energyplus

}  // namespace openstudio
