/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkCrack_FieldEnums.hxx>

#include "AirflowNetworkReferenceCrackConditions.hpp"
#include "AirflowNetworkReferenceCrackConditions_Impl.hpp"

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkCrack_Impl::AirflowNetworkCrack_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkCrack::iddObjectType());
    }

    AirflowNetworkCrack_Impl::AirflowNetworkCrack_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkCrack::iddObjectType());
    }

    AirflowNetworkCrack_Impl::AirflowNetworkCrack_Impl(const AirflowNetworkCrack_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkCrack_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkCrack_Impl::iddObjectType() const {
      return AirflowNetworkCrack::iddObjectType();
    }

    double AirflowNetworkCrack_Impl::airMassFlowCoefficient() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkCrackFields::AirMassFlowCoefficientatReferenceConditions, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkCrack_Impl::airMassFlowExponent() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkCrackFields::AirMassFlowExponent, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<AirflowNetworkReferenceCrackConditions> AirflowNetworkCrack_Impl::referenceCrackConditions() const {
      return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkReferenceCrackConditions>(
        OS_AirflowNetworkCrackFields::ReferenceCrackConditions);
    }

    bool AirflowNetworkCrack_Impl::isAirMassFlowExponentDefaulted() const {
      return isEmpty(OS_AirflowNetworkCrackFields::AirMassFlowExponent);
    }

    bool AirflowNetworkCrack_Impl::setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions) {
      bool result = setDouble(OS_AirflowNetworkCrackFields::AirMassFlowCoefficientatReferenceConditions, airMassFlowCoefficientatReferenceConditions);
      return result;
    }

    bool AirflowNetworkCrack_Impl::setAirMassFlowExponent(double airMassFlowExponent) {
      bool result = setDouble(OS_AirflowNetworkCrackFields::AirMassFlowExponent, airMassFlowExponent);
      return result;
    }

    void AirflowNetworkCrack_Impl::resetAirMassFlowExponent() {
      bool result = setString(OS_AirflowNetworkCrackFields::AirMassFlowExponent, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkCrack_Impl::setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions) {
      bool result = setPointer(OS_AirflowNetworkCrackFields::ReferenceCrackConditions, referenceCrackConditions.handle());
      return result;
    }

    void AirflowNetworkCrack_Impl::resetReferenceCrackConditions() {
      bool result = setString(OS_AirflowNetworkCrackFields::ReferenceCrackConditions, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkCrack::AirflowNetworkCrack(const Model& model, double massFlowCoefficient)
    : AirflowNetworkComponent(AirflowNetworkCrack::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkCrack_Impl>());

    bool ok = setAirMassFlowCoefficient(massFlowCoefficient);
    OS_ASSERT(ok);
  }

  AirflowNetworkCrack::AirflowNetworkCrack(const Model& model, double massFlowCoefficient, double massFlowExponent)
    : AirflowNetworkComponent(AirflowNetworkCrack::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkCrack_Impl>());

    bool ok = setAirMassFlowCoefficient(massFlowCoefficient);
    OS_ASSERT(ok);
    ok = setAirMassFlowExponent(massFlowExponent);
    OS_ASSERT(ok);
  }

  AirflowNetworkCrack::AirflowNetworkCrack(const Model& model, double massFlowCoefficient, double massFlowExponent,
                                           const AirflowNetworkReferenceCrackConditions& referenceCrackConditions)
    : AirflowNetworkComponent(AirflowNetworkCrack::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkCrack_Impl>());

    bool ok = setAirMassFlowCoefficient(massFlowCoefficient);
    OS_ASSERT(ok);
    ok = setAirMassFlowExponent(massFlowExponent);
    OS_ASSERT(ok);
    ok = setReferenceCrackConditions(referenceCrackConditions);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkCrack::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkCrack};
  }

  double AirflowNetworkCrack::airMassFlowCoefficient() const {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->airMassFlowCoefficient();
  }

  double AirflowNetworkCrack::airMassFlowExponent() const {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->airMassFlowExponent();
  }

  bool AirflowNetworkCrack::isAirMassFlowExponentDefaulted() const {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->isAirMassFlowExponentDefaulted();
  }

  boost::optional<AirflowNetworkReferenceCrackConditions> AirflowNetworkCrack::referenceCrackConditions() const {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->referenceCrackConditions();
  }

  bool AirflowNetworkCrack::setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions) {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->setAirMassFlowCoefficient(airMassFlowCoefficientatReferenceConditions);
  }

  bool AirflowNetworkCrack::setAirMassFlowExponent(double airMassFlowExponent) {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->setAirMassFlowExponent(airMassFlowExponent);
  }

  void AirflowNetworkCrack::resetAirMassFlowExponent() {
    getImpl<detail::AirflowNetworkCrack_Impl>()->resetAirMassFlowExponent();
  }

  bool AirflowNetworkCrack::setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions) {
    return getImpl<detail::AirflowNetworkCrack_Impl>()->setReferenceCrackConditions(referenceCrackConditions);
  }

  void AirflowNetworkCrack::resetReferenceCrackConditions() {
    getImpl<detail::AirflowNetworkCrack_Impl>()->resetReferenceCrackConditions();
  }

  /// @cond
  AirflowNetworkCrack::AirflowNetworkCrack(std::shared_ptr<detail::AirflowNetworkCrack_Impl> impl) : AirflowNetworkComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
