/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "InitRubyBindings.hpp"
#include "RubyEval.hpp"

// #define HAVE_ISFINITE 1
#include <ruby.h>
// #undef int128_t
// #undef uint128_t
// #undef isfinite
// #undef memcpy
#include "../../src/utilities/core/Filesystem.hpp"

// You can't use fmt when you include ruby, awesome!
// #include <fmt/format.h>
// <conan_fmt>/include/fmt/core.h:405:7: error: expected unqualified-id
// using int128_t = __int128_t;
//       ^
// <conan_openstudio_ruby>include/ruby-3.2.0/arm64-darwin21/ruby/config.h:202:18: note: expanded from macro 'int128_t'
// #define int128_t __int128

// #include <ranges>
#include <boost/range/adaptor/reversed.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

extern "C"
{
  void Init_openstudioairflow(void);
  void Init_openstudiomodelcore(void);
  void Init_openstudiomodelsimulation(void);
  void Init_openstudioutilitiescore(void);
  void Init_openstudioutilitiesplot(void);
  void Init_openstudioenergyplus(void);
  void Init_openstudioepjson(void);
  void Init_openstudioosversion(void);
  void Init_openstudioutilitiesdata(void);
  void Init_openstudioutilitiessql(void);
  void Init_openstudiogbxml(void);
  void Init_openstudiogltf(void);
  void Init_openstudiomodelgenerators(void);
  void Init_openstudioradiance(void);
  void Init_openstudioutilitiestime(void);
  void Init_openstudioisomodel(void);
  void Init_openstudiomodelgeometry(void);
  void Init_openstudiosdd(void);
  void Init_openstudioutilitiesfiletypes(void);
  void Init_openstudioutilitiesunits(void);
  void Init_openstudiomodelhvac(void);
  void Init_openstudiomodelzonehvac(void);
  void Init_openstudiomodelavailabilitymanager(void);
  void Init_openstudiomodelplantequipmentoperationscheme(void);
  void Init_openstudiomodelstraightcomponent(void);
  void Init_openstudiomodelairflow(void);
  void Init_openstudioutilities(void);
  void Init_openstudioutilitiesgeometry(void);
  void Init_openstudiomeasure(void);
  void Init_openstudiomodelrefrigeration(void);
  void Init_openstudioutilitiesbcl(void);
  void Init_openstudioutilitiesidd(void);
  void Init_openstudiomodel(void);
  void Init_openstudiomodelresources(void);
  void Init_openstudioutilitiesidf(void);
  void Init_openstudioutilitiesxml(void);

  ////void Init_openstudiomodeleditor(void); # happens separately in openstudio.so only, for SketchUp plug-in
}

