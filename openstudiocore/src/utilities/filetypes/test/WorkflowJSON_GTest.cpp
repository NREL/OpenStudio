/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../WorkflowJSON.hpp"
#include "../WorkflowJSON_Impl.hpp"
#include "../WorkflowStep.hpp"
#include "../WorkflowStep_Impl.hpp"
#include "../WorkflowStepResult.hpp"

#include "../../time/DateTime.hpp"

#include "../../core/Exception.hpp"
#include "../../core/System.hpp"
#include "../../core/Checksum.hpp"

#include <resources.hxx>

using namespace openstudio;


WorkflowStepResult getWorkflowStepResult(const WorkflowStep& step, const boost::optional<BCLMeasure>& measure)
{
  std::string stepName;
  if (step.optionalCast<MeasureStep>()){
    stepName = step.cast<MeasureStep>().measureDirName();
  }

  WorkflowStepResult workflowStepResult;
  workflowStepResult.setStartedAt(DateTime::nowUTC());
  workflowStepResult.setCompletedAt(DateTime::nowUTC());

  if (measure){
    workflowStepResult.setMeasureType(measure->measureType());
    workflowStepResult.setMeasureName(measure->name());
    workflowStepResult.setMeasureId(measure->uid());
    workflowStepResult.setMeasureVersionId(measure->versionId());
    boost::optional<DateTime> versionModified = measure->versionModified();
    if (versionModified){
      workflowStepResult.setMeasureVersionModified(versionModified.get());
    }
    workflowStepResult.setMeasureXmlChecksum(measure->xmlChecksum());
    workflowStepResult.setMeasureClassName(measure->className());
    workflowStepResult.setMeasureDisplayName(measure->displayName());
  }

  workflowStepResult.setStepResult(StepResult::Success);
  workflowStepResult.setStepInitialCondition(stepName + " Initial Condition");
  workflowStepResult.setStepFinalCondition(stepName + " Final Condition");
  workflowStepResult.addStepError(stepName + " Error 1");
  workflowStepResult.addStepError(stepName + " Error 2");
  workflowStepResult.addStepError(stepName + " Error 3");
  workflowStepResult.addStepWarning(stepName + " Warning 1");
  workflowStepResult.addStepWarning(stepName + " Warning 2");
  workflowStepResult.addStepWarning(stepName + " Warning 3");
  workflowStepResult.addStepInfo(stepName + " Info 1");
  workflowStepResult.addStepInfo(stepName + " Info 2");
  workflowStepResult.addStepInfo(stepName + " Info 3");
  workflowStepResult.addStepValue(WorkflowStepValue(stepName + " String Value", "String"));
  workflowStepResult.addStepValue(WorkflowStepValue(stepName + " Double Value", 1.1));
  workflowStepResult.addStepValue(WorkflowStepValue(stepName + " Int Value", 1));
  workflowStepResult.addStepValue(WorkflowStepValue(stepName + " Bool Value", true));
  workflowStepResult.addStepFile(stepName + " File.1");
  workflowStepResult.addStepFile(stepName + " File.2");
  workflowStepResult.setStdOut(stepName + " StdOut");
  workflowStepResult.setStdErr(stepName + " StdErr");
  return workflowStepResult;
}

