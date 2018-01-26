########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require("openstudio/sketchup_plugin/lib/PluginUserScript")

module OpenStudio

  class PluginUserScriptRunner < OpenStudio::Ruleset::OSRunner

    def initialize
      super
      @user_scripts = []
    end

    # shows the user a message box with an ok button
    def okPrompt(text)
      UI.messagebox(text, MB_OK)
    end

    # prompts the user with a yes or no choice, returns true if user selects yes
    def yesNoPrompt(text)

      result = UI.messagebox(text, MB_YESNO)
      if result == 6 # Yes
        return true
      end

      return false
    end

    # gets user input using dialog boxes, etc
    # result is map of UserArgument name to UserArgument, result is empty if user cancels
    # eventually c++ class should have a default implementation of this using Qt widgets
    def getUserInput(arguments)

      open_path_arguments = []
      save_path_arguments = []
      simple_arguments = []
      prompts = []
      defaults = []
      list = []

      arguments.each do |argument|
        if argument.type.valueName == "Path"
          if argument.isRead
            open_path_arguments << argument
          else
            save_path_arguments << argument
          end
        else
          simple_arguments << argument
          prompts << argument.displayName
          if argument.hasDefaultValue
            if argument.type.valueName == "Choice"
              defaults << argument.defaultValueDisplayName
            else
              defaults << argument.defaultValueAsString
            end
          else
            defaults << String.new
          end
          if argument.type.valueName == "Choice"
            list << argument.choiceValueDisplayNames.join("|")
          elsif argument.type.valueName == "Boolean"
            list << "true|false"
          else
            list << String.new
          end
        end
      end

      result = OpenStudio::Ruleset::OSArgumentMap.new

      default_dir = Plugin.model_manager.model_interface.openstudio_dir
      if not default_dir
        default_dir = "."
      end

      open_path_arguments.each do |argument|

        dir = default_dir
        filename = "*.#{argument.extension}"
        default = String.new
        if argument.hasDefaultValue
          default = argument.defaultValueAsString
        end

        if not default.empty?

          if File.directory?(default)
            dir = default
          else
            dir = File.dirname(default)
          end

          if dir.empty? or dir == "."
            dir = default_dir
          end

          filename = File.basename(default, ".#{argument.extension}")
          if filename.empty?
            filename = "*.#{argument.extension}"
          else
            filename = "#{filename}.#{argument.extension}"
          end
        end

        path = UI.openpanel(argument.displayName, dir, filename)
        if not path
          if argument.required
            return OpenStudio::Ruleset::OSArgumentMap.new
          end
        else
          argument.setValue(path)
        end

        result[argument.name] = argument
      end

      if not simple_arguments.empty?
        input = UI.inputbox(prompts, defaults, list, "User input.")

        if not input
          return OpenStudio::Ruleset::OSArgumentMap.new
        end

        i = 0
        simple_arguments.each do |argument|

          if input[i].to_s.empty?
            if argument.required
              return OpenStudio::Ruleset::OSArgumentMap.new
            end
          else
            if argument.setValue(input[i].to_s)
              result[argument.name] = argument
            else
              return OpenStudio::Ruleset::OSArgumentMap.new
            end
          end

          i += 1
        end
      end

      save_path_arguments.each do |argument|

        dir = default_dir
        filename = "out.#{argument.extension}"
        default = argument.defaultValueAsString
        if not default.empty?

          if File.directory?(default)
            dir = default
          else
            dir = File.dirname(default)
          end

          if dir.empty? or dir == "."
            dir = default_dir
          end

          filename = File.basename(default, ".#{argument.extension}")
          if filename.empty?
            filename = "out.#{argument.extension}"
          else
            filename = "#{filename}.#{argument.extension}"
          end

        end

        path = UI.savepanel(argument.displayName, dir, filename)
        if not path
          if argument.required
            return OpenStudio::Ruleset::OSArgumentMap.new
          end
        else
          argument.setValue(path)
        end

        result[argument.name] = argument
      end

      return result
    end

    # tests if the given ModelObject is in the app's current selection
    def inSelection(modelObject)
      result = false
      if drawing_interface = modelObject.drawing_interface
        result = drawing_interface.in_selection?(Sketchup.active_model.selection)
      end
      return result
    end

    def registerError(message)
      begin
        super(message)
      rescue
      end
      okPrompt("Error: " + message)
    end

    def registerWarning(message)
      super(message)
      return true
    end

    def registerInfo(message)
      super(message)
      return true
    end

    def registerAsNotApplicable(message)
      super(message)
      okPrompt("Not applicable: " + message)
    end

    def registerInitialCondition(message)
      super(message)
    end

    def registerFinalCondition(message)
      super(message)
      okPrompt(message)
    end

    def load_user_scripts
      if not Plugin.read_pref("Disable OpenStudio User Scripts")
        files = Dir.glob(Plugin.dir + '/user_scripts/**/*.rb')
        files.each do |file|
          begin
            load file
          rescue
            puts "Failed to load user script #{file}"
            Plugin.log(OpenStudio::Error, "Failed to load user script #{file}")
          end
        end
      end
    end

    def discover_user_script_directory(current_directory, current_menu)

      # add user script to menu
      files = Dir.glob(current_directory + '/*.rb')
      files.each do |file|
        #puts file
        begin

          before_size = @user_scripts.length

          # loading file should call registerWithSketchUp which will add script to @user_scripts
          load file

          after_size = @user_scripts.length

          if after_size > before_size
            # a user script was added
            Plugin.menu_manager.add_user_script(@user_scripts[-1].name, current_menu)
          end

        rescue
          puts "Failed to load user script #{file}"
          Plugin.log(OpenStudio::Error, "Failed to load user script #{file}")
        end
      end

      # discover user scripts in dir
      dirs = Dir.glob(current_directory + '/*/')
      dirs.each do |dir|
        next if /resource[s]?/i.match(dir)
        next if /test[s]?/i.match(dir)

        menu = current_menu.add_submenu(File.basename(dir))
        discover_user_script_directory(dir, menu)
      end
    end

    def discover_user_scripts
      if not Plugin.read_pref("Disable OpenStudio User Scripts")
        starting_directory = Plugin.dir + '/user_scripts'
        starting_menu = Plugin.menu_manager.user_scripts_menu
        discover_user_script_directory(starting_directory, starting_menu)
      end
    end

    # add a user script
    def add_user_script(user_script)
      @user_scripts.each_index do |i|
        if @user_scripts[i].name == user_script.name
          @user_scripts[i] = user_script
          return
        end
      end

      @user_scripts << user_script
    end

    # get user scripts
    def user_scripts
      return @user_scripts
    end

    # run a user script, returns true if script runs with no errors
    def run_user_script(name)

      user_script = nil
      @user_scripts.each do |script|
        if script.name == name
          user_script = script
          break
        end
      end

      if not user_script
        return false
      end

      model_interface = Plugin.model_manager.model_interface

      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing

      # store starting render mode
      starting_rendermode = model_interface.materials_interface.rendering_mode

      # switch render mode to speed things up
      model_interface.materials_interface.rendering_mode = RenderWaiting

      error_msg = ""
      has_errors = false
      has_warnings = false
      was_canceled = true

      # open ruby console to show any errors
      #SKETCHUP_CONSOLE.show

      arguments = nil
      if user_script.is_a?(OpenStudio::Ruleset::ModelUserScript)
        arguments = user_script.arguments(model_interface.openstudio_model)
      end

      if user_script.is_a?(OpenStudio::Ruleset::ReportingUserScript)
        arguments = user_script.arguments()
      end

      narg = arguments.size
      arguments = getUserInput(arguments)

      # canceling passes back empty map
      if arguments.size == narg

        was_canceled = false

        begin

          error_msg += "Running #{user_script.name}\n"

          if user_script.is_a?(OpenStudio::Ruleset::ModelUserScript)
            user_script.run(model_interface.openstudio_model, self, arguments)
          elsif user_script.is_a?(OpenStudio::Ruleset::ReportingUserScript)
            user_script.run(self, arguments)
          end

        rescue => error

          has_errors = true
          error_msg += "Error occurred while running user script:\n"
          error_msg += "#{error.message} (#{error.class})\n\n"
          error_msg += "#{error.backtrace}\n"

        end

        result = self.result
        if not result.stepErrors.empty?
          has_errors = true
          error_msg += "Errors: \n"
          result.stepErrors.each { |error|
            error_msg += "  " + error + "\n"
          }
        end
        if not result.stepWarnings.empty?
          has_warnings = true
          error_msg += "Warnings: \n"
          result.stepWarnings.each { |warn|
            error_msg += "  " + warn + "\n"
          }
        end
        if not result.stepInfo.empty?
          # error_msg += "Info: "
          result.stepInfo.each { |info|
            # error_msg += "  " + info + "\n"
          }
          # error_msg += "\n"
        end

      end

      # switch render mode back to original
      proc = Proc.new {
        destroyProgressBar

        model_interface.materials_interface.rendering_mode = starting_rendermode

        # remove stray edges from the model
        #model_interface.check_openstudio_model

        if has_errors
          error_msg += "User script #{user_script.name} completed with errors\n\n"
        elsif has_warnings
          error_msg += "User script #{user_script.name} completed with warnings\n\n"
        else
          error_msg += "User script #{user_script.name} completed\n\n"
        end

        # for some reason "\n" is showing up as <br> in the ruby console
        error_msg.split("\n").each {|part| puts part}

        #UI.messagebox(error_msg, MB_MULTILINE)
      }
      Plugin.add_event( proc )

      # resume event processing
      Plugin.start_event_processing if event_processing_stopped

      return ((not was_canceled) and (not has_errors))
    end

    # create a progress bar
    def createProgressBar(text)
      @progress_dialog = ProgressDialog.new(text)
    end

    # update the progress bar, note value is from 0 to 100
    def updateProgress(value)
      if @progress_dialog
        @progress_dialog.setValue(value.to_i)
      end
    end

    # destroy the progress bar
    def destroyProgressBar
      if @progress_dialog
        @progress_dialog.destroy
      end
      @progress_dialog = nil
    end

  end

end
