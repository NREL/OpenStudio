/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
#include "RunManagerTestFixture.hpp"
#include "../Job.hpp"
#include "../Workflow.hpp"
#include "../JobFactory.hpp"



TEST_F(RunManagerTestFixture, MergeSimpleNullJobsTest)
{
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null->Null->Null->Null").create();

  EXPECT_EQ(j.children().size(), static_cast<size_t>(1));
  EXPECT_EQ(j.children().at(0).children().size(), static_cast<size_t>(1));
  EXPECT_EQ(j.children().at(0).children().at(0).children().size(), static_cast<size_t>(1));
  EXPECT_EQ(j.children().at(0).children().at(0).children().at(0).children().size(), static_cast<size_t>(0));

  openstudio::runmanager::JobFactory::optimizeJobTree(j);

  EXPECT_EQ(j.children().size(), static_cast<size_t>(0));
}


TEST_F(RunManagerTestFixture, MergeComplexNullJobsTest)
{
  // Expect the 4 children to be merged down one level, but still stay as 4 children
  // Also, expect that each child be merge down into one level

  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null->Null").create();

  EXPECT_EQ(j.children().at(0).children().size(), static_cast<size_t>(0));

  j.children().at(0).addChild(openstudio::runmanager::Workflow("Null->Null").create());
  j.children().at(0).addChild(openstudio::runmanager::Workflow("Null->Null").create());
  j.children().at(0).addChild(openstudio::runmanager::Workflow("Null->Null").create());
  j.children().at(0).addChild(openstudio::runmanager::Workflow("Null->Null").create());

  EXPECT_EQ(j.children().size(), static_cast<size_t>(1));
  EXPECT_EQ(j.children().at(0).children().size(), static_cast<size_t>(4));
  EXPECT_EQ(j.children().at(0).children().at(0).children().size(), static_cast<size_t>(1));

  openstudio::runmanager::JobFactory::optimizeJobTree(j);


  EXPECT_EQ(j.children().size(), static_cast<size_t>(4));
  EXPECT_EQ(j.children().at(0).children().size(), static_cast<size_t>(0));
  EXPECT_EQ(j.children().at(1).children().size(), static_cast<size_t>(0));
  EXPECT_EQ(j.children().at(2).children().size(), static_cast<size_t>(0));
  EXPECT_EQ(j.children().at(3).children().size(), static_cast<size_t>(0));
}

TEST_F(RunManagerTestFixture, MergeFinishedNullJobsFailureTest)
{
  // Expect that finished job will not merge because there's another child job
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null->EnergyPlus").create();


  j.setFinishedJob(openstudio::runmanager::Workflow("Null").create());
  EXPECT_EQ(j.children().size(), static_cast<size_t>(1));
  EXPECT_TRUE(j.finishedJob());

  openstudio::runmanager::JobFactory::optimizeJobTree(j);

  EXPECT_EQ(j.children().size(), static_cast<size_t>(1));
  EXPECT_TRUE(j.finishedJob());
}


TEST_F(RunManagerTestFixture, MergeFinishedNullJobsSuccessTest)
{
  // Expect finishedJob to merge, since there are no children
  // 
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null->Null").create();


  j.setFinishedJob(openstudio::runmanager::Workflow("Null").create());
  EXPECT_EQ(j.children().size(), static_cast<size_t>(1));
  EXPECT_TRUE(j.finishedJob());

  openstudio::runmanager::JobFactory::optimizeJobTree(j);

  EXPECT_EQ(j.children().size(), static_cast<size_t>(0));
  EXPECT_FALSE(j.finishedJob());
}

// ETH@20130806: Removing MergeMixedJobsTest because ModelRulesets have been deprecated.
// Something like it should be reinstantiated when the feature to merge RubyJobs is added.
