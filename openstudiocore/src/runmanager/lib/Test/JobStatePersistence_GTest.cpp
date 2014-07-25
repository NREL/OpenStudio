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
#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../LocalProcessCreator.hpp"
#include "../RubyJobUtils.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/filetypes/EpwFile.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

#include "../../../utilities/core/System.hpp"
#include "../../../utilities/core/Path.hpp"

#include <utilities/idd/OS_TimeDependentValuation_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include "../../../ruleset/OSArgument.hpp"

#include <fstream>

#include <resources.hxx>

using namespace openstudio;

TEST_F(RunManagerTestFixture, JobStatePersistence_DBUpgradeTest)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("JobStatePersistence_DBUpgradeTest");
  openstudio::path db = outdir / openstudio::toPath("test.db");
  boost::filesystem::create_directories(outdir);
  boost::filesystem::copy_file(resourcesPath() / openstudio::toPath("runmanager/databasev1.db"), db, boost::filesystem::copy_option::overwrite_if_exists);

  {
    // open
    openstudio::runmanager::RunManager rm(db, false, true);
    ASSERT_EQ(rm.getJobs().size(), 5u);  
  }

  {
    // reopen
    openstudio::runmanager::RunManager rm(db, false, true);
    ASSERT_EQ(rm.getJobs().size(), 5u);  
  }
}

TEST_F(RunManagerTestFixture, JobStatePersistence_NoErrors)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("JobStatePersistence_NoErrors");
  boost::filesystem::create_directories(outdir);
  openstudio::path db = outdir / openstudio::toPath("test.db");


  {
    openstudio::runmanager::RunManager rm(db, true, true);
    openstudio::runmanager::Workflow wf("null->null->null");
    rm.enqueue(wf.create(outdir), true);
    ASSERT_TRUE(rm.workPending());
    ASSERT_EQ(rm.getJobs().size(), 3u);

    rm.setPaused(false);
    rm.waitForFinished();

    ASSERT_FALSE(rm.workPending());
  }

  openstudio::System::msleep(1000);

  {
    openstudio::runmanager::RunManager rm(db, false, true);
    openstudio::DateTime now = openstudio::DateTime::now();

    ASSERT_FALSE(rm.workPending());

    std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();
    ASSERT_EQ(jobs.size(), 3u);

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      ASSERT_TRUE(itr->lastRun());
      ASSERT_LT(*(itr->lastRun()), now);
      openstudio::runmanager::JobErrors errors = itr->errors();
      ASSERT_TRUE(errors.allErrors.empty());
      ASSERT_TRUE(errors.succeeded());
    }

    rm.setPaused(false);
    rm.waitForFinished();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      ASSERT_TRUE(itr->lastRun());
      ASSERT_LT(*(itr->lastRun()), now);
      openstudio::runmanager::JobErrors errors = itr->errors();
      ASSERT_TRUE(errors.allErrors.empty());
      ASSERT_TRUE(errors.succeeded());
    }

    ASSERT_FALSE(rm.workPending());
  }
}


