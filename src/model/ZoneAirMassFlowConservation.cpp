/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneAirMassFlowConservation.hpp"
#include "ZoneAirMassFlowConservation_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneAirMassFlowConservation_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneAirMassFlowConservation_Impl::ZoneAirMassFlowConservation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneAirMassFlowConservation::iddObjectType());
    }

    ZoneAirMassFlowConservation_Impl::ZoneAirMassFlowConservation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneAirMassFlowConservation::iddObjectType());
    }

    ZoneAirMassFlowConservation_Impl::ZoneAirMassFlowConservation_Impl(const ZoneAirMassFlowConservation_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneAirMassFlowConservation_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Zone Infiltration Air Mass Flow Balance Status",
                                                   "Zone Mass Balance Infiltration Air Mass Flow Rate"};
      return result;
    }

    IddObjectType ZoneAirMassFlowConservation_Impl::iddObjectType() const {
      return ZoneAirMassFlowConservation::iddObjectType();
    }

    std::string ZoneAirMassFlowConservation_Impl::adjustZoneMixingandReturnForAirMassFlowBalance() const {
      boost::optional<std::string> value = getString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneAirMassFlowConservation_Impl::isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted() const {
      return isEmpty(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance);
    }

    std::string ZoneAirMassFlowConservation_Impl::infiltrationBalancingMethod() const {
      boost::optional<std::string> value = getString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneAirMassFlowConservation_Impl::isInfiltrationBalancingMethodDefaulted() const {
      return isEmpty(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod);
    }

    std::string ZoneAirMassFlowConservation_Impl::infiltrationBalancingZones() const {
      boost::optional<std::string> value = getString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneAirMassFlowConservation_Impl::isInfiltrationBalancingZonesDefaulted() const {
      return isEmpty(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones);
    }

    bool ZoneAirMassFlowConservation_Impl::setAdjustZoneMixingandReturnForAirMassFlowBalance(
      const std::string& adjustZoneMixingandReturnForAirMassFlowBalance) {
      bool result = setString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance,
                              adjustZoneMixingandReturnForAirMassFlowBalance);
      return result;
    }

    void ZoneAirMassFlowConservation_Impl::resetAdjustZoneMixingandReturnForAirMassFlowBalance() {
      bool result = setString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance, "");
      OS_ASSERT(result);
    }

    bool ZoneAirMassFlowConservation_Impl::setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod) {
      bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, infiltrationBalancingMethod);
      return result;
    }

    void ZoneAirMassFlowConservation_Impl::resetInfiltrationBalancingMethod() {
      bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, "");
      OS_ASSERT(result);
    }

    bool ZoneAirMassFlowConservation_Impl::setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones) {
      bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, infiltrationBalancingZones);
      return result;
    }

    void ZoneAirMassFlowConservation_Impl::resetInfiltrationBalancingZones() {
      bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType ZoneAirMassFlowConservation::iddObjectType() {
    return {IddObjectType::OS_ZoneAirMassFlowConservation};
  }

  std::vector<std::string> ZoneAirMassFlowConservation::adjustZoneMixingandReturnForAirMassFlowBalanceValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance);
  }

  std::vector<std::string> ZoneAirMassFlowConservation::infiltrationBalancingMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod);
  }

  std::vector<std::string> ZoneAirMassFlowConservation::infiltrationBalancingZonesValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones);
  }

  std::string ZoneAirMassFlowConservation::adjustZoneMixingandReturnForAirMassFlowBalance() const {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->adjustZoneMixingandReturnForAirMassFlowBalance();
  }

  bool ZoneAirMassFlowConservation::isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted() const {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted();
  }

  std::string ZoneAirMassFlowConservation::infiltrationBalancingMethod() const {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->infiltrationBalancingMethod();
  }

  bool ZoneAirMassFlowConservation::isInfiltrationBalancingMethodDefaulted() const {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isInfiltrationBalancingMethodDefaulted();
  }

  std::string ZoneAirMassFlowConservation::infiltrationBalancingZones() const {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->infiltrationBalancingZones();
  }

  bool ZoneAirMassFlowConservation::isInfiltrationBalancingZonesDefaulted() const {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isInfiltrationBalancingZonesDefaulted();
  }

  bool ZoneAirMassFlowConservation::setAdjustZoneMixingandReturnForAirMassFlowBalance(
    const std::string& adjustZoneMixingandReturnForAirMassFlowBalance) {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setAdjustZoneMixingandReturnForAirMassFlowBalance(
      adjustZoneMixingandReturnForAirMassFlowBalance);
  }

  void ZoneAirMassFlowConservation::resetAdjustZoneMixingandReturnForAirMassFlowBalance() {
    getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetAdjustZoneMixingandReturnForAirMassFlowBalance();
  }

  bool ZoneAirMassFlowConservation::setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod) {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setInfiltrationBalancingMethod(infiltrationBalancingMethod);
  }

  void ZoneAirMassFlowConservation::resetInfiltrationBalancingMethod() {
    getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetInfiltrationBalancingMethod();
  }

  bool ZoneAirMassFlowConservation::setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones) {
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setInfiltrationBalancingZones(infiltrationBalancingZones);
  }

  void ZoneAirMassFlowConservation::resetInfiltrationBalancingZones() {
    getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetInfiltrationBalancingZones();
  }

  /// @cond
  ZoneAirMassFlowConservation::ZoneAirMassFlowConservation(std::shared_ptr<detail::ZoneAirMassFlowConservation_Impl> impl)
    : ModelObject(std::move(impl)) {}
  ZoneAirMassFlowConservation::ZoneAirMassFlowConservation(Model& model) : ModelObject(ZoneAirMassFlowConservation::iddObjectType(), model) {}

  // DEPRECATED METHODS

  bool ZoneAirMassFlowConservation::adjustZoneMixingForZoneAirMassFlowBalance() const {
    // TODO: DEPRECATED
    std::string cur_choice = getImpl<detail::ZoneAirMassFlowConservation_Impl>()->adjustZoneMixingandReturnForAirMassFlowBalance();
    DEPRECATED_AT_MSG(3, 2, 0, "It captures two out of five possible choices, use adjustZoneMixingandReturnForAirMassFlowBalance instead.");
    if (openstudio::istringEqual("None", cur_choice)) {
      return false;
    } else if (openstudio::istringEqual("AdjustMixingOnly", cur_choice)) {
      return true;
    } else {
      LOG(Warn,
          "ZoneAirMassFlowConservation: '" << cur_choice
                                           << "' doesn't technically mean the same thing as the former 'Yes' (which would be 'AdjustMixingOnly').");
      return true;
    }
  }

  bool ZoneAirMassFlowConservation::isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted() const {
    DEPRECATED_AT_MSG(3, 2, 0,
                      "It captures two out of five possible choices, use isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted instead.");
    return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted();
  }

  bool ZoneAirMassFlowConservation::setAdjustZoneMixingForZoneAirMassFlowBalance(bool adjustZoneMixingForZoneAirMassFlowBalance) {
    DEPRECATED_AT_MSG(3, 2, 0, "It captures two out of five possible choices, use setAdjustZoneMixingandReturnForAirMassFlowBalance instead.");
    if (adjustZoneMixingForZoneAirMassFlowBalance) {
      return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setAdjustZoneMixingandReturnForAirMassFlowBalance("AdjustMixingOnly");
    } else {
      return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setAdjustZoneMixingandReturnForAirMassFlowBalance("None");
    }
  }

  void ZoneAirMassFlowConservation::resetAdjustZoneMixingForZoneAirMassFlowBalance() {
    DEPRECATED_AT_MSG(3, 2, 0, "It captures two out of five possible choices, use resetAdjustZoneMixingandReturnForAirMassFlowBalance instead.");
    getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetAdjustZoneMixingandReturnForAirMassFlowBalance();
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
