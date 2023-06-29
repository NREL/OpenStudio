/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  IdfFile idfFile(IddFileType::EnergyPlus);  // automatically adds version
  EXPECT_TRUE(idfFile.empty());

  idfFile.addObject(IdfObject(IddObjectType::Building));
  idfFile.addObject(IdfObject(IddObjectType::Zone));
  idfFile.insertObjectByIddObjectType(IdfObject(IddObjectType::Construction));
  idfFile.addObject(IdfObject(IddObjectType::Material));
  EXPECT_FALSE(idfFile.empty());
  EXPECT_EQ(static_cast<unsigned>(4), idfFile.numObjects());  // version not reported out
  IdfObjectVector objs = idfFile.objects();
  ASSERT_EQ(static_cast<unsigned>(4), objs.size());
  EXPECT_TRUE(objs[0].iddObject().type() == IddObjectType::Building);
  EXPECT_TRUE(objs[1].iddObject().type() == IddObjectType::Construction);
  EXPECT_TRUE(objs[2].iddObject().type() == IddObjectType::Zone);
  EXPECT_TRUE(objs[3].iddObject().type() == IddObjectType::Material);
}

TEST_F(IdfFixture, IdfFile_BasicTests_LoadedFile) {
  // as loaded
  EXPECT_TRUE(!epIdfFile.objects().empty());
  LOG(Info, "Checking validity of epIdfFile.");
  ValidityReport report = epIdfFile.validityReport(StrictnessLevel::Final);
  EXPECT_EQ(static_cast<unsigned>(0), report.numErrors());
  if (report.numErrors() > 0) {
    LOG(Error, "epIdfFile is not valid at Strictness Final. The ValidityReport follows." << '\n' << report);
  }

  // write out as Idf for diff
  openstudio::path outPath = outDir / toPath("RoundTrip_in.idf");
  openstudio::Time start = openstudio::Time::currentTime();
  epIdfFile.save(outPath, true);
  openstudio::Time writeTime = openstudio::Time::currentTime() - start;
  LOG(Info, "IdfFile written to idf text in " << writeTime << "s. Please check diff by hand.");
}

TEST_F(IdfFixture, IdfFile_Header) {
  IdfFile file(IddFileType::EnergyPlus);
  std::string header = "! A one-line header. ";
  file.setHeader(header);
  EXPECT_EQ(header, file.header());

  header = "Not actually a header, should get ! pre-pended.";
  file.setHeader(header);
  EXPECT_NE(header, file.header());
  std::stringstream ss;
  ss << "! " << header;
  EXPECT_EQ(ss.str(), file.header());

  header = "Multi-line \n Non-comment.";
  file.setHeader(header);
  EXPECT_EQ("! Multi-line \n! Non-comment.", file.header());
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
  ss << "! File Header" << '\n'
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
