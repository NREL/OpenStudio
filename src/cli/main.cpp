#include "./ScriptEngineGlobals.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

int rubyCLI(int argc, char* argv[]) {
  // chop off the first argument which is the exe path/name
  openstudio::rubyEngine = openstudio::loadScriptEngine("rubyengine", argc - 1, argv + 1);

  try {
    openstudio::rubyEngine->exec(R"(
       begin
         require 'openstudio_cli'
       rescue Exception => e
         puts
         puts "Error: #{e.message}"
         puts "Backtrace:\n\t" + e.backtrace.join("\n\t")
         raise
       end
     )");
  } catch (const std::exception& e) {
    openstudio::rubyEngine->exec(R"(STDOUT.flush)");
    std::cout << "Exception: " << e.what() << std::endl;  // endl will flush
    return 1;
  } catch (...) {
    openstudio::rubyEngine->exec(R"(STDOUT.flush)");
    std::cout << "Unknown Exception" << std::endl;  // endl will flush
    return 1;
  }

  openstudio::rubyEngine->exec(R"(STDOUT.flush)");
  std::cout << std::flush;
  return 0;
}

int main(int argc, char* argv[]) {
  int result = 0;

  if ((argc > 1) && (std::string_view(argv[1]) == "experimental")) {
    CLI::App app{"openstudio"};

    const auto experimentalApp = app.add_subcommand("experimental");
    [[maybe_unused]] const auto runCommand = experimentalApp->add_subcommand("run");

    CLI11_PARSE(app, argc, argv);
  } else {
    result = rubyCLI(argc, argv);
  }

  // Important to destroy RubyEngine and finalize Ruby at the right time
  // After the main function returns it is (apparently) too late
  openstudio::rubyEngine.reset();
  return result;
}
