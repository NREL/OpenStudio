/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkSpecifiedFlowRate.hpp"
#include "AirflowNetworkSpecifiedFlowRate_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AirflowNetworkSpecifiedFlowRate_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkSpecifiedFlowRate_Impl::AirflowNetworkSpecifiedFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkSpecifiedFlowRate::iddObjectType());
    }

    AirflowNetworkSpecifiedFlowRate_Impl::AirflowNetworkSpecifiedFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkSpecifiedFlowRate::iddObjectType());
    }

    AirflowNetworkSpecifiedFlowRate_Impl::AirflowNetworkSpecifiedFlowRate_Impl(const AirflowNetworkSpecifiedFlowRate_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkSpecifiedFlowRate_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkSpecifiedFlowRate_Impl::iddObjectType() const {
      return AirflowNetworkSpecifiedFlowRate::iddObjectType();
    }

    double AirflowNetworkSpecifiedFlowRate_Impl::airFlowValue() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirflowNetworkSpecifiedFlowRate_Impl::airFlowUnits() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowUnits, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkSpecifiedFlowRate_Impl::isAirFlowUnitsDefaulted() const {
      return isEmpty(OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowUnits);
    }

    bool AirflowNetworkSpecifiedFlowRate_Impl::setAirFlowValue(double airFlowValue) {
      bool result = setDouble(OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowValue, airFlowValue);
      return result;
    }

    bool AirflowNetworkSpecifiedFlowRate_Impl::setAirFlowUnits(const std::string& airFlowUnits) {
      bool result = setString(OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowUnits, airFlowUnits);
      return result;
    }

    void AirflowNetworkSpecifiedFlowRate_Impl::resetAirFlowUnits() {
      bool result = setString(OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowUnits, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkSpecifiedFlowRate::AirflowNetworkSpecifiedFlowRate(const Model& model)
    : AirflowNetworkComponent(AirflowNetworkSpecifiedFlowRate::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>());

    bool ok = setAirFlowValue(0);
    OS_ASSERT(ok);
  }

  AirflowNetworkSpecifiedFlowRate::AirflowNetworkSpecifiedFlowRate(const Model& model, double airFlowValue)
    : AirflowNetworkComponent(AirflowNetworkSpecifiedFlowRate::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>());

    bool ok = setAirFlowValue(airFlowValue);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkSpecifiedFlowRate::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkSpecifiedFlowRate};
  }

  std::vector<std::string> AirflowNetworkSpecifiedFlowRate::airFlowUnitsValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirflowNetworkSpecifiedFlowRateFields::AirFlowUnits);
  }

  std::vector<std::string> AirflowNetworkSpecifiedFlowRate::validAirFlowUnitsValues() {
    return airFlowUnitsValues();
  }

  double AirflowNetworkSpecifiedFlowRate::airFlowValue() const {
    return getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>()->airFlowValue();
  }

  std::string AirflowNetworkSpecifiedFlowRate::airFlowUnits() const {
    return getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>()->airFlowUnits();
  }

  bool AirflowNetworkSpecifiedFlowRate::isAirFlowUnitsDefaulted() const {
    return getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>()->isAirFlowUnitsDefaulted();
  }

  bool AirflowNetworkSpecifiedFlowRate::setAirFlowValue(double airFlowValue) {
    return getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>()->setAirFlowValue(airFlowValue);
  }

  bool AirflowNetworkSpecifiedFlowRate::setAirFlowUnits(const std::string& airFlowUnits) {
    return getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>()->setAirFlowUnits(airFlowUnits);
  }

  void AirflowNetworkSpecifiedFlowRate::resetAirFlowUnits() {
    return getImpl<detail::AirflowNetworkSpecifiedFlowRate_Impl>()->resetAirFlowUnits();
  }

  /// @cond
  AirflowNetworkSpecifiedFlowRate::AirflowNetworkSpecifiedFlowRate(std::shared_ptr<detail::AirflowNetworkSpecifiedFlowRate_Impl> impl)
    : AirflowNetworkComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
