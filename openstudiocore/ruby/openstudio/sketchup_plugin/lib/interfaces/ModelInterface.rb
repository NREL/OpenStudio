######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

require("openstudio/sketchup_plugin/lib/interfaces/Site")
require("openstudio/sketchup_plugin/lib/interfaces/Building")
require("openstudio/sketchup_plugin/lib/interfaces/Space")
require("openstudio/sketchup_plugin/lib/interfaces/ShadingSurfaceGroup")
require("openstudio/sketchup_plugin/lib/interfaces/InteriorPartitionSurfaceGroup")
require("openstudio/sketchup_plugin/lib/interfaces/Surface")
require("openstudio/sketchup_plugin/lib/interfaces/SubSurface")
require("openstudio/sketchup_plugin/lib/interfaces/ShadingSurface")
require("openstudio/sketchup_plugin/lib/interfaces/InteriorPartitionSurface")
require("openstudio/sketchup_plugin/lib/interfaces/DaylightingControl")
require("openstudio/sketchup_plugin/lib/interfaces/IlluminanceMap")
require("openstudio/sketchup_plugin/lib/interfaces/Luminaire")
require("openstudio/sketchup_plugin/lib/interfaces/GlareSensor")
require("openstudio/sketchup_plugin/lib/interfaces/RenderingColor")
require("openstudio/sketchup_plugin/lib/interfaces/ConstructionBase")
require("openstudio/sketchup_plugin/lib/interfaces/ThermalZone")
require("openstudio/sketchup_plugin/lib/interfaces/BuildingStory")
require("openstudio/sketchup_plugin/lib/interfaces/SpaceType")

require("openstudio/sketchup_plugin/lib/interfaces/MaterialsInterface")
require("openstudio/sketchup_plugin/lib/interfaces/SelectionInterface")
require("openstudio/sketchup_plugin/lib/interfaces/ResultsInterface")

require("openstudio/sketchup_plugin/lib/watchers/PluginPathWatcher")
require("openstudio/sketchup_plugin/lib/watchers/PluginModelWatcher")

require("openstudio/sketchup_plugin/lib/observers/ModelObserver.rb")
require("openstudio/sketchup_plugin/lib/observers/ModelEntitiesObserver.rb")

require("openstudio/sketchup_plugin/lib/dialogs/ProgressDialog")

if Sketchup.version_number > 14000000
  require("set")
else
  require("openstudio/sketchup_plugin/stdruby/set")
end

