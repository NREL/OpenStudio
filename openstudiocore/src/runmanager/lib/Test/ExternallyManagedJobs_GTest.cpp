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
*  Likey = cense along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include "SignalListener.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>
#include "../JobFactory.hpp"
#include <boost/filesystem/path.hpp>
#include "../RunManager.hpp"
#include "../Workflow.hpp"
#include "../WorkItem.hpp"
#include "../JSON.hpp"
#include "../../../model/Model.hpp"

#include <QDir>
#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"
#include "../../../utilities/core/PathHelpers.hpp"
#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace openstudio;
using namespace openstudio::runmanager;

TEST_F(RunManagerTestFixture, UpdateJobStatus)
{
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null").create();
  openstudio::runmanager::Job j2 = openstudio::runmanager::Workflow("Null").create();

  j.makeExternallyManaged();

  ASSERT_EQ(AdvancedStatus(AdvancedStatusEnum::Idle), j.status());
  ASSERT_EQ(AdvancedStatus(AdvancedStatusEnum::Idle), j2.status());

  ASSERT_ANY_THROW(j2.setStatus(AdvancedStatus(AdvancedStatusEnum::Queuing))); // not allowed to set status, not externally managed
  ASSERT_EQ(AdvancedStatus(AdvancedStatusEnum::Idle), j2.status()); // and no change expected

  ASSERT_NO_THROW(j.setStatus(AdvancedStatus(AdvancedStatusEnum::Queuing))); 
  ASSERT_EQ(AdvancedStatus(AdvancedStatusEnum::Queuing), j.status());
  
  // And note that advanced messages are allowed too
  ASSERT_NO_THROW(j.setStatus(AdvancedStatus(AdvancedStatusEnum::Queuing, "Waiting in remote queue"))); 
  ASSERT_EQ(AdvancedStatus(AdvancedStatusEnum::Queuing), j.status().value());
  ASSERT_EQ("Waiting in remote queue", j.status().description());

}

TEST_F(RunManagerTestFixture, UpdateJobUUID)
{
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null").create();
  openstudio::runmanager::Job j2 = openstudio::runmanager::Workflow("Null").create();
  openstudio::runmanager::Job j3 = openstudio::runmanager::Workflow("EnergyPlus").create();

  j.makeExternallyManaged();
  j2.makeExternallyManaged();
  j3.makeExternallyManaged();

  openstudio::UUID juuid = j.uuid();
  openstudio::UUID j2uuid = j2.uuid();
  openstudio::UUID j3uuid = j3.uuid();

  ASSERT_NE(juuid, j2uuid);
  ASSERT_NE(juuid, j3uuid);
  ASSERT_NE(j2uuid, j3uuid);

  ASSERT_ANY_THROW(j.updateJob(j2, false)); // not allowed to update UUID normally
  ASSERT_NO_THROW(j.updateJob(j2, true)); // allowed with optional uuid flag

  // now the UUID's should be equal
  ASSERT_EQ(j.uuid(), j2.uuid());
  

  ASSERT_ANY_THROW(j.updateJob(j3, false)); // update from j3 is never allowed because...
  ASSERT_ANY_THROW(j.updateJob(j3, true)); // the job types do not match
}

TEST_F(RunManagerTestFixture, UpdateJobUUIDWithTree)
{
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null->Null").create();
  openstudio::runmanager::Job j2 = openstudio::runmanager::Workflow("Null->Null").create();
  openstudio::runmanager::Job j3 = openstudio::runmanager::Workflow("Null->EnergyPlus").create();

  j.makeExternallyManaged();
  j2.makeExternallyManaged();
  j3.makeExternallyManaged();


  ASSERT_ANY_THROW(j.updateJob(j2, false)); // not allowed to update UUID normally
  ASSERT_NO_THROW(j.updateJob(j2, true)); // allowed with optional uuid flag

  // now the UUID's should be equal
  ASSERT_EQ(j.uuid(), j2.uuid());
 
  ASSERT_NE(j.uuid(), j3.uuid());
  ASSERT_ANY_THROW(j.updateJob(j3, true)); // try to update, but should fail because of child mismatch
  ASSERT_NE(j.uuid(), j3.uuid()); // make sure it didn't update the top level ID even with the child failure
 
}

