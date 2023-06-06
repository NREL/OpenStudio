/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputTableSummaryReports.hpp"
#include "../../model/OutputTableSummaryReports_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateOutputTableSummaryReports(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Output_Table_SummaryReports) {
      LOG(Error, "WorkspaceObject is not IddObjectType: OutputTableSummaryReports");
      return boost::none;
    }

    OutputTableSummaryReports outputTableSummaryReports = m_model.getUniqueModelObject<OutputTableSummaryReports>();

    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalString summaryReport = workspaceGroup.getString(Output_Table_SummaryReportsExtensibleFields::ReportName);

      // add the summary report
      outputTableSummaryReports.addSummaryReport(*summaryReport);
    }

    std::vector<std::string> summaryReports = outputTableSummaryReports.summaryReports();
    if (summaryReports.empty()) {
      return boost::none;
    }

    return outputTableSummaryReports;
  }

}  // namespace energyplus

}  // namespace openstudio
