######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

require("openstudio/sketchup_plugin/sketchup/Sketchup")
require("openstudio/sketchup_plugin/sketchup/Geom")


# Everything in this module should be strictly based on entities and not drawing interfaces.
module DrawingUtils


  # Strictly determined using Faces, not drawing interfaces.
  # Tries to match a face to a base face.
  def DrawingUtils.detect_base_face(face, force)
    base_face = nil
    
    if not force
      drawing_interface = face.drawing_interface
      if drawing_interface
        if drawing_interface.class == OpenStudio::SubSurface
          parent = drawing_interface.parent
          if parent
            base_face = parent.entity
            if base_face and base_face = Sketchup::Face
              return base_face
            else
              base_face = nil
            end
          end
        else
          # other classes don't have parent surfaces
          return nil
        end
      end
    end
    
    face_normal = face.normal
    face_points = face.full_polygon.reduce.points

    for child_entity in face.all_connected
      if (child_entity.class == Sketchup::Face and not child_entity.equal?(face))
        # Eliminate faces that are not parallel.
        # Another test would be to check if both are in the same plane.
        # There are some precision issues with 'face.plane' however.
        if (child_entity.normal.parallel?(face_normal))
          # Detect if the vertices of the entity are a subset of this face.
          if (face_points.is_subset_of?(child_entity.full_polygon.reduce.points))
            base_face = child_entity
            break
          end
        end
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
