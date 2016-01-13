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

  class PluginUpdateManager < UpdateManager
  
    def initialize(appName, verbose)
      super(appName)
      @verbose = verbose
      Sketchup.status_text = "OpenStudio checking for update"
    end

    def replyProcessed()
    
      if not self.error
        if (self.newMajorRelease or self.newMinorRelease)
          mostRecentVersion = self.mostRecentVersion
          mostRecentDownloadUrl = self.mostRecentDownloadUrl
          button = UI.messagebox("A newer version #{mostRecentVersion} of OpenStudio is ready for download.\n" +
                "Do you want to update to the newer version?\n\n" +
                "Click YES to visit the OpenStudio website to get the download.\n" +
                "Click NO to skip this version and not ask you again.\n" +
                "Click CANCEL to remind you again next time.", MB_YESNOCANCEL)
          if (button == 6)  # YES
            UI.openURL(mostRecentDownloadUrl)
          elsif (button == 7)  # NO
            Plugin.write_pref("Check For Update #{Plugin.version}", false)
          end
        elsif (@verbose)
          UI.messagebox("You currently have the most recent version of OpenStudio.")
        else
          puts "You currently have the most recent version of OpenStudio."
        end
      elsif (@verbose)
        UI.messagebox("Error occurred while checking for update.")
      else
        puts "Error occurred while checking for update."
      end
      
      Plugin.update_manager = nil
      
    end
    
  end

end
