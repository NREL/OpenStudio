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

#include "ModelFixture.hpp"
#include "../OutputControlFiles.hpp"
#include "../OutputControlFiles_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputControlFiles_GettersSetters) {
  Model model;
  OutputControlFiles outputControlFiles = model.getUniqueModelObject<OutputControlFiles>();

  EXPECT_FALSE(outputControlFiles.outputCSV());
  EXPECT_TRUE(outputControlFiles.setOutputCSV(false));
  EXPECT_FALSE(outputControlFiles.outputCSV());
  EXPECT_TRUE(outputControlFiles.setOutputCSV(true));
  EXPECT_TRUE(outputControlFiles.outputCSV());

  EXPECT_TRUE(outputControlFiles.outputMTR());
  EXPECT_TRUE(outputControlFiles.setOutputMTR(false));
  EXPECT_FALSE(outputControlFiles.outputMTR());
  EXPECT_TRUE(outputControlFiles.setOutputMTR(true));
  EXPECT_TRUE(outputControlFiles.outputMTR());

  EXPECT_TRUE(outputControlFiles.outputESO());
  EXPECT_TRUE(outputControlFiles.setOutputESO(false));
  EXPECT_FALSE(outputControlFiles.outputESO());
  EXPECT_TRUE(outputControlFiles.setOutputESO(true));
  EXPECT_TRUE(outputControlFiles.outputESO());

  EXPECT_TRUE(outputControlFiles.outputEIO());
  EXPECT_TRUE(outputControlFiles.setOutputEIO(false));
  EXPECT_FALSE(outputControlFiles.outputEIO());
  EXPECT_TRUE(outputControlFiles.setOutputEIO(true));
  EXPECT_TRUE(outputControlFiles.outputEIO());

  EXPECT_TRUE(outputControlFiles.outputTabular());
  EXPECT_TRUE(outputControlFiles.setOutputTabular(false));
  EXPECT_FALSE(outputControlFiles.outputTabular());
  EXPECT_TRUE(outputControlFiles.setOutputTabular(true));
  EXPECT_TRUE(outputControlFiles.outputTabular());

  EXPECT_TRUE(outputControlFiles.outputSQLite());
  EXPECT_TRUE(outputControlFiles.setOutputSQLite(false));
  EXPECT_FALSE(outputControlFiles.outputSQLite());
  EXPECT_TRUE(outputControlFiles.setOutputSQLite(true));
  EXPECT_TRUE(outputControlFiles.outputSQLite());

  EXPECT_TRUE(outputControlFiles.outputJSON());
  EXPECT_TRUE(outputControlFiles.setOutputJSON(false));
  EXPECT_FALSE(outputControlFiles.outputJSON());
  EXPECT_TRUE(outputControlFiles.setOutputJSON(true));
  EXPECT_TRUE(outputControlFiles.outputJSON());

  EXPECT_TRUE(outputControlFiles.outputAUDIT());
  EXPECT_TRUE(outputControlFiles.setOutputAUDIT(false));
  EXPECT_FALSE(outputControlFiles.outputAUDIT());
  EXPECT_TRUE(outputControlFiles.setOutputAUDIT(true));
  EXPECT_TRUE(outputControlFiles.outputAUDIT());

  EXPECT_TRUE(outputControlFiles.outputZoneSizing());
  EXPECT_TRUE(outputControlFiles.setOutputZoneSizing(false));
  EXPECT_FALSE(outputControlFiles.outputZoneSizing());
  EXPECT_TRUE(outputControlFiles.setOutputZoneSizing(true));
  EXPECT_TRUE(outputControlFiles.outputZoneSizing());

  EXPECT_TRUE(outputControlFiles.outputSystemSizing());
  EXPECT_TRUE(outputControlFiles.setOutputSystemSizing(false));
  EXPECT_FALSE(outputControlFiles.outputSystemSizing());
  EXPECT_TRUE(outputControlFiles.setOutputSystemSizing(true));
  EXPECT_TRUE(outputControlFiles.outputSystemSizing());

  EXPECT_TRUE(outputControlFiles.outputDXF());
  EXPECT_TRUE(outputControlFiles.setOutputDXF(false));
  EXPECT_FALSE(outputControlFiles.outputDXF());
  EXPECT_TRUE(outputControlFiles.setOutputDXF(true));
  EXPECT_TRUE(outputControlFiles.outputDXF());

  EXPECT_TRUE(outputControlFiles.outputBND());
  EXPECT_TRUE(outputControlFiles.setOutputBND(false));
  EXPECT_FALSE(outputControlFiles.outputBND());
  EXPECT_TRUE(outputControlFiles.setOutputBND(true));
  EXPECT_TRUE(outputControlFiles.outputBND());

  EXPECT_TRUE(outputControlFiles.outputRDD());
  EXPECT_TRUE(outputControlFiles.setOutputRDD(false));
  EXPECT_FALSE(outputControlFiles.outputRDD());
  EXPECT_TRUE(outputControlFiles.setOutputRDD(true));
  EXPECT_TRUE(outputControlFiles.outputRDD());

  EXPECT_TRUE(outputControlFiles.outputMDD());
  EXPECT_TRUE(outputControlFiles.setOutputMDD(false));
  EXPECT_FALSE(outputControlFiles.outputMDD());
  EXPECT_TRUE(outputControlFiles.setOutputMDD(true));
  EXPECT_TRUE(outputControlFiles.outputMDD());

  EXPECT_TRUE(outputControlFiles.outputMTD());
  EXPECT_TRUE(outputControlFiles.setOutputMTD(false));
  EXPECT_FALSE(outputControlFiles.outputMTD());
  EXPECT_TRUE(outputControlFiles.setOutputMTD(true));
  EXPECT_TRUE(outputControlFiles.outputMTD());

  EXPECT_TRUE(outputControlFiles.outputSHD());
  EXPECT_TRUE(outputControlFiles.setOutputSHD(false));
  EXPECT_FALSE(outputControlFiles.outputSHD());
  EXPECT_TRUE(outputControlFiles.setOutputSHD(true));
  EXPECT_TRUE(outputControlFiles.outputSHD());

  EXPECT_TRUE(outputControlFiles.outputDFS());
  EXPECT_TRUE(outputControlFiles.setOutputDFS(false));
  EXPECT_FALSE(outputControlFiles.outputDFS());
  EXPECT_TRUE(outputControlFiles.setOutputDFS(true));
  EXPECT_TRUE(outputControlFiles.outputDFS());

  EXPECT_TRUE(outputControlFiles.outputGLHE());
  EXPECT_TRUE(outputControlFiles.setOutputGLHE(false));
  EXPECT_FALSE(outputControlFiles.outputGLHE());
  EXPECT_TRUE(outputControlFiles.setOutputGLHE(true));
  EXPECT_TRUE(outputControlFiles.outputGLHE());

  EXPECT_TRUE(outputControlFiles.outputDelightIn());
  EXPECT_TRUE(outputControlFiles.setOutputDelightIn(false));
  EXPECT_FALSE(outputControlFiles.outputDelightIn());
  EXPECT_TRUE(outputControlFiles.setOutputDelightIn(true));
  EXPECT_TRUE(outputControlFiles.outputDelightIn());

  EXPECT_TRUE(outputControlFiles.outputDelightELdmp());
  EXPECT_TRUE(outputControlFiles.setOutputDelightELdmp(false));
  EXPECT_FALSE(outputControlFiles.outputDelightELdmp());
  EXPECT_TRUE(outputControlFiles.setOutputDelightELdmp(true));
  EXPECT_TRUE(outputControlFiles.outputDelightELdmp());

  EXPECT_TRUE(outputControlFiles.outputDelightDFdmp());
  EXPECT_TRUE(outputControlFiles.setOutputDelightDFdmp(false));
  EXPECT_FALSE(outputControlFiles.outputDelightDFdmp());
  EXPECT_TRUE(outputControlFiles.setOutputDelightDFdmp(true));
  EXPECT_TRUE(outputControlFiles.outputDelightDFdmp());

  EXPECT_TRUE(outputControlFiles.outputEDD());
  EXPECT_TRUE(outputControlFiles.setOutputEDD(false));
  EXPECT_FALSE(outputControlFiles.outputEDD());
  EXPECT_TRUE(outputControlFiles.setOutputEDD(true));
  EXPECT_TRUE(outputControlFiles.outputEDD());

  EXPECT_TRUE(outputControlFiles.outputDBG());
  EXPECT_TRUE(outputControlFiles.setOutputDBG(false));
  EXPECT_FALSE(outputControlFiles.outputDBG());
  EXPECT_TRUE(outputControlFiles.setOutputDBG(true));
  EXPECT_TRUE(outputControlFiles.outputDBG());

  EXPECT_TRUE(outputControlFiles.outputPerfLog());
  EXPECT_TRUE(outputControlFiles.setOutputPerfLog(false));
  EXPECT_FALSE(outputControlFiles.outputPerfLog());
  EXPECT_TRUE(outputControlFiles.setOutputPerfLog(true));
  EXPECT_TRUE(outputControlFiles.outputPerfLog());

  EXPECT_TRUE(outputControlFiles.outputSLN());
  EXPECT_TRUE(outputControlFiles.setOutputSLN(false));
  EXPECT_FALSE(outputControlFiles.outputSLN());
  EXPECT_TRUE(outputControlFiles.setOutputSLN(true));
  EXPECT_TRUE(outputControlFiles.outputSLN());

  EXPECT_TRUE(outputControlFiles.outputSCI());
  EXPECT_TRUE(outputControlFiles.setOutputSCI(false));
  EXPECT_FALSE(outputControlFiles.outputSCI());
  EXPECT_TRUE(outputControlFiles.setOutputSCI(true));
  EXPECT_TRUE(outputControlFiles.outputSCI());

  EXPECT_TRUE(outputControlFiles.outputWRL());
  EXPECT_TRUE(outputControlFiles.setOutputWRL(false));
  EXPECT_FALSE(outputControlFiles.outputWRL());
  EXPECT_TRUE(outputControlFiles.setOutputWRL(true));
  EXPECT_TRUE(outputControlFiles.outputWRL());

  EXPECT_TRUE(outputControlFiles.outputScreen());
  EXPECT_TRUE(outputControlFiles.setOutputScreen(false));
  EXPECT_FALSE(outputControlFiles.outputScreen());
  EXPECT_TRUE(outputControlFiles.setOutputScreen(true));
  EXPECT_TRUE(outputControlFiles.outputScreen());

  EXPECT_TRUE(outputControlFiles.outputExtShd());
  EXPECT_TRUE(outputControlFiles.setOutputExtShd(false));
  EXPECT_FALSE(outputControlFiles.outputExtShd());
  EXPECT_TRUE(outputControlFiles.setOutputExtShd(true));
  EXPECT_TRUE(outputControlFiles.outputExtShd());

  EXPECT_TRUE(outputControlFiles.outputTarcog());
  EXPECT_TRUE(outputControlFiles.setOutputTarcog(false));
  EXPECT_FALSE(outputControlFiles.outputTarcog());
  EXPECT_TRUE(outputControlFiles.setOutputTarcog(true));
  EXPECT_TRUE(outputControlFiles.outputTarcog());
}
