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

require("openstudio/sketchup_plugin/lib/interfaces/DrawingUtils")

module OpenStudio

  class FaceObserver < Sketchup::EntityObserver

    def initialize(drawing_interface)
      @drawing_interface = drawing_interface
      @enabled = false
    end
    
    def disable
      was_enabled = @enabled
      @enabled = false
      return was_enabled
    end
    
    def enable
      @enabled = true
    end
    
    def destroy
      @drawing_interface = nil
      @enabled = false
    end    
    
    # Gets called when the geometry of the Face changes, or when it is painted.
    def onChangeEntity(entity)
      
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}, entity = #{entity}")
      
      return if not @enabled
      
      # http://www.thomthom.net/software/sketchup/observers/#note_EntityObserver
      # EntityObserver.onChangeEntity mistriggers right before EntityObserver.onEraseEntity, referencing a non-existant entity. 
      # EntityObserver.onEraseEntity reference a non-existant entity. 
      
      proc = Proc.new {
        # This proc is essential, otherwise there was an API bug with 'onChangeEntity' for faces that share an edge.
        # Only one face was getting the callback.  Appears the first callback was breaking the second callback.
        # Changing to an asynchronous proc solves the problem.
        
        Plugin.log(OpenStudio::Trace, "#{current_method_name}, entity = #{entity}")

        # destroy_observers may have been called since the time this proc was scheduled
        if (@drawing_interface)
        
          #Plugin.log(OpenStudio::Trace, "#{current_method_name}, @drawing_interface = #{@drawing_interface.class}")

          # when push/pull tool is used not all changed faces will get this observer called
          if(Sketchup.active_model.tools.active_tool_id == 21041)  # PushPullTool
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, calling @drawing_interface.parent.on_change_entity")
            @drawing_interface.parent.on_change_entity
            
          # Need to check the parent entity to make sure this face didn't find itself outside of a Group after an explode.
          elsif (@drawing_interface.valid_entity? and @drawing_interface.parent_from_entity)
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, calling @drawing_interface.on_change_entity")
            @drawing_interface.on_change_entity
          end
        
        end
      }
      
      Plugin.add_event( proc )
      
    end


    def onEraseEntity(entity)
      
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}, entity = #{entity}")
      
      return if not @enabled

      # http://www.thomthom.net/software/sketchup/observers/#note_EntityObserver
      # EntityObserver.onChangeEntity mistriggers right before EntityObserver.onEraseEntity, referencing a non-existant entity. 
      # EntityObserver.onEraseEntity reference a non-existant entity. 
      
      # Because Face entities can become swapped, @drawing_interface might not be the one that was actually deleted.
      # See the description of the problem under SurfaceGroupEntitiesObserver.onElementAdded.

      # API Bug:  the 'entity' argument passed in to this callback appears to be a dummy Face object,
      # unrelated to the deleted Face (all of the Face data is gone, except entityID which is now negative.)

      # Which is why @drawing_interface is used to store the interface.  Otherwise, entity.drawing_interface is nil.

      proc = Proc.new {
      
        Plugin.log(OpenStudio::Trace, "#{current_method_name}, entity = #{entity}")
        
        # onEraseEntity gets called for each Face even when the parent Group gets erased.
        # This check avoids an error if the Group is already deleted.
        containing_entity = @drawing_interface.containing_entity
        if (containing_entity and containing_entity.valid?)
        
          #Plugin.log(OpenStudio::Trace, "#{current_method_name}, containing_entity and containing_entity.valid?")

          swapped_face = nil

          # Check for swapping of face entities if the erased face was a Surface.
          # (Swapping never seems to happen when @drawing_interface is a SubSurface.)
          if (@drawing_interface.class == Surface)
            
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, @drawing_interface = #{@drawing_interface.class} check for swapping")
            
            # get the vertices of the surface from the model object and transformed into SketchUp coordinates
            drawing_interface_points = @drawing_interface.surface_polygon.points

            # search all entities in containing_entity for surface that matches these vertices
            # containing_entity.entities will not return entity because it has been deleted
            containing_entity.entities.each { |this_entity|
              if ((this_entity.class == Sketchup::Face))
                face_points = this_entity.outer_polygon.reduce.points

                # Check to see if all drawing_object points are a subset of the face points.
                if (drawing_interface_points.is_subset_of?(face_points))
                
                  # @drawing_interface's model object is really associated with this_entity
                
                  if @drawing_interface == this_entity.drawing_interface
                    # maybe this happens when one entity is copied to another entity?  should we really be cloning drawing interface here?
                    #Plugin.log(OpenStudio::Trace, "#{entity} and #{this_entity} both refer to #{@drawing_interface}, how did this happen?")
                  else
                    #puts "Found swapped face = " + this_entity.to_s
                    swapped_face = this_entity
                    
                    #Plugin.log(OpenStudio::Trace, "#{current_method_name}, swap detected with swapped_face = #{swapped_face}")
                    break
                  end
                end
              end
            }
          end

          if (swapped_face)
          
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, fix swapping between @drawing_interface = #{@drawing_interface.class} and swapped_face.drawing_interface = #{swapped_face.drawing_interface.class}")
            
            # 'swapped_face' is the entity that is left.  'entity' is already erased.
            
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, remove current swapped_face.drawing_interface.entity = #{swapped_face.drawing_interface.entity}, swapped_face = #{swapped_face}")

            # Detach the drawing interface from the swapped surface.
            removed_interface = swapped_face.drawing_interface
            removed_interface.remove_observers
            removed_interface.on_erase_entity 
            removed_interface.detach_entity # changes properties via clean_entity

            # Restore the drawing interface to the original face.
            # changes properties of swapped_face
            swapped_face.drawing_interface = @drawing_interface
            swapped_face.model_object_handle = @drawing_interface.model_object.handle
            
            @drawing_interface.entity = swapped_face
            @drawing_interface.on_change_entity # changes properties via paint_entity
            @drawing_interface.add_observers
            
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, swapped_face = #{swapped_face} now refers to swapped_face.drawing_interface = #{swapped_face.drawing_interface.class}")

          else
            # Normal erase
            #Plugin.log(OpenStudio::Trace, "#{current_method_name}, no swapping")
            @drawing_interface.on_erase_entity
          end

        else
          #Plugin.log(OpenStudio::Trace, "#{current_method_name}, no containing_entity or not containing_entity.valid?")
          
          # Group was deleted--make sure to still erase the drawing interface!
          @drawing_interface.on_erase_entity
        end
      }
      
      Plugin.add_event( proc )
      
    end

  end

end
