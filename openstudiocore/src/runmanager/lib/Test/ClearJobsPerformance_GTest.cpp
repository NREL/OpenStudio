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
#include "../Workflow.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/filetypes/EpwFile.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OS_TimeDependentValuation_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <boost/filesystem/path.hpp>
#include <boost/timer.hpp>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::runmanager;

void populateJobs(RunManager &rm)
{
  for (int i = 1; i < 31; ++i)
  {
    Workflow wf;
    for (int j = 0; j < i; ++j)
    {
      wf.addJob(JobType::Null);
    }
    // keep filepaths short enough for Windows
    wf.addParam(runmanager::JobParam("flatoutdir"));
    rm.enqueue(wf.create(openstudio::toPath("ClearJobsPerformanceTest")), true);
  }
}

TEST_F(RunManagerTestFixture, ClearJobsPerformanceTest)
{
  double oldway = 0;

  {
    RunManager rm;
    populateJobs(rm);

    ASSERT_EQ(465, static_cast<int>(rm.getJobs().size()));

    boost::timer t;

    for (openstudio::runmanager::Job job : rm.getJobs()) {
      rm.remove(job);
    }

    oldway = t.elapsed();

    ASSERT_TRUE(rm.getJobs().empty());
    ASSERT_FALSE(rm.workPending());
  }


  double newway = 0;

  {
    RunManager rm;
    populateJobs(rm);

    ASSERT_EQ(465, static_cast<int>(rm.getJobs().size()));

    boost::timer t;

    rm.clearJobs();

    newway = t.elapsed();

    ASSERT_TRUE(rm.getJobs().empty());
    ASSERT_FALSE(rm.workPending());
  }

  // Assert that new way is at least 2x faster than old way
  ASSERT_LT(newway * 2, oldway);

  LOG(Info, "Oldway: " << oldway << " Newway: " << newway);
}