TEST_F(RunManagerTestFixture, JobStatePersistence_Errors)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("JobStatePersistence_Errors");
  openstudio::path idffile = outdir / openstudio::toPath("in.idf");
  openstudio::path db = outdir / openstudio::toPath("test.db");
  boost::filesystem::create_directories(outdir);

  openstudio::runmanager::JobErrors e;

  std::set<openstudio::runmanager::FileInfo> eplusoutfiles;

  {
    {
      std::ofstream ofs(openstudio::toString(idffile).c_str());
      ofs << "! EmptyFile\n";
    }

    openstudio::runmanager::RunManager rm(db, true, true);
    // the energyplus job should fail and the null job not run
    openstudio::runmanager::Workflow wf("energyplus->null");

    openstudio::runmanager::Tools tools 
      = openstudio::runmanager::ConfigOptions::makeTools(
          energyPlusExePath().parent_path(), 
          openstudio::path(), 
          openstudio::path(), 
          openstudio::path(),
          openstudio::path());

    wf.add(tools);

    rm.enqueue(wf.create(outdir, idffile), true);
    ASSERT_TRUE(rm.workPending());
    ASSERT_EQ(rm.getJobs().size(), 2u);
    ASSERT_EQ(rm.getJobs()[0].jobType(), openstudio::runmanager::JobType::EnergyPlus);

    rm.setPaused(false);
    rm.waitForFinished();


    std::vector<openstudio::runmanager::FileInfo> outfiles = rm.getJobs()[0].outputFiles();
    ASSERT_TRUE(!outfiles.empty());
    ASSERT_FALSE(rm.getJobs()[0].allFiles().getAllByExtension("err").files().empty());
    e = rm.getJobs()[0].errors();
    ASSERT_FALSE(e.allErrors.empty());
    eplusoutfiles.insert(outfiles.begin(), outfiles.end());

    ASSERT_FALSE(rm.workPending());
  }

  openstudio::System::msleep(1000);

  {
    openstudio::runmanager::RunManager rm(db, false, true);

    openstudio::DateTime now = openstudio::DateTime::now();

    ASSERT_FALSE(rm.workPending());

    std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();
    ASSERT_EQ(jobs.size(), 2u);

    {
      openstudio::runmanager::Job job = jobs[0];
      ASSERT_EQ(job.jobType(), openstudio::runmanager::JobType::EnergyPlus);
      ASSERT_TRUE(job.lastRun());
      ASSERT_LT(*(job.lastRun()), now);
      openstudio::runmanager::JobErrors errors = job.errors();
      ASSERT_FALSE(errors.allErrors.empty());
      ASSERT_FALSE(errors.succeeded());

      std::set<openstudio::runmanager::FileInfo> reloadedeplusoutfiles;
      std::vector<openstudio::runmanager::FileInfo> outfiles = job.outputFiles();
      ASSERT_TRUE(!outfiles.empty());
      ASSERT_FALSE(job.allFiles().getAllByExtension("err").files().empty());
      reloadedeplusoutfiles.insert(outfiles.begin(), outfiles.end());
      ASSERT_EQ(eplusoutfiles, reloadedeplusoutfiles);

      openstudio::runmanager::JobErrors e2 = rm.getJobs()[0].errors();
      ASSERT_EQ(e, e2);
    }

    {
      openstudio::runmanager::Job job = jobs[1];
      ASSERT_EQ(job.jobType(), openstudio::runmanager::JobType::Null);
      ASSERT_FALSE(job.lastRun());
      openstudio::runmanager::JobErrors errors = job.errors();
      ASSERT_TRUE(errors.allErrors.empty());
    }


    rm.setPaused(false);
    rm.waitForFinished();

    ASSERT_FALSE(rm.workPending());
  }

}


TEST_F(RunManagerTestFixture, JobStatePersistence_Uncompleted)
{
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("JobStatePersistence_Uncompleted");
  openstudio::path db = outdir / openstudio::toPath("test.db");
  boost::filesystem::create_directories(outdir);


  {
    openstudio::runmanager::RunManager rm(db, true, true);
    openstudio::runmanager::Workflow wf("null->null->null");
    rm.enqueue(wf.create(outdir), true);
    ASSERT_TRUE(rm.workPending());
    ASSERT_EQ(rm.getJobs().size(), 3u);

    rm.setPaused(false);
    rm.waitForFinished();

    ASSERT_FALSE(rm.workPending());

    rm.setPaused(true);
    openstudio::runmanager::Workflow wf2("null->null");
    rm.enqueue(wf2.create(outdir), true);
    ASSERT_EQ(rm.getJobs().size(), 5u);
    ASSERT_TRUE(rm.workPending());
  }

  openstudio::System::msleep(1000);

  {
    openstudio::runmanager::RunManager rm(db, false, true);

    openstudio::DateTime now = openstudio::DateTime::now();

    ASSERT_TRUE(rm.workPending());

    std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();
    ASSERT_EQ(jobs.size(), 5u);

    int previouslyrun = 0;
    int notrun = 0;

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
        itr != jobs.end();
        ++itr)
    {
      if (itr->lastRun())
      {
        ASSERT_LT(*(itr->lastRun()), now);
        openstudio::runmanager::JobErrors errors = itr->errors();
        ASSERT_TRUE(errors.allErrors.empty());
        ASSERT_TRUE(errors.succeeded());
        ++previouslyrun;
      } else {
        ++notrun;
      }
    }

    EXPECT_EQ(2, notrun);
    EXPECT_EQ(3, previouslyrun); 

    rm.setPaused(false);
    rm.waitForFinished();

    ASSERT_FALSE(rm.workPending());

    previouslyrun = 0;
    int newlyrun = 0;

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
        itr != jobs.end();
        ++itr)
    {
      ASSERT_TRUE(itr->lastRun());

      if ( *(itr->lastRun()) < now)
      {
        ++previouslyrun;
      } else {
        ++newlyrun;
      }

    }

    ASSERT_EQ(previouslyrun, 3);
    ASSERT_EQ(newlyrun, 2);

    ASSERT_FALSE(rm.workPending());
  }
}


