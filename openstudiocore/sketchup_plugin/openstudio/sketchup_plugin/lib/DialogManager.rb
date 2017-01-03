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

require("set")

module OpenStudio

  class DialogManager

    attr_accessor :dialog_interfaces, :inspector_dialog

    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @dialog_interfaces = Set.new
      
      @inspector_dialog = PluginInspectorDialog.new
      @inspector_dialog.hide

      update_units
      selection_changed
      restore_state
    end

    def save_state
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      interface_names = @dialog_interfaces.collect { |interface| interface.class.to_s[12..-1] }  # Clip the "OpenStudio::" part
      Plugin.write_pref('Open Dialogs', interface_names.to_a.join(','))
      Plugin.write_pref('Inspector Dialog Visible', @inspector_dialog.isVisible)     
      @inspector_dialog.saveState
    end

    def restore_state
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      interface_names = Plugin.read_pref('Open Dialogs').split(',')
      interface_names.each do |interface_name| 
        proc = Proc.new do 
          begin
            show(OpenStudio.const_get(interface_name))
          rescue
            # interface_name does not map to a current class
          end
        end
        Plugin.add_event(proc)
      end
      
      inspector_dialog_visible = Plugin.read_pref('Inspector Dialog Visible')
      if inspector_dialog_visible
        @inspector_dialog.restoreState
        @inspector_dialog.show
      end
    end

    def active_interface(interface_class)
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if interface_class == PluginInspectorDialog
        return @inspector_dialog
      end
      return(@dialog_interfaces.find { |interface| interface.class == interface_class })
    end

    def show(interface_class)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (not interface = active_interface(interface_class))
        interface = interface_class.new
        @dialog_interfaces.add(interface)
      end
      
      interface.show
      save_state
    end

    def validate(interface_class)
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = MF_GRAYED
      if model_manager = Plugin.model_manager
        if model_manager.model_interface
          if (active_interface(interface_class))
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
      end
      return(result)
    end


    def update(interface_class)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (interface = active_interface(interface_class))
        interface.update
      end
    end


    def update_all
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @inspector_dialog.update
      @dialog_interfaces.each { |interface| interface.update }
    end
    
    def units_system
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @units_system
    end
    
    def update_units
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      puts "current @units_system = #{@units_system}"
      
      @units_system = Plugin.read_pref("Unit System")
      
      puts "new @units_system = #{@units_system}"
      
      if (@units_system == "IP")
        @inspector_dialog.displayIP(true)
      else
        @inspector_dialog.displayIP(false)
      end
      
    end    

    def remove(interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @dialog_interfaces.delete(interface)
      save_state
    end


    def close_all
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      save_state
      
      @inspector_dialog.hide
      
      @dialog_interfaces.each do |interface| 
        proc = Proc.new { interface.close }
        Plugin.add_event( proc )
      end
    end
    
    
    def selection_changed
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    
      was_dialog_enabled = @inspector_dialog.disable
    
      # updates the model
      @inspector_dialog.update
      
      model_interface = Plugin.model_manager.model_interface
      
      # drawing_interface may be changed if another object is displayed in the inspector
      drawing_interface = model_interface.selection_interface.selected_drawing_interface
      
      idd_object_type = nil
      handles = nil
      if drawing_interface

        # default is to select the current model object
        model_object = drawing_interface.model_object
        idd_object_type = model_object.iddObject.type
        handles = OpenStudio::UUIDVector.new
        handles << model_object.handle   
        
        # modify idd_object_type amd handles based on render mode and render_defaults
        rendering_mode = model_interface.materials_interface.rendering_mode
        render_defaults = model_interface.materials_interface.render_defaults
        if (rendering_mode == RenderWaiting)
          # no-op
        elsif (rendering_mode == RenderByClass)
          # no-op
        elsif (rendering_mode == RenderByDataValue)
          # no-op
        elsif (rendering_mode == RenderByBoundary)
          # no-op
        elsif (rendering_mode == RenderByLayer)
          # no-op
        elsif (rendering_mode == RenderBySurfaceNormal)
          # no-op
        elsif (rendering_mode == RenderByConstruction)
          # select surface since there are no more construction objects in the inspector
        elsif (rendering_mode == RenderBySpaceType)
        
          space = nil
          space_type = nil
          if model_object.is_a?(OpenStudio::Model::Space)
            space = model_object
          elsif model_object.is_a?(OpenStudio::Model::PlanarSurface) or
                model_object.is_a?(OpenStudio::Model::SpaceItem) or
                model_object.is_a?(OpenStudio::Model::ShadingSurfaceGroup) or 
                model_object.is_a?(OpenStudio::Model::InteriorPartitionSurfaceGroup)
            o_space = model_object.space
            if not o_space.empty?
              space = o_space.get
            end
          end
          
          if space
            if (not space.isSpaceTypeDefaulted) or render_defaults
              space_type = space.spaceType
            end
          end
            
          if space_type and (not space_type.empty?)
            #drawing_interface = space_type.get.drawing_interface
            drawing_interface = nil
            idd_object_type = space_type.get.iddObject.type
            handles = OpenStudio::UUIDVector.new
            handles << space_type.get.handle  
          end

        elsif (rendering_mode == RenderByThermalZone)

          space = nil
          thermal_zone = nil
          if model_object.is_a?(OpenStudio::Model::Space)
            space = model_object
          elsif model_object.is_a?(OpenStudio::Model::PlanarSurface) or
                model_object.is_a?(OpenStudio::Model::SpaceItem) or
                model_object.is_a?(OpenStudio::Model::ShadingSurfaceGroup) or 
                model_object.is_a?(OpenStudio::Model::InteriorPartitionSurfaceGroup)
            o_space = model_object.space
            if not o_space.empty?
              space = o_space.get
            end
          end
          
          if space
            thermal_zone = space.thermalZone
          end
            
          if thermal_zone and (not thermal_zone.empty?)
            #drawing_interface = thermal_zone.get.drawing_interface
            drawing_interface = nil
            idd_object_type = thermal_zone.get.iddObject.type
            handles = OpenStudio::UUIDVector.new
            handles << thermal_zone.get.handle  
          end
              
        elsif (rendering_mode == RenderByBuildingStory)

          space = nil
          building_story = nil
          if model_object.is_a?(OpenStudio::Model::Space)
            space = model_object
          elsif model_object.is_a?(OpenStudio::Model::PlanarSurface) or
                model_object.is_a?(OpenStudio::Model::SpaceItem) or
                model_object.is_a?(OpenStudio::Model::ShadingSurfaceGroup) or 
                model_object.is_a?(OpenStudio::Model::InteriorPartitionSurfaceGroup)
            o_space = model_object.space
            if not o_space.empty?
              space = o_space.get
            end
          end
          
          if space
            building_story = space.buildingStory
          end
            
          if building_story and (not building_story.empty?)
            #drawing_interface = building_story.get.drawing_interface
            drawing_interface = nil
            idd_object_type = building_story.get.iddObject.type
            handles = OpenStudio::UUIDVector.new
            handles << building_story.get.handle  
          end

        end

      else
      
        # select nothing
        drawing_interface = nil
        idd_object_type = nil
        handles = OpenStudio::UUIDVector.new   
      end
      
      # prevent watch from firing if inspector changes the object
      watcher_enabled = drawing_interface.disable_watcher if drawing_interface
      
      if idd_object_type
        result = @inspector_dialog.setIddObjectType(idd_object_type)
        Plugin.log(OpenStudio::Debug, "selection_changed: setting iddObjectType to #{idd_object_type}, result = #{result.to_s}")
      end
      
      if handles
        result = @inspector_dialog.setSelectedObjectHandles(handles)
        Plugin.log(OpenStudio::Debug, "selection_changed: setting setSelectedObjectHandles to #{handles.size.to_s}, result = #{result.to_s}")
      end
      
      drawing_interface.enable_watcher if watcher_enabled
      
      @inspector_dialog.enable if was_dialog_enabled
    end

  end

end
