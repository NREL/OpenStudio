/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
        // fmt::print("val={}\n", val);
        opt->no_simulation = (val == 1);
      },
      "Only run the OpenStudio and EnergyPlus measures");

    app->add_flag("-p,--postprocess_only", opt->post_process_only, "Only run the reporting measures");

    app->add_flag(
      "--export-epJSON", [opt](std::int64_t val) { (val != 0) && opt->runOptions.setEpjson((val == 1)); },
      "export epJSON file format. The default is IDF");

    app->add_option("-s,--socket", opt->socket_port, "Pipe status messages to a socket on localhost PORT")->option_text("PORT");

    auto* stdout_opt =
      app->add_flag("--show-stdout", opt->show_stdout, "Prints the output of the workflow run in real time to the console, including E+ output")
        ->group("Stdout Options");
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
      if (!workflow.run()) {
        std::exit(1);
      }
    });
  }

}  // namespace cli
}  // namespace openstudio
