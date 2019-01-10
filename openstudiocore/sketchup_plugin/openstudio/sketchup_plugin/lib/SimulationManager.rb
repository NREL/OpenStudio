########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require("fileutils")

module OpenStudio

  class SimulationManager

    attr_reader :run_manager, :status_widget

    def initialize
      @run_manager = OpenStudio::Runmanager::RunManager.new
      @status_widget = OpenStudio::Runmanager::JobStatusWidget.new(@run_manager)
      @status_widget.setWindowTitle(OpenStudio::toQString("Simulation Status"))
      @modeltoidf = nil
      @expandobjects = nil
      @energyplus = nil
      @readvars = nil
    end

    def run_simulation

      model_interface = Plugin.model_manager.model_interface
      model_interface.clear_errors

      # call translate model to idf first, later these warnings should be shown in run status widget instead?
      workspace, errors, warnings = Plugin.model_manager.model_to_workspace(model_interface.openstudio_model)

      if errors.empty?
        errors = nil
      else
        model_interface.add_error("Errors occurred on translation to EnergyPlus\n\n", false)
        errors.each {|error| model_interface.add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if warnings.empty?
        warnings = nil
      else
        model_interface.add_error("Warnings occurred on translation to EnergyPlus\n\n", false)
        warnings.each {|warning| model_interface.add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end

      if Plugin.read_pref('Show Errors on Idf Translation') and errors
        model_interface.show_errors
        result = UI.messagebox("Errors occurred on translation to EnergyPlus, continue simulation?", MB_YESNO)
        if result == 7 # NO
          return
        end
      elsif Plugin.read_pref('Show Warnings on Idf Translation') and (errors or warnings)
        model_interface.show_errors
        result = UI.messagebox("Warnings occurred on translation to EnergyPlus, continue simulation?", MB_YESNO)
        if result == 7 # NO
          return
        end
      end

      # check that new file has been saved
      if not Plugin.command_manager.save_openstudio
        UI.messagebox("OpenStudio model must be saved before simulation.")
        return(false)
      end

      if (not File.exists?(Plugin.energyplus_path))
        UI.messagebox("Cannot locate the EnergyPlus engine.  Correct the EXE path and try again.")
        Plugin.dialog_manager.show(PreferencesInterface)
        return(false)
      end

      idd_path = Plugin.energyplus_dir + "/Energy+.idd"
      if (not File.exists?(idd_path))
        UI.messagebox("Cannot locate the input data dictionary (IDD) in the EnergyPlus directory.  Correct the EXE path and try again.")
        Plugin.dialog_manager.show(PreferencesInterface)
        return(false)
      end

      expandobjects_path = ''
      if (Plugin.platform == Platform_Windows)
        expandobjects_path = Plugin.energyplus_dir + '/ExpandObjects.exe'
      else
        expandobjects_path = Plugin.energyplus_dir + '/expandobjects'
      end
      if (not File.exists?(expandobjects_path))
        UI.messagebox("Cannot locate ExpandObjects in the EnergyPlus directory.  Correct the EXE path and try again.")
        Plugin.dialog_manager.show(PreferencesInterface)
        return(false)
      end

      readvars_path = ""
      if (Plugin.platform == Platform_Windows)
        readvars_path = Plugin.energyplus_dir + '/PostProcess/ReadVarsESO.exe'
        if (not File.exists?(readvars_path))
          readvars_path = Plugin.energyplus_dir + '/readvars.exe'
        end
      else
        readvars_path = Plugin.energyplus_dir + '/readvars'
      end
      if (not File.exists?(readvars_path))
        UI.messagebox("Cannot locate ReadVarsESO in the EnergyPlus directory.  Correct the EXE path and try again.")
        Plugin.dialog_manager.show(PreferencesInterface)
        return(false)
      end

      # directory to run simulation in
      openstudio_path = model_interface.openstudio_path
      openstudio_dir = model_interface.openstudio_dir
      run_dir = OpenStudio::Path.new("run")
      rmdb_path = model_interface.model_temp_dir / OpenStudio::Path.new("resources/run.db")

      # run in the model path
      #base_dir = OpenStudio::Path.new(openstudio_dir) / OpenStudio::Path.new(OpenStudio::Path.new(model_interface.openstudio_path).stem)
      #search_dir = OpenStudio::Path.new("..") / OpenStudio::Path.new(OpenStudio::Path.new(model_interface.openstudio_path).stem)

      # run in the temp dir
      base_dir = model_interface.model_temp_dir / OpenStudio::Path.new("resources")
      search_dir = OpenStudio::Path.new("../in.osm")

      # pause run manager
      @run_manager.setPaused(true)

      # delete current jobs
      jobs = @run_manager.getJobs()
      jobs.each do |job|
        @run_manager.remove(job)
      end

      # load jobs from the temp dir
      @run_manager.loadJobs(rmdb_path)

      jobs = @run_manager.getJobs()

      # ModelToIdf
      # ExpandObjects
      # EnergyPlus
      # ReadVars (if rvi file is given)

      @modeltoidf = nil
      @expandobjects = nil
      @energyplus = nil
      @readvars = nil

      jobs.each do |job|
        jobType = job.jobType

        if jobType == "ModelToIdf".to_JobType
          @modeltoidf = job
          @modeltoidf.setBasePath(base_dir)
        elsif jobType == "ExpandObjects".to_JobType
          @expandobjects = job
        elsif jobType == "EnergyPlus".to_JobType
          @energyplus = job

          if model_interface.get_attribute("RUN_READVARS")
            rvi_files = OpenStudio::Runmanager::Files.new()
            if rvi_file = model_interface.get_attribute("RVI_FILE")
              rvi_files.append(OpenStudio::Runmanager::FileInfo.new(OpenStudio::Path.new(rvi_file), "rvi"))
            end
            @readvars = OpenStudio::Runmanager::JobFactory::createReadVarsJob(tools, OpenStudio::Runmanager::JobParams.new(), OpenStudio::Runmanager::Files.new())
            @energyplus.addChild(@readvars)
          end

        else
        end
      end

      # close sql file if open
      sql_file = model_interface.openstudio_model.sqlFile
      if not sql_file.empty?
        sql_file.get.close
        model_interface.openstudio_model.resetSqlFile
        model_interface.results_interface = ResultsInterface.new(model_interface)
        model_interface.results_interface.update
        Plugin.dialog_manager.update(ColorScaleInterface)
        model_interface.request_paint
      end

      # create the "set of tools" needed
      #tools = OpenStudio::Runmanager::Tools.new()
      #tools.append(OpenStudio::Runmanager::ToolInfo.new("energyplus", OpenStudio::Runmanager::ToolVersion.new(7,2), OpenStudio::Path.new(Plugin.energyplus_path)))
      #tools.append(OpenStudio::Runmanager::ToolInfo.new("expandobjects", OpenStudio::Runmanager::ToolVersion.new(), OpenStudio::Path.new(expandobjects_path)))
      #tools.append(OpenStudio::Runmanager::ToolInfo.new("readvars", OpenStudio::Runmanager::ToolVersion.new(), OpenStudio::Path.new(readvars_path)))

      # unpause run manager
      @run_manager.setPaused(false)

      # show status dialog
      @status_widget.setWorkflow(@modeltoidf)
      @status_widget.show
      @status_widget.activateWindow

      # add a proc to check for completion
      proc = Proc.new { self.check_complete }
      Plugin.add_event( proc )
    end


    def on_completion
      #@run_manager.hideStatusDialog

      @run_manager.setPaused(true)

      model_interface = Plugin.model_manager.model_interface

      errors = ""
      @modeltoidf.outputFiles().each do |f|
        puts "modeltoidf created #{f.fullPath.to_s}"
      end
      if @expandobjects
        @expandobjects.outputFiles().each do |f|
          puts "expandobjects created #{f.fullPath.to_s}"
        end
      end
      @energyplus.outputFiles().each do |f|
        puts "energyplus created #{f.fullPath.to_s}"
        if f.filename == "eplusout.sql"
          model_interface.results_interface.output_file_path = f.fullPath.to_s
        end
      end
      if @readvars
        @readvars.outputFiles().each do |f|
          puts "readvars created #{f.fullPath.to_s}"
        end
      end

      # copy files over after simulation
      model_interface.save_model_temp_dir

      if success = (@modeltoidf.errors.succeeded and @energyplus.errors.succeeded)
        # attach new results
        #Plugin.model_manager.model_interface.openstudio_model.setSqlFile(sql_path)
      else
        UI.messagebox("Simulation failed, please check error file for details")
      end

      @modeltoidf = nil
      @expandobjects = nil
      @energyplus = nil
      @readvars = nil
    end

    def busy?
      return(@run_manager.workPending)
    end

    def check_complete
      if (not self.busy?)
        self.on_completion
      else
        # check again next time
        proc = Proc.new { self.check_complete }
        Plugin.add_event( proc )
      end
    end

  end

end
