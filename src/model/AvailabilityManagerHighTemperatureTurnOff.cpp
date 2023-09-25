/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerHighTemperatureTurnOff.hpp"
#include "AvailabilityManagerHighTemperatureTurnOff_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_HighTemperatureTurnOff_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerHighTemperatureTurnOff_Impl::AvailabilityManagerHighTemperatureTurnOff_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                   bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerHighTemperatureTurnOff::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOff_Impl::AvailabilityManagerHighTemperatureTurnOff_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerHighTemperatureTurnOff::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOff_Impl::AvailabilityManagerHighTemperatureTurnOff_Impl(
      const AvailabilityManagerHighTemperatureTurnOff_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerHighTemperatureTurnOff_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager High Temperature Turn Off Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerHighTemperatureTurnOff_Impl::iddObjectType() const {
      return AvailabilityManagerHighTemperatureTurnOff::iddObjectType();
    }

    boost::optional<Node> AvailabilityManagerHighTemperatureTurnOff_Impl::sensorNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_HighTemperatureTurnOffFields::SensorNodeName);
    }

    double AvailabilityManagerHighTemperatureTurnOff_Impl::temperature() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_HighTemperatureTurnOffFields::Temperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerHighTemperatureTurnOff_Impl::setSensorNode(const Node& node) {
      bool result = setPointer(OS_AvailabilityManager_HighTemperatureTurnOffFields::SensorNodeName, node.handle());
      return result;
    }

    void AvailabilityManagerHighTemperatureTurnOff_Impl::resetSensorNode() {
      bool result = setString(OS_AvailabilityManager_HighTemperatureTurnOffFields::SensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerHighTemperatureTurnOff_Impl::setTemperature(double temperature) {
      bool result = setDouble(OS_AvailabilityManager_HighTemperatureTurnOffFields::Temperature, temperature);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerHighTemperatureTurnOff::AvailabilityManagerHighTemperatureTurnOff(const Model& model)
    : AvailabilityManager(AvailabilityManagerHighTemperatureTurnOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>());

    setTemperature(30);
  }

  IddObjectType AvailabilityManagerHighTemperatureTurnOff::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOff};
  }

  boost::optional<Node> AvailabilityManagerHighTemperatureTurnOff::sensorNode() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->sensorNode();
  }

  double AvailabilityManagerHighTemperatureTurnOff::temperature() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->temperature();
  }

  bool AvailabilityManagerHighTemperatureTurnOff::setSensorNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->setSensorNode(node);
  }

  void AvailabilityManagerHighTemperatureTurnOff::resetSensorNode() {
    getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->resetSensorNode();
  }

  bool AvailabilityManagerHighTemperatureTurnOff::setTemperature(double temperature) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->setTemperature(temperature);
  }

  /// @cond
  AvailabilityManagerHighTemperatureTurnOff::AvailabilityManagerHighTemperatureTurnOff(
    std::shared_ptr<detail::AvailabilityManagerHighTemperatureTurnOff_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
