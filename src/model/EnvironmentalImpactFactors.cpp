/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnvironmentalImpactFactors.hpp"
#include "EnvironmentalImpactFactors_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnvironmentalImpactFactors_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnvironmentalImpactFactors_Impl::EnvironmentalImpactFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnvironmentalImpactFactors::iddObjectType());
    }

    EnvironmentalImpactFactors_Impl::EnvironmentalImpactFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnvironmentalImpactFactors::iddObjectType());
    }

    EnvironmentalImpactFactors_Impl::EnvironmentalImpactFactors_Impl(const EnvironmentalImpactFactors_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnvironmentalImpactFactors_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType EnvironmentalImpactFactors_Impl::iddObjectType() const {
      return EnvironmentalImpactFactors::iddObjectType();
    }

    double EnvironmentalImpactFactors_Impl::districtHeatingEfficiency() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::districtCoolingCOP() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::DistrictCoolingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::steamConversionEfficiency() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::SteamConversionEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::totalCarbonEquivalentEmissionFactorFromN2O() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::totalCarbonEquivalentEmissionFactorFromCH4() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::totalCarbonEquivalentEmissionFactorFromCO2() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool EnvironmentalImpactFactors_Impl::setDistrictHeatingEfficiency(double districtHeatingEfficiency) {
      bool result = setDouble(OS_EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, districtHeatingEfficiency);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setDistrictCoolingCOP(double districtCoolingCOP) {
      bool result = setDouble(OS_EnvironmentalImpactFactorsFields::DistrictCoolingCOP, districtCoolingCOP);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setSteamConversionEfficiency(double steamConversionEfficiency) {
      bool result = setDouble(OS_EnvironmentalImpactFactorsFields::SteamConversionEfficiency, steamConversionEfficiency);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setTotalCarbonEquivalentEmissionFactorFromN2O(double totalCarbonEquivalentEmissionFactorFromN2O) {
      bool result =
        setDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O, totalCarbonEquivalentEmissionFactorFromN2O);
      OS_ASSERT(result);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setTotalCarbonEquivalentEmissionFactorFromCH4(double totalCarbonEquivalentEmissionFactorFromCH4) {
      bool result =
        setDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4, totalCarbonEquivalentEmissionFactorFromCH4);
      OS_ASSERT(result);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setTotalCarbonEquivalentEmissionFactorFromCO2(double totalCarbonEquivalentEmissionFactorFromCO2) {
      bool result =
        setDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2, totalCarbonEquivalentEmissionFactorFromCO2);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  EnvironmentalImpactFactors::EnvironmentalImpactFactors(Model& model) : ModelObject(EnvironmentalImpactFactors::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnvironmentalImpactFactors_Impl>());

    // Defaults from E+ IDD, captured at v9.5.0
    setDistrictHeatingEfficiency(0.3);
    setDistrictCoolingCOP(3.0);
    setSteamConversionEfficiency(0.25);
    setTotalCarbonEquivalentEmissionFactorFromN2O(80.7272);
    setTotalCarbonEquivalentEmissionFactorFromCH4(6.2727);
    setTotalCarbonEquivalentEmissionFactorFromCO2(0.2727);
  }

  IddObjectType EnvironmentalImpactFactors::iddObjectType() {
    return {IddObjectType::OS_EnvironmentalImpactFactors};
  }

  double EnvironmentalImpactFactors::districtHeatingEfficiency() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->districtHeatingEfficiency();
  }

  double EnvironmentalImpactFactors::districtCoolingCOP() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->districtCoolingCOP();
  }

  double EnvironmentalImpactFactors::steamConversionEfficiency() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->steamConversionEfficiency();
  }

  double EnvironmentalImpactFactors::totalCarbonEquivalentEmissionFactorFromN2O() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->totalCarbonEquivalentEmissionFactorFromN2O();
  }

  double EnvironmentalImpactFactors::totalCarbonEquivalentEmissionFactorFromCH4() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->totalCarbonEquivalentEmissionFactorFromCH4();
  }

  double EnvironmentalImpactFactors::totalCarbonEquivalentEmissionFactorFromCO2() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->totalCarbonEquivalentEmissionFactorFromCO2();
  }

  bool EnvironmentalImpactFactors::setDistrictHeatingEfficiency(double districtHeatingEfficiency) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setDistrictHeatingEfficiency(districtHeatingEfficiency);
  }

  bool EnvironmentalImpactFactors::setDistrictCoolingCOP(double districtCoolingCOP) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setDistrictCoolingCOP(districtCoolingCOP);
  }

  bool EnvironmentalImpactFactors::setSteamConversionEfficiency(double steamConversionEfficiency) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setSteamConversionEfficiency(steamConversionEfficiency);
  }

  bool EnvironmentalImpactFactors::setTotalCarbonEquivalentEmissionFactorFromN2O(double totalCarbonEquivalentEmissionFactorFromN2O) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setTotalCarbonEquivalentEmissionFactorFromN2O(
      totalCarbonEquivalentEmissionFactorFromN2O);
  }

  bool EnvironmentalImpactFactors::setTotalCarbonEquivalentEmissionFactorFromCH4(double totalCarbonEquivalentEmissionFactorFromCH4) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setTotalCarbonEquivalentEmissionFactorFromCH4(
      totalCarbonEquivalentEmissionFactorFromCH4);
  }

  bool EnvironmentalImpactFactors::setTotalCarbonEquivalentEmissionFactorFromCO2(double totalCarbonEquivalentEmissionFactorFromCO2) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setTotalCarbonEquivalentEmissionFactorFromCO2(
      totalCarbonEquivalentEmissionFactorFromCO2);
  }

  /// @cond
  EnvironmentalImpactFactors::EnvironmentalImpactFactors(std::shared_ptr<detail::EnvironmentalImpactFactors_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
