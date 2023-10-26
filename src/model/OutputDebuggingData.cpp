/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputDebuggingData.hpp"
#include "OutputDebuggingData_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_DebuggingData_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputDebuggingData_Impl::OutputDebuggingData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputDebuggingData::iddObjectType());
    }

    OutputDebuggingData_Impl::OutputDebuggingData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputDebuggingData::iddObjectType());
    }

    OutputDebuggingData_Impl::OutputDebuggingData_Impl(const OutputDebuggingData_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputDebuggingData_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType OutputDebuggingData_Impl::iddObjectType() const {
      return OutputDebuggingData::iddObjectType();
    }

    bool OutputDebuggingData_Impl::reportDebuggingData() const {
      return getBooleanFieldValue(OS_Output_DebuggingDataFields::ReportDebuggingData);
    }

    bool OutputDebuggingData_Impl::reportDuringWarmup() const {
      return getBooleanFieldValue(OS_Output_DebuggingDataFields::ReportDuringWarmup);
    }

    bool OutputDebuggingData_Impl::setReportDebuggingData(bool reportDebuggingData) {
      return setBooleanFieldValue(OS_Output_DebuggingDataFields::ReportDebuggingData, reportDebuggingData);
    }

    bool OutputDebuggingData_Impl::setReportDuringWarmup(bool reportDuringWarmup) {
      return setBooleanFieldValue(OS_Output_DebuggingDataFields::ReportDuringWarmup, reportDuringWarmup);
    }

  }  // namespace detail

  IddObjectType OutputDebuggingData::iddObjectType() {
    return {IddObjectType::OS_Output_DebuggingData};
  }

  bool OutputDebuggingData::reportDebuggingData() const {
    return getImpl<detail::OutputDebuggingData_Impl>()->reportDebuggingData();
  }

  bool OutputDebuggingData::reportDuringWarmup() const {
    return getImpl<detail::OutputDebuggingData_Impl>()->reportDuringWarmup();
  }

  bool OutputDebuggingData::setReportDebuggingData(bool reportDebuggingData) {
    return getImpl<detail::OutputDebuggingData_Impl>()->setReportDebuggingData(reportDebuggingData);
  }

  bool OutputDebuggingData::setReportDuringWarmup(bool reportDuringWarmup) {
    return getImpl<detail::OutputDebuggingData_Impl>()->setReportDuringWarmup(reportDuringWarmup);
  }

  /// @cond
  OutputDebuggingData::OutputDebuggingData(std::shared_ptr<detail::OutputDebuggingData_Impl> impl) : ModelObject(impl) {}
  OutputDebuggingData::OutputDebuggingData(Model& model) : ModelObject(OutputDebuggingData::iddObjectType(), model) {
    setReportDebuggingData(false);
    setReportDuringWarmup(false);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
