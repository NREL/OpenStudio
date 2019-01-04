########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require("openstudio/sketchup_plugin/lib/interfaces/DrawingInterface")
require("openstudio/sketchup_plugin/lib/interfaces/Space")
require("openstudio/sketchup_plugin/lib/observers/ComponentObserver")

module OpenStudio

  # entity for this class is a component instance with one child
  # the child is locked and transformation from child to entity is identity
  class IlluminanceMap < DrawingInterface

    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      super
      @observer = ComponentObserver.new(self)
    end

##### Begin override methods for the input object #####

    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      model_object = Plugin.model_manager.model_interface.openstudio_model.getIlluminanceMap(handle)
      if not model_object.empty?
        model_object = model_object.get
      else
        puts "IlluminanceMap: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"
        model_object = nil
      end
      return model_object
    end

    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      drawing_interface = IlluminanceMap.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end

    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      model_watcher_enabled = @model_interface.model_watcher.disable
      @model_object = OpenStudio::Model::IlluminanceMap.new(@model_interface.openstudio_model)
      @model_interface.model_watcher.enable if model_watcher_enabled
      super
    end

    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # Look up the Space drawing interface (might fail if the reference is bad)
      if (not parent_from_model_object)
        @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
        @model_interface.add_error("The space referenced by this illuminance map does not exist, it cannot be drawn.\n\n")
        return false
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

          entity_transformation = self.coordinate_transformation

          self.model_object_transformation = entity_transformation

          # find the current x and y scaling relative to 1m x 1m original size
          scalex = entity_transformation.to_a[0]
          scaley = entity_transformation.to_a[5]

          # set lengths
          @model_object.setXLength(scalex.to_f)
          @model_object.setYLength(scaley.to_f)

          enable_watcher if watcher_enabled
        end
      end
    end

    # Returns the parent drawing interface according to the model object.
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
      #  # Create a new space just for this IlluminanceMap.
      #  Plugin.log(OpenStudio::Warn, "Creating containing Space for IlluminanceMap #{@model_object.name}")
      #
      #  @parent = Space.new
      #  @parent.create_model_object
      #  @model_object.setParent(@parent.model_object)
      #  @parent.draw_entity
      #  @parent.add_child(self)  # Would be nice to not have to call this

        # how did this happen?
        Plugin.log(OpenStudio::Error, "Parent #{@parent} is nil, cannot create entity for illuminance map #{@model_object.name}")
        return nil
      end

      # add the component definition
      path = "#{$OPENSTUDIO_SKETCHUPPLUGIN_DIR}/resources/components/OpenStudio_IlluminanceMap.skp"
      definition = Sketchup.active_model.definitions.load(path)

      # parent entity is first a Sketchup::Group corresponding to a space
      # do an identity transformation here as this transformation seems to act on child component axes twice
      @entity = @parent.entity.entities.add_instance(definition, Geom::Transformation.new)

      # make it unique as we will be messing with the definition
      @entity.make_unique

      # have to make the interior component unique too
      @entity.definition.entities[0].make_unique

      # create or confirm layer for class"
      model = Sketchup.active_model
      layers = model.layers
      new_layer = layers.add "#{@model_object.class}"
      # put entity onto new layer
      @entity.layer = new_layer
    end

    def create_from_entity(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super

      # make it unique as we will be messing with the definition
      @entity.make_unique

      # make the interior component unique too
      @entity.definition.entities[0].make_unique

      return(self)
    end


    # Error checks, finalization, or cleanup needed after the entity is drawn.
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      return(super)
    end


    # Updates the SketchUp entity with new information from the ModelObject.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super

      if(valid_entity?)

        # do not want to trigger update_model_object in here
        had_observers = remove_observers

        # need to make unique
        @entity.make_unique

        set_entity_name

        scalex = @model_object.xLength
        scaley = @model_object.yLength

        self.coordinate_transformation = self.model_object_transformation*Geom::Transformation.scaling([0,0,0], scalex, scaley, 1)

        # get number of grid points
        numx = @model_object.numberofXGridPoints
        numy = @model_object.numberofYGridPoints
        numx_draw = [numx-1, 0.5].max
        numy_draw = [numy-1, 0.5].max

        # The ratio below is based on assumption that the grid numbers are being
        # applied to a square face, but if the user has stretched the map I need
        # to adjust the ratio based on the aspect ratio of the stretch.
        ratio = ((numy_draw*1.0)/(numx_draw*1.0))*(scalex/scaley)*1.0

        if (ratio > 15)
          gridfront = "OpenStudioGrid_20"
        elsif (ratio > 7.5)
          gridfront = "OpenStudioGrid_10"
        elsif (ratio > 3)
          gridfront = "OpenStudioGrid_5"
        elsif (ratio > 0.333333)
          gridfront = "OpenStudioGrid_1"
        elsif (ratio > 0.133333)
          gridfront = "OpenStudioGrid_02"
        elsif (ratio > 0.067777)
          gridfront = "OpenStudioGrid_01"
        else
          gridfront = "OpenStudioGrid_005"
       end

        # position the texture
        pts = []
        pts[0] = [0, 0, 0]
        pts[1] = [0,0,0]
        pts[2] = [1.m, 0, 0]
        pts[3] = [numx_draw, 0, 0]
        pts[4] = [1.m, 1.m, 0]
        pts[5] = [numx_draw, numy_draw, 0]
        pts[6] = [0, 1.m, 0]
        pts[7] = [0, numy_draw, 0]

        # find the face
        @entity.definition.entities[0].definition.entities.each do |entity|
          if entity.is_a? Sketchup::Face
            entity.position_material(gridfront, pts, true)
            entity.position_material("grid-back", pts, false)
            break
          end
        end

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
        # @entity class will be a ComponentInstance
        # @entity.parent class will be a ComponentDefinition with one instance
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
        @entity.name = "Illuminance Map:  " + "(Untitled)"
      else
        @entity.name = "Illuminance Map:  " + @model_object.name.get
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
