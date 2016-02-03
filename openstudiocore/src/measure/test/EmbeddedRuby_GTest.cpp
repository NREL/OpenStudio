/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "RulesetFixture.hpp"

#include "../OSArgument.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <vector>
#include <map>

#include "../../utilities/core/ApplicationPathHelpers.hpp"

// Pretty much the only safe place to include these files is here (or another app)
// and in this order
#include "../../utilities/core/RubyInterpreter.hpp"

// This one is a bit more globally accessible, but no one can instantiate the template without the two above
#include "../EmbeddedRubyUserScriptArgumentGetter.hpp"
#include "../EmbeddedRubyUserScriptInfoGetter.hpp"

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;


#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

TEST_F(RulesetFixture, UserScript_EmbeddedRubyTest) {
  const char *argv[] = {"executable"};
  char **argvpnc = const_cast<char **>(argv);

  int argc = 1;
  ruby_sysinit(&argc, &argvpnc);

  {
    RUBY_INIT_STACK;
    ruby_init();
  }


  // Build the list of modules we want to load
  std::vector<std::string> modules;
  for (const auto& path : openstudio::getOpenStudioBareRubyPaths()){
    modules.push_back(openstudio::toString(path));
  }

  // Initialize the embedded Ruby interpreter
  std::shared_ptr<openstudio::detail::RubyInterpreter>
    ri(new openstudio::detail::RubyInterpreter(getOpenStudioRubyPath(),
                                               getOpenStudioRubyScriptsPath(),
                                               modules));

  // Use the embedded Ruby interpreter on its own

  // Test calling a function with 0 parameters
  std::stringstream ss;
  ss << "def doSomething()" << std::endl;
  ss << "  v = OpenStudio::IntVector.new()" << std::endl;
  ss << "  v << 5" << std::endl;
  ss << "  v << 2" << std::endl;
  ss << "  v << 1*2 + 5" << std::endl;
  ss << "  return v" << std::endl;
  ss << "end" << std::endl;
  ri->evalString(ss.str()); ss.str("");
  std::vector<int> v = ri->execWithReturn<std::vector<int> >("doSomething");
  ASSERT_TRUE(!v.empty());
  ASSERT_EQ(v.size(), 3u);
  EXPECT_EQ(v[0], 5);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 1*2+5);

  // Test calling a function with 2 parameters
  ss << "def doSomething2(i, j)" << std::endl;
  ss << "  v = OpenStudio::IntVector.new()" << std::endl;
  ss << "  v << i" << std::endl;
  ss << "  v << j" << std::endl;
  ss << "  v << i*2 + j" << std::endl;
  ss << "  return v" << std::endl;
  ss << "end" << std::endl;
  ri->evalString(ss.str()); ss.str("");
  v = ri->execWithReturn<std::vector<int> >("doSomething2", 3, 8);
  ASSERT_TRUE(!v.empty());
  ASSERT_EQ(v.size(), 3u);
  EXPECT_EQ(v[0], 3);
  EXPECT_EQ(v[1], 8);
  EXPECT_EQ(v[2], 3 * 2 + 8);

  // Deprecated RubyUserScriptArgumentGetter
  {
    // Initialize the argument getter
    std::shared_ptr<openstudio::ruleset::RubyUserScriptArgumentGetter>
      rsc(new openstudio::ruleset::EmbeddedRubyUserScriptArgumentGetter<openstudio::detail::RubyInterpreter>(ri));

    // Test calling the actual function we are concerned about
    std::vector<OSArgument> args;

    Model someModel;
    Workspace someWorkspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

    openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/");
    boost::optional<BCLMeasure> oMeasure = BCLMeasure::load(dir);
    ASSERT_TRUE(oMeasure);
    BCLMeasure measure = oMeasure.get();

    // Works no matter what overloaded form we use. If not provided, empty models are passed to
    // the arguments method.
    args = rsc->getArguments(measure);
    EXPECT_EQ(3u, args.size());
    args = rsc->getArguments(measure, someModel);
    EXPECT_EQ(3u, args.size());
    args = rsc->getArguments(measure, someWorkspace);
    EXPECT_EQ(3u, args.size());
    args = rsc->getArguments(measure, someModel, someWorkspace);
    EXPECT_EQ(3u, args.size());
  }
 
  // New RubyUserScriptInfoGetter
  {
    // Initialize the info getter
    std::shared_ptr<openstudio::ruleset::RubyUserScriptInfoGetter>
      rsc(new openstudio::ruleset::EmbeddedRubyUserScriptInfoGetter<openstudio::detail::RubyInterpreter>(ri));

    // Test calling the actual function we are concerned about
    
    Model someModel;
    Workspace someWorkspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

    openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/");
    boost::optional<BCLMeasure> oMeasure = BCLMeasure::load(dir);
    ASSERT_TRUE(oMeasure);
    BCLMeasure measure = oMeasure.get();

    // Works no matter what overloaded form we use. If not provided, empty models are passed to
    // the arguments method.
    RubyUserScriptInfo info = rsc->getInfo(measure);
    EXPECT_EQ(3u, info.arguments().size());
    info = rsc->getInfo(measure, someModel);
    EXPECT_EQ(3u, info.arguments().size());
    info = rsc->getInfo(measure, someWorkspace);
    EXPECT_EQ(3u, info.arguments().size());
    info = rsc->getInfo(measure, someModel, someWorkspace);
    EXPECT_EQ(3u, info.arguments().size());
  }
  
}


#if defined(_MSC_VER)
#pragma warning( pop )
#endif
