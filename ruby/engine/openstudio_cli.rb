########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

#File.open('E:\test\test.log', 'w') do |f|
#  ENV.each_key {|k| f.puts "#{k} = #{ENV[k]}" }
#end

#Signal.trap('INT') { abort }

require 'logger'
require 'optparse'
require 'stringio'
require 'rbconfig'

#include OpenStudio::Workflow::Util::IO

$argv = ARGV.dup

$logger = Logger.new(STDOUT)
#$logger.level = Logger::ERROR
$logger.level = Logger::WARN
#$logger.level = Logger::DEBUG

#OpenStudio::Logger.instance.standardOutLogger.disable
#OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Warn)
OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Error)

OpenStudio.autoload(:Airflow, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:EnergyPlus, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:EPJSON, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:GbXML, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:Gltf, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:ISOModel, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:Measure, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:Ruleset, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:Model, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:OSVersion, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:Radiance, ":/openstudio_init_extended.rb")
OpenStudio.autoload(:SDD, ":/openstudio_init_extended.rb")

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
    @@stubs_by_name[spec.name].sort_by! { |s| s.version }
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
        # @full_gem_path.untaint
        return @full_gem_path
      else
        @full_gem_path = joined
        # @full_gem_path.untaint
        return @full_gem_path
      end
    else
      @full_gem_path = File.expand_path joined
      # @full_gem_path.untaint
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

# This is a convenience method that properly handles duping the originally argv array so that it is not destroyed. This
# method will also automatically detect "-h" and "--help" and print help. And if any invalid options are  detected, the
# help will be printed, as well
#
# @param [Object, nil] opts An instance of OptionParse to parse against, defaults to a new OptionParse instance
# @param [Array, nil] argv The argv input to be parsed, defaults to $argv
# @return[Array, nil] If this method returns `nil`, then you should assume that help was printed and parsing failed
#
def parse_options(opts=nil, argv=nil)
  # Creating a shallow copy of the arguments so the OptionParser
  # doesn't destroy the originals.
  argv ||= $argv.dup

  # Default opts to a blank optionparser if none is given
  opts ||= OptionParser.new

  # Add the help option, which must be on every command.
  opts.on_tail('-h', '--help', 'Print this help') do
    safe_puts(opts.help)
    return nil
  end

  opts.parse!(argv)
  return argv
rescue OptionParser::InvalidOption, OptionParser::MissingArgument
  raise "Error: Invalid CLI option, #{opts.help.chomp}"
end

# Method to clean the args. Windows cmd.exe treats single quotes as regular chars so we strip them if found.
def clean_argv(argv)
  argv.each_index do |i|
    # puts "Argument #{i}=<#{argv[i]}>"
    if argv[i].start_with?("'") and argv[i].end_with?("'")
      argv[i] = argv[i][1..-2]
      # puts " Cleaned #{i}=<#{argv[i]}>"
    end
  end
  return argv
end

# This method will split the argv given into three parts: the flags to this command, the command, and the flags to
# the command. For example:
#     -v status -h -v
# The above would yield 3 parts:
#     ["-v"]
#     "status"
#     ["-h", "-v"]
# These parts are useful because the first is a list of arguments given to the current command, the second is a
# command, and the third are the commands given to the command
#
# @param [Array] argv The input to be split
# @param [Array] command_list Hash of commands to look for
# @return [Array] The split command as [main arguments, sub command, sub command arguments]
#
def split_main_and_subcommand(argv, command_list)
  # Initialize return variables
  main_args   = nil
  sub_command = nil
  sub_args    = []

  commands = []
  command_list.keys.each {|k| commands << k.to_s}

  # We split the arguments into two: One set containing any flags before a word, and then the rest. The rest are what
  # get actually sent on to the command
  argv.each_index do |i|
    if commands.index(argv[i])
      main_args   = argv[0, i]
      sub_command = argv[i]
      sub_args    = argv[i+1..-1]
      break
    elsif argv[i].end_with?('.rb')
      main_args   = argv[0, i]
      sub_command = 'execute_ruby_script'
      sub_args    = argv[i..-1]
      break
    end
  end

  # Handle the case that argv was empty or didn't contain any command
  main_args = argv.dup if main_args.nil?

  [main_args, sub_command, sub_args]
end

