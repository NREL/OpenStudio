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

require("openstudio/sketchup_plugin/lib/watchers/PluginModelObjectWatcher")

require("set")

module OpenStudio

  class DrawingInterface

    attr_accessor :model_object, :entity
    attr_reader :parent, :children, :model_interface, :observer, :watcher   # for debugging only


    def initialize
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @parent = nil
      @children = Set.new
      @model_interface = Plugin.model_manager.model_interface

      @model_object = nil  # Reference to the model object:  Space, Surface, Construction, etc.
      @entity = nil  # Reference to the SketchUp entity:  group, face, material, etc.
      @observer = nil  # This is overridden in each subclass.
      @observer_added = false # true if observer has been added to the entity
      @watcher = nil  # This is overridden in each subclass.

      @deleted = false

      @paint_requested = false
    end

    def destroy
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @parent = nil
      @children.clear
      @children = nil
      @model_interface = nil
    end

    def name
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      result = ""
      if @model_object and not @model_object.name.empty?
        result = @model_object.name.get
      end
      return result
    end

    def inspect
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(to_s)
    end


    # Redefine this method so that only a comparison of the same instance of the object will return true.
    # Similar to matching "this.object_id == other.object_id".
    # This is helpful for finding an instance in an array, for example.
    # DLM: This should be 'True if the receiver and argument have both the same type and equal values.'
    def eql?(this_object)
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(equal?(this_object))
    end


    # Redefine this method so that only a comparison of the same instance of the object will return true.
    # Similar to matching "this.object_id == other.object_id".
    # This is helpful for finding an instance in an array, for example.
    # DLM: This should be 'Checks if the value of two operands are equal or not, if yes then condition becomes true.'
    def ==(this_object)
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(eql?(this_object))
    end