namespace openstudio {
namespace ruby {

void initBasicRubyBindings() {
  rb_provide("openstudio");
  rb_provide("openstudio.so");

  Init_openstudioutilitiescore();
  rb_provide("openstudioutilitiescore");
  rb_provide("openstudioutilitiescore.so");
  Init_openstudioutilitiestime();
  rb_provide("openstudioutilitiestime");
  rb_provide("openstudioutilitiestime.so");
  Init_openstudioutilitiesdata();
  rb_provide("openstudioutilitiesdata");
  rb_provide("openstudioutilitiesdata.so");
  Init_openstudioutilitiesplot();
  rb_provide("openstudioutilitiesplot");
  rb_provide("openstudioutilitiesplot.so");
  Init_openstudioutilitiesgeometry();
  rb_provide("openstudioutilitiesgeometry");
  rb_provide("openstudioutilitiesgeometry.so");
  Init_openstudioutilitiessql();
  rb_provide("openstudioutilitiessql");
  rb_provide("openstudioutilitiessql.so");
  Init_openstudioutilitiesbcl();
  rb_provide("openstudioutilitiesbcl");
  rb_provide("openstudioutilitiesbcl.so");
  Init_openstudioutilitiesunits();
  rb_provide("openstudioutilitiesunits");
  rb_provide("openstudioutilitiesunits.so");
  Init_openstudioutilitiesidd();
  rb_provide("openstudioutilitiesidd");
  rb_provide("openstudioutilitiesidd.so");
  Init_openstudioutilitiesidf();
  rb_provide("openstudioutilitiesidf");
  rb_provide("openstudioutilitiesidf.so");
  Init_openstudioutilitiesfiletypes();
  rb_provide("openstudioutilitiesfiletypes");
  rb_provide("openstudioutilitiesfiletypes.so");
  Init_openstudioutilitiesxml();
  rb_provide("openstudioutilitiesxml");
  rb_provide("openstudioutilitiesxml.so");
  Init_openstudioutilities();
  rb_provide("openstudioutilities");
  rb_provide("openstudioutilities.so");
}

void initExtendedRubyBindings() {
  Init_openstudiomodel();
  rb_provide("openstudiomodel");
  rb_provide("openstudiomodel.so");
  Init_openstudiomodelcore();
  rb_provide("openstudiomodelcore");
  rb_provide("openstudiomodelcore.so");
  Init_openstudiomodelsimulation();
  rb_provide("openstudiomodelsimulation");
  rb_provide("openstudiomodelsimulation.so");
  Init_openstudiomodelresources();
  rb_provide("openstudiomodelresources");
  rb_provide("openstudiomodelresources.so");
  Init_openstudiomodelgeometry();
  rb_provide("openstudiomodelgeometry");
  rb_provide("openstudiomodelgeometry.so");
  Init_openstudiomodelhvac();
  rb_provide("openstudiomodelhvac");
  rb_provide("openstudiomodelhvac.so");
  Init_openstudiomodelzonehvac();
  rb_provide("openstudiomodelzonehvac");
  rb_provide("openstudiomodelzonehvac.so");
  Init_openstudiomodelavailabilitymanager();
  rb_provide("openstudiomodelavailabilitymanager");
  rb_provide("openstudiomodelavailabilitymanager.so");
  Init_openstudiomodelplantequipmentoperationscheme();
  rb_provide("openstudiomodelplantequipmentoperationscheme");
  rb_provide("openstudiomodelplantequipmentoperationscheme.so");
  Init_openstudiomodelstraightcomponent();
  rb_provide("openstudiomodelstraightcomponent");
  rb_provide("openstudiomodelstraightcomponent.so");
  Init_openstudiomodelairflow();
  rb_provide("openstudiomodelairflow");
  rb_provide("openstudiomodelairflow.so");
  Init_openstudiomodelrefrigeration();
  rb_provide("openstudiomodelrefrigeration");
  rb_provide("openstudiomodelrefrigeration.so");
  Init_openstudiomodelgenerators();
  rb_provide("openstudiomodelgenerators");
  rb_provide("openstudiomodelgenerators.so");

  Init_openstudioenergyplus();
  rb_provide("openstudioenergyplus");
  rb_provide("openstudioenergyplus.so");
  Init_openstudioepjson();
  rb_provide("openstudioepjson");
  rb_provide("openstudioepjson.so");
  Init_openstudioradiance();
  rb_provide("openstudioradiance");
  rb_provide("openstudioradiance.so");
  Init_openstudiogbxml();
  rb_provide("openstudiogbxml");
  rb_provide("openstudiogbxml.so");
  Init_openstudiogltf();
  rb_provide("openstudiogltf");
  rb_provide("openstudiogltf.so");
  Init_openstudioairflow();
  rb_provide("openstudioairflow");
  rb_provide("openstudioairflow.so");
  Init_openstudioosversion();
  rb_provide("openstudioversion");
  rb_provide("openstudioversion.so");
  Init_openstudiomeasure();
  rb_provide("openstudiomeasure");
  rb_provide("openstudiomeasure.so");
  Init_openstudioisomodel();
  rb_provide("openstudioisomodel");
  rb_provide("openstudioisomodel.so");
  Init_openstudiosdd();
  rb_provide("openstudiosdd");
  rb_provide("openstudiosdd.so");

  //Init_openstudiomodeleditor(); # happens separately in openstudio.so only, for SketchUp plug-in
  //rb_provide("openstudiomodeleditor");
  //rb_provide("openstudiomodeleditor.so");

  // "typedefs" for backwards compatibility
  // keep synchronized with \openstudiocore\src\utilities\core\RubyInterpreter.hpp
  std::string ruby_typedef_script = R"ruby(
module OpenStudio
module Ruleset

  # support for name deprecated as of 0.10.1
  class UserScriptArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgument is deprecated, use OpenStudio::Measure::Argument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class OptionalUserScriptArgument < OpenStudio::Measure::OptionalOSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OptionalUserScriptArgument is deprecated, use OpenStudio::Measure::OptionalOSArgument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class UserScript < OpenStudio::Measure::OSMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScript is deprecated, use OpenStudio::Measure::OSMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class ModelUserScript < OpenStudio::Measure::ModelMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ModelUserScript is deprecated, use OpenStudio::Measure::ModelMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class WorkspaceUserScript < OpenStudio::Measure::EnergyPlusMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "WorkspaceUserScript is deprecated, use OpenStudio::Measure::EnergyPlusMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class ReportingUserScript < OpenStudio::Measure::ReportingMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ReportingUserScript is deprecated, use OpenStudio::Measure::ReportingMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class OSArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgument is deprecated, use OpenStudio::Measure::OSArgument instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  def self.convertOSArgumentVectorToMap(argument_vector)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OpenStudio::Ruleset::convertOSArgumentVectorToMap is deprecated, use OpenStudio::Measure::convertOSArgumentVectorToMap instead.")
    return OpenStudio::Measure::convertOSArgumentVectorToMap(argument_vector)
  end

