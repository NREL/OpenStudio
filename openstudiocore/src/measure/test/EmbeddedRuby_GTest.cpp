/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "MeasureFixture.hpp"

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
#include "../EmbeddedRubyMeasureInfoGetter.hpp"

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::measure;


#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

TEST_F(MeasureFixture, EmbeddedRubyTest) {
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

  // New OSMeasureInfoGetter
  {
    // Initialize the info getter
    std::shared_ptr<openstudio::measure::OSMeasureInfoGetter>
      rsc(new openstudio::measure::EmbeddedRubyMeasureInfoGetter<openstudio::detail::RubyInterpreter>(ri));

    // Test calling the actual function we are concerned about
    
    Model someModel;
    Workspace someWorkspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

    openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/");
    boost::optional<BCLMeasure> oMeasure = BCLMeasure::load(dir);
    ASSERT_TRUE(oMeasure);
    BCLMeasure measure = oMeasure.get();

    // Works no matter what overloaded form we use. If not provided, empty models are passed to
    // the arguments method.
    OSMeasureInfo info = rsc->getInfo(measure);
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
