/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
$ openstudio labs measure new --class-name MyExampleRubyModelMeasure
$ openstudio labs measure new --class-name MyExampleRubyModelMeasure --type ModelMeasure --language Ruby
```

2. Pass all optional args to create a Python EnergyPlusMeasure:

```
$ openstudio labs measure new --class-name MyExamplePythonMeasure --type EnergyPlusMeasure --language Python --name "My Python Measure" --description "This is my measure" --modeler-description "This does complicated stuff" --taxonomy-tag "Envelope.Form" ./test_measure

$ openstudio labs measure new -c MyExamplePythonMeasure -t EnergyPlusMeasure -l Python -n "My Python Measure" -d "This is my measure" -m "This does complicated stuff" --taxonomy-tag "Envelope.Form" ./test_measure
```

3. List taxonomy tags
```
$ openstudio labs measure new --list-taxonomy-tags
$ openstudio labs measure new --list-for-first-taxonomy-tag HVAC
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

      // TODO: Also need to mimic OSMeasureInfoGetter::getInfo which is actually implemented in ruby itself...
      // basically we need to compute the arguments of the measures, inspect outputs... so that requires doing something similar to what we do in the
      // `run` method

      fmt::print("Measure at {} uses language = {}.\n", directoryPathStr, measure_->measureLanguage().valueName());

      auto scriptPath_ = measure_->primaryScriptPath();
      if (!scriptPath_) {
        throw std::runtime_error(
          fmt::format("Unable to locate primary Ruby script path for BCLMeasure '{}' located at '{}'", measure_->name(), directoryPathStr));
      }

      // TODO: Here we need to do two things:
      // * Find the class name by scanning the measure.rb/.py, then instantiate the measure
      // * Do the same as in ApplyMeasure and compute the arguments with a dumb model/workspace, or the supplied one (for --compute_arguments)
      ScriptEngineInstance* thisEngine = nullptr;
      ScriptObject measureScriptObject;
      // openstudio::measure::OSMeasure* measurePtr = nullptr;

      std::string className;
      MeasureType measureType;

      std::string name;
      std::string description;
      std::string taxonomy;
      std::string modelerDescription;

      std::vector<measure::OSArgument> arguments;
      std::vector<measure::OSOutput> outputs;

      if (measure_->measureLanguage() == MeasureLanguage::Ruby) {
        // same as the beginning of the OSMeasureInfoGetter::infoExtractorRubyFunction
        auto importCmd = fmt::format(R"ruby(
currentObjects = Hash.new
ObjectSpace.each_object(OpenStudio::Measure::OSMeasure) do |obj|
  currentObjects[obj] = true
end

measurePath = "{}"
ObjectSpace.garbage_collect
# This line is REQUIRED or the ObjectSpace order will change when garbage collection runs automatically
# If ~12 measures are added and garbage collection runs, the following loop to grab the first measure
# will get the wrong one and return incorrect arguments
ObjectSpace.garbage_collect
load measurePath # need load in case have seen this script before

# Make them global, so C++ can grab them
$measure = nil
$measure_type = String.new
$measure_name = String.new
ObjectSpace.each_object(OpenStudio::Measure::OSMeasure) do |obj|
  if not currentObjects[obj]
    if obj.is_a? OpenStudio::Measure::ModelMeasure
      $measure = obj
      $measure_type = "ModelMeasure"
    elsif obj.is_a? OpenStudio::Measure::EnergyPlusMeasure
      $measure = obj
      $measure_type = "EnergyPlusMeasure"
    elsif obj.is_a? OpenStudio::Measure::ReportingMeasure
      $measure = obj
      $measure_type = "ReportingMeasure"
    end
  end
end

if not $measure
  raise "Unable to extract OpenStudio::Measure::OSMeasure object from " +
       measurePath + ". The script should contain a class that derives " +
      "from OpenStudio::Measure::OSMeasure and should close with a line stating " +
      "the class name followed by .new.registerWithApplication."
end

$measure_name = $measure.class.to_s
puts "#{{$measure}}, #{{$measure_type}}, #{{$measure_name}}"
)ruby",
                                     scriptPath_->generic_string());
        fmt::print("Debug: importCmd=\n{}\n\n", importCmd);
        thisEngine = &rubyEngine;

        rubyEngine->exec(importCmd);

        fmt::print("Import done\n");
        rubyEngine->exec("puts $measure_type");

        // TODO: gotta figure out a way to retrieve a frigging string
        // ScriptObject measureTypeObject = rubyEngine->eval("measure_type");
        ScriptObject measureTypeObject = rubyEngine->eval("$measure_type");
        std::string measureTypeStr = *(rubyEngine->getAs<std::string*>(measureTypeObject));
        fmt::print("measureTypeStr={}\n", measureTypeStr);
        measureType = MeasureType(measureTypeStr);

        measureScriptObject = rubyEngine->eval("$measure");
        ScriptObject measureClassNameObject = rubyEngine->eval("$measure_name");
        className = *(*thisEngine)->getAs<std::string*>(measureClassNameObject);
        fmt::print("className={}\n", className);

        if (measureType == MeasureType::ModelMeasure) {
          auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

          name = measurePtr->name();
          description = measurePtr->description();
          taxonomy = measurePtr->taxonomy();
          modelerDescription = measurePtr->modeler_description();

          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
          outputs = measurePtr->outputs();

        } else if (measureType == MeasureType::EnergyPlusMeasure) {
          auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::EnergyPlusMeasure*>(measureScriptObject);
          name = measurePtr->name();
          description = measurePtr->description();
          taxonomy = measurePtr->taxonomy();
          modelerDescription = measurePtr->modeler_description();

          openstudio::Workspace workspace(openstudio::StrictnessLevel::Draft, openstudio::IddFileType::EnergyPlus);
          arguments = measurePtr->arguments(workspace);
          outputs = measurePtr->outputs();
        } else if (measureType == MeasureType::ReportingMeasure) {
          auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ReportingMeasure*>(measureScriptObject);
          name = measurePtr->name();
          description = measurePtr->description();
          taxonomy = measurePtr->taxonomy();
          modelerDescription = measurePtr->modeler_description();

          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
          outputs = measurePtr->outputs();

        } else {
          throw std::runtime_error("Unknown");
        }

        if (name.empty()) {
          name = className;
        }

      } else if (measure_->measureLanguage() == MeasureLanguage::Python) {
        // TODO: call initialization of the pythonEngine
        auto importCmd = fmt::format(R"python(
import importlib.util
import inspect
spec = importlib.util.spec_from_file_location(f"throwaway", "{}")

module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
class_members = inspect.getmembers(module, lambda x: inspect.isclass(x) and issubclass(x, openstudio.measure.OSMeasure))
assert len(class_members) == 1
measure_name, measure_typeinfo = class_members[0]
print(f"{{measure_name}}, {{measure_typeinfo}}")
measure_type = None
if issubclass(measure_typeinfo, openstudio.measure.ModelMeasure):
    measure_type = "ModelMeasure"
elif issubclass(measure_typeinfo, openstudio.measure.EnergyPlusMeasure):
    measure_type = "EnergyPlusMeasure"
elif issubclass(measure_typeinfo, openstudio.measure.ReportingMeasure):
    measure_type = "ReportingMeasure"
print(f"{{measure_name}}, {{measure_typeinfo}}, {{measure_type}}")
)python",
                                     scriptPath_->generic_string());
        pythonEngine->exec(importCmd);
        thisEngine = &pythonEngine;
        // measureScriptObject = pythonEngine->eval(fmt::format("module.{}()", className));
        measureScriptObject = pythonEngine->eval("measure_typeinfo()");

        // TODO: gotta figure out a way to retrieve a frigging string
        ScriptObject measureTypeObject = pythonEngine->eval("measure_type");
        std::string measureTypeStr = *(*thisEngine)->getAs<std::string*>(measureTypeObject);
        measureType = MeasureType(measureTypeStr);

        ScriptObject measureClassNameObject = pythonEngine->eval("measure_name");
        className = *(*thisEngine)->getAs<std::string*>(measureClassNameObject);

        fmt::print("measureTypeStr={}\n", measureTypeStr);

        if (measureType == MeasureType::ModelMeasure) {
          auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);

          name = measurePtr->name();
          description = measurePtr->description();
          taxonomy = measurePtr->taxonomy();
          modelerDescription = measurePtr->modeler_description();

          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
          outputs = measurePtr->outputs();

        } else if (measureType == MeasureType::EnergyPlusMeasure) {
          auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::EnergyPlusMeasure*>(measureScriptObject);
          name = measurePtr->name();
          description = measurePtr->description();
          taxonomy = measurePtr->taxonomy();
          modelerDescription = measurePtr->modeler_description();

          openstudio::Workspace workspace(openstudio::StrictnessLevel::Draft, openstudio::IddFileType::EnergyPlus);
          arguments = measurePtr->arguments(workspace);
          outputs = measurePtr->outputs();
        } else if (measureType == MeasureType::ReportingMeasure) {
          auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::ReportingMeasure*>(measureScriptObject);
          name = measurePtr->name();
          description = measurePtr->description();
          taxonomy = measurePtr->taxonomy();
          modelerDescription = measurePtr->modeler_description();

          openstudio::model::Model model;
          arguments = measurePtr->arguments(model);
          outputs = measurePtr->outputs();

        } else {
          throw std::runtime_error("Unknown");
        }

        if (name.empty()) {
          name = className;
        }
      }

      openstudio::measure::OSMeasureInfo info(measureType, className, name, description, taxonomy, modelerDescription, arguments, outputs);
      info.update(measure_.get());
      for (auto& arg : arguments) {
        fmt::print("arg={}\n", arg.displayName());
        // auto outputs = measurePtr->outputs();
      }

      // Save the xml file with changes triggered by checkForUpdatesFiles() / checkForUpdatesXML() above
      measure_->save();
    }

    return measure_;
  }

  void MeasureUpdateOptions::execute(MeasureUpdateOptions const& opt, ScriptEngineInstance& rubyEngine, ScriptEngineInstance& pythonEngine) {
    // opt.debug_print();

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
