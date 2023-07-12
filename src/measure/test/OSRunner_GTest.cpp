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
