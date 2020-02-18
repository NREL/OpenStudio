/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <vector>
#include <string>
#include "OutputTableSummaryReports.hpp"
#include "OutputTableSummaryReports_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Table_SummaryReports_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  OutputTableSummaryReports_Impl::OutputTableSummaryReports_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutputTableSummaryReports::iddObjectType());
  }

  OutputTableSummaryReports_Impl::OutputTableSummaryReports_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutputTableSummaryReports::iddObjectType());
  }

  OutputTableSummaryReports_Impl::OutputTableSummaryReports_Impl(const OutputTableSummaryReports_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> OutputTableSummaryReports_Impl::parent() const {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& OutputTableSummaryReports_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType OutputTableSummaryReports_Impl::iddObjectType() const {
    return OutputTableSummaryReports::iddObjectType();
  }

  unsigned int OutputTableSummaryReports_Impl::numberofSummaryReports() const {
    return numExtensibleGroups();
  }

  bool OutputTableSummaryReports_Impl::addSummaryReport(std::string summaryReport) {
    // Check if summaryReport already exists
    boost::optional<unsigned> _existingIndex = summaryReportIndex(summaryReport);
    if (_existingIndex) {
      boost::optional<std::string> _summaryReport = getSummaryReport(_existingIndex.get());
      OS_ASSERT(_summaryReport);
      LOG(Warn, "For " << briefDescription() << ", SummaryReport already exists.");
      return true;
    }

    bool result;
    // Push an extensible group
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    bool result = eg.setString(OS_Output_Table_SummaryReportsExtensibleFields::ReportName, summaryReport);
    if (result) {
      return true;
    } else {
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
  
  void OutputTableSummaryReports_Impl::removeAllSummaryReports() {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  std::vector<std::string> OutputTableSummaryReports_Impl::summaryReports() const {
    std::vector<std::string> result;
    
    std::vector<IdfExtensibleGroup> groups = extensibleGroups();
    
    for (const auto & group : groups) {
      boost::optional<std::string> summaryReport = group.cast<WorkspaceExtensibleGroup>().getDouble(OS_Output_Table_SummaryReportsExtensibleFields::ReportName);
      
      if (summaryReport) {
        result.push_back(summaryReport.get());
      }
    }
    
    return result;
  }

  bool OutputTableSummaryReports_Impl::addSummaryReports(std::vector<std::string> &summaryReports) {
    unsigned int num = numberofSummaryReports();
    for (std::string summaryReport : summaryReports) {
      addSummaryReport(summaryReport);
    }
    return true;    
  }

  boost::optional<unsigned> OutputTableSummaryReports_Impl::summaryReportIndex(std::string summaryReport) const {
    boost::optional<unsigned> result;

    auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
    auto it = std::find_if(egs.begin(), egs.end(),
      [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_Output_Table_SummaryReportsExtensibleFields::ReportName).get() == summaryReport);
      });

    // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
    if (it != egs.end()) {
      result = std::distance(egs.begin(), it);
    }

    return result;
  }

  boost::optional<std::string> OutputTableSummaryReports_Impl::getSummaryReport(unsigned groupIndex) const {

    boost::optional<std::string> result;

    ModelExtensibleGroup group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();

    boost::optional<std::string> _summaryReport = eg.getField(OS_Output_Table_SummaryReportsExtensibleFields::ReportName);

    if (!_summaryReport) {
      LOG(Error, "Could not retrieve ReportName for extensible group " << group.groupIndex() << ".");
    }

    return _summaryReport.get();
  }

} // detail

IddObjectType OutputTableSummaryReports::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Output_Table_SummaryReports);
}

unsigned int OutputTableSummaryReports::numberofSummaryReports() const {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->numberofSummaryReports();
}

bool OutputTableSummaryReports::addSummaryReport(std::string summaryReport) {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->addSummaryReport(summaryReport);
}

void OutputTableSummaryReports::removeSummaryReport(int groupIndex) {
  getImpl<detail::OutputTableSummaryReports_Impl>()->removeSummaryReport(groupIndex);
}

void OutputTableSummaryReports::removeAllSummaryReports() {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->removeAllSummaryReports();
}

std::vector<CustomBlock> OutputTableSummaryReports::summaryReports() const {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->customBlocks();
}

bool OutputTableSummaryReports::addSummaryReport(std::vector<std::string> summaryReports) {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->addSummaryReports(summaryReports);
}

boost::optional<unsigned> OutputTableSummaryReports::summaryReportIndex(std::string summaryReport) const {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->summaryReportIndex(summaryReport);
}

boost::optional<std::string> OutputTableSummaryReports::getSummaryReport(unsigned groupIndex) const {
  return getImpl<detail::OutputTableSummaryReports_Impl>()->getSummaryReport(groupIndex);
}

/// @cond
OutputTableSummaryReports::OutputTableSummaryReports(std::shared_ptr<detail::OutputTableSummaryReports_Impl> impl)
  : ModelObject(std::move(impl))
{}

OutputTableSummaryReports::OutputTableSummaryReports(Model& model)
  : ModelObject(OutputTableSummaryReports::iddObjectType(), model)
{}

/// @endcond

} // model
} // openstudio