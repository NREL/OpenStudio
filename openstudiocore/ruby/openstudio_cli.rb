#!/usr/bin/env ruby

######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################


Signal.trap('INT') { abort }

require 'openstudio-workflow'
require 'optparse'
require 'irb'
include OpenStudio::Workflow::Util::IO

$logger = Logger.new(STDOUT)
$logger.level = Logger::WARN


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

# This method will split the argv given into three parts: the flags to this command, the subcommand, and the flags to
# the subcommand. For example:
#     -v status -h -v
# The above would yield 3 parts:
#     ["-v"]
#     "status"
#     ["-h", "-v"]
# These parts are useful because the first is a list of arguments given to the current command, the second is a
# subcommand, and the third are the commands given to the subcommand
#
# @param [Array] argv The input to be split
# @return [Array] The split command as [main arguments, sub command, sub command arguments]
#
def split_main_and_subcommand(argv)
  # Initialize return variables
  main_args   = nil
  sub_command = nil
  sub_args    = []

  # We split the arguments into two: One set containing any flags before a word, and then the rest. The rest are what
  # get actually sent on to the subcommand
  argv.each_index do |i|
    unless argv[i].start_with?('-')
      # We found the beginning of the sub command. Split the
      # args up.
      main_args   = argv[0, i]
      sub_command = argv[i]
      sub_args    = argv[i + 1, argv.length - i + 1]

      # Break so we don't find the next non flag and shift our main args
      break
    end
  end

  # Handle the case that argv was empty or didn't contain any subcommand
  main_args = argv.dup if main_args.nil?

  [main_args, sub_command, sub_args]
end

