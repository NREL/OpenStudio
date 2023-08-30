/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerMixedAir(SetpointManagerMixedAir& modelObject) {
    boost::optional<Node> node;
    std::string s;

    IdfObject idfObject(IddObjectType::SetpointManager_MixedAir);

    // Name
    s = modelObject.name().get();
    idfObject.setString(SetpointManager_MixedAirFields::Name, s);

    // ControlVariable
    idfObject.setString(SetpointManager_MixedAirFields::ControlVariable, "Temperature");

    // ReferenceSetpointNodeName
    node = modelObject.referenceSetpointNode();
    if (node) {
      idfObject.setString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName, node->name().get());
    }

    // FanInletNodeName
    node = modelObject.fanInletNode();
    if (node) {
      idfObject.setString(SetpointManager_MixedAirFields::FanInletNodeName, node->name().get());
    }

    // FanOutletNodeName
    node = modelObject.fanOutletNode();
    if (node) {
      idfObject.setString(SetpointManager_MixedAirFields::FanOutletNodeName, node->name().get());
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName, node->name().get());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
