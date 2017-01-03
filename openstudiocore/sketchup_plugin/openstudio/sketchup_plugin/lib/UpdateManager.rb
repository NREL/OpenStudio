########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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