  # support for name deprecated as of 2.0.0
  class OSArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class OSArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end
  end

  # class was replaced by OpenStudio::WorkflowStepResult
#  # support for name deprecated as of 2.0.0
#  class OSResult < OpenStudio::Measure::OSResult
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResult is deprecated, use OpenStudio::Measure::OSResult instead.")
#      super
#    end
#  end
#
#  # support for name deprecated as of 2.0.0
#  class OSResultVector < OpenStudio::Measure::OSResultVector
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResultVector is deprecated, use OpenStudio::Measure::OSResultVector instead.")
#      super
#    end
#  end

  # support for name deprecated as of 2.0.0
  class OSRunner < OpenStudio::Measure::OSRunner
    def initialize(workflow_json = nil)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSRunner is deprecated, use OpenStudio::Measure::OSRunner instead.")
      if workflow_json.nil?
        workflow_json = OpenStudio::WorkflowJSON.new
        OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "No workflow provided, using empty WorkflowJSON.")
      end
      super(workflow_json)
    end
  end

  # support for name deprecated as of 2.0.0
  class RubyUserScriptInfo < OpenStudio::Measure::OSMeasureInfo
    def initialize(*args)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "RubyUserScriptInfo is deprecated, use OpenStudio::Measure::OSMeasureInfo instead.")
      if args.size == 1
        super(args[0])
      elsif args.size == 8
        super(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8])
      end
    end
  end

  # support for name deprecated as of 2.0.0
  def self.infoExtractorRubyFunction
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::infoExtractorRubyFunction instead.")
    return OpenStudio::Measure.infoExtractorRubyFunction
  end

  # support for name deprecated as of 2.0.0
  def self.getInfo(measure, model, workspace)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::getInfo instead.")
    return OpenStudio::Measure.getInfo(measure, model, workspace)
  end

  # support for name deprecated as of 2.0.0
  def self.makeChoiceArgumentOfWorkspaceObjects(name, iddObjectType, workspace, required=true)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::makeChoiceArgumentOfWorkspaceObjects instead.")
    return OpenStudio::Measure.makeChoiceArgumentOfWorkspaceObjects(name, iddObjectType, workspace, required)
  end

end # module Ruleset

