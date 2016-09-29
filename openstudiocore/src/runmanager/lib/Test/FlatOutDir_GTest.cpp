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
  openstudio::Application::instance().application(false);
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
  openstudio::Application::instance().application(false);
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