# parse the main args, those that come before the sub command
def parse_main_args(main_args)

  # Unset RUBY ENVs if previously set (e.g. rvm sets these in shell)

  ENV.delete('GEM_HOME') if ENV['GEM_HOME']
  ENV.delete('GEM_PATH') if ENV['GEM_PATH']
  ENV.delete('BUNDLE_GEMFILE') if ENV['BUNDLE_GEMFILE']
  ENV.delete('BUNDLE_PATH') if ENV['BUNDLE_PATH']
  ENV.delete('BUNDLE_WITHOUT') if ENV['BUNDLE_WITHOUT']

  $logger.debug "Parsing main_args #{main_args}"

  # verbose already handled
  main_args.delete('--verbose')

  # Operate on the include option to add to $LOAD_PATH
  remove_indices = []
  new_path = []
  main_args.each_index do |i|

    if main_args[i] == '-I' || main_args[i] == '--include'
      # remove from further processing
      remove_indices << i
      remove_indices << i+1

      dir = main_args[i + 1]

      if dir.nil?
        $logger.error "#{main_args[i]} requires second argument DIR"
        return false
      elsif !File.exist?(dir) || !File.directory?(dir)
        # DLM: Ruby doesn't warn for this
        #$logger.warn "'#{dir}' passed to #{main_args[i]} is not a directory"
      end
      new_path << dir
    elsif main_args[i] == '--no-ssl'
      $logger.warn "'--no-ssl' flag is deprecated"
    end
  end

  remove_indices.reverse_each {|i| main_args.delete_at(i)}

  if !new_path.empty?

    new_path = new_path.concat($LOAD_PATH)

    $logger.info "Setting $LOAD_PATH to #{new_path}"
    $LOAD_PATH.clear

    new_path.each {|p| $LOAD_PATH << p}
  end

  # Operate on the gem_path option to set GEM_PATH
  remove_indices = []
  new_path = []
  main_args.each_index do |i|

    if main_args[i] == '--gem_path'

      # remove from further processing
      remove_indices << i
      remove_indices << i+1

      dir = main_args[i + 1]

      if dir.nil?
        $logger.error "#{main_args[i]} requires second argument DIR"
        return false
      elsif !File.exist?(dir) || !File.directory?(dir)
        # DLM: Ruby doesn't warn for this
        #$logger.warn "'#{dir}' passed to #{main_args[i]} is not a directory"
      end
      new_path << dir
    end
  end
  remove_indices.reverse_each {|i| main_args.delete_at(i)}

  if !new_path.empty?

    new_path = new_path.join(File::PATH_SEPARATOR)

    $logger.info "Setting GEM_PATH to #{new_path}"
    ENV['GEM_PATH'] = new_path
  end

  # Operate on the gem_home option to set GEM_HOME
  if main_args.include? '--gem_home'
    option_index = main_args.index '--gem_home'
    path_index = option_index + 1
    new_home = main_args[path_index]
    main_args.slice! path_index
    main_args.slice! main_args.index '--gem_home'

    $logger.info "Setting GEM_HOME to #{new_home}"
    ENV['GEM_HOME'] = new_home
  end

  # Operate on the bundle option to set BUNDLE_GEMFILE
  use_bundler = false
  if main_args.include? '--bundle'
    option_index = main_args.index '--bundle'
    path_index = option_index + 1
    gemfile = main_args[path_index]
    main_args.slice! path_index
    main_args.slice! main_args.index '--bundle'

    $logger.info "Setting BUNDLE_GEMFILE to #{gemfile}"
    ENV['BUNDLE_GEMFILE'] = gemfile
    use_bundler = true

  end

  if main_args.include? '--bundle_path'
    option_index = main_args.index '--bundle_path'
    path_index = option_index + 1
    bundle_path = main_args[path_index]
    main_args.slice! path_index
    main_args.slice! main_args.index '--bundle_path'

    $logger.info "Setting BUNDLE_PATH to #{bundle_path}"
    ENV['BUNDLE_PATH'] = bundle_path

  elsif use_bundler
    # bundle was requested but bundle_path was not provided
    $logger.warn "Bundle activated but ENV['BUNDLE_PATH'] is not set"

    $logger.info "Setting BUNDLE_PATH to ':/ruby/3.2.0/'"
    ENV['BUNDLE_PATH'] = ':/ruby/3.2.0/'

  end

  if main_args.include? '--bundle_without'
    option_index = main_args.index '--bundle_without'
    path_index = option_index + 1
    bundle_without = main_args[path_index]
    main_args.slice! path_index
    main_args.slice! main_args.index '--bundle_without'

    $logger.info "Setting BUNDLE_WITHOUT to #{bundle_without}"
    ENV['BUNDLE_WITHOUT'] = bundle_without

  elsif use_bundler
    # bundle was requested but bundle_path was not provided
    $logger.info "Bundle activated but ENV['BUNDLE_WITHOUT'] is not set"

    # match configuration in build_openstudio_gems
    $logger.info "Setting BUNDLE_WITHOUT to 'test'"
    ENV['BUNDLE_WITHOUT'] = 'test'

    # ignore any local config on disk
    #DLM: this would be correct if the bundle was created here
    #it would not be correct if the bundle was transfered from another computer
    #ENV['BUNDLE_IGNORE_CONFIG'] = 'true'

  end

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

  # Load the bundle before activating any embedded gems
  if use_bundler

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
  end

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


  # Handle -e commands
  remove_indices = []
  $eval_cmds = []
  main_args.each_index do |i|

    if main_args[i] == '-e' || main_args[i] == '--execute'
      # remove from further processing
      remove_indices << i
      remove_indices << i+1

      cmd = main_args[i + 1]

      if cmd.nil?
        $logger.error "#{main_args[i]} requires second argument CMD"
        return false
      end

      $eval_cmds << cmd
    end
  end
  remove_indices.reverse_each {|i| main_args.delete_at(i)}

  if !main_args.empty?
    $logger.error "Unknown arguments #{main_args} found"
    return false
  end

  return true
end


