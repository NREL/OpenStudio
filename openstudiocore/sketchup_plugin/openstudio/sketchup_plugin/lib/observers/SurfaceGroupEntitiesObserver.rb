########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require("openstudio/sketchup_plugin/lib/interfaces/DrawingUtils")

module OpenStudio

  class SurfaceGroupEntitiesObserver < Sketchup::EntitiesObserver

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

    def onElementAdded(entities, entity)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      proc = Proc.new {

        Plugin.log(OpenStudio::Info, "SurfaceGroupEntitiesObserver.onElementAdded: adding #{entity} to #{@drawing_interface.entity}")

        if (!entity.deleted? and entity.class == Sketchup::Face)  # In SU7, deleted entities get passed in sometimes.

          Plugin.log(OpenStudio::Info, "entityID = #{entity.entityID}")

          # Prevent copy-paste between a space group and a detached shading group, etc
          # Clean any entities that appear with the wrong class for this group.
          if (entity.drawing_interface)
            if (@drawing_interface.class == Space)
              if (entity.drawing_interface.class != Surface and entity.drawing_interface.class != SubSurface)
                Plugin.log(OpenStudio::Info, "Cleaning entity " + entity.to_s + " pasted in Space")
                DrawingUtils.clean_entity(entity)
              end
            elsif (@drawing_interface.class == ShadingSurfaceGroup)
              if (entity.drawing_interface.class != ShadingSurface)
                Plugin.log(OpenStudio::Info, "Cleaning entity " + entity.to_s + " pasted in ShadingSurfaceGroup")
                DrawingUtils.clean_entity(entity)
              end
            elsif (@drawing_interface.class == InteriorPartitionSurfaceGroup)
              if (entity.drawing_interface.class != InteriorPartitionSurface)
                Plugin.log(OpenStudio::Info, "Cleaning entity " + entity.to_s + " pasted in InteriorPartitionSurfaceGroup")
                DrawingUtils.clean_entity(entity)
              end
            end
          end

          if (entity.drawing_interface.nil?)
            #  This is a brand new surface or a copy that had to be cleaned.
            if (@drawing_interface.class == Space)

              base_face = DrawingUtils.detect_base_face(entity)

              if (base_face.nil?)
                Plugin.log(OpenStudio::Info, "New Surface in Space")
                surface = Surface.new_from_entity(entity)

                if not surface.model_object
                  # model object was not created
                  surface.on_delete_model_object
                end
              else

                # add a second proc to ensure that surfaces will be drawn before sub surfaces
                proc2 = Proc.new {
                  surface = base_face.drawing_interface
                  if not surface
                    Plugin.log(OpenStudio::Error, "New SubSurface in Space, no Surface found!")
                  else
                    Plugin.log(OpenStudio::Info, "New SubSurface in Space")
                    subSurface = SubSurface.new_from_entity(entity)

                    if not subSurface.model_object
                      # model object was not created
                      subSurface.on_delete_model_object
                    end

                    # Must trigger the base surface to recalculate vertices to account for the new sub surface.
                    surface.on_change_entity
                  end
                }
                Plugin.add_event( proc2 )

              end

            elsif (@drawing_interface.class == ShadingSurfaceGroup)
              Plugin.log(OpenStudio::Info, "New ShadingSurface in ShadingSurfaceGroup")
              ShadingSurface.new_from_entity(entity)
            elsif (@drawing_interface.class == InteriorPartitionSurfaceGroup)
              Plugin.log(OpenStudio::Info, "New InteriorPartitionSurface in InteriorPartitionSurfaceGroup")
              InteriorPartitionSurface.new_from_entity(entity)
            else
              Plugin.log(OpenStudio::Info, "Face added to unknown SurfaceGroup subclass")
            end

            # DLM: The following lines of code were added r6581 to fix Fixing bug 231 Creating surfaces while in render mode other than class or boundary stops model objects from being made
            # However, I think they are unneccesary and the cause of Bug 874 - Surface classification is much slower when you paste it into a space vs. building it within a space
            # # reparent any subsurfaces to the new surface
            # @drawing_interface.update_child_model_objects
            #
            # # repaint surfaces
            # @drawing_interface.update_child_entities

          elsif (entity.drawing_interface.deleted?)

            case (Sketchup.active_model.tools.active_tool_id)
            when (21041)  # PushPullTool
              # if a surface is pushed until it cuts through the space, sometimes the new surface will have been swapped with the old one
              Plugin.log(OpenStudio::Info, "Push-pull surface:  new surface, ignoring deleted_interface")
              surface_class = entity.drawing_interface.class
              surface_class.new_from_entity(entity)
            #when (21020)  # SketchTool (Line/Pencil)
            #when (21094)  # RectangleTool
            #when (21013)  # PasteTool
            #when (21048)  # MoveTool  (also gets called when doing a multi-copy/paste)
            else
              # This is a cut/paste or undo of a previous delete of this surface.
              Plugin.log(OpenStudio::Info, "Cut-paste/delete-undo of surface")
              entity.drawing_interface.on_undelete_entity(entity)
            end

            # May have to handle swapping in here too?
            # DLM: I don't think so, the object should get the correct drawing interface back from stored idf object
            #swapped = DrawingUtils.swapped_face_on_divide?(entity)
            #Plugin.log(OpenStudio::Info, "swapped = " + swapped.to_s)

          else
            # This is a divide of an existing surface.
            # A divide can be caused by drawing a line that divides a face, or by push/pull.
            # This also gets called for a copy and paste.
            # If this is a divide, the parent face vertices have already changed.
            # This could be a divide of a base surface, or a new window or door subsurface.
            Plugin.log(OpenStudio::Info, "Copy-paste/divide/push-pull of surface")

            # This falsely detects a copy-paste of a base surface in the same space as a swap.  It's not.
            swapped = DrawingUtils.swapped_face_on_divide?(entity)
            Plugin.log(OpenStudio::Info, "swapped = " + swapped.to_s)

            base_face = nil
            if (@drawing_interface.class == Space)
              if (swapped)
                base_face = DrawingUtils.detect_base_face(entity.drawing_interface.entity)
              else
                base_face = DrawingUtils.detect_base_face(entity)
              end
            end

            if (base_face.nil?)
              # This is a copy-paste/divide/push-pull of a surface.
              # New surface should belong to the same class as the original.

              original_interface = entity.drawing_interface
              surface_class = entity.drawing_interface.class
              Plugin.log(OpenStudio::Info, "surface_class = " + surface_class.to_s)

              case (Sketchup.active_model.tools.active_tool_id)
              when (21041)  # PushPullTool
                Plugin.log(OpenStudio::Info, "Push-pull surface:  new surface")
                surface_class.new_from_entity(entity)
              #when (21020)  # SketchTool (Line/Pencil)
              #when (21094)  # RectangleTool
              #when (21013)  # PasteTool
              #when (21048)  # MoveTool  (also gets called when doing a multi-copy/paste)
              else
                Plugin.log(OpenStudio::Info, "Copy-paste/divide surface:  copy surface")
                surface_class.new_from_entity_copy(entity)
              end

              # add a second proc to ensure that surfaces will be drawn before sub surfaces
              proc2 = Proc.new {
                # update any children of the original interface that may need to switch to the new interface
                original_interface.children.each do |child|
                  child.update_parent_from_entity
                  child.update_model_object
                end
              }
              Plugin.add_event( proc2 )

            else
              # This is a new sub surface located on a base surface.

              # NEED SEPARATE TOOL HANDLER HERE
              #  swapping happens differently for subsurfaces and push/pull


              if (swapped)
                # Original face and new face were swapped!
                # Fix the original surface drawing interface so that it points to the new face.
                Plugin.log(OpenStudio::Info, "Copy-paste/divide surface:  new sub surface with swap")

                # Save the original entity to become the *new* surface below.
                original_entity = entity.drawing_interface.entity

                original_surface = entity.drawing_interface
                original_surface.remove_observers

                original_surface.entity = entity
                original_surface.entity.drawing_interface = original_surface
                original_surface.entity.model_object_handle = original_surface.model_object.handle

                original_surface.on_change_entity  # Recalculates vertices and paints the entity.
                original_surface.add_observers

                new_surface = SubSurface.new_from_entity(original_entity)
                #puts "new_surface = #{new_surface}, #{new_surface.model_object.name}, #{new_surface.entity}"
                #puts "new_surface.parent = #{new_surface.parent}, #{new_surface.parent.model_object.name}, #{new_surface.parent.entity}"

                # Must trigger the base surface to recalculate vertices to account for the new sub surface.
                original_surface.on_change_entity

              else
                # Normal sub surface--no swapping.
                Plugin.log(OpenStudio::Info, "Copy-paste/divide surface:  new sub surface no swap")
                original_surface = entity.drawing_interface

                new_surface = SubSurface.new_from_entity(entity)
                #puts "new_surface = #{new_surface}, #{new_surface.model_object.name}, #{new_surface.entity}"
                #puts "new_surface.parent = #{new_surface.parent}, #{new_surface.parent.model_object.name}, #{new_surface.parent.entity}"

                # Must trigger the base surface to recalculate vertices to account for the new sub surface.
                original_surface.on_change_entity

              end
            end
          end

        elsif (!entity.deleted? and (entity.is_a?(Sketchup::Group) or entity.is_a?(Sketchup::ComponentInstance) or entity.is_a?(Sketchup::ComponentDefinition)))

          if (drawing_interface = entity.drawing_interface)

            need_to_remove = false
            already_exists = false
            has_children = false
            error_message = ""

            if (drawing_interface.class == DaylightingControl)
              Plugin.log(OpenStudio::Info, "New DaylightingControl")

              if (@drawing_interface.class == Space)
                # see if we already have this object
                Plugin.model_manager.model_interface.daylighting_controls.each do |daylighting_control|
                  if daylighting_control.entity == entity
                    already_exists = true
                    break
                  end
                end
              else
                # not added to a space
                need_to_remove = true
                error_message = "Can only add DaylightingControl to a Space"
              end

            elsif(drawing_interface.class == IlluminanceMap)
              Plugin.log(OpenStudio::Info, "New IlluminanceMap")

              if (@drawing_interface.class == Space)
                # see if we already have this object
                Plugin.model_manager.model_interface.illuminance_maps.each do |illuminance_map|
                  if illuminance_map.entity == entity
                    already_exists = true
                    break
                  end
                end
              else
                # not added to a space
                need_to_remove = true
                error_message = "Can only add IlluminanceMap to a Space"
              end

            elsif(drawing_interface.class == Luminaire)
              Plugin.log(OpenStudio::Info, "New Luminaire")

              if (@drawing_interface.class == Space)
                # see if we already have this object
                Plugin.model_manager.model_interface.luminaires.each do |luminaire|
                  if luminaire.entity == entity
                    already_exists = true
                    break
                  end
                end
              else
                # not added to a space
                need_to_remove = true
                error_message = "Can only add Luminaire to a Space"
              end

            elsif(drawing_interface.class == ShadingSurfaceGroup)
              Plugin.log(OpenStudio::Info, "New ShadingSurfaceGroup")

              has_children = true

              if (@drawing_interface.class == Space)
                # see if we already have this object
                Plugin.model_manager.model_interface.shading_surface_groups.each do |shading_surface_group|
                  if shading_surface_group.entity == entity
                    already_exists = true
                    break
                  end
                end
              else
                # not added to a space
                need_to_remove = true
                error_message = "Can only add ShadingSurfaceGroup at Space or Building level"
              end

            elsif(drawing_interface.class == InteriorPartitionSurfaceGroup)
              Plugin.log(OpenStudio::Info, "New InteriorPartitionSurfaceGroup")

              has_children = true

              if (@drawing_interface.class == Space)
                # see if we already have this object
                Plugin.model_manager.model_interface.interior_partition_surface_groups.each do |interior_partition_surface_group|
                  if interior_partition_surface_group.entity == entity
                    already_exists = true
                    break
                  end
                end
              else
                # not added to a space
                need_to_remove = true
                error_message = "Can only add InteriorPartitionSurfaceGroup at Space level"
              end

            elsif(drawing_interface.class == Space)
              Plugin.log(OpenStudio::Info, "New Space")

              has_children = true

              need_to_remove = true
              error_message = "Can only add Space at Building level"
            else
              Plugin.log(OpenStudio::Info, "New #{drawing_interface.class}")

              need_to_remove = true
              error_message = "Unknown object of type #{drawing_interface.class} added"
            end

            if need_to_remove
              DrawingUtils.clean_entity(entity)
              Sketchup.active_model.entities.erase_entities(entity)
              Sketchup.send_action("selectSelectionTool:")
              UI.messagebox(error_message)
            elsif not already_exists
              new_interface = drawing_interface.class.new_from_entity_copy(entity)
              new_interface.update_entity if has_children
              #Sketchup.send_action("selectSelectionTool:")
            end

          else
            Plugin.log(OpenStudio::Info, "Unknown object added")
          end

        end

      }  # Proc

      Plugin.add_event( proc )

    end


    def onElementRemoved(entities, entity)

      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")

      return if not @enabled

      Plugin.log(OpenStudio::Info, "SurfaceGroupEntitiesObserver.onElementRemoved: removing #{entity.class} #{entity.to_s} from #{@drawing_interface.entity}")

      # sometimes fixnum objects are passed in for entity
      if entity.respond_to?("drawing_interface")

        proc = Proc.new {
          @drawing_interface.paint_entity
        }

        Plugin.add_event( proc )

      end
    end


    #def onElementModified(entities, entity)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end


    # Gets called when ALL entities are deleted, but only if the group is closed, or after the group is closed.
    # This would be a good place to prompt if the user wants to erase the space completely.
    #def onEraseEntities(entities)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end

  end

end
