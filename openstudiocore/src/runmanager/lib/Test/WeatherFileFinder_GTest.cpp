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
