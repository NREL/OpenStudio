/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/RunPeriodControlSpecialDays.hpp"
#include <utilities/idd/RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/core/StringHelpers.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRunPeriodControlSpecialDays(RunPeriodControlSpecialDays& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::RunPeriodControl_SpecialDays);

    m_idfObjects.push_back(idfObject);

    boost::optional<std::string> s;

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    s = modelObject.getString(OS_RunPeriodControl_SpecialDaysFields::StartDate);
    if (s) {
      const auto temp = openstudio::replace(s.get(), "5th", "Last");
      idfObject.setString(RunPeriodControl_SpecialDaysFields::StartDate, temp);
    }

    s = modelObject.getString(OS_RunPeriodControl_SpecialDaysFields::Duration);
    if (s) {
      idfObject.setString(RunPeriodControl_SpecialDaysFields::Duration, s.get());
    }

    s = modelObject.getString(OS_RunPeriodControl_SpecialDaysFields::SpecialDayType);
    if (s) {
      idfObject.setString(RunPeriodControl_SpecialDaysFields::SpecialDayType, s.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
