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

    s = workspaceObject.getString(OS_Schedule_DayFields::InterpolatetoTimestep, true);
    if (s) {
      if (openstudio::istringEqual(*s, "Yes")) {
        scheduleDay.setInterpolatetoTimestep(true);
      } else if (openstudio::istringEqual(*s, "Linear")) {
        scheduleDay.setInterpolatetoTimestep(true);
      } else if (openstudio::istringEqual(*s, "Average")) {
        scheduleDay.setInterpolatetoTimestep(true);
      }
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
