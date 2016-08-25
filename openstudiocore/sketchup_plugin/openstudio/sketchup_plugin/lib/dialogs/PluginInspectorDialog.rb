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

  class PluginInspectorDialog < Modeleditor::InspectorDialog
    
    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
     
      super("SketchUpPlugin".to_InspectorDialogClient, OpenStudio::SketchUpWidget)
      
      @ignore = false
    end

    def enabled?
      return (not @ignore)
    end

    def enable
      @ignore = false
    end

    def disable
      watcher_enabled = enabled?
      @ignore = true
      return watcher_enabled
    end

    #def onIddObjectTypeChanged(iddObjectType)
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, enabled? = #{enabled?}")
    #  super
    #end

    def onSelectedObjectHandlesChanged(handles)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, enabled? = #{enabled?}, handles = #{handles}")
      super

      if enabled?
        model_interface =  Plugin.model_manager.model_interface
        if model_interface
          had_observers = model_interface.selection_interface.remove_observers
          model_interface.selection_interface.select_drawing_interfaces(handles)
          model_interface.selection_interface.add_observers if had_observers
        end
      end

    end

    #def onModelChanged(model)
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, enabled? = #{enabled?}")
    #  super
    #end

    def show_error
      msg  = "An error occurred in the OpenStudio SketchUp plug-in.\n\n"
      msg += "It is advised that you save a backup of your current OpenStudio model and restart SketchUp."
      UI.messagebox(msg)
    end
    
    def onPushButtonNew(checked)
      model_interface =  Plugin.model_manager.model_interface
      if model_interface.model_watcher.enabled
        super(checked)
      else
        show_error
      end
    end
    
    def onPushButtonCopy(checked)
      model_interface =  Plugin.model_manager.model_interface
      if model_interface.model_watcher.enabled
        super(checked)
      else
        show_error
      end
    end
    
    def onPushButtonDelete(checked)
      model_interface =  Plugin.model_manager.model_interface
      if model_interface.model_watcher.enabled
        super(checked)
      else
        show_error
      end
    end
    
    def onPushButtonPurge(checked)
      model_interface =  Plugin.model_manager.model_interface
      if model_interface.model_watcher.enabled
        super(checked)
      else
        show_error
      end
    end
    
    def show
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, enabled? = #{enabled?}")
      update
      super
      activateWindow

      #OpenStudio::Application.instance.processEvents
    end

    def update
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, enabled? = #{enabled?}")
      
      model_interface = Plugin.model_manager.model_interface
      if model_interface
        setModel(model_interface.openstudio_model)
        setEnabled(true)
      else
        openstudio_model = OpenStudio::Model::Model.new
        setModel(openstudio_model)
        setEnabled(false)
      end
    end

  end

end
