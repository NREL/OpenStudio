/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputControlFiles.hpp"

#include <utilities/idd/OutputControl_Files_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputControlFiles(OutputControlFiles& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::OutputControl_Files, modelObject);

    if (modelObject.outputCSV()) {
      idfObject.setString(OutputControl_FilesFields::OutputCSV, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputCSV, "No");
    }

    if (modelObject.outputMTR()) {
      idfObject.setString(OutputControl_FilesFields::OutputMTR, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputMTR, "No");
    }

    if (modelObject.outputESO()) {
      idfObject.setString(OutputControl_FilesFields::OutputESO, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputESO, "No");
    }

    if (modelObject.outputEIO()) {
      idfObject.setString(OutputControl_FilesFields::OutputEIO, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputEIO, "No");
    }

    if (modelObject.outputTabular()) {
      idfObject.setString(OutputControl_FilesFields::OutputTabular, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputTabular, "No");
    }

    if (modelObject.outputSQLite()) {
      idfObject.setString(OutputControl_FilesFields::OutputSQLite, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputSQLite, "No");
    }

    if (modelObject.outputJSON()) {
      idfObject.setString(OutputControl_FilesFields::OutputJSON, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputJSON, "No");
    }

    if (modelObject.outputAUDIT()) {
      idfObject.setString(OutputControl_FilesFields::OutputAUDIT, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputAUDIT, "No");
    }

    if (modelObject.outputSpaceSizing()) {
      idfObject.setString(OutputControl_FilesFields::OutputSpaceSizing, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputSpaceSizing, "No");
    }

    if (modelObject.outputZoneSizing()) {
      idfObject.setString(OutputControl_FilesFields::OutputZoneSizing, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputZoneSizing, "No");
    }

    if (modelObject.outputSystemSizing()) {
      idfObject.setString(OutputControl_FilesFields::OutputSystemSizing, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputSystemSizing, "No");
    }

    if (modelObject.outputDXF()) {
      idfObject.setString(OutputControl_FilesFields::OutputDXF, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputDXF, "No");
    }

    if (modelObject.outputBND()) {
      idfObject.setString(OutputControl_FilesFields::OutputBND, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputBND, "No");
    }

    if (modelObject.outputRDD()) {
      idfObject.setString(OutputControl_FilesFields::OutputRDD, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputRDD, "No");
    }

    if (modelObject.outputMDD()) {
      idfObject.setString(OutputControl_FilesFields::OutputMDD, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputMDD, "No");
    }

    if (modelObject.outputMTD()) {
      idfObject.setString(OutputControl_FilesFields::OutputMTD, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputMTD, "No");
    }

    idfObject.setString(OutputControl_FilesFields::OutputEND, "Yes");

    if (modelObject.outputSHD()) {
      idfObject.setString(OutputControl_FilesFields::OutputSHD, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputSHD, "No");
    }

    if (modelObject.outputDFS()) {
      idfObject.setString(OutputControl_FilesFields::OutputDFS, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputDFS, "No");
    }

    if (modelObject.outputGLHE()) {
      idfObject.setString(OutputControl_FilesFields::OutputGLHE, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputGLHE, "No");
    }

    if (modelObject.outputDelightIn()) {
      idfObject.setString(OutputControl_FilesFields::OutputDelightIn, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputDelightIn, "No");
    }

    if (modelObject.outputDelightELdmp()) {
      idfObject.setString(OutputControl_FilesFields::OutputDelightELdmp, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputDelightELdmp, "No");
    }

    if (modelObject.outputDelightDFdmp()) {
      idfObject.setString(OutputControl_FilesFields::OutputDelightDFdmp, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputDelightDFdmp, "No");
    }

    if (modelObject.outputEDD()) {
      idfObject.setString(OutputControl_FilesFields::OutputEDD, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputEDD, "No");
    }

    if (modelObject.outputDBG()) {
      idfObject.setString(OutputControl_FilesFields::OutputDBG, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputDBG, "No");
    }

    if (modelObject.outputPerfLog()) {
      idfObject.setString(OutputControl_FilesFields::OutputPerfLog, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputPerfLog, "No");
    }

    if (modelObject.outputSLN()) {
      idfObject.setString(OutputControl_FilesFields::OutputSLN, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputSLN, "No");
    }

    if (modelObject.outputSCI()) {
      idfObject.setString(OutputControl_FilesFields::OutputSCI, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputSCI, "No");
    }

    if (modelObject.outputWRL()) {
      idfObject.setString(OutputControl_FilesFields::OutputWRL, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputWRL, "No");
    }

    if (modelObject.outputScreen()) {
      idfObject.setString(OutputControl_FilesFields::OutputScreen, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputScreen, "No");
    }

    if (modelObject.outputExtShd()) {
      idfObject.setString(OutputControl_FilesFields::OutputExtShd, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputExtShd, "No");
    }

    if (modelObject.outputTarcog()) {
      idfObject.setString(OutputControl_FilesFields::OutputTarcog, "Yes");
    } else {
      idfObject.setString(OutputControl_FilesFields::OutputTarcog, "No");
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
