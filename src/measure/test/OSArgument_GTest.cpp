/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "MeasureFixture.hpp"

#include "../OSArgument.hpp"

#include <json/value.h>
#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::measure;

TEST_F(MeasureFixture, OSArgument_Clone) {

  std::vector<std::string> choices;
  choices.push_back("On");
  choices.push_back("Off");

  OSArgument boolArgument = OSArgument::makeBoolArgument("bool");
  OSArgument doubleArgument = OSArgument::makeDoubleArgument("double");
  OSArgument integerArgument = OSArgument::makeIntegerArgument("integer");
  OSArgument stringArgument = OSArgument::makeStringArgument("string");
  OSArgument choiceArgument = OSArgument::makeChoiceArgument("choice", choices);

  EXPECT_FALSE(boolArgument.hasValue());
  EXPECT_FALSE(doubleArgument.hasValue());
  EXPECT_FALSE(integerArgument.hasValue());
  EXPECT_FALSE(stringArgument.hasValue());
  EXPECT_FALSE(choiceArgument.hasValue());

  EXPECT_TRUE(boolArgument.setValue(true));
  EXPECT_TRUE(doubleArgument.setValue(1.0));
  EXPECT_TRUE(doubleArgument.setValue((int)1));  // can also set double arg using int
  EXPECT_TRUE(integerArgument.setValue((int)1));
  EXPECT_TRUE(stringArgument.setValue(std::string("value")));
  EXPECT_TRUE(choiceArgument.setValue(std::string("On")));

  ASSERT_TRUE(boolArgument.hasValue());
  ASSERT_TRUE(doubleArgument.hasValue());
  ASSERT_TRUE(integerArgument.hasValue());
  ASSERT_TRUE(stringArgument.hasValue());
  ASSERT_TRUE(choiceArgument.hasValue());

  EXPECT_EQ(true, boolArgument.valueAsBool());
  EXPECT_EQ(1.0, doubleArgument.valueAsDouble());
  EXPECT_EQ(1, integerArgument.valueAsInteger());
  EXPECT_EQ("value", stringArgument.valueAsString());
  EXPECT_EQ("On", choiceArgument.valueAsString());

  OSArgument boolArgument2 = boolArgument.clone();
  OSArgument doubleArgument2 = doubleArgument.clone();
  OSArgument integerArgument2 = integerArgument.clone();
  OSArgument stringArgument2 = stringArgument.clone();
  OSArgument choiceArgument2 = choiceArgument.clone();

  ASSERT_TRUE(boolArgument2.hasValue());
  ASSERT_TRUE(doubleArgument2.hasValue());
  ASSERT_TRUE(integerArgument2.hasValue());
  ASSERT_TRUE(stringArgument2.hasValue());
  ASSERT_TRUE(choiceArgument2.hasValue());

  EXPECT_EQ(true, boolArgument2.valueAsBool());
  EXPECT_EQ(1.0, doubleArgument2.valueAsDouble());
  EXPECT_EQ(1, integerArgument2.valueAsInteger());
  EXPECT_EQ("value", stringArgument2.valueAsString());
  EXPECT_EQ("On", choiceArgument2.valueAsString());

  std::vector<OSArgument> argumentVector;
  argumentVector.push_back(boolArgument);
  argumentVector.push_back(doubleArgument);
  argumentVector.push_back(integerArgument);
  argumentVector.push_back(stringArgument);
  argumentVector.push_back(choiceArgument);
  std::map<std::string, OSArgument> argumentMap = convertOSArgumentVectorToMap(argumentVector);

  ASSERT_FALSE(argumentMap.find("bool") == argumentMap.end());
  ASSERT_FALSE(argumentMap.find("double") == argumentMap.end());
  ASSERT_FALSE(argumentMap.find("integer") == argumentMap.end());
  ASSERT_FALSE(argumentMap.find("string") == argumentMap.end());
  ASSERT_FALSE(argumentMap.find("choice") == argumentMap.end());

  ASSERT_TRUE(argumentMap.find("bool")->second.hasValue());
  ASSERT_TRUE(argumentMap.find("double")->second.hasValue());
  ASSERT_TRUE(argumentMap.find("integer")->second.hasValue());
  ASSERT_TRUE(argumentMap.find("string")->second.hasValue());
  ASSERT_TRUE(argumentMap.find("choice")->second.hasValue());

  EXPECT_EQ(true, argumentMap.find("bool")->second.valueAsBool());
  EXPECT_EQ(1.0, argumentMap.find("double")->second.valueAsDouble());
  EXPECT_EQ(1, argumentMap.find("integer")->second.valueAsInteger());
  EXPECT_EQ("value", argumentMap.find("string")->second.valueAsString());
  EXPECT_EQ("On", argumentMap.find("choice")->second.valueAsString());
}

