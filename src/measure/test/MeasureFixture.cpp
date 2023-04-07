/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "MeasureFixture.hpp"

#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"

#include "../../utilities/idd/IddEnums.hpp"

#include <resources.hxx>

/*
// Pretty much the only safe place to include these files is here (or another app)
// and in this order

// DLM: copied from /cli/main.cpp

#include "../../cli/RubyInterpreter.hpp"
#include "../../cli//GC_Value.hpp"
#include <cli/InitMacros.hxx>


#include <cli/embedded_files.hxx>

namespace embedded_files {


  std::vector<std::string> fileNames()
  {
    return std::vector<std::string>();
  }

  std::map<std::string, std::pair<size_t, const uint8_t *> > files()
  {
    return std::map<std::string, std::pair<size_t, const uint8_t *> >();
  }
}

std::vector<std::string> paths;
static RubyInterpreter rubyInterpreter(paths);//(paths);

extern "C" {
  int rb_hasFile(const char *t_filename) {
    // TODO Consider expanding this to use the path which we have artificially defined in embedded_help.rb
    std::string expandedName = std::string(":/ruby/2.2.0/") + std::string(t_filename) + ".rb";
    return embedded_files::hasFile(expandedName);
  }

  int rb_require_embedded(const char *t_filename) {
    std::string require_script = R"(require ')" + std::string(t_filename) + R"(')";
    rubyInterpreter.evalString(require_script);
    return 0;
  }
}

using openstudio::FileLogSink;
using openstudio::toPath;

void MeasureFixture::SetUp()
{
  // required for EmbeddedRubyTest
  // the ruby interpreter must be initialized this way exactly once
  // normally this should be done right at the beginning of main
  // this will do for this test suite but do not add any more fixtures to this test suite
  int argc = 0;
  char **argv;
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
  }

}
*/

void MeasureFixture::SetUp() {}

void MeasureFixture::TearDown() {}

void MeasureFixture::SetUpTestSuite() {
  // set up logging
  logFile = openstudio::FileLogSink(openstudio::toPath("./MeasureFixture.log"));
  logFile->setLogLevel(Info);
  openstudio::Logger::instance().standardOutLogger().disable();

  // clear scratch
  openstudio::path p = resourcesPath() / openstudio::toPath("ruleset/scratch");
  openstudio::filesystem::remove_all(p);
  openstudio::filesystem::create_directories(p);
}

void MeasureFixture::TearDownTestSuite() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> MeasureFixture::logFile;
