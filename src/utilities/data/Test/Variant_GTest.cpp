/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "DataFixture.hpp"

#include "../Variant.hpp"

#include <json/value.h>

using namespace openstudio;

TEST_F(DataFixture, Variant) {
  Variant boolVariant(true);
  Variant intVariant(1);
  Variant doubleVariant(1.0);
  Variant stringVariant("1");

  EXPECT_EQ(VariantType::Boolean, boolVariant.variantType().value());
  EXPECT_EQ(VariantType::Integer, intVariant.variantType().value());
  EXPECT_EQ(VariantType::Double, doubleVariant.variantType().value());
  EXPECT_EQ(VariantType::String, stringVariant.variantType().value());

  EXPECT_EQ(true, boolVariant.valueAsBoolean());
  EXPECT_EQ(1, intVariant.valueAsInteger());
  EXPECT_EQ(1.0, doubleVariant.valueAsDouble());
  EXPECT_EQ("1", stringVariant.valueAsString());

  EXPECT_THROW(boolVariant.valueAsInteger(), openstudio::Exception);
  EXPECT_THROW(boolVariant.valueAsDouble(), openstudio::Exception);
  EXPECT_THROW(boolVariant.valueAsString(), openstudio::Exception);

  EXPECT_THROW(intVariant.valueAsBoolean(), openstudio::Exception);
  //EXPECT_THROW(intVariant.valueAsDouble(), openstudio::Exception); // DLM: allowed this since double values in OSW might appear to be ints
  EXPECT_THROW(intVariant.valueAsString(), openstudio::Exception);

  EXPECT_THROW(doubleVariant.valueAsBoolean(), openstudio::Exception);
  EXPECT_THROW(doubleVariant.valueAsInteger(), openstudio::Exception);
  EXPECT_THROW(doubleVariant.valueAsString(), openstudio::Exception);

  EXPECT_THROW(stringVariant.valueAsBoolean(), openstudio::Exception);
  EXPECT_THROW(stringVariant.valueAsInteger(), openstudio::Exception);
  EXPECT_THROW(stringVariant.valueAsDouble(), openstudio::Exception);
}

TEST_F(DataFixture, Variant_valueAsJSON) {
  {
    const Variant boolVariant(true);
    auto boolJson = boolVariant.valueAsJSON();
    ASSERT_TRUE(boolJson.isBool());
    EXPECT_TRUE(boolJson.asBool());
  }

  {
    const Variant intVariant(1);
    auto intJson = intVariant.valueAsJSON();
    ASSERT_TRUE(intJson.isInt());
    EXPECT_EQ(1, intJson.asInt());
  }

  {
    const Variant doubleVariant(1.0);
    auto doubleJson = doubleVariant.valueAsJSON();
    ASSERT_TRUE(doubleJson.isDouble());
    EXPECT_DOUBLE_EQ(1, doubleJson.asDouble());
  }

  {
    const Variant stringVariant("1");
    auto stringJson = stringVariant.valueAsJSON();
    ASSERT_TRUE(stringJson.isString());
    EXPECT_EQ("1", stringJson.asString());
  }
}
