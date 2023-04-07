/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "XMLValidatorFixture.hpp"
#include "utilities/core/Filesystem.hpp"
#include "utilities/core/PathHelpers.hpp"

#include <algorithm>
#include <iostream>
#include <boost/regex.hpp>
#include <resources.hxx>
#include <stdexcept>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(XMLValidatorFixture, XMLValidator_isValid) {
  auto xmlValidator = XMLValidator::gbxmlValidator();

  EXPECT_FALSE(xmlValidator.isValid());
  ASSERT_EQ(1, xmlValidator.warnings().size());
  ASSERT_EQ(0, xmlValidator.errors().size());
  EXPECT_TRUE(xmlValidator.warnings()[0].logMessage().find("Nothing has yet been validated against") != std::string::npos);
}

TEST_F(XMLValidatorFixture, XMLValidator_NonSchemaPath) {
  openstudio::path nonSchemaPath = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/eplusout.sql");

  StringStreamLogSink sink;
  sink.setLogLevel(Error);

  EXPECT_THROW(XMLValidator xmlValidator(nonSchemaPath), openstudio::Exception);
  ASSERT_EQ(1, sink.logMessages().size());
  EXPECT_EQ("Schema path extension '.sql' not supported.", sink.logMessages()[0].logMessage());
}

TEST_F(XMLValidatorFixture, XMLValidator_NonXMLPath) {
  auto xmlValidator = XMLValidator::gbxmlValidator();
  EXPECT_NE("", xmlValidator.schemaPath());
  EXPECT_FALSE(xmlValidator.xmlPath());

  openstudio::path nonXMLPath = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  EXPECT_THROW(xmlValidator.validate(nonXMLPath), openstudio::Exception);
  ASSERT_EQ(0, xmlValidator.warnings().size());
  ASSERT_EQ(1, xmlValidator.errors().size());
  EXPECT_EQ("XML path extension '.sql' not supported.", xmlValidator.errors()[0].logMessage());
  EXPECT_FALSE(xmlValidator.xmlPath());
}

TEST_F(XMLValidatorFixture, XMLValidator_HPXMLvalidator_XSD) {
  openstudio::path xmlPath = resourcesPath() / openstudio::toPath("utilities/xml/base.xml");
  openstudio::path xsdPath = resourcesPath() / openstudio::toPath("utilities/xml/schema/HPXML.xsd");

  XMLValidator xmlValidator(xsdPath);
  EXPECT_FALSE(xmlValidator.xmlPath());

  for (int i = 0; i < 2; ++i) {
    EXPECT_FALSE(xmlValidator.validate(xmlPath)) << "Failed at iteration " << i + 1;
    EXPECT_FALSE(xmlValidator.isValid());

    ASSERT_TRUE(xmlValidator.xmlPath());
    EXPECT_EQ(xmlPath, xmlValidator.xmlPath().get());
    EXPECT_EQ(0, xmlValidator.warnings().size());

    auto errors = xmlValidator.errors();
    ASSERT_EQ(1, errors.size());

    EXPECT_EQ(LogLevel::Error, errors[0].logLevel());
    EXPECT_EQ("xsdValidate.parseFileError: XML error 17.1835: Element '{http://hpxmlonline.com/2019/10}"
              "AssemblyEffectiveRValue': [facet 'minExclusive'] The value '0.0' must be greater than '0'.\n"
              "    at "
                + toString(xmlPath) + ":117 while processing \"0.0\"",
              errors[0].logMessage());
  }
}

TEST_F(XMLValidatorFixture, XMLValidator_HPXMLvalidator_XSLT) {
  openstudio::path xmlPath = resourcesPath() / openstudio::toPath("utilities/xml/hpxml_with_error.xml");
  openstudio::path schematronPath = resourcesPath() / openstudio::toPath("utilities/xml/schema/HPXMLvalidator.xslt");

  XMLValidator xmlValidator(schematronPath);
  EXPECT_FALSE(xmlValidator.xmlPath());

  for (int i = 0; i < 2; ++i) {
    EXPECT_FALSE(xmlValidator.validate(xmlPath)) << "Failed at iteration " << i + 1;
    EXPECT_FALSE(xmlValidator.isValid());

    ASSERT_TRUE(xmlValidator.fullValidationReport());
    ASSERT_TRUE(xmlValidator.xmlPath());
    EXPECT_EQ(xmlPath, xmlValidator.xmlPath().get());

    EXPECT_NE("", xmlValidator.fullValidationReport().get());
    EXPECT_EQ(0, xmlValidator.warnings().size());

    auto errors = xmlValidator.errors();
    ASSERT_EQ(1, errors.size());
    EXPECT_EQ(LogLevel::Error, errors[0].logLevel());
    EXPECT_EQ("xsltValidate: Expected EventType to be 'audit' or 'proposed workscope' or 'approved workscope' or "
              "'construction-period testing/daily test out' or 'job completion testing/final inspection' or "
              "'quality assurance/monitoring' or 'preconstruction'",
              errors[0].logMessage());
  }
}