# This CLI class processes the input args and invokes the proper command class
class CLI

  # This constant maps commands to classes in this CLI and stores meta-data on them
  def command_list
    {
        run: [ Proc.new { ::Run }, {primary: true, working: true}],
        #apply_measure: [ Proc.new { ::ApplyMeasure }, {primary: true, working: false}], # DLM: remove, can do this with run
        gem_list: [ Proc.new { ::GemList }, {primary: false, working: true}],
        #gem_install: [ Proc.new { ::InstallGem }, {primary: false, working: false}], # DLM: needs Ruby built with FFI
        measure: [ Proc.new { ::Measure }, {primary: true, working: false}],
        update: [ Proc.new { ::Update }, {primary: true, working: false}],
        execute_ruby_script: [ Proc.new { ::ExecuteRubyScript }, {primary: false, working: true}],
        #interactive_ruby: [ Proc.new { ::InteractiveRubyShell }, {primary: false, working: false}], # DLM: not working
        openstudio_version: [ Proc.new { ::OpenStudioVersion }, {primary: true, working: true}],
        energyplus_version: [ Proc.new { ::EnergyPlusVersion }, {primary: true, working: true}],
        ruby_version: [ Proc.new { ::RubyVersion }, {primary: false, working: true}],
        list_commands: [ Proc.new { ::ListCommands }, {primary: true, working: true}]
    }
  end

  # This method instantiates the global variables $main_args, $sub_command, and $sub_args
  #
  # @param [Array] argv The arguments passed through the CLI
  # @return [Object] An instance of the CLI class with initialized globals
  #
  def initialize(argv)
    argv = clean_argv(argv)
    $main_args, $sub_command, $sub_args = split_main_and_subcommand(argv, command_list)

    if $main_args.include? '--verbose'
      $logger.level = Logger::DEBUG
      OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Debug)
    end

    $logger.info("CLI Parsed Inputs: #{$main_args.inspect} #{$sub_command.inspect} #{$sub_args.inspect}")
  end

  # Checks to see if it should print the main help, and if not parses the command into a class and executes it
  def execute
    $logger.debug "Main arguments are #{$main_args}"
    $logger.debug "Sub-command is #{$sub_command}"
    $logger.debug "Sub-arguments are #{$sub_args}"
    if $main_args.include?('-h') || $main_args.include?('--help')
      # Help is next in short-circuiting everything. Print
      # the help and exit.
      help
      return 0
    end

    if $main_args.include?('--version')
      # Version is next in short-circuiting everything. Print it and exit.
      # This is to have the same behavior as pretty much every CLI, you expect
      # `<cli> --version` to return the version
      safe_puts OpenStudio.openStudioLongVersion
      return 0
    end

    if !parse_main_args($main_args)
      help
      return 1
    end

    # -e commands detected
    if !$eval_cmds.empty?
      $eval_cmds.each do |cmd|
        $logger.debug "Executing cmd: #{cmd}"
        eval(cmd, BINDING)
      end
      if $sub_command
        $logger.warn "Evaluate mode detected, ignoring sub_command #{$sub_command}"
        return 0
      end
      return 0
    end

    # If we reached this far then we must have a command. If not,
    # then we also just print the help and exit.
    command_plugin = nil
    if $sub_command
      command_plugin = command_list[$sub_command.to_sym]
    end

    if !command_plugin || !$sub_command
      help
      return 1
    end

    command_class = command_plugin[0].call
    $logger.debug("Invoking command class: #{command_class} #{$sub_args.inspect}")

    # Initialize and execute the command class, returning the exit status.
    result = 0
    begin
      result = command_class.new.execute($sub_args)
    rescue Interrupt
      $logger.error '?'
      result = 1
    end

    result = 0 unless result.is_a?(Integer)
    result
  end

  # Prints out the help text for the CLI
  #
  # @param [Boolean] list_all If set to true, the help prints all commands, however it otherwise only prints those
  #   marked as primary in #command_list
  # @param [Boolean] quiet If set to true, list only the names of commands without the header
  # @return [void]
  # @see #command_list #::ListCommands
  #
  def help(list_all=false, quiet=false)
    if quiet
      commands = ['-h','--help',
                  '--verbose',
                  '-i', '--include',
                  '-e', '--execute',
                  '--gem_path', '--gem_home', '--bundle', '--bundle_path', '--bundle_without']
      command_list.each do |key, data|
        # Skip non-primary commands. These only show up in extended
        # help output.
        next unless data[1][:primary] unless list_all
        commands << key.to_s
      end
      safe_puts commands #.join(" ")

    else
      opts = OptionParser.new do |o|
        o.banner = 'Usage: openstudio [options] <command> [<args>]'
        o.separator ''
        o.on('-h', '--help', 'Print this help.')
        o.on('--verbose', 'Print the full log to STDOUT')
        o.on('-I', '--include DIR', 'Add additional directory to add to front of Ruby $LOAD_PATH (may be used more than once)')
        o.on('-e', '--execute CMD', 'Execute one line of script (may be used more than once). Returns after executing commands.')
        o.on('--gem_path DIR', 'Add additional directory to add to front of GEM_PATH environment variable (may be used more than once)')
        o.on('--gem_home DIR', 'Set GEM_HOME environment variable')
        o.on('--bundle GEMFILE', 'Use bundler for GEMFILE')
        o.on('--bundle_path BUNDLE_PATH', 'Use bundler installed gems in BUNDLE_PATH')
        o.on('--bundle_without WITHOUT_GROUPS', 'Space separated list of groups for bundler to exclude in WITHOUT_GROUPS.  Surround multiple groups with quotes like "test development".')
        o.separator ''
        o.separator 'Common commands:'

        # Add the available commands as separators in order to print them
        # out as well.
        commands = {}
        longest = 0
        command_list.each do |key, data|
          # Skip non-primary commands. These only show up in extended
          # help output.
          next unless data[1][:primary] unless list_all

          key           = key.to_s
          klass         = data[0].call
          commands[key] = klass.synopsis
          longest       = key.length if key.length > longest
        end

        commands.keys.sort.each do |key|
          o.separator "     #{key.ljust(longest+2)} #{commands[key]}"
        end

        o.separator ''
        o.separator 'For help on any individual command run `openstudio COMMAND -h`'
        unless list_all
          o.separator ''
          o.separator 'Additional commands are available, but are either more advanced'
          o.separator 'or not commonly used. To see all commands, run the command'
          o.separator '`openstudio list_commands`.'
        end
      end

      safe_puts opts.help
    end
  end
