module OpenStudio
  module Workflow
    module Util

      require_relative 'measure'
      require 'csv'
      
      # This module serves as a wrapper around various post-processing tasks used to manage outputs
      # @todo (rhorsey) ummmm. So some of this is pretty ugly. Since @dmacumber had ideas about this maybe he can figure
      #   out what to do about it all
      #
      module PostProcess

        # This method parses all sorts of stuff which something needs
        #
        # @param [String] run_dir The directory that the simulation was run in
        # @return [Hash, Hash] results and objective_function (which may be empty) are returned
        # @todo (rhorsey) fix the description
        #
        def run_extract_inputs_and_outputs(run_dir)
          # For xml, the measure attributes are in the measure_attributes_xml.json file
          # TODO: somehow pass the metadata around on which JSONs to suck into the database
          results = {}
          if File.exist? "#{run_dir}/measure_attributes_xml.json"
            h = JSON.parse(File.read("#{run_dir}/measure_attributes_xml.json"), symbolize_names: true)
            h = rename_hash_keys(h)
            results.merge! h
          end

          # Inputs are in the measure_attributes.json file
          if File.exist? "#{run_dir}/measure_attributes.json"
            h = JSON.parse(File.read("#{run_dir}/measure_attributes.json"), symbolize_names: true)
            h = rename_hash_keys(h)
            results.merge! h
          end

          # Inputs are in the reporting_measure_attributes.jsonfile
          if File.exist? "#{run_dir}/reporting_measure_attributes.json"
            h = JSON.parse(File.read("#{run_dir}/reporting_measure_attributes.json"), symbolize_names: true)
            h = rename_hash_keys(h)
            results.merge! h
          end

          # Initialize the objective function variable.
          objective_functions = {}
          if File.exist? "#{run_dir}/standard_report_legacy.json"
            h = JSON.parse(File.read("#{run_dir}/standard_report_legacy.json"), symbolize_names: true)
            h = rename_hash_keys(h)
            results[:standard_report_legacy] = h
          end

          Workflow.logger.info 'Saving the result hash to file'
          File.open("#{run_dir}/results.json", 'w') { |f| f << JSON.pretty_generate(results) }

          if @registry[:analysis]
            Workflow.logger.info 'Iterating over Analysis JSON Output Variables'
            # Save the objective functions to the object for sending back to the simulation executive

            if @analysis_json[:analysis] && @analysis_json[:analysis][:output_variables]
              @analysis_json[:analysis][:output_variables].each do |variable|
                # determine which ones are the objective functions (code smell: todo: use enumerator)
                if variable[:objective_function]
                  Workflow.logger.info "Looking for objective function #{variable[:name]}"
                  # TODO: move this to cleaner logic. Use ostruct?
                  k, v = variable[:name].split('.')

                  # look for the objective function key and make sure that it is not nil. False is an okay obj function.
                  if results.key?(k.to_sym) && !results[k.to_sym][v.to_sym].nil?
                    objective_functions["objective_function_#{variable[:objective_function_index] + 1}"] = results[k.to_sym][v.to_sym]
                    if variable[:objective_function_target]
                      Workflow.logger.info "Found objective function target for #{variable[:name]}"
                      objective_functions["objective_function_target_#{variable[:objective_function_index] + 1}"] = variable[:objective_function_target].to_f
                    end
                    if variable[:scaling_factor]
                      Workflow.logger.info "Found scaling factor for #{variable[:name]}"
                      objective_functions["scaling_factor_#{variable[:objective_function_index] + 1}"] = variable[:scaling_factor].to_f
                    end
                    if variable[:objective_function_group]
                      Workflow.logger.info "Found objective function group for #{variable[:name]}"
                      objective_functions["objective_function_group_#{variable[:objective_function_index] + 1}"] = variable[:objective_function_group].to_f
                    end
                  else
                    Workflow.logger.warn "No results for objective function #{variable[:name]}"
                    objective_functions["objective_function_#{variable[:objective_function_index] + 1}"] = Float::MAX
                    objective_functions["objective_function_target_#{variable[:objective_function_index] + 1}"] = nil
                    objective_functions["scaling_factor_#{variable[:objective_function_index] + 1}"] = nil
                    objective_functions["objective_function_group_#{variable[:objective_function_index] + 1}"] = nil
                  end
                end
              end
            end
          end

          return results, objective_functions
        end

        # Turn the eplustbl into a json and save it as 'standard_report_legacy'
        #
        # @param [String] run_dir The directory that the simulation was run in
        # @todo add deprication warning
        #
        def translate_csv_to_json(run_dir)
          if File.exist?("#{run_dir}/eplustbl.csv")
            Workflow.logger.info 'Translating EnergyPlus table CSV to JSON file'
            results = {}
            csv = CSV.read("#{run_dir}/eplustbl.csv")
            csv.transpose.each do |k, v|
              longname = k.gsub(/\(.*\)/, '').strip
              short_name = longname.downcase.tr(' ', '_')
              units = k.match(/\(.*\)/)[0].delete('(').delete(')')
              results[short_name.to_sym] = v.nil? ? nil : v.to_f
              results["#{short_name}_units".to_sym] = units
              results["#{short_name}_display_name".to_sym] = longname
            end

            Workflow.logger.info 'Saving results to json'

            # save out results
            File.open("#{run_dir}/standard_report_legacy.json", 'w') { |f| f << JSON.pretty_generate(results) }
          end
        end

        # Remove any invalid characters in the measure attribute keys. Periods and Pipes are the most problematic
        #   because mongo does not allow hash keys with periods, and the pipes are used in the map/reduce method that
        #   was written to speed up the data write in openstudio-server. Also remove any trailing underscores and spaces
        #
        # @param [Hash] hash Any hash with potentially problematic characters
        #
        def rename_hash_keys(hash)
          # @todo should we log the name changes?
          regex = /[|!@#\$%^&\*\(\)\{\}\\\[\];:'",<.>\/?\+=]+/

          rename_keys = lambda do |h|
            if Hash === h
              h.each_key do |key|
                if key.to_s =~ regex
                  Workflow.logger.warn "Renaming result key '#{key}' to remove invalid characters"
                end
              end
              Hash[h.map { |k, v| [k.to_s.gsub(regex, '_').squeeze('_').gsub(/[_\s]+$/, '').chomp.to_sym, rename_keys[v]] }]
            else
              h
            end
          end

          rename_keys[hash]
        end

        # A general post-processing step which could be made significantly more modular
        #
        # @param [String] run_dir
        # @todo (rhorsey) What is the @reports directory referring to?
        #
        def cleanup(run_dir, directory)
          # move any of the reporting file to the 'reports' directory for serverside access
          FileUtils.mkdir_p "#{directory}/reports"

          # try to find the energyplus result file
          eplus_html = "#{run_dir}/eplustbl.htm"
          unless File.exist? eplus_html
            eplus_html = Dir["#{directory}/*EnergyPlus*/eplustbl.htm"].last || nil
          end

          if eplus_html
            if File.exist? eplus_html
              # do some encoding on the html if possible
              html = File.read(eplus_html)
              html = html.force_encoding('ISO-8859-1').encode('utf-8', replace: nil)
              File.open("#{directory}/reports/eplustbl.html", 'w') { |f| f << html }
            end
          end

          # Also, find any "report*.*" files
          Dir["#{run_dir}/*/report*.*"].each do |report|
            # get the parent directory of the file and snake case it
            # do i need to force encoding on this as well?
            measure_class_name = File.basename(File.dirname(report)).to_underscore
            file_ext = File.extname(report)
            append_str = File.basename(report, '.*')
            new_file_name = "#{directory}/reports/#{measure_class_name}_#{append_str}#{file_ext}"
            FileUtils.copy report, new_file_name
          end

          # Remove empty directories in run folder
          Dir["#{run_dir}/*"].select { |d| File.directory? d }.select { |d| (Dir.entries(d) - %w(. ..)).empty? }.each do |d|
            Workflow.logger.info "Removing empty directory #{d}"
            Dir.rmdir d
          end

          paths_to_rm = []
          # paths_to_rm << Pathname.glob("#{run_dir}/*.osm")
          # paths_to_rm << Pathname.glob("#{run_dir}/*.idf") # keep the idfs
          # paths_to_rm << Pathname.glob("*.audit")
          # paths_to_rm << Pathname.glob("*.bnd")
          # paths_to_rm << Pathname.glob("#{run_dir}/*.eso")
          paths_to_rm << Pathname.glob("#{run_dir}/*.mtr")
          paths_to_rm << Pathname.glob("#{run_dir}/*.epw")
          paths_to_rm << Pathname.glob("#{run_dir}/*.mtd")
          paths_to_rm << Pathname.glob("#{run_dir}/*.rdd")
          paths_to_rm.each { |p| FileUtils.rm_rf(p) }
        end
      end
    end
  end
end
