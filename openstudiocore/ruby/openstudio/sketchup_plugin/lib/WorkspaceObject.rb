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
