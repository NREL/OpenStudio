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
  0
rescue SystemExit => e
  # puts "System Exit: #{{e.status}}"
  if !e.success?
    STDERR.puts
    STDERR.puts "#{{e.class}}: #{{e.message}} with return code #{{e.status}} "
    STDERR.puts "Backtrace:\n\t" + e.backtrace.join("\n\t")
  end
  e.status
rescue Exception => e
  STDERR.puts
  STDERR.puts "#{{e.class}}: #{{e.message}}"
  STDERR.puts "Backtrace:\n\t" + e.backtrace.join("\n\t")
  raise
end
     )",
                       rubyScriptPath.generic_string());
    try {
      auto ret_so = rubyEngine->eval(cmd);
      auto ret_code = rubyEngine->getAs<int>(ret_so);
      // If ret_code is already none zero, just exit faster
      // Otherwise let it be, so that the at_exit(s) can run in particular (for minitest for eg)
      if (ret_code != 0) {
        exit(ret_code);
      }
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
module_name = '__main__'
spec = importlib.util.spec_from_file_location(module_name, r'{}')
module = importlib.util.module_from_spec(spec)
sys.modules[module_name] = module
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

  void executeRubyRepl(ScriptEngineInstance& rubyEngine) {

    rubyEngine->exec("ARGV.clear");

    const std::string cmd = R"ruby(
require 'irb'
require 'irb/lc/error'

# ENV["IRB_LANG"] = "C"
# puts "Reline encoding_system_needs: #{Reline.encoding_system_needs.name}"
# puts "Reline::IOGate.encoding: #{Reline::IOGate.encoding}"

# Workaround issue in line_editor.rb where it fails to read a full block
# unicode character. Probably because of the way we embbed it in C++
# The full block is \xe2\x96\x88
ENV['RELINE_ALT_SCROLLBAR'] = "1"

class Reline::LineEditor
  def reset(prompt = '', encoding:)
    @rest_height = (Reline::IOGate.get_screen_size.first - 1) - Reline::IOGate.cursor_pos.y
    @screen_size = Reline::IOGate.get_screen_size
    @screen_height = @screen_size.first
    reset_variables(prompt, encoding: encoding)
    Reline::IOGate.set_winch_handler do
      @resized = true
    end
    if ENV.key?('RELINE_ALT_SCROLLBAR')
      @full_block = '::'
      @upper_half_block = "''"
      @lower_half_block = '..'
      @block_elem_width = 2
    elsif Reline::IOGate.win?
      @full_block = '█'
      @upper_half_block = '▀'
      @lower_half_block = '▄'
      @block_elem_width = 1
    elsif @encoding == Encoding::UTF_8
      @full_block = '█'
      @upper_half_block = '▀'
      @lower_half_block = '▄'
      @block_elem_width = Reline::Unicode.calculate_width('█')
    else
      @full_block = '::'
      @upper_half_block = "''"
      @lower_half_block = '..'
      @block_elem_width = 2
    end
  end
end

module IRB # :nodoc:
  class Locale

    alias :original_load :load
    alias :original_require :require

    def require(file, priv = nil)
      # puts "require: #{file}"
      original_require(file, priv)
    end

    # Some shenanigans being done to detect localized files, and that relies ton File.readable? and co...
    def load(file, priv=nil)
      # puts "file=#{file}"
      if file == 'irb/error.rb'
        $".push file
        @@loaded << 'irb/lc/error.rb'
        return
      end
      original_load(file, priv)
    end

  end


  def self.start_session(binding)
    unless @__initialized
      args = ARGV
      ARGV.replace(ARGV.dup)
      IRB.setup(nil)
      # locale = @CONF[:LC_MESSAGES]
      # p locale

      ARGV.replace(args)
      @__initialized = true
    end

    @CONF[:IRB_RC].call(irb.context) if @CONF[:IRB_RC]
    # puts @CONF[:PROMPT_MODE]
    os_version = "3.8.0" # OpenStudio::openStudioVersion
    @CONF[:PROMPT][:OPENSTUDIO] = {
      :PROMPT_I=>"(os #{os_version}) :%03n > ",
      :PROMPT_S=>"(os #{os_version}) :%03n%l> ",
      :PROMPT_C=>"(os #{os_version}) :%03n > ",
      :PROMPT_N=>"(os #{os_version}) :%03n?> ",
      :RETURN=>" => %s \n",
      :AUTO_INDENT=>true
    }
    @CONF[:PROMPT_MODE] = :OPENSTUDIO
    workspace = WorkSpace.new(binding)
    input_method = IRB::RelineInputMethod.new
    irb = Irb.new(workspace, input_method)


    @CONF[:MAIN_CONTEXT] = irb.context

    catch(:IRB_EXIT) do
      irb.eval_input
    end
  end
end

IRB.start_session(binding)
    )ruby";

    rubyEngine->exec(cmd);
  }

  void executePythonRepl(ScriptEngineInstance& pythonEngine) {
    const std::string cmd = R"python(
import readline # optional, will allow Up/Down/History in the console
import code
variables = globals().copy()
variables.update(locals())
shell = code.InteractiveConsole(variables)
shell.interact()
    )python";
    pythonEngine->exec(cmd);
  }

  void executePipListCommand(ScriptEngineInstance& pythonEngine) {
    // TODO: seems like the CLI is picking up your virtualenvironment, so we manipulate sys.path to keep only the E+ folder for now
    const std::string cmd = R"python(
import sys
sys.path = [x for x in sys.path if "EnergyPlus" in x]
import importlib.metadata
mods = sorted([f"{x.name}=={x.version}" for x in importlib.metadata.distributions()])
[print(x) for x in mods]
    )python";
    pythonEngine->exec(cmd);
  }

}  // namespace cli
}  // namespace openstudio
