#include "./RubyCLI.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include <iostream>

namespace openstudio {

int rubyCLI(ScriptEngineInstance &rubyEngine) {
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
  return 0;
}

} // openstudio
