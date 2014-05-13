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

require("openstudio/sketchup_plugin/lib/interfaces/PlanarSurface")


module OpenStudio

  class Surface < PlanarSurface

    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
    end


##### Begin override methods for the input object #####

    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_object = Plugin.model_manager.model_interface.openstudio_model.getSurface(handle)
      if not model_object.empty?
        model_object = model_object.get
      else
        puts "Surface: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"                    
        model_object = nil
      end
      return model_object
    end
    
    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = Surface.new
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
        @model_object = OpenStudio::Model::Surface.new(vertices, @model_interface.openstudio_model)
        @model_interface.model_watcher.enable if model_watcher_enabled
      rescue RuntimeError => error
        Plugin.log(Error, "Could not create Surface for vertices #{vertices}")
        return nil
      end

      super
    end

    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (super)
        surfaceType = @model_object.surfaceType.upcase
        outsideBoundaryCondition = @model_object.outsideBoundaryCondition.upcase
        
        # Check problems with boundary conditions
        adjacentSurface = @model_object.adjacentSurface
        if adjacentSurface.empty?
          if outsideBoundaryCondition == "SURFACE"
            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This Surface specifies 'Surface' boundary condition but does not reference another Surface.\n")   
            @model_interface.add_error("It has been automatically fixed.\n\n")   
            
            watcher_enabled = @watcher.disable
            @model_object.assignDefaultBoundaryCondition
            @model_object.assignDefaultSunExposure
            @model_object.assignDefaultWindExposure
            @watcher.enable if watcher_enabled
          elsif outsideBoundaryCondition.empty?
            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This Surface specifies '' boundary condition.\n")   
            @model_interface.add_error("It has been automatically fixed.\n\n")   
            
            watcher_enabled = @watcher.disable
            @model_object.assignDefaultBoundaryCondition
            @model_object.assignDefaultSunExposure
            @model_object.assignDefaultWindExposure
            @watcher.enable if watcher_enabled
          end
        else
          testSurface = adjacentSurface.get.adjacentSurface
          if (adjacentSurface.get.handle.to_s == @model_object.handle.to_s)
            # this surface points to itself, this is allowable for stories with multipliers
            
            #@model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            #@model_interface.add_error("This Surface specifies itself as an adjacent surface.\n")   
            #@model_interface.add_error("It has been automatically fixed.\n\n")   
            
            #watcher_enabled = @watcher.disable
            #@model_object.resetAdjacentSurface
            #@watcher.enable if watcher_enabled  
            
            if outsideBoundaryCondition != "SURFACE"
              @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
              @model_interface.add_error("This Surface specifies an adjacent surface and that surface specifies this surface, but this boundary condition is not given as 'Surface'.\n")   
              @model_interface.add_error("It has been automatically fixed.\n\n")   
              
              watcher_enabled = @watcher.disable
              @model_object.setOutsideBoundaryCondition("Surface")
              @watcher.enable if watcher_enabled  
            end
            
          elsif testSurface.empty? or (testSurface.get.handle.to_s != @model_object.handle.to_s)
            # this surface points to other surface which does not point to this surface
            
            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This Surface specifies an adjacent surface but that surface does not specify this surface.\n")   
            @model_interface.add_error("It has been automatically fixed.\n\n")   
            
            watcher_enabled = @watcher.disable
            if not @model_object.setAdjacentSurface(adjacentSurface.get)
              @model_object.resetAdjacentSurface
            end
            @watcher.enable if watcher_enabled  
            
          elsif outsideBoundaryCondition != "SURFACE"
            # this surface points to other surface which does point to this surface
            
            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This Surface specifies an adjacent surface and that surface specifies this surface, but this boundary condition is not given as 'Surface'.\n")   
            @model_interface.add_error("It has been automatically fixed.\n\n")   
            
            watcher_enabled = @watcher.disable
            @model_object.setOutsideBoundaryCondition("Surface")
            @watcher.enable if watcher_enabled  
          end
        end
        
        # Check for upside-down floors, roofs, or ceilings
        dot_product = model_object_polygon.normal % Geom::Vector3d.new(0, 0, 1)
        if (surfaceType == "FLOOR")
          if (dot_product > 0.000001)

            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This Floor surface is upside-down.\nIt has been automatically fixed.\n\n")

            # Better to add a 'reverse!' method to PlanarSurface.
            watcher_enabled = @watcher.disable
            self.model_object_polygon = self.model_object_polygon.reverse
            @watcher.enable if watcher_enabled
          end
        elsif (surfaceType == "ROOFCEILING")
          if (dot_product < -0.000001)

            @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
            @model_interface.add_error("This Roof or Ceiling surface is upside-down.\nIt has been automatically fixed.\n\n")

            # Better to add a 'reverse!' method to Surface.
            watcher_enabled = @watcher.disable
            self.model_object_polygon = self.model_object_polygon.reverse
            @watcher.enable if watcher_enabled
          end
        end

        # Look up the Space drawing interface (might fail if the reference is bad)
        if (not parent_from_model_object)
          @model_interface.add_error("Warning:  " + @model_object.name.to_s + "\n")
          @model_interface.add_error("The space referenced by this base surface does not exist.\n")
          @model_interface.add_error("A new space object has been automatically created for this surface.\n\n")  # Done in create_entity
        end

        # Check for coincident surfaces (check against other sub surfaces as well as base surfaces)

        return(true)
      else
        return(false)
      end
    end


    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object 
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super  # PlanarSurface superclass updates the vertices

      if (valid_entity?)
        if (@parent.class == Space)
          watcher_enabled = disable_watcher

          @model_object.setSpace(@parent.model_object)  # Parent should already have been updated.

          enable_watcher if watcher_enabled
        end
      end

    end


    # Returns the parent drawing interface according to the input object.
    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      parent = nil
      if (@model_object)
        #puts "#{@model_object}, #{@model_object}"
        space = @model_object.space
        if (not space.empty?)
          parent = space.get.drawing_interface
        else
          #puts "space is nil"
        end
      else
        #puts "@model_object is nil"
      end
      
      return(parent)
    end

    # Deletes the model object and marks the drawing interface when the SketchUp entity is erased.
    def delete_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # duplicates code in Surface::remove
      if not @model_object.adjacentSurface.empty?
        @model_object.resetAdjacentSurface
      end
      
      super
    end
    
    
