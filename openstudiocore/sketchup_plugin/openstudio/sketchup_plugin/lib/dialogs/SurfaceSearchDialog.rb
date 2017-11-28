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

require("openstudio/sketchup_plugin/lib/dialogs/Dialogs")
require("openstudio/sketchup_plugin/lib/dialogs/LastReportInterface")

module OpenStudio

  class SurfaceSearchDialog < PropertiesDialog

    def initialize(container, interface, hash)
      super
      @container = WindowContainer.new("Surface Search", 550, 375, 150, 150)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/SurfaceSearch.html")
      
      @hash['CLASS'] = ""
      @hash['NAME'] = ""
      @hash['TYPE'] = ""
      @hash['CONSTRUCTION'] = ""
      @hash['OUTSIDE_BOUNDARY_CONDITION'] = ""
      @hash['SUN'] = ""
      @hash['WIND'] = ""
      #@hash['SHADING_CONTROL_NAME'] = ""
      #@hash['FRAME_AND_DIVIDER_NAME'] = ""
      @hash['AZIMUTH_FROM'] = "0"
      @hash['AZIMUTH_TO'] = "360"
      @hash['TILT_FROM'] = "0"
      @hash['TILT_TO'] = "180"
      @hash['EXCLUDE_HORIZONTAL_SURFACES'] = false
      @hash['NON_CONVEX_SURFACES'] = false
      @hash['HIGH_POLY_SURFACES'] = false
      @hash['VERTEX_LIMIT'] = "4"
      @hash['SCENE_NAME'] = ""
      
      @last_report = ""
      
      add_callbacks
    end


    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_change_class") { on_change_class }
      @container.web_dialog.add_action_callback("on_search_model") { on_search_model }
      @container.web_dialog.add_action_callback("on_search_selection") { on_search_selection }
      @container.web_dialog.add_action_callback("on_unhide_all") { on_unhide_all }
      @container.web_dialog.add_action_callback("on_last_report") { on_last_report }
      @container.web_dialog.add_action_callback("on_cancel") { on_cancel }
      @container.web_dialog.add_action_callback("on_change_azimuth") { on_change_azimuth }
      @container.web_dialog.add_action_callback("on_high_poly_surfaces") { on_high_poly_surfaces }
    end
    
    def on_load
      super
      set_select_options("CLASS", ["", "OS:Surface", "OS:SubSurface", "OS:ShadingSurface",  "OS:InteriorPartitionSurface"])
      on_change_class
      disable_element("VERTEX_LIMIT")
    end 
    
    def on_change_azimuth
      if @hash['AZIMUTH_FROM'].to_f == 0 and @hash['AZIMUTH_TO'].to_f == 360
        @hash['EXCLUDE_HORIZONTAL_SURFACES'] = false
      else
        @hash['EXCLUDE_HORIZONTAL_SURFACES'] = true
      end
      update
    end

    def on_high_poly_surfaces
      if @hash['HIGH_POLY_SURFACES']
        enable_element("VERTEX_LIMIT")
      elsif
        disable_element("VERTEX_LIMIT")
      end
    end

    def on_change_class

      case (@hash['CLASS'])

      when ""
      
        enable_element("CLASS")
        enable_element("NAME")
        enable_element("CONSTRUCTION")
        enable_element("EXCLUDE_HORIZONTAL_SURFACES")
        enable_element("NON_CONVEX_SURFACES")
        enable_element("HIGH_POLY_SURFACES")

        @hash['TYPE'] = ""
        disable_element("TYPE")
        @hash['OUTSIDE_BOUNDARY_CONDITION'] = ""
        disable_element("OUTSIDE_BOUNDARY_CONDITION")
        @hash['SUN'] = ""
        disable_element("SUN")
        @hash['WIND'] = ""
        disable_element("WIND")
        #@hash['SHADING_CONTROL_NAME'] = ""
        #disable_element("SHADING_CONTROL_NAME")
        #@hash['FRAME_AND_DIVIDER_NAME'] = ""
        #disable_element("FRAME_AND_DIVIDER_NAME")
        
        object_names = Plugin.model_manager.model_interface.openstudio_model.getConstructionBases.collect { |object| object.name.get }
        object_names = [""].concat(object_names.sort)
        set_select_options("CONSTRUCTION", object_names)

        update
      
      when "OS:Surface"

        enable_element("CLASS")
        enable_element("NAME")
        enable_element("TYPE")
        enable_element("CONSTRUCTION")
        enable_element("OUTSIDE_BOUNDARY_CONDITION")
        enable_element("SUN")
        enable_element("WIND")
        enable_element("EXCLUDE_HORIZONTAL_SURFACES")
        enable_element("NON_CONVEX_SURFACES")
        enable_element("HIGH_POLY_SURFACES")
       
        #@hash['SHADING_CONTROL_NAME'] = ""
        #disable_element("SHADING_CONTROL_NAME")
        #@hash['FRAME_AND_DIVIDER_NAME'] = ""
        #disable_element("FRAME_AND_DIVIDER_NAME")
                
        set_select_options("TYPE", [""].concat(OpenStudio::Model::Surface.validSurfaceTypeValues))
        
        object_names = Plugin.model_manager.model_interface.openstudio_model.getConstructionBases.collect { |object| object.name.get }
        object_names = [""].concat(object_names.sort)
        set_select_options("CONSTRUCTION", object_names)
        set_select_options("OUTSIDE_BOUNDARY_CONDITION", [""].concat(OpenStudio::Model::Surface.validOutsideBoundaryConditionValues))
        set_select_options("SUN", [""].concat(OpenStudio::Model::Surface.validSunExposureValues))
        set_select_options("WIND", [""].concat(OpenStudio::Model::Surface.validWindExposureValues))
        #set_select_options("SHADING_CONTROL_NAME", [""])
        #set_select_options("FRAME_AND_DIVIDER_NAME", [""])

        update

      when "OS:SubSurface"

        enable_element("CLASS")
        enable_element("NAME")
        enable_element("TYPE")
        enable_element("CONSTRUCTION")
        #enable_element("SHADING_CONTROL_NAME")
        #enable_element("FRAME_AND_DIVIDER_NAME")
        enable_element("EXCLUDE_HORIZONTAL_SURFACES")
        enable_element("NON_CONVEX_SURFACES")
        enable_element("HIGH_POLY_SURFACES")
       
        @hash['OUTSIDE_BOUNDARY_CONDITION'] = ""
        disable_element("OUTSIDE_BOUNDARY_CONDITION")
        @hash['SUN'] = ""
        disable_element("SUN")
        @hash['WIND'] = ""
        disable_element("WIND")
        
        set_select_options("TYPE", [""].concat(OpenStudio::Model::SubSurface.validSubSurfaceTypeValues))
        
        object_names = Plugin.model_manager.model_interface.openstudio_model.getConstructionBases.collect { |object| object.name.get }
        object_names = [""].concat(object_names.sort)
        set_select_options("CONSTRUCTION", object_names)
        
        set_select_options("OUTSIDE_BOUNDARY_CONDITION", [""])
        set_select_options("SUN", [""])
        set_select_options("WIND", [""])

        #object_names = Plugin.model_manager.model_interface.openstudio_model.getObjectsByType("WindowProperty:ShadingControl".to_IddObjectType).collect { |object| object.name.get }
        #object_names = [""].concat(object_names.sort)
        #set_select_options("SHADING_CONTROL_NAME", object_names)

        #object_names = Plugin.model_manager.model_interface.openstudio_model.getObjectsByType("WindowProperty:FrameAndDivider".to_IddObjectType).collect { |object| object.name.get }
        #object_names = [""].concat(object_names.sort)
        #set_select_options("FRAME_AND_DIVIDER_NAME", object_names)
        
        update
        
      when "OS:ShadingSurface"
      
        enable_element("CLASS")
        enable_element("NAME")
        enable_element("EXCLUDE_HORIZONTAL_SURFACES")
        enable_element("NON_CONVEX_SURFACES")
        enable_element("HIGH_POLY_SURFACES")
        enable_element("CONSTRUCTION")

        @hash['TYPE'] = ""
        disable_element("TYPE")
        @hash['OUTSIDE_BOUNDARY_CONDITION'] = ""
        disable_element("OUTSIDE_BOUNDARY_CONDITION")
        @hash['SUN'] = ""
        disable_element("SUN")
        @hash['WIND'] = ""
        disable_element("WIND")
        #@hash['SHADING_CONTROL_NAME'] = ""
        #disable_element("SHADING_CONTROL_NAME")
        #@hash['FRAME_AND_DIVIDER_NAME'] = ""
        #disable_element("FRAME_AND_DIVIDER_NAME")
        
        set_select_options("TYPE", [""])
        set_select_options("CONSTRUCTION", [""])
        set_select_options("OUTSIDE_BOUNDARY_CONDITION", [""])
        set_select_options("SUN", [""])
        set_select_options("WIND", [""])
        #set_select_options("SHADING_CONTROL_NAME", [""])
        #set_select_options("FRAME_AND_DIVIDER_NAME", [""])

        update
        
      when "OS:InteriorPartitionSurface"
      
        enable_element("CLASS")
        enable_element("NAME")
        enable_element("EXCLUDE_HORIZONTAL_SURFACES")
        enable_element("NON_CONVEX_SURFACES")
        enable_element("HIGH_POLY_SURFACES")
        enable_element("CONSTRUCTION")

        @hash['TYPE'] = ""
        disable_element("TYPE")
        @hash['OUTSIDE_BOUNDARY_CONDITION'] = ""
        disable_element("OUTSIDE_BOUNDARY_CONDITION")
        @hash['SUN'] = ""
        disable_element("SUN")
        @hash['WIND'] = ""
        disable_element("WIND")
        #@hash['SHADING_CONTROL_NAME'] = ""
        #disable_element("SHADING_CONTROL_NAME")
        #@hash['FRAME_AND_DIVIDER_NAME'] = ""
        #disable_element("FRAME_AND_DIVIDER_NAME")
        
        set_select_options("TYPE", [""])

        object_names = Plugin.model_manager.model_interface.openstudio_model.getConstructionBases.collect { |object| object.name.get }
        object_names = [""].concat(object_names.sort)
        set_select_options("CONSTRUCTION", object_names)

        set_select_options("OUTSIDE_BOUNDARY_CONDITION", [""])
        set_select_options("SUN", [""])
        set_select_options("WIND", [""])
        #set_select_options("SHADING_CONTROL_NAME", [""])
        #set_select_options("FRAME_AND_DIVIDER_NAME", [""])

        update
        
      end
      
    end

    def apply_surface_orientation_filter(planar_surfaces)

      if not ((@hash['AZIMUTH_FROM'] == "0" and @hash['AZIMUTH_TO'] == "360") or (@hash['AZIMUTH_FROM'] == "" and @hash['AZIMUTH_TO'] == ""))
        azimuth_from = OpenStudio::OptionalDouble.new
        azimuth_to = OpenStudio::OptionalDouble.new
        
        # if statement to support negative input numbers in "from" field
        if @hash['AZIMUTH_FROM'].to_f < 0
          azimuth_from = OpenStudio::OptionalDouble.new( @hash['AZIMUTH_FROM'].to_f + 360 )
        else
          azimuth_from = OpenStudio::OptionalDouble.new( @hash['AZIMUTH_FROM'].to_f )
        end

        # if statement to support negative input numbers in "to" field
        if @hash['AZIMUTH_TO'].to_f < 0
          azimuth_to = OpenStudio::OptionalDouble.new( @hash['AZIMUTH_TO'].to_f + 360 )
        else
          azimuth_to = OpenStudio::OptionalDouble.new( @hash['AZIMUTH_TO'].to_f )
        end

        # if statements to convert supporte text to numbers from "from" field
        if @hash['AZIMUTH_FROM'].upcase == "N" or @hash['AZIMUTH_FROM'].upcase == "NORTH"
          azimuth_from = OpenStudio::OptionalDouble.new(0)
        elsif @hash['AZIMUTH_FROM'].upcase == "S" or @hash['AZIMUTH_FROM'].upcase == "SOUTH"
          azimuth_from = OpenStudio::OptionalDouble.new(180)
        elsif @hash['AZIMUTH_FROM'].upcase == "E" or @hash['AZIMUTH_FROM'].upcase == "EAST"
          azimuth_from = OpenStudio::OptionalDouble.new(90)
        elsif @hash['AZIMUTH_FROM'].upcase == "W" or @hash['AZIMUTH_FROM'].upcase == "WEST"
          azimuth_from = OpenStudio::OptionalDouble.new(270)
        elsif @hash['AZIMUTH_FROM'].upcase == "NE" or @hash['AZIMUTH_FROM'].upcase == "NORTHEAST" or @hash['AZIMUTH_FROM'].upcase == "NORTH EAST"
          azimuth_from = OpenStudio::OptionalDouble.new(45)
        elsif @hash['AZIMUTH_FROM'].upcase == "SE" or @hash['AZIMUTH_FROM'].upcase == "SOUTHEAST" or @hash['AZIMUTH_FROM'].upcase == "SOUTH EAST"
          azimuth_from = OpenStudio::OptionalDouble.new(135)
        elsif @hash['AZIMUTH_FROM'].upcase == "SW" or @hash['AZIMUTH_FROM'].upcase == "SOUTHWEST" or @hash['AZIMUTH_FROM'].upcase == "SOUTH WEST"
          azimuth_from = OpenStudio::OptionalDouble.new(225)
        elsif @hash['AZIMUTH_FROM'].upcase == "NW" or @hash['AZIMUTH_FROM'].upcase == "NORTHWEST" or @hash['AZIMUTH_FROM'].upcase == "NORTH WEST"
          azimuth_from = OpenStudio::OptionalDouble.new(315)
        end

        # if statements to convert supporte text to numbers from "to" field
        if @hash['AZIMUTH_TO'].upcase == "N" or @hash['AZIMUTH_TO'].upcase == "NORTH"
          azimuth_to = OpenStudio::OptionalDouble.new(0)
        elsif @hash['AZIMUTH_TO'].upcase == "S" or @hash['AZIMUTH_TO'].upcase == "SOUTH"
          azimuth_to = OpenStudio::OptionalDouble.new(180)
        elsif @hash['AZIMUTH_TO'].upcase == "E" or @hash['AZIMUTH_TO'].upcase == "EAST"
          azimuth_to = OpenStudio::OptionalDouble.new(90)
        elsif @hash['AZIMUTH_TO'].upcase == "W" or @hash['AZIMUTH_TO'].upcase == "WEST"
          azimuth_to = OpenStudio::OptionalDouble.new(270)
        elsif @hash['AZIMUTH_TO'].upcase == "NE" or @hash['AZIMUTH_TO'].upcase == "NORTHEAST" or @hash['AZIMUTH_TO'].upcase == "NORTH EAST"
          azimuth_to = OpenStudio::OptionalDouble.new(45)
        elsif @hash['AZIMUTH_TO'].upcase == "SE" or @hash['AZIMUTH_TO'].upcase == "SOUTHEAST" or @hash['AZIMUTH_TO'].upcase == "SOUTH EAST"
          azimuth_to = OpenStudio::OptionalDouble.new(135)
        elsif @hash['AZIMUTH_TO'].upcase == "SW" or @hash['AZIMUTH_TO'].upcase == "SOUTHWEST" or @hash['AZIMUTH_TO'].upcase == "SOUTH WEST"
          azimuth_to = OpenStudio::OptionalDouble.new(225)
        elsif @hash['AZIMUTH_TO'].upcase == "NW" or @hash['AZIMUTH_TO'].upcase == "NORTHWEST" or @hash['AZIMUTH_TO'].upcase == "NORTH WEST"
          azimuth_to = OpenStudio::OptionalDouble.new(315)
        end
        
        # get model objects to search
        model_objects = nil
        if (@hash['CLASS'] == "")
          model_objects = Plugin.model_manager.model_interface.openstudio_model.getPlanarSurfaces
        elsif (@hash['CLASS'] == "OS:Surface")
          model_objects = Plugin.model_manager.model_interface.openstudio_model.getSurfaces
        elsif (@hash['CLASS'] == "OS:SubSurface")
          model_objects = Plugin.model_manager.model_interface.openstudio_model.getSubSurfaces
        elsif (@hash['CLASS'] == "OS:ShadingSurface")
          model_objects = Plugin.model_manager.model_interface.openstudio_model.getShadingSurfaces
        elsif (@hash['CLASS'] == "OS:InteriorPartitionSurface")
          model_objects = Plugin.model_manager.model_interface.openstudio_model.getInteriorPartitionSurfaces
        end
        
        model_objects = OpenStudio::Model::PlanarSurface::findPlanarSurfaces(model_objects, azimuth_from, azimuth_to, OpenStudio::OptionalDouble.new, OpenStudio::OptionalDouble.new, 0.5)
        
        # reject surfaces not in results
        planar_surfaces.reject! { |planar_surface| not model_objects.include?(planar_surface.model_object) }

      end

      return true
    end

    def horizontal_test(model_object)
      if @hash['EXCLUDE_HORIZONTAL_SURFACES']
        tilt = model_object.tilt.radians
        if tilt >= 179.9 or tilt <= 0.1
          return false
        end
      end
      return true
    end
    
    def non_convex_test(drawing_interface)
      if @hash['NON_CONVEX_SURFACES']
        face = drawing_interface.entity
        loop = face.outer_loop
        status = loop.convex?
        
        # if face is convex skip over it
        if status
          return false
        else
          # failed first convex test, run second one
          pointstemp = drawing_interface.model_object_polygon.points
          skp_model = Sketchup.active_model
          entities = skp_model.active_entities
          facetemp = entities.add_face pointstemp
          loop2 = facetemp.outer_loop
          status2 = loop2.convex?
          # erase facetemp
          facetempall = facetemp.all_connected
          entities.erase_entities facetempall
          if status2
            # next statement skips the rest of this if face is convex
            return false
          end
        end
      end
      return true
    end

    def high_poly_surfaces_test(model_object)
      if @hash['HIGH_POLY_SURFACES']
        # if face vertex count is below limit skip over it
        if @hash['VERTEX_LIMIT'].to_f >= model_object.vertices.size
          return false
        end
      end

      return true
    end

    def surface_name_test(model_object)
      name = @hash["NAME"].upcase
      if not model_object.name.get.upcase.include?(name)
        return false
      end

      return true
    end

    def on_search_model
      model_interface = Plugin.model_manager.model_interface
      skp_model = model_interface.skp_model
      
      model_interface.remove_observers(true)
      skp_model.selection.clear
      skp_model.entities.each {|e| skp_model.selection.add(e)}
      selected_entities = search(skp_model.selection)   
      skp_model.selection.clear
      model_interface.add_observers(true)

      skp_model.selection.add(selected_entities)
    end
    
    def on_search_selection
      model_interface = Plugin.model_manager.model_interface
      skp_model = model_interface.skp_model
    
      model_interface.remove_observers(true)
      selected_entities = search(skp_model.selection)
      skp_model.selection.clear
      model_interface.add_observers(true)
      
      skp_model.selection.add(selected_entities)
    end
    
    def search(selection)
      
      if selection.empty?
        UI.messagebox("Selection is empty, please select objects for searching or choose 'Search in Entire Model'.")
        return []
      end
      
      @last_report = "Search results:\n"
      
      selected_entities = []
      
      model_interface = Plugin.model_manager.model_interface
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      model_interface.remove_observers(true)
      
      skp_model = model_interface.skp_model

      begin
      
        model_interface.start_operation("Surface Search", true)
        
        progress_dialog = ProgressDialog.new("Searching Surfaces")

        # hide all groups, faces, and edges
        model_interface.spaces.each { |group| group.entity.hidden = true }
        model_interface.shading_surface_groups.each { |group| group.entity.hidden = true }
        model_interface.interior_partition_surface_groups.each { |group| group.entity.hidden = true }
        model_interface.illuminance_maps.each { |group| group.entity.hidden = true }      
        model_interface.daylighting_controls.each { |group| group.entity.hidden = true }      
        model_interface.luminaires.each { |group| group.entity.hidden = true }

        model_interface.surfaces.each do |face| 
          face.entity.hidden = true 
          face.entity.edges.each { |edge| edge.hidden = true }
        end
        model_interface.sub_surfaces.each do |face| 
          face.entity.hidden = true 
          face.entity.edges.each { |edge| edge.hidden = true }
        end
        model_interface.shading_surfaces.each do |face| 
          face.entity.hidden = true 
          face.entity.edges.each { |edge| edge.hidden = true }
        end
        model_interface.interior_partition_surfaces.each do |face| 
          face.entity.hidden = true 
          face.entity.edges.each { |edge| edge.hidden = true }
        end

        # upcase search criteria
        type = @hash["TYPE"].upcase
        construction = @hash["CONSTRUCTION"].upcase
        outside_boundary_condition = @hash["OUTSIDE_BOUNDARY_CONDITION"].upcase
        sun = @hash["SUN"].upcase
        wind = @hash["WIND"].upcase
        #shading_control = @hash['SHADING_CONTROL_NAME'].upcase
        #frame_and_divider = @hash['FRAME_AND_DIVIDER_NAME'].upcase
        
        # populate list of planar surfaces to search
        planar_surfaces = nil
        if (@hash['CLASS'] == "")
          planar_surfaces = model_interface.all_surfaces.to_a
        elsif (@hash['CLASS'] == "OS:Surface")
          planar_surfaces = model_interface.surfaces.to_a
        elsif (@hash['CLASS'] == "OS:SubSurface")
          planar_surfaces = model_interface.sub_surfaces.to_a
        elsif (@hash['CLASS'] == "OS:ShadingSurface")
          planar_surfaces = model_interface.shading_surfaces.to_a
        elsif (@hash['CLASS'] == "OS:InteriorPartitionSurface")
          planar_surfaces = model_interface.interior_partition_surfaces.to_a
        end
        
        # start the progress bar
        num_surfaces = planar_surfaces.length
        
        # reject surfaces not in selection
        planar_surfaces.reject! { |planar_surface| not planar_surface.in_selection?(selection) }
        
        # do angular search
        apply_surface_orientation_filter(planar_surfaces)
        
        # loop over surfaces
        planar_surfaces.each_index do |index|
        
          progress_dialog.setValue((100*index)/num_surfaces)
          
          planar_surface = planar_surfaces[index]
          model_object = planar_surface.model_object
        
          # common tests
          if not horizontal_test(model_object)
            next
          end
          
          if not non_convex_test(planar_surface)
            next
          end

          if not high_poly_surfaces_test(model_object)
            next
          end

          if not surface_name_test(model_object)
            next
          end
          
          if ((planar_surface.class == Surface) and (@hash['CLASS'] == "" or @hash['CLASS'] == "OS:Surface"))
            if model_object.getString(2,true).to_s.upcase.include?(type) and
               model_object.getString(3,true).to_s.upcase.include?(construction) and
               model_object.getString(5,true).to_s.upcase.include?(outside_boundary_condition) and
               model_object.getString(7,true).to_s.upcase.include?(sun) and
               model_object.getString(8,true).to_s.upcase.include?(wind) #and
               #shading_control.empty? and frame_and_divider.empty? 

               # unhide face
               planar_surface.entity.visible = true    
               selected_entities << planar_surface.entity

               # unhide edges
               planar_surface.entity.edges.each {|edge| edge.visible = true }

               # unhide space
               planar_surface.parent.entity.visible = true      
               
               # add to report
               @last_report << "#{model_object.iddObject.name.to_s}, #{model_object.name.to_s}\n"

            end
          elsif ((planar_surface.class == SubSurface) and (@hash['CLASS'] == "" or @hash['CLASS'] == "OS:SubSurface"))
            if model_object.getString(2,true).to_s.upcase.include?(type) and
               model_object.getString(3,true).to_s.upcase.include?(construction) and
               outside_boundary_condition.empty? and sun.empty? and wind.empty? #and
               #model_object.getString(7,true).to_s.upcase.include?(shading_control)
               #model_object.getString(8,true).to_s.upcase.include?(frame_and_divider)

               # unhide face
               planar_surface.entity.visible = true  
               selected_entities << planar_surface.entity

               # unhide edges
               planar_surface.entity.edges.each {|edge| edge.visible = true }

               # unhide base surface
               #planar_surface.parent.entity.visible = true   

               # unhide base surface edges
               planar_surface.parent.entity.edges.each {|edge| edge.visible = true }

               # unhide space
               planar_surface.parent.parent.entity.visible = true   
               
               # add to report
               @last_report << "#{model_object.iddObject.name.to_s}, #{model_object.name.to_s}\n"
            end
          elsif ((planar_surface.class == ShadingSurface) and (@hash['CLASS'] == "" or @hash['CLASS'] == "OS:ShadingSurface"))
            if type.empty? and construction.empty? and
               outside_boundary_condition.empty? and sun.empty? and wind.empty? #and
               #shading_control.empty? and frame_and_divider.empty?

                # unhide face
               planar_surface.entity.visible = true 
               selected_entities << planar_surface.entity

               # unhide edges
               planar_surface.entity.edges.each {|edge| edge.visible = true }
               
               # unhide shading surface group
               planar_surface.parent.entity.visible = true

               # unhide space
               planar_surface.parent.parent.entity.visible = true if planar_surface.parent.parent.entity   
               
               # add to report
               @last_report << "#{model_object.iddObject.name.to_s}, #{model_object.name.to_s}\n"
            end
          elsif ((planar_surface.class == InteriorPartitionSurface) and (@hash['CLASS'] == "" or @hash['CLASS'] == "OS:InteriorPartitionSurface"))
            if type.empty? and
               model_object.getString(2,true).to_s.upcase.include?(construction) and
               outside_boundary_condition.empty? and sun.empty? and wind.empty? #and
               #shading_control.empty? and frame_and_divider.empty?

                # unhide face
               planar_surface.entity.visible = true  
               selected_entities << planar_surface.entity

               # unhide edges
               planar_surface.entity.edges.each {|edge| edge.visible = true }

               # unhide interior partition surface group
               planar_surface.parent.entity.visible = true    

               # unhide space
               planar_surface.parent.parent.entity.visible = true

               # add to report
               @last_report << "#{model_object.iddObject.name.to_s}, #{model_object.name.to_s}\n"
            end
          end
        end
        
      ensure
      
        model_interface.commit_operation 
        progress_dialog.destroy
      
      end
      
      model_interface.add_observers(true)
      
      return selected_entities
    end
    
    def on_unhide_all
    
      # unhide all
      model_interface = Plugin.model_manager.model_interface
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      model_interface.remove_observers(true) 
      
      skp_model = model_interface.skp_model
      
      begin
      
        model_interface.start_operation("Unhide All", true)

        model_interface.spaces.each { |group| group.entity.visible = true }
        model_interface.shading_surface_groups.each { |group| group.entity.visible = true }
        model_interface.interior_partition_surface_groups.each { |group| group.entity.visible = true }      
        model_interface.illuminance_maps.each { |group| group.entity.visible = true }      
        model_interface.daylighting_controls.each { |group| group.entity.visible = true }      
        model_interface.luminaires.each { |group| group.entity.visible = true }
        model_interface.all_surfaces.each do |face| 
          face.entity.visible = true 
          face.entity.edges.each { |edge| edge.visible = true }
        end
        
      ensure
      
        model_interface.commit_operation
        
      end
      
      model_interface.add_observers(true) 
      
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
