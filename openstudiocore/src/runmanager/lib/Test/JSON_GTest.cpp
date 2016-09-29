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
#include "../WorkItem.hpp"
#include <QDir>
#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"

#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace openstudio;
using namespace openstudio::runmanager;

TEST_F(RunManagerTestFixture, JSON_jobTest)
{
  openstudio::runmanager::Workflow wf("ExpandObjects->EnergyPlus");
  openstudio::runmanager::Job j = wf.create(openstudio::toPath("outdir"), openstudio::toPath("my.idf"));

  std::string json = j.toJSON();

  EXPECT_FALSE(json.empty());


  openstudio::runmanager::Job j2 = Job::fromJSON(json);

  EXPECT_EQ(j2.jobType(), JobType::ExpandObjects);
  EXPECT_EQ(j2.inputFiles().size(), 1u);


  std::string json2 = j2.toJSON();

  EXPECT_EQ(json, json2);
}


TEST_F(RunManagerTestFixture, JSON_workItem)
{
  Tools tools;
  tools.tools().push_back(ToolInfo("tool", ToolVersion(1,5,6, "TAG"), 
        openstudio::toPath("path1"),
        boost::regex(".*")
        ));

  Files files;
  files.files().push_back(FileInfo("filename",
        openstudio::DateTime::now(),
        "key",
        openstudio::toPath("fullpath"),
        true));

  files.files()[0].addRequiredFile(QUrl("url"), openstudio::toPath("target"));

  JobParams params;
  params.append("key", "value");


  WorkItem wi(JobType::EnergyPlus,
      tools,
      params,
      files,
      "keyname");

  std::string json = wi.toJSON();

  EXPECT_FALSE(json.empty());

  openstudio::runmanager::WorkItem wi2 = WorkItem::fromJSON(json);

  EXPECT_EQ(wi2.type, JobType::EnergyPlus);
  EXPECT_EQ(wi2.tools, tools);
  EXPECT_EQ(wi2.params, params);
  EXPECT_EQ(wi2.files, files);
  EXPECT_EQ(wi2.jobkeyname, "keyname");


  std::string json2 = wi2.toJSON();

  EXPECT_EQ(json, json2);

}
