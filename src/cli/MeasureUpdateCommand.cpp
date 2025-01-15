/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "MeasureUpdateCommand.hpp"
#include "MeasureManager.hpp"

#include "../utilities/core/Filesystem.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../scriptengine/ScriptEngine.hpp"
#include "../measure/OSMeasure.hpp"
#include "../measure/ModelMeasure.hpp"
#include "../measure/EnergyPlusMeasure.hpp"
#include "../measure/ReportingMeasure.hpp"
#include "../measure/OSArgument.hpp"
#include "../measure/OSOutput.hpp"
#include "../measure/OSRunner.hpp"
#include "../measure/OSMeasureInfoGetter.hpp"
#include "../utilities/idf/Workspace.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

#include <boost/optional.hpp>

#include <algorithm>  // For std::transform
#include <cstdlib>    // For std::exit
#include <memory>     // make_shared
#include <map>        // For std::map
#include <stdexcept>  // For std::runtime_error
#include <string>
#include <string_view>
#include <vector>

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
      measureCommand->add_option("-a,--compute_arguments", opt->compute_arguments_model, "Compute arguments for the given OSM or IDF")
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

    {
      auto* newMeasureSubCommand = measureCommand->add_subcommand("new", "Create a new measure");

      // TODO: this includes 'UtilityMeasure' which I don't think we want to include
      std::vector<std::string> measureTypeNames;
      {
        const auto& m = openstudio::MeasureType::getNames();
        std::transform(m.cbegin(), m.cend(), back_inserter(measureTypeNames), [](const auto& x) { return x.second; });
        // fmt::print("measureTypeNames={}\n", measureTypeNames);
        [[maybe_unused]] auto* measureTypeOpt =
          newMeasureSubCommand
            ->add_option("-t,--type", opt->newMeasureOpts.measureType,
                         fmt::format("Type of Measure [Default: '{}']: {}", opt->newMeasureOpts.measureType.valueName(), measureTypeNames))
            ->option_text("measureType")
            ->check(CLI::IsMember(measureTypeNames));
      }

      std::vector<std::string> measureLanguageNames;
      {
        const auto& m = openstudio::MeasureLanguage::getNames();
        std::transform(m.cbegin(), m.cend(), back_inserter(measureLanguageNames), [](const auto& x) { return x.second; });
        // fmt::print("measureLanguageNames={}\n", measureLanguageNames);
        [[maybe_unused]] auto* measureLanguageOpt = newMeasureSubCommand
                                                      ->add_option("-l,--language", opt->newMeasureOpts.measureLanguage,
                                                                   fmt::format("Language of Measure [Default: '{}']: {}",
                                                                               opt->newMeasureOpts.measureLanguage.valueName(), measureLanguageNames))
                                                      ->option_text("measureLanguage")
                                                      ->check(CLI::IsMember(measureLanguageNames));
      }

      newMeasureSubCommand->add_option("-c,--class-name", opt->newMeasureOpts.className, "Measure Class Name [Required]")
        ->option_text("className")
        ->required(true);

      newMeasureSubCommand->add_option("-n,--name", opt->newMeasureOpts.name, "Measure Human Readable Name [Default: className]")
        ->option_text("name")
        ->required(false);

      std::vector<std::string> taxonomyTags;
      std::vector<std::string> firstLevelTaxonomyTerms = openstudio::BCLMeasure::suggestedFirstLevelTaxonomyTerms();
      taxonomyTags.reserve(35);  // 2023-05-03: at this moment, this is the total number
      for (auto& firstLevelTaxonomyTerm : firstLevelTaxonomyTerms) {
        auto secondLevelTaxonomyTerms = openstudio::BCLMeasure::suggestedSecondLevelTaxonomyTerms(firstLevelTaxonomyTerm);
        taxonomyTags.reserve(taxonomyTags.size() + secondLevelTaxonomyTerms.size());
        std::transform(secondLevelTaxonomyTerms.begin(), secondLevelTaxonomyTerms.end(), std::back_inserter(taxonomyTags),
                       [&firstLevelTaxonomyTerm](auto&& secondLevelTaxonomyTerm) {
                         return fmt::format("{}.{}", firstLevelTaxonomyTerm, secondLevelTaxonomyTerm);
                       });
      }

      newMeasureSubCommand
        ->add_option("--taxonomy-tag", opt->newMeasureOpts.taxonomyTag, fmt::format("Taxonomy Tag [Default: '{}']", opt->newMeasureOpts.taxonomyTag))
        ->option_text("tag")
        ->required(false)
        ->check(CLI::IsMember(taxonomyTags))
        ->capture_default_str();
      newMeasureSubCommand->add_option("-d,--description", opt->newMeasureOpts.description, "Description")->option_text("desc")->required(false);
      newMeasureSubCommand->add_option("-m,--modeler-description", opt->newMeasureOpts.modelerDescription, "Modeler Description")
        ->option_text("modeler_desc")
        ->required(false);

      [[maybe_unused]] auto* directoryPathOpt =
        newMeasureSubCommand->add_option("DIRECTORY", opt->newMeasureOpts.directoryPath, "Directory for the measure")
          ->option_text(" ")
          ->required(true)
          ->check(CLI::NonexistentPath);

      {
        static constexpr auto helpOptionsGroupName = "Help";
        static constexpr auto newMeasureExamples = R"(Examples:

1. Create a Ruby ModelMeasure:

```
$ openstudio measure new --class-name MyExampleRubyModelMeasure
$ openstudio measure new --class-name MyExampleRubyModelMeasure --type ModelMeasure --language Ruby
```

2. Pass all optional args to create a Python EnergyPlusMeasure:

```
$ openstudio measure new --class-name MyExamplePythonMeasure --type EnergyPlusMeasure --language Python --name "My Python Measure" --description "This is my measure" --modeler-description "This does complicated stuff" --taxonomy-tag "Envelope.Form" ./test_measure

$ openstudio measure new -c MyExamplePythonMeasure -t EnergyPlusMeasure -l Python -n "My Python Measure" -d "This is my measure" -m "This does complicated stuff" --taxonomy-tag "Envelope.Form" ./test_measure
```

3. List taxonomy tags
```
$ openstudio measure new --list-taxonomy-tags
$ openstudio measure new --list-for-first-taxonomy-tag HVAC
```
)";
        newMeasureSubCommand->set_help_flag("-h,--help", "Print this help message and exit")->group(helpOptionsGroupName);

        newMeasureSubCommand
          ->add_flag_callback(
            "--examples",
            []() {
              fmt::print("{}\n", newMeasureExamples);
              std::exit(0);  // NOLINT(concurrency-mt-unsafe)
            },
            "Show Example usage")
          ->group(helpOptionsGroupName);

        [[maybe_unused]] auto* listTaxonomyFlag = newMeasureSubCommand
                                                    ->add_flag_callback(
                                                      "--list-taxonomy-tags",
                                                      [taxonomyTags]() {
                                                        fmt::print("{}\n", taxonomyTags);
                                                        std::exit(0);  // NOLINT(concurrency-mt-unsafe)
                                                      },
                                                      "List all available Taxonomy tags")
                                                    ->group(helpOptionsGroupName);

        newMeasureSubCommand
          ->add_option_function<std::string>(
            "--list-for-first-taxonomy-tag",
            [taxonomyTags](const std::string& firstLevelTaxonomyTerm) {
              auto secondLevelTaxonomyTerms = openstudio::BCLMeasure::suggestedSecondLevelTaxonomyTerms(firstLevelTaxonomyTerm);
              std::vector<std::string> taxonomyTags;
              taxonomyTags.reserve(secondLevelTaxonomyTerms.size());
              std::transform(secondLevelTaxonomyTerms.begin(), secondLevelTaxonomyTerms.end(), std::back_inserter(taxonomyTags),
                             [&firstLevelTaxonomyTerm](auto&& secondLevelTaxonomyTerm) {
                               return fmt::format("{}.{}", firstLevelTaxonomyTerm, secondLevelTaxonomyTerm);
                             });
              fmt::print("{}\n", taxonomyTags);
              std::exit(0);  // NOLINT(concurrency-mt-unsafe)
            },
            "Limit taxonomy tags to this first level")
          ->option_text("tag")
          // ->excludes(listTaxonomyFlag)
          ->check(CLI::IsMember(firstLevelTaxonomyTerms))
          ->group(helpOptionsGroupName);

        // newMeasureSubCommand->footer(std::string{newMeasureExamples});
      }

      newMeasureSubCommand->callback([opt] {
        // opt->newMeasureOpts.debug_print();

        if (opt->newMeasureOpts.name.empty()) {
          opt->newMeasureOpts.name = opt->newMeasureOpts.className;
        }

        BCLMeasure b(opt->newMeasureOpts.name, opt->newMeasureOpts.className, opt->newMeasureOpts.directoryPath, opt->newMeasureOpts.taxonomyTag,
                     opt->newMeasureOpts.measureType, opt->newMeasureOpts.description, opt->newMeasureOpts.modelerDescription,
                     opt->newMeasureOpts.measureLanguage);

        fmt::print("Created a {} {} with class name '{}' in '{}'\n", opt->newMeasureOpts.measureLanguage.valueName(),
                   opt->newMeasureOpts.measureType.valueName(), opt->newMeasureOpts.className,
                   openstudio::toString(openstudio::filesystem::canonical(b.directory())));

        std::exit(0);  // NOLINT(concurrency-mt-unsafe)
      });
    }

    {

      auto* copyMeasureSubCommand = measureCommand->add_subcommand("copy", "Copy a measure");
      [[maybe_unused]] auto* existingDirectoryPathOpt =
        copyMeasureSubCommand->add_option("EXISTING_DIRECTORY", opt->directoryPath, "Existing Directory for the measure")
          ->option_text(" ")
          ->required(true)
          ->check(CLI::ExistingDirectory);
      [[maybe_unused]] auto* newDirectoryPathOpt =
        copyMeasureSubCommand->add_option("NEW_DIRECTORY", opt->newMeasureOpts.directoryPath, "New Directory for the measure")
          ->option_text(" ")
          ->required(true)
          ->check(CLI::NonexistentPath);

      copyMeasureSubCommand->callback([opt] {
        boost::optional<BCLMeasure> b_;
        try {
          b_ = BCLMeasure(opt->directoryPath);
        } catch (...) {
          fmt::print(stderr, "Could not find a valid measure at '{}'\n", openstudio::toString(opt->directoryPath));
          std::exit(1);
        }
        auto bClone_ = b_->clone(opt->newMeasureOpts.directoryPath);
        if (bClone_) {
          // Force updating the UID
          bClone_->changeUID();
          bClone_->checkForUpdatesFiles();
          bClone_->checkForUpdatesXML();
          bClone_->save();
          fmt::print("Cloned the {} {} with class name '{}' from '{}' to '{}'\n", b_->measureLanguage().valueName(), b_->measureType().valueName(),
                     b_->className(), openstudio::toString(openstudio::filesystem::canonical(b_->directory())),
                     openstudio::toString(openstudio::filesystem::canonical(bClone_->directory())));
          std::exit(0);  // NOLINT(concurrency-mt-unsafe)
        } else {
          fmt::print(stderr, "Something went wrong when cloning the measure");
          std::exit(1);  // NOLINT(concurrency-mt-unsafe)
        }
      });
    }

    measureCommand->callback([opt, &rubyEngine, &pythonEngine] { MeasureUpdateOptions::execute(*opt, rubyEngine, pythonEngine); });
  }

  void MeasureUpdateOptions::execute(MeasureUpdateOptions const& opt, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine) {
    // opt.debug_print();

    if (opt.server_port > 0) {
      MeasureManagerServer server(opt.server_port, rubyEngine, pythonEngine);
      server.open();              // This starts the cpprestsdk http listener, which processes tasks in subthreads
      server.do_tasks_forever();  // This starts the event loop on the **main** thread to process the requests there and NOT in a subthread

      return;
    } else if (opt.update) {
      MeasureManager measureManager(rubyEngine, pythonEngine);
      if (auto measure_ = measureManager.getMeasure(opt.directoryPath, true)) {
        // TODO: maybe I should write an OSMeasureInfo::toJSON() method, but that'd be duplicating the code in BCLMeasure (BCLXML to be exact).
        // So since the only thing that's different is the OSArgument (OSMeasureInfo) versus BCLMeasureArgument (BCLMeasure), we just override
        auto result = measure_->toJSON();

        fmt::print("{}\n", result.toStyledString());
      } else {
        fmt::print(stderr, "Cannot load measure from {}\n", opt.directoryPath.generic_string());
      }
      return;
    } else if (opt.update_all) {
      MeasureManager measureManager(rubyEngine, pythonEngine);
      std::vector<openstudio::path> subDirPaths;
      for (auto const& dir_entry : boost::filesystem::directory_iterator{opt.directoryPath}) {
        const auto& subDirPath = dir_entry.path();

        if (openstudio::filesystem::is_directory(subDirPath) && openstudio::filesystem::is_regular_file(subDirPath / "measure.xml")) {
          subDirPaths.emplace_back(subDirPath);
        }
      }
      fmt::print("Found {} measure directories to update\n", subDirPaths.size());
      for (const auto& subDirPath : subDirPaths) {
        measureManager.getMeasure(subDirPath, true);
      }
    } else if (!opt.compute_arguments_model.empty()) {
      // NOTE: cannot move into MeasureManager to avoid repeating the code shared with MeasureManagerServer. This expects an OSM **or an IDF** in the
      // case of an EnergyPlus Measure, whereas the server /compute_arguments endpoint always expects an OSM
      MeasureManager measureManager(rubyEngine, pythonEngine);
      auto measure_ = measureManager.getMeasure(opt.directoryPath, true);
      if (!measure_) {
        auto msg = fmt::format("Cannot load measure at '{}'", opt.directoryPath.generic_string());
        fmt::print(stderr, "{}\n", msg);
        return;
      }

      openstudio::measure::OSMeasureInfo info = measureManager.getMeasureInfo(opt.directoryPath, *measure_, opt.compute_arguments_model);
      if (auto errorString_ = info.error()) {
        fmt::print(stderr, "{}\n", *errorString_);
        return;
      }
      // TODO: maybe I should write an OSMeasureInfo::toJSON() method, but that'd be duplicating the code in BCLMeasure (BCLXML to be exact).
      // So since the only thing that's different is the OSArgument (OSMeasureInfo) versus BCLMeasureArgument (BCLMeasure), we just override
      auto result = measure_->toJSON();
      auto& arguments = result["arguments"];
      arguments.clear();
      for (const measure::OSArgument& argument : info.arguments()) {
        arguments.append(argument.toJSON());
      }
      Json::StreamWriterBuilder builder;
      builder["commentStyle"] = "None";
      builder["indentation"] = "  ";
      std::string resultStr = Json::writeString(builder, result);
      fmt::print("{}\n", resultStr);
      return;
    } else if (opt.run_tests) {

      auto canonicalTestDir = openstudio::filesystem::canonical(opt.directoryPath);

      // Pytest
      fmt::print(fmt::fg(fmt::color::yellow),
                 "┌{0:─^{2}}┐\n"
                 "│{1: ^{2}}│\n"
                 "└{0:─^{2}}┘",
                 "", "Starting Python Tests", 80);
      fmt::print("\n");
      auto pytestOutDir = canonicalTestDir / "test_results" / "pytest";

      // -o junit_familar=legacy is to mimic having a pytest.ini with:
      // [pytest]
      // junit_family=legacy
      // This uses xunit1 format, and adds the line + file at which error occured, something we don't have with xunit2 (default)

      auto runPytestCmd = fmt::format(
        R"python(
import pytest

pytest.main([
  "--junit-xml={junit}",
  "--cov={test_dir}",
  "--cov-report=term-missing",
  "--cov-report=json:{json}",
  "--cov-report=lcov:{lcov}",
  "--cov-report=html:{html}",
  "--cov-report=xml:{xml}",
  "-o", "junit_family=legacy",
  "{test_dir}",
])
)python",
        fmt::arg("junit", (pytestOutDir / "junit.xml").generic_string()),  //
        fmt::arg("test_dir", canonicalTestDir.generic_string()),           //
        fmt::arg("json", (pytestOutDir / "python_coverage.json").generic_string()),
        fmt::arg("lcov", (pytestOutDir / "python_coverage.lcov").generic_string()),
        fmt::arg("html", (pytestOutDir / "python_coverage_html").generic_string()),
        fmt::arg("xml", (pytestOutDir / "python_coverage.xml").generic_string()));

      fmt::print("runPytestCmd={}\n", runPytestCmd);
      pythonEngine->exec(runPytestCmd);

      fmt::print(fmt::fg(fmt::color::red),
                 "┌{0:─^{2}}┐\n"
                 "│{1: ^{2}}│\n"
                 "└{0:─^{2}}┘",
                 "", "Starting ruby tests", 80);
      fmt::print("\n");

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
        canonicalTestDir.generic_string());
      rubyEngine->exec(runTestCmd);
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

  void MeasureNewOptions::debug_print() const {
    fmt::print("\nMeasureNewOptions:\n");
    fmt::print("name={}\n", name);
    fmt::print("className={}\n", className);
    fmt::print("directoryPath={}\n", openstudio::toString(directoryPath));
    fmt::print("taxonomyTag={}\n", taxonomyTag);
    fmt::print("measureType={}\n", measureType.valueName());
    fmt::print("description={}\n", description);
    fmt::print("modelerDescription={}\n", modelerDescription);
    fmt::print("measureLanguage={}\n", measureLanguage.valueName());
  };

}  // namespace cli
}  // namespace openstudio
