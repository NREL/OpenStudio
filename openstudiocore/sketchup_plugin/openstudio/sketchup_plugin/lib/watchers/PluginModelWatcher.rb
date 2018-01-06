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

  class PluginModelWatcher < WorkspaceWatcher
  
    def initialize(model_interface)
      super(model_interface.openstudio_model)

      @model_interface = model_interface
      
      @added_object_handles = []
    end

    #def clearState
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end
    
    #def onChangeWorkspace
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

    def onObjectAdd(addedObject)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      # wrapper object is not fully constructed yet, just store the handle
      @added_object_handles << addedObject.handle
    end
    
    def processAddedObjects
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      openstudio_model = @model_interface.openstudio_model
      
      # loop over all loaded objects
      @added_object_handles.each do |added_object_handle|
      
        model_object = openstudio_model.getObject(added_object_handle)
        
        if model_object.empty?
          Plugin.log(OpenStudio::Warn, "Can't find added object by handle = #{added_object_handle}")
        else
          addedObject = model_object.get

          @model_interface.on_new_model_object(addedObject)

          class_name = addedObject.iddObject.name.upcase

          # these objects do not have a drawing interface where we can call check_model_object
          if class_name == "OS:ELECTRICEQUIPMENT" or 
             class_name == "OS:GASEQUIPMENT" or 
             class_name == "OS:HOTWATEREQUIPMENT" or
             class_name == "OS:INTERNALMASS" or 
             class_name == "OS:LIGHTS" or 
             class_name == "OS:LUMINAIRE" or  # should not ever get luminaire
             class_name == "OS:PEOPLE" 

            spaceLoadInstance = addedObject.to_SpaceLoadInstance
            
            OpenStudio::Modeleditor::ensureSpaceLoadDefinition(spaceLoadInstance.get)

          end

        end
      end
      
      @added_object_handles.clear
      
    end

    # this is called before the object has been removed
    def onObjectRemove(removedObject)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      if drawing_interface = removedObject.drawing_interface
        drawing_interface.on_pre_delete_model_object
      end
    end

  end

end

