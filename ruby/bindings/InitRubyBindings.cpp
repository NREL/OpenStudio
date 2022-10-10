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
// <conan_openstudio_ruby>include/ruby-2.7.0/arm64-darwin21/ruby/config.h:202:18: note: expanded from macro 'int128_t'
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

require 'logger'
require 'rbconfig'

$logger = Logger.new(STDOUT)
#$logger.level = Logger::ERROR
$logger.level = Logger::WARN

# debug Gem::Resolver, must go before resolver is required
#ENV['DEBUG_RESOLVER'] = "1"
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

module Gem
class Specification < BasicSpecification

  # This isn't ideal but there really is no available method to add specs for our use case.
  # Using self.dirs=() works for ruby official gems but since it appends the dir paths with 'specifications' it breaks for bundled gem specs
  def self.add_spec spec
    warn "Gem::Specification.add_spec is deprecated and will be removed in RubyGems 3.0" unless Gem::Deprecate.skip
    # TODO: find all extraneous adds
    # puts
    # p :add_spec => [spec.full_name, caller.reject { |s| s =~ /minitest/ }]

    # TODO: flush the rest of the crap from the tests
    # raise "no dupes #{spec.full_name} in #{all_names.inspect}" if
    #   _all.include? spec

    raise "nil spec!" unless spec # TODO: remove once we're happy with tests

    return if _all.include? spec

    _all << spec
    stubs << spec
    (@@stubs_by_name[spec.name] ||= []) << spec
    sort_by!(@@stubs_by_name[spec.name]) { |s| s.version }
    _resort!(_all)
    _resort!(stubs)
  end

  def gem_dir
    embedded = false
    tmp_loaded_from = loaded_from.clone
    if tmp_loaded_from.chars.first == ':'
      tmp_loaded_from[0] = ''
      embedded = true
    end

    joined = File.join(gems_dir, full_name)
    if embedded
      test = /bundler\/gems/.match(tmp_loaded_from)
      if test
        @gem_dir = ':' + (File.dirname tmp_loaded_from)
      else
        @gem_dir = joined
      end
    else
      @gem_dir = File.expand_path joined
    end
  end

  def full_gem_path
    # TODO: This is a heavily used method by gems, so we'll need
    # to aleast just alias it to #gem_dir rather than remove it.
    embedded = false
    tmp_loaded_from = loaded_from.clone
    if tmp_loaded_from.chars.first == ':'
      tmp_loaded_from[0] = ''
      embedded = true
    end

    joined = File.join(gems_dir, full_name)
    if embedded
      test = /bundler\/gems/.match(tmp_loaded_from)
      if test
        @full_gem_path = ':' + (File.dirname tmp_loaded_from)
        @full_gem_path.untaint
        return @full_gem_path
      else
        @full_gem_path = joined
        @full_gem_path.untaint
        return @full_gem_path
      end
    else
      @full_gem_path = File.expand_path joined
      @full_gem_path.untaint
    end
    return @full_gem_path if File.directory? @full_gem_path

    @full_gem_path = File.expand_path File.join(gems_dir, original_name)
  end

  def gems_dir
    # TODO: this logic seems terribly broken, but tests fail if just base_dir
    @gems_dir = File.join(loaded_from && base_dir || Gem.dir, "gems")
  end

  def base_dir
    return Gem.dir unless loaded_from

    embedded = false
    tmp_loaded_from = loaded_from.clone
    if tmp_loaded_from.chars.first == ':'
      tmp_loaded_from[0] = ''
      embedded = true
    end

    test = /bundler\/gems/.match(tmp_loaded_from)
    result = if (default_gem? || test) then
        File.dirname File.dirname File.dirname tmp_loaded_from
      else
        File.dirname File.dirname tmp_loaded_from
      end

    if embedded
      result = ':' + result
    end
    @base_dir = result
  end

end
end

# have to do some forward declaration and pre-require to get around autoload cycles
#module Bundler
#end

# This is the code chunk to allow for an embedded IRB shell. From Jason Roelofs, found on StackOverflow
module IRB # :nodoc:
  def self.start_session(binding)
    unless @__initialized
      args = ARGV
      ARGV.replace(ARGV.dup)
      IRB.setup(nil)
      ARGV.replace(args)
      @__initialized = true
    end

    workspace = WorkSpace.new(binding)

    irb = Irb.new(workspace)

    @CONF[:IRB_RC].call(irb.context) if @CONF[:IRB_RC]
    @CONF[:MAIN_CONTEXT] = irb.context

    catch(:IRB_EXIT) do
      irb.eval_input
    end
  end
end
  )ruby";

  openstudio::evalString(initScript);
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
    <<  "ENV['" << name << "'] = '" << val << "'\n";
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

  std::string initCmd = R"ruby(

  Gem.paths.path << ':/ruby/2.7.0/gems/'
  Gem.paths.path << ':/ruby/2.7.0/bundler/gems/'
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
          if File.exists?(File.join(lp, spec.name)) || File.exists?(File.join(lp, spec.name + '.rb')) || File.exists?(File.join(lp, spec.name + '.so'))
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
  if true # use_bundler

    embedded_gems_to_activate.each do |spec|
      if spec.name == "bundler"
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
    require 'bundler'

    begin
      # activate bundled gems
      # bundler will look in:
      # 1) ENV["BUNDLE_GEMFILE"]
      # 2) find_file("Gemfile", "gems.rb")
      #require 'bundler/setup'

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
  end)ruby";
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
    std::cout << "Warn: Bundle activated but ENV['BUNDLE_PATH'] is not set" << '\n' << "Info: Setting BUNDLE_PATH to ':/ruby/2.7.0/'" << std::endl;
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