TEST_F(MeasureFixture, OSArgument_ClearValue) {
  std::vector<std::string> choices;
  choices.push_back("On");
  choices.push_back("Off");

  OSArgument boolArgument = OSArgument::makeBoolArgument("bool");
  OSArgument doubleArgument = OSArgument::makeDoubleArgument("double");
  OSArgument integerArgument = OSArgument::makeIntegerArgument("integer");
  OSArgument stringArgument = OSArgument::makeStringArgument("string");
  OSArgument choiceArgument = OSArgument::makeChoiceArgument("choice", choices);

  EXPECT_TRUE(boolArgument.setValue(true));
  EXPECT_TRUE(doubleArgument.setValue(1.0));
  EXPECT_TRUE(doubleArgument.setValue((int)1));  // can also set double arg using int
  EXPECT_TRUE(integerArgument.setValue((int)1));
  EXPECT_TRUE(stringArgument.setValue(std::string("value")));
  EXPECT_TRUE(choiceArgument.setValue(std::string("On")));

  ASSERT_TRUE(boolArgument.hasValue());
  ASSERT_TRUE(doubleArgument.hasValue());
  ASSERT_TRUE(integerArgument.hasValue());
  ASSERT_TRUE(stringArgument.hasValue());
  ASSERT_TRUE(choiceArgument.hasValue());

  boolArgument.clearValue();
  doubleArgument.clearValue();
  integerArgument.clearValue();
  stringArgument.clearValue();
  choiceArgument.clearValue();

  EXPECT_FALSE(boolArgument.hasValue());
  EXPECT_FALSE(doubleArgument.hasValue());
  EXPECT_FALSE(integerArgument.hasValue());
  EXPECT_FALSE(stringArgument.hasValue());
  EXPECT_FALSE(choiceArgument.hasValue());

  EXPECT_ANY_THROW(boolArgument.valueAsBool());
  EXPECT_ANY_THROW(doubleArgument.valueAsDouble());
  EXPECT_ANY_THROW(integerArgument.valueAsInteger());
  EXPECT_ANY_THROW(stringArgument.valueAsString());
  EXPECT_ANY_THROW(choiceArgument.valueAsString());
}

TEST_F(MeasureFixture, OSArgument_Domain) {

  OSArgument doubleArg = OSArgument::makeDoubleArgument("double", true);
  OSArgument integerArg = OSArgument::makeIntegerArgument("integer", true);
  OSArgument stringArg = OSArgument::makeStringArgument("string", true);

  EXPECT_FALSE(doubleArg.hasDomain());
  EXPECT_FALSE(integerArg.hasDomain());
  EXPECT_FALSE(stringArg.hasDomain());

  double d = 0.0;
  int i = 0;

  EXPECT_TRUE(doubleArg.setMinValue(d));
  EXPECT_TRUE(integerArg.setMinValue(i));
  EXPECT_FALSE(stringArg.setMinValue(0.0));

  ASSERT_TRUE(doubleArg.hasDomain());
  ASSERT_TRUE(integerArg.hasDomain());
  EXPECT_FALSE(stringArg.hasDomain());

  EXPECT_EQ(0.0, doubleArg.domainAsDouble()[0]);
  EXPECT_EQ(std::numeric_limits<double>::max(), doubleArg.domainAsDouble()[1]);
  EXPECT_EQ(0, integerArg.domainAsInteger()[0]);
  EXPECT_EQ(std::numeric_limits<int>::max(), integerArg.domainAsInteger()[1]);

  d = 1.0;
  i = 1;

  EXPECT_TRUE(doubleArg.setMaxValue(i));
  EXPECT_TRUE(integerArg.setMaxValue(d));
  EXPECT_FALSE(stringArg.setMaxValue(1));

  ASSERT_TRUE(doubleArg.hasDomain());
  ASSERT_TRUE(integerArg.hasDomain());
  EXPECT_FALSE(stringArg.hasDomain());

  EXPECT_EQ(0.0, doubleArg.domainAsDouble()[0]);
  EXPECT_EQ(1.0, doubleArg.domainAsDouble()[1]);
  EXPECT_EQ(0, integerArg.domainAsInteger()[0]);
  EXPECT_EQ(1, integerArg.domainAsInteger()[1]);

  // the domain is not currently used to validate these values
  EXPECT_TRUE(doubleArg.setValue(-1.0));
  EXPECT_TRUE(doubleArg.setValue(0.0));
  EXPECT_TRUE(doubleArg.setValue(0.5));
  EXPECT_TRUE(doubleArg.setValue(1.0));
  EXPECT_TRUE(doubleArg.setValue(2.0));

  EXPECT_TRUE(integerArg.setValue(-1));
  EXPECT_TRUE(integerArg.setValue(0));
  EXPECT_TRUE(integerArg.setValue(1));
  EXPECT_TRUE(integerArg.setValue(2));
}