module OpenStudio

  # A ModelInterface ties a SketchUp Model to a OpenStudio Model
  class ModelInterface

    attr_accessor :skp_model, :skp_model_guid, :openstudio_model, :model_temp_dir
    attr_accessor :parent, :children, :interfaces, :error_log, :unviewed_errors
    attr_accessor :entities_observer, :model_observer
    attr_accessor :model_watcher, :path_watcher
    attr_accessor :materials_interface, :selection_interface, :results_interface
    attr_accessor :deleted_model_object_hash
    attr_accessor :untranslated_idf_objects
    attr_reader :class_hash, :paint_requested

    def initialize(skp_model, openstudio_model, openstudio_path = nil)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @skp_model = skp_model
      @skp_model_guid = skp_model.guid
      @openstudio_model = openstudio_model

      @skp_model.model_interface = self
      @skp_model.openstudio_path = openstudio_path
      
      @model_temp_dir = OpenStudio::Openstudiolib::createModelTempDir
      #puts "@model_temp_dir = #{@model_temp_dir}"
      if (openstudio_path)
        OpenStudio::Openstudiolib::initializeModelTempDir(OpenStudio::Path.new(openstudio_path), @model_temp_dir)
      end
      OpenStudio::Openstudiolib::updateModelTempDir(@openstudio_model, @model_temp_dir)
      
      @parent = nil
      @children = Set.new  
      
      # populate hash of interface class to set of interfaces
      # optimization to avoid recursing all children all the time
      @interfaces = Hash.new  
      @interfaces[Site] = Set.new
      @interfaces[Building] = Set.new
      @interfaces[Space] = Set.new
      @interfaces[ShadingSurfaceGroup] = Set.new
      @interfaces[InteriorPartitionSurfaceGroup] = Set.new
      @interfaces[Surface] = Set.new
      @interfaces[SubSurface] = Set.new
      @interfaces[ShadingSurface] = Set.new
      @interfaces[InteriorPartitionSurface] = Set.new
      @interfaces[DaylightingControl] = Set.new
      @interfaces[IlluminanceMap] = Set.new
      @interfaces[Luminaire] = Set.new
      @interfaces[GlareSensor] = Set.new      
      @interfaces[RenderingColor] = Set.new
      @interfaces[ConstructionBase] = Set.new
      @interfaces[ThermalZone] = Set.new
      @interfaces[BuildingStory] = Set.new
      @interfaces[SpaceType] = Set.new
      
      # observers for this model
      @model_observer = ModelObserver.new(self)
      @entities_observer = ModelEntitiesObserver.new(self)
      @observer_added = false # true if observer has been added to the entity
      
      # openstudio watchers for this model
      @model_watcher = PluginModelWatcher.new(self)
      if openstudio_path
        @path_watcher = PluginPathWatcher.new(self, OpenStudio::Path.new(openstudio_path))
      else
        @path_watcher = nil
      end
      
      # sub model interfaces for this model
      @materials_interface = MaterialsInterface.new(self)
      @selection_interface = SelectionInterface.new(self)
      @results_interface = ResultsInterface.new(self)
      
      #search for results
      sql_files = Dir.glob("#{@model_temp_dir}/**/eplusout.sql")
      if not sql_files.empty?
        @results_interface.output_file_path = sql_files[0]
      end
      
       # keep track of errors for this model
      @error_log = ""
      @unviewed_errors = false
      
      # keep track of when we are in an operation
      @operation_active = false
      
      # idf objects left over from the reverse translator
      @untranslated_idf_objects = []
      
      # record the edit_transform at each stage in the active path
      @active_path_transform = []
      
      # map model_object handle to a vector of idf objects to restore if the model_object ever comes back
      @deleted_model_object_hash = Hash.new

      # Setup the class lookup hash for creating drawing interfaces from input objects.
      @class_hash = Hash.new
      @class_hash['OS:SITE'] = Site
      @class_hash['OS:BUILDING'] = Building
      @class_hash['OS:SPACE'] = Space
      @class_hash['OS:SHADINGSURFACEGROUP'] = ShadingSurfaceGroup
      @class_hash['OS:INTERIORPARTITIONSURFACEGROUP'] = InteriorPartitionSurfaceGroup
      @class_hash['OS:SURFACE'] = Surface
      @class_hash['OS:SUBSURFACE'] = SubSurface
      @class_hash['OS:SHADINGSURFACE'] = ShadingSurface
      @class_hash['OS:INTERIORPARTITIONSURFACE'] = InteriorPartitionSurface
      @class_hash['OS:DAYLIGHTING:CONTROL'] = DaylightingControl
      @class_hash['OS:ILLUMINANCEMAP'] = IlluminanceMap
      @class_hash['OS:LUMINAIRE'] = Luminaire
      @class_hash['OS:GLARE:SENSOR'] = GlareSensor
      @class_hash['OS:RENDERING:COLOR'] = RenderingColor
      @class_hash['OS:CONSTRUCTION'] = ConstructionBase
      @class_hash['OS:CONSTRUCTION:CFACTORUNDERGROUNDWALL'] = ConstructionBase
      @class_hash['OS:CONSTRUCTION:FFACTORGROUNDFLOOR'] = ConstructionBase
      @class_hash['OS:CONSTRUCTION:INTERNALSOURCE'] = ConstructionBase
      @class_hash['OS:CONSTRUCTION:WINDOWDATAFILE'] = ConstructionBase
      @class_hash['OS:THERMALZONE'] = ThermalZone
      @class_hash['OS:BUILDINGSTORY'] = BuildingStory
      @class_hash['OS:SPACETYPE'] = SpaceType
      
      # Setup the order in which interfaces are drawn in the model.
      @draw_order = [Site, 
                     Building, 
                     Space, 
                     ShadingSurfaceGroup, 
                     InteriorPartitionSurfaceGroup, 
                     Surface, 
                     SubSurface, 
                     ShadingSurface, 
                     InteriorPartitionSurface, 
                     DaylightingControl, 
                     IlluminanceMap, 
                     Luminaire,
                     GlareSensor,
                     RenderingColor,
                     ConstructionBase,
                     ThermalZone,
                     BuildingStory,
                     SpaceType]
                    
      @paint_requested = false
    end

    def entity
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@skp_model)
    end

    def inspect
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(to_s)
    end
    
    # keeps track of the edit_transform at each point in the active_path
    def active_path_transform
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@active_path_transform)
    end

    # keeps track of the edit_transform at each point in the active_path
    def update_active_path_transform
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      active_path_transform_size = @active_path_transform.size 
      active_path_size = 0

      active_path = @skp_model.active_path
      active_path_size = active_path.size if active_path

      if (active_path_transform_size > active_path_size)
        @active_path_transform.pop
      elsif (active_path_transform_size < active_path_size)
        @active_path_transform << @skp_model.edit_transform
      end
    end
    
    # get name of SketchUp file
    def skp_name
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      name = nil
      if (@skp_model.path.empty?)
        name = "Untitled"
      else
        name = File.basename(@skp_model.path, ".skp")
      end
      return(name)
    end

    # get dir of SketchUp file
    def skp_dir
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      dir = nil
      if (not @skp_model.path.empty?)
        dir = File.dirname(@skp_model.path)
      end
      return(dir)
    end
    
    def openstudio_path
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return @skp_model.openstudio_path
    end

    # path to OpenStudio file that has already been saved
    # be very careful about where you call this function from as it creates a new path watcher
    def openstudio_path=(path)  
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    
      current_path = openstudio_path
      
      if current_path and current_path == path
        return true
      end 
    
      # destroy current watcher
      destroy_path_watcher
      
      @skp_model.openstudio_path = path
    
      # add a new path watcher
      add_path_watcher
    end      

    # get name of OpenStudio file
    def openstudio_name
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      name = nil
      if (openstudio_path)
        name = File.basename(openstudio_path)
      else
        name = File.basename(skp_name) + ".osm"
      end
      return(name)
    end


    # get directory for OpenStudio file
    def openstudio_dir
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      dir = nil
      if (openstudio_path)
        dir = File.dirname(openstudio_path)      
      elsif (skp_dir)
        dir = skp_dir
      else
        dir = Plugin.read_pref("Last OpenStudio Dir")
      end
      return(dir)
    end
    
    # export an openstudio model
    def export_openstudio(path) 
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      if @path_watcher
        #puts "ignoring path_watcher #{@path_watcher}"
        @path_watcher.disable
      end
      
      begin
        FileUtils.mkdir_p(File.dirname(path))

        saved_path = OpenStudio::Openstudiolib::saveModel(@openstudio_model, OpenStudio::Path.new(path), @model_temp_dir)

        # save run manager database first so saveModelTempDir copies it
        # ACS: Commenting this out to prevent scanning for tools (creates run.db for running within SketchUp)
        #OpenStudio::Openstudiolib::saveRunManagerDatabase(saved_path, @model_temp_dir, false) 
        
        OpenStudio::Openstudiolib::saveModelTempDir(@model_temp_dir, saved_path)
        
        result = true
      end
      
      if @path_watcher
        # if path changed then we will have a new path watcher
        @path_watcher_to_unignore = @path_watcher
        proc = Proc.new {
          #puts "@path_watcher #{@path_watcher}, @path_watcher_to_unignore #{@path_watcher_to_unignore}"
          if @path_watcher == @path_watcher_to_unignore
            #puts "un-ignoring path_watcher #{@path_watcher}"
            @path_watcher.clearState
            @path_watcher.enable
          end
        }
        Plugin.add_event( proc )
      end
            
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
      return(result)
    end

    # save the osm file and resources files
    def save_model_temp_dir 
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      saved_path = self.openstudio_path
      
      if not openstudio_path or openstudio_path.empty?
        return false
      end
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      if @path_watcher
        #puts "ignoring path_watcher #{@path_watcher}"
        @path_watcher.disable
      end
      
      result = false
      
      begin
        # do not want to save the database or osm here
        OpenStudio::Openstudiolib::saveModelTempDir(@model_temp_dir, OpenStudio::Path.new(saved_path))
        result = true
      end
      
      if @path_watcher
        # if path changed then we will have a new path watcher
        @path_watcher_to_unignore = @path_watcher
        proc = Proc.new {
          #puts "@path_watcher #{@path_watcher}, @path_watcher_to_unignore #{@path_watcher_to_unignore}"
          if @path_watcher == @path_watcher_to_unignore
            #puts "un-ignoring path_watcher #{@path_watcher}"
            @path_watcher.clearState
            @path_watcher.enable
          end
        }
        Plugin.add_event( proc )
      end
            
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
      return(result)
    end
    
    # export untranslated EnergyPlus objects to idf
    def export_untranslated_idf(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      
      begin
        FileUtils.mkdir_p(File.dirname(path))
        File.open(path, 'w') do |file|
          @untranslated_idf_objects.each do |object|
            file.puts object.to_s
          end
        end
        result = true
      end
      
      return(result)
      
    end
    
    # export an EnergyPlus idf
    def export_idf(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      clear_errors
      
      result = false
      
      workspace, errors, warnings = Plugin.model_manager.model_to_workspace(@openstudio_model)

      if errors.empty?
        errors = nil
      else
        add_error("Errors occurred on translation to EnergyPlus\n\n", false)
        errors.each {|error| add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if warnings.empty?
        warnings = nil
      else
        add_error("Warnings occurred on translation to EnergyPlus\n\n", false)
        warnings.each {|warning| add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end
      
      if Plugin.read_pref('Show Errors on Idf Translation') and errors
        show_errors
      elsif Plugin.read_pref('Show Warnings on Idf Translation') and (errors or warnings)
        show_errors
      end
      
      begin
        FileUtils.mkdir_p(File.dirname(path))
        File.open(path, 'w') do |file|
          file.puts workspace.toIdfFile.to_s
        end
        result = true
      end

      return(result)
    end

    #export a gbXML path
    def export_gbxml(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      clear_errors
      
      translator = OpenStudio::GbXML::GbXMLForwardTranslator.new
      
      result = nil
      if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS 
        progress_dialog = ProgressDialog.new("Translating Model to gbXML")
        result = translator.modelToGbXML(@openstudio_model, OpenStudio::Path.new(path), progress_dialog)
        progress_dialog.destroy
      else
        result = translator.modelToGbXML(@openstudio_model, OpenStudio::Path.new(path))
      end
      
      errors = translator.errors
      warnings = translator.warnings
    
      if errors.empty?
        errors = nil
      else
        add_error("Errors occurred on translation to gbXML\n\n", false)
        errors.each {|error| add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if warnings.empty?
        warnings = nil
      else
        add_error("Warnings occurred on translation to gbXML\n\n", false)
        warnings.each {|warning| add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end

      if Plugin.read_pref('Show Errors on gbXML Translation') and not result
        show_errors
      end

      return(result)
    end

    #export a sdd path
    def export_sdd(path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      clear_errors
      
      translator = OpenStudio::SDD::SddForwardTranslator.new
      
      result = nil
      if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS 
        progress_dialog = ProgressDialog.new("Translating Model to SDD")
        result = translator.modelToSDD(@openstudio_model, OpenStudio::Path.new(path), progress_dialog)
        progress_dialog.destroy
      else
        result = translator.modelToSDD(@openstudio_model, OpenStudio::Path.new(path))
      end
      
      errors = translator.errors
      warnings = translator.warnings
    
      if errors.empty?
        errors = nil
      else
        add_error("Errors occurred on translation to SDD\n\n", false)
        errors.each {|error| add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if warnings.empty?
        warnings = nil
      else
        add_error("Warnings occurred on translation to SDD\n\n", false)
        warnings.each {|warning| add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end

      if Plugin.read_pref('Show Errors on SDD Translation') and not result
        show_errors
      end

      return(result)
    end
    
    # import constructions from another model
    def import_constructions(other_model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      idd_object_types = ["OS_DefaultConstructionSet".to_IddObjectType,
                          "OS_DefaultSurfaceConstructions".to_IddObjectType,
                          "OS_DefaultSubSurfaceConstructions".to_IddObjectType,
                          "OS_Construction".to_IddObjectType,
                          "OS_Construction_CfactorUndergroundWall".to_IddObjectType,
                          "OS_Construction_FfactorGroundFloor".to_IddObjectType,
                          "OS_Construction_InternalSource".to_IddObjectType,
                          "OS_Construction_WindowDataFile".to_IddObjectType]
                          
      # don't use clone yet to avoid duplicates, need to use clone eventually                    
      temp_model = import_objects_with_targets(other_model, idd_object_types)
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      @model_watcher.disable
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      remove_observers
      
      begin
        self.start_operation("Import Constructions", true)
        new_objects = @openstudio_model.insertObjects(temp_model.objects(true))
        new_objects.each {|new_object| on_new_model_object(new_object)}
      ensure
        self.commit_operation
      end
      
      add_observers
      
      @model_watcher.enable
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
    end
    
    # import schedules from another model
    def import_schedules(other_model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      idd_object_types = ["OS_DefaultScheduleSet".to_IddObjectType,
                          "OS_Schedule_Ruleset".to_IddObjectType,
                          "OS_Schedule_Rule".to_IddObjectType,
                          "OS_Schedule_VariableInterval".to_IddObjectType,
                          "OS_Schedule_FixedInterval".to_IddObjectType,
                          "OS_Schedule_Constant".to_IddObjectType,
                          "OS_Schedule_Compact".to_IddObjectType]
                        
      # don't use clone yet to avoid duplicates, need to use clone eventually
      temp_model = import_objects_with_targets(other_model, idd_object_types)
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      @model_watcher.disable
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      remove_observers
      
      begin
        self.start_operation("Import Schedules", true)
        new_objects = @openstudio_model.insertObjects(temp_model.objects(true))
        new_objects.each {|new_object| on_new_model_object(new_object)}
      ensure
        self.commit_operation
      end
      
      add_observers
      
      @model_watcher.enable
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
    end
    
    # import space types from another model
    def import_space_types(other_model)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # transfer types to temp model
      # use clone here to get children, may create duplicate schedules, etc
      other_model.getSpaceTypes.each {|object| object.clone(@openstudio_model)}
    end

    # get handles to all objects and their targets recursively
    def get_target_handles_recursively(object, handles)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      handles << object.handle
      object.targets.each {|target| get_target_handles_recursively(target, handles)}
    end
    
    # returns a workspace with all objects of the selected types and all recursive targets
    def import_objects_with_targets(workspace, idd_object_types)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # handles of objects to insert
      handles = OpenStudio::UUIDVector.new
      
      # select objects
      idd_object_types.each do |idd_object_type|
        workspace.getObjectsByType(idd_object_type).each {|object| get_target_handles_recursively(object, handles) }
      end
      
      # only add each object only once
      handles_set = Set.new
      objects = OpenStudio::IdfObjectVector.new
      handles.each do |handle| 
        objects << workspace.getObject(handle).get.idfObject if not handles_set.include?(handle.to_s)
        handles_set.add(handle.to_s)
      end

      # create a return workspace
      result = OpenStudio::Workspace.new(workspace.strictnessLevel, workspace.iddFileType)
      result.addObjects(objects)

      return (result)
    end
    
    # import construction objects from idf
    def import_idf_constructions(workspace)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      clear_errors
      
      idd_object_types = ["Construction".to_IddObjectType,
                          "Construction_CfactorUndergroundWall".to_IddObjectType,
                          "Construction_FfactorGroundFloor".to_IddObjectType,
                          "Construction_InternalSource".to_IddObjectType,
                          "Construction_WindowDataFile".to_IddObjectType]
        
      workspace = import_objects_with_targets(workspace, idd_object_types)

      model, errors, warnings = Plugin.model_manager.model_from_workspace(workspace)
      
      objects = OpenStudio::IdfObjectVector.new
      model.objects(true).each {|object| objects << object.idfObject}

      if Plugin.read_pref('Show Errors on Idf Translation') and errors
        errors.each {|error| self.add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if Plugin.read_pref('Show Warnings on Idf Translation') and warnings
        warnings.each {|warning| self.add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
      
      @model_watcher.disable
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      remove_observers
      
      begin
        self.start_operation("Import Idf Constructions", true)
        new_objects = @openstudio_model.insertObjects(objects)
        new_objects.each {|new_object| on_new_model_object(new_object)}
      ensure
        self.commit_operation
      end
      
      add_observers
      
      @model_watcher.enable
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
      # show errors
      if (unviewed_errors)
        show_errors
      end
      
    end
    
    # import schedule objects from idf
    def import_idf_schedules(workspace)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      clear_errors
      
      idd_object_types = ["Schedule_Year".to_IddObjectType,
                          "Schedule_Compact".to_IddObjectType,
                          "Schedule_File".to_IddObjectType,
                          "Schedule_Constant".to_IddObjectType]
        
      workspace = import_objects_with_targets(workspace, idd_object_types)

      model, errors, warnings = Plugin.model_manager.model_from_workspace(workspace)
      
      objects = OpenStudio::IdfObjectVector.new
      model.objects(true).each {|object| objects << object.idfObject}
      
      if Plugin.read_pref('Show Errors on Idf Translation') and errors
        errors.each {|error| self.add_error("Error: #{error.logMessage}\n\n", false)}
      end

      if Plugin.read_pref('Show Warnings on Idf Translation') and warnings
        warnings.each {|warning| self.add_warning("Warning: #{warning.logMessage}\n\n", false)}
      end
              
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
        
      @model_watcher.disable
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      remove_observers

      begin
        self.start_operation("Import Idf Schedules", true)
        new_objects = @openstudio_model.insertObjects(objects)
        new_objects.each {|new_object| on_new_model_object(new_object)}
      ensure
        self.commit_operation
      end
      
      add_observers
      
      @model_watcher.enable
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
      # show errors
      if (unviewed_errors)
        show_errors
      end
      
    end

    
    # Create hash table of entities that have a model object handle.
    # (Meaning they were associated with a model object at one time.)
    # Maximum depth is two groups (e.g. for interior surface partition and space shading groups)
    def make_entity_hash
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      entity_hash = Hash.new
      for entity in @skp_model.entities
        
        # intentioanlly does not include shadow_info because that entity is shared between building and site
        
        # spaces and exterior shading groups
        if (entity.class == Sketchup::Group and entity.model_object_handle)
          entity_hash[entity.model_object_handle.to_s] = entity

          # Iterate over surfaces in the group.
          for child_entity in entity.entities
            if (child_entity.class == Sketchup::Face and child_entity.model_object_handle)
              # shading surface, surface, or subsurface
              entity_hash[child_entity.model_object_handle.to_s] = child_entity
              
            elsif (child_entity.class == Sketchup::ComponentInstance and child_entity.model_object_handle)
              # daylighting controls or illuminance map
              entity_hash[child_entity.model_object_handle.to_s] = child_entity
              
            elsif (child_entity.class == Sketchup::Group and child_entity.model_object_handle)
              # space shading or interior surface partition group
              entity_hash[child_entity.model_object_handle.to_s] = child_entity

              # Iterate over surfaces in the group.
              for grand_child_entity in child_entity.entities
                if (grand_child_entity.class == Sketchup::Face and grand_child_entity.model_object_handle)
                  # shading surface, or interior partition surface
                  entity_hash[grand_child_entity.model_object_handle.to_s] = grand_child_entity
                end
              end
            end
          end
        end
      end
      
      return(entity_hash)
    end
    
    # Resynch openstudio model with skp model
    # connect entities with model object if handles match
    # delete entity if it's model object handle references a missing model object
    # draw new entity if none exists for model object
    # This method does all of the drawing of new entities or updating of existing entities.
    def attach_openstudio_model()
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
        
      @model_watcher.disable
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      remove_observers(true)
      
      # store starting render mode
      starting_rendermode = materials_interface.rendering_mode
      
      # switch render mode to speed things up 
      materials_interface.rendering_mode = RenderWaiting
      
      begin

        # start a discrete operation
        self.start_operation("Attach OpenStudio Model", true)

        # progress dialog
        progress_dialog = ProgressDialog.new("Attaching OpenStudio Model")

        # get hash of model object handle to sketchup entity
        entity_hash = make_entity_hash

        # Clean drawing interface refereces.  The old object references are all invalid, but keep the model object handle.
        entity_hash.each_value {|value| value.drawing_interface = nil }

        # Loop through the input file and create an interface for each drawable input object.
        # Input objects that do not have a corresponding drawing interface are ignored.
        drawing_interfaces = Array.new
        model_objects = @openstudio_model.objects(true)
        for model_object in model_objects
          class_name = model_object.iddObject.name.upcase
          if (this_class = @class_hash[class_name])
            drawing_interface = this_class.new_from_handle(model_object.handle)
            drawing_interface.remove_observers(true)
            drawing_interfaces.push(drawing_interface)
          end
        end

        # Sort the drawing interfaces according to the "draw order".
        drawing_interfaces.sort! { |a, b| @draw_order.index(a.class) <=> @draw_order.index(b.class) }

        # Reattach drawing interfaces and entities.
        for drawing_interface in drawing_interfaces
          # Delete each pair from the hash as it is found for efficiency.
          entity = entity_hash.delete(drawing_interface.model_object.handle.to_s)
          if entity
            drawing_interface.attach_entity(entity)
          else
            #puts "no drawing_interface found for handle #{drawing_interface.model_object.handle.to_s} which is #{drawing_interface}"
          end
        end

        # Erase unattached entities.
        unattached_entities = entity_hash.values
        @skp_model.entities.erase_entities(unattached_entities)

        # Missing spaces named by base surfaces and spaces for other orphans could be added here.
        # That might be a lot cleaner than doing it inside the child interface.

        # Draw all the drawing interfaces.  New entities will be drawn.
        # Existing entities will be updated which may even erase and redraw.
        count = drawing_interfaces.length
        drawing_interfaces.each_with_index { |drawing_interface, i|
          progress_dialog.setValue((100*i)/count)
          drawing_interface.draw_entity
        }
        
        # check the model for errors
        check_openstudio_model

        # show north axis on open if north axis != 0
        building.update_entity
      
      ensure

        self.commit_operation
        progress_dialog.destroy
 
      end
      
      proc = Proc.new { 
        
        # switch render mode back to original
        materials_interface.rendering_mode = starting_rendermode 

        # After everything is drawn and finished changing, add the observer classes.
        @model_watcher.enable
        add_observers(true)
        
        if Plugin.dialog_manager
          Plugin.dialog_manager.update_all 
        end
        
        # show errors
        if (unviewed_errors)
          show_errors
        end
      }
      Plugin.add_event( proc )
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped

    end
    
    def on_new_model_object(new_model_object)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # see if this class is one we have an interface to
      class_name = new_model_object.iddObject.name.upcase
      
      Plugin.log(OpenStudio::Debug, "class_name = '#{class_name}'")
      Plugin.log(OpenStudio::Debug, "@class_hash.include?(class_name) = '#{@class_hash.include?(class_name)}'")
      
      if (@class_hash.include?(class_name))
      
        # pause event processing
        event_processing_stopped = Plugin.stop_event_processing
      
        this_class = @class_hash[class_name]
        Plugin.log(OpenStudio::Debug, "Adding new interface of type '#{this_class}'")
      
        # make a new interface
        drawing_interface = this_class.new_from_handle(new_model_object.handle)

        # Do not capture drawing of the new interface
        parent = drawing_interface.parent_from_model_object
        parent_had_observers = parent.remove_observers if parent

        parent_parent = nil
        parent_parent_had_observers = false
        if parent and parent.is_a?(Surface)
          parent_parent = parent.parent
          parent_parent_had_observers = parent_parent.remove_observers if parent_parent
        end

        drawing_interface.draw_entity
        
        # add observers back after everything is done drawing
        parent.add_observers if parent_had_observers
        parent_parent.add_observers if parent_parent_had_observers
        
        proc = Proc.new { drawing_interface.add_observers }
        Plugin.add_event( proc )
        
        # resume event processing
        Plugin.start_event_processing if event_processing_stopped
      end
      
      Plugin.log(OpenStudio::Trace, "leaving #{current_method_name}")
    end
    
    def detach_openstudio_model    
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing
        
      # This really speeds up the erase when deleting many entities at once
      destroy_observers(true)       
      destroy_model_watcher
      destroy_path_watcher

      begin

        # start a discrete operation
        self.start_operation("Detach OpenStudio Model", true)

        # get all top level entities
        entities = []
        self.spaces.each {|space| entities << space.entity if space.valid_entity?}
        self.shading_surface_groups.each {|shading_surface_group| entities << shading_surface_group.entity if shading_surface_group.valid_entity?}

        # close all open groups so we are at top level
        while @skp_model.close_active do end

        # delete top level entities
        @skp_model.entities.erase_entities(entities.to_a) if @skp_model.entities
        
        # get all materials
        materials = []
        self.rendering_colors.each {|rendering_color| materials << rendering_color.entity if rendering_color.valid_entity?}

        # delete materials
        #puts "materials before = #{@skp_model.materials}, #{@skp_model.materials.count}"
        if @skp_model.materials
          materials.each do |material| 
            #puts "removing #{material}"
            result = @skp_model.materials.remove(material) 
            #puts "  result = #{result}"
          end
        end
        #puts "materials after = #{@skp_model.materials}, #{@skp_model.materials.count}"

        # clean up children
        recurse_children.each do |interface| 
          interface.destroy_watcher 
          interface.destroy
        end
      
        # clear children
        @children.clear
        
        # clear interfaces
        @interfaces.clear

        # reset sql
        @openstudio_model.resetSqlFile if @openstudio_model

        # delete openstudio model
        @openstudio_model = nil
        
        # delete the temp dir
        OpenStudio::Openstudiolib::removeModelTempDir(@model_temp_dir)

        # erase openstudio path
        self.openstudio_path = nil
      
      ensure
      
        # commit the operation
        self.commit_operation
       
      end
      
      @materials_interface.destroy
      @selection_interface.destroy
      @results_interface.destroy
      
      # remove references to objects no longer needed
      @skp_model.model_interface = nil
      @skp_model = nil
      @model_observer = nil
      @entities_observer = nil
      @materials_interface = nil
      @selection_interface = nil
      @results_interface = nil
      @deleted_model_object_hash = nil
      
      # resume event processing
      Plugin.start_event_processing if event_processing_stopped
      
      if Plugin.dialog_manager
        Plugin.dialog_manager.update_all 
      end
      
      GC.start
      
    end    

    # Final check for any errors (generated by the plugin) in the drawing after opening a file.
    def check_openstudio_model
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # cleanup_entity may create new interfaces
      recurse_children.each { |child| child.cleanup_entity }
      
      # search for drawing interfaces with same entity
      # this can happen if two faces are exactly the same
      entity_to_drawing_interfaces = Hash.new
      entities_with_duplicate_interfaces = Set.new
      recurse_children.each do |child| 
        next if child.entity.nil?
        
        # other classes (like Building and Site do share the same entity)
        next unless child.entity.is_a?(Sketchup::Face)
        
        drawing_interfaces = entity_to_drawing_interfaces[child.entity]
        if drawing_interfaces.nil?
          # good, this is expected
          entity_to_drawing_interfaces[child.entity] = [child]
        else
          # a duplicate was found
          entity_to_drawing_interfaces[child.entity] << child
          entities_with_duplicate_interfaces.add(child.entity)
        end
      end
      
      entities_with_duplicate_interfaces.each do |entity|
      
        entity_to_drawing_interfaces[entity].each do |drawing_interface|
        
          # keep the interface that the entity is pointing to
          next if drawing_interface == entity.drawing_interface
        
          # duplicates code in DrawingInterface::draw_entity if check_model_object fails
          drawing_interface.destroy_observers
          drawing_interface.destroy_watcher
          
          drawing_interface.parent.remove_child(drawing_interface) if drawing_interface.parent 
          remove_drawing_interface(drawing_interface)
          drawing_interface.model_object.drawing_interface = nil
        
          #add_error("Found potential duplicate drawing interface for object #{drawing_interface.model_object.name}\n", false)
          add_error("Removed duplicate drawing interface for object #{drawing_interface.model_object.name}\n", false)
          entity_to_drawing_interfaces[entity].each do |drawing_interface2|
            if drawing_interface2 != drawing_interface
              add_error("  Potential duplicate of object #{drawing_interface2.model_object.name}\n", false)
            end
          end
          add_error("\n", false)
        end
      end  
    end

    # removes reference to drawing interface but does not remove model object handle
    def clean_openstudio_model
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      recurse_children.each { |interface| interface.clean_entity }
    end

    def add_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if recursive
        recurse_children.each { |interface| interface.add_observers(recursive) }
      end
      
      @materials_interface.add_observers(recursive)
      @selection_interface.add_observers(recursive)
      
      if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
        if not @observer_added
          @skp_model.add_observer(@model_observer)
          @skp_model.entities.add_observer(@entities_observer)
          @observer_added = true
        end
        @model_observer.enable
        @entities_observer.enable
      else
        @skp_model.add_observer(@model_observer)
        @skp_model.entities.add_observer(@entities_observer)
        @observer_added = true
        @model_observer.enable
        @entities_observer.enable
      end
    end

    def remove_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      had_observers = false
      if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
        if @observer_added
          had_observers = @model_observer.disable
          @entities_observer.disable
        end
      else
        had_observers = @skp_model.remove_observer(@model_observer)
        @skp_model.entities.remove_observer(@entities_observer) if @skp_model.entities
        @model_observer.disable
        @entities_observer.disable
        @observer_added = false
      end
      
      @materials_interface.remove_observers(recursive)
      @selection_interface.remove_observers(recursive)

      if recursive
        recurse_children.each { |interface| interface.remove_observers(recursive) }
      end
      
      return had_observers
    end
    
    def destroy_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      if @model_observer
        if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
          # actually do remove here
          @skp_model.remove_observer(@model_observer)
          @skp_model.entities.remove_observer(@entities_observer) if @skp_model.entities
          @model_observer.disable
          @entities_observer.disable
          @observer_added = false
        else
          @skp_model.remove_observer(@model_observer)
          @skp_model.entities.remove_observer(@entities_observer) if @skp_model.entities
          @model_observer.disable
          @entities_observer.disable
          @observer_added = false
        end
        
        @model_observer.destroy
        @model_observer = nil
        @entities_observer.destroy
        @entities_observer = nil
        result = true
      
        @materials_interface.destroy_observers(recursive)
        @selection_interface.destroy_observers(recursive)

        if recursive
          recurse_children.each { |interface| interface.destroy_observers(recursive) }
        end
      end
      
      return result
      
    end
    
    def add_model_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @model_watcher = PluginModelWatcher.new(self)
    end
    
    def destroy_model_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      if (@model_watcher)
        @model_watcher.disable
        @model_watcher = nil
        result = true
      end
      return result
    end
    
    def add_path_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      path = openstudio_path
      if path and OpenStudio::exists(OpenStudio::Path.new(path))
        @path_watcher = PluginPathWatcher.new(self, OpenStudio::Path.new(path))
      end
    end
    
    def destroy_path_watcher
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      if (@path_watcher)
        @path_watcher.disable
        @path_watcher = nil
        result = true
      end
      return result
    end
    
    def add_child(child)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @children.add(child)
      add_drawing_interface(child)
    end

    def remove_child(child)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @children.delete(child)
      remove_drawing_interface(child)
    end
    
    def add_drawing_interface(interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name} #{interface.class}")
      
      @interfaces[interface.class].add(interface)
    end
    
    def remove_drawing_interface(interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name} #{interface.class}")
      
      @interfaces[interface.class].delete(interface)
    end
    
    def recurse_children
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = @children.to_a
      @children.each { |interface| result.concat(interface.recurse_children) }
      return(result)
    end

    # used to check if this sketchup model has openstudio model objects or if it is a totally clean model
    def has_openstudio_model_objects?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      for entity in @skp_model.entities
        next if (entity.class != Sketchup::Group)
        return(true) if (entity.model_object_handle)
      end
      return(false)
    end
    
    def add_error(error_string, do_log = true)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      Plugin.log(OpenStudio::Error, "#{error_string}") if do_log
      @error_log += error_string
      @unviewed_errors = true
    end

    def add_warning(warning_string, do_log = true)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      Plugin.log(OpenStudio::Warn, "#{warning_string}") if do_log
      @error_log += warning_string
      @unviewed_errors = true
    end
    
    def show_errors
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if (@error_log.empty?)
        @error_log = "No errors or warnings."
      end
      UI.messagebox(@error_log, MB_MULTILINE, Plugin.name + ":  Input File Errors And Warnings")
      @unviewed_errors = false
    end
    
    def clear_errors
      @error_log = ""
      @unviewed_errors = false
    end

 if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OPERATIONS
     
    def start_operation(name, disable_gui=true)
      return false
    end   
    
    def abort_operation
      return false
    end
    
    def commit_operation
      return false
    end
    
 else # $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OPERATIONS
    
    def start_operation(name, disable_gui=true)
      result = false
      if not @operation_active
        result = @skp_model.start_operation(name, disable_gui)
        if result
          @operation_active = true
        end
      end
      return result
    end   
    
    def abort_operation
      result = false
      if @operation_active
        result = @skp_model.abort_operation
        if result 
          @operation_active = false
        end
      end
      return result
    end
    
    def commit_operation
      result = false
      if @operation_active
        result = @skp_model.commit_operation
        if result
          @operation_active = false
        end
      end
      return result
    end
    
 end # $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OPERATIONS
 
    def operation_active
      return @operation_active
    end
    
    def site
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[Site].to_a.first)
    end
    
    def building
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[Building].to_a.first)
    end

    def spaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[Space])
    end

    def shading_surface_groups
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[ShadingSurfaceGroup])
    end
    
    def interior_partition_surface_groups
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
        
      return(@interfaces[InteriorPartitionSurfaceGroup])
    end

    def surfaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[Surface])
    end

    def sub_surfaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[SubSurface])
    end

    def shading_surfaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[ShadingSurface])
    end
    
    def interior_partition_surfaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[InteriorPartitionSurface])
    end    

    def all_surfaces
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = surfaces.clone
      result.merge(sub_surfaces.clone)
      result.merge(shading_surfaces.clone)
      result.merge(interior_partition_surfaces.clone)
      return(result)
    end
    
    def illuminance_maps
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[IlluminanceMap])
    end

    def daylighting_controls
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[DaylightingControl])
    end
    
    def luminaires
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[Luminaire])
    end
    
    def glare_sensors
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[Glare_Sensor])
    end    
    
    def rendering_colors
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[RenderingColor])
    end
    
    def construction_bases
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[ConstructionBase])
    end
    
    def space_types
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[SpaceType])
    end
    
    def thermal_zones
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[ThermalZone])
    end
    
    def building_stories
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      return(@interfaces[BuildingStory])
    end
    
    def request_paint
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      if not @paint_requested
        @paint_requested = true
        
        # do not add the proc to call paint_now here, this is a special case in the event loop
      end
    end
    
    def paint_now
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      @paint_requested = false
      
      time = @skp_model.shadow_info.time

      # hash of information used for paint data, this is kind of kludgy but don't want to recompute time for each surface
      info = Hash.new
      info['date_time'] = OpenStudio::DateTime.new(OpenStudio::Date.new(OpenStudio::monthOfYear(time.month), time.day), OpenStudio::Time.new(0, time.hour, time.min))
      info['range_min'] = @results_interface.range_minimum.to_f
      info['range_max'] = @results_interface.range_maximum.to_f
      info['range_units'] = @results_interface.range_units
      info['rendering_appearance'] = @results_interface.rendering_appearance
      info['out_of_range_value'] = @results_interface.out_of_range_value
      info['normalize'] = @results_interface.normalize
      info['rendering_colors'] = self.rendering_colors
      
      # must remove observers outside of operation, if they are removed and then added inside operation they will fire
      remove_observers(true)
      
      begin
      
        operation = self.start_operation("Paint Model", true)

        # Suspicious that recursion is causing a major slow down here.
        # It seems to do recursion in a multithreaded way so that current thread completes but the rendering mode button does not update
        # until all the recursive threads are finished.
        for child in all_surfaces
          next if (not (child.respond_to?(:outside_variable) or child.respond_to?(:inside_variable)))
          child.paint_entity(info)
        end
        
      ensure
      
        self.commit_operation if operation
        
      end  
      
      add_observers(true)
      
    end


    def update_surface_variables
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      # clean everything
      all_surfaces.each do |child| 
        child.outside_variable = nil if child.respond_to?(:outside_variable)
        child.inside_variable = nil if child.respond_to?(:inside_variable)
      end
        
      sql_file = @openstudio_model.sqlFile
      return(nil) if sql_file.empty?

      sql_file = sql_file.get
      run_period = @results_interface.run_period
      outside_variable_name = @results_interface.outside_variable_name
      inside_variable_name = @results_interface.inside_variable_name      

      if (@results_interface.variable_type == "SURFACE")
        for child in all_surfaces
          if (child.respond_to?(:outside_variable))
            surface = child

            outside_variable = nil
            if outside_variable_name and not outside_variable_name.empty?
              outside_variable = sql_file.timeSeries(run_period, "Hourly", outside_variable_name, surface.name.upcase)
              if outside_variable.empty?
                outside_variable = nil
              else
                outside_variable = outside_variable.get
                outside_variable.setOutOfRangeValue(@results_interface.out_of_range_value)
              end
            end
            surface.outside_variable = outside_variable
            surface.outside_normalization = surface.model_object.netArea

            inside_variable = nil
            if inside_variable_name and not inside_variable_name.empty?
              inside_variable = sql_file.timeSeries(run_period, "Hourly", inside_variable_name, surface.name.upcase)
              if inside_variable.empty?
                inside_variable = nil     
              else
                inside_variable = inside_variable.get
                inside_variable.setOutOfRangeValue(@results_interface.out_of_range_value)            
              end
            end
            surface.inside_variable = inside_variable
            surface.inside_normalization = surface.model_object.netArea

          end
        end

      else  #(@results_interface.variable_type == "THERMALZONE")

        for space in self.spaces
          
          thermal_zone = space.model_object.thermalZone
          
          next if thermal_zone.empty?
          
          thermal_zone = thermal_zone.get
          
          outside_variable = sql_file.timeSeries(run_period, "Hourly", outside_variable_name, thermal_zone.name.to_s.upcase)
          if outside_variable.empty?
            outside_variable = nil? 
          else
            outside_variable = outside_variable.get
            outside_variable.setOutOfRangeValue(@results_interface.out_of_range_value)
          end
            
          inside_variable = sql_file.timeSeries(run_period, "Hourly", inside_variable_name, thermal_zone.name.to_s.upcase)
            if inside_variable.empty?
              inside_variable = nil? 
            else
              inside_variable = inside_variable.get
              inside_variable.setOutOfRangeValue(@results_interface.out_of_range_value)
            end
          
          for surface in space.children
            if (surface.respond_to?(:outside_variable))
              surface.outside_variable = outside_variable
              surface.inside_variable = inside_variable

              surface.outside_normalization = thermal_zone.floorArea
              surface.inside_normalization = thermal_zone.floorArea

              for sub_surface in surface.children
                sub_surface.outside_variable = outside_variable
                sub_surface.inside_variable = inside_variable

                sub_surface.outside_normalization = thermal_zone.floorArea
                sub_surface.inside_normalization = thermal_zone.floorArea
              end
            end
          end
        end
      end
    end
    
    def get_attribute(name)
      return @skp_model.get_attribute('OpenStudio', name)
    end
    
    def set_attribute(name, value)
      @skp_model.set_attribute('OpenStudio', name, value)
    end

  end