# This CLI class processes the input args and invokes the proper command class
class CLI

  # This constant maps subcommands to classes in this CLI and stores meta-data on them
  COMMAND_LIST = {
      run: [ Proc.new { ::Run }, {primary: true, working: true}],
      apply_measure: [ Proc.new { ::ApplyMeasure }, {primary: true, working: false}],
      gem_list: [ Proc.new { ::GemList }, {primary: true, working: true}],
      gem_install: [ Proc.new { ::InstallGem }, {primary: false, working: true}],
      measure: [ Proc.new { ::Measure }, {primary: true, working: false}],
      e: [ Proc.new { ::ExecuteRubyScript }, {primary: false, working: true}],
      i: [ Proc.new { ::InteractiveRubyShell }, {primary: false, working: true}],
      openstudio_version: [ Proc.new { ::OpenStudioVersion }, {primary: true, working: true}],
      energyplus_version: [ Proc.new { ::EnergyPlusVersion }, {primary: true, working: true}],
      ruby_version: [ Proc.new { ::RubyVersion }, {primary: false, working: true}],
      list_commands: [ Proc.new { ::ListCommands }, {primary: true, working: true}]
  }

  # This method instantiates the global variables $main_args, $sub_command, and $sub_args
  #
  # @param [Array] argv The arguments passed through the CLI
  # @return [Object] An instance of the CLI class with initialized globals
  #
  def initialize(argv)
    $main_args, $sub_command, $sub_args = split_main_and_subcommand(argv)

    $logger.info("CLI Parsed Inputs: #{$main_args.inspect} #{$sub_command.inspect} #{$sub_args.inspect}")
  end

  # Checks to see if it should print the main help, and if not parses the subcommand into a class and executes it
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

    # If we reached this far then we must have a subcommand. If not,
    # then we also just print the help and exit.
    command_plugin = nil
    if $sub_command
      command_plugin = COMMAND_LIST[$sub_command.to_sym]
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

    result = 0 unless result.is_a?(Fixnum)
    result
  end

  # Prints out the help text for the CLI
  #
  # @param [Boolean] list_all If set to true, the help prints all commands, however it otherwise only prints those
  #   marked as primary in #COMMAND_LIST
  # @return [void]
  # @see #COMMAND_LIST #::ListCommands
  #
  def help(list_all=false)
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli [options] <command> [<args>]'
      o.separator ''
      o.on('-h', '--help', 'Print this help.')
      o.on('--verbose', 'Print the full log to STDOUT')
      o.on('--gem_path PATH', 'Path to use for installing gems to and loading gems from')
      o.separator ''
      o.separator 'Common commands:'

      # Add the available subcommands as separators in order to print them
      # out as well.
      commands = {}
      longest = 0
      COMMAND_LIST.each do |key, data|
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
      o.separator 'For help on any individual command run `openstudio_cli COMMAND -h`'
      unless list_all
        o.separator ''
        o.separator 'Additional subcommands are available, but are either more advanced'
        o.separator 'or not commonly used. To see all subcommands, run the command'
        o.separator '`openstudio_cli list_commands`.'
      end
    end

    safe_puts opts.help
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
  # @param [Array] sub_argv Options passed to the run subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}
    options[:debug] = false
    options[:no_simulation] = false
    options[:workflow] = './workflow.osw'
    options[:post_process] = false

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli run [options]'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('-w', '--workflow [FILE]', 'Specify the FILE path to the workflow to run') do |workflow|
        options[:workflow] = workflow
      end
      o.on('-m', '--measures_only', 'Only run the OpenStudio and EnergyPlus measures') do
        options[:no_simulation] = true
      end
      o.on('-p', '--postprocess_only', 'Only run the reporting measures') do
        options[:post_process] = true
      end
      o.on('--debug', 'Includes additional outputs for debugging failing workflows and does not clean up the run directory') do |f|
        options[:debug] = f
      end
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil
    unless argv == []
      $logger.error 'Extra arguments passed to the run command. Please refer to the help documentation.'
      return 1
    end
    $logger.debug("Run command: #{argv.inspect} #{options.inspect}")

    if options[:post_process] && options[:no_simulation]
      $logger.error 'Both the -m and -p flags were set, which is an invalid combination.'
      return 1
    end

    osw_path = options[:workflow]
    osw_path = File.absolute_path(File.join(Dir.pwd, osw_path)) unless Pathname.new(osw_path).absolute?
    $logger.debug "Path for the OSW: #{osw_path}"

    adapter_options = {workflow_filename: File.basename(osw_path)}
    adapter = OpenStudio::Workflow.load_adapter 'local', adapter_options
    run_options = options[:debug] ? {debug: true, cleanup: false} : {}
    if options[:no_simulation]
      run_options[:jobs] = [
        { state: :queued, next_state: :initialization, options: { initial: true } },
        { state: :initialization, next_state: :os_measures, job: :RunInitialization,
          file: './jobs/run_initialization.rb', options: {} },
        { state: :os_measures, next_state: :translator, job: :RunOpenStudioMeasures,
          file: './jobs/run_os_measures.rb', options: {} },
        { state: :translator, next_state: :ep_measures, job: :RunTranslation,
          file: './jobs/run_translation.rb', options: {} },
        { state: :ep_measures, next_state: :finished, job: :RunEnergyPlusMeasures,
          file: './jobs/run_ep_measures.rb', options: {} },
        { state: :finished },
        { state: :errored }
      ]
    elsif options[:post_process]
      run_options[:jobs] = [
        { state: :queued, next_state: :initialization, options: { initial: true } },
        { state: :initialization, next_state: :reporting_measures, job: :RunInitialization,
          file: './jobs/run_initialization.rb', options: {} },
        { state: :reporting_measures, next_state: :postprocess, job: :RunReportingMeasures,
          file: './jobs/run_reporting_measures.rb', options: {} },
        { state: :postprocess, next_state: :finished, job: :RunPostprocess,
          file: './jobs/run_postprocess.rb', options: {} },
        { state: :finished },
        { state: :errored }
      ]
      run_options[:load_simulation_osm] = true
      run_options[:load_simulation_idf] = true
      run_options[:load_simulation_sql] = true
      run_options[:preserve_run_dir] = true
    end
    k = OpenStudio::Workflow::Run.new adapter, File.dirname(osw_path), run_options
    k.run

    0
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
  # @param [Array] sub_argv Options passed to the apply_measures subcommand from the user input
  # @return [Fixnum] Return status
  # @abstract
  #
  def execute(sub_argv)
    # options = {}
    # options[:debug] = false

    # opts = OptionParser.new do |o|
    #   o.banner = 'Usage: openstudio_cli apply_measure'
    #   o.separator ''
    #   o.separator 'Options:'
    #   o.separator ''
    # end

    $logger.error 'This interface has yet to be defined.'

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 1 unless argv
    # $logger.debug("ApplyMeasure command: #{argv.inspect} #{options.inspect}")

    1
  end
end

