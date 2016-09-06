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

require("lib/dialogs/DialogInterface")
require("lib/dialogs/LastReportDialog")


module OpenStudio

  class LastReportInterface < DialogInterface

    def initialize
      super
      @dialog = LastReportDialog.new(nil, self, @hash)
      
      @last_report = ''
      @hash['LAST_REPORT'] = @last_report
    end
    
    def last_report=(text)
      @last_report = text
      populate_hash
      update
    end
    
    def populate_hash
      @hash['LAST_REPORT'] = @last_report
      super
    end
    
    def report
      super
    end
    

   
  end

end