module Measure
class RubyMeasureInfoBinding < OpenStudio::Measure::MeasureInfoBinding
  def initialize
    super() # needed
    @info = nil
  end
  def setMeasureInfo(info)
    @info = info
  end
  def measure_info
    @info
  end
  def error
    @info.error.to_s
  end
  def measureType
    @info.measureType.valueName.to_s
  end
  def className
    @info.className.to_s
  end
  def name
    @info.name.to_s
  end
  def description
    @info.description.to_s
  end
  def taxonomy
    @info.taxonomy.to_s
  end
  def modelerDescription
    @info.modelerDescription.to_s
  end
  def arguments
    @info.arguments.map{|a| a.toJSON()}
  end
  def outputs
    @info.outputs.map{|a| a.toJSON()}
  end
  def get_binding
    result = binding()
    return result
  end
  def renderFile(readme_in_path)
    require 'erb'
    begin
      readme_out_path = File.join(File.dirname(readme_in_path), File.basename(readme_in_path, File.extname(readme_in_path)))
      readme_in = File.read(readme_in_path)
      renderer = ERB.new(readme_in)
      b = get_binding()
      readme_out = renderer.result(b)
    rescue => e
      exception_msg = "Failed to Render ERB file: #{e.class}: #{e.message}\nTraceback:\n"
      exception_msg += e.backtrace.join("\n")
      STDERR.puts exception_msg
      # info = OpenStudio::Measure::OSMeasureInfo.new(exception_msg)
      return false # @info
    end
    # write README.md file
    File.open(readme_out_path, 'w') do |file|
      file << readme_out
      # make sure data is written to the disk one way or the other
      begin
        file.fsync
      rescue StandardError
        file.flush
      end
    end
    return true #  @info
  end
end
end # module Measure

end # module OpenStudio

module OpenStudio
  def self.getSharedResourcesPath()
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio", "getSharedResourcesPath is deprecated.")
    return OpenStudio::Path.new()
  end
end # module OpenStudio

)ruby";

  openstudio::evalString(ruby_typedef_script);
}

void initEmbeddedGems() {

  // Note: We instantiate a global $logger here
  // It used to be a ruby Logger (require 'logger'), and we would have needed to set the log level based on the CLI parameters passed
  // Instead, I mimic it by providing a convenience class that forwards to the OpenStudio logger, which takes care of the log level

  std::string initScript = R"ruby(
# This is the save puts to use to catch EPIPE. Uses `puts` on the given IO object and safely ignores any Errno::EPIPE
#
# @param [String] message Message to output
# @param [Hash] opts Options hash
#
def safe_puts(message=nil, opts=nil)
  message ||= ''
  opts = {
      io: $stdout,
      printer: :puts
  }.merge(opts || {})

  begin
    opts[:io].send(opts[:printer], message)
  rescue Errno::EPIPE
    # This is what makes this a `safe` puts
    return
  end
end

require 'rbconfig'

# require 'logger'
# $logger = Logger.new(STDOUT)
# $logger.level = Logger::DEBUG #WARN
# Mimic ruby Logger, but forward to OpenStudio, so we don't have to worry about setting the LogLevel
class ConvenienceLogger

  attr_accessor :level

  def initialize(log_level)
    @level = log_level
  end

  def trace?
    @level <= OpenStudio::Trace
  end

  def debug(msg)
    OpenStudio::logFree(OpenStudio::Debug, "ruby", msg)
  end

  def info(msg)
    OpenStudio::logFree(OpenStudio::Info, "ruby", msg)
  end

  def warn(msg)
    OpenStudio::logFree(OpenStudio::Warn, "ruby", msg)
  end

  def error(msg)
    OpenStudio::logFree(OpenStudio::Error, "ruby", msg)
  end

  def fatal(msg)
    OpenStudio::logFree(OpenStudio::Fatal, "ruby", msg)
  end

end
$logger = ConvenienceLogger.new(OpenStudio::Logger.instance.standardOutLogger.logLevel.get)

# debug Gem::Resolver, must go before resolver is required
if $logger.trace?
  ENV['DEBUG_RESOLVER'] = "1"
end

original_arch = nil
if RbConfig::CONFIG['arch'] =~ /x64-mswin64/
  # assume that system ruby of 'x64-mingw32' architecture was used to create bundle
  original_arch = RbConfig::CONFIG['arch']
  RbConfig::CONFIG['arch'] = 'x64-mingw32'
end

# load embedded ruby gems
require 'rubygems'
require 'rubygems/version'
Gem::Platform.local

if original_arch
  RbConfig::CONFIG['arch'] = original_arch
end
  )ruby";

  openstudio::evalString(initScript);

  const std::string irbPatch = R"ruby(if $logger.trace?
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
    os_version = "3.7.0" # OpenStudio::openStudioVersion
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
end)ruby";

  openstudio::evalString(irbPatch);
}

