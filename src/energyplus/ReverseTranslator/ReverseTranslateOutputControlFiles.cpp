/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputControlFiles.hpp"
#include "../../model/OutputControlFiles_Impl.hpp"

#include <utilities/idd/OutputControl_Files_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputControlFiles(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // This is a Unique ModelObject
    openstudio::model::OutputControlFiles modelObject = m_model.getUniqueModelObject<OutputControlFiles>();

    if (boost::optional<std::string> _outputCSV = workspaceObject.getString(OutputControl_FilesFields::OutputCSV, true)) {
      if (istringEqual("Yes", _outputCSV.get())) {
        modelObject.setOutputCSV(true);
      } else {
        modelObject.setOutputCSV(false);
      }
    }

    if (boost::optional<std::string> _outputMTR = workspaceObject.getString(OutputControl_FilesFields::OutputMTR, true)) {
      if (istringEqual("Yes", _outputMTR.get())) {
        modelObject.setOutputMTR(true);
      } else {
        modelObject.setOutputMTR(false);
      }
    }

    if (boost::optional<std::string> _outputESO = workspaceObject.getString(OutputControl_FilesFields::OutputESO, true)) {
      if (istringEqual("Yes", _outputESO.get())) {
        modelObject.setOutputESO(true);
      } else {
        modelObject.setOutputESO(false);
      }
    }

    if (boost::optional<std::string> _outputEIO = workspaceObject.getString(OutputControl_FilesFields::OutputEIO, true)) {
      if (istringEqual("Yes", _outputEIO.get())) {
        modelObject.setOutputEIO(true);
      } else {
        modelObject.setOutputEIO(false);
      }
    }

    if (boost::optional<std::string> _outputTabular = workspaceObject.getString(OutputControl_FilesFields::OutputTabular, true)) {
      if (istringEqual("Yes", _outputTabular.get())) {
        modelObject.setOutputTabular(true);
      } else {
        modelObject.setOutputTabular(false);
      }
    }

    if (boost::optional<std::string> _outputSQLite = workspaceObject.getString(OutputControl_FilesFields::OutputSQLite, true)) {
      if (istringEqual("Yes", _outputSQLite.get())) {
        modelObject.setOutputSQLite(true);
      } else {
        modelObject.setOutputSQLite(false);
      }
    }

    if (boost::optional<std::string> _outputJSON = workspaceObject.getString(OutputControl_FilesFields::OutputJSON, true)) {
      if (istringEqual("Yes", _outputJSON.get())) {
        modelObject.setOutputJSON(true);
      } else {
        modelObject.setOutputJSON(false);
      }
    }

    if (boost::optional<std::string> _outputAUDIT = workspaceObject.getString(OutputControl_FilesFields::OutputAUDIT, true)) {
      if (istringEqual("Yes", _outputAUDIT.get())) {
        modelObject.setOutputAUDIT(true);
      } else {
        modelObject.setOutputAUDIT(false);
      }
    }

    if (boost::optional<std::string> _outputSpaceSizing = workspaceObject.getString(OutputControl_FilesFields::OutputSpaceSizing, true)) {
      if (istringEqual("Yes", _outputSpaceSizing.get())) {
        modelObject.setOutputSpaceSizing(true);
      } else {
        modelObject.setOutputSpaceSizing(false);
      }
    }

    if (boost::optional<std::string> _outputZoneSizing = workspaceObject.getString(OutputControl_FilesFields::OutputZoneSizing, true)) {
      if (istringEqual("Yes", _outputZoneSizing.get())) {
        modelObject.setOutputZoneSizing(true);
      } else {
        modelObject.setOutputZoneSizing(false);
      }
    }

    if (boost::optional<std::string> _outputSystemSizing = workspaceObject.getString(OutputControl_FilesFields::OutputSystemSizing, true)) {
      if (istringEqual("Yes", _outputSystemSizing.get())) {
        modelObject.setOutputSystemSizing(true);
      } else {
        modelObject.setOutputSystemSizing(false);
      }
    }

    if (boost::optional<std::string> _outputDXF = workspaceObject.getString(OutputControl_FilesFields::OutputDXF, true)) {
      if (istringEqual("Yes", _outputDXF.get())) {
        modelObject.setOutputDXF(true);
      } else {
        modelObject.setOutputDXF(false);
      }
    }

    if (boost::optional<std::string> _outputBND = workspaceObject.getString(OutputControl_FilesFields::OutputBND, true)) {
      if (istringEqual("Yes", _outputBND.get())) {
        modelObject.setOutputBND(true);
      } else {
        modelObject.setOutputBND(false);
      }
    }

    if (boost::optional<std::string> _outputRDD = workspaceObject.getString(OutputControl_FilesFields::OutputRDD, true)) {
      if (istringEqual("Yes", _outputRDD.get())) {
        modelObject.setOutputRDD(true);
      } else {
        modelObject.setOutputRDD(false);
      }
    }

    if (boost::optional<std::string> _outputMDD = workspaceObject.getString(OutputControl_FilesFields::OutputMDD, true)) {
      if (istringEqual("Yes", _outputMDD.get())) {
        modelObject.setOutputMDD(true);
      } else {
        modelObject.setOutputMDD(false);
      }
    }

    if (boost::optional<std::string> _outputMTD = workspaceObject.getString(OutputControl_FilesFields::OutputMTD, true)) {
      if (istringEqual("Yes", _outputMTD.get())) {
        modelObject.setOutputMTD(true);
      } else {
        modelObject.setOutputMTD(false);
      }
    }

    if (boost::optional<std::string> _outputSHD = workspaceObject.getString(OutputControl_FilesFields::OutputSHD, true)) {
      if (istringEqual("Yes", _outputSHD.get())) {
        modelObject.setOutputSHD(true);
      } else {
        modelObject.setOutputSHD(false);
      }
    }

    if (boost::optional<std::string> _outputDFS = workspaceObject.getString(OutputControl_FilesFields::OutputDFS, true)) {
      if (istringEqual("Yes", _outputDFS.get())) {
        modelObject.setOutputDFS(true);
      } else {
        modelObject.setOutputDFS(false);
      }
    }

    if (boost::optional<std::string> _outputGLHE = workspaceObject.getString(OutputControl_FilesFields::OutputGLHE, true)) {
      if (istringEqual("Yes", _outputGLHE.get())) {
        modelObject.setOutputGLHE(true);
      } else {
        modelObject.setOutputGLHE(false);
      }
    }

    if (boost::optional<std::string> _outputDelightIn = workspaceObject.getString(OutputControl_FilesFields::OutputDelightIn, true)) {
      if (istringEqual("Yes", _outputDelightIn.get())) {
        modelObject.setOutputDelightIn(true);
      } else {
        modelObject.setOutputDelightIn(false);
      }
    }

    if (boost::optional<std::string> _outputDelightELdmp = workspaceObject.getString(OutputControl_FilesFields::OutputDelightELdmp, true)) {
      if (istringEqual("Yes", _outputDelightELdmp.get())) {
        modelObject.setOutputDelightELdmp(true);
      } else {
        modelObject.setOutputDelightELdmp(false);
      }
    }

    if (boost::optional<std::string> _outputDelightDFdmp = workspaceObject.getString(OutputControl_FilesFields::OutputDelightDFdmp, true)) {
      if (istringEqual("Yes", _outputDelightDFdmp.get())) {
        modelObject.setOutputDelightDFdmp(true);
      } else {
        modelObject.setOutputDelightDFdmp(false);
      }
    }

    if (boost::optional<std::string> _outputEDD = workspaceObject.getString(OutputControl_FilesFields::OutputEDD, true)) {
      if (istringEqual("Yes", _outputEDD.get())) {
        modelObject.setOutputEDD(true);
      } else {
        modelObject.setOutputEDD(false);
      }
    }

    if (boost::optional<std::string> _outputDBG = workspaceObject.getString(OutputControl_FilesFields::OutputDBG, true)) {
      if (istringEqual("Yes", _outputDBG.get())) {
        modelObject.setOutputDBG(true);
      } else {
        modelObject.setOutputDBG(false);
      }
    }

    if (boost::optional<std::string> _outputPerfLog = workspaceObject.getString(OutputControl_FilesFields::OutputPerfLog, true)) {
      if (istringEqual("Yes", _outputPerfLog.get())) {
        modelObject.setOutputPerfLog(true);
      } else {
        modelObject.setOutputPerfLog(false);
      }
    }

    if (boost::optional<std::string> _outputSLN = workspaceObject.getString(OutputControl_FilesFields::OutputSLN, true)) {
      if (istringEqual("Yes", _outputSLN.get())) {
        modelObject.setOutputSLN(true);
      } else {
        modelObject.setOutputSLN(false);
      }
    }

    if (boost::optional<std::string> _outputSCI = workspaceObject.getString(OutputControl_FilesFields::OutputSCI, true)) {
      if (istringEqual("Yes", _outputSCI.get())) {
        modelObject.setOutputSCI(true);
      } else {
        modelObject.setOutputSCI(false);
      }
    }

    if (boost::optional<std::string> _outputWRL = workspaceObject.getString(OutputControl_FilesFields::OutputWRL, true)) {
      if (istringEqual("Yes", _outputWRL.get())) {
        modelObject.setOutputWRL(true);
      } else {
        modelObject.setOutputWRL(false);
      }
    }

    if (boost::optional<std::string> _outputScreen = workspaceObject.getString(OutputControl_FilesFields::OutputScreen, true)) {
      if (istringEqual("Yes", _outputScreen.get())) {
        modelObject.setOutputScreen(true);
      } else {
        modelObject.setOutputScreen(false);
      }
    }

    if (boost::optional<std::string> _outputExtShd = workspaceObject.getString(OutputControl_FilesFields::OutputExtShd, true)) {
      if (istringEqual("Yes", _outputExtShd.get())) {
        modelObject.setOutputExtShd(true);
      } else {
        modelObject.setOutputExtShd(false);
      }
    }

    if (boost::optional<std::string> _outputTarcog = workspaceObject.getString(OutputControl_FilesFields::OutputTarcog, true)) {
      if (istringEqual("Yes", _outputTarcog.get())) {
        modelObject.setOutputTarcog(true);
      } else {
        modelObject.setOutputTarcog(false);
      }
    }

    result = modelObject;
    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
