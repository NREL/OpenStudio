/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "MeasureFixture.hpp"

#include "../OSRunner.hpp"
#include "../OSMeasure.hpp"
#include "../ModelMeasure.hpp"
#include "../OSArgument.hpp"

#include "../../model/Model.hpp"
#include "../../model/BoilerHotWater.hpp"

#include "../../utilities/filetypes/WorkflowJSON.hpp"
#include "../../utilities/filetypes/WorkflowStep.hpp"
#include "../../utilities/filetypes/WorkflowStepResult.hpp"

#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::measure;

class TestModelMeasure : public ModelMeasure
{
 public:
  virtual std::string name() const override {
    return "TestModelMeasure";
  }

  virtual bool run(model::Model& model, OSRunner& runner, const std::map<std::string, OSArgument>& user_arguments) const override {
    ModelMeasure::run(model, runner, user_arguments);
    return true;
  }
};

TEST_F(MeasureFixture, OSRunner_StdOut) {

  MeasureStep step("TestModelMeasure");

  std::vector<WorkflowStep> steps;
  steps.push_back(step);

  WorkflowJSON workflow;
  workflow.setWorkflowSteps(steps);

  OSRunner runner(workflow);

  std::cout << "Hi Output" << '\n';
  std::cerr << "Hi Error" << '\n';

  TestModelMeasure measure;
  runner.prepareForMeasureRun(measure);

  std::cout << "Standard Output" << '\n';
  std::cerr << "Standard Error" << '\n';

  runner.incrementStep();

  std::cout << "Bye Output" << '\n';
  std::cerr << "Bye Error" << '\n';

  ASSERT_TRUE(step.result());
  ASSERT_TRUE(step.result()->stdOut());
  EXPECT_EQ("Standard Output\n", step.result()->stdOut().get());
  ASSERT_TRUE(step.result()->stdErr());
  EXPECT_EQ("Standard Error\n", step.result()->stdErr().get());
}

TEST_F(MeasureFixture, OSRunner_getOptionalBoolArgumentValue) {

  WorkflowJSON workflow;
  OSRunner runner(workflow);

  constexpr bool required = false;
  constexpr auto arg_name = "heat_pump_is_ducted";

  std::vector<boost::optional<bool>> tests{
    boost::optional<bool>(),
    boost::optional<bool>(true),
    boost::optional<bool>(false),
  };

  for (const auto& test : tests) {
    std::vector<OSArgument> argumentVector;
    OSArgument boolArgument = OSArgument::makeBoolArgument(arg_name, required);
    if (test.has_value()) {
      boolArgument.setValue(test.get());
    }
    argumentVector.push_back(boolArgument);

    std::map<std::string, OSArgument> argumentMap = convertOSArgumentVectorToMap(argumentVector);

    boost::optional<bool> result_ = runner.getOptionalBoolArgumentValue(arg_name, argumentMap);
    if (!test.has_value()) {
      EXPECT_FALSE(result_.has_value());
    } else {
      ASSERT_TRUE(result_.has_value());
      EXPECT_EQ(test.get(), result_.get());
    }
  }
}

