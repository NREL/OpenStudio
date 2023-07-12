/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/TemperingValve.hpp"
#include "../../model/TemperingValve_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/TemperingValve_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateTemperingValve(TemperingValve& modelObject) {
    IdfObject idfObject(IddObjectType::TemperingValve);
    m_idfObjects.push_back(idfObject);

    modelObject.getImpl<model::detail::TemperingValve_Impl>()->setControlNodes();

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(TemperingValveFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(TemperingValveFields::OutletNodeName, node->name().get());
      }
    }

    // Stream2SourceNodeName
    if (auto node = modelObject.stream2SourceNode()) {
      idfObject.setString(TemperingValveFields::Stream2SourceNodeName, node->name().get());
    }

    // TemperatureSetpointNodeName
    if (auto node = modelObject.temperatureSetpointNode()) {
      idfObject.setString(TemperingValveFields::TemperatureSetpointNodeName, node->name().get());
    }

    // PumpOutletNodeName
    if (auto node = modelObject.pumpOutletNode()) {
      idfObject.setString(TemperingValveFields::PumpOutletNodeName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
