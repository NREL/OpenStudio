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

require("openstudio/sketchup_plugin/lib/interfaces/DrawingInterface")
require("openstudio/sketchup_plugin/lib/observers/ShadowInfoObserver")

      
module OpenStudio

  class Site < DrawingInterface
  
    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      super
      @observer = ShadowInfoObserver.new(self)
    end
  
    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_object = Plugin.model_manager.model_interface.openstudio_model.getOptionalSite
      if not model_object.empty? and (handle.to_s == model_object.get.handle.to_s)
        model_object = model_object.get
      else
        puts "Site: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"                    
        model_object = nil
      end
      return model_object
    end
    
    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = Site.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end
    
    
    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_watcher_enabled = @model_interface.model_watcher.disable
      @model_object = @model_interface.openstudio_model.getSite
      @model_interface.model_watcher.enable if model_watcher_enabled
      super
    end


    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super

      if (valid_entity?)
        watcher_enabled = disable_watcher
        
        @model_object.setName(@entity["City"])
        @model_object.setLatitude(@entity["Latitude"])
        @model_object.setLongitude(@entity["Longitude"])
        @model_object.setTimeZone(@entity["TZOffset"])
        
        if dictionary = @model_interface.skp_model.attribute_dictionary("GeoReference", false)
          if value = dictionary["ModelTranslationZ"]
            @model_object.setElevation(-value.to_m)
          end
        end
        
        enable_watcher if watcher_enabled
      end
    end


    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@model_interface)
    end


    # Site is unlike other drawing interface because it does not actually create the entity.
    # Instead it gets the current ShadowInfo object.
    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @entity = @model_interface.skp_model.shadow_info
    end


    def check_entity
      return(false) 
    end


    # Updates the SketchUp entity with new information from the ModelObject.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (valid_entity?)
        had_observers = remove_observers(false)
        
        @entity["City"] = @model_object.name().get
        @entity["Latitude"] = @model_object.latitude
        @entity["Longitude"] = @model_object.longitude
        @entity["TZOffset"] = @model_object.timeZone
        
        dictionary = @model_interface.skp_model.attribute_dictionary("GeoReference", true)
        dictionary["ModelTranslationZ"] = -@model_object.elevation.m
        
        add_observers(false) if had_observers
      end
    end


    def on_change_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      update_model_object
      #paint_entity # do not paint
    end


    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @model_interface
    end
    
    def coordinate_transformation   
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # to OpenStudio, site has identify transformation
      return Geom::Transformation.new
    end
    
    def coordinate_transformation=(transform)   
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # nothing to do
    end    
    
  end


end
