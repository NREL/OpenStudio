#include "../scriptengine/ScriptEngine.hpp"
#include <iostream>

std::unique_ptr<openstudio::ScriptEngine> rubyEngine;
std::unique_ptr<openstudio::ScriptEngine> pythonEngine;

int main(int argc, char* argv[]) {
  // chop off the first argument which is the exe path/name
  rubyEngine = openstudio::loadScriptEngine("rubyengine", argc - 1, argv + 1);

  try {
    rubyEngine->exec(R"(
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
    rubyEngine->exec(R"(STDOUT.flush)");
    std::cout << "Exception: " << e.what() << std::endl;  // endl will flush
    return 1;
  } catch (...) {
    rubyEngine->exec(R"(STDOUT.flush)");
    std::cout << "Unknown Exception" << std::endl;  // endl will flush
    return 1;
  }

  rubyEngine->exec(R"(STDOUT.flush)");
  std::cout << std::flush;
}
