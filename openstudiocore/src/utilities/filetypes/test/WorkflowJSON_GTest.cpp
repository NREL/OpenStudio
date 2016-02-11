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
    EXPECT_EQ(p, workflow.path());
    EXPECT_EQ(p.parent_path() / toPath("/."), workflow.rootPath());
    EXPECT_TRUE(workflow.seedPath().empty());
    EXPECT_TRUE(workflow.weatherPath().empty());

    EXPECT_TRUE(workflow.saveAs(p2));

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    EXPECT_EQ("IncreaseWallRValue", workflowSteps[0].measureDirName());
    ASSERT_TRUE(workflowSteps[0].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[0].getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, workflowSteps[0].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[0].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, workflowSteps[0].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[0].getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, workflowSteps[0].getArgument("operating_expenses")->valueAsInteger());

    EXPECT_EQ("IncreaseRoofRValue", workflowSteps[1].measureDirName());
    ASSERT_TRUE(workflowSteps[1].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[1].getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, workflowSteps[1].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[1].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[1].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, workflowSteps[1].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[1].getArgument("space_type"));
    ASSERT_EQ(VariantType::String, workflowSteps[1].getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", workflowSteps[1].getArgument("space_type")->valueAsString());

    EXPECT_EQ("DecreaseThermalMass", workflowSteps[2].measureDirName());
    ASSERT_TRUE(workflowSteps[2].getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, workflowSteps[2].getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, workflowSteps[2].getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(workflowSteps[2].getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[2].getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, workflowSteps[2].getArgument("percent_decrease")->valueAsInteger());

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try{
    WorkflowJSON workflow(p2);
    EXPECT_FALSE(workflow.checkForUpdates());
    EXPECT_EQ(p2, workflow.path());
    EXPECT_EQ(p2.parent_path() / toPath("/."), workflow.rootPath());
    EXPECT_TRUE(workflow.seedPath().empty());
    EXPECT_TRUE(workflow.weatherPath().empty());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    EXPECT_EQ("IncreaseWallRValue", workflowSteps[0].measureDirName());
    ASSERT_TRUE(workflowSteps[0].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[0].getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, workflowSteps[0].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[0].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, workflowSteps[0].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[0].getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, workflowSteps[0].getArgument("operating_expenses")->valueAsInteger());

    EXPECT_EQ("IncreaseRoofRValue", workflowSteps[1].measureDirName());
    ASSERT_TRUE(workflowSteps[1].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[1].getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, workflowSteps[1].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[1].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[1].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, workflowSteps[1].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[1].getArgument("space_type"));
    ASSERT_EQ(VariantType::String, workflowSteps[1].getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", workflowSteps[1].getArgument("space_type")->valueAsString());

    EXPECT_EQ("DecreaseThermalMass", workflowSteps[2].measureDirName());
    ASSERT_TRUE(workflowSteps[2].getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, workflowSteps[2].getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, workflowSteps[2].getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(workflowSteps[2].getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[2].getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, workflowSteps[2].getArgument("percent_decrease")->valueAsInteger());

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
    EXPECT_EQ(p, workflow.path());
    EXPECT_EQ(p.parent_path() / toPath("../root/"), workflow.rootPath());
    EXPECT_EQ(p.parent_path() / toPath("../root/seed/example.osm"), workflow.seedPath());
    EXPECT_EQ(p.parent_path() / toPath("../root/weather/Golden_NREL.epw"), workflow.weatherPath());

    EXPECT_TRUE(workflow.saveAs(p2));

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    EXPECT_EQ("IncreaseWallRValue", workflowSteps[0].measureDirName());
    ASSERT_TRUE(workflowSteps[0].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[0].getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, workflowSteps[0].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[0].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, workflowSteps[0].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[0].getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, workflowSteps[0].getArgument("operating_expenses")->valueAsInteger());

    EXPECT_EQ("IncreaseRoofRValue", workflowSteps[1].measureDirName());
    ASSERT_TRUE(workflowSteps[1].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[1].getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, workflowSteps[1].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[1].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[1].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, workflowSteps[1].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[1].getArgument("space_type"));
    ASSERT_EQ(VariantType::String, workflowSteps[1].getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", workflowSteps[1].getArgument("space_type")->valueAsString());

    EXPECT_EQ("DecreaseThermalMass", workflowSteps[2].measureDirName());
    ASSERT_TRUE(workflowSteps[2].getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, workflowSteps[2].getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, workflowSteps[2].getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(workflowSteps[2].getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[2].getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, workflowSteps[2].getArgument("percent_decrease")->valueAsInteger());

  } catch (const std::exception& e){
    EXPECT_TRUE(false) << e.what();
  }

  ASSERT_TRUE(WorkflowJSON::load(p2));
  try{
    WorkflowJSON workflow(p2);
    EXPECT_FALSE(workflow.checkForUpdates());
    EXPECT_EQ(p2, workflow.path());
    EXPECT_EQ(p2.parent_path() / toPath("../root/"), workflow.rootPath());
    EXPECT_EQ(p2.parent_path() / toPath("../root/seed/example.osm"), workflow.seedPath());
    EXPECT_EQ(p2.parent_path() / toPath("../root/weather/Golden_NREL.epw"), workflow.weatherPath());

    std::vector<WorkflowStep> workflowSteps = workflow.workflowSteps();
    ASSERT_EQ(3u, workflowSteps.size());

    EXPECT_EQ("IncreaseWallRValue", workflowSteps[0].measureDirName());
    ASSERT_TRUE(workflowSteps[0].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[0].getArgument("cost")->variantType().value());
    EXPECT_EQ(123.321, workflowSteps[0].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[0].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(5, workflowSteps[0].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[0].getArgument("operating_expenses"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[0].getArgument("operating_expenses")->variantType().value());
    EXPECT_EQ(0, workflowSteps[0].getArgument("operating_expenses")->valueAsInteger());

    EXPECT_EQ("IncreaseRoofRValue", workflowSteps[1].measureDirName());
    ASSERT_TRUE(workflowSteps[1].getArgument("cost"));
    ASSERT_EQ(VariantType::Double, workflowSteps[1].getArgument("cost")->variantType().value());
    EXPECT_EQ(321.123, workflowSteps[1].getArgument("cost")->valueAsDouble());
    ASSERT_TRUE(workflowSteps[1].getArgument("percent_increase"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[1].getArgument("percent_increase")->variantType().value());
    EXPECT_EQ(15, workflowSteps[1].getArgument("percent_increase")->valueAsInteger());
    ASSERT_TRUE(workflowSteps[1].getArgument("space_type"));
    ASSERT_EQ(VariantType::String, workflowSteps[1].getArgument("space_type")->variantType().value());
    EXPECT_EQ("*All*", workflowSteps[1].getArgument("space_type")->valueAsString());

    EXPECT_EQ("DecreaseThermalMass", workflowSteps[2].measureDirName());
    ASSERT_TRUE(workflowSteps[2].getArgument("has_bool"));
    ASSERT_EQ(VariantType::Boolean, workflowSteps[2].getArgument("has_bool")->variantType().value());
    EXPECT_EQ(true, workflowSteps[2].getArgument("has_bool")->valueAsBoolean());
    ASSERT_TRUE(workflowSteps[2].getArgument("percent_decrease"));
    ASSERT_EQ(VariantType::Integer, workflowSteps[2].getArgument("percent_decrease")->variantType().value());
    EXPECT_EQ(5, workflowSteps[2].getArgument("percent_decrease")->valueAsInteger());

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