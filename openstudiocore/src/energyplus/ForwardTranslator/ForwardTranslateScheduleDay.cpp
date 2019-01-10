/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

boost::optional<IdfObject> ForwardTranslator::translateScheduleDay( ScheduleDay & modelObject )
{
  IdfObject scheduleDay = createRegisterAndNameIdfObject(openstudio::IddObjectType::Schedule_Day_Interval,
                                                         modelObject);

  boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
  if (scheduleTypeLimits){
    boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
    if (idfScheduleTypeLimits){
      scheduleDay.setString(Schedule_Day_IntervalFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
    }
  }

  if (modelObject.interpolatetoTimestep()){
    scheduleDay.setString(Schedule_Day_IntervalFields::InterpolatetoTimestep, "Average");
  }else{
    scheduleDay.setString(Schedule_Day_IntervalFields::InterpolatetoTimestep, "No");
  }

  std::vector<double> values = modelObject.values();
  std::vector<openstudio::Time> times = modelObject.times();

  unsigned N = values.size();
  OS_ASSERT(N == times.size());

  scheduleDay.clearExtensibleGroups();

  for (unsigned i = 0; i < N; ++i){
    IdfExtensibleGroup group = scheduleDay.pushExtensibleGroup();

    std::string hourPrefix;
    std::string minutePrefix;

    int hours = times[i].hours() + 24*times[i].days();
    if (hours < 10){
      hourPrefix = "0";
    }

    int minutes = times[i].minutes() + floor((times[i].seconds()/60.0) + 0.5);
    if (minutes < 10){
      minutePrefix = "0";
    }

    std::stringstream ss;
    ss << hourPrefix << hours << ":" << minutePrefix << minutes;

    group.setString(Schedule_Day_IntervalExtensibleFields::Time, ss.str());
    group.setDouble(Schedule_Day_IntervalExtensibleFields::ValueUntilTime, values[i]);
  }

  return scheduleDay;
}

} // energyplus

} // openstudio

