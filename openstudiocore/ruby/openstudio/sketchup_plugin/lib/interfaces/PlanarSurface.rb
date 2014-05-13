######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
require("openstudio/sketchup_plugin/lib/observers/FaceObserver")


module OpenStudio

  class PlanarSurface < DrawingInterface

    attr_accessor :outside_variable, :outside_value, :outside_color, :outside_texture, :outside_normalization # for data rendering mode
    attr_accessor :inside_variable, :inside_value, :inside_color, :inside_texture, :inside_normalization   # for data rendering mode
    attr_accessor :redraw_scheduled
    
    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      super
      @observer = FaceObserver.new(self)

      @container_class = Space

      @outside_variable = nil
      @outside_value = nil
      @outside_color = Sketchup::Color.new(255, 255, 255, 1.0)
      @outside_material = nil  # not accessible
      @outside_texture = nil

      @inside_variable = nil
      @inside_value = nil
      @inside_color = Sketchup::Color.new(255, 255, 255, 1.0)
      @inside_material = nil  # not accessible
      @inside_texture = nil
      
      @redraw_scheduled = false
    end


##### Begin override methods for the input object #####


    # This method is overridden for each different type of surface.
    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
    end


    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (super)
        polygon = model_object_polygon
        points = polygon.points

        #Plugin.log(OpenStudio::Info, "check_model_object, polygon = #{polygon}")
        #Plugin.log(OpenStudio::Info, "check_model_object, points = #{points}")
        
        # Check for less than 3 vertices
        if (points.length < 3)
          @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
          @model_interface.add_error("This planar surface has less than 3 vertices.\n")
          @model_interface.add_error("This error cannot be automatically fixed.  The surface will not be drawn.\n\n")
          return(false)
        end
        
        # Check for outward normal
        outward_normal = nil
        begin
          # unknown if vertices_from_polygon may throw, unknown if getOutwardNormal may throw
          outward_normal = OpenStudio::getOutwardNormal(vertices_from_polygon(polygon))
        rescue
        end
        if (outward_normal.nil? or outward_normal.empty?)
          @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
          @model_interface.add_error("Cannot compute outward normal.\n")
          @model_interface.add_error("This error cannot be automatically fixed.  The surface will not be drawn.\n\n")
          return(false)
        end

        # Check that vertices are all in the same plane
        #plane = Geom.fit_plane_to_points(points[0..2]) # these may be collinear in which case the method fails
        plane = Geom.fit_plane_to_points(points)
        new_points = []
        points.each { |point|
        
          new_point = point.project_to_plane(plane)
          
          distance = new_point.distance(point)
          if distance > 1.inch
            @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("Not all the vertices for this planar surface are in the same plane.\n")
            @model_interface.add_error("Point out of plane distance=" + point.distance_to_plane(plane).to_s + "\n")
            @model_interface.add_error("It has been automatically fixed.\n\n")
          end
            
          new_points << point.project_to_plane(plane)
        }

        # OTHER CHECKS TO ADD:
        # Check for more or less vertices than 'number of vertices' flag
        # Check that no crossing lines are formed (wrong order of vertices)
        
        polygon = Geom::Polygon.new(new_points)
        polygon.reduce!  # Remove duplicate and collinear vertices

        watcher_enabled = @watcher.disable if @watcher
        self.model_object_polygon = polygon
        @watcher.enable if watcher_enabled

        return(true)
      else
        return(false)
      end
    end


    # Updates the input object with the current state of the entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      super
      if (valid_entity?)
        watcher_enabled = disable_watcher
        
        self.surface_polygon = self.face_polygon  # Update surface vertices
        
        enable_watcher if watcher_enabled
      end
    end


