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

class RunEnergyPlusMeasures < OpenStudio::Workflow::Job

  require_relative '../util'
  include OpenStudio::Workflow::Util::Measure
  include OpenStudio::Workflow::Util::Model

  # Initialize
  # param directory: base directory where the simulation files are prepared
  # param logger: logger object in which to write log messages
  # @todo fix where osm and idf are saved
  def initialize(adapter, registry, options = {})
    super
  end

  def perform
    @logger.info "Calling #{__method__} in the #{self.class} class"

    # Ensure output_attributes is initialized in the registry
    @registry.register(:output_attributes) { {} } unless @registry[:output_attributes]
    
    @logger.info 'Beginning to execute EnergyPlus measures.'
    apply_measures(:energyplus, @registry, @options)
    @logger.info('Finished applying EnergyPlus measures.')

    @logger.info 'Saving measure output attributes JSON'
    File.open("#{@registry[:run_dir]}/measure_attributes.json", 'w') do |f|
      f << JSON.pretty_generate(@registry[:output_attributes])
    end

    @registry[:time_logger].start('Saving OSM and IDF') if @registry[:time_logger]
    osm_name = save_osm(@registry[:model], @registry[:root_dir])
    idf_name = save_idf(@registry[:model], @registry[:root_dir])
    @registry[:time_logger].stop('Saving OSM and IDF') if @registry[:time_logger]
    @logger.info "Saved OSM as #{osm_name}"
    @logger.info "Saved IDF as #{idf_name}"

    results = {}
  end
end