TEST_F(MeasureFixture, OSRunner_getArgumentValues) {
  WorkflowJSON workflow;
  OSRunner runner(workflow);

  model::Model m;
  model::BoilerHotWater boiler1(m);
  model::BoilerHotWater boiler2(m);

  std::vector<OSArgument> argumentVector;

  OSArgument requiredBoolArgument = OSArgument::makeBoolArgument("required_bool", true);
  EXPECT_TRUE(requiredBoolArgument.setValue(true));
  argumentVector.push_back(requiredBoolArgument);

  OSArgument requiredBoolArgument2 = OSArgument::makeBoolArgument("required_bool2", true);
  EXPECT_TRUE(requiredBoolArgument2.setValue(false));
  argumentVector.push_back(requiredBoolArgument2);

  OSArgument optionalBoolArgument = OSArgument::makeBoolArgument("optional_bool", false);
  argumentVector.push_back(optionalBoolArgument);

  OSArgument requiredDoubleArgument = OSArgument::makeDoubleArgument("required_double", true);
  EXPECT_TRUE(requiredDoubleArgument.setValue(1.0));
  argumentVector.push_back(requiredDoubleArgument);

  OSArgument requiredDoubleArgument2 = OSArgument::makeDoubleArgument("required_double2", true);
  EXPECT_TRUE(requiredDoubleArgument2.setValue(1786.45));
  argumentVector.push_back(requiredDoubleArgument2);

  OSArgument optionalDoubleArgument = OSArgument::makeDoubleArgument("optional_double", false);
  argumentVector.push_back(optionalDoubleArgument);

  OSArgument optionalDoubleArgument2 = OSArgument::makeDoubleArgument("optional_double2", false);
  EXPECT_TRUE(optionalDoubleArgument2.setValue(10.5));
  argumentVector.push_back(optionalDoubleArgument2);

  OSArgument requiredIntegerArgument = OSArgument::makeIntegerArgument("required_integer", true);
  EXPECT_TRUE(requiredIntegerArgument.setValue(2));
  argumentVector.push_back(requiredIntegerArgument);

  OSArgument optionalIntegerArgument = OSArgument::makeDoubleArgument("optional_integer", false);
  argumentVector.push_back(optionalIntegerArgument);

  OSArgument requiredStringArgument = OSArgument::makeStringArgument("required_string", true);
  requiredStringArgument.setValue("Value");
  argumentVector.push_back(requiredStringArgument);

  OSArgument optionalStringArgument = OSArgument::makeStringArgument("optional_string", false);
  argumentVector.push_back(optionalStringArgument);

  std::vector<std::string> choices{"On", "Off"};
  OSArgument requiredChoiceArgument = OSArgument::makeChoiceArgument("required_choice", choices, true);
  requiredChoiceArgument.setValue("Off");
  argumentVector.push_back(requiredChoiceArgument);

  OSArgument optionalChoiceArgument = OSArgument::makeChoiceArgument("optional_choice", choices, false);
  argumentVector.push_back(optionalChoiceArgument);

  std::vector<std::string> choiceHandles;
  choiceHandles.push_back(openstudio::toString(boiler1.handle()));
  choiceHandles.push_back(openstudio::toString(boiler2.handle()));

  std::vector<std::string> displayNames;
  displayNames.push_back(boiler1.nameString());
  displayNames.push_back(boiler2.nameString());

  OSArgument optionalChoiceArgument2 = OSArgument::makeChoiceArgument("optional_choice2", choiceHandles, displayNames, false);
  optionalChoiceArgument2.setValue(openstudio::toString(boiler2.handle()));
  argumentVector.push_back(optionalChoiceArgument2);

  std::map<std::string, OSArgument> argumentMap = convertOSArgumentVectorToMap(argumentVector);

  // Create an extra argument that is just in the argumentMap and wouldn't be in the original measure argument vector
  OSArgument extraArgument = OSArgument::makeStringArgument("an_extra_argument_not_expected", false);
  argumentMap.emplace("an_extra_argument_not_expected", extraArgument);

  const bool b = runner.getBoolArgumentValue(requiredBoolArgument.name(), argumentMap);
  EXPECT_TRUE(b);

  const bool b2 = runner.getBoolArgumentValue(requiredBoolArgument2.name(), argumentMap);
  EXPECT_FALSE(b2);

  const double d = runner.getDoubleArgumentValue(requiredDoubleArgument.name(), argumentMap);
  EXPECT_EQ(1.0, d);

  const double d2 = runner.getDoubleArgumentValue(requiredDoubleArgument2.name(), argumentMap);
  EXPECT_DOUBLE_EQ(1786.45, d2);

  const int i = runner.getIntegerArgumentValue(requiredIntegerArgument.name(), argumentMap);
  EXPECT_EQ(2, i);

  const std::string s = runner.getStringArgumentValue(requiredStringArgument.name(), argumentMap);
  EXPECT_EQ("Value", s);

  const std::string c = runner.getStringArgumentValue(requiredChoiceArgument.name(), argumentMap);
  EXPECT_EQ("Off", c);

  const boost::optional<openstudio::WorkspaceObject> w = runner.getOptionalWorkspaceObjectChoiceValue(optionalChoiceArgument2.name(), argumentMap, m);
  EXPECT_TRUE(w);

  Json::Value argumentValues = runner.getArgumentValues(argumentVector, argumentMap);

  EXPECT_FALSE(argumentValues["required_bool"].isNull());
  EXPECT_TRUE(argumentValues["required_bool"].asBool());
  EXPECT_FALSE(argumentValues["required_bool2"].isNull());
  EXPECT_FALSE(argumentValues["required_bool2"].asBool());
  EXPECT_TRUE(argumentValues["optional_bool"].isNull());

  EXPECT_EQ(1.0, argumentValues["required_double"].asDouble());
  EXPECT_DOUBLE_EQ(1786.45, argumentValues["required_double2"].asDouble());
  EXPECT_TRUE(argumentValues["optional_double"].isNull());
  EXPECT_FALSE(argumentValues["optional_double2"].isNull());
  EXPECT_EQ(10.5, argumentValues["optional_double2"].asDouble());

  EXPECT_EQ(2, argumentValues["required_integer"].asInt());
  EXPECT_TRUE(argumentValues["optional_integer"].isNull());

  EXPECT_EQ("Value", argumentValues["required_string"].asString());
  EXPECT_TRUE(argumentValues["optional_string"].isNull());

  EXPECT_EQ("Off", argumentValues["required_choice"].asString());
  EXPECT_TRUE(argumentValues["optional_choice"].isNull());
  EXPECT_EQ(openstudio::toString(boiler2.handle()), argumentValues["optional_choice2"].asString());

  EXPECT_FALSE(argumentValues.isMember("an_extra_argument_not_expected"));
}