##### Begin override methods for the entity #####


    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # parent is the containing entity except for sub surfaces
      
      if (@parent.nil?)
        # Create a new space just for this surface.
        Plugin.log(OpenStudio::Warn, "Creating containing SurfaceGroup for PlanarSurface #{@model_object.name}")
        
        @parent = @container_class.new
        @parent.create_model_object
        @parent.draw_entity
        @parent.add_child(self)  # Would be nice to not have to call this
      end
      
      if @parent.deleted?
        # how did this happen?
        Plugin.log(OpenStudio::Error, "Parent #{@parent} has been deleted, cannot create entity for PlanarSurface #{@model_object.name}")
        return nil
      end
      
      points = model_object_polygon.points
      
      new_points = []
      transform = coordinate_transformation.inverse
      points.each do |point|
        new_points << (transform * point)
      end

      containing_entity = self.containing_entity
      
      if containing_entity.deleted?
        # how did this happen?
        Plugin.log(OpenStudio::Error, "Containing entity #{containing_entity} has been deleted, cannot create entity for PlanarSurface #{@model_object.name}")
        Plugin.log(OpenStudio::Error, "Parent = #{@parent}, parent entity = #{@parent.entity}")
        return nil
      end
      
      group = containing_entity.drawing_interface if containing_entity
      group_had_observers = group.remove_observers if group
        
      begin
        @entity = containing_entity.entities.add_face(new_points)
      rescue RuntimeError, ArgumentError, TypeError, Exception => error
        error_msg  = "containing_entity.entities.add_face failed: #{error.message} (#{error.class})"
        puts error_msg
        Plugin.log(OpenStudio::Error, error_msg)
      end
      
      group.add_observers if group_had_observers
      
      if not @entity
        Plugin.log(OpenStudio::Error, "Could not create face for points")
        Plugin.log(OpenStudio::Error, "containing_entity = #{containing_entity}")
        Plugin.log(OpenStudio::Error, "points = #{points}")
        return nil
      end
      
      # Swapping of face entities can occur with the 'add_face' method.
      # Identify and fix any swapped faces now before they can cause any trouble later.

      # The existence of 'drawing_interface' is a sign that an existing face was divided by the new face (which is true for all subsurfaces).
      # This is a prerequisite for a swap and helps eliminate some extra searching.
      if (@entity.drawing_interface)
      
        # if this entity is a new face (face B) split from an existing face (face A)
        # then both face A and face B will reference the same drawing_interface, indeterminate which entity will 
        # be returned by add_face (either face A or face B)

        # Loop through all other faces in the Group to fix any swapped faces.
        # This is not an efficient technique, but it seems fast enough.
        faces = containing_entity.entities.find_all { |this_entity| this_entity.class == Sketchup::Face and this_entity != @entity}

        for face in faces
          # if the face's drawing interface does not reference the face as it's entity
          if (face.drawing_interface.entity != face)
            # Fix the swap--surprisingly this seems to be sufficient.
            face.drawing_interface.entity = face
            puts "Surface.create_entity:  fixed swap for " + face.to_s
          end
        end
        
        # if the vertices of this face were exactly equal to another face then a new entity was not created, the existing entity was returned
        group.children.each do |child|
          next if child == self
          
          # if the child references @entity
          if child.entity and child.entity == @entity
            # if @entity references child
            if child.entity.drawing_interface == child
              # we don't want to steal this from the other interface
              @model_interface.add_error("Error:  " + @model_object.name.to_s + "\n")
              @model_interface.add_error("This planar surface shares the same SketchUp face as #{child.model_object.name}.\n")
              @model_interface.add_error("This error cannot be automatically fixed.  The surface will not be drawn.\n\n")
              
              @entity = nil
            end
          end
        end        
      end
      
      # check that face has correct vertices
      # DLM: this is currently being done in confirm_entity
      #surface_polygon = self.surface_polygon
      #face_polygon = self.face_polygon
      #Plugin.log(OpenStudio::Info, "surface_polygon = #{surface_polygon.points}")
      #Plugin.log(OpenStudio::Info, "face_polygon = #{face_polygon.points}")
      #if surface_polygon.circular_eql?(face_polygon.reverse)
      #  Plugin.log(OpenStudio::Info, "surface_polygon == face_polygon.reverse")
      #  @entity.reverse!
      #end        
      
    end


    def valid_entity?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(super and @entity.vertices.size >= 3 and @entity.area > 0)
    end


    # Error checks, finalization, or cleanup needed after the entity is drawn.
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (super)

        # Even though the vertex order is correct in the input object, SketchUp will sometimes draw a face
        # upside-down because of its relationship to surrounding geometry.
        if (not @entity.normal.samedirection?(surface_polygon.normal))
          puts "Counterclockwise:  Fix unintended reversed face"
          # Fix unintended reversed face.
          @entity.reverse!
        end

        # ERROR CHECKS TO DO:
        # Cleanup unexpected holes in surfaces by adding a edge that slices from hole to outside.
        # Check for crossed lines...wrong vertex order.
        # Detect surfaces that are too tiny...maybe accidentally created
        # Fix near-shared vertices so that they are coincident.

        # Check to see if there was an extra face accidentally created
        # This can happen if there is an extra collinear vertex on the base face (because of other bordering faces)
        # that overlaps with the polygon of the new face.

        return(true)
      else
        return(false)
      end
    end

    # Updates the SketchUp entity with new information from the ModelObject.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      if (valid_entity?)
        paint_entity

        if (not surface_polygon.circular_eql?(face_polygon))

          #Plugin.log(OpenStudio::Info, "surface_polygon = #{surface_polygon.points}")
          #Plugin.log(OpenStudio::Info, "face_polygon = #{face_polygon.points}")
          
          # this should be fairly rare as this only occurs when model object vertices are changed, e.g. in a user script
          puts "redraw polygon"
          Plugin.log(OpenStudio::Info, "redraw polygon")

          # do the erasing now
          group = self.containing_entity.drawing_interface
          group_had_observers = group.remove_observers if group
          all_children = recurse_children
          erase_entity
          all_children.each {|child| child.erase_entity }
          group.add_observers if group_had_observers
          
          @redraw_scheduled = true
          
          # schedule the creating of new entities for later, this avoids intersections of old geometry with new
          proc = Proc.new { 
            
            @redraw_scheduled = false
            
            group_had_observers = group.remove_observers
            
            # remove any stray edges from the group
            group.cleanup_entity
            
            # draw_entity has the potential to be recursive here
            # relying on fact that surface_polygon.circular_eql?(face_polygon) should now be true
            draw_entity
            @children.each { |child| child.draw_entity }
            add_observers(true)
            
            group.add_observers if group_had_observers

          }
          Plugin.add_event( proc )
         
        end
      end
    end


    def paint_entity(info = nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (valid_entity?)
        had_observers = remove_observers

        group = self.containing_entity.drawing_interface
        group_had_observers = group.remove_observers if group

        material_interface = @entity.material.drawing_interface if @entity.material
        material_interface_had_observers = material_interface.remove_observers if material_interface

        back_material_interface = @entity.back_material.drawing_interface if @entity.back_material
        back_material_interface_had_observers = back_material_interface.remove_observers if back_material_interface

        materials_interface_had_observers = @model_interface.materials_interface.remove_observers
        rendering_mode = @model_interface.materials_interface.rendering_mode
        
        @entity.material = nil
        @entity.back_material = nil

        if (rendering_mode == RenderWaiting)
          # no-op
        elsif (rendering_mode == RenderByClass)
          paint_surface_type(info)
        elsif (rendering_mode == RenderByDataValue)
          paint_data(info)
        elsif (rendering_mode == RenderByBoundary)
          paint_boundary(info)
        elsif (rendering_mode == RenderByLayer)
          paint_layer(info)
        elsif (rendering_mode == RenderBySurfaceNormal)
          paint_normal(info)
        elsif (rendering_mode == RenderByConstruction)
          paint_construction(info)   
        elsif (rendering_mode == RenderBySpaceType)
          paint_space_type(info)
        elsif (rendering_mode == RenderByThermalZone)
          paint_thermal_zone(info)
        elsif (rendering_mode == RenderByBuildingStory)
          paint_building_story(info)
        end
        
        @model_interface.materials_interface.add_observers if materials_interface_had_observers

        back_material_interface.add_observers if back_material_interface_had_observers

        material_interface.add_observers if material_interface_had_observers

        group.add_observers if group_had_observers

        add_observers if had_observers
      end
    end

    # Used by info tool
    def tooltip(flags = nil, inside_info = false )
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      tooltip = ""
      
      rendering_mode = @model_interface.materials_interface.rendering_mode
      if (rendering_mode == RenderByDataValue)
      
        tooltip = " No data."  # Extra space to get out from under the icon
      
        if (inside_info)  
          variable_name = @model_interface.results_interface.inside_variable_name
          variable = inside_variable
          value = inside_value
        else  
          variable_name = @model_interface.results_interface.outside_variable_name
          variable = outside_variable
          value = outside_value
        end

        if (variable and value)
          tooltip = name + "\n"
          tooltip += variable_name + "\n"
          tooltip += value.round_to(6).to_s + " " + variable.units

          if (@model_interface.results_interface.normalize)
            tooltip += "/m2"
          end
        end
        
      elsif (rendering_mode == RenderByConstruction)
      
        tooltip = " No construction." # Extra space to get out from under the icon
      
        if @model_object.isConstructionDefaulted and not @model_interface.materials_interface.render_defaults
          tooltip = " Construction is defaulted."
        
        elsif (flags & COPY_MODIFIER_MASK > 0)  # Ctrl key is down, show material 
                
          tooltip = " No material." # Extra space to get out from under the icon

          construction = @model_object.construction
          if (not construction.empty?)
            layered_construction = construction.get.to_LayeredConstruction
            if (not layered_construction.empty?)
              layers = layered_construction.get.layers

              if (not layers.empty?)
                if (inside_info) 
                  # inside
                  tooltip = layers[-1].to_s
                else
                  # outside
                  tooltip = layers[0].to_s
                end
              end
            end
          end
        
        else
            
          # no modifier show construction
          construction = @model_object.construction
          if (not construction.empty?)
            tooltip = construction.get.to_s
          end

        end
        
          
      elsif (rendering_mode == RenderBySpaceType)
        
        tooltip = " No space type." # Extra space to get out from under the icon
      
        space = @model_object.space
        if not space.empty?
          space_type = space.get.spaceType
          if not space_type.empty?
            tooltip = space_type.get.to_s
          end
        end
          
      elsif (rendering_mode == RenderByThermalZone)
      
        tooltip = " No thermal zone." # Extra space to get out from under the icon
      
        space = @model_object.space
        if not space.empty?
          thermal_zone = space.get.thermalZone
          if not thermal_zone.empty?
            tooltip = thermal_zone.get.to_s
          end
        end
          
      elsif (rendering_mode == RenderByBuildingStory)
        
        tooltip = " No building story."
      
        space = @model_object.space
        if not space.empty?
          building_story = space.get.buildingStory
          if not building_story.empty?
            tooltip = building_story.get.to_s
          end
        end
          
      else
      
        if (flags & CONSTRAIN_MODIFIER_MASK > 0) and (flags & COPY_MODIFIER_MASK > 0) # Shift and Ctrl keys are down, show parent
          
          tooltip = " No parent."

          if parent
            parent_object = parent.model_object
            if parent_object
              tooltip = parent_object.to_s
            end
          end
          
        elsif (flags & CONSTRAIN_MODIFIER_MASK > 0) # Shift key is down, show construction
        
          tooltip = " No construction." # Extra space to get out from under the icon

          construction = @model_object.construction
          if (not construction.empty?)
            tooltip = construction.get.to_s
          end
          
        elsif (flags & COPY_MODIFIER_MASK > 0)  # Ctrl key is down, show material 
        
          tooltip = " No material." # Extra space to get out from under the icon
 
          construction = @model_object.construction
          if (not construction.empty?)
            layered_construction = construction.get.to_LayeredConstruction
            if (not layered_construction.empty?)
              layers = layered_construction.get.layers
              
              if (not layers.empty?)
                if (inside_info) 
                  # inside
                  tooltip = layers[-1].to_s
                else
                  # outside
                  tooltip = layers[0].to_s
                end
              end
            end
          end
          
        else
        
          # No modifier key
          tooltip = @model_object.to_s
        end
        
      end
      
      return tooltip
    end
    
    # Final cleanup of the entity.
    # This method is called by the model interface after the entire input file is drawn.
    def cleanup_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
    end


    # Returns the parent drawing interface according to the entity.
    # For several surfaces, the parent interface is determined by looking at the parent Group entity.
    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      parent = nil
      if (valid_entity?)
        if (@entity.parent.is_a?(Sketchup::ComponentDefinition))
          parent = @entity.parent.instances.first.drawing_interface
        else
          # Somehow the surface got outside of a Group--maybe the Group was exploded.
        end
      end
      return(parent)
    end

##### Begin override methods for the interface #####

##### Begin new methods for the interface #####

    # Gets the vertices of the ModelObject as they appear in the input fields.
    def model_object_polygon
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      points = []
      @model_object.vertices.each do |vertex|
        points << Geom::Point3d.new(vertex.x.m, vertex.y.m, vertex.z.m)
      end
      Plugin.log(OpenStudio::Info, "model_object_polygon, points = #{points}")
      return(Geom::Polygon.new(points))
    end

    # Sets the vertices of the ModelObject as they appear in the input fields.
    def model_object_polygon=(polygon)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
     
      vertices = vertices_from_polygon(polygon)
      
      Plugin.log(OpenStudio::Info, "model_object_polygon=, polygon = #{polygon.points}")
      Plugin.log(OpenStudio::Info, "vertices=, #{vertices.class} #{vertices}")
      @model_object.setVertices(vertices)
    end
    
    # unknown if vertices_from_polygon may throw
    def vertices_from_polygon(polygon)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      polygon.transform(self.coordinate_transformation)
          
      vertices = OpenStudio::Point3dVector.new
      polygon.points.each do |point|
        vertices << OpenStudio::Point3d.new(point.x.to_m, point.y.to_m, point.z.to_m)
      end
      vertices = OpenStudio::reorderULC(vertices)
          
      return vertices
    end

    # Override in subclasses.
    def paint_surface_type(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end

    # Override in subclasses.
    def paint_boundary(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end

    # Don't need to override in subclasses.
    def paint_layer(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end

    # Don't need to override in subclasses.
    def paint_normal(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end

    # Don't need to override in subclasses.
    def paint_construction(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      rendering_colors = nil
      if info
        rendering_colors = info['rendering_colors']
      else
        rendering_colors = @model_interface.rendering_colors
      end
      
      if @model_object.isConstructionDefaulted and not @model_interface.materials_interface.render_defaults
        return
      end
      
      if @model_object
        construction = @model_object.construction
        if not construction.empty?
          renderingObject = construction.get.renderingColor
          if not renderingObject.empty?
            rendering_color = renderingObject.get.drawing_interface
            if rendering_color.entity
              had_observers = rendering_color.remove_observers
              @entity.material = rendering_color.entity
              @entity.back_material = rendering_color.entity
              rendering_color.add_observers if had_observers
            end
          end
        end
      end
    end
    
    # Don't need to override in subclasses.
    def paint_space_type(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      rendering_colors = nil
      if info
        rendering_colors = info['rendering_colors']
      else
        rendering_colors = @model_interface.rendering_colors
      end

      if @model_object
        space = @model_object.space
        if not space.empty?

          if space.get.isSpaceTypeDefaulted and not @model_interface.materials_interface.render_defaults
            return
          end

          space_type = space.get.spaceType
          if not space_type.empty?
            renderingObject = space_type.get.renderingColor
            if not renderingObject.empty?
              rendering_color = renderingObject.get.drawing_interface
              if rendering_color.entity
                had_observers = rendering_color.remove_observers
                @entity.material = rendering_color.entity
                @entity.back_material = rendering_color.entity
                rendering_color.add_observers if had_observers
              end
            end
          end
        end
      end
    end
    
    # Don't need to override in subclasses.
    def paint_thermal_zone(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      rendering_colors = nil
      if info
        rendering_colors = info['rendering_colors']
      else
        rendering_colors = @model_interface.rendering_colors
      end
      
      if @model_object
        space = @model_object.space
        if not space.empty?
          thermal_zone = space.get.thermalZone
          if not thermal_zone.empty?
            renderingObject = thermal_zone.get.renderingColor
            if not renderingObject.empty?
              rendering_color = renderingObject.get.drawing_interface
              if rendering_color.entity
                had_observers = rendering_color.remove_observers
                @entity.material = rendering_color.entity
                @entity.back_material = rendering_color.entity
                rendering_color.add_observers if had_observers
              end
            end
          end
        end
      end
    end
    
    # Don't need to override in subclasses.
    def paint_building_story(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      rendering_colors = nil
      if info
        rendering_colors = info['rendering_colors']
      else
        rendering_colors = @model_interface.rendering_colors
      end
      
      if @model_object
        space = @model_object.space
        if not space.empty?
          building_story = space.get.buildingStory
          if not building_story.empty?
            renderingObject = building_story.get.renderingColor
            if not renderingObject.empty?
              rendering_color = renderingObject.get.drawing_interface
              if rendering_color.entity
                had_observers = rendering_color.remove_observers
                @entity.material = rendering_color.entity
                @entity.back_material = rendering_color.entity
                rendering_color.add_observers if had_observers
              end
            end
          end
        end
      end
    end
    
    # Painting with data doesn't matter what type of surface.
    def paint_data(info)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    
      if not info
        return(nil)
      end
    
      # unpack info
      date_time = info['date_time'] 
      range_min = info['range_min'] 
      range_max = info['range_max'] 
      out_of_range_value = info['out_of_range_value']
      rendering_appearance = info['rendering_appearance'] 
      normalize = info['normalize'] 
    
      # would be nice to do the paint once...then have a database of the materials...animations and updates can change color of materials
      # not having to call paint everytime.
      value = nil
      if (self.outside_variable)
        value = self.outside_variable.value(date_time)

        if (not value.nil?) and normalize
          value = value / (self.outside_normalization.to_m.to_m)
        end
      end

      if (value.nil?)
        color = Sketchup::Color.new(255, 255, 255, 1.0)  # No data--paint white
        texture = nil

      elsif (value == out_of_range_value)
        color = Sketchup::Color.new(255, 255, 255, 1.0)  # No data--paint white
        texture = nil

      elsif (range_max == range_min)
        color = Sketchup::Color.new(255, 255, 255, 1.0)  # No data--paint white
        texture = nil

      elsif (value < range_min)
        color = nil
        if (rendering_appearance == "COLOR")
          texture = Plugin.dir + "lib/resources/crosshatch_blue.bmp"
        else
          texture = Plugin.dir + "lib/resources/crosshatch_black.bmp"
        end

      elsif (value > range_max)
        color = nil
        if (rendering_appearance == "COLOR")
          texture = Plugin.dir + "lib/resources/crosshatch_red.bmp"
        else
          texture = Plugin.dir + "lib/resources/crosshatch_white.bmp"
        end

      else
        color = Sketchup::Color.new
        if (rendering_appearance == "COLOR")
          h = 240.0 * (range_max - value) / (range_max - range_min)
          color.hsba = [h, 100, 100, 1.0]
        else  # Gray scale
          b = 90.0 - 70.0 * (range_max - value) / (range_max - range_min)
          color.hsba = [0, 0, b, 1.0]
        end
        texture = nil
      end
      self.outside_value = value
      self.outside_color = color
      self.outside_texture = texture


      value = nil
      if (self.inside_variable)
        value = self.inside_variable.value(date_time)
        if (not value.nil?) and normalize
          value = value / (self.inside_normalization.to_m.to_m)
        end
      end

      if (value.nil?)
        color = Sketchup::Color.new(255, 255, 255, 1.0)  # No data--paint white
        texture = nil

      elsif (value == out_of_range_value)
        color = Sketchup::Color.new(255, 255, 255, 1.0)  # No data--paint white
        texture = nil

      elsif (range_max == range_min)
        color = Sketchup::Color.new(255, 255, 255, 1.0)  # No data--paint white
        texture = nil

      elsif (value < range_min)
        color = nil
        if (rendering_appearance == "COLOR")
          texture = Plugin.dir + "lib/resources/crosshatch_blue.bmp"
        else
          texture = Plugin.dir + "lib/resources/crosshatch_black.bmp"
        end

      elsif (value > range_max)
        color = nil
        if (rendering_appearance == "COLOR")
          texture = Plugin.dir + "lib/resources/crosshatch_red.bmp"
        else
          texture = Plugin.dir + "lib/resources/crosshatch_white.bmp"
        end

      else
        color = Sketchup::Color.new
        if (rendering_appearance == "COLOR")
          h = 240.0 * (range_max - value) / (range_max - range_min)
          color.hsba = [h, 100, 100, 1.0]
        else  # Gray scale
          b = 90.0 - 70.0 * (range_max - value) / (range_max - range_min)
          color.hsba = [0, 0, b, 1.0]
        end
        texture = nil
      end
      self.inside_value = value
      self.inside_color = color
      self.inside_texture = texture    
    
      if (@outside_material.nil? or not @outside_material.valid?)
        # Material might have been purged as unused
        @outside_material = Sketchup.active_model.materials.add
      end
      @outside_material.color = @outside_color
      @outside_material.texture = @outside_texture

      if (@inside_material.nil? or not @inside_material.valid?)
        # Material might have been purged as unused
        @inside_material = Sketchup.active_model.materials.add
      end
      @inside_material.color = @inside_color
      @inside_material.texture = @inside_texture

      if (valid_entity?)
        @entity.material = @outside_material
        @entity.back_material = @inside_material
      end
    end

    # Returns the vertices of the ModelObject as they should be drawn in the SketchUp coordinate system.
    def surface_polygon
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
#puts "surface_polygon, model_object_polygon = #{model_object_polygon.points}"
#puts "surface_polygon, self.coordinate_transformation.inverse = #{self.coordinate_transformation.inverse.to_a.join(',')}"
       return(model_object_polygon.transform(self.coordinate_transformation.inverse))
    end


    # Sets the vertices of the ModelObject from vertices drawn in the SketchUp coordinate system.
    def surface_polygon=(face_polygon)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
#puts "surface_polygon=, face_polygon = #{face_polygon.points}"
#puts "surface_polygon=, self.coordinate_transformation = #{self.coordinate_transformation.to_a.join(',')}"
      self.model_object_polygon = face_polygon.transform(self.coordinate_transformation)
    end


    # Returns the polygon of the SketchUp Face in SketchUp coordinates.
    # Purpose is to remove collinear points and vertices of sub surfaces (overridden in Surface)
    def face_polygon  
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # can this just call @entity.outer_polygon?
      points = @entity.full_polygon.outer_loop.reduce  # removes collinear points
      
      #Plugin.log(OpenStudio::Info, "face_polygon, face_polygon = #{points}")
      
      return(Geom::Polygon.new(points))
    end
    
  end

end
