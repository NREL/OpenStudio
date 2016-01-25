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

require("openstudio/sketchup_plugin/lib/observers/SelectionObserver.rb")

module OpenStudio
  
  class SelectionInterface
  
    # for debugging
    attr_reader :model_interface, :observer, :selection

    def initialize(model_interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @model_interface = model_interface
      @selection = @model_interface.skp_model.selection
      
      @observer = SelectionObserver.new(self)    
      @observer_added = false # true if observer has been added to the entity
    end
    
    def destroy
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @model_interface = nil
      @selection = nil
      @observer = nil
    end
    
    def add_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
        if not @observer_added
          @selection.add_observer(@observer)
          @observer_added = true
        end
        @observer.enable
      else
        @selection.add_observer(@observer)
        @observer_added = true
        @observer.enable
      end
    end

    def remove_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      had_observers = false
      if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
        if @observer_added
          had_observers = @observer.disable
        end
      else
        had_observers = @selection.remove_observer(@observer)
        @observer.disable
        @observer_added = false
      end
      
      return had_observers
    end
    
    def destroy_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      if @observer
        if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
          # actually do remove here
          @selection.remove_observer(@observer)
          @observer.disable
          @observer_added = false
        else
          @selection.remove_observer(@observer)
          @observer.disable
          @observer_added = false
        end
        @observer.destroy
        @observer = nil
        result = true
      end
      
      return result
    end
    
    # gets the drawing_interface which is actually selected, not render mode aware
    # render mode is applied in DialogManager::selection_changed
    def selected_drawing_interface
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = nil
      if (@selection.empty?)
        Plugin.log(OpenStudio::Debug, "selection is empty")
        
        parent = @model_interface.skp_model.active_entities.parent
        if (parent.class == Sketchup::ComponentDefinition)
          # Gets the SurfaceGroup interface that is currently open for editing
          drawing_interface = parent.instances.first.drawing_interface
          
          Plugin.log(OpenStudio::Debug, "selected_drawing_interface = #{drawing_interface}")
        else
          drawing_interface = @model_interface.building
          
          Plugin.log(OpenStudio::Debug, "selected_drawing_interface = #{drawing_interface}")
        end

      else
        @selection.each do |entity|
          if (entity.drawing_interface and not entity.drawing_interface.deleted? and (entity.class == Sketchup::Group or entity.class == Sketchup::Face or entity.class == Sketchup::ComponentInstance))

            # Check for entities that have been copied into a non-OpenStudio group and clean them.
            if (entity.parent.class == Sketchup::ComponentDefinition and not entity.parent.instances.first.drawing_interface)
              entity.drawing_interface = nil
              entity.model_object_handle = nil
            end

            if drawing_interface.nil?
              drawing_interface = entity.drawing_interface
              
              Plugin.log(OpenStudio::Debug, "selected_drawing_interface = #{drawing_interface}")
            else
              drawing_interface = nil
              
              Plugin.log(OpenStudio::Debug, "reseting selected_drawing_interface")
              
              # try to revert back to group or building here
              parent = @model_interface.skp_model.active_entities.parent
              if (parent.class == Sketchup::ComponentDefinition)
                # Gets the SurfaceGroup interface that is currently open for editing
                drawing_interface = parent.instances.first.drawing_interface
                
                Plugin.log(OpenStudio::Debug, "selected_drawing_interface = #{drawing_interface}")
              else
                drawing_interface = @model_interface.building
                
                Plugin.log(OpenStudio::Debug, "selected_drawing_interface = #{drawing_interface}")
              end

              break
            end  
          end
        end
      end

      return(drawing_interface)
    end
    
    def select_drawing_interfaces(handles)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @selection.clear
      
      active_path = @model_interface.skp_model.active_path
      if active_path.nil?
        active_path = [] 
      end
      
      for child in @model_interface.recurse_children
        
        if child.class == DaylightingControl or 
           child.class == IlluminanceMap or 
           child.class == InteriorPartitionSurface or 
           child.class == InteriorPartitionSurfaceGroup or 
           child.class == Luminaire or 
           child.class == ShadingSurface or 
           child.class == ShadingSurfaceGroup or
           child.class == Space or
           child.class == SubSurface or
           child.class == Surface
           
          if child.model_object
            if handles.include?(child.model_object.handle)
              # do not select nil or deleted entities
              if child.valid_entity?
              
                #do not select object in active path
                if not active_path.include?(child.entity)
                  @selection.add(child.entity)
                end
              end
            end          
          end
        end
      end
    
    end
    
  end

end
