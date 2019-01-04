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

boost::optional<IdfObject> ForwardTranslator::translateScheduleYear( ScheduleYear & modelObject )
{
  IdfObject scheduleYear = createRegisterAndNameIdfObject(openstudio::IddObjectType::Schedule_Year,
                                                          modelObject);

  std::vector<ScheduleWeek> scheduleWeeks = modelObject.scheduleWeeks();
  std::vector<openstudio::Date> dates = modelObject.dates();

  unsigned N = scheduleWeeks.size();

  if( N != dates.size() )
  {
    LOG(Error,"Could not translate " << modelObject.briefDescription() << ", because the number of week schedules does not match the number of dates.");

    return boost::none;
  }

  boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
  if (scheduleTypeLimits){
    boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
    if (idfScheduleTypeLimits){
      scheduleYear.setString(Schedule_YearFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
    }
  }

  openstudio::Date startDate(MonthOfYear::Jan, 1);

  scheduleYear.clearExtensibleGroups();

  for (unsigned i = 0; i < N; ++i){
    IdfExtensibleGroup group = scheduleYear.pushExtensibleGroup();

    boost::optional<IdfObject> idfScheduleWeek = translateAndMapModelObject(scheduleWeeks[i]);
    if (idfScheduleWeek){
      group.setString(Schedule_YearExtensibleFields::Schedule_WeekName, idfScheduleWeek->name().get());
    }

    group.setInt(Schedule_YearExtensibleFields::StartMonth, startDate.monthOfYear().value());
    group.setUnsigned(Schedule_YearExtensibleFields::StartDay, startDate.dayOfMonth());
    group.setInt(Schedule_YearExtensibleFields::EndMonth, dates[i].monthOfYear().value());
    group.setUnsigned(Schedule_YearExtensibleFields::EndDay, dates[i].dayOfMonth());

    startDate = dates[i] + openstudio::Time(1,0,0);
  }

  return scheduleYear;
}

} // energyplus

} // openstudio