void setupEmbeddedGemsClearEnvVars() {

  // in hash.c there's lot of env-related functions, but all private, aside from rb_env_clear();
  const std::string clearEnvs = R"ruby(
ENV.delete('GEM_HOME') if ENV['GEM_HOME']
ENV.delete('GEM_PATH') if ENV['GEM_PATH']
ENV.delete('BUNDLE_GEMFILE') if ENV['BUNDLE_GEMFILE']
ENV.delete('BUNDLE_PATH') if ENV['BUNDLE_PATH']
ENV.delete('BUNDLE_WITHOUT') if ENV['BUNDLE_WITHOUT']
  )ruby";

  openstudio::evalString(clearEnvs);
};

void addIncludeDirsToLoadPaths(const std::vector<openstudio::path>& includeDirs) {
  if (includeDirs.empty()) {
    return;
  }

  VALUE load_path = rb_gv_get("$LOAD_PATH");

  // Apparently this isn't exposed: `ary_resize_capa(rb_array_len(load_path) + includeDirs.size())``

  // Can't use ranges! starts at clang 15.0, damn.
  // for (const auto& dirPath : includeDirs | std::views::reverse) {
  for (const auto& dirPath : includeDirs | boost::adaptors::reversed) {
    // Insert at 0, in reverse order. Same a includeDirs.concat($LOAD_PATH) in ruby
    rb_ary_unshift(load_path, rb_str_new2(dirPath.generic_string().c_str()));
  }
}

void setRubyEnvVarStr(std::string_view name, std::string_view val) {

  if (val.empty()) {
    return;
  }

  //   const auto envVarCmd = fmt::format(
  //     R"ruby(
  // $logger.info "Setting {0} to {1}"
  // ENV['{0}'] = '{1}'
  // )ruby",
  //     name, val);

  std::stringstream ss;
  ss << "$logger.info \"Setting " << name << " to " << val << "\"\n"
     << "ENV['" << name << "'] = '" << val << "'\n";
  openstudio::evalString(ss.str());
}

void setRubyEnvVarPath(std::string_view name, const openstudio::path& p) {

  if (p.empty()) {
    return;
  }

  setRubyEnvVarStr(name, p.generic_string());
}

void setGemPathDir(const std::vector<openstudio::path>& gemPathDirs) {
  if (gemPathDirs.empty()) {
    return;
  }

  std::string result;
  for (const auto& gemDirPath : gemPathDirs) {
    const auto& s = gemDirPath.generic_string();
    result += result.empty() ? s : std::string{openstudio::path::preferred_separator} + s;
  }
  setRubyEnvVarStr("GEM_PATH", result);
}

