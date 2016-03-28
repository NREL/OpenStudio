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

# Adapter class to decide where to obtain instructions to run the simulation workflow
module OpenStudio
  module Workflow
    # @todo (rhorsey) seems like we should have separate InputAdapters for getting Workflow JSON and OutputAdapters for posting results/errors/status - DLM
    class Adapter
      attr_accessor :options

      def initialize(options = {})
        @options = options
        @log = nil
        @datapoint = nil
      end

      # class << self
      # attr_reader :problem

      # @todo (rhorsey) input method - DLM
      def load(filename, options = {})
        instance.load(filename, options)
      end

      # @todo (rhorsey) output method - DLM
      def communicate_started(id, _options = {})
        instance.communicate_started id
      end

      # @todo (rhorsey) remove - DLM
      def get_datapoint(id, options = {})
        instance.get_datapoint id, options
      end

      # @todo (rhorsey) remove - DLM
      def get_problem(id, options = {})
        instance.get_problem id, options
      end

      # @todo (rhorsey) output method - DLM
      def communicate_results(id, results)
        instance.communicate_results id, results
      end

      # @todo (rhorsey) output method - DLM
      def communicate_complete(id)
        instance.communicate_complete id
      end

      # @todo (rhorsey) output method - DLM
      def communicate_failure(id)
        instance.communicate_failure id
      end

      protected

      # Zip up a folder and it's contents
      def zip_directory(directory, zip_filename, pattern = '*')
        # Submethod for adding the directory to the zip folder.
        def add_directory_to_zip(zip_file, local_directory, root_directory)
          Dir[File.join("#{local_directory}", '**', '**')].each do |file|
            # remove the base directory from the zip file
            rel_dir = local_directory.sub("#{root_directory}/", '')
            zip_file_to_add = file.gsub("#{local_directory}", "#{rel_dir}")
            zip_file.add(zip_file_to_add, file)
          end

          zip_file
        end

        FileUtils.rm_f(zip_filename) if File.exist?(zip_filename)

        Zip.default_compression = Zlib::BEST_COMPRESSION
        Zip::File.open(zip_filename, Zip::File::CREATE) do |zf|
          Dir[File.join(directory, pattern)].each do |file|
            if File.directory?(file)
              # skip a few directory that should not be zipped as they are inputs
              if File.basename(file) =~ /seed|measures|weather/
                next
              end
              # skip x-large directory
              if File.size?(file)
                next if File.size?(file) >= 15000000
              end
              add_directory_to_zip(zf, file, directory)
            else
              next if File.extname(file) =~ /\.rb.*/
              next if File.extname(file) =~ /\.zip.*/
              # skip large non-osm/idf files
              if File.size(file)
                if File.size(file) >= 15000000
                  next unless File.extname(file) == '.osm' || File.extname(file) == '.idf'
                end
              end

              zip_file_to_add = file.gsub("#{directory}/", '')
              zf.add(zip_file_to_add, file)
            end
          end
        end

        File.chmod(0664, zip_filename)
      end

      # Main method to zip up the results of the simulation results. This will append the UUID of the data point
      # if it exists. This method will create two zip files. One for the reports and one for the entire data point. The
      # Data Point ZIP will also contain the reports.
      #
      # @param directory [String] The data point directory to zip up.
      # @return nil
      def zip_results(directory)
        # create zip file using a system call
        if Dir.exist?(directory) && File.directory?(directory)
          zip_filename = @datapoint ? "data_point_#{@datapoint.uuid}.zip" : 'data_point.zip'
          zip_filename = File.join(directory, zip_filename)
          zip_directory directory, zip_filename
        end

        # zip up only the reports folder
        report_dir = File.join(directory, 'reports')
        if Dir.exist?(report_dir) && File.directory?(report_dir)
          zip_filename = @datapoint ? "data_point_#{@datapoint.uuid}_reports.zip" : 'data_point_reports.zip'
          zip_filename = File.join(directory, zip_filename)
          zip_directory directory, zip_filename, 'reports'
        end
      end
    end
  end
end
