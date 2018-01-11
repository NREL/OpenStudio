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

module OpenStudio

  class ModelEntitiesObserver < Sketchup::EntitiesObserver
  
    def initialize(model_interface)
      @model_interface = model_interface
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
      @model_interface = nil
      @enabled = false
    end    
    
    def onElementAdded(entities, entity)
    
      Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
      
      return if not @enabled

      if (!entity.deleted? and entity.class == Sketchup::Group)
        #puts "ModelEntities.onElementAdded:" + entity.to_s

        if (entity.drawing_interface)
          if (entity.drawing_interface.deleted?)
            # This is a cut-paste/delete-undo event.

            # Important to have put Timers here inside the if-then logic, rather
            # than outside as its done in SurfaceGroupEntitiesObserver.
            # Otherwise if a space is added with the New Space Tool, the space
            # ends up getting added twice.
            proc = Proc.new {
              #puts "cut-paste/delete-undo surface group"
              
              # DLM: what if entity is deleted now?

              entity.drawing_interface.on_undelete_entity(entity)
            }
            
            Plugin.add_event( proc )

          else
            # This is a copy-paste event.

            proc = Proc.new {
              #puts "copy-paste surface group"
              
              # DLM: what if entity is deleted now?

              # At this point SketchUp has created a new Group object for 'entity'.
              # BUT the new Group and the original Group both reference the same ComponentDefinition
              # which has a single Entities object.  That means both groups share the same
              # entities.  Apparently SketchUp doesn't create a new ComponentDefinition until
              # it decides it really needs one.

              # Solution is to "touch" the new Group to force SketchUp to create a new
              # ComponentDefinition and a new Entities object.
              cpoint = entity.entities.add_cpoint(Geom::Point3d.new(0,0,0)) 
              cpoint.erase!

              if drawing_interface = entity.drawing_interface

                need_to_remove = false
                already_exists = false
                has_children = false
                error_message = ""

                if(drawing_interface.class == Space)
                  Plugin.log(OpenStudio::Info, "New Space") 

                  has_children = true

                  # see if we already have this object
                  Plugin.model_manager.model_interface.spaces.each do |space| 
                    if space.entity == entity
                      already_exists = true
                      break
                    end
                  end

                elsif(drawing_interface.class == ShadingSurfaceGroup)
                  Plugin.log(OpenStudio::Info, "New ShadingSurfaceGroup") 

                  has_children = true

                  # see if we already have this object
                  Plugin.model_manager.model_interface.shading_surface_groups.each do |shading_surface_group| 
                    if shading_surface_group.entity == entity
                      already_exists = true
                      break
                    end
                  end

                elsif(drawing_interface.class == InteriorPartitionSurfaceGroup)
                  Plugin.log(OpenStudio::Info, "New InteriorPartitionSurfaceGroup") 

                  has_children = true

                  need_to_remove = true
                  error_message = "Can only add InteriorPartitionSurfaceGroup at Space level"
                else
                  Plugin.log(OpenStudio::Info, "Unknown object added") 

                  need_to_remove = true
                  error_message = "Unknown object of type #{drawing_interface.class} added"
                end

                if need_to_remove
                  Plugin.log(OpenStudio::Info, "Removing object of type #{drawing_interface.class}") 
                  DrawingUtils.clean_entity(entity)
                  Sketchup.active_model.entities.erase_entities(entity)
                  Sketchup.send_action("selectSelectionTool:")
                  UI.messagebox(error_message)
                elsif not already_exists
                  Plugin.log(OpenStudio::Info, "New object of type #{drawing_interface.class} from entity copy") 
                  new_interface = drawing_interface.class.new_from_entity_copy(entity)
                  new_interface.update_entity if has_children
                end 

              end
            }
            
            Plugin.add_event( proc )
            
          end

        else
          # A new Group entity was added.
          # New spaces and shading groups are added with a tool instead of an Observer event.
          # There is nothing to handle here.

        end
      else
        # A class other than Group was added to the Model.

        if (!entity.deleted? and entity.drawing_interface)
          # An EnergyPlus object was pasted outside of a Space or Shading Group.

          proc = Proc.new {
            #puts "explode/paste object outside of surface group"

            # Clean the entity.  Be careful:  cannot call 'drawing_interface.clean_entity' because it
            # will clean the original drawing interface, not this copy.
            DrawingUtils.clean_entity(entity)
          }
          
          Plugin.add_event( proc )
          
        end

      end

    end


    #def onElementRemoved(entities, entity)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end


    # This method gets called for every edge that gets drawn...so gets lots of hits!
    # UPDATE:  Broken in SU6 M3--was working in previous versions!  *args should catch any number of arguments, even none.
    #def onContentsModified(*args)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end


    # Only gets called when the model closes, I think.
    #def onEraseEntities(entities)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #end

  end

end
