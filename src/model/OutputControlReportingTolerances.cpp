/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputControlReportingTolerances.hpp"
#include "OutputControlReportingTolerances_Impl.hpp"

#include <utilities/idd/OS_OutputControl_ReportingTolerances_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputControlReportingTolerances::iddObjectType());
    }

    OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputControlReportingTolerances::iddObjectType());
    }

    OutputControlReportingTolerances_Impl::OutputControlReportingTolerances_Impl(const OutputControlReportingTolerances_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputControlReportingTolerances_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputControlReportingTolerances_Impl::iddObjectType() const {
      return OutputControlReportingTolerances::iddObjectType();
    }

    double OutputControlReportingTolerances_Impl::toleranceforTimeHeatingSetpointNotMet() const {
      boost::optional<double> value = getDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputControlReportingTolerances_Impl::isToleranceforTimeHeatingSetpointNotMetDefaulted() const {
      return isEmpty(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet);
    }

    double OutputControlReportingTolerances_Impl::toleranceforTimeCoolingSetpointNotMet() const {
      boost::optional<double> value = getDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputControlReportingTolerances_Impl::isToleranceforTimeCoolingSetpointNotMetDefaulted() const {
      return isEmpty(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet);
    }

    bool OutputControlReportingTolerances_Impl::setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet) {
      bool result =
        setDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, toleranceforTimeHeatingSetpointNotMet);
      return result;
    }

    void OutputControlReportingTolerances_Impl::resetToleranceforTimeHeatingSetpointNotMet() {
      bool result = setString(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, "");
      OS_ASSERT(result);
    }

    bool OutputControlReportingTolerances_Impl::setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet) {
      bool result =
        setDouble(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, toleranceforTimeCoolingSetpointNotMet);
      return result;
    }

    void OutputControlReportingTolerances_Impl::resetToleranceforTimeCoolingSetpointNotMet() {
      bool result = setString(OS_OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  IddObjectType OutputControlReportingTolerances::iddObjectType() {
    return {IddObjectType::OS_OutputControl_ReportingTolerances};
  }

  double OutputControlReportingTolerances::toleranceforTimeHeatingSetpointNotMet() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->toleranceforTimeHeatingSetpointNotMet();
  }

  bool OutputControlReportingTolerances::isToleranceforTimeHeatingSetpointNotMetDefaulted() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->isToleranceforTimeHeatingSetpointNotMetDefaulted();
  }

  double OutputControlReportingTolerances::toleranceforTimeCoolingSetpointNotMet() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->toleranceforTimeCoolingSetpointNotMet();
  }

  bool OutputControlReportingTolerances::isToleranceforTimeCoolingSetpointNotMetDefaulted() const {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->isToleranceforTimeCoolingSetpointNotMetDefaulted();
  }

  bool OutputControlReportingTolerances::setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet) {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeHeatingSetpointNotMet(toleranceforTimeHeatingSetpointNotMet);
  }

  void OutputControlReportingTolerances::resetToleranceforTimeHeatingSetpointNotMet() {
    getImpl<detail::OutputControlReportingTolerances_Impl>()->resetToleranceforTimeHeatingSetpointNotMet();
  }

  bool OutputControlReportingTolerances::setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet) {
    return getImpl<detail::OutputControlReportingTolerances_Impl>()->setToleranceforTimeCoolingSetpointNotMet(toleranceforTimeCoolingSetpointNotMet);
  }

  void OutputControlReportingTolerances::resetToleranceforTimeCoolingSetpointNotMet() {
    getImpl<detail::OutputControlReportingTolerances_Impl>()->resetToleranceforTimeCoolingSetpointNotMet();
  }

  /// @cond
  OutputControlReportingTolerances::OutputControlReportingTolerances(std::shared_ptr<detail::OutputControlReportingTolerances_Impl> impl)
    : ModelObject(std::move(impl)) {}
  OutputControlReportingTolerances::OutputControlReportingTolerances(Model& model)
    : ModelObject(OutputControlReportingTolerances::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
