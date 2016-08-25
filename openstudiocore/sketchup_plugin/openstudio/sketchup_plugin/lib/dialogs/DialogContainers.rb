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

  # Abstract superclass for WindowContainer and FrameContainer
  # Basically a wrapper for UI::WebDialog, but enables dialog objects to be inserted into HTML frames.
  class DialogContainer  

    attr_accessor :dialog, :web_dialog, :width, :height

    def initialize
      @dialog = nil
      @web_dialog = nil
    end


    def set_file(path)
      @web_dialog.set_file(path, nil)
    end
    

# change name to execute_script for consistency...
    def execute_function(function_call)
      @web_dialog.execute_script(function_call)
    end


    def update
      @dialog.update
    end
  
  
    def show
      # Not sure what FrameContainers can do with this.
    end
    
    
    def close
      # Not sure what FrameContainers can do with this.
    end
  
  end
  
  
  class WindowContainer < DialogContainer
  
    def initialize(title, width, height, left, top, resizable = true, scrollable = true)
      @width = width
      @height = height

      if (not resizable)
        # Kludge to get around a WebDialog bug with setting width and height
        Sketchup.write_default("WebDialog_OpenStudio " + title, "Width", width)
        Sketchup.write_default("WebDialog_OpenStudio " + title, "Height", height)
      end

      @web_dialog = UI::WebDialog.new(title, scrollable, "OpenStudio " + title, width, height, left, top, resizable)

      @web_dialog.navigation_buttons_enabled = false
    end


    def center_on_parent
      #if (Plugin.platform == Platform_Windows)
      #  parent_position = WxSU.app.sketchup_frame.get_position
      #  parent_size = WxSU.app.sketchup_frame.get_size
      #  x = parent_position.x + (parent_size.get_width - @width) / 2
      #  y = parent_position.y + (parent_size.get_height - @height) / 2
      #else
      #  x = 150
      #  y = 150
      #end

      #@web_dialog.set_position(x, y)
    end


    def show
      # This method gets called when the dialog is created, or
      # if the dialog is already created and must be brought to the front.

      if (@web_dialog.visible?)
        @web_dialog.bring_to_front
      else
        @web_dialog.show
      end
    end

  
    def show_modal
      @web_dialog.show_modal
    end


    def close
      @web_dialog.close
    end
  
  end
  
  
  class FrameContainer < DialogContainer

    def initialize(parent_container, name)      
      @dialog = nil
      @web_dialog = parent_container.web_dialog
      @name = name
    end


    def set_file(path)
      # This method is nearly identical to the method 'set_element_source' in the Dialog class
      
      if (@name.nil?)
        puts "FrameContainer:  @name is nil in set_file"
      else
        if (path.nil?)
          path = ""
        end
        
        @web_dialog.execute_script("setElementSource('" + @name + "', '" + path + "')")
      end
    end


    def execute_function(function_call)
      # The function is called in the context of the enclosing frame.
      @web_dialog.execute_script(@name + "." + function_call)
    end
  
  end

  
end
