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
#include <runmanager/Test/ToolBin.hxx>
#include <resources.hxx>
#include "../JobFactory.hpp"
#include <boost/filesystem/path.hpp>
#include "../RunManager.hpp"
#include "../Workflow.hpp"
#include <QDir>
#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"

#ifdef _MSC_VER
#include <Windows.h>

void operator<<(std::ostream& os, const openstudio::path& p){
  os << openstudio::toString(p);
}

#endif



TEST_F(RunManagerTestFixture, FlatOutDir)
{
  openstudio::runmanager::Workflow wf("Null->Null->Null->Null->Null");

  // Set parameter specifying that jobs should have a flattened directory structure.
  wf.addParam(openstudio::runmanager::JobParam("flatoutdir"));
  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("FlatOutDir");
  openstudio::runmanager::RunManager rm(db, true, true);
  openstudio::runmanager::Job job = wf.create(openstudio::toPath("someoutdir"));
  rm.enqueue(job, false);

  std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();

  ASSERT_EQ(static_cast<size_t>(5), jobs.size());

  EXPECT_EQ(jobs[0].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("0-Null"));
  EXPECT_EQ(jobs[1].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("1-Null-0"));
  EXPECT_EQ(jobs[2].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("2-Null-0"));
  EXPECT_EQ(jobs[3].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("3-Null-0"));
  EXPECT_EQ(jobs[4].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("4-Null-0"));
}

TEST_F(RunManagerTestFixture, NotFlatOutDir)
{
  openstudio::runmanager::Workflow wf("Null->Null->Null->Null->Null");

  openstudio::path db = openstudio::toPath(QDir::tempPath()) / openstudio::toPath("NotFlatOutDir");
  openstudio::runmanager::RunManager rm(db, true, true);
  openstudio::runmanager::Job job = wf.create(openstudio::toPath("someoutdir"));
  rm.enqueue(job, false);

  std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();

  ASSERT_EQ(static_cast<size_t>(5), jobs.size());

  EXPECT_EQ(jobs[0].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("Null"));
  EXPECT_EQ(jobs[1].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("Null") / openstudio::toPath("Null-0"));
  EXPECT_EQ(jobs[2].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("Null") / openstudio::toPath("Null-0") / openstudio::toPath("Null-0"));
  EXPECT_EQ(jobs[3].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("Null") / openstudio::toPath("Null-0") / openstudio::toPath("Null-0") / openstudio::toPath("Null-0"));
  EXPECT_EQ(jobs[4].outdir(), openstudio::toPath("someoutdir") / openstudio::toPath("Null") / openstudio::toPath("Null-0") / openstudio::toPath("Null-0") / openstudio::toPath("Null-0") / openstudio::toPath("Null-0"));
}
