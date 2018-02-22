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

require("openstudio/sketchup_plugin/lib/interfaces/DrawingUtils")
require("openstudio/sketchup_plugin/lib/interfaces/PlanarSurface")


module OpenStudio

  class SubSurface < PlanarSurface

    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      super
    end

    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      model_object = Plugin.model_manager.model_interface.openstudio_model.getSubSurface(handle)
      if not model_object.empty?
        model_object = model_object.get
      else
        puts "SubSurface: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"
        model_object = nil
      end
      return model_object
    end

    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      drawing_interface = SubSurface.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end

    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # need to get parents transformation
      update_parent_from_entity

      model_watcher_enabled = @model_interface.model_watcher.disable
      vertices = vertices_from_polygon(face_polygon)

      begin
        @model_object = OpenStudio::Model::SubSurface.new(vertices, @model_interface.openstudio_model)
        @model_interface.model_watcher.enable if model_watcher_enabled
      rescue RuntimeError => error
        Plugin.log(Error, "Could not create SubSurface for vertices #{vertices}")
        return nil
      end

      super
    end


    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # All base surfaces should already be drawn.

      if (super)

        # This is the wrong place to do this, but no one else has set '@parent' yet, which is relied on by 'surface_polygon' later.
        @parent = parent_from_model_object
        @parent.add_child(self) if @parent

        # Check the base surface
        if (@parent.class != Surface)
          @model_interface.add_error("Error:  " + name + "\n")
          @model_interface.add_error("This sub surface is missing its base surface, it cannot be drawn.\n\n")
          return(false)
        end

        if @parent.face_polygon.nil?
          @model_interface.add_error("Error:  " + name + "\n")
          @model_interface.add_error("This sub surface's base surface '#{@parent.name}' does not have a face polygon, it cannot be drawn.\n\n")
          return(false)
        end

        if @parent.face_polygon.points.size < 3
          @model_interface.add_error("Error:  " + name + "\n")
          @model_interface.add_error("This sub surface's base surface '#{@parent.name}' has less than 3 points, it cannot be drawn.\n\n")
          return(false)
        end

        # Check if the sub surface is coplanar with the base surface
        plane = @parent.face_polygon.plane

        coplanar = true
        coplanar_points = []

        for point in surface_polygon.points
          coplanar_points << point.project_to_plane(plane)
          if (not point.on_plane?(plane))
            # point.on_plane? apparently has a greater tolerance than Sketchup in general.
            # Sub surfaces that pass this test still show up as not entirely coplanar with the base surface (thick lines).
            coplanar = false
          end
        end

        # Always fix the points to be coplanar because tolerances are different above.
        # Could still be user preference to fix or not.
        # This is a problem because reveals are done as inset surface that are NOT in the same plane!
        watcher_enabled = @watcher.disable
        self.surface_polygon = Geom::Polygon.new(coplanar_points)
        @watcher.enable if watcher_enabled

        if (not coplanar)
          @model_interface.add_error("Warning:  " + name + "\n")
          @model_interface.add_error("This sub surface was not in the same plane as its base surface.\n")
          @model_interface.add_error("It has been automatically fixed.\n\n")
        end

        # Check if the sub surface is inside-out
        if (not surface_polygon.normal.samedirection?(@parent.entity.normal))
          @model_interface.add_error("Warning:  " + name + "\n")
          @model_interface.add_error("This sub surface is inside-out; outward normal does not match its base surface.\n")
          @model_interface.add_error("It has been automatically fixed.\n\n")

          # Always fix this error.

          # Better to add a 'reverse!' method to Surface.
          watcher_enabled = @watcher.disable
          self.model_object_polygon = self.model_object_polygon.reverse
          @watcher.enable if watcher_enabled
        end

        # Check if the sub surface is contained by the base surface
        # SubSurfaces which are equal to the base Surface will pass this test
        contained = true
        for point in surface_polygon.points
          if (not @parent.entity.contains_point?(point, true))
            contained = false
            # Not sure how to fix this one without accidentally overlapping with another surface
            break
          end
        end

        if (not contained)
          @model_interface.add_error("Error:  " + name + "\n")
          @model_interface.add_error("This sub surface is not contained by its base surface, it cannot be drawn.\n\n")
          return(false)
        end

        # TO DO: fix the following conditions instead of erroring
        # Subsurface that is the same size as its base surface...fix so that slightly inset.
        # Subsurface that subdivides a base surface...fix so that slightly inset.

        had_observers = @model_interface.remove_observers

        begin

          do_operation = @model_interface.start_operation("Test SubSurface", true)

          # add a temporary group at the top level
          temp_group = @model_interface.skp_model.entities.add_group
          temp_group.entities.add_face(@parent.surface_polygon.points)
          temp_group.entities.add_face(self.surface_polygon.points)

          num_temp_faces = 0
          temp_group.entities.each do |temp_entity|
            if temp_entity.is_a?(Sketchup::Face)
              num_temp_faces += 1
            end
          end

          temp_group.erase!

        ensure

          @model_interface.commit_operation if do_operation

        end

        @model_interface.add_observers if had_observers

        if num_temp_faces == 1
          @model_interface.add_error("Error:  " + name + "\n")
          @model_interface.add_error("This sub surface is the same as its base surface, it cannot be drawn.\n\n")
          return false
        elsif num_temp_faces > 2
          @model_interface.add_error("Error:  " + name + "\n")
          @model_interface.add_error("This sub surface divides its base surface, it cannot be drawn.\n\n")
          return false
        end

        # Check for coincident surfaces (check against other base surfaces)

        # Check problems with boundary conditions
        adjacentSubSurface = @model_object.adjacentSubSurface
        if adjacentSubSurface.empty?
          if not @model_object.getSources("OS_SubSurface".to_IddObjectType).empty?
            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This SubSurface does not specify an adjacent sub surface but other sub surfaces specify this sub surface.\n")
            @model_interface.add_error("It has been automatically fixed.\n\n")

            watcher_enabled = @watcher.disable
            @model_object.resetAdjacentSubSurface
            @watcher.enable if watcher_enabled
          end
        else
          testSubSurface = adjacentSubSurface.get.adjacentSubSurface
          if testSubSurface.empty? or (testSubSurface.get.handle.to_s != @model_object.handle.to_s)
            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This SubSurface specifies an adjacent sub surface but that sub surface does not specify this sub surface.\n")
            @model_interface.add_error("It has been automatically fixed.\n\n")

            watcher_enabled = @watcher.disable
            if not @model_object.setAdjacentSubSurface(adjacentSubSurface.get)
              @model_object.resetAdjacentSubSurface
            end
            @watcher.enable if watcher_enabled
          end
        end

        # if you get here you are good
        return(true)

      else

        # super failed
        return(false)
      end

    end


    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super  # PlanarSurface superclass updates the vertices

      if (valid_entity?)
        if (@parent.class == Surface)
          watcher_enabled = disable_watcher

          @model_object.setSurface(@parent.model_object)  # Parent should already have been updated.

          enable_watcher if watcher_enabled
        end
      end
    end


    # Returns the parent drawing interface according to the input object.
    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      parent = nil
      if (@model_object)
        surface = @model_object.surface
        if (not surface.empty?)
          parent = surface.get.drawing_interface
        end
      end
      return(parent)
    end

    # Deletes the model object and marks the drawing interface when the SketchUp entity is erased.
    def delete_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # duplicates code in SubSurface::remove
      if not @model_object.adjacentSubSurface.empty?
        @model_object.resetAdjacentSubSurface
      end

      super
    end

