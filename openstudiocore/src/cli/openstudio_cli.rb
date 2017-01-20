#!/usr/bin/env ruby

########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

OpenStudio::Application::instance().application(false)
if (!OpenStudio::RemoteBCL::initializeSSL())
  puts "Unable to initialize OpenSSL: Verify that openstudio.exe can access the OpenSSL libraries"
end

#File.open('E:\test\test.log', 'w') do |f|
#  ENV.each_key {|k| f.puts "#{k} = #{ENV[k]}" }
#end

#Signal.trap('INT') { abort }

require 'logger'
require 'optparse'
#require 'irb'
#include OpenStudio::Workflow::Util::IO

$logger = Logger.new(STDOUT)
#$logger.level = Logger::ERROR
$logger.level = Logger::WARN
#$logger.level = Logger::DEBUG

#OpenStudio::Logger.instance.standardOutLogger.disable
#OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Warn)
OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Error)

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
# @return [Array] The split command as [main arguments, sub command, sub command arguments]
#
def split_main_and_subcommand(argv)
  # Initialize return variables
  main_args   = nil
  sub_command = nil
  sub_args    = []

  # We split the arguments into two: One set containing any flags before a word, and then the rest. The rest are what
  # get actually sent on to the command
  argv.each_index do |i|
    if argv[i].start_with?('-') 
      next
    elsif argv[i].end_with?('.rb')
      sub_command = 'e'
      sub_args    = argv[i..-1]
      break
    else
      # We found the beginning of the sub command. Split the
      # args up.
      main_args   = argv[0, i]
      sub_command = argv[i]
      sub_args    = argv[i + 1, argv.length - i + 1]

      # Break so we don't find the next non flag and shift our main args
      break
    end
  end

  # Handle the case that argv was empty or didn't contain any command
  main_args = argv.dup if main_args.nil?

  [main_args, sub_command, sub_args]
end