void checkWorkflowStepResult(const WorkflowStep& step, const boost::optional<BCLMeasure>& measure)
{
  std::string stepName;
  if (step.optionalCast<MeasureStep>()){
    stepName = step.cast<MeasureStep>().measureDirName();
  }

  if (!step.result()){
    // bool t = false;
  }

  ASSERT_TRUE(step.result());
  WorkflowStepResult workflowStepResult = step.result().get();

  ASSERT_TRUE(workflowStepResult.startedAt());
  EXPECT_TRUE(workflowStepResult.startedAt().get() <= DateTime::nowUTC());

  ASSERT_TRUE(workflowStepResult.completedAt());
  EXPECT_TRUE(workflowStepResult.startedAt().get() <= workflowStepResult.completedAt().get());
  EXPECT_TRUE(workflowStepResult.completedAt().get() <= DateTime::nowUTC());

  if (measure){
    ASSERT_TRUE(workflowStepResult.measureType());
    EXPECT_EQ(measure->measureType(), workflowStepResult.measureType().get());

    ASSERT_TRUE(workflowStepResult.measureName());
    EXPECT_EQ(measure->name(), workflowStepResult.measureName().get());

    ASSERT_TRUE(workflowStepResult.measureId());
    ASSERT_TRUE(workflowStepResult.measureUUID());
    EXPECT_EQ(measure->uid(), workflowStepResult.measureId().get());

    ASSERT_TRUE(workflowStepResult.measureVersionId());
    ASSERT_TRUE(workflowStepResult.measureVersionUUID());
    EXPECT_EQ(measure->versionId(), workflowStepResult.measureVersionId().get());

    ASSERT_TRUE(measure->versionModified());
    ASSERT_TRUE(workflowStepResult.measureVersionModified());
    EXPECT_TRUE(measure->versionModified().get() == workflowStepResult.measureVersionModified().get());

    ASSERT_TRUE(workflowStepResult.measureXmlChecksum());
    EXPECT_EQ(measure->xmlChecksum(), workflowStepResult.measureXmlChecksum().get());

    ASSERT_TRUE(workflowStepResult.measureClassName());
    EXPECT_EQ(measure->className(), workflowStepResult.measureClassName().get());

    ASSERT_TRUE(workflowStepResult.measureDisplayName());
    EXPECT_EQ(measure->displayName(), workflowStepResult.measureDisplayName().get());
  }

  ASSERT_TRUE(workflowStepResult.stepResult ());
  EXPECT_EQ(StepResult::Success, workflowStepResult.stepResult().get().value());

  ASSERT_TRUE(workflowStepResult.stepInitialCondition());
  EXPECT_EQ(stepName + " Initial Condition", workflowStepResult.stepInitialCondition());

  ASSERT_TRUE(workflowStepResult.stepFinalCondition());
  EXPECT_EQ(stepName + " Final Condition", workflowStepResult.stepFinalCondition());

  ASSERT_EQ(3u, workflowStepResult.stepErrors().size());
  EXPECT_EQ(stepName + " Error 1", workflowStepResult.stepErrors()[0]);
  EXPECT_EQ(stepName + " Error 2", workflowStepResult.stepErrors()[1]);
  EXPECT_EQ(stepName + " Error 3", workflowStepResult.stepErrors()[2]);

  ASSERT_EQ(3u, workflowStepResult.stepWarnings().size());
  EXPECT_EQ(stepName + " Warning 1", workflowStepResult.stepWarnings()[0]);
  EXPECT_EQ(stepName + " Warning 2", workflowStepResult.stepWarnings()[1]);
  EXPECT_EQ(stepName + " Warning 3", workflowStepResult.stepWarnings()[2]);

  ASSERT_EQ(3u, workflowStepResult.stepInfo().size());
  EXPECT_EQ(stepName + " Info 1", workflowStepResult.stepInfo()[0]);
  EXPECT_EQ(stepName + " Info 2", workflowStepResult.stepInfo()[1]);
  EXPECT_EQ(stepName + " Info 3", workflowStepResult.stepInfo()[2]);

  ASSERT_EQ(4u, workflowStepResult.stepValues().size());
  EXPECT_EQ(stepName + " String Value", workflowStepResult.stepValues()[0].name());
  EXPECT_EQ(VariantType::String, workflowStepResult.stepValues()[0].variantType().value());
  EXPECT_EQ(stepName + " Double Value", workflowStepResult.stepValues()[1].name());
  EXPECT_EQ(VariantType::Double, workflowStepResult.stepValues()[1].variantType().value());
  EXPECT_EQ(stepName + " Int Value", workflowStepResult.stepValues()[2].name());
  EXPECT_EQ(VariantType::Integer, workflowStepResult.stepValues()[2].variantType().value());
  EXPECT_EQ(stepName + " Bool Value", workflowStepResult.stepValues()[3].name());
  EXPECT_EQ(VariantType::Boolean, workflowStepResult.stepValues()[3].variantType().value());

  ASSERT_EQ(2u, workflowStepResult.stepFiles().size());
  EXPECT_EQ(stepName + " File.1", toString(workflowStepResult.stepFiles()[0]));
  EXPECT_EQ(stepName + " File.2", toString(workflowStepResult.stepFiles()[1]));

  ASSERT_TRUE(workflowStepResult.stdOut());
  EXPECT_EQ(stepName + " StdOut", workflowStepResult.stdOut().get());

  ASSERT_TRUE(workflowStepResult.stdErr());
  EXPECT_EQ(stepName + " StdErr", workflowStepResult.stdErr().get());

}

class WorkflowJSONListener
{
public:
  WorkflowJSONListener()
    : dirty(false)
  {}

  void makeDirty() { dirty = true; }
  bool dirty;
};

