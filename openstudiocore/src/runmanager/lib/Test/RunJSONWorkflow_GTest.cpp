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
#include "../Workflow.hpp"
#include "../RubyJobUtils.hpp"
#include "../JSONWorkflowOptions.hpp"

#include "../../../ruleset/OSArgument.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"
#include "../../../utilities/filetypes/EpwFile.hpp"

#include <QDir>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>


#include <resources.hxx>
#include <OpenStudio.hxx>

#ifdef _MSC_VER
#include <Windows.h>
#endif

TEST_F(RunManagerTestFixture, RunWorkflowJson)
{
  openstudio::runmanager::RunManager rm;
  rm.setPaused(true);

  openstudio::path jsonfolder = resourcesPath() / openstudio::toPath("runmanager") / openstudio::toPath("jsonworkflowtest");
  openstudio::path json = jsonfolder / openstudio::toPath("small_seed.json");
  openstudio::path jsonbasepath = jsonfolder / openstudio::toPath("inputs");

  openstudio::path osmpath = jsonfolder / openstudio::toPath("inputs") / openstudio::toPath("seed") / openstudio::toPath("small_seed.osm");
  openstudio::model::Model exampleModel = openstudio::model::exampleModel();
  openstudio::runmanager::RunManager::simplifyModelForPerformance(exampleModel);
  exampleModel.save(osmpath, true);


  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("RunWorkflowJSONTest");

  openstudio::runmanager::Tools tools
    = openstudio::runmanager::ConfigOptions::makeTools(
        energyPlusExePath().parent_path(),
        openstudio::path(),
        openstudio::path(),
        rubyExePath().parent_path(),
        openstudio::path());

  openstudio::runmanager::Job j = rm.runWorkflow(json, jsonbasepath, outdir, tools, openstudio::runmanager::JSONWorkflowOptions(true, true, false));


  rm.setPaused(false);
  rm.waitForFinished();

  ASSERT_TRUE(j.errors().succeeded());
}


