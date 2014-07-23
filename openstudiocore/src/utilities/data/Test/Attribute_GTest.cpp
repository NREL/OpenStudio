/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "DataFixture.hpp"

#include "../Attribute.hpp"

#include "../../core/Json.hpp"

#include <QDomDocument>

#include <boost/regex.hpp>

#include <limits>

#include <OpenStudio.hxx>

using namespace openstudio;


TEST_F(DataFixture, Attribute_BoolTrue)
{
  openstudio::path xmlPath = openstudio::toPath("./report_bool_true.xml");
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
  }

  Attribute attribute("bool", true, std::string("units"));
  EXPECT_EQ("bool", attribute.name());
  EXPECT_EQ(AttributeValueType::Boolean, attribute.valueType().value());
  EXPECT_EQ(true, attribute.valueAsBoolean());
  ASSERT_TRUE(attribute.units());
  EXPECT_EQ("units", attribute.units().get());

  // save to xml
  attribute.saveToXml(xmlPath);

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
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
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
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
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
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
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
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
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
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
  }

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
  if(boost::filesystem::exists(xmlPath)){
    boost::filesystem::remove(xmlPath);
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

TEST_F(DataFixture, Attribute_FromQVariant)
{

  QVariant value = QVariant::fromValue(false);
  boost::optional<Attribute> attribute = Attribute::fromQVariant("bool", value);
  ASSERT_TRUE(attribute);
  EXPECT_EQ("bool", attribute->name());
  EXPECT_EQ(AttributeValueType::Boolean, attribute->valueType().value());
  EXPECT_FALSE(attribute->valueAsBoolean());
  EXPECT_FALSE(attribute->units());

  value = QVariant::fromValue(static_cast<int>(2));
  attribute = Attribute::fromQVariant("int", value);
  ASSERT_TRUE(attribute);
  EXPECT_EQ("int", attribute->name());
  EXPECT_EQ(AttributeValueType::Integer, attribute->valueType().value());
  EXPECT_EQ(2, attribute->valueAsInteger());
  EXPECT_FALSE(attribute->units());

  value = QVariant::fromValue(static_cast<unsigned>(10));
  attribute = Attribute::fromQVariant("unsigned", value, std::string("ft"));
  ASSERT_TRUE(attribute);
  EXPECT_EQ("unsigned", attribute->name());
  EXPECT_EQ(AttributeValueType::Unsigned, attribute->valueType().value());
  EXPECT_EQ(10u, attribute->valueAsUnsigned());
  ASSERT_TRUE(attribute->units());
  EXPECT_EQ("ft", attribute->units().get());

  value = QVariant::fromValue(1.234);
  attribute = Attribute::fromQVariant("double", value, std::string("m"));
  ASSERT_TRUE(attribute);
  EXPECT_EQ("double", attribute->name());
  EXPECT_EQ(AttributeValueType::Double, attribute->valueType().value());
  EXPECT_EQ(1.234, attribute->valueAsDouble());
  ASSERT_TRUE(attribute->units());
  EXPECT_EQ("m", attribute->units().get());

  value = QVariant::fromValue(std::string("hello"));
  attribute = Attribute::fromQVariant("std::string", value);
  ASSERT_TRUE(attribute);
  EXPECT_EQ("std::string", attribute->name());
  EXPECT_EQ(AttributeValueType::String, attribute->valueType().value());
  EXPECT_EQ("hello", attribute->valueAsString());
  EXPECT_FALSE(attribute->units());

  boost::optional<double> d = 1.234;
  value = QVariant::fromValue(d);
  attribute = Attribute::fromQVariant("set optional", value);
  ASSERT_TRUE(attribute);
  EXPECT_EQ("set optional", attribute->name());
  EXPECT_EQ(AttributeValueType::Double, attribute->valueType().value());
  EXPECT_EQ(1.234, attribute->valueAsDouble());
  EXPECT_FALSE(attribute->units());

  d.reset();
  value = QVariant::fromValue(d);
  attribute = Attribute::fromQVariant("unset optional", value);
  EXPECT_FALSE(attribute);
}

TEST_F(DataFixture, Attribute_Equal)
{
  EXPECT_TRUE(Attribute("name", false) == Attribute("name", false));
  EXPECT_TRUE(Attribute("name", static_cast<int>(1), std::string("m")) == Attribute("name", static_cast<int>(1), std::string("m")));
  EXPECT_TRUE(Attribute("name", static_cast<unsigned>(1), std::string("m")) == Attribute("name", static_cast<unsigned>(1), std::string("m")));
  EXPECT_TRUE(Attribute("name", static_cast<int>(1)) == Attribute("name", static_cast<unsigned>(1))); // QVariant comparison does this
  EXPECT_TRUE(Attribute("name", static_cast<int>(1)) == Attribute("name", 1.0)); // QVariant comparison does this
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
  
  QString str = QString::number(value);  
  EXPECT_EQ("3.14159e+52",toString(str)); // original behavior, bad for http

  str = QString::number(value,'G',std::numeric_limits<double>::digits10);
  EXPECT_EQ("3.14159E52",boost::regex_replace(toString(str),boost::regex("\\+"),""));
}

TEST_F(DataFixture, Attribute_JsonSerialization) {
  // right now, just test string because know that fails
  Attribute attribute("EnergyPlusVersion","EnergyPlus-Windows-64 8.0.0.008, YMD=2013.10.02 16:22");
  QVariant variant = detail::toVariant(attribute);
  EXPECT_NO_THROW(toJSON(variant));
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
  AttributeVector attributes;

  // create vector of attributes with no sources
  attributes.push_back(Attribute("My Boolean Attribute",false));
  attributes.push_back(Attribute("My Double Attribute",34.2,"W"));
  attributes.push_back(Attribute("My Integer Attribute",5));
  attributes.push_back(Attribute("My String Attribute","flat finish"));
  attributes.push_back(Attribute("tricky_source","don't talk back"));

  // xml and back
  Attribute container("Containing Attribute",attributes);
  QDomDocument doc = container.toXml();
  OptionalAttribute containerCopy = Attribute::loadFromXml(doc);
  ASSERT_TRUE(containerCopy);
  AttributeVector attributesCopy = containerCopy.get().valueAsAttributeVector();
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_TRUE(attributeCopy.source().empty());
  }

  // json and back
  QVariant variant = detail::toVariant(attributes);
  int n = variant.toMap().size();
  attributesCopy = detail::toVectorOfAttribute(variant,VersionString(openStudioVersion()));
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_TRUE(attributeCopy.source().empty());
  }

  // apply same source to all attributes
  for (Attribute& attribute : attributes) {
    attribute.setSource("big data set");
  }

  // xml and back
  doc = container.toXml();
  containerCopy = Attribute::loadFromXml(doc);
  ASSERT_TRUE(containerCopy);
  attributesCopy = containerCopy.get().valueAsAttributeVector();
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_EQ("big data set",attributeCopy.source());
  }

  // json and back
  variant = detail::toVariant(attributes);
  EXPECT_EQ(n+1,variant.toMap().size());
  attributesCopy = detail::toVectorOfAttribute(variant,VersionString(openStudioVersion()));
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_EQ("big data set",attributeCopy.source());
  }

  // change one attribute's source to something different
  attributes[2].setSource("a wiki");

  // xml and back
  doc = container.toXml();
  containerCopy = Attribute::loadFromXml(doc);
  ASSERT_TRUE(containerCopy);
  attributesCopy = containerCopy.get().valueAsAttributeVector();
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_FALSE(attributeCopy.source().empty());
  }
  EXPECT_EQ("a wiki",attributesCopy[2].source());

  // json and back
  variant = detail::toVariant(attributes);
  EXPECT_EQ(n+attributes.size(),variant.toMap().size());
  attributesCopy = detail::toVectorOfAttribute(variant,VersionString(openStudioVersion()));
  EXPECT_EQ(attributes.size(),attributesCopy.size());
  for (const Attribute& attributeCopy : attributesCopy) {
    EXPECT_FALSE(attributeCopy.source().empty());
  }
  // order is not guaranteed

}