void locateEmbeddedGems(bool use_bundler) {
  // It is important to find, eval, and call add_spec for all of the gemspecs
  // that are in the embedded file system. The rubgems find/load mechanims will
  // attempt to use a number of Ruby file, directory, and path operations that
  // are not compaitable with the embdded file system. By doing the find, eval,
  // add_spec routine up front, rubygems will avoid trying to load things off of
  // the embedded file system itself.
  //
  // A previous version of this routine added embedded file paths to the gem
  // path, such as Gem.paths.path << ':/ruby/3.2.0/gems/'. This is not
  // sustainable because it requires shims for many of the Ruby file operations
  // so that they work with embedded files. The solution here avoids File
  // operations on embdded file system with the exception of our own
  // EmbeddedScripting::getFileAsString

  std::string initCmd = R"ruby(
  Gem::Deprecate.skip = true

  # find all the embedded gems
  original_embedded_gems = {}
  begin
    EmbeddedScripting::fileNames.each do |f|
      if md = /specifications\/.*\.gemspec$/.match(f) ||
         md = /bundler\/.*\.gemspec$/.match(f)
        begin
          spec = EmbeddedScripting::getFileAsString(f)
          s = eval(spec)

          # These require io-console, which we don't have on Windows
          if Gem.win_platform?
              next if s.name == 'reline'
              next if s.name == 'debug'
              next if s.name == 'irb'
              next if s.name == 'readline'
          end

          s.loaded_from = f
          # This is shenanigans because otherwise rubygems will think extensions are missing
          # But we are initing them manually so they are not missing
          # Here the missing_extensions? method is redefined for only this instance "s"
          class << s
            define_method(:missing_extensions?) { false }
          end
          original_embedded_gems[s.name] = s

          init_count = 0
          Gem::Specification.each {|x| init_count += 1}

          # if already have an equivalent spec this will be a no-op
          Gem::Specification.add_spec(s)

          post_count = 0
          Gem::Specification.each {|x| post_count += 1}

          if post_count == init_count
            $logger.debug "Found system gem #{s.name} #{s.version}, overrides embedded gem"
          end

        rescue LoadError => e
          safe_puts e.message
        rescue => e
          safe_puts e.message
        end
      end
    end
  rescue NameError => e
    # EmbeddedScripting not available
  end

  original_load_path = $LOAD_PATH.clone
  embedded_gems_to_activate = []

  # Identify the embedded gems (don't activate them yet)
  current_dir = Dir.pwd
  begin
    # get a list of all the embedded gems
    dependencies = []
    original_embedded_gems.each_value do |spec|
      $logger.debug "Adding dependency on #{spec.name} '~> #{spec.version}'"
      dependencies << Gem::Dependency.new(spec.name, "~> #{spec.version}")
    end

    # resolve dependencies
    activation_errors = false
    resolver = Gem::Resolver.for_current_gems(dependencies)
    activation_requests = resolver.resolve
    $logger.debug "Processing #{activation_requests.size} activation requests"
    activation_requests.each do |request|
      do_activate = true
      spec = request.spec

      # check if this is one of our embedded gems
      if original_embedded_gems[spec.name]
        # check if gem can be loaded from RUBYLIB, this supports developer use case
        original_load_path.each do |lp|
          if File.exist?(File.join(lp, spec.name)) || File.exist?(File.join(lp, spec.name + '.rb')) || File.exist?(File.join(lp, spec.name + '.so'))
            $logger.debug "Found #{spec.name} in '#{lp}', overrides gem #{spec.spec_file}"
            do_activate = false
            break
          end
        end
      end
      if do_activate
        embedded_gems_to_activate << spec
      end

    end

    if activation_errors
      return false
    end
  ensure
    Dir.chdir(current_dir)
  end
)ruby";

  if (use_bundler) {
    initCmd += R"ruby(
  # Load the bundle before activating any embedded gems
  embedded_gems_to_activate.each do |spec|
    if spec.name == "bundler"
      $logger.debug "Activating Bundler gem #{spec.spec_file}"
      if $logger.trace?
        pp spec
      end

      begin
        # Activate will manipulate the $LOAD_PATH to include the gem
        spec.activate
      rescue Gem::LoadError => e
        # There may be conflicts between the bundle and the embedded gems,
        # those will be logged here
        exception_msg = "Error activating gem #{spec.spec_file}: #{e.class}: #{e.message}\nTraceback:\n"
        exception_msg += e.backtrace.join("\n")
        STDERR.puts exception_msg
        $logger.error "Error activating gem #{spec.spec_file}"
        activation_errors = true
      end
    end
  end

  current_dir = Dir.pwd

  original_arch = nil
  if RbConfig::CONFIG['arch'] =~ /x64-mswin64/
    # assume that system ruby of 'x64-mingw32' architecture was used to create bundle
    original_arch = RbConfig::CONFIG['arch']
    $logger.info "Temporarily replacing arch '#{original_arch}' with 'x64-mingw32' for Bundle"
    RbConfig::CONFIG['arch'] = 'x64-mingw32'
  end

    # require bundler
    # have to do some forward declaration and pre-require to get around autoload cycles
    require 'bundler/errors'
    #require 'bundler/environment_preserver'
    require 'bundler/plugin'
    #require 'bundler/rubygems_ext'
    require 'bundler/rubygems_integration'
    require 'bundler/version'
    require 'bundler/ruby_version'
    #require 'bundler/constants'
    #require 'bundler/current_ruby'
    require 'bundler/gem_helpers'
    #require 'bundler/plugin'
    require 'bundler/source'
    require 'bundler/definition'
    require 'bundler/dsl'
    require 'bundler/uri_credentials_filter'
    require 'bundler/uri_normalizer'
    require 'bundler/index'
    require 'bundler/digest'
    require 'bundler/source/git'
    require 'bundler/source/git/git_proxy'
    require 'bundler/match_remote_metadata'
    require 'bundler/remote_specification'
    require 'bundler/stub_specification'
    require 'bundler'

  begin
    # activate bundled gems
    # bundler will look in:
    # 1) ENV["BUNDLE_GEMFILE"]
    # 2) find_file("Gemfile", "gems.rb")
    #require 'bundler/setup'

    if $logger.trace?
      Bundler.ui.level = "debug"
    end

    groups = Bundler.definition.groups
    keep_groups = []
    without_groups = ENV['BUNDLE_WITHOUT']
    $logger.info "without_groups = #{without_groups}"
    groups.each do |g|
      $logger.info "g = #{g}"
      if without_groups.include?(g.to_s)
        $logger.info "Bundling without group '#{g}'"
      else
        keep_groups << g
      end
    end

    $logger.info "Bundling with groups [#{keep_groups.join(',')}]"

    remaining_specs = []
    Bundler.definition.specs_for(keep_groups).each {|s| remaining_specs << s.name}

    $logger.info "Specs to be included [#{remaining_specs.join(',')}]"


    Bundler.setup(*keep_groups)
  ensure

    if original_arch
      $logger.info "Restoring arch '#{original_arch}'"
      RbConfig::CONFIG['arch'] = original_arch
    end

      Dir.chdir(current_dir)
    end
  )ruby";
  }

  initCmd += R"ruby(

  original_load_path = $LOAD_PATH.clone

  embedded_gems_to_activate.each do |spec|
    $logger.debug "Activating gem #{spec.spec_file}"
    begin
      # Activate will manipulate the $LOAD_PATH to include the gem
      spec.activate
    rescue Gem::LoadError
      # There may be conflicts between the bundle and the embedded gems,
      # those will be logged here
      $logger.error "Error activating gem #{spec.spec_file}"
      activation_errors = true
    end
  end

  # Get all of the embedded gem paths which were added by activating the embedded gems
  # This is used by embedded_help::require
  $EMBEDDED_GEM_PATH = $LOAD_PATH - original_load_path
  # Make sure no non embedded paths snuck in
  $EMBEDDED_GEM_PATH = $EMBEDDED_GEM_PATH.select {|p| p.to_s.chars.first == ':'}
  # Restore LOAD_PATH
  $LOAD_PATH.reject! {|p| not original_load_path.any? p}
  )ruby";

  openstudio::evalString(initCmd);
}

