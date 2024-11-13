/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef CLI_UPDATECOMMAND_HPP
#define CLI_UPDATECOMMAND_HPP

#include <CLI/App.hpp>
#include "../utilities/core/Filesystem.hpp"

namespace openstudio {

class ScriptEngineInstance;

namespace cli {

  bool runModelUpdateCommand(const openstudio::path& p, bool keep);

  void executeRubyScriptCommand(openstudio::path rubyScriptPath, ScriptEngineInstance& rubyEngine, const std::vector<std::string>& arguments);
  void executePythonScriptCommand(openstudio::path pythonScriptPath, ScriptEngineInstance& pythonEngine, const std::vector<std::string>& arguments);

  void executeGemListCommand(ScriptEngineInstance& rubyEngine);
  void executePipListCommand(ScriptEngineInstance& pythonEngine);

  void executeRubyRepl(ScriptEngineInstance& rubyEngine);
  void executePythonRepl(ScriptEngineInstance& pythonEngine);

}  // namespace cli
}  // namespace openstudio

#endif  // CLI_UPDATECOMMAND_HPP
