########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

  class PluginPathWatcher < PathWatcher
  
    def initialize(model_interface, path)
      super(path)
      
      @model_interface = model_interface
    end

    def onPathChanged
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # ignore any further signals until we finish processing
      disable

      skp_model = @model_interface.skp_model

      if (not skp_model.valid?)

        Plugin.log(OpenStudio::Debug, "skp_model #{skp_model} is not valid, active model is #{Sketchup.active_model}")

        # skp_model is no longer valid (e.g. closed on Mac)
        proc = Proc.new { Plugin.model_manager.purge_invalid_model_interfaces }
        Plugin.add_event( proc )
        
        # continue ignoring

      elsif skp_model != Sketchup.active_model

        Plugin.log(OpenStudio::Debug, "skp_model #{skp_model} is not active_model, active model is #{Sketchup.active_model}")

        # no-op to avoid a crash when you try to update a model that is not the active model

        # re-enable the watcher
        enable

      else

        success = false
        result = UI.messagebox("Another application has updated #{self.path.to_s}, do you want to reload it?", MB_YESNO)
        if result == 6 # Yes
          success = Plugin.model_manager.open_openstudio(self.path.to_s, Sketchup.active_model)
        end

        if success
          # continue ignoring, new watcher will take over
        else
          # re-enable the watcher
          enable
        end
      end

    end
    
  end
end