end

# Class to execute part or all of an OSW workflow
class Run

  # Provides text for the main help functionality
  def self.synopsis
    'Executes an OpenStudio Workflow file'
  end

  # Executes the standard, or one of two custom, workflows using the workflow-gem
  #
  # @param [Array] sub_argv Options passed to the run command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "Run, sub_argv = #{sub_argv}"

    # options are local to this method, run_methods are what get passed to workflow gem
    run_options = {}

    # Hidden option shhhhh
    run_options[:fast] = false
    if sub_argv.delete '--fast'
      run_options[:fast] = true
    end

    options = {}
    options[:debug] = false
    options[:no_simulation] = false
    options[:osw_path] = './workflow.osw'
    options[:post_process] = false
    options[:ep_json] = false
    options[:ft_options] = {}
    options[:show_stdout] = false
    options[:add_timings] = false
    options[:style_stdout] = false
    # TODO: I don't know if there's any value harcoding a default here really
    # options[:ft_options] = {
    #   :runcontrolspecialdays => true,
    #   :ip_tabular_output => false,
    #   :no_lifecyclecosts => false,
    #   :no_sqlite_output => false,
    #   :no_html_output => false,
    #   :no_variable_dictionary => false,
    #   :no_space_translation => false,
    # }

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio run [options]'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('-w', '--workflow [FILE]', 'Specify the FILE path to the workflow to run') do |osw_path|
        options[:osw_path] = osw_path
      end
      o.on('-m', '--measures_only', 'Only run the OpenStudio and EnergyPlus measures') do
        options[:no_simulation] = true
      end
      o.on('-p', '--postprocess_only', 'Only run the reporting measures') do
        options[:post_process] = true
      end
      o.on('--export-epJSON', 'export epJSON file format. The default is IDF') do
        options[:ep_json] = true
      end
      o.on('-s', '--socket PORT', 'Pipe status messages to a socket on localhost PORT') do |port|
        options[:socket] = port
      end
      o.on('--show-stdout', 'Prints the output of the workflow run in real time to the console, including E+ output') do
        options[:show_stdout] = true
      end
      o.on('--debug', 'Includes additional outputs for debugging failing workflows and does not clean up the run directory') do |f|
        options[:debug] = f
      end

      o.separator ""
      o.separator "Forward Translator Options:"

      o.on('--[no-]runcontrolspecialdays', "Include RunControlSpecialDays (Holidays) [Default: True]") do |b|
        options[:ft_options][:runcontrolspecialdays] = b
      end

      o.on('--set-ip-tabular-output', "Request IP units from E+ Tabular (HTML) Report [Default: False]") do |b|
        options[:ft_options][:ip_tabular_output] = b
      end

      o.on('--[no-]lifecyclecosts', "Include LifeCycleCosts [Default: True]") do |b|
        options[:ft_options][:no_lifecyclecosts] = !b
      end

      o.on('--[no-]sqlite-output', "Request Output:SQLite from E+ [Default: True]") do |b|
        options[:ft_options][:no_sqlite_output] = !b
      end

      o.on('--[no-]html-output', "Request Output:Table:SummaryReports report from E+ [Default: True]") do |b|
        options[:ft_options][:no_html_output] = !b
      end

      o.on('--[no-]space-translation', "Add individual E+ Space [Default: True]") do |b|
        options[:ft_options][:no_space_translation] = !b
      end

      o.separator ""
      o.separator "Stdout Options: only available when --show-stdout is passed"

      o.on('--add-timings', 'Print the start, end and elapsed times of each state of the simulation.') do
          options[:add_timings] = true
      end
      o.on('--style-stdout', 'Style the stdout output to more clearly show the start and end of each state of the simulation') do
          options[:style_stdout] = true
      end

      o.separator ""

    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("Run command: #{argv.inspect} #{options.inspect}")

    unless argv == []
      $logger.error 'Extra arguments passed to the run command. Please refer to the help documentation.'
      return 1
    end

    if options[:post_process] && options[:no_simulation]
      $logger.error 'Both the -m and -p flags were set, which is an invalid combination.'
      return 1
    end

    require 'openstudio-workflow'

    osw_path = options[:osw_path]
    osw_path = File.absolute_path(File.join(Dir.pwd, osw_path)) unless Pathname.new(osw_path).absolute?
    $logger.debug "Path for the OSW: #{osw_path}"

    if options[:debug]
      run_options[:debug] = true
      run_options[:cleanup] = false
    end

    if options[:ep_json]
      run_options[:ep_json] = true
    end

    if !options[:ft_options].empty?
      run_options[:ft_options] = {}
      options[:ft_options].each do |opt_flag_name, opt_flag|
        run_options[:ft_options][opt_flag_name] = opt_flag
      end
    end

    if options[:show_stdout] && options[:socket]
      raise "Error: --show-stdout and --socket cannot be used at the same time"
    end

    if options[:socket]
      require 'openstudio/workflow/adapters/input/local'
      require 'openstudio/workflow/adapters/output/socket'
      input_adapter = OpenStudio::Workflow::InputAdapter::Local.new(osw_path)
      output_adapter = OpenStudio::Workflow::OutputAdapter::Socket.new({output_directory: input_adapter.run_dir, port: options[:socket]})
      run_options[:output_adapter] = output_adapter
    end

    tcp_thread = nil
    if options[:show_stdout]
      # prefered port
      port = 8080
      server = nil
      n_try = 0
      begin
        n_try += 1
        server = TCPServer.open('localhost', port)
      rescue Errno::EADDRINUSE
        port = rand(65000 - 1024) + 1024
        if n_try < 10
          retry
        else
          raise "Cannot find a TCP port that isn't in use"
        end
      end

      def h1(str)
        len = [80, str.length + 4].max
        l1 = "#" * len
        n1 = (len - str.length - 2) / 2
        n2 = (len - str.length - 2) - n1
        l2 = "#" + " " * n1 + str + " " * n2 + "#"

        return "\n" + l1 + "\n" + l2 + "\n" + l1
      end

      def end_h1(str)
        len = [80, str.length + 4].max
        n1 = (len - str.length - 2) / 2
        n2 = (len - str.length - 2) - n1
        l2 = "#" * n1 + " " + str + + " " + "#" * n2
        return l2 + "\n"
      end

      # Start a TCP thread to receive the messages that workflow-gem with send
      tcp_thread = Thread.new do
        start_time = nil
        while client = server.accept
          while line = client.gets.strip

            if line.downcase.start_with?('starting state')
              if options[:style_stdout]
                puts h1(line)
              else
                puts line
              end
              if options[:add_timings]
                start_time = Time.now
                puts "Start Time: #{start_time}"
              end

            elsif line.downcase.start_with?('returned from state')
              if options[:add_timings]
                end_time = Time.now
                puts "End Time: #{end_time}"
                puts "Elapsed Time for state: #{(end_time - start_time).round(2)} s"
              end
              if options[:style_stdout]
                puts end_h1(line)
              else
                puts line
              end

            else
              puts line
            end
          end
        end
      end

      require 'openstudio/workflow/adapters/input/local'
      require 'openstudio/workflow/adapters/output/socket'
      input_adapter = OpenStudio::Workflow::InputAdapter::Local.new(osw_path)
      output_adapter = OpenStudio::Workflow::OutputAdapter::Socket.new({output_directory: input_adapter.run_dir, port: port})
      run_options[:output_adapter] = output_adapter
    end

    if options[:no_simulation]
      run_options[:jobs] = [
        { state: :queued, next_state: :initialization, options: { initial: true } },
        { state: :initialization, next_state: :os_measures, job: :RunInitialization,
          file: 'openstudio/workflow/jobs/run_initialization.rb', options: {} },
        { state: :os_measures, next_state: :translator, job: :RunOpenStudioMeasures,
          file: 'openstudio/workflow/jobs/run_os_measures.rb', options: {} },
        { state: :translator, next_state: :ep_measures, job: :RunTranslation,
          file: 'openstudio/workflow/jobs/run_translation.rb', options: {} },
        { state: :ep_measures, next_state: :preprocess, job: :RunEnergyPlusMeasures,
          file: 'openstudio/workflow/jobs/run_ep_measures.rb', options: {} },
        { state: :preprocess, next_state: :postprocess, job: :RunPreprocess,
          file: 'openstudio/workflow/jobs/run_preprocess.rb', options: {} },
        { state: :postprocess, next_state: :finished, job: :RunPostprocess,
          file: 'openstudio/workflow/jobs/run_postprocess.rb', options: {} },
        { state: :finished },
        { state: :errored }
      ]
    elsif options[:post_process]
      run_options[:jobs] = [
        { state: :queued, next_state: :initialization, options: { initial: true } },
        { state: :initialization, next_state: :reporting_measures, job: :RunInitialization,
          file: 'openstudio/workflow/jobs/run_initialization.rb', options: {} },
        { state: :reporting_measures, next_state: :postprocess, job: :RunReportingMeasures,
          file: 'openstudio/workflow/jobs/run_reporting_measures.rb', options: {} },
        { state: :postprocess, next_state: :finished, job: :RunPostprocess,
          file: 'openstudio/workflow/jobs/run_postprocess.rb', options: {} },
        { state: :finished },
        { state: :errored }
      ]
      run_options[:preserve_run_dir] = true
    end

    if options[:add_timings]
      workflow_start = Time.now
      puts "WORKFLOW START TIME: #{workflow_start}"
    end

    $logger.debug "Initializing run method"
    k = OpenStudio::Workflow::Run.new osw_path, run_options

    $logger.debug "Beginning run"
    state = k.run

    if options[:add_timings]
      workflow_end = Time.now
      puts "WORKFLOW END TIME: #{workflow_end}"
      puts "WORKFLOW ELAPSED TIME: #{(workflow_end - workflow_start).round(2)} s"
    end

    if options[:show_stdout]
      Thread.kill(tcp_thread)
    end

    # check if state symbol is either :finished or :errored
    if state == :finished
      return 0
    else
      return 1
    end
  end
