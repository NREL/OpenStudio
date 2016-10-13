/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
