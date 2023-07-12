/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkLeakageRatio.hpp"
#include "AirflowNetworkLeakageRatio_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkLeakageRatio_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkLeakageRatio_Impl::AirflowNetworkLeakageRatio_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkLeakageRatio::iddObjectType());
    }

    AirflowNetworkLeakageRatio_Impl::AirflowNetworkLeakageRatio_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkLeakageRatio::iddObjectType());
    }

    AirflowNetworkLeakageRatio_Impl::AirflowNetworkLeakageRatio_Impl(const AirflowNetworkLeakageRatio_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkLeakageRatio_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkLeakageRatio_Impl::iddObjectType() const {
      return AirflowNetworkLeakageRatio::iddObjectType();
    }

    boost::optional<double> AirflowNetworkLeakageRatio_Impl::effectiveLeakageRatio() const {
      return getDouble(OS_AirflowNetworkLeakageRatioFields::EffectiveLeakageRatio, true);
    }

    double AirflowNetworkLeakageRatio_Impl::maximumFlowRate() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkLeakageRatioFields::MaximumFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkLeakageRatio_Impl::referencePressureDifference() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkLeakageRatioFields::ReferencePressureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkLeakageRatio_Impl::airMassFlowExponent() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkLeakageRatioFields::AirMassFlowExponent, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkLeakageRatio_Impl::isAirMassFlowExponentDefaulted() const {
      return isEmpty(OS_AirflowNetworkLeakageRatioFields::AirMassFlowExponent);
    }

    bool AirflowNetworkLeakageRatio_Impl::setEffectiveLeakageRatio(double effectiveLeakageRatio) {
      bool result = setDouble(OS_AirflowNetworkLeakageRatioFields::EffectiveLeakageRatio, effectiveLeakageRatio);
      return result;
    }

    void AirflowNetworkLeakageRatio_Impl::resetEffectiveLeakageRatio() {
      bool result = setString(OS_AirflowNetworkLeakageRatioFields::EffectiveLeakageRatio, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkLeakageRatio_Impl::setMaximumFlowRate(double maximumFlowRate) {
      bool result = setDouble(OS_AirflowNetworkLeakageRatioFields::MaximumFlowRate, maximumFlowRate);
      return result;
    }

    bool AirflowNetworkLeakageRatio_Impl::setReferencePressureDifference(double referencePressureDifference) {
      bool result = setDouble(OS_AirflowNetworkLeakageRatioFields::ReferencePressureDifference, referencePressureDifference);
      return result;
    }

    bool AirflowNetworkLeakageRatio_Impl::setAirMassFlowExponent(double airMassFlowExponent) {
      bool result = setDouble(OS_AirflowNetworkLeakageRatioFields::AirMassFlowExponent, airMassFlowExponent);
      return result;
    }

    void AirflowNetworkLeakageRatio_Impl::resetAirMassFlowExponent() {
      bool result = setString(OS_AirflowNetworkLeakageRatioFields::AirMassFlowExponent, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkLeakageRatio::AirflowNetworkLeakageRatio(const Model& model)
    : AirflowNetworkComponent(AirflowNetworkLeakageRatio::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkLeakageRatio_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setMaximumFlowRate();
    OS_ASSERT(ok);
    // ok = setReferencePressureDifference();
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkLeakageRatio::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkLeakageRatio};
  }

  boost::optional<double> AirflowNetworkLeakageRatio::effectiveLeakageRatio() const {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->effectiveLeakageRatio();
  }

  double AirflowNetworkLeakageRatio::maximumFlowRate() const {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->maximumFlowRate();
  }

  double AirflowNetworkLeakageRatio::referencePressureDifference() const {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->referencePressureDifference();
  }

  double AirflowNetworkLeakageRatio::airMassFlowExponent() const {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->airMassFlowExponent();
  }

  bool AirflowNetworkLeakageRatio::isAirMassFlowExponentDefaulted() const {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->isAirMassFlowExponentDefaulted();
  }

  bool AirflowNetworkLeakageRatio::setEffectiveLeakageRatio(double effectiveLeakageRatio) {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->setEffectiveLeakageRatio(effectiveLeakageRatio);
  }

  void AirflowNetworkLeakageRatio::resetEffectiveLeakageRatio() {
    getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->resetEffectiveLeakageRatio();
  }

  bool AirflowNetworkLeakageRatio::setMaximumFlowRate(double maximumFlowRate) {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->setMaximumFlowRate(maximumFlowRate);
  }

  bool AirflowNetworkLeakageRatio::setReferencePressureDifference(double referencePressureDifference) {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->setReferencePressureDifference(referencePressureDifference);
  }

  bool AirflowNetworkLeakageRatio::setAirMassFlowExponent(double airMassFlowExponent) {
    return getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->setAirMassFlowExponent(airMassFlowExponent);
  }

  void AirflowNetworkLeakageRatio::resetAirMassFlowExponent() {
    getImpl<detail::AirflowNetworkLeakageRatio_Impl>()->resetAirMassFlowExponent();
  }

  /// @cond
  AirflowNetworkLeakageRatio::AirflowNetworkLeakageRatio(std::shared_ptr<detail::AirflowNetworkLeakageRatio_Impl> impl)
    : AirflowNetworkComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
