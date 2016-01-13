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

#include "RulesetFixture.hpp"


#include "../../utilities/core/Path.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include "../../utilities/idd/IddEnums.hpp"

#include <boost/filesystem.hpp>

#include <resources.hxx>

// Pretty much the only safe place to include these files is here (or another app)
// and in this order

#include "../../utilities/core/RubyInterpreter.hpp"
#include "../ruleset/EmbeddedRubyUserScriptArgumentGetter.hpp"

using openstudio::FileLogSink;
using openstudio::toPath;

void RulesetFixture::SetUp() 
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

void RulesetFixture::TearDown() {}

void RulesetFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./RulesetFixture.log"));
  logFile->setLogLevel(Info);
  openstudio::Logger::instance().standardOutLogger().disable();

  // clear scratch
  openstudio::path p = resourcesPath() / openstudio::toPath("ruleset/scratch");
  boost::filesystem::remove_all(p);
  boost::filesystem::create_directories(p);
}

void RulesetFixture::TearDownTestCase() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> RulesetFixture::logFile;

