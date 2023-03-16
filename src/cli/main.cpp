#include "RubyCLI.hpp"
#include "UpdateCommand.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../workflow/OSWorkflow.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../measure/ModelMeasure.hpp"
#include "../measure/EnergyPlusMeasure.hpp"
#include "../measure/ReportingMeasure.hpp"

#include "RunCommand.hpp"
#include "MeasureUpdateCommand.hpp"

#include <OpenStudio.hxx>

#include <fmt/format.h>
#include <fmt/color.h>
#include <string_view>

#include <CLI/CLI.hpp>

int main(int argc, char* argv[]) {

  constexpr auto rubySpecificOptionsGroupName = "Ruby Options";
  constexpr auto pythonSpecificOptionsGroupName = "Python Options";

  // constexpr auto executeGroupName = "Script Execution";
  constexpr auto versionGroupname = "Version Info";

  int result = 0;

  std::vector<std::string> args(argv, argv + argc);
  // erase the first element
  args.erase(args.begin());

  // ScriptEngineInstance will delay load the engines
  openstudio::ScriptEngineInstance rubyEngine("rubyengine", args);
  openstudio::ScriptEngineInstance pythonEngine("pythonengine", args);

  if (!args.empty() && (std::string_view(args[0]) == "labs")) {
    CLI::App app{"openstudio"};

    fmt::print(fmt::fg(fmt::color::red),
               "┌{0:─^{2}}┐\n"
               "│{1: ^{2}}│\n"
               "└{0:─^{2}}┘\n",
               "", "The `labs` command is experimental - Do not use in production", 80);
    //
    app.get_formatter()->column_width(35);

    auto* const experimentalApp = app.add_subcommand("labs");

    experimentalApp->add_flag_function(
      "--verbose",
      [](auto count) {
        if (count == 1) {
          fmt::print("Setting log Level to Debug\n");
          openstudio::Logger::instance().standardOutLogger().setLogLevel(LogLevel::Debug);
        } else if (count == 2) {
          fmt::print("Setting log Level to Trace\n");
          openstudio::Logger::instance().standardOutLogger().setLogLevel(LogLevel::Trace);
        }
      },
      "Print the full log to STDOUT");

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

    // ========================== R U B Y    O P T I O N S ==========================
    std::vector<openstudio::path> includeDirs;
    experimentalApp
      ->add_option("-I,--include", includeDirs, "Add additional directory to add to front of Ruby $LOAD_PATH (may be used more than once)")
      ->option_text("DIR")
      ->group(rubySpecificOptionsGroupName);

    std::vector<openstudio::path> gemPathDirs;
    experimentalApp
      ->add_option("--gem_path", gemPathDirs,
                   "Add additional directory to add to front of GEM_PATH environment variable (may be used more than once)")
      ->option_text("DIR")
      ->group(rubySpecificOptionsGroupName);

    openstudio::path gemHomeDir;
    experimentalApp->add_option("--gem_home", gemHomeDir, "Set GEM_HOME environment variable")
      ->option_text("DIR")
      ->group(rubySpecificOptionsGroupName);

    openstudio::path bundleGemFilePath;
    experimentalApp->add_option("--bundle", bundleGemFilePath, "Use bundler for GEMFILE")
      ->option_text("GEMFILE")
      ->group(rubySpecificOptionsGroupName);

    openstudio::path bundleGemDirPath;
    experimentalApp->add_option("--bundle_path", bundleGemDirPath, "Use bundler installed gems in BUNDLE_PATH")
      ->option_text("BUNDLE_PATH")
      ->group(rubySpecificOptionsGroupName);

    // std::vector<std::string>
    std::string bundleWithoutGroups;
    experimentalApp
      ->add_option(
        "--bundle_without", bundleWithoutGroups,
        "Space separated list of groups for bundler to exclude in WITHOUT_GROUPS.  Surround multiple groups with quotes like \"test development\"")
      ->option_text("WITHOUT_GROUPS")
      ->group(rubySpecificOptionsGroupName);  // ->delimiter(' ');

    // This is a callback that's stored on the ScriptEngineInstance, triggered only the first time. Just need to make sure it doesn't go out of scope
    // nor its captures
    std::function<void()> runSetupEmbeddedGems = [&rubyEngine, &includeDirs, &gemPathDirs, &gemHomeDir, &bundleGemFilePath, &bundleGemDirPath,
                                                  &bundleWithoutGroups]() {
      rubyEngine->setupEmbeddedGems(includeDirs, gemPathDirs, gemHomeDir, bundleGemFilePath, bundleGemDirPath, bundleWithoutGroups);
      rubyEngine->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
      rubyEngine->registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
      rubyEngine->registerType<openstudio::measure::ReportingMeasure*>("openstudio::measure::ReportingMeasure *");
      // rubyEngine->registerType<std::string>("std::string");
      // rubyEngine->registerType<std::string*>("std::string *");
      rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");
    };
    rubyEngine.registerInitializationFunction(runSetupEmbeddedGems);

    // ========================== P Y T H O N    O P T I O N S ==========================

    std::vector<openstudio::path> pythonPathDirs;
    experimentalApp
      ->add_option("--python_path", pythonPathDirs,
                   "Add additional directory to add to front of PYTHONPATH environment variable (may be used more than once)")
      ->option_text("DIR")
      ->group(pythonSpecificOptionsGroupName);

    openstudio::path pythonHomeDir;
    experimentalApp->add_option("--python_home", pythonHomeDir, "Set PYTHONHOME environment variable")
      ->option_text("DIR")
      ->group(pythonSpecificOptionsGroupName);

    // This is a callback that's stored on the ScriptEngineInstance, triggered only the first time
    std::function<void()> runSetupPythonPath = [&pythonEngine, &pythonPathDirs, &pythonHomeDir]() {
      pythonEngine->setupPythonPath(pythonPathDirs, pythonHomeDir);
      pythonEngine->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
      pythonEngine->registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
      pythonEngine->registerType<openstudio::measure::ReportingMeasure*>("openstudio::measure::ReportingMeasure *");
      // pythonEngine->registerType<std::string*>("std::string *");
    };
    pythonEngine.registerInitializationFunction(runSetupPythonPath);

    // {
    auto* execute_ruby_scriptCommand = experimentalApp->add_subcommand("execute_ruby_script", "Executes a ruby file");
    openstudio::filesystem::path rubyScriptPath;
    execute_ruby_scriptCommand->add_option("path", rubyScriptPath, "Path to ruby file")->required(true);
    std::vector<std::string> executeRubyScriptCommandArgs;
    execute_ruby_scriptCommand->add_option("arguments", executeRubyScriptCommandArgs, "Arguments to pass to the ruby file")
      ->required(false)
      ->option_text("args");
    execute_ruby_scriptCommand->callback([&rubyScriptPath, &rubyEngine, &executeRubyScriptCommandArgs] {
      openstudio::cli::executeRubyScriptCommand(rubyScriptPath, rubyEngine, executeRubyScriptCommandArgs);
    });
    // }

    // {
    auto* execute_python_scriptCommand = experimentalApp->add_subcommand("execute_python_script", "Executes a python file");
    openstudio::filesystem::path pythonScriptPath;
    execute_python_scriptCommand->add_option("path", pythonScriptPath, "Path to python file")->required(true);
    std::vector<std::string> executePythonScriptCommandArgs;
    execute_python_scriptCommand->add_option("arguments", executePythonScriptCommandArgs, "Arguments to pass to the python file")
      ->required(false)
      ->option_text("args");
    execute_python_scriptCommand->callback([&pythonScriptPath, &pythonEngine, &executePythonScriptCommandArgs] {
      openstudio::cli::executePythonScriptCommand(pythonScriptPath, pythonEngine, executePythonScriptCommandArgs);
    });
    // }

    [[maybe_unused]] auto* gem_listCommand =
      experimentalApp->add_subcommand("gem_list", "Lists the set gems available to openstudio")->callback([&rubyEngine]() {
        openstudio::cli::executeGemListCommand(rubyEngine);
      });

    // Not hidding any commands right now
    // [[maybe_unused]] auto* list_commandsCommand = experimentalApp->add_subcommand("list_commands", "Lists the entire set of available commands");

    // run command
    openstudio::cli::setupRunOptions(experimentalApp, rubyEngine, pythonEngine);

    // update (model) command
    // openstudio::cli::setupUpdateCommand(experimentalApp);
    {
      bool keep = false;
      auto* updateCommand = experimentalApp->add_subcommand("update", "Updates OpenStudio Models to the current version");
      updateCommand->add_flag("-k,--keep", keep, "Keep original files");

      openstudio::filesystem::path updateOsmPath;
      updateCommand->add_option("path", updateOsmPath, "Path to OSM or directory containing osms")->required(true);

      updateCommand->callback([&keep, &updateOsmPath] {
        if (!openstudio::cli::runModelUpdateCommand(updateOsmPath, keep)) {
          throw std::runtime_error("Failed to update some models");
        }
      });
    }

    openstudio::cli::MeasureUpdateOptions::setupMeasureUpdateOptions(experimentalApp, rubyEngine, pythonEngine);

    // ==========================  V E R S I O N ==========================
    [[maybe_unused]] auto* openstudio_versionCommand =
      experimentalApp->add_subcommand("openstudio_version", "Returns the OpenStudio version used by the CLI")
        ->group(versionGroupname)
        ->callback([]() { fmt::print("{}\n", openStudioLongVersion()); });

    [[maybe_unused]] auto* energyplus_versionCommand =
      experimentalApp->add_subcommand("energyplus_version", "Returns the EnergyPlus version used by the CLI")
        ->group(versionGroupname)
        ->callback([]() { fmt::print("{}+{}\n", energyPlusVersion(), energyPlusBuildSHA()); });
    [[maybe_unused]] auto* ruby_versionCommand =
      experimentalApp->add_subcommand("ruby_version", "Returns the Ruby version used by the CLI")->group(versionGroupname)->callback([&rubyEngine]() {
        rubyEngine->exec("puts RUBY_VERSION");
      });
    [[maybe_unused]] auto* python_versionCommand = experimentalApp->add_subcommand("python_version", "Returns the Python version used by the CLI")
                                                     ->group(versionGroupname)
                                                     ->callback([&pythonEngine]() { pythonEngine->exec("import sys; print(sys.version)"); });

    // ====================================================================

    CLI11_PARSE(app, argc, argv);

    if (*execRubyOption) {
      //  fmt::print("--execute Flag received {} times.\n", execRubyOption->count());
      for (auto& cmd : executeRubyCmds) {
        // fmt::print("{}\n", cmd);
        rubyEngine->exec(cmd);
      }
    }
    if (*execPythonOption) {
      // fmt::print("--pyexecute Flag received {} times.\n", execPythonOption->count());
      for (auto& cmd : executePythonCmds) {
        // fmt::print("{}\n", cmd);
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
