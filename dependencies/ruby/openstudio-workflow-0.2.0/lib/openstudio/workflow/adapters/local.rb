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

require_relative '../adapter'

# Local file based workflow
module OpenStudio
  module Workflow
    module Adapters
      class Local < Adapter
        def initialize(options = {})
          super
        end

        # Tell the system that the process has started
        #
        def communicate_started(directory, _options = {})
          # Watch out for namespace conflicts (::Time is okay but Time is OpenStudio::Time)
          File.open("#{directory}/started.job", 'w') { |f| f << "Started Workflow #{::Time.now}" }
        end

        # Get the OSW file from the local filesystem
        #
        def get_workflow(directory, options = {})
          defaults = { workflow_filename: 'workflow.osw', format: 'json' }
          defaults.merge! self.options
          options = defaults.merge options

          # how do we log within this file?
          osw_abs_path = File.absolute_path(File.join(directory, options[:workflow_filename]))
          if File.exist? osw_abs_path
            ::JSON.parse(File.read(osw_abs_path), {symbolize_names: true})
          else
            fail "Workflow file does not exist for #{osw_abs_path}"
          end
        end

        # Get the associated OSD (datapoint) file from the local filesystem
        #
        def get_datapoint(directory, options = {})
          defaults = { datapoint_filename: 'datapoint.osd', format: 'json' }
          defaults.merge! self.options
          options = defaults.merge options

          osd_abs_path = File.absolute_path(File.join(directory, options[:datapoint_filename]))
          if File.exist? osd_abs_path
            ::JSON.parse(File.read(osd_abs_path), {symbolize_names: true})
          else
            nil
          end
        end

        # Get the associated OSA (analysis) definition from the local filesystem
        #
        def get_analysis(directory, options = {})
          defaults = { analysis_filename: 'analysis.osa', format: 'json' }
          defaults.merge! self.options
          options = defaults.merge options

          osa_abs_path = File.absolute_path(File.join(directory, options[:analysis_filename]))
          if File.exist? osa_abs_path
            ::JSON.parse(File.read(osa_abs_path), {symbolize_names: true})
          else
            nil
          end
        end

        def communicate_complete(directory)
          File.open("#{directory}/finished.job", 'w') { |f| f << "Finished Workflow #{::Time.now}" }
        end

        # Final state of the simulation. The os_directory is the run directory and may be needed to
        # zip up the results of the simuation.
        def communicate_failure(directory)
          File.open("#{directory}/failed.job", 'w') { |f| f << "Failed Workflow #{::Time.now}" }
          # @communicate_module.communicate_failure(@communicate_object, os_directory)
        end

        def communicate_results(directory, results)
          zip_results(directory)

          if results.is_a? Hash
            File.open("#{directory}/data_point_out.json", 'w') { |f| f << JSON.pretty_generate(results) }
          else
            puts "Unknown datapoint result type. Please handle #{results.class}"
            # data_point_json_path = OpenStudio::Path.new(run_dir) / OpenStudio::Path.new('data_point_out.json')
            # os_data_point.saveJSON(data_point_json_path, true)
          end
          # end
        end
      end
    end
  end
end