TEST_F(RunManagerTestFixture, UpdateJobUUIDViaRunManager)
{
  openstudio::runmanager::Job j = openstudio::runmanager::Workflow("Null->Null").create();
  openstudio::runmanager::Job j2 = openstudio::runmanager::Workflow("Null->Null").create();

  j.makeExternallyManaged();
  j2.makeExternallyManaged();

  openstudio::runmanager::RunManager rm;
  
  ASSERT_ANY_THROW(rm.getJob(j.uuid()));  
  ASSERT_ANY_THROW(rm.getJob(j2.uuid()));  

  // note this j2 is not moved into the runmanager, "updateJob" creates a copy of the job tree
  // and adds the copy
  rm.updateJob(j2); // essentially add j2, because it didn't exist yet, and make it externally managed

  ASSERT_NE(j.uuid(), j2.uuid());
  ASSERT_NO_THROW(rm.getJob(j2.uuid()));

  rm.updateJob(j2.uuid(), j); // Update the job with J2's uuid to have j's uuid

  ASSERT_NO_THROW(rm.getJob(j.uuid())); // now this will with no longer throw

  // and now this one *will* throw
  //
  ASSERT_ANY_THROW(rm.getJob(j2.uuid()));


}


TEST_F(RunManagerTestFixture, ExternalJob)
{
  openstudio::path basedir = openstudio::tempDir() / openstudio::toPath("externaljob");
  openstudio::path rmpath1 = basedir / openstudio::toPath("rm1.db");
  openstudio::path rmpath2 = basedir / openstudio::toPath("rm2.db");
  openstudio::path rmpath3 = basedir / openstudio::toPath("rm3.db");

  boost::filesystem::create_directories(basedir);

  openstudio::runmanager::Workflow orig("ModelToIdf->ExpandObjects->EnergyPlus");
  openstudio::path origdir = basedir / openstudio::toPath("jobrun");
  openstudio::path origdir2 = basedir / openstudio::toPath("jobrun2");
  openstudio::path infile = origdir / openstudio::toPath("ExampleModel.osm");
  openstudio::path infile2 = origdir / openstudio::toPath("ExampleModel2.osm");
  openstudio::path weatherdir = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw");

  openstudio::model::Model exampleModel = openstudio::model::exampleModel();
  RunManager::simplifyModelForPerformance(exampleModel);
  exampleModel.save(infile, true);
  exampleModel.save(infile2, true);

  orig.setInputFiles(infile, weatherdir);
  
  // Build list of tools
  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(), 
          openstudio::path(), 
          openstudio::path(), 
          openstudio::path(),
          openstudio::path());
  orig.add(tools);

  openstudio::runmanager::Job jorig = orig.create(origdir);
  orig.setInputFiles(infile2, weatherdir);
  orig.addParam(openstudio::runmanager::JobParam("flatoutdir"));
  openstudio::runmanager::Job jorig2 = orig.create(origdir2);

  openstudio::runmanager::RunManager rorig(rmpath1, true, true, false); // runmanager is starting in paused state
  rorig.enqueue(jorig, true);
  rorig.enqueue(jorig2, true);


  {
    openstudio::runmanager::RunManager rnew(rmpath2, true, false, false);
    rnew.updateJob(openstudio::runmanager::detail::JSON::toJob(openstudio::runmanager::detail::JSON::toJSON(jorig2), true));
    Job updated = rnew.getJob(jorig2.uuid());
    EXPECT_FALSE(updated.lastRun());
  }

  EXPECT_FALSE(jorig2.lastRun());
  rorig.setPaused(false);
  rorig.waitForFinished();
  EXPECT_TRUE(jorig2.lastRun());

  FileInfo fi = jorig2.treeAllFiles().getLastByFilename("eplusout.sql");
  EXPECT_EQ(fi.fullPath, origdir2 / openstudio::toPath("5-EnergyPlus-0/eplusout.sql"));

  {
    openstudio::removeDirectory(basedir / openstudio::toPath("copied"));
    openstudio::copyDirectory(basedir / openstudio::toPath("jobrun"), basedir / openstudio::toPath("copied"));
    openstudio::runmanager::RunManager rnew(rmpath2, false, false, false);
    Job updated = rnew.getJob(jorig2.uuid());
    EXPECT_FALSE(updated.lastRun());
    std::string json = openstudio::runmanager::detail::JSON::toJSON(jorig2);
    rnew.updateJob(openstudio::runmanager::detail::JSON::toJob(json, true),
        basedir / openstudio::toPath("copied"));
    EXPECT_TRUE(updated.lastRun());
    FileInfo fi2 = updated.treeAllFiles().getLastByFilename("eplusout.sql");
    EXPECT_EQ(basedir / openstudio::toPath("copied") / openstudio::toPath("5-EnergyPlus-0/eplusout.sql"), fi2.fullPath);
    EXPECT_TRUE(fi2.exists);
    boost::filesystem::exists(fi2.fullPath);
  }

  {
    openstudio::removeDirectory(basedir / openstudio::toPath("copied2"));
    openstudio::copyDirectory(basedir / openstudio::toPath("jobrun"), basedir / openstudio::toPath("copied2"));
    openstudio::runmanager::RunManager rnew(rmpath3, true, true, false);

    // update with initial job having compatible structure but different uuids
    openstudio::runmanager::Job externalJob = orig.create(openstudio::path());
    externalJob.makeExternallyManaged();
    externalJob.setStatus(AdvancedStatusEnum(AdvancedStatusEnum::Queuing));
    rnew.updateJob(externalJob);
    EXPECT_FALSE(externalJob.uuid() == jorig2.uuid());
    EXPECT_FALSE(externalJob.lastRun());

    // update to waiting in queue
    externalJob.setStatus(AdvancedStatusEnum(AdvancedStatusEnum::WaitingInQueue));

    // since we are still operating on the original, not the copy that RunManager created for it
    // (something special that only updateJob() does, not enqueue(), it's documented in the function
    // call), we need to update with the new status.
    rnew.updateJob(externalJob);
    EXPECT_FALSE(externalJob.uuid() == jorig2.uuid());
    EXPECT_FALSE(externalJob.lastRun());

    // get update from server with real uuids
    std::string json = openstudio::runmanager::detail::JSON::toJSON(jorig2);
    openstudio::runmanager::Job externalJob2 = openstudio::runmanager::detail::JSON::toJob(json, true);
    rnew.updateJob(externalJob.uuid(), externalJob2);
    EXPECT_FALSE(externalJob.uuid() == jorig2.uuid());
    EXPECT_TRUE(externalJob2.uuid() == jorig2.uuid());
    EXPECT_TRUE(externalJob2.lastRun());

    // download detailed results
    rnew.updateJob(externalJob2, basedir / openstudio::toPath("copied2"));
    

    // we now need to get the job that runmanager has in it, not the copies we have locally,
    // remember, runmanager, on an "updateJob" call copies the job if it doesn't already
    // exist, it does not take ownership of the job you pass in
    Job externalJob2FromRM = rnew.getJob(externalJob2.uuid());
    EXPECT_TRUE(externalJob2FromRM.lastRun());
    FileInfo fi2 = externalJob2FromRM.treeAllFiles().getLastByFilename("eplusout.sql");
    EXPECT_EQ(basedir / openstudio::toPath("copied2") / openstudio::toPath("5-EnergyPlus-0/eplusout.sql"), fi2.fullPath);
    EXPECT_TRUE(fi2.exists);
    boost::filesystem::exists(fi2.fullPath);
  }

  {
    openstudio::runmanager::RunManager rnew(rmpath2, false, false, false);
    Job updated = rnew.getJob(jorig2.uuid());
    EXPECT_TRUE(updated.lastRun());
  }

}

