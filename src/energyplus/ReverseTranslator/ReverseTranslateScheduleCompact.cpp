/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleCompact(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Compact) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Compact");
      return boost::none;
    }

    ScheduleCompact scheduleCompact(m_model);

    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_CompactFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleCompact.setPointer(OS_Schedule_CompactFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }

    if (OptionalString os = workspaceObject.name()) {
      scheduleCompact.setName(*os);
    }

    for (IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      for (unsigned i = 0; i < eg.numFields(); ++i) {
        if (auto value = eg.getString(i)) {
          if (istringEqual(value.get(), "Interpolate:Average")) {
            eg.setString(i, "Interpolate:Yes");
          }
        }
      }
      scheduleCompact.pushExtensibleGroup(eg.fields());
    }

    return scheduleCompact;
  }

}  // namespace energyplus

}  // namespace openstudio
