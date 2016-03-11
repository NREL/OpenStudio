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


module OpenStudio

  class PluginModelWatcher < WorkspaceWatcher
  
    def initialize(model_interface)
      super(model_interface.openstudio_model)

      @model_interface = model_interface
    end

    #def clearState
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end
    
    #def onChangeWorkspace
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end

    #def onBecomeDirty
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end

    #def onBecomeClean
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end

    def onObjectAdd(addedObject)
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      @model_interface.on_new_model_object(addedObject)

      # object is not yet fully constructed so we can't do:
      # if addedObject.is_a? OpenStudio::Model::SpaceLoadInstance

      class_name = addedObject.iddObject.name.upcase

      # these objects do not have a drawing interface where we can call check_model_object
      if class_name == "OS:ELECTRICEQUIPMENT" or 
         class_name == "OS:GASEQUIPMENT" or 
         class_name == "OS:HOTWATEREQUIPMENT" or
         class_name == "OS:INTERNALMASS" or 
         class_name == "OS:LIGHTS" or 
         class_name == "OS:LUMINAIRE" or  # should not ever get luminaire
         class_name == "OS:PEOPLE" 

        spaceLoadInstance = addedObject.to_SpaceLoadInstance
        
        OpenStudio::Modeleditor::ensureSpaceLoadDefinition(spaceLoadInstance.get)

      end
      
    end

    # this is called before the object has been removed
    def onObjectRemove(removedObject)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super
      
      if drawing_interface = removedObject.drawing_interface
        drawing_interface.on_pre_delete_model_object
      end
    end

  end

end