TEST(Filetypes, WorkflowJSON_Load)
{
  path p = resourcesPath() / toPath("utilities/Filetypes/min.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/min.2.osw");
  path p3 = resourcesPath() / toPath("utilities/Filetypes/min.3.osw");
  path p4 = resourcesPath() / toPath("utilities/Filetypes/out.osw");
  ASSERT_TRUE(WorkflowJSON::load(p));

  WorkflowJSON workflow1(p);
  EXPECT_TRUE(workflow1.checkForUpdates());
  EXPECT_TRUE(workflow1.saveAs(p2));

  boost::optional<DateTime> time1 = workflow1.updatedAt();
  ASSERT_TRUE(time1);

  // needed so there would be a time delay in updated timestamp
  System::msleep(2000);

  ASSERT_TRUE(WorkflowJSON::load(p2));
  WorkflowJSON workflow2(p2);

  boost::optional<DateTime> time2 = workflow2.updatedAt();
  ASSERT_TRUE(time2);

  EXPECT_EQ(*time1, *time2);

  std::string s1 = workflow1.string(false);
  std::string s2 = workflow2.string(false);
  EXPECT_EQ(s1, s2);

  EXPECT_EQ(workflow1.hash(), workflow2.hash());
  EXPECT_EQ(workflow1.computeHash(), workflow2.computeHash());
  EXPECT_EQ(checksum(s1), checksum(s2));
  EXPECT_EQ(workflow2.hash(), workflow2.computeHash());
  EXPECT_EQ(workflow2.hash(), checksum(s2));
  EXPECT_EQ(workflow2.computeHash(), checksum(s2));

  EXPECT_FALSE(workflow2.checkForUpdates());

  s1 = workflow1.string(false);
  s2 = workflow2.string(false);
  EXPECT_EQ(s1, s2);

  if (exists(p3)){
    remove(p3);
  }
  EXPECT_FALSE(exists(p3));

  workflow2.setOswPath(p3);
  ASSERT_TRUE(workflow2.oswPath());
  EXPECT_EQ(toString(p3), toString(workflow2.oswPath().get()));
  EXPECT_EQ(toString(p4), toString(workflow2.absoluteOutPath()));
  workflow2.save();
  EXPECT_EQ(toString(p3), toString(workflow2.oswPath().get()));
  EXPECT_EQ(toString(p4), toString(workflow2.absoluteOutPath()));
  EXPECT_TRUE(exists(p3));

  if (exists(p3)){
    remove(p3);
  }
  EXPECT_FALSE(exists(p3));

  workflow1.saveAs(p3);
  ASSERT_TRUE(workflow1.oswPath());
  EXPECT_EQ(toString(p3), toString(workflow1.oswPath().get()));
  EXPECT_EQ(toString(p4), toString(workflow1.absoluteOutPath()));
  EXPECT_TRUE(exists(p3));

}


