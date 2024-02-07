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
  EXPECT_TRUE(requiredDoubleArgument2.setValue(234892384234.39485923845834534));
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

  std::vector<std::string> choices;
  choices.push_back("On");
  choices.push_back("Off");

  OSArgument requiredChoiceArgument = OSArgument::makeChoiceArgument("required_choice", choices, true);
  requiredChoiceArgument.setValue("Off");
  argumentVector.push_back(requiredChoiceArgument);

  OSArgument optionalChoiceArgument = OSArgument::makeChoiceArgument("optional_choice", choices, false);
  argumentVector.push_back(optionalChoiceArgument);

  std::map<std::string, OSArgument> argumentMap = convertOSArgumentVectorToMap(argumentVector);

  bool b = runner.getBoolArgumentValue(requiredBoolArgument.name(), argumentMap);
  EXPECT_TRUE(b);

  bool b2 = runner.getBoolArgumentValue(requiredBoolArgument2.name(), argumentMap);
  EXPECT_FALSE(b2);

  double d = runner.getDoubleArgumentValue(requiredDoubleArgument.name(), argumentMap);
  EXPECT_EQ(1.0, d);

  double d2 = runner.getDoubleArgumentValue(requiredDoubleArgument2.name(), argumentMap);
  EXPECT_EQ(234892384234.39485923845834534, d2);

  int i = runner.getIntegerArgumentValue(requiredIntegerArgument.name(), argumentMap);
  EXPECT_EQ(2, i);

  std::string s = runner.getStringArgumentValue(requiredStringArgument.name(), argumentMap);
  EXPECT_EQ("Value", s);

  std::string c = runner.getStringArgumentValue(requiredChoiceArgument.name(), argumentMap);
  EXPECT_EQ("Off", c);

  Json::Value argumentValues = runner.getArgumentValues(argumentVector, argumentMap);

  EXPECT_FALSE(argumentValues["required_bool"].isNull());
  EXPECT_TRUE(argumentValues["required_bool"].asBool());
  EXPECT_FALSE(argumentValues["required_bool2"].isNull());
  EXPECT_FALSE(argumentValues["required_bool2"].asBool());
  EXPECT_TRUE(argumentValues["optional_bool"].isNull());

  EXPECT_EQ(1.0, argumentValues["required_double"].asDouble());
  EXPECT_EQ(234892384234.39485923845834534, argumentValues["required_double2"].asDouble());
  EXPECT_TRUE(argumentValues["optional_double"].isNull());
  EXPECT_FALSE(argumentValues["optional_double2"].isNull());
  EXPECT_EQ(10.5, argumentValues["optional_double2"].asDouble());

  EXPECT_EQ(2, argumentValues["required_integer"].asInt());
  EXPECT_TRUE(argumentValues["optional_integer"].isNull());

  EXPECT_EQ("Value", argumentValues["required_string"].asString());
  EXPECT_TRUE(argumentValues["optional_string"].isNull());

  EXPECT_EQ("Off", argumentValues["required_choice"].asString());
  EXPECT_TRUE(argumentValues["optional_choice"].isNull());
}

TEST_F(MeasureFixture, OSRunner_getUpstreamMeasureArguments) {
  WorkflowJSON workflow;
  OSRunner runner(workflow);

  std::vector<WorkflowStep> workflow_steps;

  MeasureStep step1("Step1");
  step1.setArgument("Argument1", true);
  step1.setArgument("Argument2", 5);
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
  step2.setArgument("Argument3", 342.3);
  step2.setArgument("Argument4", false);
  workflow_steps.push_back(step2);
  WorkflowStepResult workflow_step_result2;
  EXPECT_FALSE(workflow_step_result2.measureName());
  workflow_step_result2.setMeasureName("MeasureName2");
  ASSERT_TRUE(workflow_step_result2.measureName());
  EXPECT_EQ("MeasureName2", workflow_step_result2.measureName().get());
  WorkflowStepValue stepValue3("StepValue3", 342.3);
  workflow_step_result2.addStepValue(stepValue3);
  EXPECT_EQ("342.300000", stepValue3.getValueAsString());
  WorkflowStepValue stepValue4("StepValue4", false);
  workflow_step_result2.addStepValue(stepValue4);
  EXPECT_EQ("false", stepValue4.getValueAsString());
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

  std::map<std::string, std::string> measureArguments;

  measureArguments = runner.getUpstreamMeasureArguments("MeasureName1");
  EXPECT_EQ(0, measureArguments.size());  // did not set step result

  measureArguments = runner.getUpstreamMeasureArguments("MeasureName2");
  EXPECT_EQ(2, measureArguments.size());

  bool a3 = (measureArguments.find("Argument3") == measureArguments.end());
  EXPECT_TRUE(a3);
  bool a4 = (measureArguments.find("Argument4") == measureArguments.end());
  EXPECT_TRUE(a4);
  bool sv1 = (measureArguments.find("StepValue1") == measureArguments.end());
  EXPECT_TRUE(sv1);
  bool sv2 = (measureArguments.find("StepValue2") == measureArguments.end());
  EXPECT_TRUE(sv2);
  EXPECT_EQ("342.300000", measureArguments["StepValue3"]);
  EXPECT_EQ("false", measureArguments["StepValue4"]);
}
