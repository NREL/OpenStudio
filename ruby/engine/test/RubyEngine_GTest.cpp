/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "measure/ModelMeasure.hpp"
#include "measure/OSArgument.hpp"
#include "measure/OSMeasure.hpp"
#include "model/Model.hpp"
#include "scriptengine/ScriptEngine.hpp"

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
    static std::regex object_address_re("0x[[:alnum:]]*>");

    return std::regex_replace(error_message, object_address_re, "ADDRESS>");
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

  const std::string classAndDirName = "BadMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Bad Measure");

  std::string expected_exception = fmt::format(R"(SWIG director method error. RuntimeError: oops
["/Users/julien/Software/Others/OpenStudio2/ruby/engine/test/BadMeasure/measure.rb:12:in `another_method'", "/Users/julien/Software/Others/OpenStudio2/ruby/engine/test/BadMeasure/measure.rb:16:in `arguments'"]

Traceback:
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
}

TEST_F(RubyEngineFixture, WrongMethodMeasure) {

  const std::string classAndDirName = "WrongMethodMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Wrong Method Measure");

  std::string expected_exception =
    fmt::format(R"(SWIG director method error. NoMethodError: undefined method `nonExistingMethod' for #<OpenStudio::Model::Model:ADDRESS>
["/Users/julien/Software/Others/OpenStudio2/ruby/engine/test/WrongMethodMeasure/measure.rb:12:in `arguments'"]

Traceback:
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
}
