/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerOutdoorAirReset.hpp"
#include "SetpointManagerOutdoorAirReset_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_OutdoorAirReset_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerOutdoorAirReset_Impl::SetpointManagerOutdoorAirReset_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerOutdoorAirReset::iddObjectType());
    }

    SetpointManagerOutdoorAirReset_Impl::SetpointManagerOutdoorAirReset_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerOutdoorAirReset::iddObjectType());
    }

    SetpointManagerOutdoorAirReset_Impl::SetpointManagerOutdoorAirReset_Impl(const SetpointManagerOutdoorAirReset_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerOutdoorAirReset_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerOutdoorAirReset_Impl::iddObjectType() const {
      return SetpointManagerOutdoorAirReset::iddObjectType();
    }

    /** This SPM is allowed on a PlantLoop */
    bool SetpointManagerOutdoorAirReset_Impl::isAllowedOnPlantLoop() const {
      return true;
    }

    std::vector<ScheduleTypeKey> SetpointManagerOutdoorAirReset_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_SetpointManager_OutdoorAirResetFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("SetpointManagerOutdoorAirReset", "Setpoint Manager Outdoor Air Reset"));
      }
      return result;
    }

    std::string SetpointManagerOutdoorAirReset_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_OutdoorAirResetFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerOutdoorAirReset_Impl::isControlVariableDefaulted() const {
      return isEmpty(OS_SetpointManager_OutdoorAirResetFields::ControlVariable);
    }

    double SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerOutdoorAirReset_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName);
    }

    boost::optional<Schedule> SetpointManagerOutdoorAirReset_Impl::schedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_OutdoorAirResetFields::ScheduleName);
    }

    boost::optional<double> SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature2() const {
      return getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, true);
    }

    boost::optional<double> SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature2() const {
      return getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, true);
    }

    boost::optional<double> SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature2() const {
      return getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, true);
    }

    boost::optional<double> SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature2() const {
      return getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, true);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setControlVariable(const std::string& controlVariable) {
      return setString(OS_SetpointManager_OutdoorAirResetFields::ControlVariable, controlVariable);
    }

    void SetpointManagerOutdoorAirReset_Impl::resetControlVariable() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::ControlVariable, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature, setpointatOutdoorLowTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerOutdoorAirReset_Impl::setOutdoorLowTemperature(double outdoorLowTemperature) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature, outdoorLowTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature, setpointatOutdoorHighTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerOutdoorAirReset_Impl::setOutdoorHighTemperature(double outdoorHighTemperature) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature, outdoorHighTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSetpointNode(const Node& node) {
      return setPointer(OS_SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName, node.handle());
    }

    void SetpointManagerOutdoorAirReset_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_SetpointManager_OutdoorAirResetFields::ScheduleName, "SetpointManagerOutdoorAirReset",
                                                  "Setpoint Manager Outdoor Air Reset", schedule);
      return result;
    }

    void SetpointManagerOutdoorAirReset_Impl::resetSchedule() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::ScheduleName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorLowTemperature2(boost::optional<double> setpointatOutdoorLowTemperature2) {
      bool result(false);
      if (setpointatOutdoorLowTemperature2) {
        result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, setpointatOutdoorLowTemperature2.get());
      } else {
        resetSetpointatOutdoorLowTemperature2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void SetpointManagerOutdoorAirReset_Impl::resetSetpointatOutdoorLowTemperature2() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setOutdoorLowTemperature2(boost::optional<double> outdoorLowTemperature2) {
      bool result(false);
      if (outdoorLowTemperature2) {
        result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, outdoorLowTemperature2.get());
      } else {
        resetOutdoorLowTemperature2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void SetpointManagerOutdoorAirReset_Impl::resetOutdoorLowTemperature2() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorHighTemperature2(boost::optional<double> setpointatOutdoorHighTemperature2) {
      bool result(false);
      if (setpointatOutdoorHighTemperature2) {
        result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, setpointatOutdoorHighTemperature2.get());
      } else {
        resetSetpointatOutdoorHighTemperature2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void SetpointManagerOutdoorAirReset_Impl::resetSetpointatOutdoorHighTemperature2() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirReset_Impl::setOutdoorHighTemperature2(boost::optional<double> outdoorHighTemperature2) {
      bool result(false);
      if (outdoorHighTemperature2) {
        result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, outdoorHighTemperature2.get());
      } else {
        resetOutdoorHighTemperature2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void SetpointManagerOutdoorAirReset_Impl::resetOutdoorHighTemperature2() {
      bool result = setString(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> SetpointManagerOutdoorAirReset_Impl::controlVariableValues() const {
      return SetpointManagerOutdoorAirReset::controlVariableValues();
    }

    boost::optional<ModelObject> SetpointManagerOutdoorAirReset_Impl::setpointNodeAsModelObject() const {
      OptionalModelObject result;
      OptionalNode intermediate = setpointNode();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> SetpointManagerOutdoorAirReset_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = schedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SetpointManagerOutdoorAirReset_Impl::setSetpointNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalNode intermediate = modelObject->optionalCast<Node>();
        if (intermediate) {
          Node node(*intermediate);
          return setSetpointNode(node);
        } else {
          return false;
        }
      } else {
        resetSetpointNode();
      }
      return true;
    }

    bool SetpointManagerOutdoorAirReset_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSchedule();
      }
      return true;
    }

  }  // namespace detail

  SetpointManagerOutdoorAirReset::SetpointManagerOutdoorAirReset(const Model& model)
    : SetpointManager(SetpointManagerOutdoorAirReset::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerOutdoorAirReset_Impl>());

    setControlVariable("Temperature");
    setSetpointatOutdoorLowTemperature(22.0);
    setOutdoorLowTemperature(10.0);
    setSetpointatOutdoorHighTemperature(10.0);
    setOutdoorHighTemperature(24.0);
    resetSchedule();
    resetSetpointatOutdoorLowTemperature2();
    resetOutdoorLowTemperature2();
    resetSetpointatOutdoorHighTemperature2();
    resetOutdoorHighTemperature2();
  }

  IddObjectType SetpointManagerOutdoorAirReset::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_OutdoorAirReset};
  }

  std::vector<std::string> SetpointManagerOutdoorAirReset::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_OutdoorAirResetFields::ControlVariable);
  }

  std::string SetpointManagerOutdoorAirReset::controlVariable() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->controlVariable();
  }

  bool SetpointManagerOutdoorAirReset::isControlVariableDefaulted() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->isControlVariableDefaulted();
  }

  double SetpointManagerOutdoorAirReset::setpointatOutdoorLowTemperature() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorLowTemperature();
  }

  double SetpointManagerOutdoorAirReset::outdoorLowTemperature() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorLowTemperature();
  }

  double SetpointManagerOutdoorAirReset::setpointatOutdoorHighTemperature() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorHighTemperature();
  }

  double SetpointManagerOutdoorAirReset::outdoorHighTemperature() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorHighTemperature();
  }

  boost::optional<Schedule> SetpointManagerOutdoorAirReset::schedule() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->schedule();
  }

  boost::optional<double> SetpointManagerOutdoorAirReset::setpointatOutdoorLowTemperature2() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorLowTemperature2();
  }

  boost::optional<double> SetpointManagerOutdoorAirReset::outdoorLowTemperature2() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorLowTemperature2();
  }

  boost::optional<double> SetpointManagerOutdoorAirReset::setpointatOutdoorHighTemperature2() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorHighTemperature2();
  }

  boost::optional<double> SetpointManagerOutdoorAirReset::outdoorHighTemperature2() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorHighTemperature2();
  }

  bool SetpointManagerOutdoorAirReset::setControlVariable(const std::string& controlVariable) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setControlVariable(controlVariable);
  }

  void SetpointManagerOutdoorAirReset::resetControlVariable() {
    getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetControlVariable();
  }

  bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorLowTemperature(setpointatOutdoorLowTemperature);
  }

  bool SetpointManagerOutdoorAirReset::setOutdoorLowTemperature(double outdoorLowTemperature) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorLowTemperature(outdoorLowTemperature);
  }

  bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorHighTemperature(setpointatOutdoorHighTemperature);
  }

  bool SetpointManagerOutdoorAirReset::setOutdoorHighTemperature(double outdoorHighTemperature) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorHighTemperature(outdoorHighTemperature);
  }

  bool SetpointManagerOutdoorAirReset::setSchedule(Schedule& schedule) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSchedule(schedule);
  }

  void SetpointManagerOutdoorAirReset::resetSchedule() {
    getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetSchedule();
  }

  bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorLowTemperature2(double setpointatOutdoorLowTemperature2) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorLowTemperature2(setpointatOutdoorLowTemperature2);
  }

  void SetpointManagerOutdoorAirReset::resetSetpointatOutdoorLowTemperature2() {
    getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetSetpointatOutdoorLowTemperature2();
  }

  bool SetpointManagerOutdoorAirReset::setOutdoorLowTemperature2(double outdoorLowTemperature2) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorLowTemperature2(outdoorLowTemperature2);
  }

  void SetpointManagerOutdoorAirReset::resetOutdoorLowTemperature2() {
    getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetOutdoorLowTemperature2();
  }

  bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorHighTemperature2(double setpointatOutdoorHighTemperature2) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorHighTemperature2(setpointatOutdoorHighTemperature2);
  }

  void SetpointManagerOutdoorAirReset::resetSetpointatOutdoorHighTemperature2() {
    getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetSetpointatOutdoorHighTemperature2();
  }

  bool SetpointManagerOutdoorAirReset::setOutdoorHighTemperature2(double outdoorHighTemperature2) {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorHighTemperature2(outdoorHighTemperature2);
  }

  void SetpointManagerOutdoorAirReset::resetOutdoorHighTemperature2() {
    getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetOutdoorHighTemperature2();
  }

  boost::optional<Node> SetpointManagerOutdoorAirReset::setpointNode() const {
    return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointNode();
  }

  /// @cond
  SetpointManagerOutdoorAirReset::SetpointManagerOutdoorAirReset(std::shared_ptr<detail::SetpointManagerOutdoorAirReset_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
