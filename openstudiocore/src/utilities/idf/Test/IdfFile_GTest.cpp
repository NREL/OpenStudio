/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "IdfFixture.hpp"

#include "../IdfFile.hpp"
#include "../ValidityReport.hpp"

#include "../../time/Time.hpp"

#include <resources.hxx>
#include <utilities/idd/IddEnums.hxx>



#include <iostream>
#include <sstream>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture, IdfFile_BasicTests_FromScratch) {
  IdfFile idfFile(IddFileType::EnergyPlus); // automatically adds version
  EXPECT_TRUE(idfFile.empty());

  idfFile.addObject(IdfObject(IddObjectType::Building));
  idfFile.addObject(IdfObject(IddObjectType::Zone));
  idfFile.insertObjectByIddObjectType(IdfObject(IddObjectType::Construction));
  idfFile.addObject(IdfObject(IddObjectType::Material));
  EXPECT_FALSE(idfFile.empty());
  EXPECT_EQ(static_cast<unsigned>(4),idfFile.numObjects()); // version not reported out
  IdfObjectVector objs = idfFile.objects();
  ASSERT_EQ(static_cast<unsigned>(4),objs.size());
  EXPECT_TRUE(objs[0].iddObject().type() == IddObjectType::Building);
  EXPECT_TRUE(objs[1].iddObject().type() == IddObjectType::Construction);
  EXPECT_TRUE(objs[2].iddObject().type() == IddObjectType::Zone);
  EXPECT_TRUE(objs[3].iddObject().type() == IddObjectType::Material);
}

TEST_F(IdfFixture, IdfFile_BasicTests_LoadedFile)
{
  // as loaded
  EXPECT_TRUE(epIdfFile.objects().size() > 0);
  LOG(Info,"Checking validity of epIdfFile.");
  ValidityReport report = epIdfFile.validityReport(StrictnessLevel::Final);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  if (report.numErrors() > 0) {
    LOG(Error,"epIdfFile is not valid at Strictness Final. The ValidityReport follows."
        << std::endl << report);
  }

  // write out as Idf for diff
  openstudio::path outPath = outDir/toPath("RoundTrip_in.idf");
  openstudio::Time start = openstudio::Time::currentTime();
  epIdfFile.save(outPath,true);
  openstudio::Time writeTime = openstudio::Time::currentTime() - start;
  LOG(Info, "IdfFile written to idf text in " << writeTime << "s. Please check diff by hand.");
}

TEST_F(IdfFixture, IdfFile_Header) {
  IdfFile file(IddFileType::EnergyPlus);
  std::string header = "! A one-line header. ";
  file.setHeader(header);
  EXPECT_EQ(header,file.header());

  header = "Not actually a header, should get ! pre-pended.";
  file.setHeader(header);
  EXPECT_NE(header,file.header());
  std::stringstream ss;
  ss << "! " << header;
  EXPECT_EQ(ss.str(),file.header());

  header = "Multi-line \n Non-comment.";
  file.setHeader(header);
  EXPECT_EQ("! Multi-line \n! Non-comment.",file.header());
}
/*
TEST_F(IdfFixture, IdfFile_UnixLineEndings) {
  OptionalIdfFile oFile = IdfFile::load(resourcesPath()/toPath("utilities/Idf/UnixLineEndingTest.idf"));
  ASSERT_TRUE(oFile);
  EXPECT_EQ(static_cast<unsigned>(5),oFile->objects().size());
  EXPECT_EQ(static_cast<unsigned>(0),oFile->getObjectsByType(IddObjectType::Catchall).size());
}

TEST_F(IdfFixture, IdfFile_MixedLineEndings) {
  OptionalIdfFile oFile = IdfFile::load(resourcesPath()/toPath("utilities/Idf/MixedLineEndingTest.idf"));
  ASSERT_TRUE(oFile);
  EXPECT_EQ(static_cast<unsigned>(5),oFile->objects().size());
  EXPECT_EQ(static_cast<unsigned>(0),oFile->getObjectsByType(IddObjectType::Catchall).size());
}

TEST_F(IdfFixture, IdfFile_DosLineEndings) {
  OptionalIdfFile oFile = IdfFile::load(resourcesPath()/toPath("utilities/Idf/DosLineEndingTest.idf"));
  ASSERT_TRUE(oFile);
  EXPECT_EQ(static_cast<unsigned>(5),oFile->objects().size());
  EXPECT_EQ(static_cast<unsigned>(0),oFile->getObjectsByType(IddObjectType::Catchall).size());
}

TEST_F(IdfFixture, IdfFile_ObjectComments) {
  OptionalIdfFile oFile = IdfFile::load(resourcesPath()/toPath("utilities/Idf/CommentTest.idf"));
  ASSERT_TRUE(oFile);
  // header
  // CommentOnlyObject
  // Version
  // Timestep w/ comment
  // SimulationControl
  // RunPeriod w/ comment
  // Building w/ comment
  std::stringstream ss;

  // check header
  ss << "! File Header" << std::endl
     << "! Written by Elaine T. Hale, 15 September 2010";
  EXPECT_EQ(ss.str(),oFile->header());

  // check objects
  IdfObjectVector objects = oFile->objects();
  EXPECT_EQ(static_cast<unsigned>(5),objects.size());
  ASSERT_TRUE(objects.size() >= 5);
  EXPECT_TRUE(objects[0].iddObject().type() == IddObjectType::CommentOnly);
  EXPECT_TRUE(objects[1].iddObject().type() == IddObjectType::Timestep);
  EXPECT_EQ("! Timestep should be > 1.",objects[1].comment());
  EXPECT_TRUE(objects[2].iddObject().type() == IddObjectType::SimulationControl);
  EXPECT_TRUE(objects[3].iddObject().type() == IddObjectType::RunPeriod);
  EXPECT_EQ("! When to start, when to stop, when to trust the weather file.",objects[3].comment());
  EXPECT_TRUE(objects[4].iddObject().type() == IddObjectType::Building);
  EXPECT_EQ("! This building has no surfaces, no volume, no nothing.",objects[4].comment());

  // print out result for debugging purposes
  openstudio::filesystem::ofstream outFile(resourcesPath()/toPath("utilities/Idf/CommentTest_Roundtrip.idf"));
  ASSERT_TRUE(outFile?true:false);
  oFile->print(outFile);
}
*/
