/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

OptionalModelObject ReverseTranslator::translateScheduleDayHourly(const WorkspaceObject & workspaceObject){
  if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Day_Hourly){
    LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Day:Hourly");
    return boost::none;
  }

  // create the schedule
  ScheduleDay scheduleDay(m_model);

  OptionalString s = workspaceObject.name();
  if (s){
    scheduleDay.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_Day_HourlyFields::ScheduleTypeLimitsName);
  if (target){
    OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
    if (scheduleTypeLimits){
      scheduleDay.setPointer(OS_Schedule_DayFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
    }
  }

  for(unsigned i = 0; i < 24; ++i){
    OptionalDouble d = workspaceObject.getDouble(Schedule_Day_HourlyFields::Hour1 + i, true);
    if (d){
      scheduleDay.addValue(openstudio::Time(0,i+1,0,0), *d);
    }
  }

  return scheduleDay;
}

} // energyplus

} // openstudio
