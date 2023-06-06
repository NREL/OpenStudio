/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ControllerWaterCoil.hpp"
#include "../../model/ControllerWaterCoil_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Controller_WaterCoil_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateControllerWaterCoil(ControllerWaterCoil& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Controller_WaterCoil);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // ControlVariable

    if ((s = modelObject.controlVariable())) {
      idfObject.setString(Controller_WaterCoilFields::ControlVariable, s.get());
    }

    // Action

    if ((s = modelObject.action())) {
      idfObject.setString(Controller_WaterCoilFields::Action, s.get());
    }

    // ActuatorVariable

    if ((s = modelObject.actuatorVariable())) {
      idfObject.setString(Controller_WaterCoilFields::ActuatorVariable, s.get());
    }

    boost::optional<model::WaterToAirComponent> waterCoil;
    if (auto hvacComp = modelObject.getImpl<model::detail::ControllerWaterCoil_Impl>()->waterCoil()) {
      waterCoil = hvacComp->optionalCast<model::WaterToAirComponent>();
    }

    // SensorNodeName

    if (boost::optional<Node> node = modelObject.sensorNode()) {
      idfObject.setString(Controller_WaterCoilFields::SensorNodeName, node->name().get());
    } else if (waterCoil) {
      if (auto node = waterCoil->airOutletModelObject()) {
        idfObject.setString(Controller_WaterCoilFields::SensorNodeName, node->name().get());
      }
    }

    // ActuatorNodeName

    if (boost::optional<Node> node = modelObject.actuatorNode()) {
      idfObject.setString(Controller_WaterCoilFields::ActuatorNodeName, node->name().get());
    } else if (waterCoil) {
      if (auto node = waterCoil->waterInletModelObject()) {
        idfObject.setString(Controller_WaterCoilFields::ActuatorNodeName, node->name().get());
      }
    }

    // ControllerConvergenceTolerance

    if (modelObject.isControllerConvergenceToleranceAutosized()) {
      idfObject.setString(Controller_WaterCoilFields::ControllerConvergenceTolerance, "Autosize");
    } else if ((value = modelObject.controllerConvergenceTolerance())) {
      idfObject.setDouble(Controller_WaterCoilFields::ControllerConvergenceTolerance, value.get());
    }

    // MaximumActuatedFlow

    if (modelObject.isMaximumActuatedFlowAutosized()) {
      idfObject.setString(Controller_WaterCoilFields::MaximumActuatedFlow, "Autosize");
    } else if ((value = modelObject.maximumActuatedFlow())) {
      idfObject.setDouble(Controller_WaterCoilFields::MaximumActuatedFlow, value.get());
    }

    // MinimumActuatedFlow

    if ((value = modelObject.minimumActuatedFlow())) {
      idfObject.setDouble(Controller_WaterCoilFields::MinimumActuatedFlow, value.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
