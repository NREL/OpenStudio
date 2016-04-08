#!/usr/bin/env ruby

Signal.trap('INT') { abort }

require 'openstudio-workflow'
require 'optparse'
require 'irb'

$logger = Logger.new(STDOUT)
$logger.level = Logger::WARN


# This is the code chunk to allow for an embedded IRB shell. Provided by Jason Roelofs, found on StackOverflow
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
# @return[nil, If this method returns `nil`, then you should assume that help was printed and parsing failed
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
# @return [Array] The three parts
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

#This is the CLI class that does the actual execution, (i.e. the one we actually invoke)
class CLI

  COMMAND_LIST = {
      run: [ Proc.new { ::Run }, {primary: true, working: true}],
      apply_measure: [ Proc.new { ::ApplyMeasure }, {primary: true, working: false}],
      gem_install: [ Proc.new { ::InstallGem }, {primary: false, working: true}],
      e: [ Proc.new { ::ExecuteRubyScript }, {primary: false, working: true}],
      i: [ Proc.new { ::InteractiveRubyShell }, {primary: false, working: true}],
      openstudio_version: [ Proc.new { ::OpenStudioVersion }, {primary: true, working: true}],
      energyplus_version: [ Proc.new { ::EnergyPlusVersion }, {primary: true, working: true}],
      ruby_version: [ Proc.new { ::RubyVersion }, {primary: false, working: true}],
      list_commands: [ Proc.new { ::ListCommands }, {primary: true, working: true}]
  }

  def initialize(argv)
    $main_args, $sub_command, $sub_args = split_main_and_subcommand(argv)

    $logger.info("CLI Parsed Inputs: #{$main_args.inspect} #{$sub_command.inspect} #{$sub_args.inspect}")
  end

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

  # This prints out the help for the CLI.
  def help(list_all=false)
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli [options] <command> [<args>]'
      o.separator ''
      o.on('-h', '--help', 'Print this help.')
      o.on('--verbose', 'Print the full log to STDOUT')
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

class Run
  def self.synopsis
    'Executes an OpenStudio Workflow file'
  end

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
      $logger.error "Both the -m and -p flags were set, which is an invalid combination."
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

class ApplyMeasure
  def self.synopsis
    'Applies an OpenStudio, EnergyPlus, or Reporting measure'
  end

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
    # argv = parse_options(opts, sub_argv)
    # return 1 unless argv
    # $logger.debug("ApplyMeasure command: #{argv.inspect} #{options.inspect}")

    1
  end
end

# Install gems
class InstallGem
  def self.synopsis
    'Installs a Gem using the embedded ruby'
  end

  def execute(sub_argv)
    require 'rubygems'
    require 'rubygems/gem_runner'

    # Parse the options
    argv = sub_argv.unshift('install')
    $logger.debug("GemInstall command: #{argv.inspect}")
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

class ExecuteRubyScript
  def self.synopsis
    'Executes a ruby file'
  end

  def execute(sub_argv)
    options = {}
    options[:debug] = false

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

    require file_path

    0
  end
end

class InteractiveRubyShell
  def self.synopsis
    'Provides an interactive ruby shell'
  end

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

class OpenStudioVersion
  def self.synopsis
    'Returns the OpenStudio version used by the CLI'
  end

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

class EnergyPlusVersion
  def self.synopsis
    'Returns the EnergyPlus version used by the CLI'
  end

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

class RubyVersion
  def self.synopsis
    'Returns the Ruby version used by the CLI'
  end

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

class ListCommands
  def self.synopsis
    'Lists the entire set of available commands for openstudio_cli'
  end

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

# FROM VAGRANT bin - the code used to invoke the CLI action
# Split arguments by "--" if its there, we'll recombine them later
$argv = ARGV.dup
if $argv.include? '--verbose'
  $logger.level = Logger::DEBUG
  $argv.delete '--verbose'
end
$logger.debug "Input ARGV is #{$argv}"

# Execute the CLI interface, and exit with the proper error code
CLI.new($argv).execute
