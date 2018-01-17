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

module OpenStudio

  class ModelObserver < Sketchup::ModelObserver

    def initialize(model_interface)
      @model_interface = model_interface
      @enabled = false
    end

    def disable
      was_enabled = @enabled
      @enabled = false
      return was_enabled
    end

    def enable
      @enabled = true
    end

    def destroy
      @model_interface = nil
      @enabled = false
    end

    def onActivePathChanged(model)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      @model_interface.update_active_path_transform

      #Plugin.dialog_manager.selection_changed

      parent = @model_interface.skp_model.active_entities.parent
      if (parent.class == Sketchup::ComponentDefinition)
        # Gets the SurfaceGroup interface that is currently open for editing
        drawing_interface = parent.instances.first.drawing_interface
        if drawing_interface.is_a?(SurfaceGroup)
          drawing_interface.delete_initial_box
        end
      end

    end

    def onPreSaveModel(model)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      # DLM: do we really want to tie saving OSM to saving SKP like this?
      #Plugin.command_manager.save_openstudio
    end

    def onPostSaveModel(model)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      if model && model_interface = model.model_interface
        model_interface.skp_model_guid = model.guid
      end

    end

    # unknown if this is called
    def onDeleteModel(model)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      puts "onDeleteModel #{model}, #{model.object_id}, #{model.guid}"

      # destroy has not yet been called so @model_interface is still good
      # destroy will be called as part of delete_model_interface

      #if model_interface == Plugin.model_manager.model_interface(model)
      #  Plugin.model_manager.delete_model_interface(model_interface)
      #end

      #Plugin.menu_manager.refresh

    end

    # these transaction observers also fire in response to SketchUp operations, which may occur in the middle of user operations
    # because of this these observers are not reliable to tell if we are in the middle of a user operation
    # instead using model_interface.start_operation and model_interface.commit_operation which tracks operation_active

    #def onTransactionStart(model)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #  http://www.thomthom.net/software/sketchup/observers/#note_onTransaction
    #  These event mistriggers between the model.start_operation and model.commit_operation every time the model changes instead of being in sync with model.start/commit_operation.
    #  Because of this it is not possible to be notified when a new item appears in the undo stack.
    #
    #end

    #def onTransactionCommit(model)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #  http://www.thomthom.net/software/sketchup/observers/#note_onTransaction
    #  These event mistriggers between the model.start_operation and model.commit_operation every time the model changes instead of being in sync with model.start/commit_operation.
    #  Because of this it is not possible to be notified when a new item appears in the undo stack.
    #
    #end

    #def onTransactionAbort(model)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end

  end

end
