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

class RubyEngineFixture : public testing::Test
{
 public:
  static openstudio::path getScriptPath(const std::string& classAndDirName) {
    openstudio::path scriptPath = openstudio::getApplicationSourceDirectory() / fmt::format("ruby/engine/test/{}/measure.rb", classAndDirName);
    OS_ASSERT(openstudio::filesystem::is_regular_file(scriptPath));
    return scriptPath;
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

  openstudio::model::Model model;
  ASSERT_ANY_THROW(measurePtr->arguments(model));
}

TEST_F(RubyEngineFixture, WrongMethodMeasure) {

  const std::string classAndDirName = "WrongMethodMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Wrong Method Measure");

  openstudio::model::Model model;
  ASSERT_ANY_THROW(measurePtr->arguments(model));
}