##### Begin override methods for the entity #####


    # Error checks, finalization, or cleanup needed after the entity is drawn.
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
    end
    
    # Undelete happens when an entity is restored after an Undo event.
    def on_undelete_entity(entity)  
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      # there may be subsurfaces that need to be re-parented to this
      if parent
        parent.update_model_object
      end
    end


##### Begin override methods for the interface #####

    def paint_surface_type(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if @model_object.isAirWall
        @entity.material = @model_interface.materials_interface.air_wall
        @entity.back_material = @model_interface.materials_interface.air_wall
      else
        surfaceType = @model_object.surfaceType.upcase
        if (surfaceType == "FLOOR")
          @entity.material = @model_interface.materials_interface.floor_ext
          @entity.back_material = @model_interface.materials_interface.floor_int
        elsif (surfaceType == "ROOFCEILING")
          @entity.material = @model_interface.materials_interface.roof_ext
          @entity.back_material = @model_interface.materials_interface.roof_int
        elsif (surfaceType == "WALL")
          @entity.material = @model_interface.materials_interface.wall_ext
          @entity.back_material = @model_interface.materials_interface.wall_int
        end
      end
    end

    def paint_boundary(info=nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      outsideBoundaryCondition = @model_object.outsideBoundaryCondition.upcase
      if (outsideBoundaryCondition == "SURFACE")
        @entity.material = @model_interface.materials_interface.surface_ext
        @entity.back_material = @model_interface.materials_interface.surface_int
      elsif (outsideBoundaryCondition == "ADIABATIC")
        @entity.material = @model_interface.materials_interface.adiabatic_ext
        @entity.back_material = @model_interface.materials_interface.adiabatic_int
      elsif (outsideBoundaryCondition == "OUTDOORS")
        # add if statements to break into nosunnowind, nosun, nowind, or both sunwind
         sunExposure = @model_object.sunExposure.upcase
         windExposure = @model_object.windExposure.upcase
         if (sunExposure == "SUNEXPOSED")
            if (windExposure == "WINDEXPOSED")
              @entity.material = @model_interface.materials_interface.outdoorssunwind_ext
              @entity.back_material = @model_interface.materials_interface.outdoorssunwind_int
             else
              @entity.material = @model_interface.materials_interface.outdoorssun_ext
              @entity.back_material = @model_interface.materials_interface.outdoorssun_int
             end
         else
            if (windExposure == "WINDEXPOSED")
              @entity.material = @model_interface.materials_interface.outdoorswind_ext
              @entity.back_material = @model_interface.materials_interface.outdoorswind_int
            else
              @entity.material = @model_interface.materials_interface.outdoors_ext
              @entity.back_material = @model_interface.materials_interface.outdoors_int
            end
         end
      elsif (outsideBoundaryCondition == "GROUND")
        @entity.material = @model_interface.materials_interface.ground_ext
        @entity.back_material = @model_interface.materials_interface.ground_int
      elsif (outsideBoundaryCondition == "GROUNDFCFACTORMETHOD")
        @entity.material = @model_interface.materials_interface.groundfcfactormethod_ext
        @entity.back_material = @model_interface.materials_interface.groundfcfactormethod_int
      elsif (outsideBoundaryCondition == "GROUNDSLABPREPROCESSORAVERAGE")
        @entity.material = @model_interface.materials_interface.groundslabpreprocessoraverage_ext
        @entity.back_material = @model_interface.materials_interface.groundslabpreprocessoraverage_int
      elsif (outsideBoundaryCondition == "GROUNDSLABPREPROCESSORCORE")
        @entity.material = @model_interface.materials_interface.groundslabpreprocessorcore_ext
        @entity.back_material = @model_interface.materials_interface.groundslabpreprocessorcore_int
      elsif (outsideBoundaryCondition == "GROUNDSLABPREPROCESSORPERIMETER")
        @entity.material = @model_interface.materials_interface.groundslabpreprocessorperimeter_ext
        @entity.back_material = @model_interface.materials_interface.groundslabpreprocessorperimeter_int
      elsif (outsideBoundaryCondition == "GROUNDBASEMENTPREPROCESSORAVERAGEWALL")
        @entity.material = @model_interface.materials_interface.groundbasementpreprocessoraveragewall_ext
        @entity.back_material = @model_interface.materials_interface.groundbasementpreprocessoraveragewall_int
      elsif (outsideBoundaryCondition == "GROUNDBASEMENTPREPROCESSORAVERAGEFLOOR")
        @entity.material = @model_interface.materials_interface.groundbasementpreprocessoraveragefloor_ext
        @entity.back_material = @model_interface.materials_interface.groundbasementpreprocessoraveragefloor_int
      elsif (outsideBoundaryCondition == "GROUNDBASEMENTPREPROCESSORUPPERWALL")
        @entity.material = @model_interface.materials_interface.groundbasementpreprocessorupperwall_ext
        @entity.back_material = @model_interface.materials_interface.groundbasementpreprocessorupperwall_int
      elsif (outsideBoundaryCondition == "GROUNDBASEMENTPREPROCESSORLOWERWALL")
        @entity.material = @model_interface.materials_interface.groundbasementpreprocessorlowerwall_ext
        @entity.back_material = @model_interface.materials_interface.groundbasementpreprocessorlowerwall_int
      elsif (outsideBoundaryCondition == "OTHERSIDECOEFFICIENTS")
        @entity.material = @model_interface.materials_interface.othersidecoefficients_ext
        @entity.back_material = @model_interface.materials_interface.othersidecoefficients_int
      elsif (outsideBoundaryCondition == "OTHERSIDECONDITIONSMODEL")
        @entity.material = @model_interface.materials_interface.othersideconditionsmodel_ext
        @entity.back_material = @model_interface.materials_interface.othersideconditionsmodel_int
      end
    end

    # Returns the polygon of the SketchUp Face in SketchUp coordinates.
    # Purpose is to remove collinear points and vertices of sub surfaces (overridden in Surface)
    def face_polygon
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (valid_entity?)
        # Get list of children based on actual faces that share vertices with the base face.
        # This is more dynamic than looking at @children which may not be up-to-date yet.
        child_faces = []
        
        #puts "face_polygon = #{self}, #{@entity}"
        
        # DLM: detect_base_face can be expensive, do we have to search all_connected?  is there a way to cache the result of detect_base_face?
        for face in @entity.all_connected 
          if face.class == Sketchup::Face
            face_normal = face.normal
            face_points = face.full_polygon.reduce.points
            if DrawingUtils.is_base_face(face, face_normal, face_points, @entity)
              #puts "found child face->" + face.to_s
              child_faces << face
            end
          end
        end
        
        #puts "child_faces = #{child_faces}"

        reduced_polygon = Geom::Polygon.new(@entity.full_polygon.outer_loop.reduce)  # Removes collinear points
        new_points = []
        for point in reduced_polygon.points

          found = false
          for child in child_faces
            for sub_point in child.full_polygon.points
              if (point == sub_point)
                #puts "sub face vertex subtracted"
                found = true
                break
              end
            end

            if (found)
              break
            end
          end

          # DLM: what if the door shares a vertex with another surface?
          if (not found)
            new_points << point
          end
        end

        return(Geom::Polygon.new(new_points))
      else
        puts "Surface.face_polygon:  entity not valid"
        return(nil)
      end
    end


##### Begin new methods for the interface #####
    
    def exterior?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return (@model_object.outsideBoundaryCondition == "OUTDOORS" or @model_object.isGroundSurface)
    end

    def in_selection?(selection)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return (selection.contains?(@entity) or selection.contains?(@parent.entity))
    end
    
=begin TODO put in model
    # area in in^2
    def glazing_area
      area = 0.0 
      for child in @children
        if (child.class == SubSurface and (child.surface_type.upcase == "WINDOW" or child.surface_type.upcase == "GLASSDOOR"))
          area += child.area # includes multiplier
        end
      end
      return(area)
    end


    def percent_glazing
      if (self.gross_area > 0.0)
        return(100.0 * self.glazing_area / self.gross_area)
      else
        return(0.0)
      end
    end
=end


=begin TODO ?
    # set adiabatic to outdoors
    def set_adiabatic_to_outdoors

      if (@model_object.getString(5, true).get.upcase == "ADIABATIC")
        @model_object.setString(5, "Outdoors")
        @model_object.setString(6, "")
        @model_object.setString(7, "SunExposed")
        @model_object.setString(8, "WindExposed")
 
        #if render set to by boundary then change materials to surface
        if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == 2)
            #apply material to front and back face
            @entity.material = Plugin.model_manager.model_interface.construction_manager.outdoors_ext
            @entity.back_material = Plugin.model_manager.model_interface.construction_manager.outdoors_int
        else
        end
      end

    end

    # set this base surface to adiabatic
    def set_to_adiabatic

        @model_object.setString(5, "Adiabatic")

        #if render set to by boundary then change materials to surface
        if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == 2)
            #apply material to front and back face
            @entity.material = Plugin.model_manager.model_interface.construction_manager.adiabatic_ext
            @entity.back_material = Plugin.model_manager.model_interface.construction_manager.adiabatic_int
        else
        end

    end
=end

  end

end
