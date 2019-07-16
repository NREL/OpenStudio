/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
    std::string url = "https://www.openstudio.net/updateGTest.html?app=GTest&version=0.0.0";
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
    EXPECT_EQ("https://www.openstudio.net/downloads/99", manager.mostRecentDownloadUrl());
    ASSERT_EQ(static_cast<unsigned>(2), manager.updateMessages().size());
  }
}
