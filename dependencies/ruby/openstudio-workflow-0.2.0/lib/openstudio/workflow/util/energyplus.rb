module OpenStudio
  module Workflow
    module Util

      # The methods needed to run simulations using EnergyPlus are stored here. See the run_simulation class for
      #   implementation details.
      # @todo Get Nick to explain how to handle the require clause here
      #
      module EnergyPlus

        ENERGYPLUS_REGEX = /^energyplus\D{0,4}$/i
        EXPAND_OBJECTS_REGEX = /^expandobjects\D{0,4}$/i

        # Find the installation directory of EnergyPlus linked to the OpenStudio version being used
        #
        # @return [String] Returns the path to EnergyPlus
        #
        def find_energyplus
          path = OpenStudio.getEnergyPlusDirectory.to_s
          fail 'Unable to find the EnergyPlus executable' unless File.exists? path
          path
        end

        # Does something
        #
        # @param [String] run_directory Directory to run the EnergyPlus simulation in
        # @param [Array] energyplus_files Array of files containing the EnergyPlus and ExpandObjects EXEs
        # @return [Void]
        #
        def clean_directory(run_directory, energyplus_files)
          Workflow.logger.info 'Removing any copied EnergyPlus files'
          energyplus_files.each do |file|
            if File.exist? file
              FileUtils.rm_f file
            end
          end

          paths_to_rm = []
          paths_to_rm << "#{run_directory}/packaged_measures"
          paths_to_rm << "#{run_directory}/Energy+.ini"
          paths_to_rm.each { |p| FileUtils.rm_rf(p) if File.exist?(p) }
        end

        # Prepare the directory to run EnergyPlus
        #
        # @param [String] run_directory Directory to copy the required EnergyPlus files to
        # @param [String] energyplus_path Path to the EnergyPlus EXE
        # @return [Array, file, file] Returns an array of strings of EnergyPlus files copied to the run_directory, the
        # ExpandObjects EXE file, and EnergyPlus EXE file
        #
        def prepare_energyplus_dir(run_directory, energyplus_path = nil)
          Workflow.logger.info "Copying EnergyPlus files to run directory: #{run_directory}"
          energyplus_path ||= find_energyplus
          Workflow.logger.info "EnergyPlus path is #{energyplus_path}"
          energyplus_files = []
          energyplus_exe, expand_objects_exe = nil
          Dir["#{energyplus_path}/*"].each do |file|
            next if File.directory? file
            next if File.extname(file).downcase =~ /.pdf|.app|.html|.gif|.txt|.xlsx/

            dest_file = "#{run_directory}/#{File.basename(file)}"
            energyplus_files << dest_file

            # @todo (rhorsey) don't need to do this copy - DLM
            energyplus_exe = File.basename(dest_file) if File.basename(dest_file) =~ ENERGYPLUS_REGEX
            expand_objects_exe = File.basename(dest_file) if File.basename(dest_file) =~ EXPAND_OBJECTS_REGEX
            FileUtils.copy file, dest_file
          end

          fail "Could not find EnergyPlus executable in #{energyplus_path}" unless energyplus_exe
          fail "Could not find ExpandObjects executable in #{energyplus_path}" unless expand_objects_exe
          
          Workflow.logger.info "EnergyPlus executable path is #{energyplus_exe}"
          Workflow.logger.info "ExpandObjects executable path is #{expand_objects_exe}"

          return energyplus_files, energyplus_exe, expand_objects_exe
        end

        # Configures and executes the EnergyPlus simulation and checks to see if the simulation was successful
        #
        # @param [String] run_directory Directory to execute the EnergyPlus simulation in. It is assumed that this
        #   directory already has the IDF and weather file in it
        # @param [String] energyplus_path (nil) Optional path to override the default path associated with the
        #   OpenStudio package being used
        # @return [Void]
        #
        def call_energyplus(run_directory, energyplus_path = nil)
          begin
            current_dir = Dir.pwd
            energyplus_path ||= find_energyplus
            Workflow.logger.info "EnergyPlus path is #{energyplus_path}"
            energyplus_files, energyplus_exe, expand_objects_exe = prepare_energyplus_dir(run_directory, energyplus_path)
            Dir.chdir(run_directory)
            Workflow.logger.info "Starting simulation in run directory: #{Dir.pwd}"

            command = Util.popen_command("./#{expand_objects_exe}")
            Workflow.logger.info "Running command '#{command}'"
            File.open('stdout-expandobject', 'w') do |file|
              IO.popen(command) do |io|
                while (line = io.gets)
                  file << line
                end
              end
            end

            # Check if expand objects did anythying
            if File.exist? 'expanded.idf'
              FileUtils.mv('in.idf', 'pre-expand.idf', force: true) if File.exist?('in.idf')
              FileUtils.mv('expanded.idf', 'in.idf', force: true)
            end

            # create stdout
            command = Util.popen_command("./#{energyplus_exe} 2>&1")
            Workflow.logger.info "Running command '#{command}'"
            File.open('stdout-energyplus', 'w') do |file|
              IO.popen(command) do |io|
                while (line = io.gets)
                  file << line
                end
              end
            end
            r = $?

            Workflow.logger.info "EnergyPlus returned '#{r}'"
            unless r == 0
              Workflow.logger.warn 'EnergyPlus returned a non-zero exit code. Check the stdout-energyplus log.'
            end

            if File.exist? 'eplusout.end'
              f = File.read('eplusout.end').force_encoding('ISO-8859-1').encode('utf-8', replace: nil)
              warnings_count = f[/(\d*).Warning/, 1]
              error_count = f[/(\d*).Severe.Errors/, 1]
              Workflow.logger.info "EnergyPlus finished with #{warnings_count} warnings and #{error_count} severe errors"
              if f =~ /EnergyPlus Terminated--Fatal Error Detected/
                fail 'EnergyPlus Terminated with a Fatal Error. Check eplusout.err log.'
              end
            else
              fail 'EnergyPlus failed and did not create an eplusout.end file. Check the stdout-energyplus log.'
            end

            if File.exist? 'eplusout.err'
              eplus_err = File.read('eplusout.err').force_encoding('ISO-8859-1').encode('utf-8', replace: nil)
              if eplus_err =~ /EnergyPlus Terminated--Fatal Error Detected/
                fail 'EnergyPlus Terminated with a Fatal Error. Check eplusout.err log.'
              end
            end
          rescue => e
            log_message = "#{__FILE__} failed with #{e.message}, #{e.backtrace.join("\n")}"
            Workflow.logger.error log_message
            raise log_message
          ensure
            Workflow.logger.info "Ensuring 'clean' directory"
            clean_directory(run_directory, energyplus_files)

            Dir.chdir(current_dir)
            Workflow.logger.info 'EnergyPlus Completed'
          end

        end

        # Run this code before running EnergyPlus to make sure the reporting variables are setup correctly
        #
        # @param [String] idf_filename The name of the IDF file to be simulated
        # @return [Void]
        #
        def energyplus_preprocess(idf_filename)
          Workflow.logger.info 'Running EnergyPlus Preprocess'
          # @todo (rhorsey) need to add report requests for all reporting measures - DLM
          # https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/src/pat_app/Measures/ReportRequest/measure.rb

          fail "Could not find IDF file in run directory (#{idf_filename})" unless File.exist? idf_filename

          new_objects = []

          idf = OpenStudio::IdfFile.load(idf_filename).get
          # save the pre-preprocess file
          File.open("#{File.dirname(idf_filename)}/pre-preprocess.idf", 'w') { |f| f << idf.to_s }

          needs_sqlobj = idf.getObjectsByType('Output:SQLite'.to_IddObjectType).empty?

          needs_monthlyoutput = idf.getObjectsByName('Building Energy Performance - Natural Gas').empty? ||
              idf.getObjectsByName('Building Energy Performance - Electricity').empty? ||
              idf.getObjectsByName('Building Energy Performance - District Heating').empty? ||
              idf.getObjectsByName('Building Energy Performance - District Cooling').empty?

          # this is a workaround for issue #1699 -- remove when 1699 is closed.
          new_objects << 'Output:Variable,*,Zone Air Temperature,Hourly;'
          new_objects << 'Output:Variable,*,Zone Air Relative Humidity,Daily;'
          new_objects << 'Output:Variable,*,Site Outdoor Air Drybulb Temperature,Monthly;'
          new_objects << 'Output:Variable,*,Site Outdoor Air Wetbulb Temperature,Timestep;'

          if needs_sqlobj
            Workflow.logger.info 'Adding SQL Output to IDF'
            new_objects << '
        Output:SQLite,
        SimpleAndTabular;         ! Option Type
        '
          end

          if needs_monthlyoutput
            monthly_report_idf = File.join(File.dirname(__FILE__), '../jobs/resources/monthly_report.idf')
            idf_file = OpenStudio::IdfFile.load(File.read(monthly_report_idf), 'EnergyPlus'.to_IddFileType).get
            idf.addObjects(idf_file.objects)
          end

          # These are supposedly needed for the calibration report
          new_objects << 'Output:Meter:MeterFileOnly,Gas:Facility,Daily;'
          new_objects << 'Output:Meter:MeterFileOnly,Electricity:Facility,Timestep;'
          new_objects << 'Output:Meter:MeterFileOnly,Electricity:Facility,Daily;'

          # Always add in the timestep facility meters
          new_objects << 'Output:Meter,Electricity:Facility,Timestep;'
          new_objects << 'Output:Meter,Gas:Facility,Timestep;'
          new_objects << 'Output:Meter,DistrictCooling:Facility,Timestep;'
          new_objects << 'Output:Meter,DistrictHeating:Facility,Timestep;'

          new_objects.each do |obj|
            object = OpenStudio::IdfObject.load(obj).get
            idf.addObject(object)
          end

          # save the file
          File.open(idf_filename, 'w') { |f| f << idf.to_s }

          Workflow.logger.info 'Finished EnergyPlus Preprocess'
        end
      end
    end
  end
end
