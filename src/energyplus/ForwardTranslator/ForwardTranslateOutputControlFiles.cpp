/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
    IdfObject idfObject(openstudio::IddObjectType::OutputControl_Files);
    m_idfObjects.push_back(idfObject);

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
