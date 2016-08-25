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

  # Add drawing_interface to WorkspaceObject
  class WorkspaceObject

    @@drawing_interface_hash = Hash.new

    # returns the OpenStudio::DrawingInterface associated with this ModelObject
    def drawing_interface
      object = nil
      if (drawing_interface_id = @@drawing_interface_hash[self.handle.to_s])
        begin
          object = ObjectSpace._id2ref(drawing_interface_id)
        rescue
          # The id_string does not reference an existing object!  Ignore the exception.
        ensure
          # Sometimes a bad reference can turn into a real object...but a random one, not the one we want.
          if (object and not object.is_a?(OpenStudio::DrawingInterface))
            puts "ModelObject.drawing_interface:  bad object reference"
            object = nil
            # To detect copy-paste between SketchUp sessions, could set 'object' to a value that can be detected on the
            # receiving end by whichever Observer the entity is pasted into.
          end
        end
      end
      return(object)
    end

    def drawing_interface=(object)
      @@drawing_interface_hash[self.handle.to_s] = object.object_id
    end

  end

end
