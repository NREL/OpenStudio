#include "measure/ModelMeasure.hpp"
#include "measure/OSArgument.hpp"
#include "measure/OSMeasure.hpp"
#include "model/Model.hpp"
#include "scriptengine/ScriptEngine.hpp"
#include <gtest/gtest.h>

class RubyEngineFixture : public testing::Test {};

TEST_F(RubyEngineFixture, Demo) {
  std::vector<std::string> args;

  openstudio::ScriptEngineInstance rubyEngine("rubyengine", args);

  rubyEngine->registerType<openstudio::measure::OSMeasure *>("openstudio::measure::OSMeasure *");
  rubyEngine->registerType<openstudio::measure::ModelMeasure *>("openstudio::measure::ModelMeasure *");
  rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");

  const std::string className = "BadMeasure";
  const openstudio::path scriptPath = openstudio::getApplicationSourceDirectory() /
      "ruby/engine/test/BadMeasure/measure.rb";

  auto measureScriptObject = rubyEngine->loadMeasure(scriptPath, className);
  auto measure = rubyEngine->getAs<openstudio::measure::ModelMeasure *>(measureScriptObject);

  ASSERT_EQ(measure->name(), "Bad Measure");

  openstudio::model::Model model;
  ASSERT_ANY_THROW(measure->arguments(model));
}
