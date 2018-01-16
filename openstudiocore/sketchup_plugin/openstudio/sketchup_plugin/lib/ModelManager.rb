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

require("openstudio/sketchup_plugin/lib/dialogs/ProgressDialog")
require("openstudio/sketchup_plugin/lib/interfaces/ModelInterface")
require("openstudio/sketchup_plugin/lib/observers/AppObserver")

module OpenStudio

  # The ModelManager is an App level class.  Its primary job is to track all ModelInterfaces and delete ModelInterfaces when the associated
  # SketchUp Model is removed.
  class ModelManager

    attr_accessor :model_interfaces, :deleted_skp_models, :dont_zoom

    def initialize
      @model_interfaces = []
      @deleted_skp_models = Set.new
    end

    def destroy
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      Plugin.dialog_manager.close_all

      @model_interfaces.each { |model_interface| model_interface.detach_openstudio_model }

      @model_interfaces.clear
    end

    def shutdown
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      Plugin.dialog_manager.close_all

      @model_interfaces.each { |model_interface| model_interface.shutdown_openstudio_model }

      @model_interfaces.clear
    end

    def inspect
      return(to_s)
    end

    def model_interface(skp_model=nil)
      if skp_model.nil?
        skp_model = Sketchup.active_model
      end
      result = @model_interfaces.find { |mi| (mi.skp_model == skp_model) }
      return result
    end

    def purge_invalid_model_interfaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      to_delete = []
      @model_interfaces.each do |model_interface|
        if Plugin.platform == Platform_Windows
          # only one model is valid at a time, valid? does not seem to work on Windows
          to_delete << model_interface if (model_interface.skp_model != Sketchup.active_model)
        else
          #valid? does work on Mac
          to_delete << model_interface if (not model_interface.skp_model.valid?)
        end
      end
      #puts "to_delete #{to_delete}"
      to_delete.each {|model_interface| delete_model_interface(model_interface)}
    end

    # this method cannot fail
    def new_from_skp_model(skp_model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # DLM: previously, we tried to find the linked OSM, open it, and reattach it

      openstudio_path = skp_model.openstudio_path
      openstudio_entities = skp_model.openstudio_entities

      if (openstudio_path && !openstudio_path.empty?) || (openstudio_entities.size > 0)

        message = "Removing previously linked OpenStudio content."
        message += "\n\nOpen OpenStudio model at '#{openstudio_path}' to restore content."
        UI.messagebox(message, MB_OK)

        # remove all OpenStudio content so user is not confused
        skp_model.start_operation("Remove all OpenStudio Content", true)
        skp_model.delete_openstudio_entities
        skp_model.commit_operation
      end

      new_from_path(skp_model, Plugin.minimal_template_path)
    end

    # this method cannot fail
    def new_from_example_model(skp_model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_interfaces.reject! {|mi| (mi.skp_model == skp_model) }
      purge_invalid_model_interfaces
      GC.start

      openstudio_model = OpenStudio::Model::exampleModel

      attach_openstudio_model(openstudio_model, skp_model, nil, false, true)

    end

    # this method cannot fail, if path cannot be loaded then will fall back on Plugin.minimal_template_path
    def new_from_path(skp_model, path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      success = false

      if not path.nil? and not path.empty?
        @dont_zoom = true
        success = open_openstudio(path, skp_model, false, false)
        Plugin.log(OpenStudio::Debug, "new_from_path: #{path}, #{success}")
        @dont_zoom = false
      end

      if not success
        # user path is bad, reset to minimal template
        path = Plugin.minimal_template_path
        @dont_zoom = true
        success = open_openstudio(path, skp_model, false, false)
        Plugin.log(OpenStudio::Debug, "new_from_path: #{path}, #{success}")
        @dont_zoom = false
      end

      if not success

        # minimal template path is bad
        UI.messagebox("Minimal template path #{path} is not valid, your OpenStudio installation has been corrupted, please reinstall.")

        Plugin.log(OpenStudio::Fatal, "new_from_path: failed to load openstudio model from template at #{path}")
        raise "new_from_path: failed to load openstudio model from template at #{path}"
      end
    end

    # this method may fail
    def open_openstudio(path, skp_model, save_path = true , do_zoom = true)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      success = false

      show_version_dialog = save_path
      openstudio_model = model_from_openstudio_path(path, show_version_dialog)

      if openstudio_model

        attach_openstudio_model(openstudio_model, skp_model, path, save_path, do_zoom)

        success = true
      end

      Plugin.log(OpenStudio::Debug, "open_openstudio: #{path}, #{success}")

      return(success)
    end

    # this method cannot fail
    def attach_openstudio_model(openstudio_model, skp_model, path = nil, save_path = false , do_zoom = true, errors = nil, warnings = nil, untranslated_idf_objects = [])
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_interfaces.each do |mi|
        if (mi.skp_model == skp_model)
          delete_model_interface(mi)
        end
      end

      # may not have some required objects, add now
      openstudio_model.getFacility
      openstudio_model.getBuilding

      # this should be the only place in the code where ModelInterface.new is called
      if save_path
        model_interface = ModelInterface.new(skp_model, openstudio_model, path)
      else
        model_interface = ModelInterface.new(skp_model, openstudio_model)
      end

      @model_interfaces << model_interface

      model_interface.untranslated_idf_objects = untranslated_idf_objects

      if not untranslated_idf_objects.empty?
        model_interface.add_error("OpenStudio could not import all EnergyPlus Idf objects into your model, #{untranslated_idf_objects.size} objects could not be translated\n\n", false)
        model_interface.add_error("To export these objects please use \n'Plugins->OpenStudio->Export->Export Untranslated EnergyPlus Idf'\n\n", false)
      end

      if errors and not errors.empty?
        model_interface.add_error("The following errors occurred on import.\n\n", false)
        errors.each {|error| model_interface.add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if warnings and not warnings.empty?
        model_interface.add_error("The following warnings occurred on import.\n\n", false)
        warnings.each {|warning| model_interface.add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end

      model_interface.attach_openstudio_model

      if do_zoom
        skp_model.active_view.zoom_extents
      end

    end

    def delete_model_interface(model_interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # DLM: store reference to skp_model to prevent this from being garbage collected below
      # Fixes Bug 781 - If you create a new SketchUp file or Open an existing one it fails to make a new open studio model
      # if these skp_models are allowed to be garbage collected then SketchUp sometimes returns a deleted model when calling SketchUp.active_model
      # On Windows these deleted_skp_models will all return the same guid as the current model
      @deleted_skp_models << model_interface.skp_model

      new_model_interfaces = @model_interfaces.reject! {|mi| mi == model_interface}

      if model_interface.skp_model.valid?
        # this will be called on Windows
        model_interface.detach_openstudio_model
      else
        # this will be called on Mac, it results in a potential memory leak
        # the sketchup model has already been deleted
        # calling detach_openstudio_model will call valid_entity? on each entity
        # this causes a crash on Mac (specifically when calling valid_entity? on faces)
      end

      @model_interfaces = new_model_interfaces
      GC.start
    end

    # return a valid openstudio model from path or nil
    def model_from_openstudio_path(path, show_version_dialog = true)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = nil

      if (path.nil?)
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "model_from_openstudio_path: path is nil")

      elsif (not File.exist?(path))
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "model_from_openstudio_path: #{path} does not exist")

      else
        # load model

        versionTranslator = OpenStudio::OSVersion::VersionTranslator.new
        versionTranslator.setAllowNewerVersions(false)
        currentVersion = OpenStudio::VersionString.new(OpenStudio::openStudioVersion())

        model = nil
        if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
          progress_dialog = ProgressDialog.new("Reading OpenStudio Model")
          model = versionTranslator.loadModel(OpenStudio::Path.new(path), progress_dialog)
          progress_dialog.destroy
        else
          model = versionTranslator.loadModel(OpenStudio::Path.new(path))
        end

        errors = versionTranslator.errors
        warnings = versionTranslator.warnings

        if not model.empty?
          result = model.get

          if (versionTranslator.originalVersion() != currentVersion)
            message = "Model at '" + path.to_s + "' updated from " + versionTranslator.originalVersion().str() + " to " + currentVersion.str() + "."
            if show_version_dialog
              UI.messagebox(message)
            else
              puts message
            end
          end

        else

          if show_version_dialog
            message = "Failed to load file at " + path.to_s + ".\n\n"
            errors.each {|error| message += "Error: #{error.logMessage}\n\n"}
            warnings.each {|warning| message += "Warning: #{warning.logMessage}\n\n"}
            UI.messagebox(message, MB_MULTILINE)
          end

        end

      end

      return(result)
    end

    # convert a model to a workspace
    def model_to_workspace(model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      translator = OpenStudio::EnergyPlus::ForwardTranslator.new

      workspace = nil
      if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
        progress_dialog = ProgressDialog.new("Translating OpenStudio to EnergyPlus")
        workspace = translator.translateModel(model, progress_dialog)
        progress_dialog.destroy
      else
        workspace = translator.translateModel(model)
      end

      errors = translator.errors
      warnings = translator.warnings

      return workspace, errors, warnings
    end

    # convert a workspace to a model
    def model_from_workspace(workspace)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      translator = OpenStudio::EnergyPlus::ReverseTranslator.new

      model = nil
      if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
        progress_dialog = ProgressDialog.new("Translating EnergyPlus to OpenStudio")
        model = translator.translateWorkspace(workspace, progress_dialog)
        progress_dialog.destroy
      else
        model = translator.translateWorkspace(workspace)
      end

      errors = translator.errors
      warnings = translator.warnings
      untranslated_idf_objects = translator.untranslatedIdfObjects

      return model, errors, warnings, untranslated_idf_objects
    end

    # load an Idf from path and return as a workspace or nil
    def workspace_from_idf_path(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = nil

      if (path.nil?)
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "workspace_from_idf_path: path is nil")

      elsif (not File.exist?(path))
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "workspace_from_idf_path: #{path} does not exist")

      else

        # load idf and convert to a workspace

        idf = nil
        if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
          progress_dialog = ProgressDialog.new("Loading EnergyPlus Idf")
          idf = OpenStudio::IdfFile::load(OpenStudio::Path.new(path), "EnergyPlus".to_IddFileType, progress_dialog)
          progress_dialog.destroy
        else
          idf = OpenStudio::IdfFile::load(OpenStudio::Path.new(path), "EnergyPlus".to_IddFileType)
        end

        # energyplus idfs may not be draft level strictness, eventually need a fixer
        if idf.empty?

          Sketchup.status_text = "Could not read file at #{path}"
          Plugin.log(OpenStudio::Error, "model_from_idf_path: could not read file at #{path}")

        else

          Plugin.log(OpenStudio::Debug, "workspace_from_idf_path: read file at #{path}")

          if not idf.get.isValid("Draft".to_StrictnessLevel)
            Plugin.log(OpenStudio::Error, "model_from_idf_path: file at #{path} is not valid to draft strictness \n #{idf.get.validityReport('Draft'.to_StrictnessLevel)}")

            model_interface.add_error("File at #{path} is not valid to draft strictness \n #{idf.get.validityReport('Draft'.to_StrictnessLevel)}")
            model_interface.show_errors
          end

          workspace = nil
          if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
            progress_dialog = ProgressDialog.new("Creating EnergyPlus Workspace")
            workspace = OpenStudio::Workspace.new("None".to_StrictnessLevel, "EnergyPlus".to_IddFileType)
            #workspace = OpenStudio::Workspace.new("Draft".to_StrictnessLevel, "EnergyPlus".to_IddFileType)
            workspace.connectProgressBar(progress_dialog)
            added_objects = workspace.addObjects(idf.get.objects)
            workspace.disconnectProgressBar(progress_dialog)
            progress_dialog.destroy
          else
            workspace = OpenStudio::Workspace.new(idf.get, "None".to_StrictnessLevel)
            #workspace = OpenStudio::Workspace.new(idf.get, "Draft".to_StrictnessLevel)
          end

          Plugin.log(OpenStudio::Debug, "workspace_from_idf_path: added objects size = #{added_objects.size}")

          result = workspace
        end
      end

      return(result)
    end

    # load a gbXML from path and convert to model
    def model_from_gbXML_path(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = nil

      if (path.nil?)
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "model_from_gbXML_path: path is nil")

      elsif (not File.exist?(path))
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "model_from_gbXML_path: #{path} does not exist")

      else

        translator = OpenStudio::GbXML::GbXMLReverseTranslator.new

        model = nil
        if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
          progress_dialog = ProgressDialog.new("Translating gbXML")
          model = translator.loadModel(OpenStudio::Path.new(path), progress_dialog)
          progress_dialog.destroy
        else
          model = translator.loadModel(OpenStudio::Path.new(path))
        end

        errors = translator.errors
        warnings = translator.warnings

        if model.empty?
          Sketchup.status_text = "Could not read file at #{path}"
          Plugin.log(OpenStudio::Error, "model_from_gbXML_path: could not read file at #{path}")
        else
          result = model.get
        end
      end

      return result, errors, warnings
    end

    # load a sdd from path and convert to model
    def model_from_sdd_path(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = nil

      if (path.nil?)
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "model_from_sdd_path: path is nil")

      elsif (not File.exist?(path))
        Sketchup.status_text = "Could not read file at #{path}"
        Plugin.log(OpenStudio::Error, "model_from_sdd_path: #{path} does not exist")

      else

        translator = OpenStudio::SDD::SddReverseTranslator.new

        model = nil
        if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
          progress_dialog = ProgressDialog.new("Translating SDD")
          model = translator.loadModel(OpenStudio::Path.new(path), progress_dialog)
          progress_dialog.destroy
        else
          model = translator.loadModel(OpenStudio::Path.new(path))
        end

        errors = translator.errors
        warnings = translator.warnings

        if model.empty?
          Sketchup.status_text = "Could not read file at #{path}"
          Plugin.log(OpenStudio::Error, "model_from_sdd_path: could not read file at #{path}")
        else
          result = model.get
        end
      end

      return result, errors, warnings
    end

=begin
    # import an entire model from gbXML into active_model, will close existing openstudio_model
    def import_gbXML(path)
      success = false

      if (path.nil?)
        puts "ModelManager.import_gbXML:  nil path"
          UI.messagebox("Please enter a valid file path")
      elsif (not File.exist?(path))
        puts "ModelManager.import_gbXML:  bad path"
          UI.messagebox("#{path} appears to be a bad path")
      else

        model_interface = self.model_interface
        if model_interface
          self.delete_model_interface(model_interface)
        end

        model_interface = ModelInterface.new(Sketchup.active_model)
        self.add_model_interface(model_interface)

        openstudio_model = model_from_gbXML_path(path)

        if openstudio_model
          # attach the openstudio model to the skp model
          model_interface.openstudio_model = openstudio_model
          model_interface.attach_openstudio_model

          if not @dont_zoom
            model_interface.skp_model.active_view.zoom_extents
          end

          model_interface.openstudio_path = nil # import does not set path

          model_interface.construction_manager.check_defaults

          success = true
        end
      end

      # This is probably not the optimal place for this.
      # Trying to keep GUI out of this class.
      if (success)
        Plugin.dialog_manager.update_all if (Plugin.dialog_manager)
        # show errors
        if (model_interface.unviewed_errors)
          model_interface.show_errors
        end
      else
         # don't trigger UI alert here, alreday exists elsewhere
      end

      return(success)
    end

    # import an entire model from EnergyPlus idf into active_model, will close existing openstudio_model
    def import_idf(path)
      success = false

      if (path.nil?)
        puts "ModelManager.import_idf:  nil path"
          UI.messagebox("Please enter a valid file path")
     elsif (not File.exist?(path))
        puts "ModelManager.import_idf:  bad path"
          UI.messagebox("#{path} appears to be a bad path")
      else

        model_interface = self.model_interface
        if model_interface
          self.delete_model_interface(model_interface)
        end

        model_interface = ModelInterface.new(Sketchup.active_model)
        self.add_model_interface(model_interface)

        openstudio_model = model_from_idf_path(path)

        if openstudio_model
          # attach the openstudio model to the skp model
          model_interface.openstudio_model = openstudio_model
          model_interface.attach_openstudio_model

          if not @dont_zoom
            model_interface.skp_model.active_view.zoom_extents
          end

          model_interface.openstudio_path = nil # import does not set path

          model_interface.construction_manager.check_defaults

          success = true
        end
      end

      # This is probably not the optimal place for this.
      # Trying to keep GUI out of this class.
      if (success)
        Plugin.dialog_manager.update_all if (Plugin.dialog_manager)
        # show errors
        if (model_interface.unviewed_errors)
          model_interface.show_errors
        end
      else
         # don't trigger UI alert here, alreday exists elsewhere
      end

      return(success)
    end

    # import an entire OpenStudio Model, will close existing openstudio_model
    def import_openstudio(path)
     success = false

      if (path.nil?)
        puts "ModelManager.import_openstudio:  nil path"
          UI.messagebox("Please enter a valid file path")
      elsif (not File.exist?(path))
        puts "ModelManager.import_openstudio:  bad path"
          UI.messagebox("#{path} appears to be a bad path")
      else

        model_interface = self.model_interface
        if model_interface
          self.delete_model_interface(model_interface)
        end

        model_interface = ModelInterface.new(Sketchup.active_model)
        self.add_model_interface(model_interface)

        openstudio_model = model_from_openstudio_path(path)

        if openstudio_model

          # attach the openstudio model to the skp model
          model_interface.openstudio_model = openstudio_model
          model_interface.attach_openstudio_model

          if not @dont_zoom
            model_interface.skp_model.active_view.zoom_extents
          end

          model_interface.openstudio_path = nil # import does not set path

          model_interface.construction_manager.check_defaults

          success = true
        end
      end

      # This is probably not the optimal place for this.
      # Trying to keep GUI out of this class.
      if (success)
        Plugin.dialog_manager.update_all if (Plugin.dialog_manager)
        # show errors
        if (model_interface.unviewed_errors)
          model_interface.show_errors
        end
      else
         # don't trigger UI alert here, alreday exists elsewhere
      end

      return(success)
    end

    # open an OpenStudio Model for the Sketchup.active_model, will close existing openstudio_model
    def open_openstudio(path)
     success = false

      if (path.nil?)
        puts "ModelManager.open_openstudio:  nil path"
          UI.messagebox("Please enter a valid file path")
      elsif (not File.exist?(path))
        puts "ModelManager.open_openstudio:  bad path"
          UI.messagebox("#{path} appears to be a bad path")
      else

        model_interface = self.model_interface
        if model_interface
          self.delete_model_interface(model_interface)
        end

        model_interface = ModelInterface.new(Sketchup.active_model)
        self.add_model_interface(model_interface)

        openstudio_model = model_from_openstudio_path(path)

        if openstudio_model

          # attach the openstudio model to the skp model
          model_interface.openstudio_model = openstudio_model
          model_interface.attach_openstudio_model

          model_interface.skp_model.active_view.zoom_extents

          # will create a new path watcher
          model_interface.openstudio_path = path

          model_interface.construction_manager.check_defaults

          success = true
        end
      end

      # This is probably not the optimal place for this.
      # Trying to keep GUI out of this class.
      if (success)
        Plugin.dialog_manager.update_all if (Plugin.dialog_manager)
        # show errors
        if (model_interface.unviewed_errors)
          model_interface.show_errors
        end
      else
         # don't trigger UI alert here, alreday exists elsewhere
      end

      return(success)
    end

=end


  end

end
