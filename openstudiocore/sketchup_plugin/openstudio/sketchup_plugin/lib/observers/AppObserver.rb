########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
