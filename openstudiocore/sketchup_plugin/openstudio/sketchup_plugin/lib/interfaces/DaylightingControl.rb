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

require("openstudio/sketchup_plugin/lib/interfaces/DrawingInterface")
require("openstudio/sketchup_plugin/lib/interfaces/Space")
require("openstudio/sketchup_plugin/lib/observers/ComponentObserver")

module OpenStudio

  # entity for this class is a component instance
  class DaylightingControl < DrawingInterface
  
    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      @observer = ComponentObserver.new(self)
    end
    
##### Begin override methods for the input object #####

    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_object = Plugin.model_manager.model_interface.openstudio_model.getDaylightingControl(handle)
      if not model_object.empty?
        model_object = model_object.get
      else
        puts "DaylightingControl: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"    
        model_object = nil
      end
      return model_object
    end
    
    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = DaylightingControl.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end
    
    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      model_watcher_enabled = @model_interface.model_watcher.disable
      @model_object = OpenStudio::Model::DaylightingControl.new(@model_interface.openstudio_model)
      @model_interface.model_watcher.enable if model_watcher_enabled
      super
    end
    
    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # Look up the Space drawing interface (might fail if the reference is bad)
      if (not parent_from_model_object)
        @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
        @model_interface.add_error("The space referenced by this daylighting control does not exist, it cannot be drawn.\n\n")
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

          #puts "update_model_object: self.coordinate_transformation = #{self.coordinate_transformation.to_a}"

          self.model_object_transformation = self.coordinate_transformation

          #puts "update_model_object: self.model_object_transformation = #{self.model_object_transformation.to_a}"

          enable_watcher if watcher_enabled
        end
      end
    end
    
    # Returns the parent drawing interface according to the input object.
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

    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (@parent.nil?)        
      #  # Create a new space just for this DaylightingControl.
      #  Plugin.log(OpenStudio::Warn, "Creating containing Space for DaylightingControl #{@model_object.name}")
      #  
      #  @parent = Space.new
      #  @parent.create_model_object
      #  @model_object.setParent(@parent.model_object)
      #  @parent.draw_entity
      #  @parent.add_child(self)  # Would be nice to not have to call this
      
        # how did this happen?
        Plugin.log(OpenStudio::Error, "Parent #{@parent} is nil, cannot create entity for daylighting control #{@model_object.name}")
        return nil
      end    

      # add component definition
      path = "#{$OpenStudio_LibPath}openstudio/sketchup_plugin/lib/resources/components/OpenStudio_DaylightingControl.skp"
      definition = Sketchup.active_model.definitions.load(path)
      
      # parent entity is first a Sketchup::Group corresponding to a space
      @entity = @parent.entity.entities.add_instance(definition, Geom::Transformation.new)
      
      # make it unique as we will be messing with the definition
      @entity.make_unique
      
      # create or confirm layer for class"
      model = Sketchup.active_model
      layers = model.layers
      new_layer = layers.add "#{@model_object.class}"
      # put entity onto new layer
      @entity.layer = new_layer 
    end


    # Error checks, finalization, or cleanup needed after the entity is drawn.
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      return(super)
    end

    # Updates the SketchUp entity with new information from the ModelObject.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # do not want to call super if just want to redraw
      super
      
      if(valid_entity?)
        
        # do not want to trigger update_model_object in here
        had_observers = remove_observers
        
        # set definition
        path = "#{$OpenStudio_LibPath}openstudio/sketchup_plugin/lib/resources/components/OpenStudio_DaylightingControl.skp"
        if @model_object.isPrimaryDaylightingControl
          path = "#{$OpenStudio_LibPath}openstudio/sketchup_plugin/lib/resources/components/OpenStudio_PrimaryDaylightingControl.skp"
        elsif @model_object.isSecondaryDaylightingControl
          path = "#{$OpenStudio_LibPath}openstudio/sketchup_plugin/lib/resources/components/OpenStudio_SecondaryDaylightingControl.skp"
        end
        @entity.definition = Sketchup.active_model.definitions.load(path)
        
        # need to make unique
        @entity.make_unique
        
        set_entity_name
        
        #puts "update_entity: self.coordinate_transformation = #{self.coordinate_transformation.to_a}"
        #puts "update_entity: self.model_object_transformation = #{self.model_object_transformation.to_a}"
        
        # move entity
        self.coordinate_transformation = self.model_object_transformation

        add_observers if had_observers
        
      end
      
    end

    # Final cleanup of the entity.
    # This method is called by the model interface after the entire input file is drawn.
    def cleanup_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
    end

    # Returns the parent drawing interface according to the entity.
    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      parent = nil
      if (valid_entity?)
        # entity class will be a ComponentInstance
        # parent class will be a ComponentDefinition with one instance
        if (@entity.parent.class == Sketchup::ComponentDefinition)
          parent = @entity.parent.instances.first.drawing_interface
        else
          # Somehow got outside of a ComponentInstance--maybe the ComponentInstance was exploded.
        end
      end
      return(parent)
    end
    
##### Begin override methods for the interface #####

##### Begin new methods for the interface #####

    def set_entity_name
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (@model_object.name.empty?)
        @entity.name = "Daylighting Control:  " + "(Untitled)"
      else
        @entity.name = "Daylighting Control:  " + @model_object.name.get
      end
    end

    # Gets the transformation of the ModelObject as it literally appears in the input fields.
    def model_object_transformation
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return Geom.transformation_from_openstudio(@model_object.transformation)
    end

    # Sets the transformation of the ModelObject as it literally appears in the input fields.
    def model_object_transformation=(transform)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_object.setTransformation(transform.to_openstudio_transformation)
    end
    
  end

end
