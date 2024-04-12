/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  scratchDir = openstudio::getApplicationBuildDirectory() / openstudio::toPath("Testing/Temporary/measure");
  openstudio::filesystem::remove_all(scratchDir);
  openstudio::filesystem::create_directories(scratchDir);
}

void MeasureFixture::TearDownTestSuite() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> MeasureFixture::logFile;
openstudio::path MeasureFixture::scratchDir;
