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

require("openstudio/sketchup_plugin/lib/interfaces/SurfaceGroup")


module OpenStudio

  class InteriorPartitionSurfaceGroup < SurfaceGroup

    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      super
    end


##### Begin override methods for the input object #####
 
   def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_object = Plugin.model_manager.model_interface.openstudio_model.getInteriorPartitionSurfaceGroup(handle)
      if not model_object.empty?
        model_object = model_object.get
      else
        puts "InteriorPartitionSurfaceGroup: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"
        model_object = nil
      end
      return model_object
    end
    
    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = InteriorPartitionSurfaceGroup.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end


    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_watcher_enabled = @model_interface.model_watcher.disable
      @model_object = OpenStudio::Model::InteriorPartitionSurfaceGroup.new(@model_interface.openstudio_model)
      @model_interface.model_watcher.enable if model_watcher_enabled
      super      
    end
    
    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # Look up the parent drawing interface (might fail if the reference is bad)
      if (not parent_from_model_object)
        @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
        @model_interface.add_error("The space referenced by this interior partition surface group does not exist, it cannot be drawn.\n\n")  
        return(false)
      end
        
      return(super)
    end
    
    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      super

      if (valid_entity?)
        if (@parent.class == Space)
          watcher_enabled = disable_watcher
          
          @model_object.setSpace(@parent.model_object)  # Parent should already have been updated.
          
          enable_watcher if watcher_enabled
        end
      end
    end


    # The parent interface is the space
    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      parent = nil
      if (@model_object)
        space = @model_object.space
        if (not space.empty?)
          parent = space.get.drawing_interface
        end
      end
      return(parent)
    end
    
##### Begin override methods for the entity #####


    # Called from InteriorPartitionSurfaceGroup.new_from_entity(entity).
    # Needed for recreating the Group when a partition surface is reassociated.
    def create_from_entity(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @entity = entity
      @entity.drawing_interface = self

      if (check_entity)
        #create_model_object
        #update_model_object

        update_entity
        update_parent_from_entity  # kludge...this is out of place here, but works:  it adds itself as a child of model interface
        #paint_entity

        #add_observers  # should be added ModelInterface  
      else
        puts "DrawingInterface.create_from_entity:  check_entity failed"
      end
      
      return(self)
    end

    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      super
      
      update_entity
    end
    
##### Begin override methods for the interface #####

    def set_entity_name
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (@model_object.name.empty?)
        @entity.name = "Interior Partition Surface Group:  " + "(Untitled)"
      else
        @entity.name = "Interior Partition Surface Group:  " + @model_object.name.get
      end
    end
    
    
##### Begin new methods for the interface #####
    
    # surface area in in^2
    def surface_area
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      area = 0.0
      for child in @children
        area += child.gross_area
      end
      return(area)
    end
    
  end

end
