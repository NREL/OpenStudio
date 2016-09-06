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

require("lib/interfaces/DrawingInterface")

module OpenStudio

  class ThermalZone < DrawingInterface
  
    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_object = Plugin.model_manager.model_interface.openstudio_model.getThermalZone(handle)
      if not model_object.empty? and (handle.to_s == model_object.get.handle.to_s)
        model_object = model_object.get
      else
        puts "ThermalZone: model_object is empty for #{handle.class}, #{handle.to_s}, #{Plugin.model_manager.model_interface.openstudio_model}"                    
        model_object = nil
      end
      return model_object
    end
    
    def self.new_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      drawing_interface = ThermalZone.new
      model_object = model_object_from_handle(handle)
      drawing_interface.model_object = model_object
      model_object.drawing_interface = drawing_interface
      drawing_interface.add_watcher
      return(drawing_interface)
    end
    
    
    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      model_watcher_enabled = @model_interface.model_watcher.disable
      @model_object = OpenStudio::Model::ThermalZone.new(@model_interface.openstudio_model)
      @model_interface.model_watcher.enable if model_watcher_enabled
      
      # no entity, nothing to do
      #super
    end

    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # minimal additional initialization 
      if (super)
        if @model_object.renderingColor.empty?
          watcher_enabled = disable_watcher
          model_watcher_enabled = @model_interface.model_watcher.disable
          had_observers = @model_interface.materials_interface.remove_observers
          
          rendering_color = OpenStudio::Model::RenderingColor.new(@model_interface.openstudio_model)
          @model_object.setRenderingColor(rendering_color)
          if @model_object.renderingColor.empty? 
            rendering_color.remove
          else
            @model_interface.model_watcher.onObjectAdd(rendering_color)
          end
          
          @model_interface.materials_interface.add_observers if had_observers
          @model_interface.model_watcher.enable if model_watcher_enabled
          enable_watcher if watcher_enabled
        end   

        return(true)
      end
      
      return(false)
      
      # # extended initialization
      # if (super)
        
        # # DLM: below is ThermalZone initialization code that can be problematic as seen in issues #1667, #1763, #1755
      
        # error = nil
        
        # # check that this object is accessible in the workspace
        # object = @model_interface.openstudio_model.getObject(@model_object.handle)
        # if object.empty?
          # error = 'Cannot retrieve ThermalZone from model by handle'
        # end
        
        # # check for SizingZone object
        # if error.nil? && @model_object.getSources("OS_Sizing_Zone".to_IddObjectType).empty? 
          # model_watcher_enabled = @model_interface.model_watcher.disable
          
          # sizing_zone = OpenStudio::Model::SizingZone.new(@model_object.model, @model_object)
          # if sizing_zone.isEmpty(1)
            # error = 'Incomplete SizingZone object created'
            # sizing_zone.remove
          # end
          
          # @model_interface.model_watcher.enable if model_watcher_enabled
        # end
        
        # # check for ZoneHVACEquipmentList object
        # if error.nil? && @model_object.getSources("OS_ZoneHVAC_EquipmentList".to_IddObjectType).empty? 
          # model_watcher_enabled = @model_interface.model_watcher.disable
          
          # zonehvac_equipmentlist = OpenStudio::Model::ZoneHVACEquipmentList.new(@model_object)
          # if zonehvac_equipmentlist.isEmpty(2)
            # error = 'Incomplete ZoneHVACEquipmentList object created'
            # zonehvac_equipmentlist.remove
          # end
          
          # @model_interface.model_watcher.enable if model_watcher_enabled
        # end
        
        # # check ZoneAirInletPortList
        # if error.nil? && @model_object.isEmpty(9) # ZoneAirInletPortList
          # watcher_enabled = disable_watcher
          # model_watcher_enabled = @model_interface.model_watcher.disable
          
          # portlist = OpenStudio::Model::PortList.new(@model_object)
          # test = @model_object.setPointer(9, portlist.handle)
          # if !test
            # error = 'Could not set ZoneAirInletPortList'
            # portlist.remove
          # end
          
          # @model_interface.model_watcher.enable if model_watcher_enabled
          # enable_watcher if watcher_enabled
        # end
        
        # # check ZoneAirExhaustPortList
        # if error.nil? && @model_object.isEmpty(10) # ZoneAirExhaustPortList
          # watcher_enabled = disable_watcher
          # model_watcher_enabled = @model_interface.model_watcher.disable
          
          # portlist = OpenStudio::Model::PortList.new(@model_object)
          # test = @model_object.setPointer(10, portlist.handle)
          # if !test
            # error = 'Could not set ZoneAirExhaustPortList'
            # portlist.remove
          # end
          
          # @model_interface.model_watcher.enable if model_watcher_enabled
          # enable_watcher if watcher_enabled
        # end
        
        # # check that useIdealAirLoads is set
        # if error.nil? && @model_object.isEmpty(20) # useIdealAirLoads
          # watcher_enabled = disable_watcher
          
          # @model_object.setUseIdealAirLoads(false)
          # if @model_object.isEmpty(20)
            # error = 'Could not set useIdealAirLoads'
          # end
          
          # enable_watcher if watcher_enabled
        # end
        
        # # check zone air node
        # if error.nil? && @model_object.connectedObject(@model_object.zoneAirPort).empty?
          # watcher_enabled = disable_watcher
          # model_watcher_enabled = @model_interface.model_watcher.disable
          
          # node = OpenStudio::Model::Node.new(@model_interface.openstudio_model)
          # @model_interface.openstudio_model.connect(@model_object, @model_object.zoneAirPort, node, node.inletPort)
          # # DLM: any way to test if this worked?
          
          # @model_interface.model_watcher.enable if model_watcher_enabled
          # enable_watcher if watcher_enabled
        # end 
        
        # # check zone has a color
        # if error.nil? && @model_object.renderingColor.empty?
          # watcher_enabled = disable_watcher
          # model_watcher_enabled = @model_interface.model_watcher.disable
          # had_observers = @model_interface.materials_interface.remove_observers
          
          # rendering_color = OpenStudio::Model::RenderingColor.new(@model_interface.openstudio_model)
          # @model_object.setRenderingColor(rendering_color)
          # if @model_object.renderingColor.empty? 
            # error = 'Could not set RenderingColor'
            # rendering_color.remove
          # else
            # @model_interface.model_watcher.onObjectAdd(rendering_color)
          # end
          
          # @model_interface.materials_interface.add_observers if had_observers
          # @model_interface.model_watcher.enable if model_watcher_enabled
          # enable_watcher if watcher_enabled
        # end      
        
        # if error
          # msg  = "ThermalZone initialization resulted in the following error:\n"
          # msg += "'#{error}'\n\n"
          # msg += "It is advised that you save a backup of your current OpenStudio model and restart SketchUp."
          # UI.messagebox(msg)
        # end
               
        # return(true)
      # else
        # return(false)
      # end
    end
    
    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # should never be called, class does not have an entity
      #super
    end


    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @model_interface.building
    end


    # There is no entity to create
    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @entity = nil
    end

    def check_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(false) 
    end
    
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(false)
    end

    # Updates the SketchUp entity with new information from the ModelObject.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # model_object changed, call paint here
      if @model_interface.materials_interface.rendering_mode == RenderByThermalZone
        @model_interface.request_paint
      end
      
    end


    def on_change_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end


    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @model_interface.building
    end
    
  end

end
