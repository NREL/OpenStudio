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

class RunPreprocess < OpenStudio::Workflow::Job

  require_relative '../util'
  include OpenStudio::Workflow::Util::EnergyPlus
  include OpenStudio::Workflow::Util::Model

  def initialize(adapter, registry, options = {})
    super
  end

  def perform
    @logger.info "Calling #{__method__} in the #{self.class} class"

    # Ensure that the directory is created (but it should already be at this point)
    FileUtils.mkdir_p(@registry[:run_dir])

    # Copy in the weather file defined in the registry, or alternately in the options
    if @registry[:wf]
      @logger.info "Weather file for EnergyPlus simulation is #{@registry[:wf]}"
      FileUtils.copy(@registry[:wf], "#{@registry[:run_dir]}/in.epw")
    elsif @options[:simulation_weather_file]
      @logger.warn "Using weather file defined in options: #{@options[:simulation_weather_file]}"
      FileUtils.copy(@options[:simulation_weather_file], "#{@registry[:run_dir]}/in.epw")
    else
      fail "EPW file not found or not sent to #{self.class}"
    end

    # Save the model objects in the registry to the run directory
    # @todo (dlm) what about the no osm case? Can we just `unless @registry[:model]` it?
    if File.exist?("#{@registry[:run_dir]}/in.idf")
      @logger.warn 'IDF (in.idf) already exists in the run directory. Will simulate using this file'
    else
      save_idf(@registry[:model_idf], @registry[:run_dir])
      save_osm(@registry[:model], @registry[:run_dir])
    end

    # Perform pre-processing on in.idf to capture logic in RunManager
    @registry[:time_logger].start('Running EnergyPlus Preprocess Script') if @registry[:time_logger]
    energyplus_preprocess("#{@registry[:run_dir]}/in.idf")
    @registry[:time_logger].start('Running EnergyPlus Preprocess Script') if @registry[:time_logger]
    @logger.info "Finished Preprocess job"


    results = {}
  end
end
