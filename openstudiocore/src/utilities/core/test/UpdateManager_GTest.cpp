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

#include "../UpdateManager.hpp"
#include "../Application.hpp"
#include "../System.hpp"
#include <OpenStudio.hxx>

#include <QApplication>
#include <sstream>
#include <time.h>

namespace openstudio
{

  TEST(UpdateManager, GeneralTest)
  {
    UpdateManager manager("GTest");
    EXPECT_EQ("GTest", manager.appName());
    while (!manager.finished()){
      System::msleep(100);
    }
    EXPECT_TRUE(manager.finished());
    EXPECT_FALSE(manager.error());
  }

  TEST(UpdateManager, ExpandedTest)
  {
    std::string url = "http://openstudio.nrel.gov/updateGTest.html?app=GTest&version=0.0.0";
    UpdateManager manager("GTest", url);
    EXPECT_EQ("GTest", manager.appName());
    while (!manager.finished()){
      System::msleep(100);
    }
    EXPECT_TRUE(manager.finished());
    EXPECT_FALSE(manager.error());
    EXPECT_TRUE(manager.newMajorRelease());
    EXPECT_FALSE(manager.newMinorRelease());
    EXPECT_FALSE(manager.newPatchRelease());
    EXPECT_EQ("99.99.99.99", manager.mostRecentVersion());
    EXPECT_EQ("http://openstudio.nrel.gov/downloads/99", manager.mostRecentDownloadUrl());
    ASSERT_EQ(static_cast<unsigned>(2), manager.updateMessages().size());
  }
}