# Class to list the gems used by the CLI
class GemList

  # Provides text for the main help functionality
  def self.synopsis
    'Lists the set gems available to openstudio_cli'
  end

  # Alters the environment variable used to define the gem install location
  #
  # @param [Array] sub_argv Options passed to the gem_install subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require 'rubygems'
    require 'rubygems/gem_runner'

    # Parse the options
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli gem_list'
    end
    argv = parse_options(opts, sub_argv)
    unless argv == []
      $logger.error 'Extra arguments passed to the gem_list command. Please refer to the help documentation.'
      return 1
    end

    begin
      Gem::GemRunner.new.run ['list']
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
  # @param [Array] sub_argv Options passed to the gem_install subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require 'rubygems'
    require 'rubygems/gem_runner'

    # Parse the options
    argv = sub_argv.unshift('install')
    if argv.include? '--install-dir'
      $logger.error 'The rubygems option --install-dir is not supported. Please use the --gem_path option'
      return 1
    else
      argv.push '--install-dir'
      argv.push ENV['GEM_PATH']
    end
    $logger.debug "GemInstall command: #{argv.inspect}"
    $logger.debug "Invoking the GemRunner with argv: #{argv}"

    begin
      Gem::GemRunner.new.run argv
    rescue => e
      $logger.error "Error installing gem: #{e.message} in #{e.backtrace.join("\n")}"
      exit e.exit_code
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
  # @param [Array] sub_argv Options passed to the e subcommand from the user input
  # @return [Fixnum] Return status
  # @todo (macumber) Add in the OpenStudio code to execute on measures
  #
  def execute(sub_argv)
    options = {}
    options[:check_for_update] = false
    options[:update] = false
    options[:compute_arguments] = nil

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli measure [options] DIRECTORY'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('-c', '--check_for_updates', 'Check to see if the measure needs to be updated') do
        options[:check_for_update] = workflow
      end
      o.on('-u', '--update', 'Update the measure.xml') do
        options[:update] = true
      end
      o.on('-a', '--compute_arguments [MODEL]', 'Compute arguments for the given model') do |model_file|
        options[:compute_arguments] = model_file
      end
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    $logger.error 'No directory provided' if argv == []
    return 1 if argv == []
    directory = argv[0]

    $logger.debug("Measure command: #{argv.inspect} #{options.inspect}")
    $logger.debug("Directory to examine is #{directory}")

    $logger.error 'Code to be added here to operate on measures'

    1
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
  # @param [Array] sub_argv Options passed to the e subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli e [file]'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil
    return 1 unless argv
    $logger.debug("ExecuteRubyScript command: #{argv.inspect} #{options.inspect}")
    file_path = argv.shift.to_s
    file_path = File.absolute_path(File.join(Dir.pwd, file_path)) unless Pathname.new(file_path).absolute?
    $logger.debug "Path for the file to run: #{file_path}"

    unless argv == []
      $logger.error 'Extra arguments passed to the e command. Please refer to the help documentation.'
      return 1
    end

    unless File.exists? file_path
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
  # @param [Array] sub_argv Options passed to the i subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli i'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    $logger.debug("InteractiveRubyShell command: #{argv.inspect} #{options.inspect}")
    return 0 if argv == nil

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
  # @param [Array] sub_argv Options passed to the energyplus_version subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli openstudio_version'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    $logger.debug("OpenStudioVersion command: #{argv.inspect} #{options.inspect}")
    return 0 if argv == nil

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
  # @param [Array] sub_argv Options passed to the openstudio_version subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli energyplus_version'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    $logger.debug("EnergyPlusVersion command: #{argv.inspect} #{options.inspect}")
    return 0 if argv == nil

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
  # @param [Array] sub_argv Options passed to the ruby_version subcommand from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli ruby_version'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    $logger.debug("RubyVersion command: #{argv.inspect} #{options.inspect}")
    return 0 if argv == nil

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
    'Lists the entire set of available commands for openstudio_cli'
  end

  # Executes the standard help method with the list_all attribute set to true
  #
  # @param [Array] sub_argv Options passed to the list_all subcommand from the user input
  # @return [Fixnum] Return status
  # @see #::CLI.help
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli list_commands'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    $logger.debug("ListCommands command: #{argv.inspect} #{options.inspect}")
    return 0 if argv == nil

    unless argv == []
      $logger.error 'Arguments passed to the list_commands command.'
      return 1
    end

    $logger.debug 'Creating a new CLI instance and calling help with list_all enabled'
    ::CLI.new([]).help(true)

    0
  end
end

# Set the logger level to DEBUG if the arguments include --verbose
$argv = ARGV.dup
if $argv.include? '--verbose'
  $logger.level = Logger::DEBUG
  $argv.delete '--verbose'
  $logger.debug 'Set Logger log level to DEBUG'
end
$logger.debug "Input ARGV is #{$argv}"

# Operate on the gem_path option to set the gem search path
if $argv.include? '--gem_path'
  $logger.info 'Setting gem path'
  option_index = $argv.index '--gem_path'
  path_index = option_index + 1
  new_home = $argv[path_index]
  $argv.slice! path_index
  $argv.slice! $argv.index '--gem_path'
  current_home = ENV['OPENSTUDIO_GEM_PATH']
  $logger.warn "Overwriting previous OPENSTUDIO_GEM_PATH of #{current_home} to #{new_home} for this command" if current_home
  $logger.info "No current gem path set in OPENSTUDIO_GEM_PATH, setting to #{new_home}" unless current_home
  ENV['OPENSTUDIO_GEM_PATH'] = new_home
end

# Configure Gems to load the correct Gem files
# @see http://rubygems.rubyforge.org/rubygems-update/Gem.html
local_dir = ENV['OPENSTUDIO_GEM_PATH'] ? ENV['OPENSTUDIO_GEM_PATH'] : "~/OpenStudio/v#{OpenStudio.openStudioVersion}"
ENV['GEM_PATH'] = OpenStudio::Workflow::Util::IO.is_windows? ? local_dir.gsub('/', '\\') : local_dir
$logger.debug "Set environment variable GEM_PATH to #{ENV['GEM_PATH']}"
ENV['GEM_HOME'] = OpenStudio::Workflow::Util::IO.is_windows? ? '\\' : '/'
$logger.debug "Set environment variable GEM_HOME to #{ENV['GEM_HOME']}"
Gem.clear_paths
$logger.debug 'Reset Gem paths; openstudio_cli associated gems should load correctly'

# Execute the CLI interface, and exit with the proper error code
$logger.info "Executing argv: #{$argv}"
CLI.new($argv).execute