TEST(Filetypes, WorkflowJSON_Min)
{
  path p = resourcesPath() / toPath("utilities/Filetypes/min.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/min.out.osw");
  ASSERT_TRUE(WorkflowJSON::load(p));

  try{
    WorkflowJSON workflow(p);
    EXPECT_TRUE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p, workflow.oswPath().get());
    EXPECT_EQ(".", toString(workflow.rootDir()));
    EXPECT_EQ(toString(p.parent_path()), toString(workflow.absoluteRootDir()));
    EXPECT_FALSE(workflow.seedFile());
    EXPECT_FALSE(workflow.weatherFile());

    EXPECT_TRUE(workflow.saveAs(p2));

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, measureStep.getArgument("operating_expenses")->valueAsInteger());

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("space_type"));
    ASSERT_EQ(VariantType::String, measureStep.getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", measureStep.getArgument("space_type")->valueAsString());

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    EXPECT_EQ("DecreaseThermalMass", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, measureStep.getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, measureStep.getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(measureStep.getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_decrease")->valueAsInteger());

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try{
    WorkflowJSON workflow(p2);
    EXPECT_FALSE(workflow.checkForUpdates());

    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p2, workflow.oswPath().get());
    EXPECT_EQ(".", toString(workflow.rootDir()));
    EXPECT_EQ(toString(p2.parent_path()), toString(workflow.absoluteRootDir()));
    EXPECT_FALSE(workflow.seedFile());
    EXPECT_FALSE(workflow.weatherFile());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, measureStep.getArgument("operating_expenses")->valueAsInteger());

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("space_type"));
    ASSERT_EQ(VariantType::String, measureStep.getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", measureStep.getArgument("space_type")->valueAsString());

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    EXPECT_EQ("DecreaseThermalMass", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, measureStep.getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, measureStep.getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(measureStep.getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_decrease")->valueAsInteger());

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  // check if strings are really paths
  std::string s = toString(p);
  ASSERT_TRUE(WorkflowJSON::load(s));

  s = "random string";
  EXPECT_FALSE(WorkflowJSON::load(s));

  s = "Definitely; Not a:\\Path?\n _ ./";
  EXPECT_FALSE(WorkflowJSON::load(s));

  // now try relative path
  p2 = toPath("./min.out.osw");
  try{
    WorkflowJSON workflow(p);
    EXPECT_TRUE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p, workflow.oswPath().get());
    EXPECT_EQ(".", toString(workflow.rootDir()));
    EXPECT_EQ(toString(p.parent_path()), toString(workflow.absoluteRootDir()));
    EXPECT_FALSE(workflow.seedFile());
    EXPECT_FALSE(workflow.weatherFile());

    EXPECT_TRUE(workflow.saveAs(p2));

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, measureStep.getArgument("operating_expenses")->valueAsInteger());

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("space_type"));
    ASSERT_EQ(VariantType::String, measureStep.getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", measureStep.getArgument("space_type")->valueAsString());

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    EXPECT_EQ("DecreaseThermalMass", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, measureStep.getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, measureStep.getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(measureStep.getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_decrease")->valueAsInteger());

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try{
    WorkflowJSON workflow(p2);
    EXPECT_FALSE(workflow.checkForUpdates());

    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p2.filename(), workflow.oswPath().get().filename());
    EXPECT_EQ(".", toString(workflow.rootDir()));
    EXPECT_EQ(toString(workflow.oswPath().get().parent_path()), toString(workflow.absoluteRootDir()));
    EXPECT_FALSE(workflow.seedFile());
    EXPECT_FALSE(workflow.weatherFile());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, measureStep.getArgument("operating_expenses")->valueAsInteger());

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("space_type"));
    ASSERT_EQ(VariantType::String, measureStep.getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", measureStep.getArgument("space_type")->valueAsString());

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    EXPECT_EQ("DecreaseThermalMass", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, measureStep.getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, measureStep.getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(measureStep.getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_decrease")->valueAsInteger());

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }
}

TEST(Filetypes, WorkflowJSON_Full)
{
  path p = resourcesPath() / toPath("utilities/Filetypes/full.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/full.out.osw");
  EXPECT_TRUE(WorkflowJSON::load(p));

  try{
    WorkflowJSON workflow(p);
    EXPECT_TRUE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p, workflow.oswPath().get());
    EXPECT_EQ("../../", toString(workflow.rootDir()));
    EXPECT_EQ(toString(resourcesPath()), toString(workflow.absoluteRootDir()));

    ASSERT_TRUE(workflow.seedFile());
    EXPECT_EQ("1_9_0/example.osm", workflow.seedFile().get());

    // DLM: version resources went away, so make a file for ourselves
    openstudio::path expectedSeedPath = resourcesPath() / toPath("osversion/1_9_0/example.osm");
    if (!boost::filesystem::exists(expectedSeedPath)){
      boost::filesystem::create_directories(expectedSeedPath.parent_path());
      std::ofstream outFile(openstudio::toString(expectedSeedPath));
      if (outFile) {
        outFile << "OS:Version,\n\
                    {8b3ac8ca-71e7-486e-ac28-74f6e601c3f2}, !- Handle\n\
                    1.12.0;                                 !- Version Identifier\n";
        outFile.close();
      }
    }

    boost::optional<path> test = workflow.findFile(workflow.seedFile().get());
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(expectedSeedPath), toString(*test));

    ASSERT_TRUE(workflow.weatherFile());
    EXPECT_EQ("./USA_CO_Golden-NREL.724666_TMY3.epw", workflow.weatherFile().get());

    test = workflow.findFile(workflow.weatherFile().get());
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw")), toString(*test));

    test = workflow.findMeasure(toPath("SetWindowToWallRatioByFacade"));
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade")), toString(*test));

    EXPECT_FALSE(workflow.checkForUpdates());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    std::vector<BCLMeasure> measures;
    boost::optional<BCLMeasure> measure;

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    measure = workflow.getBCLMeasure(measureStep);
    ASSERT_TRUE(measure);
    measures.push_back(measure.get());
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("r_value"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("r_value")->variantType().value());
    EXPECT_EQ(30.1, measureStep.getArgument("r_value")->valueAsDouble());

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    measure = workflow.getBCLMeasure(measureStep);
    ASSERT_TRUE(measure);
    measures.push_back(measure.get());
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("r_value"));
    //ASSERT_EQ(VariantType::Double, measureStep.getArgument("r_value")->variantType().value());
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("r_value")->variantType().value());
    EXPECT_EQ(30, measureStep.getArgument("r_value")->valueAsInteger());
    EXPECT_EQ(30.0, measureStep.getArgument("r_value")->valueAsDouble());

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    measure = workflow.getBCLMeasure(measureStep);
    ASSERT_TRUE(measure);
    measures.push_back(measure.get());
    EXPECT_EQ("SetEplusInfiltration", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("flowPerZoneFloorArea"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("flowPerZoneFloorArea")->variantType().value());
    EXPECT_EQ(10.76, measureStep.getArgument("flowPerZoneFloorArea")->valueAsDouble());

    EXPECT_FALSE(workflow.checkForUpdates());
    workflowSteps[0].setResult(getWorkflowStepResult(workflowSteps[0], measures[0]));
    workflowSteps[1].setResult(getWorkflowStepResult(workflowSteps[1], measures[1]));
    workflowSteps[2].setResult(getWorkflowStepResult(workflowSteps[2], measures[2]));
    EXPECT_TRUE(workflow.checkForUpdates());

    EXPECT_TRUE(workflow.saveAs(p2));

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try{
    WorkflowJSON workflow(p2);
    //EXPECT_FALSE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p2, workflow.oswPath().get());
    EXPECT_EQ("../../", toString(workflow.rootDir()));
    EXPECT_EQ(toString(resourcesPath()), toString(workflow.absoluteRootDir()));
    ASSERT_TRUE(workflow.seedFile());
    EXPECT_EQ("1_9_0/example.osm", workflow.seedFile().get());
    boost::optional<path> test = workflow.findFile(workflow.seedFile().get());
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("osversion/1_9_0/example.osm")), toString(*test));
    ASSERT_TRUE(workflow.weatherFile());
    EXPECT_EQ("./USA_CO_Golden-NREL.724666_TMY3.epw", workflow.weatherFile().get());
    test = workflow.findFile(workflow.weatherFile().get());
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw")), toString(*test));
    test = workflow.findMeasure(toPath("SetWindowToWallRatioByFacade"));
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade")), toString(*test));

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    std::vector<BCLMeasure> measures;
    boost::optional<BCLMeasure> measure;

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    measure = workflow.getBCLMeasure(measureStep);
    ASSERT_TRUE(measure);
    measures.push_back(measure.get());
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("r_value"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("r_value")->variantType().value());
    EXPECT_EQ(30.1, measureStep.getArgument("r_value")->valueAsDouble());
    checkWorkflowStepResult(workflowSteps[0], measures[0]);

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    measure = workflow.getBCLMeasure(measureStep);
    ASSERT_TRUE(measure);
    measures.push_back(measure.get());
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("r_value"));
    //ASSERT_EQ(VariantType::Double, measureStep.getArgument("r_value")->variantType().value());
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("r_value")->variantType().value());
    EXPECT_EQ(30.0, measureStep.getArgument("r_value")->valueAsDouble());
    checkWorkflowStepResult(workflowSteps[1], measures[1]);

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    measure = workflow.getBCLMeasure(measureStep);
    ASSERT_TRUE(measure);
    measures.push_back(measure.get());
    EXPECT_EQ("SetEplusInfiltration", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("flowPerZoneFloorArea"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("flowPerZoneFloorArea")->variantType().value());
    EXPECT_EQ(10.76, measureStep.getArgument("flowPerZoneFloorArea")->valueAsDouble());
    checkWorkflowStepResult(workflowSteps[2], measures[2]);

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }
}

