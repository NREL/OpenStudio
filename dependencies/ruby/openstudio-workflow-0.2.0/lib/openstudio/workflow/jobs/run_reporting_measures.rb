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

# Run precanned post processing to extract object functions
require 'csv'
require 'ostruct'

class RunReportingMeasures < OpenStudio::Workflow::Job

  require_relative '../util'
  include OpenStudio::Workflow::Util::Measure
  include OpenStudio::Workflow::Util::PostProcess

  def initialize(adapter, registry, options = {})
    super
  end

  def perform
    @logger.info "Calling #{__method__} in the #{self.class} class"
    @logger.info 'RunPostProcess Retrieving datapoint and problem'

    # Ensure output_attributes is initialized in the registry
    @registry.register(:output_attributes) { {} } unless @registry[:output_attributes]

    # Do something because
    translate_csv_to_json @registry[:root_dir]

    # Apply reporting measures
    @logger.info 'Beginning to execute OpenStudio measures.'
    apply_measures(:reporting, @registry, @options)
    @logger.info('Finished applying OpenStudio measures.')

    # Writing reporting measure attributes json
    # @todo check that measure_attributes exists where it should across all three measure applicators
    @logger.info 'Saving reporting measures output attributes JSON'
    File.open("#{@registry[:run_dir]}/measure_attributes.json", 'w') do |f|
      f << JSON.pretty_generate(@registry[:output_attributes])
    end

    # Run something else
    results, objective_functions = run_extract_inputs_and_outputs @registry[:run_dir]

    # Write out the obj function file
    # @todo add File.close
    @logger.info "Objective Function JSON is #{objective_functions}"
    obj_fun_file = "#{@registry[:directory]}/objectives.json"
    FileUtils.rm_f(obj_fun_file) if File.exist?(obj_fun_file)
    File.open(obj_fun_file, 'w') { |f| f << JSON.pretty_generate(objective_functions) }

    results = {}
  end
end
