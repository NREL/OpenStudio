/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