TEST_F(XMLValidatorFixture, XMLValidator_HPXMLvalidator_Schematron) {
  openstudio::path xmlPath = resourcesPath() / openstudio::toPath("utilities/xml/hpxml_with_error.xml");
  openstudio::path schematronPath = resourcesPath() / openstudio::toPath("utilities/xml/schema/HPXMLvalidator.xml");

  openstudio::path tmpDir;
  {
    XMLValidator xmlValidator(schematronPath);
    EXPECT_FALSE(xmlValidator.xmlPath());
    EXPECT_NE(schematronPath, xmlValidator.schemaPath());
    tmpDir = xmlValidator.schemaPath().parent_path();
    EXPECT_TRUE(openstudio::filesystem::exists(tmpDir));
    EXPECT_TRUE(openstudio::filesystem::is_directory(tmpDir));

    for (int i = 0; i < 2; ++i) {
      EXPECT_FALSE(xmlValidator.validate(xmlPath)) << "Failed at iteration " << i + 1;
      EXPECT_FALSE(xmlValidator.isValid());

      ASSERT_TRUE(xmlValidator.xmlPath());
      EXPECT_EQ(xmlPath, xmlValidator.xmlPath().get());

      ASSERT_TRUE(xmlValidator.fullValidationReport());
      EXPECT_NE("", xmlValidator.fullValidationReport().get());
      EXPECT_EQ(0, xmlValidator.warnings().size());

      auto errors = xmlValidator.errors();
      ASSERT_EQ(1, errors.size()) << "Failed at iteration " << i + 1;
      EXPECT_EQ(LogLevel::Error, errors[0].logLevel());
      EXPECT_EQ("xsltValidate: Expected EventType to be 'audit' or 'proposed workscope' or 'approved workscope' or "
                "'construction-period testing/daily test out' or 'job completion testing/final inspection' or "
                "'quality assurance/monitoring' or 'preconstruction'",
                errors[0].logMessage());
    }
  }
  // #4761 - XMLValidator's dtor should clean up the tmpDir
  EXPECT_FALSE(openstudio::filesystem::exists(tmpDir)) << "Expected tmpDir to be deleted: " << tmpDir;
}

TEST_F(XMLValidatorFixture, XMLValidator_schematronToXslt) {
  openstudio::path schematronPath = resourcesPath() / openstudio::toPath("utilities/xml/schema/HPXMLvalidator.xml");

  openstudio::path outDir = resourcesPath() / openstudio::toPath("utilities/xml/schema/schematronToXslt");
  if (openstudio::filesystem::is_directory(outDir)) {
    openstudio::removeDirectory(outDir);
  }

  openstudio::path expectedPath = outDir / openstudio::toPath("HPXMLvalidator_stylesheet.xslt");
  openstudio::path savedPath = XMLValidator::schematronToXslt(schematronPath, outDir);
  EXPECT_EQ(expectedPath, savedPath);

  EXPECT_TRUE(openstudio::filesystem::is_regular_file(expectedPath));
}

TEST_P(GbXMLValidatorParametrizedFixture, XMLValidator_GBXMLvalidator_XSD) {
  const auto& [filename, n_warnings, n_errors] = GetParam();

  auto xmlValidator = XMLValidator::gbxmlValidator();
  openstudio::path xmlPath = resourcesPath() / openstudio::toPath(filename);

  if (n_errors > 0) {
    EXPECT_FALSE(xmlValidator.validate(xmlPath));
    EXPECT_FALSE(xmlValidator.isValid());
  } else {
    EXPECT_TRUE(xmlValidator.validate(xmlPath));
    EXPECT_TRUE(xmlValidator.isValid());
  }

  ASSERT_TRUE(xmlValidator.xmlPath());
  EXPECT_EQ(xmlPath, xmlValidator.xmlPath().get());

  EXPECT_EQ(n_warnings, xmlValidator.warnings().size());
  EXPECT_EQ(n_errors, xmlValidator.errors().size());
  for (const auto& logMessage : xmlValidator.errors()) {
    EXPECT_NE("", logMessage.logMessage());
  }
}

INSTANTIATE_TEST_SUITE_P(XMLValidatorFixture, GbXMLValidatorParametrizedFixture,
                         ::testing::Values(std::make_tuple("gbxml/3951_Geometry_bug.xml", 0, 7),
                                           std::make_tuple("gbxml/3997_WindowScaling_bug.xml", 0, 14),
                                           std::make_tuple("gbxml/gbXMLStandard_Single_Family_Residential_2016.xml", 0, 1209),
                                           std::make_tuple("gbxml/seb.xml", 0, 16), std::make_tuple("gbxml/simpleBox_vasari.xml", 0, 27),
                                           std::make_tuple("gbxml/TestCube.xml", 0, 8), std::make_tuple("gbxml/TestCubeAlternateUnits.xml", 0, 8),
                                           std::make_tuple("gbxml/TestSchedules.xml", 0, 16), std::make_tuple("gbxml/TropicBird.xml", 0, 110),
                                           std::make_tuple("gbxml/TwoStoryOffice_Trane.xml", 0, 236), std::make_tuple("gbxml/ZNETH.xml", 0, 204)),
                         [](const testing::TestParamInfo<GbXMLValidatorParametrizedFixture::ParamType>& info) {
                           auto filename = std::get<0>(info.param);
                           std::replace_if(
                             filename.begin(), filename.end(), [](char c) { return !std::isalnum(c); }, '_');
                           return filename;
                         });
