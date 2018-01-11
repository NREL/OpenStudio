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

require("openstudio/sketchup_plugin/lib/dialogs/DialogInterface")
require("openstudio/sketchup_plugin/lib/dialogs/ColorScaleDialog")


module OpenStudio

  class ColorScaleInterface < DialogInterface

    def initialize
      super
      
      @dialog = ColorScaleDialog.new(nil, self, @hash)
    end


    def populate_hash

      maximum = Plugin.model_manager.model_interface.results_interface.range_maximum.to_f
      minimum = Plugin.model_manager.model_interface.results_interface.range_minimum.to_f
      units = Plugin.model_manager.model_interface.results_interface.range_units.to_s
      normalize = Plugin.model_manager.model_interface.results_interface.normalize

      tick = (maximum - minimum) / 5.0

      if normalize
        units += "/m2"
      end

      @hash['LABEL_1'] = (maximum).to_s + " " + units
      @hash['LABEL_2'] = (maximum - tick).to_s + " " + units
      @hash['LABEL_3'] = (maximum - tick * 2).to_s + " " + units
      @hash['LABEL_4'] = (maximum - tick * 3).to_s + " " + units
      @hash['LABEL_5'] = (maximum - tick * 4).to_s + " " + units
      @hash['LABEL_6'] = (maximum - tick * 5).to_s + " " + units
    end

  end

end
