/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Constant_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleConstant(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Constant) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Constant");
      return boost::none;
    }

    ScheduleConstant scheduleConstant(m_model);

    if (OptionalString os = workspaceObject.name()) {
      scheduleConstant.setName(*os);
    }

    boost::optional<WorkspaceObject> target = workspaceObject.getTarget(Schedule_ConstantFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleConstant.setPointer(OS_Schedule_ConstantFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }
    if (OptionalDouble val = workspaceObject.getDouble(Schedule_ConstantFields::HourlyValue)) {
      scheduleConstant.setValue(*val);
    }

    return scheduleConstant;
  }

}  // namespace energyplus

}  // namespace openstudio
