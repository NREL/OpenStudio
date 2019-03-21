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
#include "DataFixture.hpp"

#include "../Attribute.hpp"

#include "../../core/StringHelpers.hpp"

#include <boost/regex.hpp>

#include <pugixml.hpp>
#include <sstream>
#include <limits>

#include <OpenStudio.hxx>

using namespace openstudio;


TEST_F(DataFixture, Attribute_BoolTrue)
{
  openstudio::path xmlPath = openstudio::toPath("./report_bool_true.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  Attribute attribute("bool", true, std::string("units"));
  EXPECT_EQ("bool", attribute.name());
  EXPECT_EQ(AttributeValueType::Boolean, attribute.valueType().value());
  EXPECT_EQ(true, attribute.valueAsBoolean());
  ASSERT_TRUE(attribute.units());
  EXPECT_EQ("units", attribute.units().get());

  // save to xml
  ASSERT_TRUE(attribute.saveToXml(xmlPath));

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("bool", testAttribute->name());
  EXPECT_EQ(AttributeValueType::Boolean, testAttribute->valueType().value());
  EXPECT_EQ(true, testAttribute->valueAsBoolean());
  ASSERT_TRUE(testAttribute->units());
  EXPECT_EQ("units", testAttribute->units().get());
}

TEST_F(DataFixture, Attribute_Integer)
{
  openstudio::path xmlPath = openstudio::toPath("./report_integer.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  Attribute attribute("int", 1);
  EXPECT_EQ("int", attribute.name());
  EXPECT_EQ(AttributeValueType::Integer, attribute.valueType().value());
  EXPECT_EQ(1, attribute.valueAsInteger());
  EXPECT_FALSE(attribute.units());

  // save to xml
  attribute.saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("int", testAttribute->name());
  EXPECT_EQ(AttributeValueType::Integer, testAttribute->valueType().value());
  EXPECT_EQ(1, testAttribute->valueAsInteger());
  EXPECT_FALSE(testAttribute->units());
}

TEST_F(DataFixture, Attribute_Unsigned)
{
  openstudio::path xmlPath = openstudio::toPath("./report_unsigned.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  Attribute attribute("unsigned", 1u);
  EXPECT_EQ("unsigned", attribute.name());
  EXPECT_EQ(AttributeValueType::Unsigned, attribute.valueType().value());
  EXPECT_EQ(static_cast<unsigned>(1), attribute.valueAsUnsigned());
  EXPECT_FALSE(attribute.units());

  // save to xml
  attribute.saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("unsigned", testAttribute->name());
  EXPECT_EQ(AttributeValueType::Unsigned, testAttribute->valueType().value());
  EXPECT_EQ(1u, testAttribute->valueAsUnsigned());
  EXPECT_FALSE(testAttribute->units());
}

TEST_F(DataFixture, Attribute_Double_Small)
{
  openstudio::path xmlPath = openstudio::toPath("./report_double_small.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  Attribute attribute("double", 1.5);
  EXPECT_EQ("double", attribute.name());
  EXPECT_EQ(AttributeValueType::Double, attribute.valueType().value());
  EXPECT_EQ(1.5, attribute.valueAsDouble());
  EXPECT_FALSE(attribute.units());

  // save to xml
  attribute.saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("double", testAttribute->name());
  EXPECT_EQ(AttributeValueType::Double, testAttribute->valueType().value());
  EXPECT_EQ(1.5, testAttribute->valueAsDouble());
  EXPECT_FALSE(testAttribute->units());
}

TEST_F(DataFixture, Attribute_Double_Big)
{
  openstudio::path xmlPath = openstudio::toPath("./report_double_big.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  Attribute attribute("double", 1.189679819371987395175049501E32);
  EXPECT_EQ("double", attribute.name());
  EXPECT_EQ(AttributeValueType::Double, attribute.valueType().value());
  EXPECT_DOUBLE_EQ(1.189679819371987395175049501E32, attribute.valueAsDouble());
  EXPECT_FALSE(attribute.units());

  // save to xml
  attribute.saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("double", testAttribute->name());
  EXPECT_EQ(AttributeValueType::Double, testAttribute->valueType().value());
  double relErr = (1.189679819371987395175049501E32 - testAttribute->valueAsDouble())/1.189679819371987395175049501E32;
  EXPECT_NEAR(0.0, relErr, 5.0E-15);
  EXPECT_FALSE(testAttribute->units());
}

TEST_F(DataFixture, Attribute_String)
{

  openstudio::path xmlPath = openstudio::toPath("./report_string.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  {
    // Call the std::string constructor, should be no problem
    std::string value = "value";
    Attribute attribute("string", value);
    EXPECT_EQ("string", attribute.name());
    EXPECT_EQ(AttributeValueType::String, attribute.valueType().value());
    EXPECT_EQ("value", attribute.valueAsString());
    EXPECT_FALSE(attribute.units());
  }

  // This one calls the const char * constructor, which is needed to avoid resolution to bool ctor
  Attribute attribute("string", "value");
  EXPECT_EQ("string", attribute.name());
  EXPECT_EQ(AttributeValueType::String, attribute.valueType().value());
  EXPECT_EQ("value", attribute.valueAsString());
  EXPECT_FALSE(attribute.units());

  // save to xml
  attribute.saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("string", testAttribute->name());
  EXPECT_EQ(AttributeValueType::String, testAttribute->valueType().value());
  EXPECT_EQ("value", testAttribute->valueAsString());
  EXPECT_FALSE(testAttribute->units());
}

TEST_F(DataFixture, Attribute_AttributeVector)
{

  openstudio::path xmlPath = openstudio::toPath("./report_attribute_vector.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  std::vector<Attribute> attributes;
  attributes.push_back(Attribute("bool", true));
  attributes.push_back(Attribute("double", 1.5));

  Attribute attribute("vector", attributes);
  EXPECT_EQ("vector", attribute.name());
  EXPECT_EQ(AttributeValueType::AttributeVector, attribute.valueType().value());
  ASSERT_EQ(static_cast<unsigned>(2), attribute.valueAsAttributeVector().size());
  EXPECT_EQ(AttributeValueType::Boolean, attribute.valueAsAttributeVector()[0].valueType().value());
  EXPECT_EQ(AttributeValueType::Double, attribute.valueAsAttributeVector()[1].valueType().value());
  EXPECT_FALSE(attribute.units());

  // save to xml
  attribute.saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  EXPECT_EQ("vector", testAttribute->name());
  EXPECT_EQ(AttributeValueType::AttributeVector, testAttribute->valueType().value());
  ASSERT_EQ(static_cast<unsigned>(2), testAttribute->valueAsAttributeVector().size());
  EXPECT_EQ(AttributeValueType::Boolean, testAttribute->valueAsAttributeVector()[0].valueType().value());
  EXPECT_EQ(AttributeValueType::Double, testAttribute->valueAsAttributeVector()[1].valueType().value());
  EXPECT_FALSE(testAttribute->units());
}

TEST_F(DataFixture, Attribute_Throw)
{
  Attribute attribute("bool", false);
  EXPECT_NO_THROW(attribute.valueAsBoolean());
  EXPECT_THROW(attribute.valueAsInteger(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsDouble(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsString(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsAttributeVector(), openstudio::Exception);

  attribute = Attribute("int", 1);
  EXPECT_THROW(attribute.valueAsBoolean(), openstudio::Exception);
  EXPECT_NO_THROW(attribute.valueAsInteger());
  EXPECT_THROW(attribute.valueAsDouble(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsString(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsAttributeVector(), openstudio::Exception);

  attribute = Attribute("double", 1.5);
  EXPECT_THROW(attribute.valueAsBoolean(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsInteger(), openstudio::Exception);
  EXPECT_NO_THROW(attribute.valueAsDouble());
  EXPECT_THROW(attribute.valueAsString(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsAttributeVector(), openstudio::Exception);

  attribute = Attribute("string", std::string("value"));
  EXPECT_THROW(attribute.valueAsBoolean(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsInteger(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsDouble(), openstudio::Exception);
  EXPECT_NO_THROW(attribute.valueAsString());
  EXPECT_THROW(attribute.valueAsAttributeVector(), openstudio::Exception);

  attribute = Attribute("vector", std::vector<Attribute>());
  EXPECT_THROW(attribute.valueAsBoolean(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsInteger(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsDouble(), openstudio::Exception);
  EXPECT_THROW(attribute.valueAsString(), openstudio::Exception);
  EXPECT_NO_THROW(attribute.valueAsAttributeVector());
}

TEST_F(DataFixture, Attribute_Equal)
{
  EXPECT_TRUE(Attribute("name", false) == Attribute("name", false));
  EXPECT_TRUE(Attribute("name", static_cast<int>(1), std::string("m")) == Attribute("name", static_cast<int>(1), std::string("m")));
  EXPECT_TRUE(Attribute("name", static_cast<unsigned>(1), std::string("m")) == Attribute("name", static_cast<unsigned>(1), std::string("m")));
  EXPECT_FALSE(Attribute("name", static_cast<int>(1)) == Attribute("name", static_cast<unsigned>(1))); // Types don't match
  EXPECT_FALSE(Attribute("name", static_cast<int>(1)) == Attribute("name", 1.0)); // Types don't match

  EXPECT_TRUE(Attribute("name", "value") == Attribute("name", "value"));
  EXPECT_TRUE(Attribute("name", 1.23) == Attribute("name", 1.23));

  std::vector<Attribute> attributes1;
  attributes1.push_back(Attribute("name", 1.23, std::string("m")));

  std::vector<Attribute> attributes2;
  attributes2.push_back(Attribute("name", 1.23, std::string("m")));

  EXPECT_TRUE(Attribute("name", attributes1) == Attribute("name", attributes2));
}

TEST_F(DataFixture, Attribute_NotEqual)
{
  EXPECT_FALSE(Attribute("name", false) == Attribute("name", true));
  EXPECT_FALSE(Attribute("name", false) == Attribute("name", 1.23));
  EXPECT_FALSE(Attribute("name", static_cast<int>(1)) == Attribute("name", static_cast<int>(2)));
  EXPECT_FALSE(Attribute("name", static_cast<unsigned>(1), std::string("m")) == Attribute("name", static_cast<unsigned>(2), std::string("m")));
  EXPECT_FALSE(Attribute("name", "value") == Attribute("name", "other value"));
  EXPECT_FALSE(Attribute("name", 1.23, std::string("m")) == Attribute("name", 1.23, std::string("ft")));

  std::vector<Attribute> attributes1;
  attributes1.push_back(Attribute("name", 1.23, std::string("m")));

  std::vector<Attribute> attributes2;
  attributes2.push_back(Attribute("name", 1.23, std::string("m")));
  attributes2.push_back(Attribute("name", 1.23, std::string("m")));

  EXPECT_FALSE(Attribute("name", attributes1) == Attribute("name", std::vector<Attribute>()));
  EXPECT_FALSE(Attribute("name", attributes1) == Attribute("name", attributes2));
}

TEST_F(DataFixture, Attribute_NumberFormatting) {
  double value(3.14159e52);

  auto str = openstudio::string_conversions::number(value);
  EXPECT_EQ("3.14159e+52",str); // original behavior, bad for http

  str = openstudio::string_conversions::number(value,FloatFormat::general_capital,std::numeric_limits<double>::digits10);
  EXPECT_EQ("3.14159E52",boost::regex_replace(str,boost::regex("\\+"),""));
}

TEST_F(DataFixture, Attribute_DisplayName) {
  Attribute attribute("WWR",0.23);
  OptionalString displayName = attribute.displayName();
  EXPECT_FALSE(displayName);
  displayName = attribute.displayName(true);
  ASSERT_TRUE(displayName);
  EXPECT_EQ("WWR",displayName.get());
  attribute.setDisplayName("Window-to-wall ratio (ratio of fenestration area to gross surface area).");
  displayName = attribute.displayName(true);
  ASSERT_TRUE(displayName);
  EXPECT_NE("WWR",displayName.get());
}

TEST_F(DataFixture, Attribute_Source) {

  openstudio::path xmlPath = openstudio::toPath("./report_attribute_source.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  AttributeVector attributes;

  // create vector of attributes with no sources
  attributes.push_back(Attribute("My Boolean Attribute",false));
  attributes.push_back(Attribute("My Double Attribute",34.2,"W"));
  attributes.push_back(Attribute("My Integer Attribute",5));
  attributes.push_back(Attribute("My String Attribute","flat finish"));
  attributes.push_back(Attribute("tricky_source","don't talk back"));

  Attribute container("Containing Attribute",attributes);

  // save to xml
  container.saveToXml(xmlPath);

  // load back
  boost::optional<Attribute> containerCopy = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(containerCopy);
  AttributeVector attributesCopy = containerCopy.get().valueAsAttributeVector();
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_TRUE(attributeCopy.source().empty());
  }

  // apply same source to all attributes
  for (Attribute& attribute : attributes) {
    attribute.setSource("big data set");
  }

  // xml and back
  container.saveToXml(xmlPath);
  containerCopy = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(containerCopy);
  attributesCopy = containerCopy.get().valueAsAttributeVector();
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_EQ("big data set",attributeCopy.source());
  }

  // change one attribute's source to something different
  attributes[2].setSource("a wiki");

  // xml and back
  container.saveToXml(xmlPath);
  containerCopy = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(containerCopy);
  attributesCopy = containerCopy.get().valueAsAttributeVector();
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_FALSE(attributeCopy.source().empty());
  }
  EXPECT_EQ("a wiki",attributesCopy[2].source());


  // order is not guaranteed

}


TEST_F(DataFixture, Attribute_RequiredXMLNodes) {

  pugi::xml_document attributeXML;

  // Test that it works with something proper
  {

    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <ValueType>Double</ValueType>" << std::endl
       << "  <Value>10.0</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    Attribute(attributeXML.child("Attribute"));
  }

  // Missing UUID
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <ValueType>Double</ValueType>" << std::endl
       << "  <Value>10.0</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }

  // Missing VersionUUID
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <ValueType>Double</ValueType>" << std::endl
       << "  <Value>10.0</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }

  // Missing Name
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <ValueType>Double</ValueType>" << std::endl
       << "  <Value>10.0</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }


  // Missing ValueType
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <Value>10.0</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }

  // Wrong ValueType
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <ValueType>Something BAAAAAAAAAAAD</ValueType>" << std::endl
       << "  <Value>10.0</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }

  // Missing Value
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <ValueType>Double</ValueType>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }

  // ValueType and Value Don't match: expect a double, but value is a string that doesn't represent a number instead
  {
    std::stringstream ss;
    ss << "<Attribute>" << std::endl
       << "  <UUID>{147d6341-c9a6-41e1-a1c9-fd051c2c7bf2}</UUID>" << std::endl
       << "  <VersionUUID>{bd2b41f5-9183-4733-81e2-81ce19047e0e}</VersionUUID>" << std::endl
       << "  <Name>A double argument</Name>" << std::endl
       << "  <ValueType>Double</ValueType>" << std::endl
       << "  <Value>Blabla</Value>" << std::endl
       << "</Attribute>" << std::endl;

    pugi::xml_parse_result success = attributeXML.load(ss);
    ASSERT_TRUE(success);
    EXPECT_THROW(Attribute(attributeXML.child("Attribute")), openstudio::Exception);
  }

}

