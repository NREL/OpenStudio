/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
#include <runmanager/lib/JobFactory.hpp>
#include <boost/filesystem/path.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/WorkItem.hpp>
#include <runmanager/lib/JSON.hpp>

#include <QDir>
#include <utilities/core/Application.hpp>
#include <utilities/core/System.hpp>
#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace openstudio;
using namespace openstudio::runmanager;

TEST_F(RunManagerTestFixture, ExternalJob)
{
  openstudio::path rmpath1 = openstudio::tempDir() / openstudio::toPath("externaljob/rm1.db");
  openstudio::path rmpath2 = openstudio::tempDir() / openstudio::toPath("externaljob/rm2.db");

  boost::filesystem::create_directories(openstudio::tempDir() / openstudio::toPath("externaljob"));

  openstudio::runmanager::Workflow orig("Null->Null->Null->Null");
  openstudio::runmanager::Job jorig = orig.create(openstudio::tempDir() / openstudio::toPath("ExternallyManagedJobs"));

  openstudio::runmanager::RunManager rorig(rmpath1, true, true, false); // runmanager is starting in paused state
  rorig.enqueue(jorig, true);


  {
    openstudio::runmanager::RunManager rnew(rmpath2, true, false, false);
    rnew.updateJob(openstudio::runmanager::detail::JSON::toJob(openstudio::runmanager::detail::JSON::toJSON(jorig), true));
    Job updated = rnew.getJob(jorig.uuid());
    EXPECT_FALSE(updated.lastRun());
  }

  EXPECT_FALSE(jorig.lastRun());
  rorig.setPaused(false);
  rorig.waitForFinished();
  EXPECT_TRUE(jorig.lastRun());

  {
    openstudio::runmanager::RunManager rnew(rmpath2, false, false, false);
    Job updated = rnew.getJob(jorig.uuid());
    EXPECT_FALSE(updated.lastRun());
    rnew.updateJob(openstudio::runmanager::detail::JSON::toJob(openstudio::runmanager::detail::JSON::toJSON(jorig), true));
    EXPECT_TRUE(updated.lastRun());
  }

  {
    openstudio::runmanager::RunManager rnew(rmpath2, false, false, false);
    Job updated = rnew.getJob(jorig.uuid());
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
