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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputControlFiles.hpp"
#include "../../model/OutputControlFiles_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OutputControl_Files_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputControlFiles) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  OutputControlFiles outputControlFiles = m.getUniqueModelObject<OutputControlFiles>();

  // Check all cases where a single output request is True so we know we assigned the fields correctly
  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (size_t i = 0; i < 30; ++i) {
    bool status[30] = {false};
    status[i] = true;
    EXPECT_TRUE(outputControlFiles.setOutputCSV(status[0]));
    EXPECT_TRUE(outputControlFiles.setOutputMTR(status[1]));
    EXPECT_TRUE(outputControlFiles.setOutputESO(status[2]));
    EXPECT_TRUE(outputControlFiles.setOutputEIO(status[3]));
    EXPECT_TRUE(outputControlFiles.setOutputTabular(status[4]));
    EXPECT_TRUE(outputControlFiles.setOutputSQLite(status[5]));
    EXPECT_TRUE(outputControlFiles.setOutputJSON(status[6]));
    EXPECT_TRUE(outputControlFiles.setOutputAUDIT(status[7]));
    EXPECT_TRUE(outputControlFiles.setOutputZoneSizing(status[8]));
    EXPECT_TRUE(outputControlFiles.setOutputSystemSizing(status[9]));
    EXPECT_TRUE(outputControlFiles.setOutputDXF(status[10]));
    EXPECT_TRUE(outputControlFiles.setOutputBND(status[11]));
    EXPECT_TRUE(outputControlFiles.setOutputRDD(status[12]));
    EXPECT_TRUE(outputControlFiles.setOutputMDD(status[13]));
    EXPECT_TRUE(outputControlFiles.setOutputMTD(status[14]));
    EXPECT_TRUE(outputControlFiles.setOutputSHD(status[15]));
    EXPECT_TRUE(outputControlFiles.setOutputDFS(status[16]));
    EXPECT_TRUE(outputControlFiles.setOutputGLHE(status[17]));
    EXPECT_TRUE(outputControlFiles.setOutputDelightIn(status[18]));
    EXPECT_TRUE(outputControlFiles.setOutputDelightELdmp(status[19]));
    EXPECT_TRUE(outputControlFiles.setOutputDelightDFdmp(status[20]));
    EXPECT_TRUE(outputControlFiles.setOutputEDD(status[21]));
    EXPECT_TRUE(outputControlFiles.setOutputDBG(status[22]));
    EXPECT_TRUE(outputControlFiles.setOutputPerfLog(status[23]));
    EXPECT_TRUE(outputControlFiles.setOutputSLN(status[24]));
    EXPECT_TRUE(outputControlFiles.setOutputSCI(status[25]));
    EXPECT_TRUE(outputControlFiles.setOutputWRL(status[26]));
    EXPECT_TRUE(outputControlFiles.setOutputScreen(status[27]));
    EXPECT_TRUE(outputControlFiles.setOutputExtShd(status[28]));
    EXPECT_TRUE(outputControlFiles.setOutputTarcog(status[29]));

    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::OutputControl_Files);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_outputControlFiles(idfObjs[0]);

    EXPECT_EQ(boolToString(status[0]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputCSV).get());
    EXPECT_EQ(boolToString(status[1]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputMTR).get());
    EXPECT_EQ(boolToString(status[2]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputESO).get());
    EXPECT_EQ(boolToString(status[3]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputEIO).get());
    EXPECT_EQ(boolToString(status[4]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputTabular).get());
    EXPECT_EQ(boolToString(status[5]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSQLite).get());
    EXPECT_EQ(boolToString(status[6]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputJSON).get());
    EXPECT_EQ(boolToString(status[7]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputAUDIT).get());
    EXPECT_EQ(boolToString(status[8]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputZoneSizing).get());
    EXPECT_EQ(boolToString(status[9]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSystemSizing).get());
    EXPECT_EQ(boolToString(status[10]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDXF).get());
    EXPECT_EQ(boolToString(status[11]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputBND).get());
    EXPECT_EQ(boolToString(status[12]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputRDD).get());
    EXPECT_EQ(boolToString(status[13]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputMDD).get());
    EXPECT_EQ(boolToString(status[14]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputMTD).get());
    EXPECT_EQ(boolToString(true), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputEND).get());
    EXPECT_EQ(boolToString(status[15]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSHD).get());
    EXPECT_EQ(boolToString(status[16]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDFS).get());
    EXPECT_EQ(boolToString(status[17]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputGLHE).get());
    EXPECT_EQ(boolToString(status[18]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDelightIn).get());
    EXPECT_EQ(boolToString(status[19]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDelightELdmp).get());
    EXPECT_EQ(boolToString(status[20]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDelightDFdmp).get());
    EXPECT_EQ(boolToString(status[21]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputEDD).get());
    EXPECT_EQ(boolToString(status[22]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDBG).get());
    EXPECT_EQ(boolToString(status[23]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputPerfLog).get());
    EXPECT_EQ(boolToString(status[24]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSLN).get());
    EXPECT_EQ(boolToString(status[25]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSCI).get());
    EXPECT_EQ(boolToString(status[26]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputWRL).get());
    EXPECT_EQ(boolToString(status[27]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputScreen).get());
    EXPECT_EQ(boolToString(status[28]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputExtShd).get());
    EXPECT_EQ(boolToString(status[29]), idf_outputControlFiles.getString(OutputControl_FilesFields::OutputTarcog).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputControlFiles) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlFiles>());

  OptionalWorkspaceObject _i_outputControlFiles = w.addObject(IdfObject(IddObjectType::OutputControl_Files));
  ASSERT_TRUE(_i_outputControlFiles);

  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (size_t i = 0; i < 31; ++i) {
    bool status[31] = {false};
    status[i] = true;
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputCSV, boolToString(status[0])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputMTR, boolToString(status[1])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputESO, boolToString(status[2])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputEIO, boolToString(status[3])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputTabular, boolToString(status[4])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputSQLite, boolToString(status[5])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputJSON, boolToString(status[6])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputAUDIT, boolToString(status[7])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputZoneSizing, boolToString(status[8])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputSystemSizing, boolToString(status[9])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputDXF, boolToString(status[10])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputBND, boolToString(status[11])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputRDD, boolToString(status[12])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputMDD, boolToString(status[13])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputMTD, boolToString(status[14])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputEND, boolToString(status[15])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputSHD, boolToString(status[16])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputDFS, boolToString(status[17])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputGLHE, boolToString(status[18])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputDelightIn, boolToString(status[19])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputDelightELdmp, boolToString(status[20])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputDelightDFdmp, boolToString(status[21])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputEDD, boolToString(status[22])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputDBG, boolToString(status[23])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputPerfLog, boolToString(status[24])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputSLN, boolToString(status[25])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputSCI, boolToString(status[26])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputWRL, boolToString(status[27])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputScreen, boolToString(status[28])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputExtShd, boolToString(status[29])));
    EXPECT_TRUE(_i_outputControlFiles->setString(OutputControl_FilesFields::OutputTarcog, boolToString(status[30])));

    Model m = rt.translateWorkspace(w);

    // // Get the unique object
    OutputControlFiles outputControlFiles = m.getUniqueModelObject<OutputControlFiles>();

    EXPECT_EQ(status[0], outputControlFiles.outputCSV());
    EXPECT_EQ(status[1], outputControlFiles.outputMTR());
    EXPECT_EQ(status[2], outputControlFiles.outputESO());
    EXPECT_EQ(status[3], outputControlFiles.outputEIO());
    EXPECT_EQ(status[4], outputControlFiles.outputTabular());
    EXPECT_EQ(status[5], outputControlFiles.outputSQLite());
    EXPECT_EQ(status[6], outputControlFiles.outputJSON());
    EXPECT_EQ(status[7], outputControlFiles.outputAUDIT());
    EXPECT_EQ(status[8], outputControlFiles.outputZoneSizing());
    EXPECT_EQ(status[9], outputControlFiles.outputSystemSizing());
    EXPECT_EQ(status[10], outputControlFiles.outputDXF());
    EXPECT_EQ(status[11], outputControlFiles.outputBND());
    EXPECT_EQ(status[12], outputControlFiles.outputRDD());
    EXPECT_EQ(status[13], outputControlFiles.outputMDD());
    EXPECT_EQ(status[14], outputControlFiles.outputMTD());
    // EXPECT_EQ(true, outputControlFiles.outputEND()); // not reverse translated
    EXPECT_EQ(status[16], outputControlFiles.outputSHD());
    EXPECT_EQ(status[17], outputControlFiles.outputDFS());
    EXPECT_EQ(status[18], outputControlFiles.outputGLHE());
    EXPECT_EQ(status[19], outputControlFiles.outputDelightIn());
    EXPECT_EQ(status[20], outputControlFiles.outputDelightELdmp());
    EXPECT_EQ(status[21], outputControlFiles.outputDelightDFdmp());
    EXPECT_EQ(status[22], outputControlFiles.outputEDD());
    EXPECT_EQ(status[23], outputControlFiles.outputDBG());
    EXPECT_EQ(status[24], outputControlFiles.outputPerfLog());
    EXPECT_EQ(status[25], outputControlFiles.outputSLN());
    EXPECT_EQ(status[26], outputControlFiles.outputSCI());
    EXPECT_EQ(status[27], outputControlFiles.outputWRL());
    EXPECT_EQ(status[28], outputControlFiles.outputScreen());
    EXPECT_EQ(status[29], outputControlFiles.outputExtShd());
    EXPECT_EQ(status[30], outputControlFiles.outputTarcog());
  }
}
