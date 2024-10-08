/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "BCLFixture.hpp"

#include "../BCLXML.hpp"
#include "../BCLFileReference.hpp"
#include "../../xml/XMLValidator.hpp"

#include <pugixml.hpp>

using namespace openstudio;

TEST_F(BCLFixture, BCLXML) {
  openstudio::path path = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/measure.xml");
  boost::optional<BCLXML> bclXML = BCLXML::load(path);
  ASSERT_TRUE(bclXML);
}

TEST_F(BCLFixture, BCLXML_New) {
  BCLXML bclXML(BCLXMLType::MeasureXML);
  EXPECT_FALSE(bclXML.uid().empty());
  EXPECT_FALSE(bclXML.versionId().empty());
  EXPECT_TRUE(bclXML.name().empty());
  EXPECT_TRUE(bclXML.description().empty());
  EXPECT_TRUE(bclXML.modelerDescription().empty());
  EXPECT_TRUE(bclXML.path().empty());
  EXPECT_TRUE(bclXML.directory().empty());
  EXPECT_TRUE(bclXML.attributes().empty());
  EXPECT_TRUE(bclXML.tags().empty());
  EXPECT_TRUE(bclXML.files().empty());

  std::string versionId = bclXML.versionId();
  bclXML.setName("Name");
  EXPECT_EQ("Name", bclXML.name());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.setDescription("Description");
  EXPECT_EQ("Description", bclXML.description());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.setModelerDescription("ModelerDescription");
  EXPECT_EQ("ModelerDescription", bclXML.modelerDescription());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.addTag("tag");
  ASSERT_EQ(1u, bclXML.tags().size());
  EXPECT_EQ("tag", bclXML.tags()[0]);
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.addTag("tag");
  ASSERT_EQ(1u, bclXML.tags().size());
  EXPECT_EQ("tag", bclXML.tags()[0]);
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  Attribute attr("attribute", 1.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(1u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_EQ(1u, bclXML.getAttributes("attribute").size());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  attr = Attribute("attribute", 2.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(2u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_EQ("attribute", bclXML.attributes()[1].name());
  EXPECT_EQ(2u, bclXML.getAttributes("attribute").size());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  attr = Attribute("another attribute", 2.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(3u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_EQ("attribute", bclXML.attributes()[1].name());
  EXPECT_EQ("another attribute", bclXML.attributes()[2].name());
  EXPECT_EQ(2u, bclXML.getAttributes("attribute").size());
  EXPECT_EQ(1u, bclXML.getAttributes("another attribute").size());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  EXPECT_FALSE(bclXML.save());
  EXPECT_EQ(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  openstudio::path xmlPath = toPath("./measure.xml");
  EXPECT_TRUE(bclXML.saveAs(xmlPath));
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  EXPECT_TRUE(bclXML.save());
  EXPECT_EQ(versionId, bclXML.versionId());

  // Validate any newly written XML
  boost::optional<BCLXML> copy = BCLXML::load(xmlPath);
  ASSERT_TRUE(copy);

  auto bclXMLValidator = XMLValidator::bclXMLValidator(BCLXMLType::MeasureXML, BCLXML::currentSchemaVersion());
  EXPECT_TRUE(bclXMLValidator.validate(xmlPath));
  EXPECT_EQ(0, bclXMLValidator.errors().size()) << [&bclXMLValidator]() {
    std::string s;
    for (const auto& logMessage : bclXMLValidator.errors()) {
      s += logMessage.logMessage() + "\n";
    }
    return s;
  }();
  EXPECT_EQ(0, bclXMLValidator.warnings().size());
}

TEST_F(BCLFixture, BCLXML_sorted_files) {

  BCLXML bclXML(BCLXMLType::MeasureXML);

  const openstudio::path testDir("my_measure");

  const BCLFileReference ref1(testDir, "fileA", true);
  const BCLFileReference ref2(testDir, "fileC", true);
  const BCLFileReference ref3(testDir, "fileB", true);
  const BCLFileReference ref4(testDir, "fileD", true);

  bclXML.addFile(ref1);
  bclXML.addFile(ref2);
  bclXML.addFile(ref3);
  bclXML.addFile(ref4);

  const openstudio::path xmlPath = resourcesPath() / toPath("utilities/BCL/sorted.xml");
  bclXML.saveAs(xmlPath);

  pugi::xml_document xmlDoc;
  openstudio::filesystem::ifstream file(xmlPath);
  ASSERT_TRUE(file.is_open());
  ASSERT_TRUE(xmlDoc.load(file));
  auto measureElement = xmlDoc.child("measure");
  auto xmlFilesElement = measureElement.child("files");
  std::vector<std::string> savedFiles;
  for (auto& fileElement : xmlFilesElement.children("file")) {
    if (!fileElement.first_child().empty()) {
      // Filename is a relative path, and if usageType is doc/resource/test it omits the subdirectory
      savedFiles.emplace_back(fileElement.child("filename").text().as_string());
    }
  }

  ASSERT_EQ(4, savedFiles.size());
  EXPECT_EQ("fileA", savedFiles[0]);
  EXPECT_EQ("fileB", savedFiles[1]);
  EXPECT_EQ("fileC", savedFiles[2]);
  EXPECT_EQ("fileD", savedFiles[3]);
}

TEST_F(BCLFixture, BCLXML_validation_historical) {

  // This has an ISO8601 datetime instead of a valid xs:dateTime
  // <version_modified>20170316T161836Z</version_modified>
  const openstudio::path xmlPath = resourcesPath() / toPath("/utilities/BCL/Measures/v3/IncreaseRoofRValue/measure.xml");
  const boost::optional<BCLXML> bclXML = BCLXML::load(xmlPath);
  ASSERT_TRUE(bclXML);

  // Validate with 3.0 schema: it should work
  {
    auto bclXMLValidator = XMLValidator::bclXMLValidator(BCLXMLType::MeasureXML, VersionString(3, 0));
    EXPECT_TRUE(bclXMLValidator.validate(xmlPath));
    EXPECT_EQ(0, bclXMLValidator.errors().size()) << [&bclXMLValidator]() {
      std::string s;
      for (const auto& logMessage : bclXMLValidator.errors()) {
        s += logMessage.logMessage() + "\n";
      }
      return s;
    }();
    EXPECT_EQ(0, bclXMLValidator.warnings().size());
  }

  // Validate with 3.1 schema: it shouldn't work
  {
    auto bclXMLValidator = XMLValidator::bclXMLValidator(BCLXMLType::MeasureXML, VersionString(3, 1));
    EXPECT_FALSE(bclXMLValidator.validate(xmlPath));

    EXPECT_EQ(2, bclXMLValidator.errors().size()) << [&bclXMLValidator]() {
      std::string s;
      for (const auto& logMessage : bclXMLValidator.errors()) {
        s += logMessage.logMessage() + "\n";
      }
      return s;
    }();
    EXPECT_TRUE(checkLogMessagesContain(bclXMLValidator.errors(),
                                        {
                                          "Element 'schema_version': The actual value '3.0' does not match the fixed value constraint '3.1'",
                                          "Element 'version_modified': '20170316T161836Z' is not a valid value of the atomic type 'xs:dateTime",
                                        }));
    EXPECT_EQ(0, bclXMLValidator.warnings().size());
  }
}
