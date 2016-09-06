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

require("lib/dialogs/Dialogs")
require("lib/dialogs/DialogContainers")


module OpenStudio

  class ColorScaleDialog < Dialog

    def initialize(container, interface, hash)
      super
      h = Plugin.platform_select(380, 400)
      @container = WindowContainer.new("", 112, h, 150, 150)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/ColorScale.html")
      add_callbacks
    end


    def on_load
      super

      if (Plugin.platform == Platform_Mac)
        @container.execute_function("invalidate()")  # Force the WebDialog to redraw
      end
    end


    def update
      super
      if (Plugin.model_manager.model_interface.results_interface.rendering_appearance == "COLOR")
        set_element_source("COLOR_SCALE", "colorscale_vertical.bmp")
      else
        set_element_source("COLOR_SCALE", "grayscale_vertical.bmp")
      end
    end

  end
  
end
