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

#include <sstream>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::runmanager;

void populateJobs(RunManager &rm, bool bulkadd, const openstudio::path &t_model, int number)
{
  std::vector<openstudio::runmanager::Job> jobs;

  openstudio::runmanager::ConfigOptions co;
  co.fastFindEnergyPlus();

  for (int i = 0; i < number; ++i)
  {
    boost::timer t;
    Workflow wf("Null->Null->ModelToIdf->ExpandObjects->EnergyPlusPreProcess->EnergyPlus->EnergyPlusPostProcess");

    
    // keep filepaths short enough for Windows
    JobParams params;
    std::stringstream ss;
    ss << i;
    params.append("jobnumber", ss.str());
    params.append(JobParam("flatoutdir"));

    wf.add(co.getTools());
    wf.add(params);

    openstudio::runmanager::Job j = wf.create(openstudio::toPath("ClearJobsPerformanceTest"), t_model);
    LOG_FREE(Info, "RunManagerTiming", "Time to create job: " << t.elapsed());

    if (!bulkadd)
    {
      rm.enqueue(j, true);
    } else {
      jobs.push_back(j);
    }

    LOG_FREE(Info, "RunManagerTiming", "Total Time to crate Job: " << t.elapsed());
  }

  if (bulkadd)
  {
    rm.enqueue(jobs, true);
  }
}

TEST_F(RunManagerTestFixture, JobCreatePerformanceTest)
{
  int number = 50;

  double oldway = 0;

  openstudio::path model = openstudio::toPath("ExampleModel.osm");

  openstudio::model::exampleModel().save(model);

  {
    RunManager rm;
    rm.setPaused(true);
    boost::timer t;

    populateJobs(rm, false, model, number);
    ASSERT_EQ(number*7, static_cast<int>(rm.getJobs().size()));

    oldway = t.elapsed();
  }


  double newway = 0;

  {
    RunManager rm;
    rm.setPaused(true);

    boost::timer t;
    populateJobs(rm, true, model, number);
    ASSERT_EQ(number *7, static_cast<int>(rm.getJobs().size()));

    newway = t.elapsed();
  }

  // Assert that new way is at least 2x faster than old way
  ASSERT_LT(newway, oldway);

  LOG(Info, "Oldway: " << oldway << " Newway: " << newway);
}
