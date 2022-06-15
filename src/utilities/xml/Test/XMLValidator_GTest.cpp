/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <algorithm>
#include <iostream>
#include <boost/regex.hpp>
#include <resources.hxx>
#include <stdexcept>

using namespace std;
using namespace boost;
using namespace openstudio;

/* TEST_F(XMLValidatorFixture, Strings) {
  XMLValidator xmlValidator(xsdString);
  ASSERT_TRUE(xmlValidator.xsdString());

  std::string xmlString;

  xmlString = "";
  EXPECT_FALSE(xmlValidator.validate(xmlString));
}

TEST_F(XMLValidatorFixture, NonXMLFile) {
  XMLValidator xmlValidator(xsdPath);
  ASSERT_TRUE(xmlValidator.xsdPath());
  EXPECT_NE("", xmlValidator.xsdPath().get());

  openstudio::path xmlFile;

  xmlFile = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  EXPECT_FALSE(xmlValidator.xsdValidate(xmlFile));
} */

TEST_F(XMLValidatorFixture, GBXML_ValidateResources) {

  openstudio::path xmlPath;

  xmlPath = resourcesPath() / openstudio::toPath("gbxml/3951_Geometry_bug.xml");
  XMLValidator xmlValidator(xsdPath, xmlPath);
  EXPECT_NE("", xmlValidator.schemaPath());
  EXPECT_NE("", xmlValidator.xmlPath());
  EXPECT_TRUE(xmlValidator.validate());
  EXPECT_TRUE(xmlValidator.isValid());
  EXPECT_EQ(0u, xmlValidator.warnings().size());
  EXPECT_EQ(0u, xmlValidator.errors().size());
  for (const auto& logMessage : xmlValidator.errors()) {
    EXPECT_TRUE(false) << logMessage.logMessage();
  }

  /*   xmlFile = resourcesPath() / openstudio::toPath("gbxml/3997_WindowScaling_bug.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/gbXMLStandard_Single_Family_Residential_2016.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/seb.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/TestCubeAlternateUnits.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/TestSchedules.xml");
  EXPECT_FALSE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/TropicBird.xml");
  EXPECT_FALSE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/TropicBird_BEM_4_2018.xml");
  EXPECT_FALSE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.xml");
  EXPECT_FALSE(xmlValidator.xsdValidate(xmlFile));

  xmlFile = resourcesPath() / openstudio::toPath("gbxml/ZNETH.xml");
  EXPECT_TRUE(xmlValidator.xsdValidate(xmlFile)); */
}

TEST_F(XMLValidatorFixture, XMLValidator_HPXMLvalidator_XSLT) {
  openstudio::path xmlPath = resourcesPath() / openstudio::toPath("utilities/xml/hpxml_with_error.xml");
  openstudio::path schematronPath = resourcesPath() / openstudio::toPath("utilities/xml/HPXMLvalidator.xslt");

  XMLValidator xmlValidator(schematronPath, xmlPath);
  EXPECT_FALSE(xmlValidator.validate());

  ASSERT_TRUE(xmlValidator.fullValidationReport());
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
