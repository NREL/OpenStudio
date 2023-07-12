/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SizingParameters.hpp"
#include "SizingParameters_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"

#include <utilities/idd/OS_Sizing_Parameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SizingParameters_Impl::SizingParameters_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SizingParameters::iddObjectType());
    }

    SizingParameters_Impl::SizingParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SizingParameters::iddObjectType());
    }

    SizingParameters_Impl::SizingParameters_Impl(const SizingParameters_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SizingParameters_Impl::parent() const {
      boost::optional<SimulationControl> result = this->model().getOptionalUniqueModelObject<SimulationControl>();
      return boost::optional<ParentObject>(result);
    }

    const std::vector<std::string>& SizingParameters_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SizingParameters_Impl::iddObjectType() const {
      return SizingParameters::iddObjectType();
    }

    double SizingParameters_Impl::heatingSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Sizing_ParametersFields::HeatingSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingParameters_Impl::isHeatingSizingFactorDefaulted() const {
      return isEmpty(OS_Sizing_ParametersFields::HeatingSizingFactor);
    }

    double SizingParameters_Impl::coolingSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Sizing_ParametersFields::CoolingSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingParameters_Impl::isCoolingSizingFactorDefaulted() const {
      return isEmpty(OS_Sizing_ParametersFields::CoolingSizingFactor);
    }

    boost::optional<int> SizingParameters_Impl::timestepsinAveragingWindow() const {
      return getInt(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, true);
    }

    bool SizingParameters_Impl::setHeatingSizingFactor(double heatingSizingFactor) {
      bool result = setDouble(OS_Sizing_ParametersFields::HeatingSizingFactor, heatingSizingFactor);
      return result;
    }

    void SizingParameters_Impl::resetHeatingSizingFactor() {
      bool result = setString(OS_Sizing_ParametersFields::HeatingSizingFactor, "");
      OS_ASSERT(result);
    }

    bool SizingParameters_Impl::setCoolingSizingFactor(double coolingSizingFactor) {
      bool result = setDouble(OS_Sizing_ParametersFields::CoolingSizingFactor, coolingSizingFactor);
      return result;
    }

    void SizingParameters_Impl::resetCoolingSizingFactor() {
      bool result = setString(OS_Sizing_ParametersFields::CoolingSizingFactor, "");
      OS_ASSERT(result);
    }

    bool SizingParameters_Impl::setTimestepsinAveragingWindow(boost::optional<int> timestepsinAveragingWindow) {
      bool result(false);
      if (timestepsinAveragingWindow) {
        result = setInt(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, timestepsinAveragingWindow.get());
      } else {
        result = setString(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, "");
      }
      return result;
    }

    void SizingParameters_Impl::resetTimestepsinAveragingWindow() {
      bool result = setString(OS_Sizing_ParametersFields::TimestepsinAveragingWindow, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType SizingParameters::iddObjectType() {
    return {IddObjectType::OS_Sizing_Parameters};
  }

  double SizingParameters::heatingSizingFactor() const {
    return getImpl<detail::SizingParameters_Impl>()->heatingSizingFactor();
  }

  bool SizingParameters::isHeatingSizingFactorDefaulted() const {
    return getImpl<detail::SizingParameters_Impl>()->isHeatingSizingFactorDefaulted();
  }

  double SizingParameters::coolingSizingFactor() const {
    return getImpl<detail::SizingParameters_Impl>()->coolingSizingFactor();
  }

  bool SizingParameters::isCoolingSizingFactorDefaulted() const {
    return getImpl<detail::SizingParameters_Impl>()->isCoolingSizingFactorDefaulted();
  }

  boost::optional<int> SizingParameters::timestepsinAveragingWindow() const {
    return getImpl<detail::SizingParameters_Impl>()->timestepsinAveragingWindow();
  }

  bool SizingParameters::setHeatingSizingFactor(double heatingSizingFactor) {
    return getImpl<detail::SizingParameters_Impl>()->setHeatingSizingFactor(heatingSizingFactor);
  }

  void SizingParameters::resetHeatingSizingFactor() {
    getImpl<detail::SizingParameters_Impl>()->resetHeatingSizingFactor();
  }

  bool SizingParameters::setCoolingSizingFactor(double coolingSizingFactor) {
    return getImpl<detail::SizingParameters_Impl>()->setCoolingSizingFactor(coolingSizingFactor);
  }

  void SizingParameters::resetCoolingSizingFactor() {
    getImpl<detail::SizingParameters_Impl>()->resetCoolingSizingFactor();
  }

  bool SizingParameters::setTimestepsinAveragingWindow(int timestepsinAveragingWindow) {
    return getImpl<detail::SizingParameters_Impl>()->setTimestepsinAveragingWindow(timestepsinAveragingWindow);
  }

  void SizingParameters::resetTimestepsinAveragingWindow() {
    getImpl<detail::SizingParameters_Impl>()->resetTimestepsinAveragingWindow();
  }

  /// @cond
  SizingParameters::SizingParameters(std::shared_ptr<detail::SizingParameters_Impl> impl) : ModelObject(std::move(impl)) {}

  SizingParameters::SizingParameters(Model& model) : ModelObject(SizingParameters::iddObjectType(), model) {
    setHeatingSizingFactor(1.25);
    setCoolingSizingFactor(1.15);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
