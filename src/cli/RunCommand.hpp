/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef CLI_RUNCOMMAND_HPP
#define CLI_RUNCOMMAND_HPP

#include <CLI/App.hpp>

#include <functional>

namespace openstudio {

class ScriptEngineInstance;

namespace cli {

  void setupRunOptions(CLI::App* parentApp, ScriptEngineInstance& ruby, ScriptEngineInstance& python);
  // void setupRunFtOptions(CLI::App* app, FtOptions& ftOptions);

}  // namespace cli
}  // namespace openstudio

#endif  // CLI_RUNCOMMAND_HPP
