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

  class ProgressDialog < ProgressBar

    def initialize(message)
      super(false)
      setWindowTitle(message)
      @last_num_chars = -1    
    end

    def onPercentageUpdated(percentage)
      super
      
      if percentage < 0 or percentage > 100
        # Plugin.do_bug
        return false
      end
      
      fraction = percentage / 100.0
      max_chars = 100
      num_chars = (fraction*max_chars).to_i
      
      if @last_num_chars != num_chars
        @last_num_chars = num_chars
        Sketchup.status_text = windowTitle + "  " + "|"*num_chars
      end
      
      return true
    end
    
    def destroy
      Sketchup.status_text = ""
      return true
    end

  end

end
