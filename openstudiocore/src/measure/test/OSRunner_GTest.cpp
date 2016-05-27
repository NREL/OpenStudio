/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "MeasureFixture.hpp"

#include "../OSRunner.hpp"
#include "../OSMeasure.hpp"
#include "../ModelMeasure.hpp"

#include "../model/Model.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/filetypes/WorkflowStepResult.hpp"

#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::measure;

class TestModelMeasure : public ModelMeasure {
 public:

  virtual std::string name() const override {
    return "TestModelMeasure";
  }

  virtual bool run(model::Model& model,
                   OSRunner& runner,
                   const std::map<std::string, OSArgument>& user_arguments) const override
  {
    ModelMeasure::run(model,runner,user_arguments);
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
  
  std::cout << "Hi Output" << std::endl;
  std::cerr << "Hi Error" << std::endl;

  TestModelMeasure measure;
  runner.prepareForMeasureRun(measure);

  std::cout << "Standard Output" << std::endl;
  std::cerr << "Standard Error" << std::endl;

  runner.incrementStep();

  std::cout << "Bye Output" << std::endl;
  std::cerr << "Bye Error" << std::endl;

  ASSERT_TRUE(step.result());
  ASSERT_TRUE(step.result()->stdOut());
  EXPECT_EQ("Standard Output\n", step.result()->stdOut().get());
  ASSERT_TRUE(step.result()->stdErr());
  EXPECT_EQ("Standard Error\n", step.result()->stdErr().get());
}