end

# Class to apply an OpenStudio, EnergyPlus, or Reporting measure
# @abstract
class ApplyMeasure

  # Provides text for the main help functionality
  def self.synopsis
    'Applies an OpenStudio, EnergyPlus, or Reporting measure'
  end

  # Executes a single measure using an undefined interface
  #
  # @param [Array] sub_argv Options passed to the apply_measures command from the user input
  # @return [Fixnum] Return status
  # @abstract
  #
  def execute(sub_argv)

    $logger.info "ApplyMeasure, sub_argv = #{sub_argv}"

    options = {}
    #options[:debug] = false

    # opts = OptionParser.new do |o|
    #   o.banner = 'Usage: openstudio apply_measure'
    #   o.separator ''
    #   o.separator 'Options:'
    #   o.separator ''
    # end

    $logger.error 'This interface has yet to be defined.'

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("ApplyMeasure command: #{argv.inspect} #{options.inspect}")

    return 1 unless argv
    require 'openstudio-workflow'

    1
  end
end

# Class to list the gems used by the CLI
class GemList

  # Provides text for the main help functionality
  def self.synopsis
    'Lists the set gems available to openstudio'
  end

  # Alters the environment variable used to define the gem install location
  #
  # @param [Array] sub_argv Options passed to the gem_install command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require 'rubygems'

    $logger.info "GemList, sub_argv = #{sub_argv}"

    options = {}

    # Parse the options
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio gem_list'
    end
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("GemList command: #{argv.inspect} #{options.inspect}")

    unless argv == []
      $logger.error 'Extra arguments passed to the gem_list command. Please refer to the help documentation.'
      return 1
    end

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

    0
  end
