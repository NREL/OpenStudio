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
#include <runmanager/Test/ToolBin.hxx>
#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../LocalProcessCreator.hpp"
#include "../Workflow.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/filetypes/EpwFile.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

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
  openstudio::Application::instance().application(false);
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
