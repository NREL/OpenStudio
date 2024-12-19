/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../../../src/measure/ModelMeasure.hpp"
#include "../../../src/measure/OSArgument.hpp"
#include "../../../src/measure/OSMeasure.hpp"
#include "../../../src/model/Model.hpp"
#include "../../../src/scriptengine/ScriptEngine.hpp"

#include <fmt/format.h>

#include <regex>

class RubyEngineFixture : public testing::Test
{
 public:
  static openstudio::path getScriptPath(const std::string& classAndDirName) {
    openstudio::path scriptPath = openstudio::getApplicationSourceDirectory() / fmt::format("ruby/engine/test/{}/measure.rb", classAndDirName);
    OS_ASSERT(openstudio::filesystem::is_regular_file(scriptPath));
    return scriptPath;
  }

  static std::string stripAddressFromErrorMessage(const std::string& error_message) {
    static std::regex object_address_re("0x[[:alnum:]]* @__swigtype__=\"_p_openstudio__model__Model\">");

    return std::regex_replace(error_message, object_address_re, "ADDRESS>");
  }

  static std::string stripNumLevels(const std::string& error_message) {
    static std::regex num_levels_re("[[:alnum:]]+ levels");

    return std::regex_replace(error_message, num_levels_re, "<NUM_LEVELS> levels");
  }

 protected:
  // initialize for each test
  virtual void SetUp() override {
    std::vector<std::string> args;
    thisEngine = std::make_unique<openstudio::ScriptEngineInstance>("rubyengine", args);

    (*thisEngine)->registerType<openstudio::measure::OSMeasure*>("openstudio::measure::OSMeasure *");
    (*thisEngine)->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
    (*thisEngine)->exec("OpenStudio::init_rest_of_openstudio()");
  }

  // tear down after each test
  virtual void TearDown() override {
    // Want to ensure the engine is reset regardless of the test outcome, or it may throw a segfault
    thisEngine->reset();
  }

  std::unique_ptr<openstudio::ScriptEngineInstance> thisEngine;
};

TEST_F(RubyEngineFixture, BadMeasure) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_DEATH(
    {
      const std::string classAndDirName = "BadMeasure";

      const auto scriptPath = getScriptPath(classAndDirName);
      auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
      auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

      ASSERT_EQ(measurePtr->name(), "Bad Measure");

      std::string expected_exception = fmt::format(R"(SWIG director method error. RuntimeError: oops

Traceback (most recent call last):
{0}:12:in `another_method'
{0}:16:in `arguments')",
                                                   scriptPath.generic_string());

      openstudio::model::Model model;

      try {
        measurePtr->arguments(model);
        ASSERT_FALSE(true) << "Expected measure arguments(model) to throw";
      } catch (std::exception& e) {
        std::string error = e.what();
        EXPECT_EQ(expected_exception, error);
      }

      thisEngine->reset();
    },
    "oops");
}

TEST_F(RubyEngineFixture, WrongMethodMeasure) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_DEATH(
    {
      const std::string classAndDirName = "WrongMethodMeasure";

      const auto scriptPath = getScriptPath(classAndDirName);
      auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
      auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

      ASSERT_EQ(measurePtr->name(), "Wrong Method Measure");

      std::string expected_exception =
        fmt::format(R"(SWIG director method error. NoMethodError: undefined method `nonExistingMethod' for #<OpenStudio::Model::Model:ADDRESS>

Traceback (most recent call last):
{0}:12:in `arguments')",
                    scriptPath.generic_string());

      openstudio::model::Model model;
      try {
        measurePtr->arguments(model);
        ASSERT_FALSE(true) << "Expected measure arguments(model) to throw";
      } catch (std::exception& e) {
        std::string error = e.what();
        EXPECT_EQ(expected_exception, stripAddressFromErrorMessage(error));
      }

      thisEngine->reset();
    },
    "undefined method `nonExistingMethod'");
}

TEST_F(RubyEngineFixture, StackLevelTooDeepMeasure) {

  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_DEATH(
    {
      const std::string classAndDirName = "StackLevelTooDeepMeasure";

      const auto scriptPath = getScriptPath(classAndDirName);
      auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
      auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

      ASSERT_EQ(measurePtr->name(), "Stack Level Too Deep Measure");

      std::string expected_exception = fmt::format(R"(SWIG director method error. SystemStackError: stack level too deep

Traceback (most recent call last):
{0}:16:in `arguments'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
	... <NUM_LEVELS> levels...
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s'
{0}:12:in `s')",
                                                   scriptPath.generic_string());

      openstudio::model::Model model;
      try {
        measurePtr->arguments(model);
        ASSERT_FALSE(true) << "Expected measure arguments(model) to throw";
      } catch (std::exception& e) {
        std::string error = e.what();
        EXPECT_EQ(expected_exception, stripNumLevels(error));
      }
      thisEngine->reset();
    },
    "stack level too deep");
}
