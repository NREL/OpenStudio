/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputDebuggingData.hpp"

#include <utilities/idd/Output_DebuggingData_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputDebuggingData(model::OutputDebuggingData& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_DebuggingData, modelObject);

    // Report Debugging Data
    if (modelObject.reportDebuggingData()) {
      idfObject.setString(Output_DebuggingDataFields::ReportDebuggingData, "Yes");
    } else {
      idfObject.setString(Output_DebuggingDataFields::ReportDebuggingData, "No");
    }

    // Report During Warmup
    if (modelObject.reportDuringWarmup()) {
      idfObject.setString(Output_DebuggingDataFields::ReportDuringWarmup, "Yes");
    } else {
      idfObject.setString(Output_DebuggingDataFields::ReportDuringWarmup, "No");
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
