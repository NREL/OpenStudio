/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef CLI_MEASUREUPDATECOMMAND_HPP
#define CLI_MEASUREUPDATECOMMAND_HPP

#include <CLI/App.hpp>
#include "../utilities/core/Filesystem.hpp"

namespace openstudio {

class ScriptEngineInstance;

namespace cli {

  struct MeasureUpdateOptions
  {
   public:
    static void setupMeasureUpdateOptions(CLI::App* parentApp, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine);

    static void execute(MeasureUpdateOptions const& opt, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine);

    void debug_print() const;

    bool update = false;
    bool update_all = false;

    openstudio::path compute_arguments_model;
    openstudio::path directoryPath;

    bool run_tests = false;

    unsigned server_port = 0;
  };

}  // namespace cli
}  // namespace openstudio

#endif  // CLI_MEASUREUPDATECOMMAND_HPP
