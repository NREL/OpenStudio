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

  class SelectionObserver < Sketchup::SelectionObserver

    def initialize(selection_interface)
      @selection_interface = selection_interface
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
      @selection_interface = nil
      @enabled = false
    end

    # docs say this is not implemented correctly
    #def onSelectionAdded(*args)
    #
    #  return if not @enabled
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #
    #  http://www.thomthom.net/software/sketchup/observers/#note_SelectionObserver
    #  onSelectionBulkChange triggers instead of onSelectionAdded and onSelectionRemoved.
    #  Release notes of SketchUp 8.0 claims the events where fixed, but this appear to not be the case.
    #
    #  # Called when a new entity is added to the selection.
    #  if (Sketchup.active_model.tools.active_tool_id == 21022) # selection tool
    #    Plugin.dialog_manager.selection_changed
    #  end
    #end


    def onSelectionBulkChange(selection)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      #  http://www.thomthom.net/software/sketchup/observers/#note_SelectionObserver
      #  onSelectionBulkChange triggers instead of onSelectionAdded and onSelectionRemoved.
      #  Release notes of SketchUp 8.0 claims the events where fixed, but this appear to not be the case.

      # Called for almost every change in selection, except when going to no selection (onSelectionCleared gets called instead).
      if (Sketchup.active_model.tools.active_tool_id == 21022) # selection tool
        Plugin.dialog_manager.selection_changed
      end
    end


    def onSelectionCleared(selection)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      #  http://www.thomthom.net/software/sketchup/observers/#note_SelectionObserver
      #  onSelectionBulkChange triggers instead of onSelectionAdded and onSelectionRemoved.
      #  Release notes of SketchUp 8.0 claims the events where fixed, but this appear to not be the case.

      # Called when going from a selection to an empty selection.
      if (Sketchup.active_model.tools.active_tool_id == 21022) # selection tool
        Plugin.dialog_manager.selection_changed
      end
    end


    # docs say this is not implemented correctly
    #def onSelectionRemoved(selection)
    #
    #  return if not @enabled
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #
    #  # Not sure when this is called.
    #end

  end

end
