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

  class AppObserver < Sketchup::AppObserver
    
    # cannot enable/disable this class
    
    # onNewModel get called when the 'New' menu item is clicked, even though the user clicks cancel!  Very strange.
    # The active_model object reference is even changed as well, although none of the content of the model changes...
    # onOpenModel has the same behavior.
    # The work-around is to save and compare the 'guid' which does not change unless a truly new model is created or opened.

    def onNewModel(model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if Plugin.model_manager.model_interface and
         Plugin.model_manager.model_interface.skp_model_guid == model.guid

        # same model, no-op
        Plugin.log(OpenStudio::Trace, "New model is the same as current model")
      else

        Plugin.model_manager.new_from_skp_model(model)
        
        Plugin.menu_manager.refresh
        
        Plugin.model_manager.purge_invalid_model_interfaces
      end

    end

    def onOpenModel(model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      Plugin.model_manager.new_from_skp_model(model)
      
      Plugin.menu_manager.refresh
      
      Plugin.model_manager.purge_invalid_model_interfaces
    end

    # Note:  Sketchup.active_model is already nil at this point
    def onQuit
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      Plugin.model_manager.shutdown
    end
    
    
    #def onUnloadExtension
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #end
    
  end

end
