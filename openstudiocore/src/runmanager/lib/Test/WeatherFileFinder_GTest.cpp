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
#include "../WeatherFileFinder.hpp"
#include <QDir>
#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/System.hpp"

#ifdef _MSC_VER
#include <Windows.h>
#endif

TEST_F(RunManagerTestFixture, WeatherFileFinderEmpty)
{
  //Expect empty, nothing to find
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(openstudio::runmanager::JobParams(), boost::optional<std::string>(), boost::optional<std::string>());
  EXPECT_TRUE(p.empty());
}

TEST_F(RunManagerTestFixture, WeatherFileFinderEPWDirWithFilename)
{
  // expect epwdir/filename to be returned
  openstudio::runmanager::JobParams params;
  params.append("epwdir", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager")));
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(params, boost::optional<std::string>(), boost::optional<std::string>("USA_FL_Tampa.Intl.AP.722110_TMY3.epw"));

  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_FL_Tampa.Intl.AP.722110_TMY3.epw"), p);
}

TEST_F(RunManagerTestFixture, WeatherFileFinderEPWDirWithFileLocationName)
{
  // expect description to find the right file
  openstudio::runmanager::JobParams params;
  params.append("epwdir", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager")));
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(params, boost::optional<std::string>("USA FL Tampa"), boost::optional<std::string>());

  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_FL_Tampa.Intl.AP.722110_TMY3.epw"), p);
}

TEST_F(RunManagerTestFixture, WeatherFileFinderEPWDirWithFileLocationNameAndGoodFilename)
{
  // If description and filename are both set, expect filename to win
  openstudio::runmanager::JobParams params;
  params.append("epwdir", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager")));
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(params, boost::optional<std::string>("USA FL Tampa"), boost::optional<std::string>("USA_CA_San.Francisco.Intl.AP.724940_TMY3.epw"));

  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_CA_San.Francisco.Intl.AP.724940_TMY3.epw"), p);
}

TEST_F(RunManagerTestFixture, WeatherFileFinderEPWDirWithFileLocationNameAndBadFilename)
{
  // If description and filename are both set but filename doesn't exist, expect description to win
  openstudio::runmanager::JobParams params;
  params.append("epwdir", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager")));
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(params, boost::optional<std::string>("USA FL Tampa"), boost::optional<std::string>("NotAReafFileName"));

  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_FL_Tampa.Intl.AP.722110_TMY3.epw"), p);
}

TEST_F(RunManagerTestFixture, WeatherFileFinderAllThreeGood)
{
  // If all are good, then the epwfile param should win
  openstudio::runmanager::JobParams params;
  params.append("epwfile", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager/USA_CO_Golden-NREL.724666_TMY3.epw")));
  params.append("epwdir", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager")));
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(params, boost::optional<std::string>("USA FL Tampa"), boost::optional<std::string>("USA_CA_San.Francisco.Intl.AP.724940_TMY3.epw"));

  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_CO_Golden-NREL.724666_TMY3.epw"), p);
}

TEST_F(RunManagerTestFixture, WeatherFileFinderBadEPWFile)
{
  // If the epwfile is bad, then the filename should win
  openstudio::runmanager::JobParams params;
  params.append("epwfile", "nosuchfile");
  params.append("epwdir", openstudio::toString(resourcesPath() / openstudio::toPath("runmanager")));
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(params, boost::optional<std::string>("USA FL Tampa"), boost::optional<std::string>("USA_CA_San.Francisco.Intl.AP.724940_TMY3.epw"));

  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_CA_San.Francisco.Intl.AP.724940_TMY3.epw"), p);
}



TEST_F(RunManagerTestFixture, WeatherFileFinderNoEPWDir)
{
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(openstudio::runmanager::JobParams(), boost::optional<std::string>(), boost::optional<std::string>("USA_FL_Tampa.Intl.AP.722110_TMY3.epw"));
  EXPECT_TRUE(p.empty());
}

TEST_F(RunManagerTestFixture, WeatherFileFinderNoEPWDirCompletedPath)
{
  openstudio::path p = openstudio::runmanager::WeatherFileFinder::find(openstudio::runmanager::JobParams(), boost::optional<std::string>(), boost::optional<std::string>(openstudio::toString(resourcesPath() / openstudio::toPath("runmanager/USA_FL_Tampa.Intl.AP.722110_TMY3.epw"))));
  EXPECT_EQ(resourcesPath() / openstudio::toPath("runmanager/USA_FL_Tampa.Intl.AP.722110_TMY3.epw"), p);
}