end

# Class to install gems using the RubyGems functionality
class InstallGem

  # Provides text for the main help functionality
  def self.synopsis
    'Installs a Gem using the embedded ruby'
  end

  # Executes the RubyGems gem install process, using the RubyGems options
  #
  # @param [Array] sub_argv Options passed to the gem_install command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require 'rubygems'
    require 'rubygems/commands/install_command'

    $logger.info "InstallGem, sub_argv = #{sub_argv}"

    options = {}

    # Parse the options
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio gem_install gem_name gem_version'
      o.separator ''
      o.separator 'Options:'
      o.separator ''
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("InstallGem command: #{argv.inspect} #{options.inspect}")

    if argv == []
      $logger.error 'No gem name provided'
      return 1
    end
    gem_name = argv[0]
    gem_version = argv[1]

    cmd = Gem::Commands::InstallCommand.new
    cmd.handle_options ["--no-ri", "--no-rdoc", 'rake', '--version', '0.9']  # or omit --version and the following option to install the latest

    ARGV.clear
    ARGV << gem_name
    if gem_version
      ARGV << '--version'
      ARGV << gem_version
    end

    $logger.info "Installing gem to #{ENV['GEM_HOME']}"

    begin
      cmd.execute
    rescue => e
      $logger.error "Error installing gem: #{e.message} in #{e.backtrace.join("\n")}"
      exit e.exit_code
    rescue LoadError => e
      # DLM: gem install tries to load a Windows dll to access network functionality in win32/resolv
      # Ruby must be built with libffi to enable fiddle extension to enable win32 extension
      $logger.error "gem_install command not yet implemented, requires fiddle extension"
      #$logger.error "#{e.message} in #{e.backtrace.join("\n")}"
      return 1
    end

    $logger.info 'The gem was successfully installed'

    0
  end
end

