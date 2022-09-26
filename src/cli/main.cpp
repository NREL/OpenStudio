#include "./RubyCLI.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../workflow/OSWorkflow.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  int result = 0;

  // ScriptEngineInstance will delay load the engines
  openstudio::ScriptEngineInstance rubyEngine("rubyengine", argc - 1, argv + 1);
  openstudio::ScriptEngineInstance pythonEngine("pythonengine", argc - 1, argv + 1);

  if ((argc > 1) && (std::string_view(argv[1]) == "labs")) {
    CLI::App app{"openstudio"};

    const auto labsCommand = app.add_subcommand("labs");
    const auto runCommand = labsCommand->add_subcommand("run");
    openstudio::filesystem::path oswPath;
    runCommand->add_option("osw", oswPath);

    CLI11_PARSE(app, argc, argv);

    if (runCommand->parsed()) {
      openstudio::OSWorkflow workflow(oswPath, rubyEngine, pythonEngine);
      workflow.run();
    }
  } else {
    result = openstudio::rubyCLI(rubyEngine);
  }

  // Important to destroy RubyEngine and finalize Ruby at the right time
  // After the main function returns it is (apparently) too late
  rubyEngine.reset();
  pythonEngine.reset();

  return result;
}
