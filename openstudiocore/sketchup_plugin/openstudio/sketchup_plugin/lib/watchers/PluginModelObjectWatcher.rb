########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

