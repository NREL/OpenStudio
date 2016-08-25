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

require("openstudio/sketchup_plugin/lib/dialogs/Dialogs")
require("openstudio/sketchup_plugin/lib/interfaces/Surface")
require("openstudio/sketchup_plugin/lib/interfaces/SubSurface")
require("openstudio/sketchup_plugin/lib/dialogs/LastReportInterface")

module OpenStudio

  class SurfaceMatchingDialog < PropertiesDialog

    def initialize(container, interface, hash)
      super
      @container = WindowContainer.new("Surface Matching", 380, 200, 150, 150)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/SurfaceMatching.html")
      
      @last_report = ""
      
      add_callbacks
    end

    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_intersect_all") { on_intersect_all }
      @container.web_dialog.add_action_callback("on_intersect_selected") { on_intersect_selected }
      @container.web_dialog.add_action_callback("on_match_selected") { on_match_selected }
      @container.web_dialog.add_action_callback("on_match_all") { on_match_all }
      @container.web_dialog.add_action_callback("on_unmatch_selected") { on_unmatch_selected }
      @container.web_dialog.add_action_callback("on_unmatch_all") { on_unmatch_all }
      @container.web_dialog.add_action_callback("on_last_report") { on_last_report }
      @container.web_dialog.add_action_callback("on_cancel") { on_cancel }
    end
    
    def on_load
      super
    end     
    
    def on_intersect_selected
      model = Plugin.model_manager.model_interface.skp_model
      intersect(model.selection)
    end
    
    def on_intersect_all
      model_interface = Plugin.model_manager.model_interface
      model = model_interface.skp_model
      model.selection.clear

      model_interface.spaces.each do |space| 
        model.selection.add(space.entity)
      end
      
      intersect(model.selection)
      
      model.selection.clear
    end

    def intersect(selection)
    
      # canel if there is no selection
      if selection.empty?
        UI.messagebox("Selection is empty, please select objects for intersection routine or choose 'Intersect in Entire Model'.")
        return
      end

      # offer user chance to cancel
      result = UI.messagebox(
"Warning this will create new geometry in your spaces.\n
This operation cannot be undone. Do you want to continue?", MB_OKCANCEL)

      if result == 2 # cancel
        return false
      end
      
      model_interface = Plugin.model_manager.model_interface
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      # store starting render mode
      starting_rendermode = model_interface.materials_interface.rendering_mode
      
      # switch render mode to speed things up 
      model_interface.materials_interface.rendering_mode = RenderWaiting
      
      # DLM: creating a lot of subsurfaces in an operation appears to create problems when multiple surfaces
      # are swapped simultaneously, need more testing to understand this
      #begin
      #  # start an operation
      #  model_interface.start_operation("Intersect Space Geometry", true)

      # create a progress bar
      progress_dialog = ProgressDialog.new("Intersecting Space Geometry")
      
      # temporarily hide everything but heat transfer surfaces
      model_interface.skp_model.entities.each {|e| e.visible = false}
      model_interface.shading_surface_groups.each { |group| group.entity.visible = false }
      model_interface.interior_partition_surface_groups.each { |group| group.entity.visible = false }
      model_interface.illuminance_maps.each { |group| group.entity.visible = false }
      model_interface.daylighting_controls.each { |group| group.entity.visible = false }

      # get all spaces
      spaces = model_interface.spaces      
      spaces.each { |space| space.entity.visible = true }
      
      num_total = spaces.size
      num_complete = 0

      # iterate through spaces to create intersecting geometry
      spaces.each do |space|
        entity = space.entity
        entity.entities.intersect_with(true, entity.transformation, entity.entities.parent, entity.transformation, false, selection.to_a)
        
        num_complete += 1
        progress_dialog.setValue((100*num_complete)/num_total)
      end

      # unhide everything
      model_interface.skp_model.entities.each {|e| e.visible = true}
      model_interface.shading_surface_groups.each { |group| group.entity.visible = true }
      model_interface.interior_partition_surface_groups.each { |group| group.entity.visible = true }
      model_interface.illuminance_maps.each { |group| group.entity.visible = true }
      model_interface.daylighting_controls.each { |group| group.entity.visible = true }
    
      #ensure
      #
      #  model_interface.commit_operation
      #
      #end
      
      progress_dialog.destroy
      
      # switch render mode back to original
      proc = Proc.new { model_interface.materials_interface.rendering_mode = starting_rendermode }
      Plugin.add_event( proc )
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
    end

    def on_match_selected
      model = Plugin.model_manager.model_interface.skp_model
      match(model.selection)
    end

    def on_match_all
      model = Plugin.model_manager.model_interface.skp_model
      model.selection.clear
      model.entities.each {|e| model.selection.add(e)}
      match(model.selection)
      model.selection.clear
    end
    
    def on_unmatch_selected
      model = Plugin.model_manager.model_interface.skp_model
      unmatch(model.selection)
    end

    def on_unmatch_all
      model = Plugin.model_manager.model_interface.skp_model
      model.selection.clear
      model.entities.each {|e| model.selection.add(e)}
      unmatch(model.selection)    
      model.selection.clear
    end
    
    def grow_bounds(bounds, grow_amount = 1.inch)
      result = Geom::BoundingBox.new
      
      result.add(bounds.corner(0) + Geom::Vector3d.new(-grow_amount,-grow_amount,-grow_amount)) # 0 = [0, 0, 0] (left front bottom)
      result.add(bounds.corner(1) + Geom::Vector3d.new( grow_amount,-grow_amount,-grow_amount)) # 1 = [1, 0, 0] (right front bottom)
      result.add(bounds.corner(2) + Geom::Vector3d.new(-grow_amount, grow_amount,-grow_amount)) # 2 = [0, 1, 0] (left back bottom)
      result.add(bounds.corner(3) + Geom::Vector3d.new( grow_amount, grow_amount,-grow_amount)) # 3 = [1, 1, 0] (right back bottom)
      result.add(bounds.corner(4) + Geom::Vector3d.new(-grow_amount,-grow_amount, grow_amount)) # 4 = [0, 0, 1] (left front top)
      result.add(bounds.corner(5) + Geom::Vector3d.new( grow_amount,-grow_amount, grow_amount)) # 5 = [1, 0, 1] (right front top)
      result.add(bounds.corner(6) + Geom::Vector3d.new(-grow_amount, grow_amount, grow_amount)) # 6 = [0, 1, 1] (left back top)
      result.add(bounds.corner(7) + Geom::Vector3d.new( grow_amount, grow_amount, grow_amount)) # 7 = [1, 1, 1] (right back top)

      return result
    end

    def match(selection)
    
      @last_report = "Surface Matching Report:\n"
      @last_report << "Action, Surface #1, Space #1, Surface #2, Space #2\n"

      if selection.empty?
        UI.messagebox("Selection is empty, please select objects for matching routine or choose 'Match in Entire Model'.")
        return 
      end
      
      result = UI.messagebox(
"Warning this will match surfaces and subsurfaces 
within and surrounding the selected Spaces.\n
This will also reassign constructions on affected surfaces.\n
This operation cannot be undone. Do you want to continue?", MB_OKCANCEL)

      if result == 2 # cancel
        return false
      end
      
      model_interface = Plugin.model_manager.model_interface
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      # store starting render mode
      starting_rendermode = model_interface.materials_interface.rendering_mode
      
      # switch render mode to speed things up 
      model_interface.materials_interface.rendering_mode = RenderWaiting

      # get all spaces
      spaces = model_interface.spaces.to_a
  
      # get all base surfaces
      surfaces = model_interface.surfaces.to_a
      
      inspector_dialog_enabled = Plugin.dialog_manager.inspector_dialog.disable
      
      begin

        # create a progress dialog
        progress_dialog = ProgressDialog.new("Matching Surfaces")

        # processed and total number of comparisons
        processed_num = 0
        total_num = (surfaces.length * (surfaces.length-1)) / 2

        # num matches found
        num_matches = 0

        # precompute which spaces intersect with each other space
        space_names = []
        space_bounds = []
        space_intersections = []
        spaces.each_index do |i|
          space_names[i] = spaces[i].name
          space_bounds[i] = grow_bounds( spaces[i].entity.bounds )
          space_intersections[i]=[]
        end
        spaces.each_index do |i|
          space_intersections[i][i] = false
          # loop over remaining spaces
          (i+1..spaces.length-1).each do |j|
            #intersect api may have bugs, isn't fully functional here
            bbox = space_bounds[i].intersect(space_bounds[j])
            test = (not bbox.empty?)
            space_intersections[i][j] = test
            space_intersections[j][i] = test
          end
        end

        # loop over all base surfaces
        surface_bounds = []
        surface_space_indices = []
        surfaces.each_index do |i|
          next if not (surfaces[i].is_a?(Surface) and
                       surfaces[i].parent.is_a?(Space))

          surface_space_index = space_names.index(surfaces[i].parent.name)
          surface_space_indices[i] = surface_space_index

          # get the local bounding box
          bounds = surfaces[i].entity.bounds

          # get the parents transformation
          transform = surfaces[i].parent.coordinate_transformation

          # make a new bounding box in global coordinates
          global_bounds = Geom::BoundingBox.new
          (0..7).each do |j|
            global_bounds.add(transform*bounds.corner(j))
          end

          # put the global bounding box into the array
          surface_bounds[i] = grow_bounds(global_bounds)

        end

        # array for matching base surfaces
        surface_intersections = []
        surface_names = []
        surfaces.each_index do |i|
          surface_intersections[i] = Array.new(surfaces.length, false)
          surface_names[i] = surfaces[i].name
        end

        # loop over all base surfaces
        surfaces.each_index do |i|
        
          next if not (surfaces[i].is_a?(Surface) and
                       surfaces[i].parent.is_a?(Space))
  
          # get the parents transformation
          transform_i = surfaces[i].parent.coordinate_transformation
          
          # get the polygon, reverse it
          reverse_face_polygon = surfaces[i].face_polygon.reverse.transform(transform_i)
          
          # get the normal
          face_normal = surfaces[i].entity.normal.transform(transform_i)

          # don't process empty polygons
          next if reverse_face_polygon.empty?

          # loop over remaining surfaces
          (i+1..surfaces.length-1).each do |j|
          
            # update number of comparisons
            processed_num += 1
            percent_complete = (100*processed_num)/total_num
            progress_dialog.setValue(percent_complete)

            next if not (surfaces[j].is_a?(Surface) and 
                         surfaces[j].parent.is_a?(Space))
            
            # get the parents transformation
            transform_j = surfaces[j].parent.coordinate_transformation
          
            # check for intersection of spaces
            space_i = surface_space_indices[i]
            space_j = surface_space_indices[j]
            next if not space_intersections[space_i][space_j]
            
            # check for intersection of bounding boxes
            next if not surface_bounds[i].contains?(surface_bounds[j])

            # add to base surface intersections
            surface_intersections[i][j] = true
            surface_intersections[j][i] = true

            # selection must contain either surface
            next if not (selection.contains?(surfaces[i].entity) or 
                         selection.contains?(surfaces[i].parent.entity) or
                         selection.contains?(surfaces[j].entity) or 
                         selection.contains?(surfaces[j].parent.entity))

            # check normal dot product
            next if not face_normal.dot(surfaces[j].entity.normal.transform(transform_j)) < -0.98

            # check if the reverse of this polygon equals the other polygon
            if (reverse_face_polygon.circular_eql?(surfaces[j].face_polygon.transform(transform_j)))

              @last_report << "Match, '#{surfaces[i].name}', '#{surfaces[i].model_object.getString(4,true)}', '#{surfaces[j].name}', '#{surfaces[j].model_object.getString(4,true)}' \n"

              surfaces[i].model_object.setAdjacentSurface(surfaces[j].model_object)

              break
            end
          end
        end

      ensure

        progress_dialog.destroy
        
      end
      
      @last_report << "\nSubSurface Matching Report:\n"
      @last_report << "Action, SubSurface #1, Surface #1, Surface #2, SubSurface #2\n"

      # get all sub surfaces
      sub_surfaces = model_interface.sub_surfaces.to_a
      begin
      
        # create a progress dialog
        progress_dialog = ProgressDialog.new("Matching SubSurfaces")

        # processed and total number of comparisons
        processed_num = 0
        total_num = (sub_surfaces.length * (sub_surfaces.length-1)) / 2

        surface_indices = []
        sub_surfaces.each_index do |i|
          next if not (sub_surfaces[i].is_a?(SubSurface) and
                       sub_surfaces[i].parent.is_a?(Surface) and
                       sub_surfaces[i].parent.parent.is_a?(Space))

          surface_index = surface_names.index(sub_surfaces[i].parent.name)
          surface_indices[i] = surface_index
        end

        # loop over all sub surfaces
        sub_surfaces.each_index do |i|
        
          next if not (sub_surfaces[i].is_a?(SubSurface) and
                       sub_surfaces[i].parent.is_a?(Surface) and
                       sub_surfaces[i].parent.parent.is_a?(Space))
                       
          # get the parents transformation
          transform_i = sub_surfaces[i].parent.parent.coordinate_transformation
          
          # get the polygon, reverse it
          reverse_face_polygon = sub_surfaces[i].face_polygon.reverse.transform(transform_i)
          
          # get the normal
          face_normal = sub_surfaces[i].entity.normal.transform(transform_i)
          
          # don't process empty polygons
          next if reverse_face_polygon.empty?

          # loop over remaining surfaces
          (i+1..sub_surfaces.length-1).each do |j|

            # update number of comparisons
            processed_num += 1
            percent_complete = (100*processed_num)/total_num
            progress_dialog.setValue(percent_complete)
            
            next if not (sub_surfaces[j].is_a?(SubSurface) and
                         sub_surfaces[j].parent.is_a?(Surface) and
                         sub_surfaces[j].parent.parent.is_a?(Space))
          
            # selection must contain either sub surface
            next if not (selection.contains?(sub_surfaces[i].entity) or 
                         selection.contains?(sub_surfaces[i].parent.entity) or 
                         selection.contains?(sub_surfaces[i].parent.parent.entity) or
                         selection.contains?(sub_surfaces[j].entity) or 
                         selection.contains?(sub_surfaces[j].parent.entity) or 
                         selection.contains?(sub_surfaces[j].parent.parent.entity))
                         
            # get the parents transformation
            transform_j = sub_surfaces[j].parent.parent.coordinate_transformation
          
            # check for intersection of base surfaces
            surface_i = surface_indices[i]
            surface_j = surface_indices[j]
            next if not surface_intersections[surface_i][surface_j]
            
            # check normal dot product
            next if not face_normal.dot(sub_surfaces[j].entity.normal.transform(transform_j)) < -0.98
            
            # check if this polygon equals the reverse of the other polygon
            if (reverse_face_polygon.circular_eql?(sub_surfaces[j].face_polygon.transform(transform_j)))

              @last_report << "Match, '#{sub_surfaces[i].name}', '#{sub_surfaces[i].model_object.getString(4,true)}', '#{sub_surfaces[j].name}', '#{sub_surfaces[j].model_object.getString(4,true)}'\n"
              
              sub_surfaces[i].model_object.setAdjacentSubSurface(sub_surfaces[j].model_object)

              break
            end
          end
        end

      ensure
  
        progress_dialog.destroy
        
      end
      
      # switch render mode back to original
      proc = Proc.new { model_interface.materials_interface.rendering_mode = starting_rendermode }
      Plugin.add_event( proc )
      
      Plugin.dialog_manager.inspector_dialog.enable if inspector_dialog_enabled
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
    end 
    
    def unmatch(selection)
    
      @last_report = "Surface Unmatching Report:\n"
      @last_report << "Action, Surface #1, Space #1, Surface #2, Space #2\n"
      
      if selection.empty?
        UI.messagebox("Selection is empty, please select objects for unmatching routine or choose 'Unmatch in Entire Model'.")
        return 
      end
      
      result = UI.messagebox(
"Warning this will unmatch surfaces and subsurfaces 
within and surrounding the selected Spaces.\n
This will also reassign constructions on affected surfaces.\n
This operation cannot be undone. Do you want to continue?", MB_OKCANCEL)

      if result == 2 # cancel
        return false
      end
      
      model_interface = Plugin.model_manager.model_interface

      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      # store starting render mode
      starting_rendermode = model_interface.materials_interface.rendering_mode
      
      # switch render mode to speed things up 
      model_interface.materials_interface.rendering_mode = RenderWaiting

      begin
      
        # create a progress dialog
        progress_dialog = ProgressDialog.new("Unmatching Surfaces")
        
        surfaces =  model_interface.surfaces
        num_total = surfaces.size
        num_complete = 0
            
        surfaces.each do |surface|
          if selection.contains?(surface.entity) or selection.contains?(surface.parent.entity)
            if not surface.model_object.adjacentSurface.empty? 

              other_space_name = ""
              other_surface = surface.model_object.adjacentSurface.get
              other_surface_name = other_surface.name.get
              if not other_surface.space.empty?
                other_space_name = other_surface.space.get.name.get
              end

              surface.model_object.resetAdjacentSurface

              space_name = ""
              if not surface.model_object.space.empty?
                space_name = surface.model_object.space.get.name.get
              end

              @last_report << "Unmatch, '#{surface.name}', '#{space_name}', '#{other_surface_name}', '#{other_space_name}'\n"   
            
            elsif surface.model_object.outsideBoundaryCondition == "Surface"
              
              # this is an error fix it anyway

              surface.model_object.assignDefaultBoundaryCondition
              surface.model_object.assignDefaultSunExposure
              surface.model_object.assignDefaultWindExposure

              space_name = ""
              if not surface.model_object.space.empty?
                space_name = surface.model_object.space.get.name.get
              end

              @last_report << "Unmatch, '#{surface.name}', '#{space_name}', '', ''\n" 
            end
          end
          
          num_complete += 1
          progress_dialog.setValue((100*num_complete)/num_total)
        end
        
      ensure

        progress_dialog.destroy
        
      end

      @last_report << "\nSubSurface Unmatching Report:\n"
      @last_report << "Action, SubSurface #1, Surface #1, SubSurface #2, Surface #2\n"
      
      begin
      
        # create a progress dialog
        progress_dialog = ProgressDialog.new("Unmatching SubSurfaces")
        
        sub_surfaces =  model_interface.sub_surfaces
        num_total = sub_surfaces.size
        num_complete = 0
        
        sub_surfaces.each do |sub_surface|
          if selection.contains?(sub_surface.entity) or selection.contains?(sub_surface.parent.entity) or selection.contains?(sub_surface.parent.parent.entity)
            if not sub_surface.model_object.adjacentSubSurface.empty?

              other_surface_name = ""
              other_subsurface = surface.model_object.adjacentSubSurface.get
              other_subsurface_name = other_subsurface.name.get
              if not other_subsurface.surface.empty?
                other_surface_name = other_subsurface.surface.get.name.get
              end

              sub_surface.model_object.resetAdjacentSubSurface

              surface_name = ""
              if not sub_surface.model_object.surface.empty?
                surface_name = sub_surface.model_object.surface.get.name.get
              end

              @last_report << "Unmatch, '#{sub_surface.name}', '#{surface_name}', '#{other_subsurface_name}', '#{other_surface_name}'\n"

            end
          end
          
          num_complete += 1
          progress_dialog.setValue((100*num_complete)/num_total)
        
        end  
        
      ensure

        progress_dialog.destroy
        
      end
      
      # switch render mode back to original
      proc = Proc.new { model_interface.materials_interface.rendering_mode = starting_rendermode }
      Plugin.add_event( proc )
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
    end
    
    def on_last_report
      if (Plugin.platform == Platform_Windows)
        Plugin.dialog_manager.show(LastReportInterface)
        Plugin.dialog_manager.active_interface(LastReportInterface).last_report = @last_report
      else
        # mac last report web dialog not working, puts to ruby console or messagebox as a work around
        UI.messagebox @last_report,MB_MULTILINE
      end
    end
    
    def on_cancel
      close
    end

  end
  
end