TEST(Filetypes, WorkflowJSON_Min_Results)
{
  path p = resourcesPath() / toPath("utilities/Filetypes/min.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/min.results.out.osw");
  ASSERT_TRUE(WorkflowJSON::load(p));

  try{
    WorkflowJSON workflow(p);
    EXPECT_TRUE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p, workflow.oswPath().get());
    EXPECT_EQ(".", toString(workflow.rootDir()));
    EXPECT_EQ(toString(p.parent_path()), toString(workflow.absoluteRootDir()));
    EXPECT_FALSE(workflow.seedFile());
    EXPECT_FALSE(workflow.weatherFile());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, measureStep.getArgument("operating_expenses")->valueAsInteger());

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("space_type"));
    ASSERT_EQ(VariantType::String, measureStep.getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", measureStep.getArgument("space_type")->valueAsString());

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    EXPECT_EQ("DecreaseThermalMass", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, measureStep.getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, measureStep.getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(measureStep.getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_decrease")->valueAsInteger());

    EXPECT_FALSE(workflow.checkForUpdates());
    workflowSteps[0].setResult(getWorkflowStepResult(workflowSteps[0], boost::none));
    workflowSteps[1].setResult(getWorkflowStepResult(workflowSteps[1], boost::none));
    workflowSteps[2].setResult(getWorkflowStepResult(workflowSteps[2], boost::none));
    EXPECT_TRUE(workflow.checkForUpdates());

    EXPECT_TRUE(workflow.saveAs(p2));

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try{
    WorkflowJSON workflow(p2);
    EXPECT_FALSE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p2, workflow.oswPath().get());
    EXPECT_EQ(".", toString(workflow.rootDir()));
    EXPECT_EQ(toString(p2.parent_path()), toString(workflow.absoluteRootDir()));
    EXPECT_FALSE(workflow.seedFile());
    EXPECT_FALSE(workflow.weatherFile());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    ASSERT_TRUE(workflowSteps[0].optionalCast<MeasureStep>());
    MeasureStep measureStep = workflowSteps[0].cast<MeasureStep>();
    EXPECT_EQ("IncreaseWallRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, measureStep.getArgument("operating_expenses")->valueAsInteger());
    checkWorkflowStepResult(workflowSteps[0], boost::none);

    ASSERT_TRUE(workflowSteps[1].optionalCast<MeasureStep>());
    measureStep = workflowSteps[1].cast<MeasureStep>();
    EXPECT_EQ("IncreaseRoofRValue", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("cost"));
    ASSERT_EQ(VariantType::Double, measureStep.getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, measureStep.getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(measureStep.getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, measureStep.getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(measureStep.getArgument("space_type"));
    ASSERT_EQ(VariantType::String, measureStep.getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", measureStep.getArgument("space_type")->valueAsString());
    checkWorkflowStepResult(workflowSteps[1], boost::none);

    ASSERT_TRUE(workflowSteps[2].optionalCast<MeasureStep>());
    measureStep = workflowSteps[2].cast<MeasureStep>();
    EXPECT_EQ("DecreaseThermalMass", measureStep.measureDirName());
    ASSERT_TRUE(measureStep.getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, measureStep.getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, measureStep.getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(measureStep.getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, measureStep.getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, measureStep.getArgument("percent_decrease")->valueAsInteger());
    checkWorkflowStepResult(workflowSteps[2], boost::none);

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }
}

TEST(Filetypes, WorkflowJSON_Bad)
{
  path p = resourcesPath() / toPath("utilities/Filetypes/nonexistent.osw");
  EXPECT_THROW({ WorkflowJSON workflow(p); }, openstudio::Exception);
  EXPECT_FALSE(WorkflowJSON::load(p));

  p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
  EXPECT_THROW({ WorkflowJSON workflow(p); }, openstudio::Exception);
  EXPECT_FALSE(WorkflowJSON::load(p));
}

TEST(Filetypes, WorkflowStep_EscapeCharacters)
{
  MeasureStep step("\"My Measure\"");

  std::string jsonValue = "\"steps\" : [ {\"arguments\" : {}, \"measure_dir_name\" : \"IncreaseWallRValue\"}]";
  step.setArgument("\"My Argument\"", jsonValue);

  boost::optional<WorkflowStep> temp = WorkflowStep::fromString(step.string());
  ASSERT_TRUE(temp);
  ASSERT_TRUE(temp->optionalCast<MeasureStep>());
  MeasureStep step2 = temp->cast<MeasureStep>();

  std::cout << step.string() << std::endl;

  EXPECT_EQ("\"My Measure\"", step2.measureDirName());

  ASSERT_EQ(1u, step2.arguments().size());

  boost::optional<Variant> value = step2.getArgument("\"My Argument\"");
  ASSERT_TRUE(value);
  ASSERT_EQ(VariantType::String, value->variantType().value());
  EXPECT_EQ(jsonValue, value->valueAsString());

}

TEST(Filetypes, WorkflowStep_EscapeCharacters2)
{
  MeasureStep step(R"("My Measure")");

  std::string jsonValue = R"("steps" : [ {"arguments" : {}, "measure_dir_name" : "IncreaseWallRValue"}])";
  step.setArgument(R"("My Argument")", jsonValue);

  boost::optional<WorkflowStep> temp = WorkflowStep::fromString(step.string());
  ASSERT_TRUE(temp);
  ASSERT_TRUE(temp->optionalCast<MeasureStep>());
  MeasureStep step2 = temp->cast<MeasureStep>();

  std::cout << step.string() << std::endl;

  EXPECT_EQ("\"My Measure\"", step2.measureDirName());

  ASSERT_EQ(1u, step2.arguments().size());

  boost::optional<Variant> value = step2.getArgument("\"My Argument\"");
  ASSERT_TRUE(value);
  ASSERT_EQ(VariantType::String, value->variantType().value());
  EXPECT_EQ(jsonValue, value->valueAsString());

}

TEST(Filetypes, WorkflowStepResult_EscapeCharacters)
{
  WorkflowStepResult result;

  result.setStepInitialCondition("\"Initial Condition\"");
  result.setStepFinalCondition("\"Final Condition\"");
  result.addStepError("\"Step Error\"");
  result.addStepWarning("\"Step Warning\"");
  result.addStepInfo("\"Step Info\"");
  result.setStdOut("\"Standard Output\"");
  result.setStdErr("\"Standard Error\"");

  WorkflowStepValue value("\"Value Name\"", 1.1);
  result.addStepValue(value);

  boost::optional<WorkflowStepResult> result2 = WorkflowStepResult::fromString(result.string());
  ASSERT_TRUE(result2);

  std::cout << result.string() << std::endl;

  ASSERT_TRUE(result2->initialCondition());
  EXPECT_EQ("\"Initial Condition\"", result2->stepInitialCondition().get());
  ASSERT_TRUE(result2->finalCondition());
  EXPECT_EQ("\"Final Condition\"", result2->stepFinalCondition().get());
  /*ASSERT_TRUE(result2->initialCondition());
  result2.addStepError("\"Step Error\"");
  ASSERT_TRUE(result2->initialCondition());
  result2.addStepWarning("\"Step Warning\"");
  ASSERT_TRUE(result2->initialCondition());
  result2.addStepInfo("\"Step Info\"");
  ASSERT_TRUE(result2->initialCondition());
    */
  ASSERT_TRUE(result2->stdOut());
  EXPECT_EQ("\"Standard Output\"", result2->stdOut().get());
  ASSERT_TRUE(result2->stdErr());
  EXPECT_EQ("\"Standard Error\"", result2->stdErr().get());

}

TEST(Filetypes, WorkflowStepResult_EscapeCharacters2)
{
  WorkflowStepResult result;

  result.setStepInitialCondition(R"("Initial Condition")");
  result.setStepFinalCondition(R"("Final Condition")");
  result.addStepError(R"("Step Error")");
  result.addStepWarning(R"("Step Warning")");
  result.addStepInfo(R"("Step Info")");
  result.setStdOut(R"("Standard Output")");
  result.setStdErr(R"("Standard Error")");

  WorkflowStepValue value(R"("Value Name")", 1.1);
  result.addStepValue(value);

  boost::optional<WorkflowStepResult> result2 = WorkflowStepResult::fromString(result.string());
  ASSERT_TRUE(result2);

  std::cout << result.string() << std::endl;

  ASSERT_TRUE(result2->stepInitialCondition());
  EXPECT_EQ("\"Initial Condition\"", result2->stepInitialCondition().get());
  ASSERT_TRUE(result2->stepFinalCondition());
  EXPECT_EQ("\"Final Condition\"", result2->stepFinalCondition().get());
  /*ASSERT_TRUE(result2->initialCondition());
  result2.addStepError("\"Step Error\"");
  ASSERT_TRUE(result2->initialCondition());
  result2.addStepWarning("\"Step Warning\"");
  ASSERT_TRUE(result2->initialCondition());
  result2.addStepInfo("\"Step Info\"");
  ASSERT_TRUE(result2->initialCondition());
    */
  ASSERT_TRUE(result2->stdOut());
  EXPECT_EQ("\"Standard Output\"", result2->stdOut().get());
  ASSERT_TRUE(result2->stdErr());
  EXPECT_EQ("\"Standard Error\"", result2->stdErr().get());

}

TEST(Filetypes, WorkflowJSON_Setters)
{
  WorkflowJSON workflowJSON;

  EXPECT_FALSE(workflowJSON.seedFile());
  EXPECT_FALSE(workflowJSON.weatherFile());

  EXPECT_TRUE(workflowJSON.setSeedFile(toPath("../in.osm")));
  EXPECT_TRUE(workflowJSON.setWeatherFile(toPath("./files/in.epw")));

  ASSERT_TRUE(workflowJSON.seedFile());
  ASSERT_TRUE(workflowJSON.weatherFile());

  EXPECT_EQ(toPath("../in.osm"), workflowJSON.seedFile().get());
  EXPECT_EQ(toPath("./files/in.epw"), workflowJSON.weatherFile().get());

  std::string json = workflowJSON.string();

  workflowJSON.resetSeedFile();
  workflowJSON.resetWeatherFile();

  EXPECT_FALSE(workflowJSON.seedFile());
  EXPECT_FALSE(workflowJSON.weatherFile());

  EXPECT_NO_THROW({ workflowJSON = WorkflowJSON(json); });

  ASSERT_TRUE(workflowJSON.seedFile());
  ASSERT_TRUE(workflowJSON.weatherFile());

  EXPECT_EQ(toPath("../in.osm"), workflowJSON.seedFile().get());
  EXPECT_EQ(toPath("./files/in.epw"), workflowJSON.weatherFile().get());

  MeasureStep step("rotation_measure");
  std::vector<WorkflowStep> steps;
  steps.push_back(step);
  workflowJSON.setWorkflowSteps(steps);
  EXPECT_EQ(1u, workflowJSON.workflowSteps().size());

  EXPECT_EQ(0u, step.arguments().size());
  EXPECT_FALSE(step.getArgument("rotation"));

  step.setArgument("rotation", 0.0);
  EXPECT_EQ(1u, step.arguments().size());
  ASSERT_TRUE(step.getArgument("rotation"));
  ASSERT_TRUE(step.getArgument("rotation")->variantType() == VariantType::Double);
  EXPECT_EQ(0.0, step.getArgument("rotation")->valueAsDouble());

  step.setArgument("rotation", 90.0);
  EXPECT_EQ(1u, step.arguments().size());
  ASSERT_TRUE(step.getArgument("rotation"));
  ASSERT_TRUE(step.getArgument("rotation")->variantType() == VariantType::Double);
  EXPECT_EQ(90.0, step.getArgument("rotation")->valueAsDouble());

  step.setArgument("rotation", "none");
  EXPECT_EQ(1u, step.arguments().size());
  ASSERT_TRUE(step.getArgument("rotation"));
  ASSERT_TRUE(step.getArgument("rotation")->variantType() == VariantType::String);
  EXPECT_EQ("none", step.getArgument("rotation")->valueAsString());

  step.removeArgument("rotation");
  EXPECT_FALSE(step.getArgument("rotation"));
  EXPECT_EQ(0, step.arguments().size());
}

TEST(Filetypes, WorkflowJSON_Signals)
{
  WorkflowJSON workflowJSON;
  WorkflowJSONListener listener;

  workflowJSON.getImpl<openstudio::detail::WorkflowJSON_Impl>().get()->WorkflowJSON_Impl::onChange.connect<WorkflowJSONListener, &WorkflowJSONListener::makeDirty>(listener);

  EXPECT_EQ(false, listener.dirty);
  workflowJSON.setSeedFile(toPath("in.osm"));
  EXPECT_EQ(true, listener.dirty);
  listener.dirty = false;

  MeasureStep step("My Measure");

  std::vector<WorkflowStep> steps;
  steps.push_back(step);

  EXPECT_EQ(false, listener.dirty);
  workflowJSON.setWorkflowSteps(steps);
  EXPECT_EQ(true, listener.dirty);
  listener.dirty = false;

  EXPECT_EQ(false, listener.dirty);
  step.setName("New Name");
  EXPECT_EQ(true, listener.dirty);
  listener.dirty = false;

  EXPECT_EQ(false, listener.dirty);
  workflowJSON.resetWorkflowSteps();
  EXPECT_EQ(true, listener.dirty);
  listener.dirty = false;

  EXPECT_EQ(false, listener.dirty);
  step.setName("New Name");
  EXPECT_EQ(false, listener.dirty);
  listener.dirty = false;
}

TEST(Filetypes, RunOptions)
{
  WorkflowJSON workflow;

  EXPECT_FALSE(workflow.runOptions());

  CustomOutputAdapter adapter("my_ruby_file.rb", "MyOutputAdapter", "{}");

  RunOptions options;
  options.setDebug(true);
  options.setCustomOutputAdapter(adapter);

  workflow.setRunOptions(options);

  ASSERT_TRUE(workflow.runOptions());
  EXPECT_TRUE(workflow.runOptions()->debug());
  ASSERT_TRUE(workflow.runOptions()->customOutputAdapter());
  EXPECT_EQ("my_ruby_file.rb", workflow.runOptions()->customOutputAdapter()->customFileName());
  EXPECT_EQ("MyOutputAdapter", workflow.runOptions()->customOutputAdapter()->className());

  std::string s = workflow.string();

  boost::optional<WorkflowJSON> workflow2 = WorkflowJSON::load(s);
  ASSERT_TRUE(workflow2);

  ASSERT_TRUE(workflow2->runOptions());
  EXPECT_TRUE(workflow2->runOptions()->debug());
  ASSERT_TRUE(workflow2->runOptions()->customOutputAdapter());
  EXPECT_EQ("my_ruby_file.rb", workflow2->runOptions()->customOutputAdapter()->customFileName());
  EXPECT_EQ("MyOutputAdapter", workflow2->runOptions()->customOutputAdapter()->className());
}
