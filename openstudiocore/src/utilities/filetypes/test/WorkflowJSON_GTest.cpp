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

#include "../WorkflowJSON.hpp"
#include "../WorkflowStep.hpp"
#include "../WorkflowStep_Impl.hpp"

#include "../../core/Exception.hpp"

#include <resources.hxx>

using namespace openstudio;

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
    boost::optional<path> test = workflow.findFile(workflow.seedFile().get());
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("osversion/1_9_0/example.osm")), toString(*test));
    ASSERT_TRUE(workflow.weatherFile());
    EXPECT_EQ("./USA_CO_Golden-NREL.724666_TMY3.epw", workflow.weatherFile().get());
    test = workflow.findFile(workflow.weatherFile().get());
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/Filetypes/./USA_CO_Golden-NREL.724666_TMY3.epw")), toString(*test));
    test = workflow.findMeasure(toPath("SetWindowToWallRatioByFacade"));
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade")), toString(*test));

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
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/Filetypes/./USA_CO_Golden-NREL.724666_TMY3.epw")), toString(*test));
    test = workflow.findMeasure(toPath("SetWindowToWallRatioByFacade"));
    ASSERT_TRUE(test);
    EXPECT_EQ(toString(resourcesPath() / toPath("utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade")), toString(*test));

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

TEST(Filetypes, WorkflowJSON_Bad)
{
  path p = resourcesPath() / toPath("utilities/Filetypes/nonexistent.osw");
  EXPECT_THROW({ WorkflowJSON workflow(p); }, openstudio::Exception);
  EXPECT_FALSE(WorkflowJSON::load(p));

  p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
  EXPECT_THROW({ WorkflowJSON workflow(p); }, openstudio::Exception);
  EXPECT_FALSE(WorkflowJSON::load(p));
}