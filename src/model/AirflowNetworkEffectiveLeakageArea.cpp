/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkEffectiveLeakageArea.hpp"
#include "AirflowNetworkEffectiveLeakageArea_Impl.hpp"

#include <utilities/idd/OS_AirflowNetworkEffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkEffectiveLeakageArea_Impl::AirflowNetworkEffectiveLeakageArea_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkEffectiveLeakageArea::iddObjectType());
    }

    AirflowNetworkEffectiveLeakageArea_Impl::AirflowNetworkEffectiveLeakageArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkEffectiveLeakageArea::iddObjectType());
    }

    AirflowNetworkEffectiveLeakageArea_Impl::AirflowNetworkEffectiveLeakageArea_Impl(const AirflowNetworkEffectiveLeakageArea_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkEffectiveLeakageArea_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkEffectiveLeakageArea_Impl::iddObjectType() const {
      return AirflowNetworkEffectiveLeakageArea::iddObjectType();
    }

    double AirflowNetworkEffectiveLeakageArea_Impl::effectiveLeakageArea() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::EffectiveLeakageArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkEffectiveLeakageArea_Impl::dischargeCoefficient() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::DischargeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::isDischargeCoefficientDefaulted() const {
      return isEmpty(OS_AirflowNetworkEffectiveLeakageAreaFields::DischargeCoefficient);
    }

    double AirflowNetworkEffectiveLeakageArea_Impl::referencePressureDifference() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::ReferencePressureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::isReferencePressureDifferenceDefaulted() const {
      return isEmpty(OS_AirflowNetworkEffectiveLeakageAreaFields::ReferencePressureDifference);
    }

    double AirflowNetworkEffectiveLeakageArea_Impl::airMassFlowExponent() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::AirMassFlowExponent, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::isAirMassFlowExponentDefaulted() const {
      return isEmpty(OS_AirflowNetworkEffectiveLeakageAreaFields::AirMassFlowExponent);
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::setEffectiveLeakageArea(double effectiveLeakageArea) {
      bool result = setDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::EffectiveLeakageArea, effectiveLeakageArea);
      return result;
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::setDischargeCoefficient(double dischargeCoefficient) {
      bool result = setDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::DischargeCoefficient, dischargeCoefficient);
      return result;
    }

    void AirflowNetworkEffectiveLeakageArea_Impl::resetDischargeCoefficient() {
      bool result = setString(OS_AirflowNetworkEffectiveLeakageAreaFields::DischargeCoefficient, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::setReferencePressureDifference(double referencePressureDifference) {
      bool result = setDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::ReferencePressureDifference, referencePressureDifference);
      return result;
    }

    void AirflowNetworkEffectiveLeakageArea_Impl::resetReferencePressureDifference() {
      bool result = setString(OS_AirflowNetworkEffectiveLeakageAreaFields::ReferencePressureDifference, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkEffectiveLeakageArea_Impl::setAirMassFlowExponent(double airMassFlowExponent) {
      bool result = setDouble(OS_AirflowNetworkEffectiveLeakageAreaFields::AirMassFlowExponent, airMassFlowExponent);
      return result;
    }

    void AirflowNetworkEffectiveLeakageArea_Impl::resetAirMassFlowExponent() {
      bool result = setString(OS_AirflowNetworkEffectiveLeakageAreaFields::AirMassFlowExponent, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkEffectiveLeakageArea::AirflowNetworkEffectiveLeakageArea(const Model& model, double effectiveLeakageArea)
    : AirflowNetworkComponent(AirflowNetworkEffectiveLeakageArea::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>());

    bool ok = setEffectiveLeakageArea(effectiveLeakageArea);
    OS_ASSERT(ok);
  }

  AirflowNetworkEffectiveLeakageArea::AirflowNetworkEffectiveLeakageArea(const Model& model, double effectiveLeakageArea, double dischargeCoefficient,
                                                                         double referencePressureDifference, double massFlowExponent)
    : AirflowNetworkComponent(AirflowNetworkEffectiveLeakageArea::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>());

    bool ok = setEffectiveLeakageArea(effectiveLeakageArea);
    OS_ASSERT(ok);
    ok = setDischargeCoefficient(dischargeCoefficient);
    OS_ASSERT(ok);
    ok = setReferencePressureDifference(referencePressureDifference);
    OS_ASSERT(ok);
    ok = setAirMassFlowExponent(massFlowExponent);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkEffectiveLeakageArea::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkEffectiveLeakageArea};
  }

  double AirflowNetworkEffectiveLeakageArea::effectiveLeakageArea() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->effectiveLeakageArea();
  }

  double AirflowNetworkEffectiveLeakageArea::dischargeCoefficient() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->dischargeCoefficient();
  }

  bool AirflowNetworkEffectiveLeakageArea::isDischargeCoefficientDefaulted() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->isDischargeCoefficientDefaulted();
  }

  double AirflowNetworkEffectiveLeakageArea::referencePressureDifference() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->referencePressureDifference();
  }

  bool AirflowNetworkEffectiveLeakageArea::isReferencePressureDifferenceDefaulted() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->isReferencePressureDifferenceDefaulted();
  }

  double AirflowNetworkEffectiveLeakageArea::airMassFlowExponent() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->airMassFlowExponent();
  }

  bool AirflowNetworkEffectiveLeakageArea::isAirMassFlowExponentDefaulted() const {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->isAirMassFlowExponentDefaulted();
  }

  bool AirflowNetworkEffectiveLeakageArea::setEffectiveLeakageArea(double effectiveLeakageArea) {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->setEffectiveLeakageArea(effectiveLeakageArea);
  }

  bool AirflowNetworkEffectiveLeakageArea::setDischargeCoefficient(double dischargeCoefficient) {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->setDischargeCoefficient(dischargeCoefficient);
  }

  void AirflowNetworkEffectiveLeakageArea::resetDischargeCoefficient() {
    getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->resetDischargeCoefficient();
  }

  bool AirflowNetworkEffectiveLeakageArea::setReferencePressureDifference(double referencePressureDifference) {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->setReferencePressureDifference(referencePressureDifference);
  }

  void AirflowNetworkEffectiveLeakageArea::resetReferencePressureDifference() {
    getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->resetReferencePressureDifference();
  }

  bool AirflowNetworkEffectiveLeakageArea::setAirMassFlowExponent(double airMassFlowExponent) {
    return getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->setAirMassFlowExponent(airMassFlowExponent);
  }

  void AirflowNetworkEffectiveLeakageArea::resetAirMassFlowExponent() {
    getImpl<detail::AirflowNetworkEffectiveLeakageArea_Impl>()->resetAirMassFlowExponent();
  }

  /// @cond
  AirflowNetworkEffectiveLeakageArea::AirflowNetworkEffectiveLeakageArea(std::shared_ptr<detail::AirflowNetworkEffectiveLeakageArea_Impl> impl)
    : AirflowNetworkComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
