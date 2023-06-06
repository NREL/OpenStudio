/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkSimpleOpening.hpp"
#include "AirflowNetworkSimpleOpening_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkSimpleOpening_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkSimpleOpening_Impl::AirflowNetworkSimpleOpening_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkSimpleOpening::iddObjectType());
    }

    AirflowNetworkSimpleOpening_Impl::AirflowNetworkSimpleOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkSimpleOpening::iddObjectType());
    }

    AirflowNetworkSimpleOpening_Impl::AirflowNetworkSimpleOpening_Impl(const AirflowNetworkSimpleOpening_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkSimpleOpening_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkSimpleOpening_Impl::iddObjectType() const {
      return AirflowNetworkSimpleOpening::iddObjectType();
    }

    double AirflowNetworkSimpleOpening_Impl::airMassFlowCoefficientWhenOpeningisClosed() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkSimpleOpening_Impl::airMassFlowExponentWhenOpeningisClosed() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSimpleOpening_Impl::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const {
      return isEmpty(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed);
    }

    double AirflowNetworkSimpleOpening_Impl::minimumDensityDifferenceforTwoWayFlow() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::MinimumDensityDifferenceforTwoWayFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkSimpleOpening_Impl::dischargeCoefficient() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSimpleOpeningFields::DischargeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSimpleOpening_Impl::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed) {
      return setDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, airMassFlowCoefficientWhenOpeningisClosed);
    }

    bool AirflowNetworkSimpleOpening_Impl::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
      return setDouble(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed, airMassFlowExponentWhenOpeningisClosed);
    }

    void AirflowNetworkSimpleOpening_Impl::resetAirMassFlowExponentWhenOpeningisClosed() {
      bool result = setString(OS_AirflowNetworkSimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkSimpleOpening_Impl::setMinimumDensityDifferenceforTwoWayFlow(double minimumDensityDifferenceforTwoWayFlow) {
      return setDouble(OS_AirflowNetworkSimpleOpeningFields::MinimumDensityDifferenceforTwoWayFlow, minimumDensityDifferenceforTwoWayFlow);
    }

    bool AirflowNetworkSimpleOpening_Impl::setDischargeCoefficient(double dischargeCoefficient) {
      bool result = setDouble(OS_AirflowNetworkSimpleOpeningFields::DischargeCoefficient, dischargeCoefficient);
      return result;
    }

  }  // namespace detail

  AirflowNetworkSimpleOpening::AirflowNetworkSimpleOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                                           double minimumDensityDifferenceforTwoWayFlow, double dischargeCoefficient)
    : AirflowNetworkComponent(AirflowNetworkSimpleOpening::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkSimpleOpening_Impl>());

    bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setMinimumDensityDifferenceforTwoWayFlow(minimumDensityDifferenceforTwoWayFlow);
    OS_ASSERT(ok);
    ok = setDischargeCoefficient(dischargeCoefficient);
    OS_ASSERT(ok);
  }

  AirflowNetworkSimpleOpening::AirflowNetworkSimpleOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                                           double massFlowExponentWhenOpeningisClosed, double minimumDensityDifferenceforTwoWayFlow,
                                                           double dischargeCoefficient)
    : AirflowNetworkComponent(AirflowNetworkSimpleOpening::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkSimpleOpening_Impl>());

    bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setAirMassFlowExponentWhenOpeningisClosed(massFlowExponentWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setMinimumDensityDifferenceforTwoWayFlow(minimumDensityDifferenceforTwoWayFlow);
    OS_ASSERT(ok);
    ok = setDischargeCoefficient(dischargeCoefficient);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkSimpleOpening::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkSimpleOpening};
  }

  double AirflowNetworkSimpleOpening::airMassFlowCoefficientWhenOpeningisClosed() const {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->airMassFlowCoefficientWhenOpeningisClosed();
  }

  double AirflowNetworkSimpleOpening::airMassFlowExponentWhenOpeningisClosed() const {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->airMassFlowExponentWhenOpeningisClosed();
  }

  bool AirflowNetworkSimpleOpening::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->isAirMassFlowExponentWhenOpeningisClosedDefaulted();
  }

  double AirflowNetworkSimpleOpening::minimumDensityDifferenceforTwoWayFlow() const {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->minimumDensityDifferenceforTwoWayFlow();
  }

  double AirflowNetworkSimpleOpening::dischargeCoefficient() const {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->dischargeCoefficient();
  }

  bool AirflowNetworkSimpleOpening::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed) {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setAirMassFlowCoefficientWhenOpeningisClosed(
      airMassFlowCoefficientWhenOpeningisClosed);
  }

  bool AirflowNetworkSimpleOpening::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setAirMassFlowExponentWhenOpeningisClosed(airMassFlowExponentWhenOpeningisClosed);
  }

  void AirflowNetworkSimpleOpening::resetAirMassFlowExponentWhenOpeningisClosed() {
    getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->resetAirMassFlowExponentWhenOpeningisClosed();
  }

  bool AirflowNetworkSimpleOpening::setMinimumDensityDifferenceforTwoWayFlow(double minimumDensityDifferenceforTwoWayFlow) {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setMinimumDensityDifferenceforTwoWayFlow(minimumDensityDifferenceforTwoWayFlow);
  }

  bool AirflowNetworkSimpleOpening::setDischargeCoefficient(double dischargeCoefficient) {
    return getImpl<detail::AirflowNetworkSimpleOpening_Impl>()->setDischargeCoefficient(dischargeCoefficient);
  }

  /// @cond
  AirflowNetworkSimpleOpening::AirflowNetworkSimpleOpening(std::shared_ptr<detail::AirflowNetworkSimpleOpening_Impl> impl)
    : AirflowNetworkComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