TEST_F(MeasureFixture, OSArgument_MakeChoiceArgumentFromMap) {
  const std::map<std::string, std::string> choices_to_display_values_map{
    {"handle1", "choice1"},
    {"handle2", "choice2"},
  };
  const bool required = true;
  const bool modelDependent = true;
  OSArgument choiceArgument = OSArgument::makeChoiceArgument("choice", choices_to_display_values_map, required, modelDependent);
  ASSERT_EQ(2, choiceArgument.choiceValues().size());
  EXPECT_EQ("handle1", choiceArgument.choiceValues().front());
  EXPECT_EQ("handle2", choiceArgument.choiceValues().back());

  ASSERT_EQ(2, choiceArgument.choiceValueDisplayNames().size());
  EXPECT_EQ("choice1", choiceArgument.choiceValueDisplayNames().front());
  EXPECT_EQ("choice2", choiceArgument.choiceValueDisplayNames().back());

  EXPECT_EQ(required, choiceArgument.required());
  EXPECT_EQ(modelDependent, choiceArgument.modelDependent());
}

TEST_F(MeasureFixture, OSArgument_valueAsJSON) {

  {
    OSArgument boolArgument = OSArgument::makeBoolArgument("bool");
    Json::Value valNotAssigned = boolArgument.valueAsJSON();
    EXPECT_TRUE(valNotAssigned.isNull());
    boolArgument.setValue(true);
    Json::Value val = boolArgument.valueAsJSON();
    EXPECT_TRUE(val.isBool());
    EXPECT_TRUE(val.asBool());
  }

  {
    OSArgument doubleArgument = OSArgument::makeDoubleArgument("double");
    Json::Value valNotAssigned = doubleArgument.valueAsJSON();
    EXPECT_TRUE(valNotAssigned.isNull());
    doubleArgument.setValue(10.0);
    Json::Value val = doubleArgument.valueAsJSON();
    EXPECT_TRUE(val.isDouble());
    EXPECT_DOUBLE_EQ(10.0, val.asDouble());
  }

  {
    OSArgument integerArgument = OSArgument::makeIntegerArgument("integer");
    Json::Value valNotAssigned = integerArgument.valueAsJSON();
    EXPECT_TRUE(valNotAssigned.isNull());
    integerArgument.setValue(10);
    Json::Value val = integerArgument.valueAsJSON();
    EXPECT_TRUE(val.isInt());
    EXPECT_EQ(10, val.asInt());
  }

  {
    OSArgument stringArgument = OSArgument::makeStringArgument("string");
    Json::Value valNotAssigned = stringArgument.valueAsJSON();
    EXPECT_TRUE(valNotAssigned.isNull());
    stringArgument.setValue("hello");
    Json::Value val = stringArgument.valueAsJSON();
    EXPECT_TRUE(val.isString());
    EXPECT_EQ("hello", val.asString());
  }

  {
    const std::map<std::string, std::string> choices_to_display_values_map{
      {"handle1", "choice1"},
      {"handle2", "choice2"},
    };
    OSArgument choiceArgument = OSArgument::makeChoiceArgument("choice", choices_to_display_values_map);
    Json::Value valNotAssigned = choiceArgument.valueAsJSON();
    EXPECT_TRUE(valNotAssigned.isNull());
    choiceArgument.setValue("handle1");
    Json::Value val = choiceArgument.valueAsJSON();
    EXPECT_TRUE(val.isString());
    EXPECT_EQ("handle1", val.asString());
  }
}
