/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Node.hpp"
#include "../../model/SetpointManager.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateNode(Node& modelObject) {
    std::vector<SetpointManager> _setpointManagers = modelObject.setpointManagers();
    for (auto& _setpointManager : _setpointManagers) {
      translateAndMapModelObject(_setpointManager);
    }

    return {};
  }

}  // namespace energyplus

}  // namespace openstudio
