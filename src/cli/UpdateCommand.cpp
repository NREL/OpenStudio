/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    cmd += fmt::format("require '{}'", rubyScriptPath.generic_string());
    rubyEngine->exec(cmd);
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
    pythonEngine->exec(cmd);
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
