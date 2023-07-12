/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerDifferentialThermostat.hpp"
#include "AvailabilityManagerDifferentialThermostat_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_DifferentialThermostat_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerDifferentialThermostat_Impl::AvailabilityManagerDifferentialThermostat_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                   bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerDifferentialThermostat::iddObjectType());
    }

    AvailabilityManagerDifferentialThermostat_Impl::AvailabilityManagerDifferentialThermostat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerDifferentialThermostat::iddObjectType());
    }

    AvailabilityManagerDifferentialThermostat_Impl::AvailabilityManagerDifferentialThermostat_Impl(
      const AvailabilityManagerDifferentialThermostat_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerDifferentialThermostat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Differential Thermostat Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerDifferentialThermostat_Impl::iddObjectType() const {
      return AvailabilityManagerDifferentialThermostat::iddObjectType();
    }

    boost::optional<Node> AvailabilityManagerDifferentialThermostat_Impl::hotNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_DifferentialThermostatFields::HotNode);
    }

    boost::optional<Node> AvailabilityManagerDifferentialThermostat_Impl::coldNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_DifferentialThermostatFields::ColdNode);
    }

    double AvailabilityManagerDifferentialThermostat_Impl::temperatureDifferenceOnLimit() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOnLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AvailabilityManagerDifferentialThermostat_Impl::temperatureDifferenceOffLimit() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOffLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerDifferentialThermostat_Impl::setHotNode(const boost::optional<Node>& node) {
      bool result(false);
      if (node) {
        result = setPointer(OS_AvailabilityManager_DifferentialThermostatFields::HotNode, node.get().handle());
      } else {
        resetHotNode();
        result = true;
      }
      return result;
    }

    void AvailabilityManagerDifferentialThermostat_Impl::resetHotNode() {
      bool result = setString(OS_AvailabilityManager_DifferentialThermostatFields::HotNode, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerDifferentialThermostat_Impl::setColdNode(const boost::optional<Node>& node) {
      bool result(false);
      if (node) {
        result = setPointer(OS_AvailabilityManager_DifferentialThermostatFields::ColdNode, node.get().handle());
      } else {
        resetColdNode();
        result = true;
      }
      return result;
    }

    void AvailabilityManagerDifferentialThermostat_Impl::resetColdNode() {
      bool result = setString(OS_AvailabilityManager_DifferentialThermostatFields::ColdNode, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerDifferentialThermostat_Impl::setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit) {
      bool result = setDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOnLimit, temperatureDifferenceOnLimit);
      OS_ASSERT(result);
      return result;
    }

    bool AvailabilityManagerDifferentialThermostat_Impl::setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit) {
      bool result = setDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOffLimit, temperatureDifferenceOffLimit);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerDifferentialThermostat::AvailabilityManagerDifferentialThermostat(const Model& model)
    : AvailabilityManager(AvailabilityManagerDifferentialThermostat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>());

    setTemperatureDifferenceOnLimit(10.0);
    setTemperatureDifferenceOffLimit(2.0);
  }

  IddObjectType AvailabilityManagerDifferentialThermostat::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_DifferentialThermostat};
  }

  boost::optional<Node> AvailabilityManagerDifferentialThermostat::hotNode() const {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->hotNode();
  }

  boost::optional<Node> AvailabilityManagerDifferentialThermostat::coldNode() const {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->coldNode();
  }

  double AvailabilityManagerDifferentialThermostat::temperatureDifferenceOnLimit() const {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->temperatureDifferenceOnLimit();
  }

  double AvailabilityManagerDifferentialThermostat::temperatureDifferenceOffLimit() const {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->temperatureDifferenceOffLimit();
  }

  bool AvailabilityManagerDifferentialThermostat::setHotNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setHotNode(node);
  }

  void AvailabilityManagerDifferentialThermostat::resetHotNode() {
    getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->resetHotNode();
  }

  bool AvailabilityManagerDifferentialThermostat::setColdNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setColdNode(node);
  }

  void AvailabilityManagerDifferentialThermostat::resetColdNode() {
    getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->resetColdNode();
  }

  bool AvailabilityManagerDifferentialThermostat::setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit) {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setTemperatureDifferenceOnLimit(temperatureDifferenceOnLimit);
  }

  bool AvailabilityManagerDifferentialThermostat::setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit) {
    return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setTemperatureDifferenceOffLimit(temperatureDifferenceOffLimit);
  }

  /// @cond
  AvailabilityManagerDifferentialThermostat::AvailabilityManagerDifferentialThermostat(
    std::shared_ptr<detail::AvailabilityManagerDifferentialThermostat_Impl> impl)
    : AvailabilityManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
