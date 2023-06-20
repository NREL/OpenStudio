/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/RunPeriodControlDaylightSavingTime.hpp"
#include "../../model/RunPeriodControlDaylightSavingTime_Impl.hpp"
#include <utilities/idd/OS_RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <utilities/idd/RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateRunPeriodControlDaylightSavingTime(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::RunPeriodControl_DaylightSavingTime) {
      LOG(Error, "WorkspaceObject is not IddObjectType: RunPeriodControl_DaylightSavingTime");
      return boost::none;
    }

    auto dst = m_model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();

    OptionalString s = workspaceObject.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate);
    if (s) {
      dst.setStartDate(*s);
    }

    s = workspaceObject.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate);
    if (s) {
      dst.setEndDate(*s);
    }

    return dst;
  }

}  // namespace energyplus

}  // namespace openstudio
