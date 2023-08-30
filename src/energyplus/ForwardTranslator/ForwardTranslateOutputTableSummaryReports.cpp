/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputTableSummaryReports.hpp"
#include "../../model/OutputTableSummaryReports_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputTableSummaryReports(OutputTableSummaryReports& modelObject) {
    // If object has no report, don't bother translating
    std::vector<std::string> summaryReports = modelObject.summaryReports();
    if (summaryReports.empty()) {
      return boost::none;
    }

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_Table_SummaryReports, modelObject);

    for (const std::string& summaryReport : summaryReports) {
      auto eg = idfObject.pushExtensibleGroup();
      eg.setString(Output_Table_SummaryReportsExtensibleFields::ReportName, summaryReport);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
