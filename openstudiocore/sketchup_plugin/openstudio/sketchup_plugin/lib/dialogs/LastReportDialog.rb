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

require("openstudio/sketchup_plugin/lib/dialogs/Dialogs")
require("openstudio/sketchup_plugin/lib/dialogs/DialogContainers")


module OpenStudio

  class LastReportDialog < MessageDialog

    def initialize(container, interface, hash)
      super
      @container = WindowContainer.new(Plugin.name + " Last Report Window", 400, 400, 150, 150)
      @container.center_on_parent
      @container.set_file(Plugin.dir + "/lib/dialogs/html/LastReport.html")
      
      add_callbacks
    end
    
    def on_load
      super
    end
    
    def update
      super
    end

  end

end