end





=begin
  # get the openstudio model out of the skp model and set it
  def load_openstudio_model_from_skp_model

    result = false

    # see if we have the right attribute dictionary
    create_if_empty = false
    attribute_dictionary = @skp_model.attribute_dictionary("OpenStudioModelObjects", create_if_empty)

    if attribute_dictionary

      # make a new model
      model = OpenStudio::Model::Model.new
      workspace = model.workspace

      # populate model with obejcts, add in bulk so references all exist
      idfObjects = OpenStudio::IdfObjectVector.new
      attribute_dictionary.each_pair do |old_handle, object_string|
        idfObjects << OpenStudio::IdfObject.load(object_string).get
      end

      # add objects
      new_objects = workspace.addObjects(idfObjects)
      new_handles = OpenStudio::getHandles(new_objects)

      # hash of old handles to new handles
      handle_hash = Hash.new

      # map old handles to new ones
      if not new_handles.empty?
        new_handles = new_handles.get
        old_handles = attribute_dictionary.keys
        if old_handles.size == new_handles.size
          old_handles.each_index {|i| handle_hash[old_handles[i]] = new_handles[i]}
        end
      end

      # get hash of model object handle to sketchup entity
      entity_hash = make_entity_hash

      # update handles in all existing sketchup entities
      entity_hash.each_pair do |old_handle, entity|
        if new_handle = handle_hash[old_handle]
          entity.model_object_handle = new_handle
          #puts "updated handle for #{entity} from #{old_handle} to #{new_handle}"
        else
          puts "failed to update handle for #{entity} with old handle #{old_handle}"
        end
      end

      # set openstudio model
      @openstudio_model = model

      result = true
    end

    return(result)
  end

  # clear the OpenStudioModelObjects attribute_dictionary
  def clear_openstudio_model_in_skp_model
    create_if_empty = false
    attribute_dictionary = @skp_model.attribute_dictionary("OpenStudioModelObjects", create_if_empty)

    if attribute_dictionary
      # delete current model
      attribute_dictionary.each_key {|handle| attribute_dictionary.delete_key(handle)}
    end
  end

  # save the openstudio model in the skp model
  def save_openstudio_model_in_skp_model
    # clear the OpenStudioModelObjects attribute_dictionary
    clear_openstudio_model_in_skp_model

    # get attribute dictionary
    create_if_empty = true
    attribute_dictionary = @skp_model.attribute_dictionary("OpenStudioModelObjects", create_if_empty)

    # write current model
    sorted = true
    @openstudio_model.workspace.objects(sorted).each do |object|
      attribute_dictionary[object.handle.to_s] = object.to_s
    end
  end
=end
