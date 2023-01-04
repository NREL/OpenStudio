

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

#include "MeasureUpdateCommand.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../scriptengine/ScriptEngine.hpp"

#include <fmt/format.h>
#include <boost/optional.hpp>
#include <memory>
#include <stdexcept>

namespace openstudio {
namespace cli {

  void MeasureUpdateOptions::setupMeasureUpdateOptions(CLI::App* parentApp, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine) {
    /// Set up a subcommand and capture a shared_ptr to a struct that holds all its options.
    /// The variables of the struct are bound to the CLI options.
    /// We use a shared ptr so that the addresses of the variables remain for binding,
    /// You could return the shared pointer if you wanted to access the values in main.

    auto opt = std::make_shared<MeasureUpdateOptions>();

    auto* measureCommand = parentApp->add_subcommand("measure", "Measure Commands");

    // Positional
    auto* directoryPathOpt = measureCommand->add_option("DIRECTORY", opt->directoryPath, "Directory containing the measure(s)")->option_text(" ");

    // TODO: I'm trying to preserve historical behavior, but --update, --update_all, --start_server should all be subcommands instead since they are
    // exclusive! At least start_server should, so that I can make directory required...
    auto* updateOpt = measureCommand->add_flag("-u,--update", opt->update, "Update the measure.xml")->needs(directoryPathOpt);

    auto* updateAllOpt = measureCommand->add_flag("-t,--update_all", opt->update_all, "Update all measures in a directory")
                           ->needs(directoryPathOpt)
                           ->excludes(updateOpt);

    auto* computeArgsOpt =
      measureCommand->add_option("-a,--compute_arguments", opt->compute_arguments_model, "Specify the FILE path to the workflow to run")
        ->option_text("MODEL")
        ->needs(directoryPathOpt)
        ->excludes(updateOpt, updateAllOpt);

    measureCommand
      ->add_flag("-r,--run_tests", opt->run_tests, "Run all tests recursively found in a directory, additional arguments are passed to minitest")
      ->needs(directoryPathOpt)
      ->excludes(updateOpt, updateAllOpt, computeArgsOpt);

    [[maybe_unused]] auto* startServerOpt = measureCommand->add_option("-s,--start_server", opt->server_port, "Start a measure manager server")
                                              ->option_text("PORT")
                                              ->excludes(directoryPathOpt);

    measureCommand->callback([opt, &rubyEngine, &pythonEngine] { MeasureUpdateOptions::execute(*opt, rubyEngine, pythonEngine); });
  }

  boost::optional<BCLMeasure> getAndUpdateMeasure(const openstudio::path& directoryPath, ScriptEngineInstance& rubyEngine,
                                                  ScriptEngineInstance& pythonEngine) {

    const auto& directoryPathStr = directoryPath.string();

    // check if measure exists on disk
    if (!openstudio::filesystem::is_directory(directoryPath)) {
      fmt::print("Measure '{}' does not exist.\n", directoryPathStr);
      return boost::none;
    }
    if (!openstudio::filesystem::is_regular_file(directoryPath / "measure.xml")) {
      fmt::print("Measure directory '{}' exists but does not have a measure.xml.\n", directoryPathStr);
      return boost::none;
    }

    // load from disk
    fmt::print("Attempting to load measure '{}'\n", directoryPathStr);

    auto measure_ = openstudio::BCLMeasure::load(directoryPath);

    if (!measure_) {
      fmt::print("Failed to load measure '{}'\n", directoryPathStr);
      return boost::none;
    }
    fmt::print("Successfully loaded measure '{}'\n", directoryPathStr);

    // see if there are updates, want to make sure to perform both checks so do outside of conditional
    bool file_updates = measure_->checkForUpdatesFiles();  // checks if any files have been updated
    bool xml_updates = measure_->checkForUpdatesXML();     // only checks if xml as loaded has been changed since last save

    auto readmeInPath = directoryPath / "README.md.erb";
    auto readmeOutPath = directoryPath / "README.md";

    bool readme_out_of_date = openstudio::filesystem::is_regular_file(readmeInPath) && !openstudio::filesystem::is_regular_file(readmeOutPath);

    // TODO: try catch like in measure_manager.rb?
    bool missing_fields = measure_->missingRequiredFields();

    if (file_updates || xml_updates || missing_fields || readme_out_of_date) {
      fmt::print("Changes detected, updating '{}'\n", directoryPathStr);

      // TODO: the readme.md generation from readme.md.erb requires ruby.
      fmt::print("Warn: readme.md generation from ERB is not supported yet\n");

      // Save the xml file with changes triggered by checkForUpdatesFiles() / checkForUpdatesXML() above
      measure_->save();

      // TODO: also need to mimic OSMeasureInfoGetter::getInfo which is actually implemented in ruby itself...
      // basically we need to compute the arguments of the measures, inspect outputs... so that requires doing something similar to what we do in the
      // `run` method
    }

    return measure_;
  }

