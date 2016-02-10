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
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/min.osw");
    EXPECT_TRUE(WorkflowJSON::load(p));

    WorkflowJSON workflow(p);
    EXPECT_EQ(p, workflow.path());
    EXPECT_EQ(p.parent_path() / toPath("/."), workflow.rootPath());
    EXPECT_TRUE(workflow.seedPath().empty());
    EXPECT_TRUE(workflow.weatherPath().empty());

    ASSERT_EQ(3u, workflow.workflowSteps().size());
    EXPECT_EQ("IncreaseWallRValue", workflow.workflowSteps()[0].measureDirName());
    EXPECT_EQ("IncreaseRoofRValue", workflow.workflowSteps()[1].measureDirName());
    EXPECT_EQ("DecreaseThermalMass", workflow.workflowSteps()[2].measureDirName());
  }catch(...){
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, WorkflowJSON_Full)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/full.osw");
    EXPECT_TRUE(WorkflowJSON::load(p));

    WorkflowJSON workflow(p);
    EXPECT_EQ(p, workflow.path());
    EXPECT_EQ(p.parent_path() / toPath("../root/"), workflow.rootPath());
    EXPECT_EQ(p.parent_path() / toPath("../root/seed/example.osm"), workflow.seedPath());
    EXPECT_EQ(p.parent_path() / toPath("../root/weather/Golden_NREL.epw"), workflow.weatherPath());

    ASSERT_EQ(3u, workflow.workflowSteps().size());
    EXPECT_EQ("IncreaseWallRValue", workflow.workflowSteps()[0].measureDirName());
    EXPECT_EQ("IncreaseRoofRValue", workflow.workflowSteps()[1].measureDirName());
    EXPECT_EQ("DecreaseThermalMass", workflow.workflowSteps()[2].measureDirName());
  } catch (...){
    ASSERT_TRUE(false);
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