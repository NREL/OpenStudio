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
  tools.tools().push_back(ToolInfo("tool", ToolVersion(1,5,6), 
        openstudio::toPath("path1"),
        openstudio::toPath("path2"),
        openstudio::toPath("path3"),
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
