/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
    }

    SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
    }

    SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(const SetpointManagerSingleZoneReheat_Impl& other, Model_Impl* model,
                                                                               bool keepHandles)
      : SetpointManager_Impl(other, model, keepHandles) {}

    const std::vector<std::string>& SetpointManagerSingleZoneReheat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerSingleZoneReheat_Impl::iddObjectType() const {
      return SetpointManagerSingleZoneReheat::iddObjectType();
    }

    bool SetpointManagerSingleZoneReheat_Impl::addToNode(Node& node) {
      bool added = SetpointManager_Impl::addToNode(node);
      if (added) {
        if (boost::optional<AirLoopHVAC> _airLoop = node.airLoopHVAC()) {
          ModelObjectVector modelObjectVector = _airLoop->demandComponents(openstudio::IddObjectType::OS_ThermalZone);
          if (!modelObjectVector.empty()) {
            ModelObject mo = modelObjectVector.front();
            auto thermalZone = mo.cast<ThermalZone>();
            this->setControlZone(thermalZone);
          }
          return true;
        }
      }
      return false;
    }

    ModelObject SetpointManagerSingleZoneReheat_Impl::clone(Model model) const {
      auto clonedObject = SetpointManager_Impl::clone(model).cast<SetpointManagerSingleZoneReheat>();
      clonedObject.resetControlZone();
      return std::move(clonedObject);
    }

    std::string SetpointManagerSingleZoneReheat_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_ReheatFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSingleZoneReheat_Impl::minimumSupplyAirTemperature() {
      return getDouble(OS_SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature).get();
    }

    bool SetpointManagerSingleZoneReheat_Impl::setMinimumSupplyAirTemperature(double value) {
      return setDouble(OS_SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature, value);
      ;
    }

    double SetpointManagerSingleZoneReheat_Impl::maximumSupplyAirTemperature() {
      return getDouble(OS_SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature).get();
    }

    bool SetpointManagerSingleZoneReheat_Impl::setMaximumSupplyAirTemperature(double value) {
      return setDouble(OS_SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature, value);
      ;
    }

    boost::optional<Node> SetpointManagerSingleZoneReheat_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSingleZoneReheat_Impl::setSetpointNode(const Node& node) {
      return setPointer(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName, node.handle());
    }

    void SetpointManagerSingleZoneReheat_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    boost::optional<ThermalZone> SetpointManagerSingleZoneReheat_Impl::controlZone() {
      auto thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

      return thisModelObject.getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName);
    }

    bool SetpointManagerSingleZoneReheat_Impl::setControlZone(ThermalZone& thermalZone) {
      auto thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

      return thisModelObject.setPointer(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName, thermalZone.handle());
    }

    bool SetpointManagerSingleZoneReheat_Impl::setControlVariable(const std::string& controlVariable) {
      return setString(OS_SetpointManager_SingleZone_ReheatFields::ControlVariable, controlVariable);
    }

    boost::optional<ModelObject> SetpointManagerSingleZoneReheat_Impl::controlZoneAsModelObject() {
      OptionalModelObject result;
      OptionalThermalZone intermediate = controlZone();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SetpointManagerSingleZoneReheat_Impl::setControlZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
        if (intermediate) {
          setControlZone(*intermediate);
          return true;
        }
      }
      return false;
    }

    void SetpointManagerSingleZoneReheat_Impl::resetControlZone() {
      setString(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName, "");
    }

  }  // namespace detail

  SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(const Model& model)
    : SetpointManager(SetpointManagerSingleZoneReheat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneReheat_Impl>());

    setMinimumSupplyAirTemperature(-99.0);

    setMaximumSupplyAirTemperature(99.0);
  }

  SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(std::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> p)
    : SetpointManager(std::move(p)) {}

  std::vector<std::string> SetpointManagerSingleZoneReheat::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_SingleZone_ReheatFields::ControlVariable);
  }

  IddObjectType SetpointManagerSingleZoneReheat::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SetpointManager_SingleZone_Reheat);
    return result;
  }

  double SetpointManagerSingleZoneReheat::minimumSupplyAirTemperature() {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->minimumSupplyAirTemperature();
  }

  bool SetpointManagerSingleZoneReheat::setMinimumSupplyAirTemperature(double value) {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setMinimumSupplyAirTemperature(value);
  }

  double SetpointManagerSingleZoneReheat::maximumSupplyAirTemperature() {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->maximumSupplyAirTemperature();
  }

  bool SetpointManagerSingleZoneReheat::setMaximumSupplyAirTemperature(double value) {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setMaximumSupplyAirTemperature(value);
  }

  boost::optional<Node> SetpointManagerSingleZoneReheat::setpointNode() const {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setpointNode();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneReheat::controlZone() {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->controlZone();
  }

  bool SetpointManagerSingleZoneReheat::setControlZone(ThermalZone& thermalZone) {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setControlZone(thermalZone);
  }

  void SetpointManagerSingleZoneReheat::resetControlZone() {
    getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->resetControlZone();
  }

  std::string SetpointManagerSingleZoneReheat::controlVariable() const {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->controlVariable();
  }

  bool SetpointManagerSingleZoneReheat::setControlVariable(const std::string& controlVariable) {
    return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setControlVariable(controlVariable);
  }

}  // namespace model
}  // namespace openstudio
