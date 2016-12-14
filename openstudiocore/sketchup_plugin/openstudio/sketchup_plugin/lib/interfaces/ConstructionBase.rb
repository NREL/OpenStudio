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

require("openstudio/sketchup_plugin/lib/interfaces/DrawingInterface")

      
module OpenStudio

  class ConstructionBase < DrawingInterface
  
    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_object = Plugin.model_manager.model_interface.openstudio_model.getConstructionBase(handle)
      if not model_object.empty? and (handle.to_s == model_object.get.handle.to_s)
        model_object = model_object.get
      else
        puts "ConstructionBase: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"
        model_object = nil
      end
      return model_object
    end
    
    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = ConstructionBase.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end
    
    
    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_watcher_enabled = @model_interface.model_watcher.disable
      @model_object = OpenStudio::Model::Construction.new(@model_interface.openstudio_model)
      @model_interface.model_watcher.enabled if model_watcher_enabled
      
      # no entity, nothing to do
      #super
    end


    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (super)
        if @model_object.renderingColor.empty?
          watcher_enabled = disable_watcher
          model_watcher_enabled = @model_interface.model_watcher.disable
          had_observers = @model_interface.materials_interface.remove_observers
          
          rendering_color = OpenStudio::Model::RenderingColor.new(@model_interface.openstudio_model)
          @model_object.setRenderingColor(rendering_color)
          @model_interface.model_watcher.onObjectAdd(rendering_color)
          
          @model_interface.materials_interface.add_observers if had_observers
          @model_interface.model_watcher.enable if model_watcher_enabled
          enable_watcher if watcher_enabled
        end    
        
        return(true)
      else
        return(false)
      end
    end
    
    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # should never be called, class does not have an entity
      #super
    end


    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @model_interface
    end


    # There is no entity to create
    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @entity = nil
    end

    def check_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(false) 
    end
    
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(false)
    end

    # Updates the SketchUp entity with new information from the ModelObject.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # model_object changed, call paint here
      if @model_interface.materials_interface.rendering_mode == RenderByConstruction
        @model_interface.request_paint
      end
        
    end


    def on_change_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end


    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @model_interface
    end
    
  end

end
