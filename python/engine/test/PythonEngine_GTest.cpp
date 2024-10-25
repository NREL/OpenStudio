/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "measure/ModelMeasure.hpp"
#include "measure/OSArgument.hpp"
#include "measure/OSMeasure.hpp"
#include "measure/OSRunner.hpp"
#include "model/Model.hpp"
#include "scriptengine/ScriptEngine.hpp"

#include <fmt/format.h>

class PythonEngineFixture : public testing::Test
{
 public:
  static openstudio::path getScriptPath(const std::string& classAndDirName) {
    openstudio::path scriptPath = openstudio::getApplicationSourceDirectory() / fmt::format("python/engine/test/{}/measure.py", classAndDirName);
    OS_ASSERT(openstudio::filesystem::is_regular_file(scriptPath));
    return scriptPath;
  }

 protected:
  // initialize for each test
  virtual void SetUp() override {
    std::vector<std::string> args;
    thisEngine = std::make_unique<openstudio::ScriptEngineInstance>("pythonengine", args);

    (*thisEngine)->registerType<openstudio::measure::OSMeasure*>("openstudio::measure::OSMeasure *");
    (*thisEngine)->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  }
  // tear down after each test
  virtual void TearDown() override {
    // Want to ensure the engine is reset regardless of the test outcome, or it may throw a segfault
    thisEngine->reset();
  }

  std::unique_ptr<openstudio::ScriptEngineInstance> thisEngine;
};

TEST_F(PythonEngineFixture, BadMeasure) {

  const std::string classAndDirName = "BadMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Bad Measure");

  std::string expected_exception = fmt::format(R"(SWIG director method error. In method 'arguments': `ValueError('oops')`

Traceback (most recent call last):
  File "{0}", line 17, in arguments
    self.another_method()
  File "{0}", line 14, in another_method
    raise ValueError("oops")
ValueError: oops)",
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

TEST_F(PythonEngineFixture, WrongMethodMeasure) {

  const std::string classAndDirName = "WrongMethodMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Wrong Method Measure");

  std::string expected_exception =
    fmt::format(R"(SWIG director method error. In method 'arguments': `AttributeError("'Model' object has no attribute 'nonExistingMethod'")`

Traceback (most recent call last):
  File "{}", line 14, in arguments
    model.nonExistingMethod()
    ^^^^^^^^^^^^^^^^^^^^^^^
AttributeError: 'Model' object has no attribute 'nonExistingMethod')",
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

TEST_F(PythonEngineFixture, StackLevelTooDeepMeasure) {

  const std::string classAndDirName = "StackLevelTooDeepMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Stack Level Too Deep Measure");

  std::string expected_exception =
    fmt::format(R"(SWIG director method error. In method 'arguments': `RecursionError('maximum recursion depth exceeded')`

Traceback (most recent call last):
  File "{0}", line 16, in arguments
    s(10)
  File "{0}", line 6, in s
    return s(x)
           ^^^^
  File "{0}", line 6, in s
    return s(x)
           ^^^^
  File "{0}", line 6, in s
    return s(x)
           ^^^^
  [Previous line repeated 996 more times]
RecursionError: maximum recursion depth exceeded)",
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

TEST_F(PythonEngineFixture, AlfalfaMeasure) {
  const std::string classAndDirName = "AlfalfaMeasure";

  const auto scriptPath = getScriptPath(classAndDirName);
  auto measureScriptObject = (*thisEngine)->loadMeasure(scriptPath, classAndDirName);
  auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

  ASSERT_EQ(measurePtr->name(), "Alfalfa Measure");

  std::string workflow_json = R"json(
{
  "seed": "../seed.osm",
  "weather_file": "../weather.epw",
  "steps": [
    {
      "arguments": {},
      "description": "The method attempts to build an alfalfa json in the measure",
      "measure_dir_name": "AlfalfaMeasure",
      "modeler_description": "The method attempts to build an alfalfa json in the measure",
      "name": "AlfalfaMeasure"
    }
  ]
}
  )json";

  openstudio::model::Model model;
  openstudio::WorkflowJSON workflow = *openstudio::WorkflowJSON::load(workflow_json);
  openstudio::measure::OSRunner runner(workflow);
  EXPECT_TRUE(runner.alfalfa().points().empty());

  openstudio::measure::OSArgumentMap arguments;
  measurePtr->run(model, runner, arguments);
  EXPECT_EQ(5, runner.alfalfa().points().size());
}
