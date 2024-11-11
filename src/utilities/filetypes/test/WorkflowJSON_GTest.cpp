/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../WorkflowJSON.hpp"
#include "../WorkflowJSON_Impl.hpp"
#include "../WorkflowStep.hpp"
#include "../WorkflowStep_Impl.hpp"
#include "../WorkflowStepResult.hpp"
#include "../RunOptions.hpp"
#include "../ForwardTranslatorOptions.hpp"

#include "../../time/DateTime.hpp"

#include "../../core/Filesystem.hpp"
#include "../../core/Exception.hpp"
#include "../../core/System.hpp"
#include "../../core/Checksum.hpp"
#include "../../core/StringStreamLogSink.hpp"

#include <resources.hxx>

using namespace openstudio;

WorkflowStepResult getWorkflowStepResult(const WorkflowStep& step, const boost::optional<BCLMeasure>& measure) {
  std::string stepName;
  if (step.optionalCast<MeasureStep>()) {
    stepName = step.cast<MeasureStep>().measureDirName();
  }

  WorkflowStepResult workflowStepResult;
  workflowStepResult.setStartedAt(DateTime::nowUTC());
  workflowStepResult.setCompletedAt(DateTime::nowUTC());

  if (measure) {
    workflowStepResult.setMeasureType(measure->measureType());
    workflowStepResult.setMeasureName(measure->name());
    workflowStepResult.setMeasureId(measure->uid());
    workflowStepResult.setMeasureVersionId(measure->versionId());
    boost::optional<DateTime> versionModified = measure->versionModified();
    if (versionModified) {
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

void checkWorkflowStepResult(const WorkflowStep& step, const boost::optional<BCLMeasure>& measure) {
  std::string stepName;
  if (step.optionalCast<MeasureStep>()) {
    stepName = step.cast<MeasureStep>().measureDirName();
  }

  if (!step.result()) {
    // bool t = false;
  }

  ASSERT_TRUE(step.result());
  WorkflowStepResult workflowStepResult = step.result().get();

  ASSERT_TRUE(workflowStepResult.startedAt());
  EXPECT_TRUE(workflowStepResult.startedAt().get() <= DateTime::nowUTC());

  ASSERT_TRUE(workflowStepResult.completedAt());
  EXPECT_TRUE(workflowStepResult.startedAt().get() <= workflowStepResult.completedAt().get());
  EXPECT_TRUE(workflowStepResult.completedAt().get() <= DateTime::nowUTC());

  if (measure) {
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

  ASSERT_TRUE(workflowStepResult.stepResult());
  EXPECT_EQ(StepResult::Success, workflowStepResult.stepResult().get().value());

  ASSERT_TRUE(workflowStepResult.stepInitialCondition());
  EXPECT_EQ(stepName + " Initial Condition", workflowStepResult.stepInitialCondition().get());

  ASSERT_TRUE(workflowStepResult.stepFinalCondition());
  EXPECT_EQ(stepName + " Final Condition", workflowStepResult.stepFinalCondition().get());

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
  void makeDirty() {
    dirty = true;
  }
  bool dirty = false;
};

TEST(Filetypes, QtGUI_WorkflowJSON_Load) {
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

  if (exists(p3)) {
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

  if (exists(p3)) {
    remove(p3);
  }
  EXPECT_FALSE(exists(p3));

  workflow1.saveAs(p3);
  ASSERT_TRUE(workflow1.oswPath());
  EXPECT_EQ(toString(p3), toString(workflow1.oswPath().get()));
  EXPECT_EQ(toString(p4), toString(workflow1.absoluteOutPath()));
  EXPECT_TRUE(exists(p3));
}

TEST(Filetypes, WorkflowJSON_Min) {
  path p = resourcesPath() / toPath("utilities/Filetypes/min.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/min.out.osw");
  ASSERT_TRUE(WorkflowJSON::load(p));

  try {
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try {
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
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
  try {
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try {
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }
}

TEST(Filetypes, WorkflowJSON_Full) {
  path p = resourcesPath() / toPath("utilities/Filetypes/full.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/full.out.osw");
  EXPECT_TRUE(WorkflowJSON::load(p));

  try {
    WorkflowJSON workflow(p);
    EXPECT_TRUE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p, workflow.oswPath().get());
    EXPECT_EQ("../..", toString(workflow.rootDir()));
    EXPECT_EQ(toString(resourcesPath()), toString(workflow.absoluteRootDir()));

    ASSERT_TRUE(workflow.seedFile());
    EXPECT_EQ("1_9_0/example.osm", workflow.seedFile().get());

    // DLM: version resources went away, so make a file for ourselves
    openstudio::path expectedSeedPath = resourcesPath() / toPath("osversion/1_9_0/example.osm");
    if (!boost::filesystem::exists(expectedSeedPath)) {
      boost::filesystem::create_directories(expectedSeedPath.parent_path());
      std::ofstream outFile(openstudio::toSystemFilename(expectedSeedPath));
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try {
    WorkflowJSON workflow(p2);
    //EXPECT_FALSE(workflow.checkForUpdates());
    ASSERT_TRUE(workflow.oswPath());
    EXPECT_EQ(p2, workflow.oswPath().get());
    EXPECT_EQ("../..", toString(workflow.rootDir()));
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }
}

TEST(Filetypes, WorkflowJSON_Min_Results) {
  path p = resourcesPath() / toPath("utilities/Filetypes/min.osw");
  path p2 = resourcesPath() / toPath("utilities/Filetypes/min.results.out.osw");
  ASSERT_TRUE(WorkflowJSON::load(p));

  try {
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try {
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
    auto measureStep = workflowSteps[0].cast<MeasureStep>();
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

  } catch (const std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }
}

TEST(Filetypes, WorkflowJSON_Bad) {
  path p = resourcesPath() / toPath("utilities/Filetypes/nonexistent.osw");
  EXPECT_THROW({ WorkflowJSON{p}; }, openstudio::Exception);
  EXPECT_FALSE(WorkflowJSON::load(p));

  p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
  EXPECT_THROW({ WorkflowJSON{p}; }, openstudio::Exception);
  EXPECT_FALSE(WorkflowJSON::load(p));
}

TEST(Filetypes, WorkflowStep_EscapeCharacters) {
  MeasureStep step("\"My Measure\"");

  std::string jsonValue = "\"steps\" : [ {\"arguments\" : {}, \"measure_dir_name\" : \"IncreaseWallRValue\"}]";
  step.setArgument("\"My Argument\"", jsonValue);

  boost::optional<WorkflowStep> temp = WorkflowStep::fromString(step.string());
  ASSERT_TRUE(temp);
  ASSERT_TRUE(temp->optionalCast<MeasureStep>());
  auto step2 = temp->cast<MeasureStep>();

  std::cout << step.string() << '\n';

  EXPECT_EQ("\"My Measure\"", step2.measureDirName());

  ASSERT_EQ(1u, step2.arguments().size());

  boost::optional<Variant> value = step2.getArgument("\"My Argument\"");
  ASSERT_TRUE(value);
  ASSERT_EQ(VariantType::String, value->variantType().value());
  EXPECT_EQ(jsonValue, value->valueAsString());
}

TEST(Filetypes, WorkflowStep_EscapeCharacters2) {
  MeasureStep step(R"("My Measure")");

  std::string jsonValue = R"("steps" : [ {"arguments" : {}, "measure_dir_name" : "IncreaseWallRValue"}])";
  step.setArgument(R"("My Argument")", jsonValue);

  boost::optional<WorkflowStep> temp = WorkflowStep::fromString(step.string());
  ASSERT_TRUE(temp);
  ASSERT_TRUE(temp->optionalCast<MeasureStep>());
  auto step2 = temp->cast<MeasureStep>();

  std::cout << step.string() << '\n';

  EXPECT_EQ("\"My Measure\"", step2.measureDirName());

  ASSERT_EQ(1u, step2.arguments().size());

  boost::optional<Variant> value = step2.getArgument("\"My Argument\"");
  ASSERT_TRUE(value);
  ASSERT_EQ(VariantType::String, value->variantType().value());
  EXPECT_EQ(jsonValue, value->valueAsString());
}

TEST(Filetypes, WorkflowStepResult_EscapeCharacters) {
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

  std::cout << result.string() << '\n';

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

TEST(Filetypes, WorkflowStepResult_EscapeCharacters2) {
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

  std::cout << result.string() << '\n';

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

TEST(Filetypes, WorkflowStepResult_RepeatedKeys) {
  WorkflowStepResult result;

  result.addStepValue(WorkflowStepValue("key1", 1.1));

  ASSERT_EQ(1u, result.stepValues().size());
  EXPECT_EQ("key1", result.stepValues()[0].name());
  EXPECT_EQ(1.1, result.stepValues()[0].valueAsDouble());

  result.addStepValue(WorkflowStepValue("key2", 2.1));

  ASSERT_EQ(2u, result.stepValues().size());
  EXPECT_EQ("key1", result.stepValues()[0].name());
  EXPECT_EQ(1.1, result.stepValues()[0].valueAsDouble());
  EXPECT_EQ("key2", result.stepValues()[1].name());
  EXPECT_EQ(2.1, result.stepValues()[1].valueAsDouble());

  // adding same key name will remove previous value and add a new one
  result.addStepValue(WorkflowStepValue("key1", 1.2));

  ASSERT_EQ(2u, result.stepValues().size());
  EXPECT_EQ("key2", result.stepValues()[0].name());
  EXPECT_EQ(2.1, result.stepValues()[0].valueAsDouble());
  EXPECT_EQ("key1", result.stepValues()[1].name());
  EXPECT_EQ(1.2, result.stepValues()[1].valueAsDouble());

  // adding same key name will remove previous value and add a new one
  result.addStepValue(WorkflowStepValue("key2", 2.2));

  ASSERT_EQ(2u, result.stepValues().size());
  EXPECT_EQ("key1", result.stepValues()[0].name());
  EXPECT_EQ(1.2, result.stepValues()[0].valueAsDouble());
  EXPECT_EQ("key2", result.stepValues()[1].name());
  EXPECT_EQ(2.2, result.stepValues()[1].valueAsDouble());

  // key names are case sensitive
  result.addStepValue(WorkflowStepValue("Key1", 1.3));

  ASSERT_EQ(3u, result.stepValues().size());
  EXPECT_EQ("key1", result.stepValues()[0].name());
  EXPECT_EQ(1.2, result.stepValues()[0].valueAsDouble());
  EXPECT_EQ("key2", result.stepValues()[1].name());
  EXPECT_EQ(2.2, result.stepValues()[1].valueAsDouble());
  EXPECT_EQ("Key1", result.stepValues()[2].name());
  EXPECT_EQ(1.3, result.stepValues()[2].valueAsDouble());
}

TEST(Filetypes, WorkflowJSON_Setters) {
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

TEST(Filetypes, WorkflowJSON_Signals) {
  WorkflowJSON workflowJSON;
  WorkflowJSONListener listener;

  workflowJSON.getImpl<openstudio::detail::WorkflowJSON_Impl>()
    .get()
    ->WorkflowJSON_Impl::onChange.connect<WorkflowJSONListener, &WorkflowJSONListener::makeDirty>(listener);

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

TEST(Filetypes, RunOptions) {
  WorkflowJSON workflow;

  EXPECT_FALSE(workflow.runOptions());

  CustomOutputAdapter adapter("my_ruby_file.rb", "MyOutputAdapter");

  RunOptions options;
  options.setDebug(true);
  options.setCustomOutputAdapter(adapter);

  workflow.setRunOptions(options);

  ASSERT_TRUE(workflow.runOptions());
  EXPECT_TRUE(workflow.runOptions()->debug());
  ASSERT_TRUE(workflow.runOptions()->customOutputAdapter());
  EXPECT_EQ("my_ruby_file.rb", workflow.runOptions()->customOutputAdapter()->customFileName());
  EXPECT_EQ("MyOutputAdapter", workflow.runOptions()->customOutputAdapter()->className());
  EXPECT_TRUE(workflow.runOptions()->customOutputAdapter()->options().empty());
  EXPECT_TRUE(workflow.runOptions()->customOutputAdapter()->optionsJSON().isNull());

  std::string s = workflow.string();

  boost::optional<WorkflowJSON> workflow2 = WorkflowJSON::load(s);
  ASSERT_TRUE(workflow2);

  ASSERT_TRUE(workflow2->runOptions());
  EXPECT_TRUE(workflow2->runOptions()->debug());
  ASSERT_TRUE(workflow2->runOptions()->customOutputAdapter());
  EXPECT_EQ("my_ruby_file.rb", workflow2->runOptions()->customOutputAdapter()->customFileName());
  EXPECT_EQ("MyOutputAdapter", workflow2->runOptions()->customOutputAdapter()->className());
}

TEST(Filetypes, RunOptions_ForwardTranslate_BackwardCompatibility) {
  WorkflowJSON workflow;

  EXPECT_FALSE(workflow.runOptions());

  CustomOutputAdapter adapter("my_ruby_file.rb", "MyOutputAdapter", "{}");

  RunOptions options;
  options.setDebug(true);
  options.setEpjson(true);
  options.setCustomOutputAdapter(adapter);

  {
    ForwardTranslatorOptions ftOptions = options.forwardTranslatorOptions();
    EXPECT_TRUE(ftOptions.keepRunControlSpecialDays());
    EXPECT_FALSE(ftOptions.iPTabularOutput());
    EXPECT_FALSE(ftOptions.excludeLCCObjects());
    EXPECT_FALSE(ftOptions.excludeSQliteOutputReport());
    EXPECT_FALSE(ftOptions.excludeHTMLOutputReport());
    EXPECT_FALSE(ftOptions.excludeVariableDictionary());
    EXPECT_FALSE(ftOptions.excludeSpaceTranslation());
  }

  // This makes no sense, but we picked 3 spaces for some reason... return string is formated with wbuilder["indentation"] = "   ";
  std::string ft_options = R"json({
   "ip_tabular_output" : true,
   "no_html_output" : true,
   "no_lifecyclecosts" : true,
   "no_space_translation" : true,
   "no_sqlite_output" : true,
   "no_variable_dictionary" : true,
   "runcontrolspecialdays" : false
})json";

  // This settter internally creates a new ForwardTranslatorOptions, so we can't reuse the one above
  options.setForwardTranslateOptions(ft_options);

  {
    ForwardTranslatorOptions ftOptions = options.forwardTranslatorOptions();
    EXPECT_FALSE(ftOptions.keepRunControlSpecialDays());
    EXPECT_TRUE(ftOptions.iPTabularOutput());
    EXPECT_TRUE(ftOptions.excludeLCCObjects());
    EXPECT_TRUE(ftOptions.excludeSQliteOutputReport());
    EXPECT_TRUE(ftOptions.excludeHTMLOutputReport());
    EXPECT_TRUE(ftOptions.excludeVariableDictionary());
    EXPECT_TRUE(ftOptions.excludeSpaceTranslation());
  }

  workflow.setRunOptions(options);

  ASSERT_TRUE(workflow.runOptions());
  EXPECT_TRUE(workflow.runOptions()->debug());
  EXPECT_TRUE(workflow.runOptions()->epjson());
  ASSERT_TRUE(workflow.runOptions()->customOutputAdapter());
  EXPECT_EQ(ft_options, workflow.runOptions()->forwardTranslateOptions());
  EXPECT_EQ("my_ruby_file.rb", workflow.runOptions()->customOutputAdapter()->customFileName());
  EXPECT_EQ("MyOutputAdapter", workflow.runOptions()->customOutputAdapter()->className());

  std::string s = workflow.string();

  boost::optional<WorkflowJSON> workflow2 = WorkflowJSON::load(s);
  ASSERT_TRUE(workflow2);

  ASSERT_TRUE(workflow2->runOptions());
  EXPECT_TRUE(workflow2->runOptions()->debug());
  ASSERT_TRUE(workflow2->runOptions()->customOutputAdapter());
  EXPECT_EQ(ft_options, workflow2->runOptions()->forwardTranslateOptions());
  EXPECT_EQ("my_ruby_file.rb", workflow2->runOptions()->customOutputAdapter()->customFileName());
  EXPECT_EQ("MyOutputAdapter", workflow2->runOptions()->customOutputAdapter()->className());
}

TEST(Filetypes, RunOptions_NoFtOptions) {
  // Test for #4815
  const RunOptions options;
  EXPECT_EQ("", options.forwardTranslateOptions());
}

TEST(Filetypes, RunOptions_GettersSetters) {
  RunOptions runOptions;

  // Ctor Default
  ASSERT_FALSE(runOptions.debug());
  ASSERT_TRUE(runOptions.isDebugDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setDebug(true));
  ASSERT_TRUE(runOptions.debug());
  ASSERT_FALSE(runOptions.isDebugDefaulted());
  // Reset
  runOptions.resetDebug();
  ASSERT_FALSE(runOptions.debug());
  ASSERT_TRUE(runOptions.isDebugDefaulted());
  ASSERT_TRUE(runOptions.isDebugDefaulted());

  // Ctor Default
  ASSERT_FALSE(runOptions.epjson());
  ASSERT_TRUE(runOptions.isEpjsonDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setEpjson(true));
  ASSERT_TRUE(runOptions.epjson());
  ASSERT_FALSE(runOptions.isEpjsonDefaulted());
  // Reset
  runOptions.resetEpjson();
  ASSERT_FALSE(runOptions.epjson());
  ASSERT_TRUE(runOptions.isEpjsonDefaulted());
  ASSERT_TRUE(runOptions.isEpjsonDefaulted());

  // Ctor Default
  ASSERT_FALSE(runOptions.fast());
  ASSERT_TRUE(runOptions.isFastDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setFast(true));
  ASSERT_TRUE(runOptions.fast());
  ASSERT_FALSE(runOptions.isFastDefaulted());
  // Reset
  runOptions.resetFast();
  ASSERT_FALSE(runOptions.fast());
  ASSERT_TRUE(runOptions.isFastDefaulted());
  ASSERT_TRUE(runOptions.isFastDefaulted());

  // Ctor Default
  ASSERT_FALSE(runOptions.preserveRunDir());
  ASSERT_TRUE(runOptions.isPreserveRunDirDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setPreserveRunDir(true));
  ASSERT_TRUE(runOptions.preserveRunDir());
  ASSERT_FALSE(runOptions.isPreserveRunDirDefaulted());
  // Reset
  runOptions.resetPreserveRunDir();
  ASSERT_FALSE(runOptions.preserveRunDir());
  ASSERT_TRUE(runOptions.isPreserveRunDirDefaulted());
  ASSERT_TRUE(runOptions.isPreserveRunDirDefaulted());

  // Ctor Default
  ASSERT_FALSE(runOptions.skipExpandObjects());
  ASSERT_TRUE(runOptions.isSkipExpandObjectsDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setSkipExpandObjects(true));
  ASSERT_TRUE(runOptions.skipExpandObjects());
  ASSERT_FALSE(runOptions.isSkipExpandObjectsDefaulted());
  // Reset
  runOptions.resetSkipExpandObjects();
  ASSERT_FALSE(runOptions.skipExpandObjects());
  ASSERT_TRUE(runOptions.isSkipExpandObjectsDefaulted());
  ASSERT_TRUE(runOptions.isSkipExpandObjectsDefaulted());

  // Ctor Default
  ASSERT_FALSE(runOptions.skipEnergyPlusPreprocess());
  ASSERT_TRUE(runOptions.isSkipEnergyPlusPreprocessDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setSkipEnergyPlusPreprocess(true));
  ASSERT_TRUE(runOptions.skipEnergyPlusPreprocess());
  ASSERT_FALSE(runOptions.isSkipEnergyPlusPreprocessDefaulted());
  // Reset
  runOptions.resetSkipEnergyPlusPreprocess();
  ASSERT_FALSE(runOptions.skipEnergyPlusPreprocess());
  ASSERT_TRUE(runOptions.isSkipEnergyPlusPreprocessDefaulted());
  ASSERT_TRUE(runOptions.isSkipEnergyPlusPreprocessDefaulted());

  // Ctor Default
  ASSERT_FALSE(runOptions.skipZipResults());
  ASSERT_TRUE(runOptions.isSkipZipResultsDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setSkipZipResults(true));
  ASSERT_TRUE(runOptions.skipZipResults());
  ASSERT_FALSE(runOptions.isSkipZipResultsDefaulted());
  // Reset
  runOptions.resetSkipZipResults();
  ASSERT_FALSE(runOptions.skipZipResults());
  ASSERT_TRUE(runOptions.isSkipZipResultsDefaulted());
  ASSERT_TRUE(runOptions.isSkipZipResultsDefaulted());

  // Ctor Default
  ASSERT_TRUE(runOptions.cleanup());
  ASSERT_TRUE(runOptions.isCleanupDefaulted());
  // Set to opposite of default
  ASSERT_TRUE(runOptions.setCleanup(false));
  ASSERT_FALSE(runOptions.cleanup());
  ASSERT_FALSE(runOptions.isCleanupDefaulted());
  // Reset
  runOptions.resetCleanup();
  ASSERT_TRUE(runOptions.cleanup());
  ASSERT_TRUE(runOptions.isCleanupDefaulted());
  ASSERT_TRUE(runOptions.isCleanupDefaulted());
}
TEST(Filetypes, ForwardTranslatorOptions_GettersSetters) {

  ForwardTranslatorOptions ftOptions;

  // Ctor Default
  ASSERT_TRUE(ftOptions.keepRunControlSpecialDays());
  ASSERT_TRUE(ftOptions.isKeepRunControlSpecialDaysDefaulted());
  // Set to opposite of default
  ftOptions.setKeepRunControlSpecialDays(false);
  ASSERT_FALSE(ftOptions.keepRunControlSpecialDays());
  ASSERT_FALSE(ftOptions.isKeepRunControlSpecialDaysDefaulted());
  // Reset
  ftOptions.resetKeepRunControlSpecialDays();
  ASSERT_TRUE(ftOptions.keepRunControlSpecialDays());
  ASSERT_TRUE(ftOptions.isKeepRunControlSpecialDaysDefaulted());
  ASSERT_TRUE(ftOptions.isKeepRunControlSpecialDaysDefaulted());

  // Ctor Default
  ASSERT_FALSE(ftOptions.iPTabularOutput());
  ASSERT_TRUE(ftOptions.isIPTabularOutputDefaulted());
  // Set to opposite of default
  ftOptions.setIPTabularOutput(true);
  ASSERT_TRUE(ftOptions.iPTabularOutput());
  ASSERT_FALSE(ftOptions.isIPTabularOutputDefaulted());
  // Reset
  ftOptions.resetIPTabularOutput();
  ASSERT_FALSE(ftOptions.iPTabularOutput());
  ASSERT_TRUE(ftOptions.isIPTabularOutputDefaulted());
  ASSERT_TRUE(ftOptions.isIPTabularOutputDefaulted());

  // Ctor Default
  ASSERT_FALSE(ftOptions.excludeLCCObjects());
  ASSERT_TRUE(ftOptions.isExcludeLCCObjectsDefaulted());
  // Set to opposite of default
  ftOptions.setExcludeLCCObjects(true);
  ASSERT_TRUE(ftOptions.excludeLCCObjects());
  ASSERT_FALSE(ftOptions.isExcludeLCCObjectsDefaulted());
  // Reset
  ftOptions.resetExcludeLCCObjects();
  ASSERT_FALSE(ftOptions.excludeLCCObjects());
  ASSERT_TRUE(ftOptions.isExcludeLCCObjectsDefaulted());
  ASSERT_TRUE(ftOptions.isExcludeLCCObjectsDefaulted());

  // Ctor Default
  ASSERT_FALSE(ftOptions.excludeSQliteOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeSQliteOutputReportDefaulted());
  // Set to opposite of default
  ftOptions.setExcludeSQliteOutputReport(true);
  ASSERT_TRUE(ftOptions.excludeSQliteOutputReport());
  ASSERT_FALSE(ftOptions.isExcludeSQliteOutputReportDefaulted());
  // Reset
  ftOptions.resetExcludeSQliteOutputReport();
  ASSERT_FALSE(ftOptions.excludeSQliteOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeSQliteOutputReportDefaulted());
  ASSERT_TRUE(ftOptions.isExcludeSQliteOutputReportDefaulted());

  // Ctor Default
  ASSERT_FALSE(ftOptions.excludeHTMLOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeHTMLOutputReportDefaulted());
  // Set to opposite of default
  ftOptions.setExcludeHTMLOutputReport(true);
  ASSERT_TRUE(ftOptions.excludeHTMLOutputReport());
  ASSERT_FALSE(ftOptions.isExcludeHTMLOutputReportDefaulted());
  // Reset
  ftOptions.resetExcludeHTMLOutputReport();
  ASSERT_FALSE(ftOptions.excludeHTMLOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeHTMLOutputReportDefaulted());
  ASSERT_TRUE(ftOptions.isExcludeHTMLOutputReportDefaulted());

  // Ctor Default
  ASSERT_FALSE(ftOptions.excludeVariableDictionary());
  ASSERT_TRUE(ftOptions.isExcludeVariableDictionaryDefaulted());
  // Set to opposite of default
  ftOptions.setExcludeVariableDictionary(true);
  ASSERT_TRUE(ftOptions.excludeVariableDictionary());
  ASSERT_FALSE(ftOptions.isExcludeVariableDictionaryDefaulted());
  // Reset
  ftOptions.resetExcludeVariableDictionary();
  ASSERT_FALSE(ftOptions.excludeVariableDictionary());
  ASSERT_TRUE(ftOptions.isExcludeVariableDictionaryDefaulted());
  ASSERT_TRUE(ftOptions.isExcludeVariableDictionaryDefaulted());

  // Ctor Default
  ASSERT_FALSE(ftOptions.excludeSpaceTranslation());
  ASSERT_TRUE(ftOptions.isExcludeSpaceTranslationDefaulted());
  // Set to opposite of default
  ftOptions.setExcludeSpaceTranslation(true);
  ASSERT_TRUE(ftOptions.excludeSpaceTranslation());
  ASSERT_FALSE(ftOptions.isExcludeSpaceTranslationDefaulted());
  // Reset
  ftOptions.resetExcludeSpaceTranslation();
  ASSERT_FALSE(ftOptions.excludeSpaceTranslation());
  ASSERT_TRUE(ftOptions.isExcludeSpaceTranslationDefaulted());
  ASSERT_TRUE(ftOptions.isExcludeSpaceTranslationDefaulted());
}

TEST(Filetypes, RunOptions_overrideValuesWith) {

  RunOptions runOptions;
  ForwardTranslatorOptions ftOptions = runOptions.forwardTranslatorOptions();

  ASSERT_FALSE(runOptions.debug());
  ASSERT_TRUE(runOptions.isDebugDefaulted());
  ASSERT_FALSE(runOptions.epjson());
  ASSERT_TRUE(runOptions.isEpjsonDefaulted());
  ASSERT_FALSE(runOptions.fast());
  ASSERT_TRUE(runOptions.isFastDefaulted());
  ASSERT_FALSE(runOptions.preserveRunDir());
  ASSERT_TRUE(runOptions.isPreserveRunDirDefaulted());
  ASSERT_FALSE(runOptions.skipExpandObjects());
  ASSERT_TRUE(runOptions.isSkipExpandObjectsDefaulted());
  ASSERT_FALSE(runOptions.skipEnergyPlusPreprocess());
  ASSERT_TRUE(runOptions.isSkipEnergyPlusPreprocessDefaulted());
  ASSERT_FALSE(runOptions.skipZipResults());
  ASSERT_TRUE(runOptions.isSkipZipResultsDefaulted());
  ASSERT_TRUE(runOptions.cleanup());
  ASSERT_TRUE(runOptions.isCleanupDefaulted());

  ASSERT_TRUE(ftOptions.keepRunControlSpecialDays());
  ASSERT_TRUE(ftOptions.isKeepRunControlSpecialDaysDefaulted());
  ASSERT_FALSE(ftOptions.iPTabularOutput());
  ASSERT_TRUE(ftOptions.isIPTabularOutputDefaulted());
  ASSERT_FALSE(ftOptions.excludeLCCObjects());
  ASSERT_TRUE(ftOptions.isExcludeLCCObjectsDefaulted());
  ASSERT_FALSE(ftOptions.excludeSQliteOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeSQliteOutputReportDefaulted());
  ASSERT_FALSE(ftOptions.excludeHTMLOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeHTMLOutputReportDefaulted());
  ASSERT_FALSE(ftOptions.excludeVariableDictionary());
  ASSERT_TRUE(ftOptions.isExcludeVariableDictionaryDefaulted());
  ASSERT_FALSE(ftOptions.excludeSpaceTranslation());
  ASSERT_TRUE(ftOptions.isExcludeSpaceTranslationDefaulted());

  RunOptions otherRunOptions;
  ForwardTranslatorOptions otherftOptions = otherRunOptions.forwardTranslatorOptions();

  otherRunOptions.setDebug(false);                    // Explicitly set to default
  otherRunOptions.setEpjson(true);                    // Explicitly set to opposite of default
  otherftOptions.setKeepRunControlSpecialDays(true);  // Explicitly set to default
  otherftOptions.setIPTabularOutput(true);            // Explicitly set to opposite of default

  runOptions.overrideValuesWith(otherRunOptions);
  ASSERT_FALSE(runOptions.debug());
  ASSERT_FALSE(runOptions.isDebugDefaulted());  // No longer defaulted
  ASSERT_TRUE(runOptions.epjson());
  ASSERT_FALSE(runOptions.isEpjsonDefaulted());  // No longer defaulted
  ASSERT_FALSE(runOptions.fast());
  ASSERT_TRUE(runOptions.isFastDefaulted());
  ASSERT_FALSE(runOptions.preserveRunDir());
  ASSERT_TRUE(runOptions.isPreserveRunDirDefaulted());
  ASSERT_FALSE(runOptions.skipExpandObjects());
  ASSERT_TRUE(runOptions.isSkipExpandObjectsDefaulted());
  ASSERT_FALSE(runOptions.skipEnergyPlusPreprocess());
  ASSERT_TRUE(runOptions.isSkipEnergyPlusPreprocessDefaulted());
  ASSERT_FALSE(runOptions.skipEnergyPlusPreprocess());
  ASSERT_TRUE(runOptions.isSkipEnergyPlusPreprocessDefaulted());
  ASSERT_TRUE(runOptions.cleanup());
  ASSERT_TRUE(runOptions.isCleanupDefaulted());

  ASSERT_TRUE(ftOptions.keepRunControlSpecialDays());
  ASSERT_FALSE(ftOptions.isKeepRunControlSpecialDaysDefaulted());  // No longer defaulted
  ASSERT_TRUE(ftOptions.iPTabularOutput());
  ASSERT_FALSE(ftOptions.isIPTabularOutputDefaulted());  // No longer defaulted
  ASSERT_FALSE(ftOptions.excludeLCCObjects());
  ASSERT_TRUE(ftOptions.isExcludeLCCObjectsDefaulted());
  ASSERT_FALSE(ftOptions.excludeSQliteOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeSQliteOutputReportDefaulted());
  ASSERT_FALSE(ftOptions.excludeHTMLOutputReport());
  ASSERT_TRUE(ftOptions.isExcludeHTMLOutputReportDefaulted());
  ASSERT_FALSE(ftOptions.excludeVariableDictionary());
  ASSERT_TRUE(ftOptions.isExcludeVariableDictionaryDefaulted());
  ASSERT_FALSE(ftOptions.excludeSpaceTranslation());
  ASSERT_TRUE(ftOptions.isExcludeSpaceTranslationDefaulted());
}

TEST(Filetypes, WorkflowJSON_ValidateMeasures_Ok) {
  auto p = resourcesPath() / toPath("utilities/Filetypes/full.osw");
  WorkflowJSON w(p);
  EXPECT_TRUE(w.validateMeasures());
}

TEST(Filetypes, WorkflowJSON_ValidateMeasures_Missing) {
  auto p = resourcesPath() / toPath("workflow/invalid_measures/missing_a_measure.osw");
  ASSERT_TRUE(boost::filesystem::is_regular_file(p));
  WorkflowJSON w(p);
  StringStreamLogSink sink;
  sink.setLogLevel(Error);
  EXPECT_FALSE(w.validateMeasures());
  ASSERT_EQ(1, sink.logMessages().size());
  EXPECT_EQ("Cannot find measure 'NON_EXISTING_MEASURE_THIS_SHOULD_BE_CAUGHT'", sink.logMessages()[0].logMessage());
}

TEST(Filetypes, WorkflowJSON_ValidateMeasures_Unloadable) {
  auto p = resourcesPath() / toPath("workflow/invalid_measures/unloadable_measure.osw");
  ASSERT_TRUE(boost::filesystem::is_regular_file(p));
  WorkflowJSON w(p);
  StringStreamLogSink sink;
  sink.setLogLevel(Error);
  EXPECT_FALSE(w.validateMeasures());
  auto logMessages = sink.logMessages();
  ASSERT_EQ(3, logMessages.size());
  EXPECT_EQ("utilities.bcl.BCLXML", logMessages.at(0).logChannel());
  EXPECT_EQ("utilities.bcl.BCLMeasure", logMessages.at(1).logChannel());
  EXPECT_EQ("openstudio.WorkflowJSON", logMessages.at(2).logChannel());
  auto logMessage = sink.logMessages()[2].logMessage();
  EXPECT_TRUE(logMessage.find("Cannot load measure 'UnloadableMeasure' at '") != std::string::npos) << logMessage;
}

TEST(Filetypes, WorkflowJSON_ValidateMeasures_WrongOrder) {
  auto p = resourcesPath() / toPath("workflow/invalid_measures/wrong_measure_type_order.osw");
  ASSERT_TRUE(boost::filesystem::is_regular_file(p));
  WorkflowJSON w(p);
  StringStreamLogSink sink;
  sink.setLogLevel(Error);
  EXPECT_FALSE(w.validateMeasures());
  ASSERT_EQ(1, sink.logMessages().size());

  EXPECT_EQ("OpenStudio measure 'FakeModelMeasure' called after Energyplus simulation.", sink.logMessages()[0].logMessage());
}
