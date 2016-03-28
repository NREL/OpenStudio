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

# Run the initialization job to run validations and initializations
# @todo include util submodules specifically
class RunInitialization < OpenStudio::Workflow::Job

  require_relative '../util'
  include OpenStudio::Workflow::Util::Directory
  include OpenStudio::Workflow::Util::WeatherFile
  include OpenStudio::Workflow::Util::Model
  include OpenStudio::Workflow::Util::Measure

  def initialize(adapter, registry, options = {})
    defaults = {
        verify_osw: true,
        measure_paths: ['measures', '../../measures'],
        file_paths: ['files', 'weather', '../../files', '../../weather'],

    }
    super(adapter, registry, options, defaults)
  end

  # This method starts the adapter and verifies the OSW if the options contain verify_osw
  def perform
    @logger.info "Calling #{__method__} in the #{self.class} class"

    # Start the adapter
    # @todo (rhorsey) Figure out how to deprecate this
    @logger.info 'Starting communication with the adapter'
    @adapter.communicate_started @registry[:directory]

    # Load various files and set basic directories for the registry
    @registry.register(:workflow) { @adapter.get_workflow(@registry[:directory], @options) }
    @logger.info 'Retrieved the workflow from the adapter'
    fail 'Specified workflow was nil' unless @registry[:workflow]
    @registry.register(:root_dir) { get_root_dir(@registry[:workflow], @registry[:directory]) }
    @logger.info "The root_dir for the analysis is #{@registry[:root_dir]}"
    @registry.register(:datapoint) { @adapter.get_datapoint(@registry[:directory], @options) }
    @logger.info 'Found associated OSD file' if @registry[:datapoint]
    @registry.register(:analysis) { @adapter.get_analysis(@registry[:directory], @options) }
    @logger.info 'Found associated OSA file' if @registry[:analysis]
    @registry.register(:measure_paths) { @registry[:workflow][:measure_paths] } if @registry[:workflow][:measure_paths]
    @logger.info "Set measure_paths array in the registry to #{@registry[:measure_paths]}" if @registry[:measure_paths]
    @registry.register(:file_paths) { @registry[:workflow][:file_paths] } if @registry[:workflow][:file_paths]
    @logger.info "Set measure_paths array in the registry to #{@registry[:file_paths]}" if @registry[:file_paths]

    # Validate the OSW measures if the flag is set to true, (the default state)
    if @options[:verify_osw]
      @logger.info 'Attempting to validate the measure workflow'
      validate_measures(@registry[:workflow], @registry[:root_dir], @logger)
    end

    # Load or create the seed OSM object
    @logger.info 'Finding and loading the seed OSM file'
    model_name = @registry[:workflow][:seed_model] ? @registry[:workflow][:seed_model] : nil
    if @registry[:file_paths]
      file_search_paths = @registry[:file_paths].concat @options[:file_paths]
    else
      file_search_paths = @options[:file_paths]
    end
    @registry.register(:model) { load_seed_osm(@registry[:root_dir], model_name, file_search_paths, @logger) }

    # Load the weather file, should it exist and be findable
    @logger.info 'Getting the initial weather file'
    @registry[:workflow][:weather_file] ? wf = @registry[:workflow][:weather_file] : wf = nil
    @registry.register(:wf) { get_weather_file(@registry[:root_dir], wf, file_search_paths, @registry[:model], @logger) }
    @logger.warn 'No valid weather file defined in either the osm or osw.' unless @registry[:wf]

    # return the results back to the caller -- always
    results = {}
  end
end
