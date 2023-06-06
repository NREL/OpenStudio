/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerScheduled.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/SetpointManager_Scheduled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerScheduled(SetpointManagerScheduled& modelObject) {
    boost::optional<Node> node;
    std::string s;

    IdfObject idfObject(IddObjectType::SetpointManager_Scheduled);

    // Name
    s = modelObject.name().get();
    idfObject.setString(SetpointManager_ScheduledFields::Name, s);

    // ControlVariable
    s = modelObject.controlVariable();
    idfObject.setString(SetpointManager_ScheduledFields::ControlVariable, s);

    // Schedule
    if (modelObject.hasSchedule()) {
      Schedule schedule = modelObject.schedule();
      boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule);
      if (_schedule && _schedule->name()) {
        idfObject.setString(SetpointManager_ScheduledFields::ScheduleName, _schedule->name().get());
      }
    } else {
      LOG(Error, "SetpointManager:Scheduled '" << modelObject.name().get() << "' is missing required schedule");
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName, node->name().get());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
