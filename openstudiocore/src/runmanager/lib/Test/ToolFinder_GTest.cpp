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
#include <resources.hxx>
#include "../JobFactory.hpp"
#include <boost/filesystem/path.hpp>
#include "../RunManager.hpp"
#include "../LocalProcessCreator.hpp"
#include "../ToolFinder.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

TEST_F(RunManagerTestFixture, ToolFinder)
{
  std::vector<openstudio::path> search;

 
#ifdef Q_OS_WIN32
  search.push_back(openstudio::toPath("C:\\"));
#else
  search.push_back(openstudio::toPath("/"));
  search.push_back(openstudio::toPath("/usr/local"));
#endif

  std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
    foundTools = openstudio::runmanager::ToolFinder::findTools(search);

  EXPECT_TRUE(foundTools.size() >= 2);
}


TEST_F(RunManagerTestFixture, ToolFinderWithConfigOptions)
{
  openstudio::runmanager::ConfigOptions co;
  co.findTools(false,false,false,false);

  openstudio::path epwdir = co.getDefaultEPWLocation();
  openstudio::path idfdir = co.getDefaultIDFLocation();

  EXPECT_FALSE(epwdir.empty());
  EXPECT_FALSE(idfdir.empty());

  EXPECT_TRUE(boost::filesystem::exists(epwdir));
  EXPECT_TRUE(boost::filesystem::exists(idfdir));

  EXPECT_TRUE(boost::filesystem::is_directory(epwdir));
  EXPECT_TRUE(boost::filesystem::is_directory(idfdir));

  EXPECT_GT(*co.getTools().getTool("energyplus").version.getMajor(), 4);
}


TEST_F(RunManagerTestFixture, ToolFinderWithConfigOptionsFastFind)
{
  openstudio::runmanager::ConfigOptions co;
  co.fastFindEnergyPlus();

  openstudio::path epwdir = co.getDefaultEPWLocation();
  openstudio::path idfdir = co.getDefaultIDFLocation();

  EXPECT_FALSE(epwdir.empty());
  EXPECT_FALSE(idfdir.empty());

  EXPECT_TRUE(boost::filesystem::exists(epwdir));
  EXPECT_TRUE(boost::filesystem::exists(idfdir));

  EXPECT_TRUE(boost::filesystem::is_directory(epwdir));
  EXPECT_TRUE(boost::filesystem::is_directory(idfdir));

  EXPECT_GT(*co.getTools().getTool("energyplus").version.getMajor(), 4);
}


