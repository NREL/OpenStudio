module OpenStudio
  module Workflow
    module Util

      # The current precedence rules for weather files are defined in this module. Best practice is to only use the
      #  #get_weather_file method, as it will be forward compatible
      #
      module WeatherFile

        # Returns the weather file with precedence
        #
        # @param [String] directory The directory to append all relative directories to, see #get_weather_file_from_fs
        # @param [String] wf The weather file being searched for. If not the name of the file this parameter should be
        #   the absolute path specifying it's location
        # @param [Array] wf_search_array The set of precedence ordered relative directories to search for the wf in. A
        #   typical entry might look like `['files', '../../files', '../../weather']`
        # @param [Object] model The OpenStudio::Model object to parse, see #get_weather_file_from_osm
        # @return [String, nil] The weather file with precedence if defined, nil if not, and a failure if the wf is
        #   defined but not in the filesystem
        #
        def get_weather_file(directory, wf, wf_search_array, model, logger=nil)
          logger ||= ::Logger.new(STDOUT) unless logger
          if wf
            weather_file = get_weather_file_from_fs(directory, wf, wf_search_array, logger)
            fail 'Could not locate the weather file in the filesystem. Please see the log' if weather_file == false
          end
          weather_file = get_weather_file_from_osm(model, logger) if weather_file == nil
          fail 'Could not locate the weather file in the filesystem. Please see the log' if weather_file == false
          Workflow.logger.warn 'The weather file could not be determined. Please see the log for details' unless weather_file
          weather_file
        end

        private

        # Returns the weather file from the model. If the weather file is defined in the model, then
        # it checks the file paths to check if the model exists. This allows for a user to upload a
        # weather file in a measure and then have the measure's path be used for the weather file.
        #
        # @todo (rhorsey) verify the description of this method, as it seems suspect
        # @param [Object] model The OpenStudio::Model object to retrieve the weather file from
        # @return [nil,false, String] If the result is nil the weather file was not defined in the model, if the result
        #   is false the weather file was set but could not be found on the filesystem, if a string the weather file was
        #   defined and it's existence verified
        #
        def get_weather_file_from_osm(model, logger)
          wf = nil
          # grab the weather file out of the OSM if it exists
          if model.weatherFile.empty?
            logger.warn 'No weather file defined in the model'
          else
            p = model.weatherFile.get.path.get.to_s.gsub('file://', '')
            if File.exist? p
              wf = File.absolute_path(p)
            else
              # this is the weather file from the OSM model
              wf = File.absolute_path(@model.weatherFile.get.path.get.to_s)
            end
            logger.info "The weather file path found in the model object: #{wf}"
            unless File.exist? wf
              logger.warn 'The weather file could not be found on the filesystem.'
              wf = false
            end
          end
          wf
        end

        # Returns the weather file defined in the OSW
        #
        # @param [String] directory The base directory to append all relative directories to
        # @param [String] wf The weather file being searched for. If not the name of the file this parameter should be
        #   the absolute path specifying it's location
        # @param [Array] wf_search_array The set of precedence ordered relative directories to search for the wf in. A
        #   typical entry might look like `['files', '../../files', '../../weather']`
        # @return [nil, false, String] If the result is nil the weather file was not defined in the workflow, if the
        #   result is false the weather file was set but could not be found on the filesystem, if a string the weather
        #   file was defined and it's existence verified. The order of precedence for paths is as follows: 1 - an
        #   absolute path defined in wf, 2 - the wf_search_array, should it be defined, joined with the weather file and
        #   appended to the directory, with each entry in the array searched until the wf is found
        #
        def get_weather_file_from_fs(directory, wf, wf_search_array, logger)
          fail "wf was defined as #{wf}. Please correct" unless wf
          weather_file = nil
          if Pathname.new(wf).absolute?
            weather_file = wf
          else
            wf_search_array.each do |wf_dir|
              logger.warn "The path #{wf_dir} does not exist" unless File.exists? File.join(directory, wf_dir)
              next unless File.exists? File.join(directory, wf_dir)
              if Dir.entries(File.join(directory, wf_dir)).include? wf
                weather_file = File.absolute_path(File.join(directory, wf_dir, wf))
                break
              end
            end
          end
          unless weather_file
            logger.warn 'The weather file was not found on the filesystem'
            return nil
          end
          logger.info "Weather file with precedence in the file system is #{weather_file}"
          unless File.exist? weather_file
            logger.warn 'The weather file could not be found on the filesystem'
            weather_file = false
          end
          weather_file
        end
      end
    end
  end
end
