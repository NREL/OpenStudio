/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <vector>
#include <string>
#include "PerformancePrecisionTradeoffs.hpp"
#include "PerformancePrecisionTradeoffs_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PerformancePrecisionTradeoffs_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PerformancePrecisionTradeoffs_Impl::PerformancePrecisionTradeoffs_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PerformancePrecisionTradeoffs::iddObjectType());
    }

    PerformancePrecisionTradeoffs_Impl::PerformancePrecisionTradeoffs_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PerformancePrecisionTradeoffs::iddObjectType());
    }

    PerformancePrecisionTradeoffs_Impl::PerformancePrecisionTradeoffs_Impl(const PerformancePrecisionTradeoffs_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PerformancePrecisionTradeoffs_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PerformancePrecisionTradeoffs_Impl::iddObjectType() const {
      return PerformancePrecisionTradeoffs::iddObjectType();
    }

    bool PerformancePrecisionTradeoffs_Impl::useCoilDirectSolutions() const {
      boost::optional<std::string> value = getString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool PerformancePrecisionTradeoffs_Impl::isUseCoilDirectSolutionsDefaulted() const {
      return isEmpty(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions);
    }

    bool PerformancePrecisionTradeoffs_Impl::setUseCoilDirectSolutions(bool useCoilDirectSolutions) {
      bool result = false;
      if (useCoilDirectSolutions) {
        result = setString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "Yes");
      } else {
        result = setString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void PerformancePrecisionTradeoffs_Impl::resetUseCoilDirectSolutions() {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "");
      OS_ASSERT(result);
    }

    std::string PerformancePrecisionTradeoffs_Impl::zoneRadiantExchangeAlgorithm() const {
      boost::optional<std::string> value = getString(OS_PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PerformancePrecisionTradeoffs_Impl::isZoneRadiantExchangeAlgorithmDefaulted() const {
      return isEmpty(OS_PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm);
    }

    bool PerformancePrecisionTradeoffs_Impl::setZoneRadiantExchangeAlgorithm(const std::string& zoneRadiantExchangeAlgorithm) {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm, zoneRadiantExchangeAlgorithm);
      // OS_ASSERT(result);
      return result;
    }

    void PerformancePrecisionTradeoffs_Impl::resetZoneRadiantExchangeAlgorithm() {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm, "");
      OS_ASSERT(result);
    }

    std::string PerformancePrecisionTradeoffs_Impl::overrideMode() const {
      boost::optional<std::string> value = getString(OS_PerformancePrecisionTradeoffsFields::OverrideMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PerformancePrecisionTradeoffs_Impl::isOverrideModeDefaulted() const {
      return isEmpty(OS_PerformancePrecisionTradeoffsFields::OverrideMode);
    }

    bool PerformancePrecisionTradeoffs_Impl::setOverrideMode(const std::string& overrideMode) {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::OverrideMode, overrideMode);
      // OS_ASSERT(result);
      return result;
    }

    void PerformancePrecisionTradeoffs_Impl::resetOverrideMode() {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::OverrideMode, "");
      OS_ASSERT(result);
    }

    double PerformancePrecisionTradeoffs_Impl::maxZoneTempDiff() const {
      boost::optional<double> value = getDouble(OS_PerformancePrecisionTradeoffsFields::MaxZoneTempDiff, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PerformancePrecisionTradeoffs_Impl::isMaxZoneTempDiffDefaulted() const {
      return isEmpty(OS_PerformancePrecisionTradeoffsFields::MaxZoneTempDiff);
    }

    bool PerformancePrecisionTradeoffs_Impl::setMaxZoneTempDiff(double maxZoneTempDiff) {
      bool result = setDouble(OS_PerformancePrecisionTradeoffsFields::MaxZoneTempDiff, maxZoneTempDiff);
      // OS_ASSERT(result);
      return result;
    }

    void PerformancePrecisionTradeoffs_Impl::resetMaxZoneTempDiff() {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::MaxZoneTempDiff, "");
      OS_ASSERT(result);
    }

    double PerformancePrecisionTradeoffs_Impl::maxAllowedDelTemp() const {
      boost::optional<double> value = getDouble(OS_PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PerformancePrecisionTradeoffs_Impl::isMaxAllowedDelTempDefaulted() const {
      return isEmpty(OS_PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp);
    }

    bool PerformancePrecisionTradeoffs_Impl::setMaxAllowedDelTemp(double maxAllowedDelTemp) {
      bool result = setDouble(OS_PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp, maxAllowedDelTemp);
      // OS_ASSERT(result);
      return result;
    }

    void PerformancePrecisionTradeoffs_Impl::resetMaxAllowedDelTemp() {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp, "");
      OS_ASSERT(result);
    }

    bool PerformancePrecisionTradeoffs_Impl::useRepresentativeSurfacesforCalculations() const {
      boost::optional<std::string> value = getString(OS_PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool PerformancePrecisionTradeoffs_Impl::isUseRepresentativeSurfacesforCalculationsDefaulted() const {
      return isEmpty(OS_PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations);
    }

    bool PerformancePrecisionTradeoffs_Impl::setUseRepresentativeSurfacesforCalculations(bool useRepresentativeSurfacesforCalculations) {
      bool result = false;
      if (useRepresentativeSurfacesforCalculations) {
        result = setString(OS_PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations, "Yes");
      } else {
        result = setString(OS_PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void PerformancePrecisionTradeoffs_Impl::resetUseRepresentativeSurfacesforCalculations() {
      bool result = setString(OS_PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType PerformancePrecisionTradeoffs::iddObjectType() {
    return {IddObjectType::OS_PerformancePrecisionTradeoffs};
  }

  // Use Coil Direct Solutions
  bool PerformancePrecisionTradeoffs::useCoilDirectSolutions() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->useCoilDirectSolutions();
  }

  bool PerformancePrecisionTradeoffs::isUseCoilDirectSolutionsDefaulted() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isUseCoilDirectSolutionsDefaulted();
  }

  bool PerformancePrecisionTradeoffs::setUseCoilDirectSolutions(bool useCoilDirectSolutions) {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setUseCoilDirectSolutions(useCoilDirectSolutions);
  }

  void PerformancePrecisionTradeoffs::resetUseCoilDirectSolutions() {
    getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetUseCoilDirectSolutions();
  }

  // Zone Radiant Exchange Algorithm
  std::string PerformancePrecisionTradeoffs::zoneRadiantExchangeAlgorithm() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->zoneRadiantExchangeAlgorithm();
  }

  bool PerformancePrecisionTradeoffs::isZoneRadiantExchangeAlgorithmDefaulted() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isZoneRadiantExchangeAlgorithmDefaulted();
  }

  bool PerformancePrecisionTradeoffs::setZoneRadiantExchangeAlgorithm(const std::string& zoneRadiantExchangeAlgorithm) {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setZoneRadiantExchangeAlgorithm(zoneRadiantExchangeAlgorithm);
  }

  void PerformancePrecisionTradeoffs::resetZoneRadiantExchangeAlgorithm() {
    getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetZoneRadiantExchangeAlgorithm();
  }

  // Override Mode
  std::string PerformancePrecisionTradeoffs::overrideMode() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->overrideMode();
  }

  bool PerformancePrecisionTradeoffs::isOverrideModeDefaulted() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isOverrideModeDefaulted();
  }

  bool PerformancePrecisionTradeoffs::setOverrideMode(const std::string& overrideMode) {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setOverrideMode(overrideMode);
  }

  void PerformancePrecisionTradeoffs::resetOverrideMode() {
    getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetOverrideMode();
  }

  // MaxZoneTempDiff
  double PerformancePrecisionTradeoffs::maxZoneTempDiff() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->maxZoneTempDiff();
  }

  bool PerformancePrecisionTradeoffs::isMaxZoneTempDiffDefaulted() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isMaxZoneTempDiffDefaulted();
  }

  bool PerformancePrecisionTradeoffs::setMaxZoneTempDiff(double maxZoneTempDiff) {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setMaxZoneTempDiff(maxZoneTempDiff);
  }

  void PerformancePrecisionTradeoffs::resetMaxZoneTempDiff() {
    getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetMaxZoneTempDiff();
  }

  // MaxAllowedDelTemp
  double PerformancePrecisionTradeoffs::maxAllowedDelTemp() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->maxAllowedDelTemp();
  }

  bool PerformancePrecisionTradeoffs::isMaxAllowedDelTempDefaulted() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isMaxAllowedDelTempDefaulted();
  }

  bool PerformancePrecisionTradeoffs::setMaxAllowedDelTemp(double maxAllowedDelTemp) {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setMaxAllowedDelTemp(maxAllowedDelTemp);
  }

  void PerformancePrecisionTradeoffs::resetMaxAllowedDelTemp() {
    getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetMaxAllowedDelTemp();
  }

  // Use Representative Surfaces for Calculations
  bool PerformancePrecisionTradeoffs::useRepresentativeSurfacesforCalculations() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->useRepresentativeSurfacesforCalculations();
  }

  bool PerformancePrecisionTradeoffs::isUseRepresentativeSurfacesforCalculationsDefaulted() const {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->isUseRepresentativeSurfacesforCalculationsDefaulted();
  }

  bool PerformancePrecisionTradeoffs::setUseRepresentativeSurfacesforCalculations(bool useRepresentativeSurfacesforCalculations) {
    return getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->setUseRepresentativeSurfacesforCalculations(
      useRepresentativeSurfacesforCalculations);
  }

  void PerformancePrecisionTradeoffs::resetUseRepresentativeSurfacesforCalculations() {
    getImpl<detail::PerformancePrecisionTradeoffs_Impl>()->resetUseRepresentativeSurfacesforCalculations();
  }

  /// @cond
  PerformancePrecisionTradeoffs::PerformancePrecisionTradeoffs(std::shared_ptr<detail::PerformancePrecisionTradeoffs_Impl> impl)
    : ModelObject(std::move(impl)) {}

  PerformancePrecisionTradeoffs::PerformancePrecisionTradeoffs(Model& model) : ModelObject(PerformancePrecisionTradeoffs::iddObjectType(), model) {}

  /// @endcond

  std::vector<std::string> PerformancePrecisionTradeoffs::zoneRadiantExchangeAlgorithmValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm);
  }

  std::vector<std::string> PerformancePrecisionTradeoffs::validZoneRadiantExchangeAlgorithmValues() {
    return PerformancePrecisionTradeoffs::zoneRadiantExchangeAlgorithmValues();
  }

  std::vector<std::string> PerformancePrecisionTradeoffs::overrideModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PerformancePrecisionTradeoffsFields::OverrideMode);
  }

  std::vector<std::string> PerformancePrecisionTradeoffs::validOverrideModeValues() {
    return PerformancePrecisionTradeoffs::overrideModeValues();
  }

}  // namespace model
}  // namespace openstudio
