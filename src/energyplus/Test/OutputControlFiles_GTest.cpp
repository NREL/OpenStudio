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

  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::OutputControl_Files);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idf_outputControlFiles(idfObjs[0]);

  EXPECT_EQ("No", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputCSV).get());
  EXPECT_EQ("No", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputMTR).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputESO).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputEIO).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputTabular).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSQLite).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputJSON).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputAUDIT).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputZoneSizing).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSystemSizing).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDXF).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputBND).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputRDD).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputMDD).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputMTD).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputEND).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSHD).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDFS).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputGLHE).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDelightIn).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDelightELdmp).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDelightDFdmp).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputEDD).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputDBG).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputPerfLog).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSLN).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputSCI).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputWRL).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputScreen).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputExtShd).get());
  EXPECT_EQ("Yes", idf_outputControlFiles.getString(OutputControl_FilesFields::OutputTarcog).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputControlFiles) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlFiles>());

  OptionalWorkspaceObject _i_outputControlFiles = w.addObject(IdfObject(IddObjectType::OutputControl_Files));
  ASSERT_TRUE(_i_outputControlFiles);

  Model m2 = rt.translateWorkspace(w);

  // Get the unique object
  OutputControlFiles outputControlFiles = m2.getUniqueModelObject<OutputControlFiles>();

  EXPECT_FALSE(outputControlFiles.outputCSV());
  EXPECT_FALSE(outputControlFiles.outputMTR());
  EXPECT_TRUE(outputControlFiles.outputESO());
  EXPECT_TRUE(outputControlFiles.outputEIO());
  EXPECT_TRUE(outputControlFiles.outputTabular());
  EXPECT_TRUE(outputControlFiles.outputSQLite());
  EXPECT_TRUE(outputControlFiles.outputJSON());
  EXPECT_TRUE(outputControlFiles.outputAUDIT());
  EXPECT_TRUE(outputControlFiles.outputZoneSizing());
  EXPECT_TRUE(outputControlFiles.outputSystemSizing());
  EXPECT_TRUE(outputControlFiles.outputDXF());
  EXPECT_TRUE(outputControlFiles.outputBND());
  EXPECT_TRUE(outputControlFiles.outputRDD());
  EXPECT_TRUE(outputControlFiles.outputMDD());
  EXPECT_TRUE(outputControlFiles.outputMTD());
  EXPECT_TRUE(outputControlFiles.outputEND());
  EXPECT_TRUE(outputControlFiles.outputSHD());
  EXPECT_TRUE(outputControlFiles.outputDFS());
  EXPECT_TRUE(outputControlFiles.outputGLHE());
  EXPECT_TRUE(outputControlFiles.outputDelightIn());
  EXPECT_TRUE(outputControlFiles.outputDelightELdmp());
  EXPECT_TRUE(outputControlFiles.outputDelightDFdmp());
  EXPECT_TRUE(outputControlFiles.outputEDD());
  EXPECT_TRUE(outputControlFiles.outputDBG());
  EXPECT_TRUE(outputControlFiles.outputPerfLog());
  EXPECT_TRUE(outputControlFiles.outputSLN());
  EXPECT_TRUE(outputControlFiles.outputSCI());
  EXPECT_TRUE(outputControlFiles.outputWRL());
  EXPECT_TRUE(outputControlFiles.outputScreen());
  EXPECT_TRUE(outputControlFiles.outputExtShd());
  EXPECT_TRUE(outputControlFiles.outputTarcog());
}
