/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerHighTemperatureTurnOn.hpp"
#include "AvailabilityManagerHighTemperatureTurnOn_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_HighTemperatureTurnOn_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerHighTemperatureTurnOn_Impl::AvailabilityManagerHighTemperatureTurnOn_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerHighTemperatureTurnOn::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOn_Impl::AvailabilityManagerHighTemperatureTurnOn_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerHighTemperatureTurnOn::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOn_Impl::AvailabilityManagerHighTemperatureTurnOn_Impl(
      const AvailabilityManagerHighTemperatureTurnOn_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerHighTemperatureTurnOn_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager High Temperature Turn On Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerHighTemperatureTurnOn_Impl::iddObjectType() const {
      return AvailabilityManagerHighTemperatureTurnOn::iddObjectType();
    }

    boost::optional<Node> AvailabilityManagerHighTemperatureTurnOn_Impl::sensorNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_HighTemperatureTurnOnFields::SensorNodeName);
    }

    double AvailabilityManagerHighTemperatureTurnOn_Impl::temperature() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_HighTemperatureTurnOnFields::Temperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerHighTemperatureTurnOn_Impl::setSensorNode(const Node& node) {
      bool result = setPointer(OS_AvailabilityManager_HighTemperatureTurnOnFields::SensorNodeName, node.handle());
      return result;
    }

    void AvailabilityManagerHighTemperatureTurnOn_Impl::resetSensorNode() {
      bool result = setString(OS_AvailabilityManager_HighTemperatureTurnOnFields::SensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerHighTemperatureTurnOn_Impl::setTemperature(double temperature) {
      bool result = setDouble(OS_AvailabilityManager_HighTemperatureTurnOnFields::Temperature, temperature);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerHighTemperatureTurnOn::AvailabilityManagerHighTemperatureTurnOn(const Model& model)
    : AvailabilityManager(AvailabilityManagerHighTemperatureTurnOn::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>());

    setTemperature(30);
  }

  IddObjectType AvailabilityManagerHighTemperatureTurnOn::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOn};
  }

  boost::optional<Node> AvailabilityManagerHighTemperatureTurnOn::sensorNode() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->sensorNode();
  }

  double AvailabilityManagerHighTemperatureTurnOn::temperature() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->temperature();
  }

  bool AvailabilityManagerHighTemperatureTurnOn::setSensorNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->setSensorNode(node);
  }

  void AvailabilityManagerHighTemperatureTurnOn::resetSensorNode() {
    getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->resetSensorNode();
  }

  bool AvailabilityManagerHighTemperatureTurnOn::setTemperature(double temperature) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->setTemperature(temperature);
  }

  /// @cond
  AvailabilityManagerHighTemperatureTurnOn::AvailabilityManagerHighTemperatureTurnOn(
    std::shared_ptr<detail::AvailabilityManagerHighTemperatureTurnOn_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
