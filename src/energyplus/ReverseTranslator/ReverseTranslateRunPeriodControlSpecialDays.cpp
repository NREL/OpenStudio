/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/RunPeriodControlSpecialDays.hpp"
#include "../../model/RunPeriodControlSpecialDays_Impl.hpp"
#include <utilities/idd/RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateRunPeriodControlSpecialDays(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::RunPeriodControl_SpecialDays) {
      LOG(Error, "WorkspaceObject is not IddObjectType: RunPeriodControl_SpecialDays");
      return boost::none;
    }

    boost::optional<std::string> s = workspaceObject.getString(RunPeriodControl_SpecialDaysFields::StartDate);
    if (!s) {
      LOG(Error, "No start date specified for RunPeriodControl:SpecialDays");
      return boost::none;
    }

    boost::optional<RunPeriodControlSpecialDays> mo;
    try {
      mo = RunPeriodControlSpecialDays(*s, m_model);
    } catch (...) {
      LOG(Error, "'" << *s << "'  is not a correct date specification");
      return boost::none;
    }

    s = workspaceObject.getString(RunPeriodControl_SpecialDaysFields::Name);
    if (s) {
      mo->setName(s.get());
    }

    boost::optional<int> i = workspaceObject.getInt(RunPeriodControl_SpecialDaysFields::Duration);
    if (i) {
      mo->setDuration(i.get());
    }

    s = workspaceObject.getString(RunPeriodControl_SpecialDaysFields::SpecialDayType);
    if (s) {
      mo->setString(OS_RunPeriodControl_SpecialDaysFields::SpecialDayType, s.get());
    }

    return *mo;
  }

}  // namespace energyplus

}  // namespace openstudio
