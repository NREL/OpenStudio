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

require("openstudio/sketchup_plugin/sketchup/Sketchup")
require("openstudio/sketchup_plugin/sketchup/Geom")


# Everything in this module should be strictly based on entities and not drawing interfaces.
module DrawingUtils

  # returns true if entity is the base face of face
  def DrawingUtils.is_base_face(face, face_normal, face_points, entity)
    if (entity.class == Sketchup::Face and not entity.equal?(face))
      # Eliminate faces that are not parallel.
      # Another test would be to check if both are in the same plane.
      # There are some precision issues with 'face.plane' however.
      if (entity.normal.parallel?(face_normal))
        # Detect if the vertices of the entity are a subset of this face.
        if (face_points.is_subset_of?(entity.full_polygon.reduce.points))
          return true
        end
      end
    end  
    return false
  end
  
  # Strictly determined using Faces, not drawing interfaces.
  # Tries to match a face to a base face.
  def DrawingUtils.detect_base_face(face)
    base_face = nil
    first_guess = nil
    
    # try the current parent as a first guess
    if drawing_interface = face.drawing_interface
      if drawing_interface.class == OpenStudio::SubSurface
        if parent = drawing_interface.parent
          if temp = parent.entity and temp.class == Sketchup::Face
            first_guess = temp
          end
        end
      end
    end
    
    face_normal = face.normal
    face_points = face.full_polygon.reduce.points
    
    all_connected = face.all_connected
    if first_guess
      if all_connected.reject!{|e| e == first_guess}
        all_connected = [first_guess].concat(all_connected)
      end
    end
    
    for entity in all_connected
      if is_base_face(face, face_normal, face_points, entity)
        base_face = entity
        break
      end
    end
    return(base_face)
  end


  # This would be called by sub surface swaps, as well as swaps from push/pull.
  # 'entity1' already has an interface.
  def DrawingUtils.swap_interfaces(entity1, entity2)

  
    #drawing_interface.attach_entity(entity)
        #    detach_entity(@entity)  # fix old entity
        #
        #    check_entity(entity)  ...test before continuing
        #
        #    @entity = entity
        #    @entity.drawing_interface = self
        #    @entity.model_object_key = @model_object.key
        #      ? maybe call on_changed_entity
        #    ##add_observers  (optional)  or do externally

  
    #  .attach_model_object(model_object)
    #      @model_object = model_object
    #      @entity.model_object_key = @model_object.key

  end


  def DrawingUtils.clean_entity(entity)
    # This could be added as a method on Face and Group.

    if (entity.drawing_interface)
      entity.drawing_interface.remove_observers
    end

    entity.drawing_interface = nil
    entity.model_object_handle = nil
  end


  # Big kludge:
  # When a face is divided into two faces such that the smaller face cuts into the original face,
  # e.g., changing the original vertice count from 4 to 8, the entity object assignments will
  # often become swapped.  For example, the 8 vertex face is now considered the 'new entity' and
  # the smaller face is assigned to the original entity.  This is a problem when trying to detect
  # windows and doors that are added.
  # Solution is that both faces will share the same drawing interface at this point.  The task is
  # to identify which is which.

  # Checks only the case of swapping a sub_face with a base_face.
  def DrawingUtils.swapped_face_on_divide?(entity)
    
    # first check if either entity or the drawing_interface have been deleted
    if entity.deleted?
      raise("entity.deleted? is true")
    end
    
    drawing_interface = entity.drawing_interface
    if drawing_interface.nil? or drawing_interface.deleted?
      raise("drawing_interface.nil? or drawing_interface.deleted? is true")
    end
    
    old_entity = drawing_interface.entity
    if old_entity.nil? or old_entity.deleted?
      # this can happen if a push/pull operation creates a new face and deletes the old face at the same time
      #raise("old_entity.nil? or old_entity.deleted? is true")
      #puts "old_entity.nil? or old_entity.deleted? is true"
      #OpenStudio::Plugin.log(OpenStudio::Info, "old_entity.nil? or old_entity.deleted? is true")
      return(false)
    end
    
    # the new entity has the same id as the old entity
    if entity.entityID == old_entity.entityID
      raise("entity.entityID == old_entity.entityID is true")
      #OpenStudio::Plugin.log(OpenStudio::Info, "entity.entityID == old_entity.entityID is true")
      #OpenStudio::Plugin.log(OpenStudio::Info, "drawing_interface = #{drawing_interface}")
      #OpenStudio::Plugin.log(OpenStudio::Info, "drawing_interface.model_object = #{drawing_interface.model_object}")
      return(false)
    end
    
    new_face_points = entity.full_polygon.reduce.points
    old_face_points = old_entity.full_polygon.reduce.points
    
    OpenStudio::Plugin.log(OpenStudio::Info, "new_face = #{entity}, entityID = #{entity.entityID}")
    OpenStudio::Plugin.log(OpenStudio::Info, "new_face_points = [#{new_face_points.join(',')}]")
    OpenStudio::Plugin.log(OpenStudio::Info, "old_face = #{old_entity}, entityID = #{old_entity.entityID}")
    OpenStudio::Plugin.log(OpenStudio::Info, "old_face_points = [#{old_face_points.join(',')}]")
    
    # in the no swap case, old_entity is the base surface and new_entity is the sub surface
    
    # in the swap case, new_entity is the base surface and old_entity is the sub surface
    
    swap = old_face_points.is_subset_of?(new_face_points)

    OpenStudio::Plugin.log(OpenStudio::Info, "swap = #{swap}")
    
    if (swap)
      puts "swap"
      return(true)  # swapped
    else
      puts "no swap"
      return(false)  # no swap
    end
  end



  def DrawingUtils.swapped_face_on_pushpull?(entity)   # swal_on_pushpull?
    return(false)
  end


end
