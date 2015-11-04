######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

require 'openstudio'

OpenStudio::Application::instance().application()

model = OpenStudio::Model::Model.new
building = model.getBuilding
space1 = OpenStudio::Model::Space.new(model)
space2 = OpenStudio::Model::Space.new(model)

dialog = OpenStudio::Modeleditor::InspectorDialog.new("SketchUpPlugin".to_InspectorDialogClient)
dialog.setModel(model)
dialog.show
dialog.activateWindow

while dialog.isVisible do 
  OpenStudio::Application.instance.processEvents
  sleep(0.01.to_f)
end
