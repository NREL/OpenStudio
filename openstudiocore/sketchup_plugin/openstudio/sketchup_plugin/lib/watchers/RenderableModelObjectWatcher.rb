########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

require("openstudio/sketchup_plugin/lib/watchers/PluginModelObjectWatcher")

module OpenStudio

  class RenderableModelObjectWatcher < PluginModelObjectWatcher
  
    def initialize(drawing_interface, interface_to_paint, rendering_indices, rendering_modes)
      super(drawing_interface)
      
      @interface_to_paint = interface_to_paint
      @rendering_indices = rendering_indices
      @rendering_modes = rendering_modes
    end

    #def onChangeIdfObject
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end
    
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
  
    def onRelationshipChange(index, newHandle, oldHandle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super

      if i = @rendering_indices.index(index)
        if @drawing_interface.model_interface.materials_interface.rendering_mode == @rendering_modes[i]
          @interface_to_paint.request_paint
        end
      end
    end
    
    #def onRemoveFromWorkspace(handle)
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end
    
  end
end

