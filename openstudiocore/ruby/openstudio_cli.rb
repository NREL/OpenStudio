#!/usr/bin/env ruby

require 'openstudio-workflow'
require 'optparse'
require 'irb'

$logger = Logger.new(STDOUT)

# This is the code chunk to allow for an embedded IRB shell. Written by Jason Roelofs, found on StackOverflow
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
def parse_options(opts=nil)
  # Creating a shallow copy of the arguments so the OptionParser
  # doesn't destroy the originals.
  argv = $argv.dup

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
      gem_install: [ Proc.new { ::InstallGem }, {primary: true, working: false}],
      e: [ Proc.new { ::ExecuteRubyScript }, {primary: false, working: true}],
      i: [ Proc.new { ::InteractiveRubyShell }, {primary: false, working: true}],
      openstudio_version: [ Proc.new { ::OpenStudioVersion }, {primary: true, working: true}],
      energyplus_version: [ Proc.new { ::EnergyPlusVersion }, {primary: true, working: true}],
      ruby_version: [ Proc.new { ::RubyVersion }, {primary: true, working: true}]
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
      puts "Should print help"
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
  def help
    opts = OptionParser.new do |o|
      o.banner = 'Usage: vagrant [options] <command> [<args>]'
      o.separator ''
      o.on('-v', '--version', 'Print the version and exit.')
      o.on('-h', '--help', 'Print this help.')
      o.separator ''
      o.separator 'Common commands:'

      # Add the available subcommands as separators in order to print them
      # out as well.
      commands = {}
      longest = 0
      COMMAND_LIST.each do |key, data|
        # Skip non-primary commands. These only show up in extended
        # help output.
        next unless data[1][:primary]

        key           = key.to_s
        klass         = data[0].call
        commands[key] = klass.synopsis
        longest       = key.length if key.length > longest
      end

      commands.keys.sort.each do |key|
        o.separator "     #{key.ljust(longest+2)} #{commands[key]}"
        # @env.ui.machine("cli-command", key.dup) # @todo What to do with this?
      end

      o.separator ''
      o.separator 'For help on any individual command run `openstudio-cli COMMAND -h`'
      o.separator ''
      o.separator 'Additional subcommands are available, but are either more advanced'
      o.separator 'or not commonly used. To see all subcommands, run the command'
      o.separator '`openstudio-cli list-commands`.'
    end

    safe_puts opts.help
  end
end

class Run
  def self.synopsis
    'Executes an OpenStudio Workflow file'
  end

  def execute(opts)
    options = {}
    options[:debug] = false

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio-cli run [options] [file]'
      o.separator ''
      o.separator 'Options:'
      o.separator ''

      o.on('--debug', 'Includes additional outputs for debugging failing workflows') do |f|
        options[:debug] = f
      end
    end

    # Parse the options
    argv = parse_options(opts)
    return 1 unless argv
    $logger.debug("Run command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...
    osw_path = argv.shift.to_s
    $logger.debug "Path for the OSW: #{osw_path}"

    unless argv == []
      $logger.error 'Extra arguments passed to the run command. Please refer to the help documentation.'
      return 1
    end

    adapter_options = {workflow_filename: File.basename(osw_path)}
    adapter = OpenStudio::Workflow.load_adapter 'local', adapter_options
    run_options = options[:debug] ? {debug: true} : {}
    k = OpenStudio::Workflow::Run.new adapter, File.dirname(osw_path), run_options
    k.run
    # Success, exit status 0
    return 0
  end
end

class ApplyMeasure
  def self.synopsis
    'Applies an OpenStudio, EnergyPlus, or Reporting measure'
  end

  def execute(opts)
    options = {}
    options[:debug] = false

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli apply_measure'
      o.separator ''
      o.separator 'Options:'
      o.separator ''
    end

    $logger.error 'This interface has yet to be defined.'

    0
  end
end

# Install gems @todo (kbenne) We need to figure out what the right thing to do here is for Packaging... See below
class InstallGem
  def self.synopsis
    'Installs a Gem using the embedded ruby'
  end

  def execute(opts)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli gem_install [gem] [options]'
      o.separator ''
      o.separator 'Options:'
      o.separator ''
    end

    # Parse the options
    argv = parse_options(opts)
    return 1 unless argv
    $logger.debug("GemInstall command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...

    # @todo (jason degraw) https://github.com/rubygems/rubygems/blob/master/bin/gem has the code we need to run, with
    #   a copywrite. Thoughts on what to do?
    $logger.error 'gem_install is not currently supported'

    0
  end
end

class ExecuteRubyScript
  def self.synopsis
    'Executes a ruby file'
  end

  def execute(opts)
    options = {}
    options[:debug] = false

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli e [file]'
    end

    # Parse the options
    argv = parse_options(opts)
    return 1 unless argv
    $logger.debug("ExecuteRubyScript command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...
    file_path = argv.shift.to_s
    $logger.debug "Path for the file to run: #{osw_path}"

    unless argv == []
      $logger.error 'Extra arguments passed to the e command. Please refer to the help documentation.'
      return 1
    end

    adapter_options = {workflow_filename: File.basename(osw_path)}
    adapter = OpenStudio::Workflow.load_adapter 'local', adapter_options
    run_options = options[:debug] ? {debug: true} : {}
    k = OpenStudio::Workflow::Run.new adapter, File.dirname(osw_path), run_options
    k.run

    0
  end
end

class InteractiveRubyShell
  def self.synopsis
    'Provides an interactive ruby shell'
  end

  def execute(opts)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli i'
    end

    # Parse the options
    argv = parse_options(opts)
    $logger.debug("InteractiveRubyShell command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...

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

  def execute(opts)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli openstudio_version'
    end

    # Parse the options
    argv = parse_options(opts)
    $logger.debug("OpenStudioVersion command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...

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

  def execute(opts)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli energyplus_version'
    end

    # Parse the options
    argv = parse_options(opts)
    $logger.debug("EnergyPlusVersion command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...

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

  def execute(opts)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio_cli ruby_version'
    end

    # Parse the options
    argv = parse_options(opts)
    $logger.debug("RubyVersion command: #{argv.inspect} #{options.inspect}")

    argv.shift # @todo (rhorsey) figure out why run is prepended to the file argv...

    unless argv == []
      $logger.error 'Arguments passed to the ruby_version command.'
      return 1
    end

    safe_puts RUBY_VERSION

    0
  end
end

# FROM VAGRANT bin - the code used to invoke the CLI action
# Split arguments by "--" if its there, we'll recombine them later
$argv = ARGV.dup
$logger.debug "Input ARGV is #{$argv}"

# Execute the CLI interface, and exit with the proper error code
CLI.new($argv).execute