void setupEmbeddedGems(const std::vector<openstudio::path>& includeDirs, const std::vector<openstudio::path>& gemPathDirs,
                       const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath, const openstudio::path& bundleGemDirPath,
                       const std::string& bundleWithoutGroups) {

  initEmbeddedGems();
  setupEmbeddedGemsClearEnvVars();

  addIncludeDirsToLoadPaths(includeDirs);
  setGemPathDir(gemPathDirs);
  setRubyEnvVarPath("GEM_HOME", gemHomeDir);
  bool use_bundler = false;
  if (!bundleGemFilePath.empty()) {
    setRubyEnvVarPath("BUNDLE_GEMFILE", bundleGemFilePath);
    use_bundler = true;
  }
  if (!bundleGemDirPath.empty()) {
    setRubyEnvVarPath("BUNDLE_PATH", bundleGemDirPath);
  } else if (use_bundler) {
    // bundle was requested but bundle_path was not provided
    std::cout << "Warn: Bundle activated but ENV['BUNDLE_PATH'] is not set" << '\n' << "Info: Setting BUNDLE_PATH to ':/ruby/3.2.0/'" << std::endl;
  }

  if (!bundleWithoutGroups.empty()) {
    setRubyEnvVarPath("BUNDLE_WITHOUT", bundleWithoutGroups);
  } else if (use_bundler) {
    // bundle was requested but bundle_path was not provided
    std::cout << "Warn: Bundle activated but ENV['BUNDLE_WITHOUT'] is not set" << '\n' << "Info: Setting BUNDLE_WITHOUT to 'test'" << std::endl;
    setRubyEnvVarPath("BUNDLE_WITHOUT", "test");
  }

  locateEmbeddedGems(use_bundler);
}

void initRubyBindings() {
  initBasicRubyBindings();
  initExtendedRubyBindings();
}

}  // namespace ruby
}  // namespace openstudio