TEST_F(RunManagerTestFixture, ExternallyManagedJobs)
{
  test::Test_Listener listener;

  openstudio::runmanager::Workflow orig("Null->Null->Null->Null");
  openstudio::runmanager::Job jorig = orig.create(openstudio::tempDir() / openstudio::toPath("ExternallyManagedJobs"));
  
  openstudio::runmanager::RunManager rorig(true, false); // runmanager is starting in paused state
  rorig.enqueue(jorig, true);

  std::vector<Job> jobs = rorig.getJobs();

  // original state of jobs on original runmanager
  for (std::vector<Job>::const_iterator itr = jobs.begin();
       itr != jobs.end();
       ++itr)
  {
    if (!itr->parent())
    {
      EXPECT_TRUE(itr->runnable());
    }
    EXPECT_FALSE(itr->lastRun());
    EXPECT_FALSE(itr->externallyManaged());
  }


  openstudio::runmanager::RunManager rnew(true, false); // runmanager is starting in paused state

  EXPECT_TRUE(rnew.getJobs().empty());

  rnew.updateJobs(rorig.jobsToJson(), true);

  jobs = rnew.getJobs();
  EXPECT_EQ(jobs.size(), 4u);

  // original state of jobs in new runmanager
  for (std::vector<Job>::const_iterator itr = jobs.begin();
       itr != jobs.end();
       ++itr)
  {
    EXPECT_FALSE(itr->runnable());
    EXPECT_FALSE(itr->lastRun());
    EXPECT_TRUE(itr->externallyManaged());
    itr->connect(SIGNAL(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors &)), &listener, SLOT(listen()));
  }

  // unpause rnew
  rnew.setPaused(false);
  openstudio::System::msleep(1000);

  jobs = rnew.getJobs();
  EXPECT_EQ(jobs.size(), 4u);

  // verify jobs have not run in new runmanager
  for (std::vector<Job>::const_iterator itr = jobs.begin();
       itr != jobs.end();
       ++itr)
  {
    EXPECT_FALSE(itr->runnable());
    EXPECT_FALSE(itr->lastRun());
    EXPECT_TRUE(itr->externallyManaged());
  }

  // start up the original runmanager
  rorig.setPaused(false);
  rorig.waitForFinished();

  //verify rorig jobs have run
  jobs = rorig.getJobs();
  EXPECT_EQ(jobs.size(), 4u);
  for (std::vector<Job>::const_iterator itr = jobs.begin();
       itr != jobs.end();
       ++itr)
  {
    EXPECT_FALSE(itr->runnable());
    EXPECT_TRUE(itr->lastRun());
    EXPECT_FALSE(itr->externallyManaged());
  }


  // verify rnew jobs are still not run
  jobs = rnew.getJobs();
  EXPECT_EQ(jobs.size(), 4u);
  // verify jobs have not run
  for (std::vector<Job>::const_iterator itr = jobs.begin();
       itr != jobs.end();
       ++itr)
  {
    EXPECT_FALSE(itr->runnable());
    EXPECT_FALSE(itr->lastRun());
    EXPECT_TRUE(itr->externallyManaged());
  }

  EXPECT_FALSE(listener.slot_fired);

  rnew.updateJobs(rorig.jobsToJson(), true);
  jobs = rnew.getJobs();
  EXPECT_EQ(jobs.size(), 4u);

  // verify jobs have run
  for (std::vector<Job>::const_iterator itr = jobs.begin();
       itr != jobs.end();
       ++itr)
  {
    EXPECT_FALSE(itr->runnable());
    EXPECT_TRUE(itr->lastRun());
    EXPECT_TRUE(itr->externallyManaged());
  }

  // make sure signal is sent
  openstudio::Application::instance().processEvents();
  EXPECT_TRUE(listener.slot_fired);
}
