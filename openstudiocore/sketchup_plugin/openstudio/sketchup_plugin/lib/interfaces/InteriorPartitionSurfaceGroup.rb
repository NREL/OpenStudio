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
