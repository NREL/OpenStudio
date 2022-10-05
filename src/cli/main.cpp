#include "RubyCLI.hpp"
#include "UpdateCommand.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../workflow/OSWorkflow.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#include "RunCommand.hpp"

#include <OpenStudio.hxx>

#include <iostream>

#include <CLI/CLI.hpp>

int main(int argc, char* argv[]) {
  int result = 0;

  // ScriptEngineInstance will delay load the engines
  openstudio::ScriptEngineInstance rubyEngine("rubyengine", argc - 1, argv + 1);
  openstudio::ScriptEngineInstance pythonEngine("pythonengine", argc - 1, argv + 1);

  if ((argc > 1) && (std::string_view(argv[1]) == "labs")) {
    CLI::App app{"openstudio"};

    auto* const experimentalApp = app.add_subcommand("labs");

    experimentalApp->add_flag_function(
      "--verbose",
      [](int count) {
        if (count == 1) {
          fmt::print("Setting log Level to Debug\n");
          openstudio::Logger::instance().standardOutLogger().setLogLevel(LogLevel::Debug);
        } else if (count == 2) {
          fmt::print("Setting log Level to Trace\n");
          openstudio::Logger::instance().standardOutLogger().setLogLevel(LogLevel::Trace);
        }
      },
      "Print the full log to STDOUT");

    std::vector<std::string> includeDirs;
    experimentalApp
      ->add_option("-I,--include", includeDirs, "Add additional directory to add to front of Ruby $LOAD_PATH (may be used more than once)")
      ->option_text("DIR");

    std::vector<std::string> executeRubyCmds;
    CLI::Option* execRubyOption = experimentalApp
                                    ->add_option("-e,--execute", executeRubyCmds,
                                                 "Execute one line of Ruby script (may be used more than once). Returns after executing commands.")
                                    ->option_text("CMD");

    std::vector<std::string> executePythonCmds;
    CLI::Option* execPythonOption =
      experimentalApp
        ->add_option("-c,--pyexecute", executePythonCmds,
                     "Execute one line of Python script (may be used more than once). Returns after executing commands.")
        ->option_text("CMD");

    std::vector<std::string> gemPathDirs;
    app
      .add_option("--gem_path", gemPathDirs, "Add additional directory to add to front of GEM_PATH environment variable (may be used more than once)")
      ->option_text("DIR");

    std::string gemHomeDir;
    experimentalApp->add_option("--gem_home", gemHomeDir, "Set GEM_HOME environment variable")->option_text("DIR");

    std::string gemFile;
    experimentalApp->add_option("--bundle", gemFile, "Use bundler for GEMFILE'")->option_text("GEMFILE");

    [[maybe_unused]] auto* energyplus_versionCommand =
      experimentalApp->add_subcommand("energyplus_version", "Returns the EnergyPlus version used by the CLI")->callback([]() {
        fmt::print("{}+{}\n", energyPlusVersion(), energyPlusBuildSHA());
      });

    auto* execute_ruby_scriptCommand = experimentalApp->add_subcommand("execute_ruby_script", "Executes a ruby file");
    openstudio::filesystem::path rubyScriptPath;
    execute_ruby_scriptCommand->add_option("path", rubyScriptPath, "Path to ruby file")->required(true);
    execute_ruby_scriptCommand->callback([&rubyScriptPath, &rubyEngine] { openstudio::cli::executeRubyScriptCommand(rubyScriptPath, rubyEngine); });

    [[maybe_unused]] auto* gem_listCommand = experimentalApp->add_subcommand("gem_list", "Lists the set gems available to openstudio");
    [[maybe_unused]] auto* list_commandsCommand = experimentalApp->add_subcommand("list_commands", "Lists the entire set of available commands");
    [[maybe_unused]] auto* measureCommand = experimentalApp->add_subcommand("measure", "Updates measures and compute arguments");
    [[maybe_unused]] auto* openstudio_versionCommand =
      experimentalApp->add_subcommand("openstudio_version", "Returns the OpenStudio version used by the CLI")->callback([]() {
        fmt::print("{}\n", openStudioLongVersion());
      });
    [[maybe_unused]] auto* ruby_versionCommand =
      experimentalApp->add_subcommand("ruby_version", "Returns the Ruby version used by the CLI")->callback([&rubyEngine]() {
        rubyEngine->exec("puts RUBY_VERSION");
      });
    [[maybe_unused]] auto* python_versionCommand =
      experimentalApp->add_subcommand("python_version", "Returns the Ruby version used by the CLI")->callback([&pythonEngine]() {
        pythonEngine->exec("import sys; print(sys.version)");
      });

    // run command
    openstudio::cli::setupRunOptions(experimentalApp, rubyEngine, pythonEngine);

    // update command
    // openstudio::cli::setupUpdateCommand(experimentalApp);
    bool keep = false;
    [[maybe_unused]] auto* updateCommand = experimentalApp->add_subcommand("update", "Updates OpenStudio Models to the current version");
    updateCommand->add_flag("-k,--keep", keep, "Keep original files");

    openstudio::filesystem::path updateOsmPath;
    updateCommand->add_option("path", updateOsmPath, "Path to OSM or directory containing osms")->required(true);

    updateCommand->callback([&keep, &updateOsmPath] {
      bool result = openstudio::cli::runModelUpdateCommand(updateOsmPath, keep);
      if (!result) {
        throw std::runtime_error("Failed to update some models");
      }
    });

    CLI11_PARSE(app, argc, argv);

    if (*execRubyOption) {
      fmt::print("--execute Flag received {} times.\n", execRubyOption->count());
      rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");
      for (auto& cmd : executeRubyCmds) {
        fmt::print("{}\n", cmd);
        rubyEngine->exec(cmd);
      }
    }
    if (*execPythonOption) {
      fmt::print("--pyexecute Flag received {} times.\n", execPythonOption->count());
      for (auto& cmd : executePythonCmds) {
        fmt::print("{}\n", cmd);
        pythonEngine->exec(cmd);
      }
    }
    // fmt::print("includeDirs={}\n", fmt::join(includeDirs, ","));
    // fmt::print("gemPathDirs={}\n", fmt::join(gemPathDirs, ","));
    // fmt::print("gemHomeDir={}\n", gemHomeDir);
  } else {
    result = openstudio::rubyCLI(rubyEngine);
  }

  // Important to destroy RubyEngine and finalize Ruby at the right time
  // After the main function returns it is (apparently) too late
  rubyEngine.reset();
  pythonEngine.reset();

  return result;
}
