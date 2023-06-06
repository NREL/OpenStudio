/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "UpdateCommand.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/core/Logger.hpp"
#include "../osversion/VersionTranslator.hpp"
#include "../model/Model.hpp"
#include "../scriptengine/ScriptEngine.hpp"

#include <fmt/format.h>

#include <memory>
#include <stdexcept>

namespace openstudio {
namespace cli {

  bool runModelUpdateCommand(const openstudio::path& p, bool keep) {
    std::vector<openstudio::path> osmPaths;

    openstudio::osversion::VersionTranslator vt;

    if (openstudio::filesystem::is_directory(p)) {
      for (auto const& dir_entry : boost::filesystem::directory_iterator{p}) {
        const auto& thePath = dir_entry.path();
        if (openstudio::filesystem::is_regular_file(thePath) && thePath.extension() == ".osm") {
          osmPaths.emplace_back(thePath);
        }
      }
    } else {
      osmPaths.push_back(p);
    }

    bool result = true;
    for (const auto& relPath : osmPaths) {
      fmt::print("'{}'\n", relPath.string());
      auto osmPath = openstudio::filesystem::system_complete(relPath);
      if (keep) {
        openstudio::path backupPath = osmPath;
        backupPath.replace_extension(openstudio::toPath(".osm.orig"));
        openstudio::filesystem::copy_file(osmPath, backupPath, openstudio::filesystem::copy_options::overwrite_existing);
      }
      if (auto model_ = vt.loadModel(osmPath)) {
        fmt::print("Updated '{}'\n", osmPath.string());
        model_->save(osmPath, true);
      } else {
        fmt::print("Could not read model at '{}'\n", osmPath.string());
        result = false;
      }
    }
    return result;
  }

  void executeRubyScriptCommand(openstudio::path rubyScriptPath, ScriptEngineInstance& rubyEngine, const std::vector<std::string>& arguments) {
    rubyScriptPath = openstudio::filesystem::system_complete(rubyScriptPath);
    LOG_FREE(Debug, "executeRubyScriptCommand", "Path for the file to run: " << rubyScriptPath);
    if (!openstudio::filesystem::is_regular_file(rubyScriptPath)) {
      throw std::runtime_error(fmt::format("Unable to find the file '{}' on the filesystem", rubyScriptPath.string()));
    }

    std::string cmd = "ARGV.clear\n";
    for (const auto& arg : arguments) {
      cmd += fmt::format("ARGV << \"{}\"\n", arg);
    }
    cmd += fmt::format(R"(
begin
  require '{}'
rescue Exception => e
  puts
  puts "Error: #{{e.message}}"
  puts "Backtrace:\n\t" + e.backtrace.join("\n\t")
  raise
end
     )",
                       rubyScriptPath.generic_string());
    try {
      rubyEngine->exec(cmd);
    } catch (...) {
      // Bail faster though ruby isn't slow like python
      fmt::print(stderr, "Failed to execute '{}'\n", rubyScriptPath.generic_string());
      exit(1);
      // throw std::runtime_error(fmt::format("Failed to execute '{}'\n", rubyScriptPath.generic_string()));
    }
  }

  void executePythonScriptCommand(openstudio::path pythonScriptPath, ScriptEngineInstance& pythonEngine, const std::vector<std::string>& arguments) {
    pythonScriptPath = openstudio::filesystem::system_complete(pythonScriptPath);
    LOG_FREE(Debug, "executePythonScriptCommand", "Path for the file to run: " << pythonScriptPath);
    if (!openstudio::filesystem::is_regular_file(pythonScriptPath)) {
      throw std::runtime_error(fmt::format("Unable to find the file '{}' on the filesystem", pythonScriptPath.string()));
    }
    // There's probably better to be done, like instantiating the pythonEngine with the argc/argv then calling PyRun_SimpleFile but whatever
    std::string cmd = fmt::format(R"python(import sys
sys.argv.clear()
sys.argv.append("{}")
)python",
                                  pythonScriptPath.filename().string());
    for (const auto& arg : arguments) {
      cmd += fmt::format("sys.argv.append(\"{}\")\n", arg);
    }
    cmd += fmt::format(R"python(
import importlib.util
spec = importlib.util.spec_from_file_location('__main__', r'{}')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
)python",
                       pythonScriptPath.generic_string());
    // fmt::print("{}\n", cmd);
    try {
      pythonEngine->exec(cmd);
    } catch (...) {
      // Bail faster...
      fmt::print(stderr, "Failed to execute '{}'\n", pythonScriptPath.generic_string());
      exit(1);
      // throw std::runtime_error(fmt::format("Failed to execute '{}'\n", pythonScriptPath.generic_string()));
    }
  }

  void executeGemListCommand(ScriptEngineInstance& rubyEngine) {

    std::string cmd = R"ruby(
begin
  embedded = []
  user = []
  Gem::Specification.find_all.each do |spec|
    if spec.gem_dir.chars.first == ':'
      embedded << spec
    else
      user << spec
    end
  end

  embedded.each do |spec|
    safe_puts "#{spec.name} (#{spec.version}) '#{spec.gem_dir}'"
  end

  user.each do |spec|
    safe_puts "#{spec.name} (#{spec.version}) '#{spec.gem_dir}'"
  end

rescue => e
  $logger.error "Error listing gems: #{e.message} in #{e.backtrace.join("\n")}"
  exit e.exit_code
end
    )ruby";
    rubyEngine->exec(cmd);
  }

}  // namespace cli
}  // namespace openstudio
