/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerLowTemperatureTurnOn.hpp"
#include "AvailabilityManagerLowTemperatureTurnOn_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_LowTemperatureTurnOn_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerLowTemperatureTurnOn_Impl::AvailabilityManagerLowTemperatureTurnOn_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                               bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerLowTemperatureTurnOn::iddObjectType());
    }

    AvailabilityManagerLowTemperatureTurnOn_Impl::AvailabilityManagerLowTemperatureTurnOn_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerLowTemperatureTurnOn::iddObjectType());
    }

    AvailabilityManagerLowTemperatureTurnOn_Impl::AvailabilityManagerLowTemperatureTurnOn_Impl(
      const AvailabilityManagerLowTemperatureTurnOn_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerLowTemperatureTurnOn_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Low Temperature Turn On Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerLowTemperatureTurnOn_Impl::iddObjectType() const {
      return AvailabilityManagerLowTemperatureTurnOn::iddObjectType();
    }

    boost::optional<Node> AvailabilityManagerLowTemperatureTurnOn_Impl::sensorNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_LowTemperatureTurnOnFields::SensorNodeName);
    }

    double AvailabilityManagerLowTemperatureTurnOn_Impl::temperature() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_LowTemperatureTurnOnFields::Temperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerLowTemperatureTurnOn_Impl::setSensorNode(const Node& node) {
      bool result = setPointer(OS_AvailabilityManager_LowTemperatureTurnOnFields::SensorNodeName, node.handle());
      return result;
    }

    void AvailabilityManagerLowTemperatureTurnOn_Impl::resetSensorNode() {
      bool result = setString(OS_AvailabilityManager_LowTemperatureTurnOnFields::SensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerLowTemperatureTurnOn_Impl::setTemperature(double temperature) {
      bool result = setDouble(OS_AvailabilityManager_LowTemperatureTurnOnFields::Temperature, temperature);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerLowTemperatureTurnOn::AvailabilityManagerLowTemperatureTurnOn(const Model& model)
    : AvailabilityManager(AvailabilityManagerLowTemperatureTurnOn::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerLowTemperatureTurnOn_Impl>());

    setTemperature(30);
  }

  IddObjectType AvailabilityManagerLowTemperatureTurnOn::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOn};
  }

  boost::optional<Node> AvailabilityManagerLowTemperatureTurnOn::sensorNode() const {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOn_Impl>()->sensorNode();
  }

  double AvailabilityManagerLowTemperatureTurnOn::temperature() const {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOn_Impl>()->temperature();
  }

  bool AvailabilityManagerLowTemperatureTurnOn::setSensorNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOn_Impl>()->setSensorNode(node);
  }

  void AvailabilityManagerLowTemperatureTurnOn::resetSensorNode() {
    getImpl<detail::AvailabilityManagerLowTemperatureTurnOn_Impl>()->resetSensorNode();
  }

  bool AvailabilityManagerLowTemperatureTurnOn::setTemperature(double temperature) {
    return getImpl<detail::AvailabilityManagerLowTemperatureTurnOn_Impl>()->setTemperature(temperature);
  }

  /// @cond
  AvailabilityManagerLowTemperatureTurnOn::AvailabilityManagerLowTemperatureTurnOn(
    std::shared_ptr<detail::AvailabilityManagerLowTemperatureTurnOn_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
