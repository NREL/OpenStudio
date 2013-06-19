/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <standardsinterface/test/StandardsInterfaceFixture.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <boost/foreach.hpp>

using namespace openstudio::standardsinterface;

TEST_F(StandardsInterfaceFixture,DataDictionary_ObjectTypes) {
  ObjectTypeVector values = DataDictionary::objectTypes();
  BOOST_FOREACH(const ObjectType& val,values) {
    EXPECT_TRUE(DataDictionary::isObjectType(val.valueName()));
    EXPECT_TRUE(DataDictionary::isObjectType(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isBooleanAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isIntegerAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isDoubleAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isEnumAttribute(val.valueDescription()));
    ASSERT_TRUE(DataDictionary::partOfSpeech(val.valueName()));
    EXPECT_TRUE(DataDictionary::partOfSpeech(val.valueName()).get() == PartsOfSpeech::ObjectType);
  }
  EXPECT_FALSE(DataDictionary::isObjectType("dor"));
  EXPECT_FALSE(DataDictionary::partOfSpeech("dor"));
  EXPECT_FALSE(DataDictionary::isObjectType("exterior_window"));
  EXPECT_FALSE(DataDictionary::partOfSpeech("exterior_window"));
}

TEST_F(StandardsInterfaceFixture,DataDictionary_BooleanAttributes) {
  BooleanAttributeVector values = DataDictionary::booleanAttributes();
  BOOST_FOREACH(const BooleanAttribute& val,values) {
    EXPECT_FALSE(DataDictionary::isObjectType(val.valueDescription()));
    EXPECT_TRUE(DataDictionary::isBooleanAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isIntegerAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isDoubleAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isEnumAttribute(val.valueDescription()));
    ASSERT_TRUE(DataDictionary::partOfSpeech(val.valueName()));
    EXPECT_TRUE(DataDictionary::partOfSpeech(val.valueName()).get() == PartsOfSpeech::Boolean);
  }
  EXPECT_FALSE(DataDictionary::isBooleanAttribute("is cold"));
  EXPECT_FALSE(DataDictionary::partOfSpeech("is cold"));
}

TEST_F(StandardsInterfaceFixture,DataDictionary_IntegerAttributes) {
  IntegerAttributeVector values = DataDictionary::integerAttributes();
  BOOST_FOREACH(const IntegerAttribute& val,values) {
    EXPECT_FALSE(DataDictionary::isObjectType(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isBooleanAttribute(val.valueDescription()));
    EXPECT_TRUE(DataDictionary::isIntegerAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isDoubleAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isEnumAttribute(val.valueDescription()));
    ASSERT_TRUE(DataDictionary::partOfSpeech(val.valueName()));
    EXPECT_TRUE(DataDictionary::partOfSpeech(val.valueName()).get() == PartsOfSpeech::Integer);
  }
}

TEST_F(StandardsInterfaceFixture,DataDictionary_DoubleAttributes) {
  DoubleAttributeVector values = DataDictionary::doubleAttributes();
  BOOST_FOREACH(const DoubleAttribute& val,values) {
    EXPECT_FALSE(DataDictionary::isObjectType(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isBooleanAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isIntegerAttribute(val.valueDescription()));
    EXPECT_TRUE(DataDictionary::isDoubleAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isEnumAttribute(val.valueDescription()));
    ASSERT_TRUE(DataDictionary::partOfSpeech(val.valueName()));
    EXPECT_TRUE(DataDictionary::partOfSpeech(val.valueName()).get() == PartsOfSpeech::Double);
  }
}

TEST_F(StandardsInterfaceFixture,DataDictionary_EnumAttributes) {
  EnumAttributeVector values = DataDictionary::enumAttributes();
  BOOST_FOREACH(const EnumAttribute& val,values) {
    EXPECT_FALSE(DataDictionary::isObjectType(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isBooleanAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isIntegerAttribute(val.valueDescription()));
    EXPECT_FALSE(DataDictionary::isDoubleAttribute(val.valueDescription()));
    EXPECT_TRUE(DataDictionary::isEnumAttribute(val.valueDescription()));
    ASSERT_TRUE(DataDictionary::partOfSpeech(val.valueName()));
    EXPECT_TRUE(DataDictionary::partOfSpeech(val.valueName()).get() == PartsOfSpeech::Enum);
  }
}
