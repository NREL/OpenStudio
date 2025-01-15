/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../../scriptengine/ScriptEngine.hpp"
#include "../../utilities/core/Logger.hpp"

#include <gtest/gtest.h>

// cppcheck-suppress syntaxError
TEST(OpenStudioTest, LoggerGlobal) {
  auto isEnabled = openstudio::Logger::instance().standardOutLogger().isEnabled();
  EXPECT_TRUE(isEnabled);

  openstudio::ScriptEngineInstance pythonEngine("pythonengine", {});
  openstudio::ScriptEngineInstance rubyEngine("rubyengine", {});

  pythonEngine->exec("openstudio.Logger.instance().standardOutLogger().disable()");
  isEnabled = openstudio::Logger::instance().standardOutLogger().isEnabled();
  EXPECT_FALSE(isEnabled);

  rubyEngine->exec("OpenStudio::Logger::instance().standardOutLogger().enable()");
  isEnabled = openstudio::Logger::instance().standardOutLogger().isEnabled();
  EXPECT_TRUE(isEnabled);

  pythonEngine.reset();
  rubyEngine.reset();
}
