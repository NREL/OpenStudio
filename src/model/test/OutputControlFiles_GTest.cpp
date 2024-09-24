/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  EXPECT_TRUE(outputControlFiles.outputSpaceSizing());
  EXPECT_TRUE(outputControlFiles.setOutputSpaceSizing(false));
  EXPECT_FALSE(outputControlFiles.outputSpaceSizing());
  EXPECT_TRUE(outputControlFiles.setOutputSpaceSizing(true));
  EXPECT_TRUE(outputControlFiles.outputSpaceSizing());

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
