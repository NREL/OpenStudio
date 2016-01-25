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