# Class to update measures and compute arguments
class Measure

  # Provides text for the main help functionality
  def self.synopsis
    'Updates measures and compute arguments'
  end

  # Executes code to update and compute arguments for measures
  #
  # @param [Array] sub_argv Options passed to the e command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "Measure, sub_argv = #{sub_argv}"

    require_relative 'measure_manager'

    options = {}
    options[:update] = false
    options[:compute_arguments] = nil

    # save some arguments to pass to minitest
    saved_subargv = []
    if sub_argv[0] == '-r' || sub_argv[0] == '--run_tests'
      saved_subargv = sub_argv[2..-1]
      sub_argv = sub_argv[0...2]
    end

    # find the directory
    directory = nil
    if sub_argv.size > 1
      unless (sub_argv[0] == '-s' || sub_argv[0] == '--start_server')
        directory = sub_argv.pop
        $logger.debug("Directory to examine is #{directory}")
        $logger.debug("Remaining args are #{sub_argv}")
      end
    end

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio measure [options] DIRECTORY'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('-t', '--update_all', 'Update all measures in a directory') do
        options[:update_all] = true
      end
      o.on('-u', '--update', 'Update the measure.xml') do
        options[:update] = true
      end
      o.on('-a', '--compute_arguments [MODEL]', 'Compute arguments for the given OSM or IDF') do |model_file|
        options[:compute_arguments] = true
        options[:compute_arguments_model] = model_file
      end
      o.on('-r', '--run_tests', 'Run all tests recursively found in a directory, additional arguments are passed to minitest') do
        options[:run_tests] = true
      end
      o.on('-s', '--start_server [PORT]', 'Start a measure manager server') do |port|
        options[:start_server] = true
        options[:start_server_port] = port
      end
      # TODO: run unit tests
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("Measure command: #{argv.inspect} #{options.inspect}")

    if !options[:start_server]
      if directory.nil?
        $logger.error 'No directory provided'
        return 1
      end
      directory = File.expand_path(directory)
    end

    $logger.debug("Directory to examine is #{directory}")

    if options[:update_all]
      measure_manager = MeasureManager.new($logger)

      # loop over all directories
      result = []
      Dir.glob("#{directory}/*/").each do |measure_dir|
        if File.directory?(measure_dir) && File.exist?(File.join(measure_dir, "measure.xml"))
          measure = measure_manager.get_measure(measure_dir, true)
          if measure.nil?
            $logger.debug("Directory #{measure_dir} is not a measure")
          else
            result << measure_manager.measure_hash(measure_dir, measure)
          end
        end
      end

      safe_puts JSON.generate(result)

    elsif options[:update]
      measure_manager = MeasureManager.new($logger)
      measure = measure_manager.get_measure(directory, true)
      if measure.nil?
        $logger.error("Cannot load measure from '#{directory}'")
        return 1
      end

      hash = measure_manager.measure_hash(directory, measure)
      safe_puts JSON.generate(hash)

    elsif options[:compute_arguments]
      measure_manager = MeasureManager.new($logger)
      measure = measure_manager.get_measure(directory, true)
      if measure.nil?
        $logger.error("Cannot load measure from '#{directory}'")
        return 1
      end

      model_path = options[:compute_arguments_model]

      model = OpenStudio::Model::OptionalModel.new()
      workspace = OpenStudio::OptionalWorkspace.new()
      if model_path
        measure_type = measure.measureType.valueName
        if measure_type == 'ModelMeasure'

          value = measure_manager.get_model(model_path, true)
          if value.nil?
            $logger.error("Cannot load model from '#{model_path}'")
            return 1
          else
            model = value[0].clone(true).to_Model
            workspace = value[1].clone(true)
          end

        elsif measure_type == 'EnergyPlusMeasure'
          value = measure_manager.get_idf(model_path, true)

          if value.nil?
            $logger.error("Cannot load workspace from '#{model_path}'")
            return 1
          else
            workspace = value.clone(true)
          end

        elsif measure_type == 'ReportingMeasure'
          # New in 3.0.0, it does take a model path
          value = measure_manager.get_model(model_path, true)
          if value.nil?
            $logger.error("Cannot load model from '#{model_path}'")
            return 1
          else
            model = value[0].clone(true).to_Model
            workspace = value[1].clone(true)
          end

        else
          $logger.error("Measure type '#{measure_type}' does not take a model path")
          return 1
        end

      else
        model_path = ""
      end

      measure_info = measure_manager.get_measure_info(directory, measure, model_path, model, workspace)

      hash = measure_manager.measure_hash(directory, measure, measure_info)
      safe_puts JSON.generate(hash)

    elsif options[:run_tests]

      # restore saved arguments for minitest
      ARGV.clear
      saved_subargv.each do |arg|
        ARGV << arg
      end
      $logger.debug("Minitest arguments are '#{saved_subargv}'")

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

      runner = OpenStudioMeasureTester::Runner.new(directory)
      result = runner.run_all(Dir.pwd)

      if result != 0
        $logger.error("Measure tester returned errors")
        return 1
      end

    elsif options[:start_server]

      require_relative 'measure_manager_server'

      port = options[:start_server_port]
      if port.nil?
        port = 1234
      end

      server = WEBrick::HTTPServer.new(:Port => port)

      server.mount "/", MeasureManagerServlet

      trap("INT") {
          server.shutdown
      }

      server.start

    else
      $logger.error("Unknown measure command")
      return 1
    end

    0
  end
end

# Class to update openstudio models
class Update

  # Provides text for the main help functionality
  def self.synopsis
    'Updates OpenStudio Models to the current version'
  end

  # Executes code to update and compute arguments for measures
  #
  # @param [Array] sub_argv Options passed to the e command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "Update, sub_argv = #{sub_argv}"

    options = {}
    options[:keep] = false

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio update [options] PATH'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('-k', '--keep', 'Keep original files') do
        options[:keep] = true
      end
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("Measure command: #{argv.inspect} #{options.inspect}")

    if argv == []
      $logger.error 'No path provided'
      return 1
    end
    path = File.expand_path(argv[0])

    $logger.debug("Path to examine is #{path}")

    paths = []
    if File.file?(path)
      $logger.debug("Path is regular file")
      paths << path
    else
      $logger.debug("Path is directory")
      Dir.glob(path + "/*.osm").each do |path|
        paths << path
      end
    end

    vt = OpenStudio::OSVersion::VersionTranslator.new

    result = 0
    paths.each do |path|
      if options[:keep]
        FileUtils.cp(path, path + ".orig")
      end

      model = vt.loadModel(path)
      if model.empty?
        $logger.error("Could not read model at #{path}")
        result = 1
        next
      end
      model.get.save(path, true)
    end

    result
  end
end

# Class to execute a ruby script
class ExecuteRubyScript

  # Provides text for the main help functionality
  def self.synopsis
    'Executes a ruby file'
  end

  # Executes an arbitrary ruby script
  #
  # @param [Array] sub_argv Options passed to the execute_ruby_script command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "ExecuteRubyScript, sub_argv = #{sub_argv}"

    require 'pathname'

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio execute_ruby_script file [arguments]'
    end

    if sub_argv.size == 1
      if sub_argv[0] == '-h' || sub_argv[0] == '--help'
        safe_puts(opts.help)
        return 0
      end
    end

    # Parse the options
    # DLM: don't do argument parsing as in other commands since we want to pass the remaining arguments to the ruby script
    return 0 if sub_argv == nil
    return 1 unless sub_argv
    $logger.debug("ExecuteRubyScript command: #{sub_argv.inspect}")
    file_path = sub_argv.shift.to_s
    file_path = File.absolute_path(File.join(Dir.pwd, file_path)) unless Pathname.new(file_path).absolute?
    $logger.debug "Path for the file to run: #{file_path}"

    ARGV.clear
    sub_argv.each do |arg|
      ARGV << arg
    end

    $logger.debug "ARGV: #{ARGV}"

    unless File.exist? file_path
      $logger.error "Unable to find the file #{file_path} on the filesystem"
      return 1
    end

    require file_path

    0
  end
