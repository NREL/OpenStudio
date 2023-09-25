/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/SetpointManagerScheduled.hpp"
#include "../../model/SetpointManagerScheduled_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/SetpointManager_Scheduled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSetpointManagerScheduled(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SetpointManager_Scheduled) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SetpointManager_Scheduled");
      return boost::none;
    }

    boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(SetpointManager_ScheduledFields::ScheduleName);
    boost::optional<Schedule> schedule;

    if (wo) {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
      if (mo) {
        if (!(schedule = mo->optionalCast<Schedule>())) {
          LOG(Error, workspaceObject.briefDescription() << " does not have an associated schedule");

          return boost::none;
        }
      }
    }

    bool nodeFound = false;

    if (boost::optional<std::string> setpointNodeName = workspaceObject.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName)) {
      boost::optional<Node> setpointNode = m_model.getConcreteModelObjectByName<Node>(setpointNodeName.get());

      if (setpointNode) {
        nodeFound = true;
      }
    }

    if (!nodeFound) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a node in the model");

      return boost::none;
    }

    if (schedule) {
      SetpointManagerScheduled mo(m_model, schedule.get());

      // Name
      boost::optional<std::string> s = workspaceObject.getString(SetpointManager_ScheduledFields::Name);
      if (s) {
        mo.setName(s.get());
      }

      // Setpoint Node
      s = workspaceObject.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName);
      if (s) {
        boost::optional<Node> node = m_model.getConcreteModelObjectByName<Node>(s.get());

        if (node) {
          mo.addToNode(node.get());
        }
      }

      // Control Variable
      s = workspaceObject.getString(SetpointManager_ScheduledFields::ControlVariable);
      if (s) {
        mo.setControlVariable(s.get());
      }

      if (mo.setpointNode()) {
        return mo;
      } else {
        return boost::none;
      }
    } else {
      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
