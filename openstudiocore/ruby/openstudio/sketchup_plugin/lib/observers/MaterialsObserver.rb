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

require("openstudio/sketchup_plugin/lib/interfaces/ModelInterface")

module OpenStudio

  class MaterialsObserver < Sketchup::MaterialsObserver
  
    def initialize(materials_interface)
      @materials_interface = materials_interface
      @enabled = false
    end
    
    def disable
      was_enabled = @enabled
      @enabled = false
      return was_enabled
    end
    
    def enable
      @enabled = true
    end
    
    def destroy
      @materials_interface = nil
      @enabled = false
    end    
    
    #def onMaterialAdd(materials, material)
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #end

    def onMaterialChange(materials, material)
    
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
      
      return if not @enabled
      
      proc = Proc.new {
        if !material.deleted? and material.drawing_interface
          material.drawing_interface.update_model_object
        end
      }
      Plugin.add_event( proc )
    end

    #def onMaterialRefChange(materials, material)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #  http://www.thomthom.net/software/sketchup/observers/#note_onMaterialRefChange
    #  When purging materials, or right-clicking a material in the Material Browser, this event triggers one time for every entity with a material. This causes a long series of events to trigger unnecessarily. 
    #
    #end

    #def onMaterialRemove(materials, material)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #  
    #  # this event will be handled by the material's EntityObserver
    #end

    #def onMaterialSetCurrent(materials, material)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end

    #def onMaterialUndoRedo(materials, material)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end
 
  end

end
