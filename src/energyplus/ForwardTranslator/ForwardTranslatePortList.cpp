/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/NodeList_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePortList(PortList& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    // Create a new IddObjectType::NodeList
    // If you don't want a node list based on the port list, don't use this translator

    std::vector<ModelObject> modelObjects = modelObject.modelObjects();

    if (modelObjects.empty()) {
      // do not write out this object
      return boost::none;
    }

    IdfObject idfObject(IddObjectType::NodeList);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    for (const auto& modelObject : modelObjects) {
      if (boost::optional<Node> node = modelObject.optionalCast<Node>()) {
        IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

        group.setString(NodeListExtensibleFields::NodeName, node->name().get());
      }
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