  void MeasureUpdateOptions::execute(MeasureUpdateOptions const& opt, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine) {
    opt.debug_print();

    if (opt.server_port > 0) {
      const auto measureManagerCmd = fmt::format(
        R"ruby(
require 'measure_manager_server'
port = {}

server = WEBrick::HTTPServer.new(:Port => port)

server.mount "/", MeasureManagerServlet

trap("INT") {{
    server.shutdown
}}

server.start)ruby",
        opt.server_port);

      rubyEngine->exec(measureManagerCmd);

    } else if (opt.update) {
      getAndUpdateMeasure(opt.directoryPath, rubyEngine, pythonEngine);

    } else if (opt.update_all) {
      std::vector<openstudio::path> subDirPaths;
      for (auto const& dir_entry : boost::filesystem::directory_iterator{opt.directoryPath}) {
        const auto& subDirPath = dir_entry.path();

        if (openstudio::filesystem::is_directory(subDirPath) && openstudio::filesystem::is_regular_file(subDirPath / "measure.xml")) {
          subDirPaths.emplace_back(subDirPath);
        }
      }
      fmt::print("Found {} measure directories to update\n", subDirPaths.size());
      for (const auto& subDirPath : subDirPaths) {
        getAndUpdateMeasure(subDirPath, rubyEngine, pythonEngine);
      }

    } else if (!opt.compute_arguments_model.empty()) {
      // TODO: Same comment as above: we need to call the measure methods same as what we do in OSWorkflow::run
      throw std::runtime_error("compute_arguments_model not implemented yet");
    } else if (opt.run_tests) {
      auto measure_ = getAndUpdateMeasure(opt.directoryPath, rubyEngine, pythonEngine);
      if (measure_->measureLanguage() == MeasureLanguage::Ruby) {
        // TODO: need to capture arguments and pass as ARGV

        rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");

        auto runTestCmd = fmt::format(
          R"ruby(
# load openstudio_measure_tester gem
#begin
  require 'minitest'
  require 'minitest/reporters'

  # Minitest Reports use a plugin that is normally found by Minitest::load_plugins using Gem.find
  # until Gem.find is overloaded to find embedded gems, we will manually load the plugin here
  require 'minitest/minitest_reporter_plugin'
  Minitest.extensions << 'minitest_reporter'

  require 'openstudio_measure_tester'
#rescue LoadError
  #puts "Cannot load 'openstudio_measure_tester'"
  #return 1
#end

runner = OpenStudioMeasureTester::Runner.new('{}')
result = runner.run_all(Dir.pwd)    # <--------------- TODO: this is problematic?

if result != 0
  $logger.error("Measure tester returned errors")
  return 1
end
)ruby",
          opt.directoryPath.generic_string());
        rubyEngine->exec(runTestCmd);

      } else if (measure_->measureLanguage() == MeasureLanguage::Python) {
        // TODO:
        throw std::runtime_error("run_tests not implemented yet for python");
      }
    }
  }

  void MeasureUpdateOptions::debug_print() const {
    fmt::print("\nMeasureUpdateOptions:\n");
    fmt::print("directoryPath={}\n", this->directoryPath.string());
    fmt::print("update={}\n", this->update);
    fmt::print("update_all={}\n", this->update_all);
    fmt::print("compute_arguments_model={}\n", this->compute_arguments_model.string());
    fmt::print("run_tests={}\n", this->run_tests);
    fmt::print("server_port={}\n", this->server_port);
    fmt::print("\n\n");
  }

}  // namespace cli
}  // namespace openstudio