# This CLI class processes the input args and invokes the proper command class
class CLI

  # This constant maps commands to classes in this CLI and stores meta-data on them
  def command_list
    {
        run: [ Proc.new { ::Run }, {primary: true, working: true}],
        #apply_measure: [ Proc.new { ::ApplyMeasure }, {primary: true, working: false}],
        gem_list: [ Proc.new { ::GemList }, {primary: false, working: true}],
        gem_install: [ Proc.new { ::InstallGem }, {primary: false, working: true}],
        measure: [ Proc.new { ::Measure }, {primary: true, working: false}],
        update: [ Proc.new { ::Update }, {primary: true, working: false}],
        e: [ Proc.new { ::ExecuteRubyScript }, {primary: false, working: true}],
        i: [ Proc.new { ::InteractiveRubyShell }, {primary: false, working: true}],
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
    $main_args, $sub_command, $sub_args = split_main_and_subcommand(argv)

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

    result = 0 unless result.is_a?(Fixnum)
    result
  end

  # Prints out the help text for the CLI
  #
  # @param [Boolean] list_all If set to true, the help prints all commands, however it otherwise only prints those
  #   marked as primary in #command_list
  # @return [void]
  # @see #command_list #::ListCommands
  #
  def help(list_all=false)
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio [options] <command> [<args>]'
      o.separator ''
      o.on('-h', '--help', 'Print this help.')
      o.on('--verbose', 'Print the full log to STDOUT')
      o.on('-I', '--include DIR', 'Add additional directory to Ruby $LOAD_PATH (may be used more than once)')
      o.on('--gem_path PATH', 'Path to use for installing gems to and loading gems from')
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
  
    # options are local to this method, run_methods are what get passed to workflow gem
    run_options = {}
    
    options = {}
    options[:debug] = false
    options[:no_simulation] = false
    options[:osw_path] = './workflow.osw'
    options[:post_process] = false

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
      o.on('-s', '--socket PORT', 'Pipe status messages to a socket on localhost PORT') do |port|
        options[:socket] = port
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

    require 'openstudio-workflow'
    
    osw_path = options[:osw_path]
    osw_path = File.absolute_path(File.join(Dir.pwd, osw_path)) unless Pathname.new(osw_path).absolute?
    $logger.debug "Path for the OSW: #{osw_path}"

    if options[:debug]
      run_options[:debug] = true
      run_options[:cleanup] = false
    end
    
    if options[:socket]
      require 'openstudio/workflow/adapters/input/local'
      require 'openstudio/workflow/adapters/output/socket'
      input_adapter = OpenStudio::Workflow::InputAdapter::Local.new(osw_path)
      output_adapter = OpenStudio::Workflow::OutputAdapter::Socket.new({output_directory: input_adapter.run_dir, port: options[:socket]})
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
        { state: :ep_measures, next_state: :finished, job: :RunEnergyPlusMeasures,
          file: 'openstudio/workflow/jobs/run_ep_measures.rb', options: {} },
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
    
    $logger.debug "Initializing run method"
    k = OpenStudio::Workflow::Run.new osw_path, run_options
    
    $logger.debug "Beginning run"
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
  # @param [Array] sub_argv Options passed to the apply_measures command from the user input
  # @return [Fixnum] Return status
  # @abstract
  #
  def execute(sub_argv)
    # options = {}
    # options[:debug] = false

    # opts = OptionParser.new do |o|
    #   o.banner = 'Usage: openstudio apply_measure'
    #   o.separator ''
    #   o.separator 'Options:'
    #   o.separator ''
    # end

    $logger.error 'This interface has yet to be defined.'

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 1 unless argv
    require 'openstudio-workflow'
    # $logger.debug("ApplyMeasure command: #{argv.inspect} #{options.inspect}")

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
    require 'rubygems/gem_runner'

    # Parse the options
    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio gem_list'
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
  # @param [Array] sub_argv Options passed to the gem_install command from the user input
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
  # @param [Array] sub_argv Options passed to the e command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require_relative 'measure_manager'
    
    options = {}
    options[:update] = false
    options[:compute_arguments] = nil

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
      o.on('-s', '--start_server [PORT]', 'Start a measure manager server') do |port|
        options[:start_server] = true
        options[:start_server_port] = port
      end
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil
    
    directory = nil
    if !options[:start_server]
      if argv == []
        $logger.error 'No directory provided' 
        return 1
      end
      directory = File.expand_path(argv[0])
    end

    $logger.debug("Measure command: #{argv.inspect} #{options.inspect}")
    $logger.debug("Directory to examine is #{directory}")

    if options[:update_all]
      measure_manager = MeasureManager.new
      
      # loop over all directories
      result = []
      Dir.glob("#{directory}/*/").each do |measure_dir|
        if File.directory?(measure_dir) && File.exists?(File.join(measure_dir, "measure.xml"))
          measure = measure_manager.get_measure(measure_dir, true)
          if measure.nil?
            $logger.debug("Directory #{measure_dir} is not a measure")
          else
            result << measure_manager.measure_hash(measure_dir, measure)
          end
        end
      end
         
      puts JSON.generate(result)
      
    elsif options[:update]
      measure_manager = MeasureManager.new
      measure = measure_manager.get_measure(directory, true)
      if measure.nil?
        $logger.error("Cannot load measure from '#{directory}'")
        return 1
      end
      
      hash = measure_manager.measure_hash(directory, measure)
      puts JSON.generate(hash)
      
    elsif options[:compute_arguments]
      measure_manager = MeasureManager.new
      measure = measure_manager.get_measure(directory, true)
      if measure.nil?
        $logger.error("Cannot load measure from '#{directory}'")
        return 1
      end
      
      # todo - handle the case where compute_arguments_model is an IDF
      osm_path = options[:compute_arguments_model]
      
      model = OpenStudio::Model::OptionalModel.new()
      workspace = OpenStudio::OptionalWorkspace.new()
      if osm_path
        value = measure_manager.get_model(osm_path, true)
        if value.nil?
          $logger.error("Cannot load model from '#{osm_path}'")
          return 1
        else
          model = value[0].clone(true).to_Model
          workspace = value[1].clone(true)
        end
      else
        osm_path = ""
      end
      
      measure_info = measure_manager.get_measure_info(directory, measure, osm_path, model, workspace)
      
      hash = measure_manager.measure_hash(directory, measure, measure_info)
      puts JSON.generate(hash)
      
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
      $logger.error("Measure command missing command")
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
    
    options = {}
    options[:keep] = false

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio measure [options] PATH'
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
    
    if argv == []
      $logger.error 'No path provided' 
      return 1
    end
    path = File.expand_path(argv[0])

    $logger.debug("Measure command: #{argv.inspect} #{options.inspect}")
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
  # @param [Array] sub_argv Options passed to the e command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    require 'pathname'

    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio e [file]'
    end

    # Parse the options
    argv = parse_options(opts, sub_argv)
    return 0 if argv == nil
    return 1 unless argv
    $logger.debug("ExecuteRubyScript command: #{argv.inspect} #{options.inspect}")
    file_path = argv.shift.to_s
    file_path = File.absolute_path(File.join(Dir.pwd, file_path)) unless Pathname.new(file_path).absolute?
    $logger.debug "Path for the file to run: #{file_path}"
    
    ARGV.clear
    argv.each do |arg|
      ARGV << arg
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
  # @param [Array] sub_argv Options passed to the i command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio i'
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
  # @param [Array] sub_argv Options passed to the energyplus_version command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio openstudio_version'
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
  # @param [Array] sub_argv Options passed to the openstudio_version command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio energyplus_version'
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
  # @param [Array] sub_argv Options passed to the ruby_version command from the user input
  # @return [Fixnum] Return status
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio ruby_version'
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
    'Lists the entire set of available commands'
  end

  # Executes the standard help method with the list_all attribute set to true
  #
  # @param [Array] sub_argv Options passed to the list_all command from the user input
  # @return [Fixnum] Return status
  # @see #::CLI.help
  #
  def execute(sub_argv)
    options = {}

    opts = OptionParser.new do |o|
      o.banner = 'Usage: openstudio list_commands'
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
  OpenStudio::Logger.instance.standardOutLogger.setLogLevel(OpenStudio::Debug)
  $argv.delete '--verbose'
  $logger.debug 'Set Logger log level to DEBUG'
end
$logger.debug "Input ARGV is #{$argv}"

# Operate on the include option to add to $LOAD_PATH
$argv.each_index do |i|
  remove_indices = []
  if $argv[i] == '-I' || $argv[i] == '--include'
    # remove from further processing
    remove_indices << i 
    remove_indices << i+1
    
    dir = $argv[i + 1]
    
    if dir.nil? 
      safe_puts "#{$argv[i]} requires second argument DIR"
      return 0
    elsif !File.exists?(dir) || !File.directory?(dir)
      $logger.warn "'#{dir}' passed to #{$argv[i]} is not a directory"
    end
    $LOAD_PATH << dir
  end
  
  remove_indices.reverse_each {|i| $argv.delete_at(i)}
end

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
$logger.info "Executing argv: #{$argv}"
CLI.new($argv).execute
