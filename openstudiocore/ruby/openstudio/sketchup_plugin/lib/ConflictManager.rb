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

  class ConflictManager 
  
    def initialize
      @ignore = false
    end

    def check_for_conflicts
      
      # timer keeps repeating when messagebox is open, use this to disable new message boxes when one is open
      if @ignore
        return
      end
      @ignore = true
    
      if Module.constants.include?("IESVE")

      text = <<IESWARNING
The IES-VE Plug-in has been detected.  

The OpenStudio Plug-in may experience issues due to interactions with the IES-VE Plug-in.  

To temporarily disable the IES-VE Plug-in, rename the file IESLink.rbs in the SketchUp Plugins directory to IESLink.__rbs__.

The OpenStudio Plug-in can be enabled or disabled through SketchUp's Preferences->Extension menu item.

Do you want to show this warning in the future?
IESWARNING

        show_ies_warning = Sketchup.read_default("OpenStudio", "Show IES Warning", true)

        if show_ies_warning
          result = UI.messagebox(text, MB_YESNO)
          if result == 6 # Yes
            Sketchup.write_default("OpenStudio", "Show IES Warning", true)
          else # No
            Sketchup.write_default("OpenStudio", "Show IES Warning", false)
          end
        end
      
      end
      
      # re-enable
      @ignore = false
      
    end
    
  end

end
