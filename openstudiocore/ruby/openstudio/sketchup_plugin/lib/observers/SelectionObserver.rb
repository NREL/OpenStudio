######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
