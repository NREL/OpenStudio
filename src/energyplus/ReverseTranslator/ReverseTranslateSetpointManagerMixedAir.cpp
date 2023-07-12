/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/SetpointManagerMixedAir_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSetpointManagerMixedAir(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SetpointManager_MixedAir) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SetpointManager_MixedAir");
      return boost::none;
    }

    bool nodeFound = false;

    if (boost::optional<std::string> setpointNodeName = workspaceObject.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName)) {
      boost::optional<Node> setpointNode = m_model.getConcreteModelObjectByName<Node>(setpointNodeName.get());

      if (setpointNode) {
        nodeFound = true;
      }
    }

    if (!nodeFound) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a node in the model");

      return boost::none;
    }

    SetpointManagerMixedAir mo(m_model);

    boost::optional<std::string> s = workspaceObject.getString(SetpointManager_MixedAirFields::Name);
    if (s) {
      mo.setName(s.get());
    }

    s = workspaceObject.getString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName);
    if (s) {
      boost::optional<Node> node = m_model.getConcreteModelObjectByName<Node>(s.get());

      if (node) {
        mo.setReferenceSetpointNode(node.get());
      }
    }

    s = workspaceObject.getString(SetpointManager_MixedAirFields::FanInletNodeName);
    if (s) {
      boost::optional<Node> node = m_model.getConcreteModelObjectByName<Node>(s.get());

      if (node) {
        mo.setFanInletNode(node.get());
      }
    }

    s = workspaceObject.getString(SetpointManager_MixedAirFields::FanOutletNodeName);
    if (s) {
      boost::optional<Node> node = m_model.getConcreteModelObjectByName<Node>(s.get());

      if (node) {
        mo.setFanOutletNode(node.get());
      }
    }

    s = workspaceObject.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName);
    if (s) {
      boost::optional<Node> node = m_model.getConcreteModelObjectByName<Node>(s.get());

      if (node) {
        mo.addToNode(node.get());
      }
    }

    if (mo.setpointNode()) {
      return mo;
    } else {
      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