end

# Class to put the user into an irb shell
class InteractiveRubyShell

  # Provides text for the main help functionality
  def self.synopsis
    'Provides an interactive ruby shell'
  end

  # Executes the commands to get into an IRB prompt
  #
  # @param [Array] sub_argv Options passed to the interactive_ruby command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require 'irb'

    $logger.info "InteractiveRubyShell, sub_argv = #{sub_argv}"

    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio interactive_ruby'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("InteractiveRubyShell command: #{argv.inspect} #{options.inspect}")

    unless argv == []
      $logger.error 'Extra arguments passed to the i command.'
      return 1
    end

    IRB.start_session(binding)

    0
  end
end

# Class to return the packaged OpenStudio version
class OpenStudioVersion

  # Provides text for the main help functionality
  def self.synopsis
    'Returns the OpenStudio version used by the CLI'
  end

  # Executes the OpenStudio commands to return the OpenStudio version
  #
  # @param [Array] sub_argv Options passed to the energyplus_version command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "OpenStudioVersion, sub_argv = #{sub_argv}"

    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio openstudio_version'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("OpenStudioVersion command: #{argv.inspect} #{options.inspect}")

    unless argv == []
      $logger.error 'Extra arguments passed to the openstudio_version command.'
      return 1
    end

    safe_puts OpenStudio.openStudioLongVersion

    0
  end
end

# Class to return the EnergyPlus version
class EnergyPlusVersion

  # Provides text for the main help functionality
  def self.synopsis
    'Returns the EnergyPlus version used by the CLI'
  end

  # Executes the OpenStudio command to return the EnergyPlus version
  #
  # @param [Array] sub_argv Options passed to the openstudio_version command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "EnergyPlusVersion, sub_argv = #{sub_argv}"

    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio energyplus_version'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("EnergyPlusVersion command: #{argv.inspect} #{options.inspect}")

    unless argv == []
      $logger.error 'Arguments passed to the energyplus_version command.'
      return 1
    end

    safe_puts OpenStudio.energyPlusVersion

    0
  end
end

# Class to return the ruby version used by the packaged CLI
class RubyVersion

  # Provides text for the main help functionality
  def self.synopsis
    'Returns the Ruby version used by the CLI'
  end

  # Evaluates the RUBY_VERSION constant and returns it
  #
  # @param [Array] sub_argv Options passed to the ruby_version command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)

    $logger.info "RubyVersion, sub_argv = #{sub_argv}"

    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio ruby_version'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("RubyVersion command: #{argv.inspect} #{options.inspect}")

    unless argv == []
      $logger.error 'Arguments passed to the ruby_version command.'
      return 1
    end

    safe_puts RUBY_VERSION

    0
  end
end

# Class to return the full set of possible commands for the CLI
class ListCommands

  # Provides text for the main help functionality
  def self.synopsis
    'Lists the entire set of available commands'
  end

  # Executes the standard help method with the list_all attribute set to true
  #
  # @param [Array] sub_argv Options passed to the list_all command from the user input
  # @return [Fixnum] Return status
  # @see #::CLI.help
  #
  def execute(sub_argv)

    $logger.info "ListCommands, sub_argv = #{sub_argv}"

    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio list_commands'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('--quiet', "If --quiet is passed, list only the names of commands without the header") do
        options[:quiet] = true
      end

    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil

    $logger.debug("ListCommands command: #{argv.inspect} #{options.inspect}")

    # If anything else than --quiet is passed
    unless argv == []
      $logger.error 'Extra Arguments passed to the list_commands command, please refer to the help documentation.'
      return 1
    end

    if options[:quiet]
      $logger.debug 'Creating a new CLI instance and calling help with list_all AND quiet enabled'
      ::CLI.new([]).help(true, true)
    else
      $logger.debug 'Creating a new CLI instance and calling help with list_all enabled'
      ::CLI.new([]).help(true)
    end
    0
  end
end


### Configure Gems to load the correct Gem files
### @see http://rubygems.rubyforge.org/rubygems-update/Gem.html
##local_dir = ENV['OPENSTUDIO_GEM_PATH'] ? ENV['OPENSTUDIO_GEM_PATH'] : "~/OpenStudio/v#{OpenStudio.openStudioVersion}"
##ENV['GEM_PATH'] = OpenStudio::Workflow::Util::IO.is_windows? ? local_dir.gsub('/', '\\') : local_dir
##$logger.debug "Set environment variable GEM_PATH to #{ENV['GEM_PATH']}"
##ENV['GEM_HOME'] = OpenStudio::Workflow::Util::IO.is_windows? ? '\\' : '/'
##$logger.debug "Set environment variable GEM_HOME to #{ENV['GEM_HOME']}"
##Gem.clear_paths
##$logger.debug 'Reset Gem paths; openstudio associated gems should load correctly'

# Execute the CLI interface, and exit with the proper error code
$logger.info "Executing argv: #{ARGV}"

begin
  result = CLI.new(ARGV).execute
rescue Exception => e
  puts "Error executing argv: #{ARGV}"
  if e.backtrace.nil?
    puts "Error: #{e.message}"
  else
    puts "Error: #{e.message} in #{e.backtrace.join("\n")}"
  end
  result = 1
end
STDOUT.flush

if result != 0
  # DLM: exit without a call stack but with a non-zero exit code
  exit!(false)
end
