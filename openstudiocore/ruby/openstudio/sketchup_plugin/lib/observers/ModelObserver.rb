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
