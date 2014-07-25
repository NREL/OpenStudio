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
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>
#include <OpenStudio.hxx>
#include "../JobFactory.hpp"
#include <boost/filesystem/path.hpp>
#include "../RunManager.hpp"
#include "../Workflow.hpp"
#include "../RubyJobUtils.hpp"
#include <QDir>
#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"
#include "../../../utilities/core/Logger.hpp"

#ifdef _MSC_VER
#include <Windows.h>
#endif

void checkFinishedAfter(const openstudio::runmanager::Job &t_before, const openstudio::runmanager::Job &t_after)
{
  EXPECT_TRUE(t_before.lastRun());
  EXPECT_TRUE(t_after.lastRun());

  EXPECT_TRUE(t_before.ranBefore(t_after) || (!t_before.ranBefore(t_after) && !t_after.ranBefore(t_before))); // timer resolution requires us to consider the two jobs having run at the same time as "runbefore"
  EXPECT_FALSE(t_after.ranBefore(t_before));

  EXPECT_FALSE(t_before.outOfDate());
  EXPECT_FALSE(t_after.outOfDate());
}

TEST_F(RunManagerTestFixture, JobRunOrder)
{
  openstudio::path outdir1 = openstudio::tempDir() / openstudio::toPath("JobRunOrder") / openstudio::toPath("1");
  openstudio::path outdir2 = openstudio::tempDir() / openstudio::toPath("JobRunOrder") / openstudio::toPath("2");

  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("JobRunOrderDB");
  openstudio::runmanager::RunManager kit(db, true);
  kit.setPaused(true);

  openstudio::runmanager::Job successful11(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful12(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful13(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful14(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful15(openstudio::runmanager::JobFactory::createNullJob());

  openstudio::runmanager::Job successful21(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful22(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful23(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful24(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful25(openstudio::runmanager::JobFactory::createNullJob());

  openstudio::runmanager::Job successful31(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful32(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful33(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful34(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job successful35(openstudio::runmanager::JobFactory::createNullJob());

  openstudio::runmanager::Job headjob(openstudio::runmanager::Workflow("Null").create(outdir1));

  openstudio::runmanager::Job finished1(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job finished2(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job finished3(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job finished4(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job finished5(openstudio::runmanager::JobFactory::createNullJob());

  successful11.addChild(successful12);
  successful12.addChild(successful13);
  successful13.addChild(successful14);
  successful14.addChild(successful15);

  successful21.addChild(successful22);
  successful22.addChild(successful23);
  successful23.addChild(successful24);
  successful24.addChild(successful25);

  successful31.addChild(successful32);
  successful32.addChild(successful33);
  successful33.addChild(successful34);
  successful34.addChild(successful35);

  finished1.addChild(finished2);
  finished2.addChild(finished3);
  finished3.addChild(finished4);
  finished4.addChild(finished5);

  headjob.addChild(successful11);
  headjob.addChild(successful21);
  headjob.addChild(successful31);

  headjob.setFinishedJob(finished1);

  kit.enqueue(headjob, false);


  openstudio::runmanager::Job failed1(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job failed2(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job failed3(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job failed4(openstudio::runmanager::JobFactory::createRubyJob(
        openstudio::runmanager::Tools(), openstudio::runmanager::JobParams(), openstudio::runmanager::Files()));
  openstudio::runmanager::Job failed5(openstudio::runmanager::JobFactory::createNullJob());

  openstudio::runmanager::Job headfailed(openstudio::runmanager::Workflow("Null").create(outdir2));

  openstudio::runmanager::Job failedfinished1(openstudio::runmanager::JobFactory::createNullJob());
  openstudio::runmanager::Job failedfinished2(openstudio::runmanager::JobFactory::createNullJob());

  failed1.addChild(failed2);
  failed2.addChild(failed3);
  failed3.addChild(failed4);
  failed4.addChild(failed5);

  failedfinished1.addChild(failedfinished2);

  headfailed.addChild(failed1);
  headfailed.setFinishedJob(failedfinished1);

  kit.enqueue(headfailed, false);

  kit.setPaused(false);
  kit.waitForFinished();

  // successful tree first
  EXPECT_TRUE(successful11.errors().succeeded());
  EXPECT_TRUE(successful12.errors().succeeded());
  EXPECT_TRUE(successful13.errors().succeeded());
  EXPECT_TRUE(successful14.errors().succeeded());
  EXPECT_TRUE(successful15.errors().succeeded());
  EXPECT_TRUE(successful21.errors().succeeded());
  EXPECT_TRUE(successful22.errors().succeeded());
  EXPECT_TRUE(successful23.errors().succeeded());
  EXPECT_TRUE(successful24.errors().succeeded());
  EXPECT_TRUE(successful25.errors().succeeded());
  EXPECT_TRUE(successful31.errors().succeeded());
  EXPECT_TRUE(successful32.errors().succeeded());
  EXPECT_TRUE(successful33.errors().succeeded());
  EXPECT_TRUE(successful34.errors().succeeded());
  EXPECT_TRUE(successful35.errors().succeeded());
  EXPECT_TRUE(headjob.errors().succeeded());
  EXPECT_TRUE(finished1.errors().succeeded());
  EXPECT_TRUE(finished2.errors().succeeded());
  EXPECT_TRUE(finished3.errors().succeeded());
  EXPECT_TRUE(finished4.errors().succeeded());
  EXPECT_TRUE(finished5.errors().succeeded());

  checkFinishedAfter(headjob, successful11);
  checkFinishedAfter(headjob, successful21);
  checkFinishedAfter(headjob, successful31);
  checkFinishedAfter(headjob, finished1);

  checkFinishedAfter(successful11, successful12);
  checkFinishedAfter(successful12, successful13);
  checkFinishedAfter(successful13, successful14);
  checkFinishedAfter(successful14, successful15);

  checkFinishedAfter(successful21, successful22);
  checkFinishedAfter(successful22, successful23);
  checkFinishedAfter(successful23, successful24);
  checkFinishedAfter(successful24, successful25);

  checkFinishedAfter(successful31, successful32);
  checkFinishedAfter(successful32, successful33);
  checkFinishedAfter(successful33, successful34);
  checkFinishedAfter(successful34, successful35);

  checkFinishedAfter(finished1, finished2);
  checkFinishedAfter(finished2, finished3);
  checkFinishedAfter(finished3, finished4);
  checkFinishedAfter(finished4, finished5);

  checkFinishedAfter(successful15, finished1);
  checkFinishedAfter(successful25, finished1);
  checkFinishedAfter(successful35, finished1);

  EXPECT_EQ(headjob.treeLastRun(), finished5.lastRun());
  EXPECT_EQ(headjob.treeStatus(), openstudio::runmanager::TreeStatusEnum::Finished);

  // failed job tests 
  checkFinishedAfter(failed1, failed2);
  checkFinishedAfter(failed2, failed3);
  checkFinishedAfter(failed3, failed4);

  EXPECT_TRUE(failed1.errors().succeeded());
  EXPECT_TRUE(failed2.errors().succeeded());
  EXPECT_TRUE(failed3.errors().succeeded());
  EXPECT_FALSE(failed4.errors().succeeded());
  EXPECT_FALSE(failed5.errors().succeeded());

  EXPECT_FALSE(failed5.lastRun());
  EXPECT_FALSE(failedfinished1.lastRun());
  EXPECT_FALSE(failedfinished2.lastRun());
  EXPECT_FALSE(failedfinished1.errors().succeeded());
  EXPECT_FALSE(failedfinished2.errors().succeeded());

  EXPECT_TRUE(headfailed.errors().succeeded());
  EXPECT_FALSE(headfailed.treeLastRun());
  EXPECT_EQ(headfailed.treeStatus(), openstudio::runmanager::TreeStatusEnum::Failed);

}