##### Begin override methods for the entity #####


    # Error checks, finalization, or cleanup needed after the entity is drawn.
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(super)

      # Check for overlapping sub surfaces in the same space.
      # Check for sub surfaces not on their base surface.
      # Check for more than 4 vertices.  subdivide if necessary.
      # if check really fails, might be able to call Sketchup.undo.


      #points = @entity.full_polygon.reduce.points
      #if (points.length > 4)

        #puts "Oops!  Too many vertices!"

        # Draw an edge from the first vertex to the 4th

        #@entity.parent.entities.add_line(points[0], points[3])

        #Sketchup.undo  # oh yeah can't undo because observer has already assigned some things.
        #return(false)
      #end
    end

    # Erases the entity when the input object is deleted, or in preparation for a re-draw.
    def erase_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      remove_observers(true)
      if (valid_entity?)
        if containing_entity
          entities = [@entity]

          if @parent
            if parent_face = @parent.entity
              @entity.edges.each do |edge|
                if edge.faces.include?(parent_face)
                  entities << edge
                end
              end
            end
          end

          containing_interface = containing_entity.drawing_interface
          had_observers = containing_interface.remove_observers if containing_interface
          containing_entity.entities.erase_entities(entities)
          containing_interface.add_observers if had_observers

        end
      end
    end

    # Returns the parent drawing interface according to the entity.
    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if (valid_entity?)
        if (base_face = DrawingUtils.detect_base_face(@entity))
          return(base_face.drawing_interface)
        else
          return(super)  # Return the space interface
        end
      else
        return(super)  # Return the space interface
      end
    end

    def containing_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = nil
      if @parent.parent
        result = @parent.parent.entity
      end
      return(result)
    end

##### Begin override methods for the interface #####


##### Begin new methods for the interface #####

    def in_selection?(selection)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return (selection.contains?(@entity) or selection.contains?(@parent.entity) or (not @parent.parent.nil? and selection.contains?(@parent.parent.entity)))
    end

    def paint_surface_type(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if @model_object.isAirWall
        @entity.material = @model_interface.materials_interface.air_wall
        @entity.back_material = @model_interface.materials_interface.air_wall
      else
        surface_type = @model_object.subSurfaceType.upcase
        if (surface_type == "DOOR" or surface_type == "OVERHEADDOOR")
          @entity.material = @model_interface.materials_interface.door_ext
          @entity.back_material = @model_interface.materials_interface.door_int
        else
          @entity.material = @model_interface.materials_interface.window_ext
          @entity.back_material = @model_interface.materials_interface.window_int
        end
      end
    end

    def paint_boundary(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if @model_object.adjacentSubSurface.empty?
        @entity.material = @model_interface.materials_interface.subext_ext
        @entity.back_material = @model_interface.materials_interface.subext_int
      else
        @entity.material = @model_interface.materials_interface.subint_ext
        @entity.back_material = @model_interface.materials_interface.subint_int
      end
    end

  end

end
