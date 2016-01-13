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

  class PluginModelObjectWatcher < WorkspaceObjectWatcher
  
    def initialize(drawing_interface)
      super(drawing_interface.model_object)
      
      @drawing_interface = drawing_interface
      @call_on_change_model_object_scheduled = false
    end
    
    def onChangeIdfObject
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super 
      
      # check if we have already recieved this signal in the current OpenStudio Model event loop
      if not @call_on_change_model_object_scheduled
        @call_on_change_model_object_scheduled = true
        
        # add a SketchUp event to call on_change_model_object
        proc = Proc.new { self.call_on_change_model_object }
        Plugin.add_event( proc )
      end
    end
    
    def call_on_change_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @call_on_change_model_object_scheduled = false
      
      # check that model object has not been deleted
      if @drawing_interface and not @drawing_interface.model_object.handle.isNull
        @drawing_interface.on_change_model_object
      end
    end
    
    #def onDataFieldChange
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end
    
    #def onNameChange
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end

    #def onBecomeDirty
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end

    #def onBecomeClean
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end

    #def clearState
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super   
    #end
  
    #def onRelationshipChange(index, newHandle, oldHandle)
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end
    
    # this is called after the object has been removed
    def onRemoveFromWorkspace(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      # DLM: scheduling on_delete_model_object for the next event loop was a problem for the window to wall ratio script
      # sequence of events was: on_pre_delete_model_object for old window, on_new_model_object for new window, on_delete_model_object for old window
      # this resulted in old window geometry intersecting new window geometry
      
      # call on_delete_model_object now
      @drawing_interface.on_delete_model_object
      
      # add a SketchUp event to call on_delete_model_object
      #proc = Proc.new { @drawing_interface.on_delete_model_object }
      #Plugin.add_event( proc )
    end
    
  end
end