##### Begin methods for the input object #####

    # Drawing interface is being created because a model object is being loaded.
    def self.model_object_from_handle(handle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      raise "Bad call to DrawingInterface.model_object_from_handle, override in derived class"
    end

    # Drawing interface is being created because a model object is being loaded.
    def self.new_from_handle(handle)
      raise "Bad call to DrawingInterface.new_from_handle, override in derived class"
    end

# check_model_object / accept_model_object?


    # Draws the input object as an entity.
    # This method probably should not be overridden in subclasses; override the internal methods instead.
    # For symmetry this method could be called 'create_from_model_object'.
    #
    # Remove/add of observers is disabled by passing in false as an argument.
    # This allows the model interface to draw many entities without the performance
    # hit to constantly add and remove observers on child and parent.
    # There was also some evidence that too much shuffling of observers was
    # causing BugSplats.
    def draw_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if (check_model_object)

        had_observers = remove_observers(true)

        # Needs to happen first since @parent is needed in 'check_model_object'
        old_parent = @parent
        new_parent = parent_from_model_object
        all_children = recurse_children
        parent_updated = false
        if old_parent !=  new_parent

          if old_parent
            # parent changed from one parent to another
            # this should be fairly rare as this only occurs when model object is changed, e.g. in a user script
            Plugin.log(OpenStudio::Trace, "old_parent = #{old_parent}, new_parent = #{new_parent}")

            parent_updated = true

            # parent changed, need to redraw self and any children, do the erase before changing @parent
            erase_entity
            all_children.each { |child| child.erase_entity }
          end

          # now update @parent
          update_parent_from_model_object
        end

        if (valid_entity?)  #
          update_entity   # This can call 'erase_entity'/'create_entity'
        else
          create_entity

          # DLM: this call to update_entity is problematic for PlanarSurface in the case that confirm_entity needs to reverse vertices
          # in that case this call to update entity will incorrectly think the vertices changed
          # DLM: this call to update_entity is needed for SpaceType etc so model is repainted when rendering color changes
          # these objects have no entity
          if not self.is_a? PlanarSurface
            update_entity  # This should go away maybe?
          end
        end

        if (confirm_entity)
          @entity.drawing_interface = self
          @entity.model_object_handle = @model_object.handle

          paint_entity
          #update_model_object

        else

          # catch errors here that occur during drawing

          if self.is_a? PlanarSurface

            if not self.redraw_scheduled
              # this surface shares the same face as another surface or could not be drawn for some other reason

              Plugin.log(OpenStudio::Error, "confirm_entity failed for model object #{@model_object}")

              had_observers = false
              destroy_observers
              destroy_watcher

              @entity = nil
              @parent.remove_child(self) if @parent
              @parent = nil
              @model_interface.remove_drawing_interface(self)
              @model_object.drawing_interface = nil
            end
          end
        end

        add_observers(true) if had_observers

        if parent_updated
          proc = Proc.new {
            # clean up parent (e.g. remove stray edges)
            old_parent.cleanup_entity if old_parent

            # redraw any children if parent changed
            all_children.each { |child| child.draw_entity }
          }
          Plugin.add_event( proc )
        end
      else

        Plugin.log(OpenStudio::Error, "check_model_object failed for model object #{@model_object}")

        destroy_observers
        destroy_watcher

        @entity = nil
        @parent.remove_child(self) if @parent
        @parent = nil
        @model_interface.remove_drawing_interface(self)
        @model_object.drawing_interface = nil
      end

      return(@entity)
    end


    # Creates new defaulted model model and adds it to the model, override in derived classes
    def create_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if not @model_object
        raise "Bad call to DrawingInterface.create_model_object, override in derived class"
      else
        @model_object.drawing_interface = self
      end

    end


    # This method handles delete status separately from the input object 'deleted?' flag.
    def deleted?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(@deleted)
    end


    # Checks needed before the entity can be drawn.
    # Checks the input object for errors and tries to fix them before drawing the entity.
    # Returns false if errors are beyond repair.
    def check_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if @model_object.nil?
        return(false)
      elsif @model_object.handle.isNull
        return(false)
      else
        return(true)
      end
    end


    # Updates the ModelObject with new information from the SketchUp entity.
    def update_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if (valid_entity?)
        update_parent_from_entity
      end
    end

    # selects all children which are not tied to drawing interfaces recursively
    # results are stored in handles (UUIDVector) and idf_objects (IdfObjectVector)
    def select_non_visible_children_recursive(model_object, handles, idf_objects, class_hash)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if not class_hash[model_object.iddObject.name.upcase]
        handles << model_object.handle if handles
        idf_objects << model_object.idfObject if idf_objects

        model_object.lifeCycleCosts.each do |lifeCycleCost|
          handles << lifeCycleCost.handle
          idf_objects << lifeCycleCost.idfObject
        end

        parent_object = model_object.to_ParentObject
        if not parent_object.empty?
          parent_object.get.children.each do |child|
            select_non_visible_children_recursive(child, handles, idf_objects, class_hash)
          end
        end
      end
    end

    # selects all children which are tied to drawing interfaces recursively
    # results are stored in handles (UUIDVector) and idf_objects (IdfObjectVector)
    def select_visible_children_recursive(model_object, handles, idf_objects, class_hash)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      Plugin.log(OpenStudio::Info, "Child #{model_object.class} with handle #{model_object.handle}")
      if class_hash[model_object.iddObject.name.upcase]
        Plugin.log(OpenStudio::Info, "Child #{model_object.class} with handle #{model_object.handle} selected")
        handles << model_object.handle if handles
        idf_objects << model_object.idfObject if idf_objects

        # life cycle costs are not associated with an entity

        parent_object = model_object.to_ParentObject
        if not parent_object.empty?
          Plugin.log(OpenStudio::Info, "Child #{model_object.class} has #{parent_object.get.children.size} children")
          parent_object.get.children.each do |child|
            Plugin.log(OpenStudio::Info, "Child #{model_object.class} has child #{child.class}")
            select_visible_children_recursive(child, handles, idf_objects, class_hash)
          end
        end
      end
    end

    # Deletes the model object and marks the drawing interface when the SketchUp entity is erased.
    def delete_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @deleted = true

      if (@model_object)

        destroy_watcher

        if (@model_interface.openstudio_model)

          # store any removed objects to restore if this drawing interface comes back indexed by this handle
          old_handle = @model_object.handle.to_s

          # collect handles to remove, do not include children if their type is mapped to an entitye
          class_hash = @model_interface.class_hash
          handles_to_remove = OpenStudio::UUIDVector.new
          idf_objects_to_restore = OpenStudio::IdfObjectVector.new
          handles_to_remove << @model_object.handle
          idf_objects_to_restore << @model_object.idfObject

          @model_object.lifeCycleCosts.each do |lifeCycleCost|
            handles_to_remove << lifeCycleCost.handle
            idf_objects_to_restore << lifeCycleCost.idfObject
          end

          parent_object = @model_object.to_ParentObject
          if not parent_object.empty?

            # needed so we don't get change signals for visible children not deleted here
            recurse_children.each {|child| child.disable_watcher}

            # we will not remove visible children here, those get deleted when their entity is deleted
            parent_object.get.children.each do |child|
              select_non_visible_children_recursive(child, handles_to_remove, idf_objects_to_restore, class_hash)
            end
          end

          Plugin.log(OpenStudio::Info,"delete_model_object of type #{@model_object.class} with handle #{old_handle} and #{handles_to_remove.size - 1} non-visible children")

          @model_interface.deleted_model_object_hash[old_handle] = idf_objects_to_restore

          model_watcher_enabled = @model_interface.model_watcher.disable
          @model_interface.openstudio_model.removeObjects(handles_to_remove)
          @model_interface.model_watcher.enable if model_watcher_enabled
        end
      end
    end

    # Called by the model object watcher
    def on_change_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      draw_entity
    end

    # Called by the model watcher.
    def on_pre_delete_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # store any removed objects to restore if this drawing interface comes back indexed by this handle
      old_handle = @model_object.handle.to_s

      # collect handles to remove, do not include children if their type is mapped to an entity
      class_hash = @model_interface.class_hash
      handles_to_remove = OpenStudio::UUIDVector.new
      idf_objects_to_restore = OpenStudio::IdfObjectVector.new
      handles_to_remove << @model_object.handle
      idf_objects_to_restore << @model_object.idfObject

      @model_object.lifeCycleCosts.each do |lifeCycleCost|
        handles_to_remove << lifeCycleCost.handle
        idf_objects_to_restore << lifeCycleCost.idfObject
      end

      parent_object = @model_object.to_ParentObject
      if not parent_object.empty?
        parent_object.get.children.each do |child|
          select_non_visible_children_recursive(child, handles_to_remove, idf_objects_to_restore, class_hash)
        end
      end

      Plugin.log(OpenStudio::Info,"delete_model_object of type #{@model_object.class} with handle #{old_handle} and #{handles_to_remove.size - 1} non-visible children")

      @model_interface.deleted_model_object_hash[old_handle] = idf_objects_to_restore

    end

    # Called by the model object watcher.
    def on_delete_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @deleted = true

      if (valid_entity?)
        erase_entity
      end

      @parent.remove_child(self) if (@parent)  # Duplicate line in 'update_parent_from_entity'
      @parent = nil
    end


    # Overridden for every subclass.
    def parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(nil)
    end


    # This method should not be overridden by subclasses.
    def update_parent_from_model_object
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      new_parent = parent_from_model_object

      if new_parent != @parent
        @parent.remove_child(self) if (@parent)
        @parent = new_parent
        @parent.add_child(self) if (@parent)
      end
    end


##### Begin methods for the entity #####


    #def self.valid_entity?(entity)
    #  if (entity.nil?)
    #    puts self.to_s + ".valid_entity?:  entity is nil"
    #    return(false)
    #  else
    #    return(true)
    #  end
    #end


    # Error checks before an entity can be accepted by an interface.
    # Does not modify the entity.
    # Returns false if the entity cannot be used.
    #def self.accept_entity?(entity)
    #  if (self.valid_entity?(entity))
    #    return(false)
    #  if (entity.drawing_interface)
    #    puts self.to_s + ".check_entity:  entity already has drawing interface"
    #    return(false)
    #  else
    #    return(true)
    #  end
    #end


    # Drawing interface is being created because a new entity was drawn by the user.
    def self.new_from_entity(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      #if (self.accept_entity?(entity))
        return(self.new.create_from_entity(entity))
      #end
    end


    def create_from_entity(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @entity = entity
      @entity.drawing_interface = self

      if (check_entity)  # class.check_entity(entity)   # should check before the interface accepts the entity
        remove_observers
        disable_watcher

        #attach_entity(entity)

        create_model_object

        if @model_object
          @entity.model_object_handle = @model_object.handle

          update_model_object

          #update_entity
          paint_entity

          add_watcher
          add_observers
        else
          puts "DrawingInterface.create_from_entity:  create_model_object failed"
        end

      else
        puts "DrawingInterface.create_from_entity:  check_entity failed"
      end

      return(self)
    end

    # Drawing interface is being created because an entity was copied or divided by the user.
    def self.new_from_entity_copy(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(new.create_from_entity_copy(entity))
    end


    def create_from_entity_copy(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      original_interface = entity.drawing_interface

      @entity = entity
      @entity.drawing_interface = self

      if (check_entity)
        # object will have no observers
        # object will have no watcher

        if (original_interface.model_object)

          model_watcher_enabled = @model_interface.model_watcher.disable

          Plugin.log(OpenStudio::Info, "Cloning #{original_interface} with model_object #{original_interface.model_object}")

          # clone will copy all children, even visible ones that are going to be copied again in SurfaceGroup.create_from_entity_copy
          @model_object = original_interface.model_object.clone(@model_interface.openstudio_model)
          @model_object = self.class.model_object_from_handle(@model_object.handle)
          @model_object.drawing_interface = self
          @entity.model_object_handle = @model_object.handle

          Plugin.log(OpenStudio::Info, "Cloning finished for #{self} with model_object #{@model_object}")

          # delete all children so they won't be copied twice
          class_hash = @model_interface.class_hash
          handles_to_remove = OpenStudio::UUIDVector.new
          idf_objects_to_remove = OpenStudio::IdfObjectVector.new

          # life cycle costs do not have sketchup entities

          parent_object = @model_object.to_ParentObject
          if not parent_object.empty?
            parent_object.get.children.each do |child|
              select_visible_children_recursive(child, handles_to_remove, idf_objects_to_remove, class_hash)
            end
          end

          Plugin.log(OpenStudio::Info, "Removing #{handles_to_remove.size} visible children #{idf_objects_to_remove}")
          @model_interface.openstudio_model.removeObjects(handles_to_remove)

          Plugin.dialog_manager.update_all
          @model_interface.model_watcher.enable if model_watcher_enabled

        end

        remove_observers
        disable_watcher

        update_model_object # will call update_parent_from_entity

        paint_entity

        add_watcher
        add_observers

      else
        puts "DrawingInterface.create_from_entity_copy:  check_entity failed"
      end
      return(self)
    end


    # attach an entity
    def attach_entity(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @entity = entity
      @entity.drawing_interface = self
      @entity.model_object_handle = @model_object.handle if (@model_object)
    end


    # detach an entity
    def detach_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      clean_entity

      @entity = nil
    end


    # Creates the entity appropriate for the class of input object.
    def create_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      raise "Bad call to DrawingInterface.create_entity, override in derived class"
    end


    def valid_entity?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return((not @entity.nil?) and (not @entity.deleted?) and @entity.valid? and (@entity.entityID > 0))
    end


    # Error checks and cleanup before an entity is accepted by the interface.
    # Return false if the entity cannot be used.
    # Drawing interfaces that don't correspond directly to a geometric entity (e.g., Site, Building)
    # should return false here.
    def check_entity  # accept_entity?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(valid_entity?)
    end


    # Error checks, finalization, or cleanup needed after the entity is drawn.
    def confirm_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(valid_entity?)
    end


    # Updates the SketchUp entity with new information from the ModelObject.
    # If necessary, this method will erase and re-draw the entity.
    def update_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end

    # for consistency with ModelInterface
    def request_paint
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if not @paint_requested
        @paint_requested = true
        proc = Proc.new { self.paint_now }
        Plugin.add_event( proc )
      end
    end

    def paint_now
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @paint_requested = false
      paint_entity
    end

    # Called by draw_entity, but also can be called independently to repaint everything under different paint modes.
    def paint_entity(info = nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end

    # Used by info tool
    def tooltip(flags = nil, inside_info = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return @model_object.to_s
    end

    # Final cleanup of the entity.
    # This method is called by the model interface after the entire input file is drawn.
    def cleanup_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    end


    def clean_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      remove_observers
      if (valid_entity?)
        entity.drawing_interface = nil
        entity.model_object_handle = nil
      end
    end


    # Erases the entity when the input object is deleted, or in preparation for a re-draw.
    def erase_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # for this section always really remove observers because the entity is going away
      was_disable_only = $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
      $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS = false
      remove_observers(true)
      $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS = was_disable_only

      if (valid_entity?)
        Plugin.log(OpenStudio::Debug, "valid")
        if containing_entity = self.containing_entity
          Plugin.log(OpenStudio::Debug, "containing_entity = #{containing_entity}")
          if ((not containing_entity.nil?) and containing_entity.valid?)
            Plugin.log(OpenStudio::Debug, "containing_entity ok")
            if containing_entity.is_a?(Sketchup::Model)
              Plugin.log(OpenStudio::Debug, "containing_entity is model")
              had_observers = @model_interface.remove_observers
              if containing_entity.entities.include?(@entity)
                Plugin.log(OpenStudio::Debug, "removing from entities")
                containing_entity.entities.erase_entities(@entity)
              elsif containing_entity.materials.include?(@entity)
                # this was added in 8.0 M1
                if containing_entity.materials.respond_to?("remove")
                  Plugin.log(OpenStudio::Debug, "removing from materials")
                  containing_entity.materials.remove(@entity)
                else
                  Plugin.log(OpenStudio::Debug, "could not remove material")
                end
              end
              @model_interface.add_observers if had_observers
            elsif ((not containing_entity.deleted?) and (containing_entity.entityID > 0))
              Plugin.log(OpenStudio::Debug, "containing_entity not deleted")
              containing_interface = containing_entity.drawing_interface
              had_observers = containing_interface.remove_observers if containing_interface

              #Plugin.log(OpenStudio::Debug, "attempting to remove entity #{@entity} from containing_entity #{containing_entity}")
              #Plugin.log(OpenStudio::Debug, "@entity.entityID = #{@entity.entityID}")
              #count = 0
              #containing_entity.entities.each do |entity|
              #  if entity.entityID == @entity.entityID
              #    count += 1
              #  end
              #end
              #Plugin.log(OpenStudio::Debug, "found #{count} entities with this id")

              containing_entity.entities.erase_entities(@entity)
              containing_interface.add_observers if had_observers
              Plugin.log(OpenStudio::Debug, "entity erased from containing_entity.entities")
            end
          end
        end
      end
    end


    def on_change_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      watcher_enabled = disable_watcher

      update_model_object
      paint_entity  # Needed to fix the floor surface colors after a push/pull into a box.

      enable_watcher if watcher_enabled
    end


    def on_erase_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      delete_model_object

      @parent.remove_child(self) if (@parent)  # Duplicate line in 'update_parent_from_entity'
      @parent = nil
    end


    # Undelete happens when an entity is restored after an Undo event.
    def on_undelete_entity(entity)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @deleted = false
      @model_object.drawing_interface = nil if @model_object
      @model_object = nil

      old_handle = entity.model_object_handle

      Plugin.log(OpenStudio::Info, "on_undelete_entity #{entity}, old_handle = #{old_handle}")

      if old_handle

        # objects to restore
        if idf_objects_to_restore = @model_interface.deleted_model_object_hash[old_handle]

          model_watcher_enabled = @model_interface.model_watcher.disable

          # add objects to current model
          added_objects = @model_interface.openstudio_model.addObjects(idf_objects_to_restore)

          Plugin.dialog_manager.update_all
          @model_interface.model_watcher.enable if model_watcher_enabled

          if added_objects.size > 0
            # cast model object to correct type, first object in added should be this
            @model_object = self.class.model_object_from_handle(added_objects[0].handle)
            @model_object.drawing_interface = self
            @model_interface.add_drawing_interface(self) # will not add a duplicate
            Plugin.log(OpenStudio::Info, "undeleted model object of type #{@model_object.class}")
          end
        end

        # objects are restored get rid of stored backups
        @model_interface.deleted_model_object_hash[old_handle]  = nil
      end

      @entity = entity  # The entity comes back with a different reference than it had originally.
      @entity.drawing_interface = self  # The reference to the drawing interface is lost when it is deleted.
      if (@model_object)
        @entity.model_object_handle = @model_object.handle
        add_watcher
      else
        @entity.model_object_handle = nil
      end

      on_change_entity  # Restore the parent links, etc.
      add_observers

    end


    # Overridden for every subclass.
    def parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(nil)
    end


    # This method should not be overridden by subclasses.
    def update_parent_from_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      new_parent = parent_from_entity

      if @parent != new_parent
        @parent.remove_child(self) if (@parent)
        @parent = new_parent
        @parent.add_child(self) if (@parent)
      end
    end


    def containing_entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return(@parent.entity)
    end

    # Returns the coordinate transformation of this entity relative to the parent (e.g. what OpenStudio wants)
    # undoes all edit_transform nonsense from SketchUp
    def coordinate_transformation
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      #Plugin.log(OpenStudio::Info, "coordinate_transformation @model_object = #{@model_object}")
      #Plugin.log(OpenStudio::Info, "coordinate_transformation @entity = #{@entity}")

      transform = Geom::Transformation.new
      if self.entity.is_a?(Sketchup::Group) or self.entity.is_a?(Sketchup::ComponentInstance)
        # if nothing in active_path this is sufficient
        transform = @entity.transformation
      end
      #Plugin.log(OpenStudio::Info, "coordinate_transformation 1, transform = #{transform.to_a.join(',')}")

      # active_path is entities which are open, e.g. inside a group
      if active_path = @model_interface.skp_model.active_path
        active_path_transform = @model_interface.active_path_transform

        if active_path_transform.nil?
          raise "Invalid active path transform"
        end

        # add transformation for this entity back in
        if index = active_path.index(self.entity)
          transform = active_path_transform[index]*transform
          #Plugin.log(OpenStudio::Info, "coordinate_transformation 2, in active path, active_path_transform[index] = #{active_path_transform[index].to_a.join(',')}, transform = #{transform.to_a.join(',')}")
        end

        # if we are inside of a parent then children are reported in absolute coordinates, need to undo that
        parent_inverse_transform = Geom::Transformation.new
        parent_interface = self.parent

        while parent_interface and parent_interface.entity.is_a?(Sketchup::Face)
          parent_interface = parent_interface.parent
        end

        #Plugin.log(OpenStudio::Info, "coordinate_transformation=, parent_interface = #{parent_interface}")
        if parent_interface
          #Plugin.log(OpenStudio::Info, "coordinate_transformation=, parent_interface.entity = #{parent_interface.entity}")
          if index = active_path.index(parent_interface.entity)
            parent_inverse_transform = parent_inverse_transform * active_path_transform[index].inverse
            #Plugin.log(OpenStudio::Info, "coordinate_transformation 3, parent in active path, active_path_transform[index].inverse = #{active_path_transform[index].inverse.to_a.join(',')}, transform = #{transform.to_a.join(',')}")
          end
        end

        # remove parent transform
        transform = parent_inverse_transform * transform
        #Plugin.log(OpenStudio::Info, "coordinate_transformation 4, parent_inverse_transform = #{parent_inverse_transform.to_a.join(',')}, transform = #{transform.to_a.join(',')}")
      end

      return(transform)
    end

    # Sets the coordinate transformation of this entity relative to the parent (e.g. what OpenStudio wants)
    # undoes all edit_transform nonsense from SketchUp
    def coordinate_transformation=(transform)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      #Plugin.log(OpenStudio::Info, "coordinate_transformation=, #{@model_object}")
      #Plugin.log(OpenStudio::Info, "coordinate_transformation=, transform = #{transform.to_a.join(',')}")

      if self.entity.is_a?(Sketchup::Group) or self.entity.is_a?(Sketchup::ComponentInstance)

        # active_path is entities which are open, e.g. inside a group
        if active_path = @model_interface.skp_model.active_path
          active_path_transform = @model_interface.active_path_transform

          if active_path_transform.nil?
            raise "Invalid active path transform"
          end

          # changing transformation for entity in active path would involve setting edit_transform, I don't think we
          # want to do this, this case should not happen any way (e.g. moving a space while in the space)
          if index = active_path.index(self.entity)
            Plugin.log(OpenStudio::Error, "Cannot set entity transformation while entity is in the active_path")
            return(nil)
          else

            # if we are inside of a parent then children are reported in absolute coordinates, need to redo that
            parent_transform = Geom::Transformation.new
            parent_interface = self.parent

            while parent_interface and parent_interface.entity.is_a?(Sketchup::Face)
              parent_interface = parent_interface.parent
            end

            #Plugin.log(OpenStudio::Info, "coordinate_transformation=, parent_interface = #{parent_interface}")
            if parent_interface
              #Plugin.log(OpenStudio::Info, "coordinate_transformation=, parent_interface.entity = #{parent_interface.entity}")
              if index = active_path.index(parent_interface.entity)
                transform = active_path_transform[index]*transform
                #Plugin.log(OpenStudio::Info, "coordinate_transformation=, parent in active path, active_path_transform[index] = #{active_path_transform[index].to_a.join(',')}, transform = #{transform.to_a.join(',')}")
              end
            end

          end
        end

        @entity.transformation = transform
        #Plugin.log(OpenStudio::Info, "coordinate_transformation=, transform = #{transform.to_a.join(',')}")

      end
    end

##### Begin methods for the interface #####


    # Attaches any Observer classes, usually called after all drawing is complete.
    # Also called to reattach an Observer when a drawing interface is restored via undo.
    # This method should be overriden by subclasses.
    def add_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if (valid_entity?)
        if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
          if not @observer_added
            @entity.add_observer(@observer)
            @observer_added = true
          end
          @observer.enable
        else
          @entity.add_observer(@observer)
          @observer_added = true
          @observer.enable
        end
      end

      if recursive
        recurse_children.each { |interface| interface.add_observers(recursive) }
      end

    end


    # This method can be overriden by subclasses.
    def remove_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      had_observers = false
      if (valid_entity?)
        if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
          if @observer_added
            had_observers = @observer.disable
          end
        else
          had_observers = @entity.remove_observer(@observer)
          @observer.disable
          @observer_added = false
        end
      end

      if recursive
        recurse_children.each { |interface| interface.remove_observers(recursive) }
      end

      return had_observers
    end

    # This method can be overriden by subclasses.
    def destroy_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = false
      if @observer
        if (valid_entity?)
          if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
            # actually do remove here
            @entity.remove_observer(@observer)
            @observer.disable
            @observer_added = false
          else
            @entity.remove_observer(@observer)
            @observer.disable
            @observer_added = false
          end
        end
        @observer.destroy
        @observer = nil
        result = true

        if recursive
          recurse_children.each { |interface| interface.destroy_observers(recursive) }
        end
      end

      return result

    end

    # This method can be overridden by subclasses.
    def add_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if (@model_object)
        @watcher = PluginModelObjectWatcher.new(self)
      end
    end


    # This method should not be overridden by subclasses.
    def destroy_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = false
      if (@watcher)
        @watcher.disable
        @watcher = nil
        result = true
      end
      return result
    end

    def disable_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = false
      if (@watcher)
        result = @watcher.disable
      end

      return result
    end

    def enable_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @watcher.enable
    end

    # This method should not be overridden by subclasses.
    def add_child(child)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @children.add(child)
      @model_interface.add_drawing_interface(child)
    end

    def in_selection?(selection)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return (selection.contains?(@entity))
    end

    # This method should not be overridden by subclasses.
    def remove_child(child)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @children.delete(child)
      @model_interface.remove_drawing_interface(child)
    end


    # This method should not be overridden by subclasses.
    def recurse_children
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = @children.to_a
      @children.each { |interface| result.concat(interface.recurse_children) }
      return(result)
    end


  end

end
