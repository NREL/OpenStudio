/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputDebuggingData.hpp"
#include "../../model/OutputDebuggingData_Impl.hpp"

#include "../../utilities/math/FloatCompare.hpp"

#include <utilities/idd/Output_DebuggingData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputDebuggingData(const WorkspaceObject& wo) {
    boost::optional<ModelObject> result;

    // Check whether we have either field set to True before we bother instantiating the object...
    bool reportDebuggingData = false;
    bool reportDuringWarmup = false;

    // Note: E+ is wrong here: IDD uses numeric fields to store what is really a boolean. If 1 = true, else false.

    // Report Debugging Data: Optional Boolean
    if (boost::optional<std::string> _reportDebuggingData = wo.getString(Output_DebuggingDataFields::ReportDebuggingData, true)) {
      if (istringEqual("Yes", _reportDebuggingData.get())) {
        reportDebuggingData = true;
      }
    }

    // Report During Warmup: Optional Boolean
    if (boost::optional<std::string> _reportDuringWarmup = wo.getString(Output_DebuggingDataFields::ReportDuringWarmup, true)) {
      if (istringEqual("Yes", _reportDuringWarmup.get())) {
        reportDuringWarmup = true;
      }
    }

    if (reportDebuggingData || reportDuringWarmup) {
      // This is a Unique ModelObject
      auto modelObject = m_model.getUniqueModelObject<OutputDebuggingData>();
      modelObject.setReportDebuggingData(reportDebuggingData);
      modelObject.setReportDuringWarmup(reportDuringWarmup);
      result = modelObject;
    }

    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
