/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <vector>
#include <string>
#include "OutputTableSummaryReports.hpp"
#include "OutputTableSummaryReports_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Table_SummaryReports_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include <algorithm>

namespace openstudio {
namespace model {

  namespace detail {

    OutputTableSummaryReports_Impl::OutputTableSummaryReports_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputTableSummaryReports::iddObjectType());
    }

    OutputTableSummaryReports_Impl::OutputTableSummaryReports_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputTableSummaryReports::iddObjectType());
    }

    OutputTableSummaryReports_Impl::OutputTableSummaryReports_Impl(const OutputTableSummaryReports_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputTableSummaryReports_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputTableSummaryReports_Impl::iddObjectType() const {
      return OutputTableSummaryReports::iddObjectType();
    }

    unsigned int OutputTableSummaryReports_Impl::numberofSummaryReports() const {
      return numExtensibleGroups();
    }

    bool OutputTableSummaryReports_Impl::addSummaryReport(const std::string& summaryReport) {
      // Check if summaryReport already exists
      boost::optional<unsigned> _existingIndex = summaryReportIndex(summaryReport);
      if (_existingIndex) {
        boost::optional<std::string> _summaryReport = getSummaryReport(_existingIndex.get());
        OS_ASSERT(_summaryReport);
        LOG(Warn, "For " << briefDescription() << ", summary report '" << summaryReport << "' already exists.");
        return false;
      }

      // Push an extensible group
      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool result = eg.setString(OS_Output_Table_SummaryReportsExtensibleFields::ReportName, summaryReport);
      if (result) {
        return true;
      } else {
        // Something went wrong, probably the summary report name isn't in the list of possible choices
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        return false;
      }
    }

    bool OutputTableSummaryReports_Impl::removeSummaryReport(unsigned groupIndex) {
      bool result;

      unsigned int num = numberofSummaryReports();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      } else {
        result = false;
      }
      return result;
    }

    bool OutputTableSummaryReports_Impl::enableAllSummaryReport() {
      return addSummaryReport("AllSummary");
    }

    void OutputTableSummaryReports_Impl::removeAllSummaryReports() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    std::vector<std::string> OutputTableSummaryReports_Impl::summaryReports() const {
      std::vector<std::string> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<std::string> summaryReport =
          group.cast<WorkspaceExtensibleGroup>().getString(OS_Output_Table_SummaryReportsExtensibleFields::ReportName);

        if (summaryReport) {
          result.push_back(summaryReport.get());
        }
      }

      return result;
    }

    bool OutputTableSummaryReports_Impl::addSummaryReports(const std::vector<std::string>& summaryReports) {
      bool result = true;
      for (const std::string& summaryReport : summaryReports) {
        result &= addSummaryReport(summaryReport);
      }
      return result;
    }

    boost::optional<unsigned> OutputTableSummaryReports_Impl::summaryReportIndex(const std::string& summaryReport) const {
      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return openstudio::istringEqual(summaryReport, eg.getField(OS_Output_Table_SummaryReportsExtensibleFields::ReportName).get());
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it);
      }

      return result;
    }

    boost::optional<std::string> OutputTableSummaryReports_Impl::getSummaryReport(unsigned groupIndex) const {

      boost::optional<std::string> result;

      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();

      boost::optional<std::string> _summaryReport = group.getString(OS_Output_Table_SummaryReportsExtensibleFields::ReportName);

      if (!_summaryReport) {
        LOG(Error, "Could not retrieve ReportName for extensible group " << group.groupIndex() << ".");
      }

      return _summaryReport.get();
    }

  }  // namespace detail

  IddObjectType OutputTableSummaryReports::iddObjectType() {
    return {IddObjectType::OS_Output_Table_SummaryReports};
  }

  std::vector<std::string> OutputTableSummaryReports::summaryReportValues() {
    IddObject obj = IddFactory::instance().getObject(iddObjectType()).get();
    // Return IddKeyNames in extensible portion
    return getIddKeyNames(obj, obj.numFields() + OS_Output_Table_SummaryReportsExtensibleFields::ReportName);
  }

  std::vector<std::string> OutputTableSummaryReports::validSummaryReportValues() {
    return summaryReportValues();
  }

  unsigned int OutputTableSummaryReports::numberofSummaryReports() const {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->numberofSummaryReports();
  }

  bool OutputTableSummaryReports::addSummaryReport(const std::string& summaryReport) {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->addSummaryReport(summaryReport);
  }

  bool OutputTableSummaryReports::removeSummaryReport(int groupIndex) {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->removeSummaryReport(groupIndex);
  }

  void OutputTableSummaryReports::removeAllSummaryReports() {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->removeAllSummaryReports();
  }

  std::vector<std::string> OutputTableSummaryReports::summaryReports() const {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->summaryReports();
  }

  bool OutputTableSummaryReports::addSummaryReports(const std::vector<std::string>& summaryReports) {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->addSummaryReports(summaryReports);
  }

  boost::optional<unsigned> OutputTableSummaryReports::summaryReportIndex(const std::string& summaryReport) const {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->summaryReportIndex(summaryReport);
  }

  boost::optional<std::string> OutputTableSummaryReports::getSummaryReport(unsigned groupIndex) const {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->getSummaryReport(groupIndex);
  }

  bool OutputTableSummaryReports::enableAllSummaryReport() {
    return getImpl<detail::OutputTableSummaryReports_Impl>()->enableAllSummaryReport();
  }

  /// @cond
  OutputTableSummaryReports::OutputTableSummaryReports(std::shared_ptr<detail::OutputTableSummaryReports_Impl> impl) : ModelObject(std::move(impl)) {}

  OutputTableSummaryReports::OutputTableSummaryReports(Model& model) : ModelObject(OutputTableSummaryReports::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
