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

#include "RunCommand.hpp"
#include "../utilities/filetypes/ForwardTranslatorOptions.hpp"
#include "../workflow/WorkflowRunOptions.hpp"

#include "../workflow/OSWorkflow.hpp"
#include "../scriptengine/ScriptEngine.hpp"

#include <cstdint>
#include <fmt/format.h>

#include <memory>

namespace openstudio {
namespace cli {

  void setupRunOptions(CLI::App* parentApp, ScriptEngineInstance& ruby, ScriptEngineInstance& python) {
    /// Set up a subcommand and capture a shared_ptr to a struct that holds all its options.
    /// The variables of the struct are bound to the CLI options.
    /// We use a shared ptr so that the addresses of the variables remain for binding,
    /// You could return the shared pointer if you wanted to access the values in main.

    // TODO: modify utilities/filetypes/RunOptions.hpp and use that instead
    auto opt = std::make_shared<WorkflowRunOptions>();

    auto* const app = parentApp->add_subcommand("run", "Executes an OpenStudio Workflow file");

    app->add_option("-w,--workflow", opt->osw_path, "Specify the FILE path to the workflow to run")->option_text("FILE");

    app->add_flag(
      "-m,--measures_only",
      [opt](std::int64_t val) {
        fmt::print("val={}\n", val);
        opt->no_simulation = (val == 1);
      },
      "Only run the OpenStudio and EnergyPlus measures");

    app->add_flag("-p,--postprocess_only", opt->post_process_only, "Only run the reporting measures");

    app->add_flag(
      "--export-epJSON", [opt](std::int64_t val) { (val != 0) && opt->runOptions.setEpjson((val == 1)); },
      "export epJSON file format. The default is IDF");

    app->add_option("-s,--socket", opt->socket_port, "Pipe status messages to a socket on localhost PORT")->option_text("PORT");

    auto* stdout_opt = app->add_flag("--show-stdout", opt->show_stdout, "export epJSON file format. The default is IDF")->group("Stdout Options");
    app->add_flag("--add-timings", opt->add_timings, "Print the start, end and elapsed times of each state of the simulation.")
      ->needs(stdout_opt)
      ->group("Stdout Options");
    app
      ->add_flag("--style-stdout", opt->style_stdout,
                 "Style the stdout output to more clearly show the start and end of each state of the simulation.")
      ->needs(stdout_opt)
      ->group("Stdout Options");

    app->add_flag(
      "--debug", [opt](std::int64_t val) { (val != 0) && opt->runOptions.setDebug((val == 1)); },
      "Includes additional outputs for debugging failing workflows and does not clean up the run directory");

    // FT options
    static constexpr auto ftGroupName = "Forward Translator Options";
    app
      ->add_flag(
        "--runcontrolspecialdays,!--no-runcontrolspecialdays",
        [opt](std::int64_t val) {
          if (val != 0) {
            opt->runOptions.forwardTranslatorOptions().setKeepRunControlSpecialDays((val == 1));
          }
        },
        "Include RunControlSpecialDays (Holidays) [Default: True]")
      ->group(ftGroupName);

    app
      ->add_flag(
        "--set-ip-tabular-output",
        [opt](std::int64_t val) {
          if (val != 0) {
            opt->runOptions.forwardTranslatorOptions().setIPTabularOutput((val == 1));
          }
        },
        "Request IP units from E+ Tabular (HTML) Report [Default: False]")
      ->group(ftGroupName);

    app
      ->add_flag(
        "--lifecyclecosts,!--no-lifecyclecosts",
        [opt](std::int64_t val) {
          if (val != 0) {
            opt->runOptions.forwardTranslatorOptions().setExcludeLCCObjects((val != 1));
          }
        },
        "Include LifeCycleCosts [Default: True]")
      ->group(ftGroupName);

    app
      ->add_flag(
        "--sqlite-output,!--no-sqlite-output",
        [opt](std::int64_t val) {
          if (val != 0) {
            opt->runOptions.forwardTranslatorOptions().setExcludeSQliteOutputReport((val != 1));
          }
        },
        "Request Output:SQLite from E+ [Default: True]")
      ->group(ftGroupName);

    app
      ->add_flag(
        "--html-output,!--no-html-output",
        [opt](std::int64_t val) {
          if (val != 0) {
            opt->runOptions.forwardTranslatorOptions().setExcludeHTMLOutputReport((val != 1));
          }
        },
        "Request Output:Table:SummaryReports from E+ [Default: True]")
      ->group(ftGroupName);

    app
      ->add_flag(
        "--space-translation,!--no-space-translation",
        [opt](std::int64_t val) {
          if (val != 0) {
            opt->runOptions.forwardTranslatorOptions().setExcludeSpaceTranslation((val != 1));
          }
        },
        "Add individual E+ Space [Default: True]")
      ->group(ftGroupName);

    // Subcommand callback
    app->callback([opt, &ruby, &python] {
      openstudio::OSWorkflow workflow(*opt, ruby, python);
      workflow.run();
    });
  }

}  // namespace cli
}  // namespace openstudio
