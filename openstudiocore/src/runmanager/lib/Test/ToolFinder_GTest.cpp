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
#include "../LocalProcessCreator.hpp"
#include "../ToolFinder.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

TEST_F(RunManagerTestFixture, ToolFinder)
{
  std::vector<openstudio::path> search;

 
#ifdef Q_OS_WIN
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


