######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

require 'fileutils'
 # @todo (rhorsey) OpenStudio does have class UUID, do we really need to keep securerandom as a dependency? - DLM
require 'securerandom' # uuids 
require 'json' # needed for a single pretty generate call
require 'pathname'

require_relative 'openstudio/workflow/version'
require_relative 'openstudio/workflow/multi_delegator'
require_relative 'openstudio/workflow/run'
require_relative 'openstudio/workflow/job'
require_relative 'openstudio/workflow/time_logger'
require_relative 'openstudio/workflow/registry'
require_relative 'openstudio/workflow/util'

require 'openstudio'
require_relative 'openstudio/workflow_runner'

 # @todo (rhorsey) what is this for?  can we remove it? - DLM
ENV['OPENSTUDIO_WORKFLOW'] = 'true'

# some core extensions
# @todo (rhorsey) is this really needed? extensions to built in classes are not a great idea, they may conflict with other people's code - DLM
class String
  def to_underscore
    gsub(/::/, '/')
      .gsub(/([A-Z]+)([A-Z][a-z])/, '\1_\2')
      .gsub(/([a-z\d])([A-Z])/, '\1_\2')
      .tr('-', '_')
      .downcase
  end
end

module OpenStudio

  # @todo (rhorsey) move the load and energyplus methods into the core CLI
  module Workflow
    extend self

    # Log the message sent to the logger
    def logger(targets=nil)
      @logger ||= ::Logger.new MultiDelegator.delegate(:write, :close).to(*targets)
      @logger
    end

=begin # This needs to all get moved into the Core CLI
    # Create a new workflow instance using the defined adapter and UUID
    #
    def load(adapter_name, run_directory, options = {})
      defaults = { adapter_options: {} }
      options = defaults.merge(options)
      @targets = [STDOUT, File.new('run.log')]
      logger(@targets)

      # Convert various paths to absolute paths
      if options[:analysis_root_path] &&
         (Pathname.new options[:analysis_root_path]).absolute? == false
        options[:analysis_root_path] = File.expand_path options[:analysis_root_path]
      end
      unless (Pathname.new run_directory).absolute?
        # relative to wherever you are running the script
        run_directory = File.expand_path run_directory
      end
      adapter = load_adapter adapter_name, options[:adapter_options]
      run_klass = OpenStudio::Workflow::Run.new(adapter, run_directory, options)
      # return the run class
      run_klass
    end

    # predefined method that simply runs EnergyPlus in the specified directory. It does not apply any workflow steps
    # such as preprocessing / postprocessing. The directory must have the IDF and EPW file in the folder. The
    # simulations will run in the directory/run path
    #
    # @todo (rhorsey) Do we want the transitions here to be init->translate->preprocess->simulate->finish?
    #
    # @param adapter_name [String] Type of adapter, local or mongo.
    # @param run_directory [String] Path to where the simulation is to run
    # @param options [Hash] List of options for the adapter
    #
    def run_energyplus(adapter_name, run_directory, options = {})
      unless (Pathname.new run_directory).absolute?
        # relative to wherever you are running the script
        run_directory = File.expand_path run_directory
      end

      transitions = [
        { from: :queued, to: :preflight },
        { from: :preflight, to: :energyplus },
        { from: :energyplus, to: :finished }
      ]

      states = [
        { state: :queued, options: { initial: true } },
        { state: :preflight, options: { after_enter: :run_preflight } },
        { state: :energyplus, options: { after_enter: :run_energyplus } },
        { state: :finished },
        { state: :errored }
      ]
      options = {
        transitions: transitions,
        states: states
      }

      adapter = load_adapter adapter_name, options[:adapter_options]
      run_klass = OpenStudio::Workflow::Run.new(adapter, run_directory, options)

      run_klass
    end
=end

    # Extract an archive to a specific location
    #
    # @param archive_filename [String] Path and name of the file to extract
    # @param destination [String] Path to extract to
    # @param overwrite [Boolean] If true, will overwrite any extracted file that may already exist
    #
    def extract_archive(archive_filename, destination, overwrite = true)
      # @todo (rhorsey) OpenStudio does have classes ZipFile and UnzipFile, do we really need to keep rubyzip as a dependency? - DLM
      Zip::File.open(archive_filename) do |zf|
        zf.each do |f|
          f_path = File.join(destination, f.name)
          FileUtils.mkdir_p(File.dirname(f_path))

          if File.exist?(f_path) && overwrite
            FileUtils.rm_rf(f_path)
            zf.extract(f, f_path)
          elsif !File.exist? f_path
            zf.extract(f, f_path)
          end
        end
      end
    end

    def load_adapter(name, adapter_options = {})
      require "openstudio/workflow/adapters/#{name.downcase}"
      klass_name = name.to_s.split('_').map(&:capitalize) * ''
      # pp "#{klass_name} is the adapter class name"
      klass = OpenStudio::Workflow::Adapters.const_get(klass_name).new(adapter_options)
      klass
    end
  end
end
