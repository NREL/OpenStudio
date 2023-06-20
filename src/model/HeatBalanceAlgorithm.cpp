/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HeatBalanceAlgorithm.hpp"
#include "HeatBalanceAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    HeatBalanceAlgorithm_Impl::HeatBalanceAlgorithm_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatBalanceAlgorithm::iddObjectType());
    }

    HeatBalanceAlgorithm_Impl::HeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatBalanceAlgorithm::iddObjectType());
    }

    HeatBalanceAlgorithm_Impl::HeatBalanceAlgorithm_Impl(const HeatBalanceAlgorithm_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> HeatBalanceAlgorithm_Impl::parent() const {
      boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
      return result;
    }

    // set the parent, child may have to call methods on the parent
    bool HeatBalanceAlgorithm_Impl::setParent(ParentObject& newParent) {
      if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
        return true;
      }
      return false;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& HeatBalanceAlgorithm_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType HeatBalanceAlgorithm_Impl::iddObjectType() const {
      return HeatBalanceAlgorithm::iddObjectType();
    }

    std::string HeatBalanceAlgorithm_Impl::algorithm() const {
      boost::optional<std::string> value = getString(OS_HeatBalanceAlgorithmFields::Algorithm, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatBalanceAlgorithm_Impl::isAlgorithmDefaulted() const {
      return isEmpty(OS_HeatBalanceAlgorithmFields::Algorithm);
    }

    double HeatBalanceAlgorithm_Impl::surfaceTemperatureUpperLimit() const {
      boost::optional<double> value = getDouble(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatBalanceAlgorithm_Impl::isSurfaceTemperatureUpperLimitDefaulted() const {
      return isEmpty(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit);
    }

    double HeatBalanceAlgorithm_Impl::minimumSurfaceConvectionHeatTransferCoefficientValue() const {
      boost::optional<double> value = getDouble(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatBalanceAlgorithm_Impl::isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
      return isEmpty(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue);
    }

    double HeatBalanceAlgorithm_Impl::maximumSurfaceConvectionHeatTransferCoefficientValue() const {
      boost::optional<double> value = getDouble(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatBalanceAlgorithm_Impl::isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
      return isEmpty(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue);
    }

    bool HeatBalanceAlgorithm_Impl::setAlgorithm(const std::string& algorithm) {
      bool result = setString(OS_HeatBalanceAlgorithmFields::Algorithm, algorithm);
      return result;
    }

    void HeatBalanceAlgorithm_Impl::resetAlgorithm() {
      bool result = setString(OS_HeatBalanceAlgorithmFields::Algorithm, "");
      OS_ASSERT(result);
    }

    bool HeatBalanceAlgorithm_Impl::setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit) {
      bool result = setDouble(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, surfaceTemperatureUpperLimit);
      return result;
    }

    void HeatBalanceAlgorithm_Impl::resetSurfaceTemperatureUpperLimit() {
      bool result = setString(OS_HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, "");
      OS_ASSERT(result);
    }

    bool HeatBalanceAlgorithm_Impl::setMinimumSurfaceConvectionHeatTransferCoefficientValue(
      double minimumSurfaceConvectionHeatTransferCoefficientValue) {
      bool result = setDouble(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue,
                              minimumSurfaceConvectionHeatTransferCoefficientValue);
      return result;
    }

    void HeatBalanceAlgorithm_Impl::resetMinimumSurfaceConvectionHeatTransferCoefficientValue() {
      bool result = setString(OS_HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue, "");
      OS_ASSERT(result);
    }

    bool HeatBalanceAlgorithm_Impl::setMaximumSurfaceConvectionHeatTransferCoefficientValue(
      double maximumSurfaceConvectionHeatTransferCoefficientValue) {
      bool result = setDouble(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue,
                              maximumSurfaceConvectionHeatTransferCoefficientValue);
      return result;
    }

    void HeatBalanceAlgorithm_Impl::resetMaximumSurfaceConvectionHeatTransferCoefficientValue() {
      bool result = setString(OS_HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> HeatBalanceAlgorithm_Impl::algorithmValues() const {
      return HeatBalanceAlgorithm::algorithmValues();
    }

  }  // namespace detail

  IddObjectType HeatBalanceAlgorithm::iddObjectType() {
    return {IddObjectType::OS_HeatBalanceAlgorithm};
  }

  std::vector<std::string> HeatBalanceAlgorithm::algorithmValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatBalanceAlgorithmFields::Algorithm);
  }

  std::string HeatBalanceAlgorithm::algorithm() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->algorithm();
  }

  bool HeatBalanceAlgorithm::isAlgorithmDefaulted() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isAlgorithmDefaulted();
  }

  double HeatBalanceAlgorithm::surfaceTemperatureUpperLimit() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->surfaceTemperatureUpperLimit();
  }

  bool HeatBalanceAlgorithm::isSurfaceTemperatureUpperLimitDefaulted() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isSurfaceTemperatureUpperLimitDefaulted();
  }

  double HeatBalanceAlgorithm::minimumSurfaceConvectionHeatTransferCoefficientValue() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->minimumSurfaceConvectionHeatTransferCoefficientValue();
  }

  bool HeatBalanceAlgorithm::isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted();
  }

  double HeatBalanceAlgorithm::maximumSurfaceConvectionHeatTransferCoefficientValue() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->maximumSurfaceConvectionHeatTransferCoefficientValue();
  }

  bool HeatBalanceAlgorithm::isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted();
  }

  bool HeatBalanceAlgorithm::setAlgorithm(const std::string& algorithm) {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setAlgorithm(algorithm);
  }

  void HeatBalanceAlgorithm::resetAlgorithm() {
    getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetAlgorithm();
  }

  bool HeatBalanceAlgorithm::setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit) {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setSurfaceTemperatureUpperLimit(surfaceTemperatureUpperLimit);
  }

  void HeatBalanceAlgorithm::resetSurfaceTemperatureUpperLimit() {
    getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetSurfaceTemperatureUpperLimit();
  }

  bool HeatBalanceAlgorithm::setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue) {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setMinimumSurfaceConvectionHeatTransferCoefficientValue(
      minimumSurfaceConvectionHeatTransferCoefficientValue);
  }

  void HeatBalanceAlgorithm::resetMinimumSurfaceConvectionHeatTransferCoefficientValue() {
    getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetMinimumSurfaceConvectionHeatTransferCoefficientValue();
  }

  bool HeatBalanceAlgorithm::setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue) {
    return getImpl<detail::HeatBalanceAlgorithm_Impl>()->setMaximumSurfaceConvectionHeatTransferCoefficientValue(
      maximumSurfaceConvectionHeatTransferCoefficientValue);
  }

  void HeatBalanceAlgorithm::resetMaximumSurfaceConvectionHeatTransferCoefficientValue() {
    getImpl<detail::HeatBalanceAlgorithm_Impl>()->resetMaximumSurfaceConvectionHeatTransferCoefficientValue();
  }

  /// @cond
  /// constructor
  HeatBalanceAlgorithm::HeatBalanceAlgorithm(std::shared_ptr<detail::HeatBalanceAlgorithm_Impl> impl) : ModelObject(std::move(impl)) {}

  /// constructor
  HeatBalanceAlgorithm::HeatBalanceAlgorithm(Model& model) : ModelObject(HeatBalanceAlgorithm::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatBalanceAlgorithm_Impl>());

    this->setAlgorithm("ConductionTransferFunction");

    this->setSurfaceTemperatureUpperLimit(200.0);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