TEST_F(MeasureFixture, OSRunner_getPastStepValues) {
  WorkflowJSON workflow;
  OSRunner runner(workflow);

  std::vector<WorkflowStep> workflow_steps;

  MeasureStep step1("Step1");
  step1.setArgument("Argument1", 100);
  step1.setArgument("Argument2", 200);
  workflow_steps.push_back(step1);
  WorkflowStepResult workflow_step_result1;
  EXPECT_FALSE(workflow_step_result1.measureName());
  workflow_step_result1.setMeasureName("MeasureName1");
  ASSERT_TRUE(workflow_step_result1.measureName());
  EXPECT_EQ("MeasureName1", workflow_step_result1.measureName().get());
  WorkflowStepValue stepValue1("StepValue1", true);
  workflow_step_result1.addStepValue(stepValue1);
  WorkflowStepValue stepValue2("StepValue2", 5);
  workflow_step_result1.addStepValue(stepValue2);

  MeasureStep step2("Step2");
  step2.setArgument("Argument3", 300);
  step2.setArgument("Argument4", 400);
  workflow_steps.push_back(step2);
  WorkflowStepResult workflow_step_result2;
  EXPECT_FALSE(workflow_step_result2.measureName());
  workflow_step_result2.setMeasureName("MeasureName2");
  ASSERT_TRUE(workflow_step_result2.measureName());
  EXPECT_EQ("MeasureName2", workflow_step_result2.measureName().get());
  WorkflowStepValue stepValue3("StepValue3", 342.3);
  workflow_step_result2.addStepValue(stepValue3);
  EXPECT_EQ(342.3, stepValue3.valueAsDouble());
  WorkflowStepValue stepValue4("StepValue4", false);
  workflow_step_result2.addStepValue(stepValue4);
  EXPECT_FALSE(stepValue4.valueAsBoolean());
  EXPECT_EQ(2, workflow_step_result2.stepValues().size());
  EXPECT_TRUE(workflow.setWorkflowSteps(workflow_steps));
  EXPECT_EQ(2, workflow.workflowSteps().size());
  EXPECT_EQ(workflow.string(), runner.workflow().string());
  workflow_step_result2.setStepResult(StepResult::Success);
  step2.setResult(workflow_step_result2);
  ASSERT_TRUE(step2.result());
  WorkflowStepResult workflow_step_result2_ = step2.result().get();
  ASSERT_TRUE(workflow_step_result2_.stepResult());
  EXPECT_EQ(workflow_step_result2.string(), workflow_step_result2_.string());
  ASSERT_TRUE(workflow_step_result2.stepResult());
  EXPECT_EQ(StepResult::Success, workflow_step_result2.stepResult().get());

  Json::Value stepValues;

  stepValues = runner.getPastStepValuesForMeasure("MeasureName1");
  EXPECT_EQ(0, stepValues.size());  // did not set step result

  stepValues = runner.getPastStepValuesForMeasure("MeasureName2");
  EXPECT_EQ(2, stepValues.size());

  EXPECT_TRUE(stepValues["Argument3"].isNull());
  EXPECT_TRUE(stepValues["Argument4"].isNull());
  EXPECT_TRUE(stepValues["StepValue1"].isNull());
  EXPECT_TRUE(stepValues["StepValue2"].isNull());
  EXPECT_FALSE(stepValues["StepValue3"].isNull());
  EXPECT_EQ(342.3, stepValues["StepValue3"].asDouble());
  EXPECT_FALSE(stepValues["StepValue4"].isNull());
  EXPECT_FALSE(stepValues["StepValue4"].asBool());

  MeasureStep step3("Step3");
  workflow_steps.push_back(step3);
  WorkflowStepResult workflow_step_result3;
  workflow_step_result3.setMeasureName("MeasureName3");
  workflow_step_result3.setStepResult(StepResult::Success);
  step3.setResult(workflow_step_result3);
  WorkflowStepValue stepValue5("StepValue3", 20);
  workflow_step_result3.addStepValue(stepValue5);
  EXPECT_TRUE(workflow.setWorkflowSteps(workflow_steps));
  EXPECT_EQ(3, workflow.workflowSteps().size());

  stepValues = runner.getPastStepValuesForName("StepValue1");
  EXPECT_EQ(0, stepValues.size());  // did not set step result

  stepValues = runner.getPastStepValuesForName("StepValue3");
  EXPECT_EQ(2, stepValues.size());

  EXPECT_TRUE(stepValues["Step1"].isNull());
  EXPECT_TRUE(stepValues["Step2"].isNull());
  EXPECT_TRUE(stepValues["MeasureName1"].isNull());
  EXPECT_FALSE(stepValues["MeasureName2"].isNull());
  EXPECT_FALSE(stepValues["MeasureName3"].isNull());
  EXPECT_EQ(342.3, stepValues["MeasureName2"].asDouble());
  EXPECT_EQ(20, stepValues["MeasureName3"].asInt());
}
