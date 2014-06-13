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

require("openstudio/sketchup_plugin/lib/IdfImporter")
require("openstudio/sketchup_plugin/lib/GbXMLImporter")
require("openstudio/sketchup_plugin/lib/SddImporter")
require("openstudio/sketchup_plugin/lib/OpenStudioImporter")
require("openstudio/sketchup_plugin/lib/dialogs/PluginInspectorDialog")
require("openstudio/sketchup_plugin/lib/dialogs/RunSimulationInterface")
require("openstudio/sketchup_plugin/lib/dialogs/RenderingSettingsInterface")
require("openstudio/sketchup_plugin/lib/dialogs/ColorScaleInterface")
require("openstudio/sketchup_plugin/lib/dialogs/AnimationSettingsInterface")
require("openstudio/sketchup_plugin/lib/dialogs/PreferencesInterface")
require("openstudio/sketchup_plugin/lib/dialogs/AboutInterface")
require("openstudio/sketchup_plugin/lib/dialogs/SurfaceSearchInterface")
require("openstudio/sketchup_plugin/lib/dialogs/SurfaceMatchingInterface")
require("openstudio/sketchup_plugin/lib/dialogs/LooseGeometryInterface")
require("openstudio/sketchup_plugin/lib/tools/InfoTool")
require("openstudio/sketchup_plugin/lib/tools/NewShadingSurfaceGroupTool")
require("openstudio/sketchup_plugin/lib/tools/NewInteriorPartitionSurfaceGroupTool")
require("openstudio/sketchup_plugin/lib/tools/NewDaylightingControlTool")
require("openstudio/sketchup_plugin/lib/tools/NewGlareSensorTool")
require("openstudio/sketchup_plugin/lib/tools/NewIlluminanceMapTool")
require("openstudio/sketchup_plugin/lib/tools/NewLuminaireTool")
require("openstudio/sketchup_plugin/lib/tools/NewSpaceTool")
require("openstudio/sketchup_plugin/lib/observers/ErrorObserver")  # This is hopefully only a temporary location

module OpenStudio

  class MenuManager
    
    attr_accessor :about_cmd, :prefs_cmd
    attr_accessor :online_help_cmd, :forum_cmd, :contact_cmd
    attr_accessor :new_cmd, :new_from_template_cmd, :open_cmd, :save_cmd, :save_as_cmd
    attr_accessor :import_openstudio_cmd, :import_constructions_cmd, :import_schedules_cmd, :import_space_types_cmd
    attr_accessor :import_idf_cmd, :import_idf_constructions_cmd, :import_idf_schedules_cmd, :import_gbxml_cmd, :import_sdd_cmd
    attr_accessor :export_openstudio_cmd, :export_untranslated_idf_cmd, :export_idf_cmd, :export_gbxml_cmd, :export_sdd_cmd
    attr_accessor :new_space_cmd, :new_shading_surface_group_cmd, :new_interior_partition_surface_cmd
    attr_accessor :new_daylighting_cmd, :new_illuminance_cmd, :new_luminaire_cmd, :new_glare_cmd
    attr_accessor :surface_matching_cmd, :loose_geometry_cmd
    attr_accessor :inspector_dialog_cmd, :surface_search_cmd, :info_tool_cmd, :show_errors_cmd
    attr_accessor :run_cmd, :resultsviewer_cmd, :openstudio_cmd
    attr_accessor :anim_settings_cmd, :rwd_to_start_cmd, :rwd_anim_cmd, :play_anim_cmd, :fwd_anim_cmd, :fwd_to_end_cmd
    attr_accessor :logging_cmd, :start_profile_cmd, :stop_profile_cmd, :mem_profile_cmd
    
    attr_accessor :plugin_menu, :file_menu, :import_menu, :export_menu, :new_item_menu, :modify_model_menu, :inspect_model_menu, :rendering_menu, :animation_menu, :help_menu, :developer_menu
    attr_accessor :command_toolbar, :rendering_toolbar, :animation_toolbar


    def initialize
      create_tools
      create_commands
      create_menus
      create_toolbars
      create_context_menu
    end


    def create_tools
      @new_space_tool = NewSpaceTool.new
      @new_shading_surface_group_tool = NewShadingSurfaceGroupTool.new
      @new_interior_partition_surface_tool = NewInteriorPartitionSurfaceGroupTool.new
      @new_daylighting_tool = NewDaylightingControlTool.new
      @new_illuminance_tool = NewIlluminanceMapTool.new
      @new_luminaire_tool = NewLuminaireTool.new
      @new_glare_tool = NewGlareSensorTool.new
      @info_tool = InfoTool.new
    end
    
    def refresh
      # needed to force SU to run the validation procs on Mac
      if @command_toolbar and @command_toolbar.visible?
        @command_toolbar.hide
        @command_toolbar.show 
      end
      
      if @rendering_toolbar and @rendering_toolbar.visible?
        @rendering_toolbar.hide
        @rendering_toolbar.show 
      end
      
      if @animation_toolbar and @animation_toolbar.visible?
        @animation_toolbar.hide 
        @animation_toolbar.show 
      end
    end
    
    def enable_if_model_interface
    
      result = MF_GRAYED
      if model_manager = Plugin.model_manager
        if model_manager.model_interface
          result = MF_ENABLED
        end
      end   
      
      result
    end


    def create_commands
    
      # Create all the commands (They must still be added to menus and toolbars next)
      @new_cmd = UI::Command.new("New OpenStudio Model") { Plugin.command_manager.new_openstudio }
      @new_cmd.small_icon = Plugin.dir + "/lib/resources/icons/OSNew-16.png"
      @new_cmd.large_icon = Plugin.dir + "/lib/resources/icons/OSNew-24.png"
      @new_cmd.tooltip = "New OpenStudio Model"
      @new_cmd.status_bar_text = "Create a new OpenStudio model"
      @new_cmd.set_validation_proc { 
        
        # kludge to run error checking for ruby script errors in the plugin, only need this in one validation proc
        detect_errors

        result = MF_GRAYED
        if model_manager = Plugin.model_manager
          if model_manager.model_interface
            result = MF_ENABLED
          end
        end   

        result
      }
        
      # Create all the commands (They must still be added to menus and toolbars next)
      @new_from_template_cmd = UI::Command.new("New OpenStudio Model From Template") { Plugin.command_manager.new_openstudio_from_template }
      @new_from_template_cmd.small_icon = Plugin.dir + "/lib/resources/icons/OSNew-16.png"
      @new_from_template_cmd.large_icon = Plugin.dir + "/lib/resources/icons/OSNew-24.png"
      @new_from_template_cmd.tooltip = "New OpenStudio Model From Template"
      @new_from_template_cmd.status_bar_text = "Create a new OpenStudio model from a template"
      @new_from_template_cmd.set_validation_proc { enable_if_model_interface }
        
      @open_cmd = UI::Command.new("Open OpenStudio Model") { Plugin.command_manager.open_openstudio }
      @open_cmd.small_icon = Plugin.dir + "/lib/resources/icons/OSOpen-16.png"
      @open_cmd.large_icon = Plugin.dir + "/lib/resources/icons/OSOpen-24.png"
      @open_cmd.tooltip = "Open OpenStudio Model"
      @open_cmd.status_bar_text = "Open an existing OpenStudio model"
      @open_cmd.set_validation_proc { enable_if_model_interface }

      @save_cmd = UI::Command.new("Save OpenStudio Model") { Plugin.command_manager.save_openstudio }
      @save_cmd.small_icon = Plugin.dir + "/lib/resources/icons/OSSave-16.png"
      @save_cmd.large_icon = Plugin.dir + "/lib/resources/icons/OSSave-24.png"
      @save_cmd.tooltip = "Save OpenStudio Model"
      @save_cmd.status_bar_text = "Save the OpenStudio model"
      @save_cmd.set_validation_proc { enable_if_model_interface }

      @save_as_cmd = UI::Command.new("Save OpenStudio Model As") { Plugin.command_manager.save_openstudio_as }
      @save_as_cmd.small_icon = Plugin.dir + "/lib/resources/icons/OSSaveAs-16.png"
      @save_as_cmd.large_icon = Plugin.dir + "/lib/resources/icons/OSSaveAs-24.png"
      @save_as_cmd.tooltip = "Save OpenStudio Model As"
      @save_as_cmd.status_bar_text = "Save the OpenStudio model as a new file"
      @save_as_cmd.set_validation_proc { enable_if_model_interface }
      
      @import_openstudio_cmd = UI::Command.new("Import OpenStudio Model") { Plugin.command_manager.import_openstudio }
      @import_openstudio_cmd.set_validation_proc { enable_if_model_interface }

      @import_constructions_cmd = UI::Command.new("Import Constructions") { Plugin.command_manager.import_constructions }
      @import_constructions_cmd.set_validation_proc { enable_if_model_interface }
      
      @import_schedules_cmd = UI::Command.new("Import Schedules") { Plugin.command_manager.import_schedules }
      @import_schedules_cmd.set_validation_proc { enable_if_model_interface }
      
      @import_space_types_cmd = UI::Command.new("Import Space Loads") { Plugin.command_manager.import_space_types }
      @import_space_types_cmd.set_validation_proc { enable_if_model_interface }
      
      @import_idf_cmd = UI::Command.new("Import EnergyPlus Idf") { Plugin.command_manager.import_idf }
      @import_idf_cmd.small_icon = Plugin.dir + "/lib/resources/icons/IDFImport-16.png"
      @import_idf_cmd.large_icon = Plugin.dir + "/lib/resources/icons/IDFImport-24.png"
      @import_idf_cmd.tooltip = "Import EnergyPlus Idf"
      @import_idf_cmd.status_bar_text = "Import an EnergyPlus Idf to OpenStudio"
      @import_idf_cmd.set_validation_proc { enable_if_model_interface }

      @import_idf_constructions_cmd = UI::Command.new("Import EnergyPlus Idf Constructions") { Plugin.command_manager.import_idf_constructions }
      @import_idf_constructions_cmd.set_validation_proc { enable_if_model_interface }

      @import_idf_schedules_cmd = UI::Command.new("Import EnergyPlus Idf Schedules") { Plugin.command_manager.import_idf_schedules }
      @import_idf_schedules_cmd.set_validation_proc { enable_if_model_interface }
      
      @import_gbxml_cmd = UI::Command.new("Import gbXML Model") { Plugin.command_manager.import_gbxml }
      @import_gbxml_cmd.set_validation_proc { enable_if_model_interface }
      
      @import_sdd_cmd = UI::Command.new("Import SDD Model") { Plugin.command_manager.import_sdd }
      @import_sdd_cmd.set_validation_proc { enable_if_model_interface }
      
      @export_openstudio_cmd = UI::Command.new("Export OpenStudio Model") { Plugin.command_manager.export_openstudio }
      @export_openstudio_cmd.set_validation_proc { enable_if_model_interface }
      
      @export_untranslated_idf_cmd = UI::Command.new("Export Untranslated EnergyPlus Idf") { Plugin.command_manager.export_untranslated_idf }
      @export_untranslated_idf_cmd.tooltip = "Export Untranslated EnergyPlus Idf"
      @export_untranslated_idf_cmd.status_bar_text = "Export EnergyPlus Idf Objects Not Translated to OpenStudio"
      @export_untranslated_idf_cmd.set_validation_proc { 
        result = MF_GRAYED
        if model_interface = Plugin.model_manager.model_interface
          if not model_interface.untranslated_idf_objects.empty?
            result = MF_ENABLED
          end
        end   

        result
       }
      
      @export_idf_cmd = UI::Command.new("Export EnergyPlus Idf") { Plugin.command_manager.export_idf }
      @export_idf_cmd.small_icon = Plugin.dir + "/lib/resources/icons/IDFExport-16.png"
      @export_idf_cmd.large_icon = Plugin.dir + "/lib/resources/icons/IDFExport-24.png"
      @export_idf_cmd.tooltip = "Export EnergyPlus Idf"
      @export_idf_cmd.status_bar_text = "Export an OpenStudio model as an EnergyPlus Idf"
      @export_idf_cmd.set_validation_proc { enable_if_model_interface }
            
      @export_gbxml_cmd = UI::Command.new("Export gbXML Model") { Plugin.command_manager.export_gbxml }
      @export_gbxml_cmd.set_validation_proc { enable_if_model_interface }
            
      @export_sdd_cmd = UI::Command.new("Export SDD Model") { Plugin.command_manager.export_sdd }
      @export_sdd_cmd.set_validation_proc { enable_if_model_interface }
      
      @new_space_cmd = UI::Command.new("New Space") { Sketchup.active_model.select_tool(@new_space_tool) }
      @new_space_cmd.small_icon = Plugin.dir + "/lib/resources/icons/new_space_16.png"
      @new_space_cmd.large_icon = Plugin.dir + "/lib/resources/icons/new_space.png"
      @new_space_cmd.tooltip = "New Space"
      @new_space_cmd.status_bar_text = "Create a new space"
      @new_space_cmd.set_validation_proc { enable_if_model_interface }

      @new_shading_surface_group_cmd = UI::Command.new("New Shading Surface Group") { Sketchup.active_model.select_tool(@new_shading_surface_group_tool)  }
      @new_shading_surface_group_cmd.small_icon = Plugin.dir + "/lib/resources/icons/NewShading-16.png"
      @new_shading_surface_group_cmd.large_icon = Plugin.dir + "/lib/resources/icons/NewShading-24.png"
      @new_shading_surface_group_cmd.tooltip = "New Shading Surface Group"
      @new_shading_surface_group_cmd.status_bar_text = "Create a new shading surface group"
      @new_shading_surface_group_cmd.set_validation_proc { enable_if_model_interface }

      @new_interior_partition_surface_cmd = UI::Command.new("New Interior Partition Surface Group") { Sketchup.active_model.select_tool(@new_interior_partition_surface_tool)  }
      @new_interior_partition_surface_cmd.small_icon = Plugin.dir + "/lib/resources/icons/NewPartition-16.png"
      @new_interior_partition_surface_cmd.large_icon = Plugin.dir + "/lib/resources/icons/NewPartition-24.png"
      @new_interior_partition_surface_cmd.tooltip = "New Interior Partition Surface Group"
      @new_interior_partition_surface_cmd.status_bar_text = "Create a new interior partition surface group"
      @new_interior_partition_surface_cmd.set_validation_proc { enable_if_model_interface }

      @new_daylighting_cmd = UI::Command.new("New Daylighting Control") { Sketchup.active_model.select_tool(@new_daylighting_tool)  }
      @new_daylighting_cmd.small_icon = Plugin.dir + "/lib/resources/icons/NewDaylighting-16.png"
      @new_daylighting_cmd.large_icon = Plugin.dir + "/lib/resources/icons/NewDaylighting-24.png"
      @new_daylighting_cmd.tooltip = "New Daylighting Control"
      @new_daylighting_cmd.status_bar_text = "Create a new daylighting control"
      @new_daylighting_cmd.set_validation_proc { enable_if_model_interface }

      @new_illuminance_cmd = UI::Command.new("New Illuminance Map") { Sketchup.active_model.select_tool(@new_illuminance_tool)  }
      @new_illuminance_cmd.small_icon = Plugin.dir + "/lib/resources/icons/NewIlluminance-16.png"
      @new_illuminance_cmd.large_icon = Plugin.dir + "/lib/resources/icons/NewIlluminance-24.png"
      @new_illuminance_cmd.tooltip = "New Illuminance Map"
      @new_illuminance_cmd.status_bar_text = "Create a new illuminance map"
      @new_illuminance_cmd.set_validation_proc { enable_if_model_interface }
      
      @new_luminaire_cmd = UI::Command.new("New Luminaire") { Sketchup.active_model.select_tool(@new_luminaire_tool)  }
      @new_luminaire_cmd.small_icon = Plugin.dir + "/lib/resources/icons/NewLuminaire-16.png"
      @new_luminaire_cmd.large_icon = Plugin.dir + "/lib/resources/icons/NewLuminaire-24.png"
      @new_luminaire_cmd.tooltip = "New Luminaire"
      @new_luminaire_cmd.status_bar_text = "Create a new luminaire"
      @new_luminaire_cmd.set_validation_proc { enable_if_model_interface }
      
      @new_glare_cmd = UI::Command.new("New Glare Sensor") { Sketchup.active_model.select_tool(@new_glare_tool)  }
      @new_glare_cmd.small_icon = Plugin.dir + "/lib/resources/icons/NewGlare-16.png"
      @new_glare_cmd.large_icon = Plugin.dir + "/lib/resources/icons/NewGlare-24.png"
      @new_glare_cmd.tooltip = "New Glare Sensor"
      @new_glare_cmd.status_bar_text = "Create a new glare sensor"
      @new_glare_cmd.set_validation_proc { enable_if_model_interface }
      
      @surface_matching_cmd = UI::Command.new("Surface Matching") { Plugin.dialog_manager.show(SurfaceMatchingInterface) }
      @surface_matching_cmd.small_icon = Plugin.dir + "/lib/resources/icons/SurfaceMatchingSelected-16.png"
      @surface_matching_cmd.large_icon = Plugin.dir + "/lib/resources/icons/SurfaceMatchingSelected-24.png"
      @surface_matching_cmd.tooltip = "Surface Matching"
      @surface_matching_cmd.status_bar_text = "Match surfaces of selected objects across Spaces"
      @surface_matching_cmd.set_validation_proc { Plugin.dialog_manager.validate(SurfaceMatchingInterface) if (Plugin.dialog_manager) } 

      @space_attributes_cmd = UI::Command.new("Set Attributes for Selected Spaces") { load Plugin.dir + '/lib/dialogs/SpaceAttributesInterface.rb' }
      @space_attributes_cmd.small_icon = Plugin.dir + "/lib/resources/icons/edit_spaces_16.png"
      @space_attributes_cmd.large_icon = Plugin.dir + "/lib/resources/icons/edit_spaces.png"
      @space_attributes_cmd.tooltip = "Set Attributes for Selected Spaces"
      @space_attributes_cmd.status_bar_text = "Set Attributes for Selected Spaces"
      @space_attributes_cmd.set_validation_proc { enable_if_model_interface }

      @space_diagram_cmd = UI::Command.new("Create Spaces From Diagram") { load Plugin.dir + '/lib/dialogs/SpaceDiagramInterface.rb' }
      @space_diagram_cmd.small_icon = Plugin.dir + "/lib/resources/icons/extrude_16.png"
      @space_diagram_cmd.large_icon = Plugin.dir + "/lib/resources/icons/extrude.png"
      @space_diagram_cmd.tooltip = "Create Spaces From Diagram"
      @space_diagram_cmd.status_bar_text = "Create Spaces From Diagram"
      @space_diagram_cmd.set_validation_proc { enable_if_model_interface }

      @loose_geometry_cmd = UI::Command.new("Project Loose Geometry") { Plugin.dialog_manager.show(LooseGeometryInterface) }
      @loose_geometry_cmd.small_icon = Plugin.dir + "/lib/resources/icons/windows_transparent-16.png"
      @loose_geometry_cmd.large_icon = Plugin.dir + "/lib/resources/icons/windows_transparent.png"
      @loose_geometry_cmd.tooltip = "Project Loose Geometry"
      @loose_geometry_cmd.status_bar_text = "Project loose geometry onto OpenStudio surfaces"
      @loose_geometry_cmd.set_validation_proc { Plugin.dialog_manager.validate(LooseGeometryInterface) if (Plugin.dialog_manager) }
      
      @space_loads_cmd = UI::Command.new("Space Loads") { } #Plugin.dialog_manager.show(SpaceLoadsInterface) }
      @space_loads_cmd.small_icon = Plugin.dir + "/lib/resources/icons/SpaceLoads-16.png"
      @space_loads_cmd.large_icon = Plugin.dir + "/lib/resources/icons/SpaceLoads-24.png"
      @space_loads_cmd.tooltip = "Space Loads"
      @space_loads_cmd.status_bar_text = "Add common loads to spaces"
      @space_loads_cmd.set_validation_proc { enable_if_model_interface }

      @inspector_dialog_cmd = UI::Command.new("Inspector") { 
        Plugin.dialog_manager.inspector_dialog.restoreState
        Plugin.dialog_manager.inspector_dialog.show 
      }
      @inspector_dialog_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Inspector-16.png"
      @inspector_dialog_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Inspector-24.png"
      @inspector_dialog_cmd.tooltip = "Inspector"
      @inspector_dialog_cmd.status_bar_text = "Display and edit the selected object"
      @inspector_dialog_cmd.set_validation_proc { enable_if_model_interface }

      @surface_search_cmd = UI::Command.new("Surface Search") { Plugin.dialog_manager.show(SurfaceSearchInterface) }
      @surface_search_cmd.small_icon = Plugin.dir + "/lib/resources/icons/SurfaceSearch-16.png"
      @surface_search_cmd.large_icon = Plugin.dir + "/lib/resources/icons/SurfaceSearch-24.png"
      @surface_search_cmd.tooltip = "Search Surfaces"
      @surface_search_cmd.status_bar_text = "Search surfaces in the model"
      @surface_search_cmd.set_validation_proc { Plugin.dialog_manager.validate(SurfaceSearchInterface) if (Plugin.dialog_manager) } 
      
      @info_tool_cmd = UI::Command.new("Info Tool") { Sketchup.active_model.tools.push_tool(@info_tool) }
      @info_tool_cmd.small_icon = Plugin.dir + "/lib/resources/icons/InfoTool-16.png"
      @info_tool_cmd.large_icon = Plugin.dir + "/lib/resources/icons/InfoTool-24.png"
      @info_tool_cmd.tooltip = "Info Tool"
      @info_tool_cmd.status_bar_text = "Show object data at a glance"
      @info_tool_cmd.set_validation_proc { enable_if_model_interface }
      
      @show_errors_cmd = UI::Command.new("Show Errors And Warnings") { Plugin.model_manager.model_interface.show_errors }
      @show_errors_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Warning-16.png"
      @show_errors_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Warning-24.png"
      @show_errors_cmd.tooltip = "Show Errors And Warnings"
      @show_errors_cmd.status_bar_text = "Show errors and warnings"
      @show_errors_cmd.set_validation_proc { enable_if_model_interface }
      
      @render_defaults_cmd = UI::Command.new("Render Default Values") {
        Plugin.model_manager.model_interface.materials_interface.render_defaults = (not Plugin.model_manager.model_interface.materials_interface.render_defaults) }
      #@render_defaults_cmd.small_icon = Plugin.dir + "/lib/resources/icons/tbRenderTransparentSmall.png"
      #@render_defaults_cmd.large_icon = Plugin.dir + "/lib/resources/icons/tbRenderTransparentLarge.png"
      @render_defaults_cmd.tooltip = "Render Default Values"
      @render_defaults_cmd.status_bar_text = "Render default values"
      @render_defaults_cmd.set_validation_proc {
        result = MF_GRAYED
        if model_manager = Plugin.model_manager
          if model_interface = model_manager.model_interface
            rendering_mode = model_interface.materials_interface.rendering_mode 
            if rendering_mode == RenderByConstruction
              if (model_interface.materials_interface.render_defaults)
                result = MF_CHECKED
              else
                result = MF_UNCHECKED
              end
            else
              result = MF_GRAYED
            end
          end
        end
        result
      }
      
      @render_by_class_cmd = UI::Command.new("Render By Surface Type") { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByClass }
      @render_by_class_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_surface_class_16.png"
      @render_by_class_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_surface_class.png"
      @render_by_class_cmd.tooltip = "Render By SurfaceType"
      @render_by_class_cmd.status_bar_text = "Render objects by surface type"
      @render_by_class_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByClass)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }

      @render_by_data_value_cmd = UI::Command.new("Render By Data Value") { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByDataValue }
      @render_by_data_value_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_data_16.png"
      @render_by_data_value_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_data.png"
      @render_by_data_value_cmd.tooltip = "Render By Data Value"
      @render_by_data_value_cmd.status_bar_text = "Render objects by data value"
      @render_by_data_value_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByDataValue)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }

      @render_by_boundary_cmd = UI::Command.new("Render By Boundary") { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByBoundary }
      @render_by_boundary_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_boundry_16.png"
      @render_by_boundary_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_boundry.png"
      @render_by_boundary_cmd.tooltip = "Render By Boundary Condition"
      @render_by_boundary_cmd.status_bar_text = "Render objects by boundary condition"
      @render_by_boundary_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByBoundary)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }

      @render_by_layer_cmd = UI::Command.new("Render By Layer")  { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByLayer }
      #@render_by_layer_cmd.small_icon = Plugin.dir + "/lib/resources/icons/DisplayColorByLayer-16x16.png"
      #@render_by_layer_cmd.large_icon = Plugin.dir + "/lib/resources/icons/DisplayColorByLayer-24x24.png"
      @render_by_layer_cmd.tooltip = "Render By Layer"
      @render_by_layer_cmd.status_bar_text = "Render objects by layer"
      @render_by_layer_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByLayer)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }
      
      @render_by_surface_normal_cmd = UI::Command.new("Render By Surface Normal")  { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderBySurfaceNormal }
      #@render_by_surface_normal_cmd.small_icon = Plugin.dir + "/lib/resources/icons/RenderMode5-16x16.png"
      #@render_by_surface_normal_cmd.large_icon = Plugin.dir + "/lib/resources/icons/RenderMode5-24x24.png"
      @render_by_surface_normal_cmd.tooltip = "Render By Surface Normal"
      @render_by_surface_normal_cmd.status_bar_text = "Render objects by surface normal"
      @render_by_surface_normal_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderBySurfaceNormal)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }
      
      @render_by_construction_cmd = UI::Command.new("Render By Construction")  { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByConstruction }
      @render_by_construction_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_construction_16.png"
      @render_by_construction_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_construction.png"
      @render_by_construction_cmd.tooltip = "Render By Construction"
      @render_by_construction_cmd.status_bar_text = "Render objects by construction"
      @render_by_construction_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByConstruction)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }
      
      @render_by_space_type_cmd = UI::Command.new("Render By Space Type")  { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderBySpaceType }
      @render_by_space_type_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_space_type_16.png"
      @render_by_space_type_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_space_type.png"
      @render_by_space_type_cmd.tooltip = "Render By Space Type"
      @render_by_space_type_cmd.status_bar_text = "Render objects by space type"
      @render_by_space_type_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderBySpaceType)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }
            
      @render_by_thermal_zone_cmd = UI::Command.new("Render By Thermal Zone")  { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByThermalZone }
      @render_by_thermal_zone_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_therm_zone_16.png"
      @render_by_thermal_zone_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_therm_zone.png"
      @render_by_thermal_zone_cmd.tooltip = "Render By Thermal Zone"
      @render_by_thermal_zone_cmd.status_bar_text = "Render objects by thermal zone"
      @render_by_thermal_zone_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByThermalZone)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }
      
      @render_by_building_story_cmd = UI::Command.new("Render By Building Story")  { Plugin.model_manager.model_interface.materials_interface.rendering_mode = RenderByBuildingStory }
      @render_by_building_story_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_story_16.png"
      @render_by_building_story_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_story.png"
      @render_by_building_story_cmd.tooltip = "Render By Building Story"
      @render_by_building_story_cmd.status_bar_text = "Render objects by building story"
      @render_by_building_story_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Plugin.model_manager and Plugin.model_manager.model_interface)
          if (Plugin.model_manager.model_interface.materials_interface.rendering_mode == RenderByBuildingStory)
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }
      
      @color_scale_cmd = UI::Command.new("Color Scale") { Plugin.dialog_manager.show(ColorScaleInterface) }
      @color_scale_cmd.small_icon = Plugin.dir + "/lib/resources/icons/render_color_scale_16.png"
      @color_scale_cmd.large_icon = Plugin.dir + "/lib/resources/icons/render_color_scale.png"
      @color_scale_cmd.tooltip = "Color Scale"
      @color_scale_cmd.status_bar_text = "Show color scale window"
      @color_scale_cmd.set_validation_proc { Plugin.dialog_manager.validate(ColorScaleInterface) if (Plugin.dialog_manager) }

      @run_cmd = UI::Command.new("Run Simulation") { Plugin.dialog_manager.show(RunSimulationInterface) }
      @run_cmd.small_icon = Plugin.dir + "/lib/resources/icons/RunManager-16.png"
      @run_cmd.large_icon = Plugin.dir + "/lib/resources/icons/RunManager-24.png"
      @run_cmd.tooltip = "Run Simulation"
      @run_cmd.status_bar_text = "Run simulation"
      @run_cmd.set_validation_proc { Plugin.dialog_manager.validate(RunSimulationInterface) if (Plugin.dialog_manager) }

      @resultsviewer_cmd = UI::Command.new("View Results") { Plugin.dialog_manager.show(RenderingSettingsInterface) }
      @resultsviewer_cmd.small_icon = Plugin.dir + "/lib/resources/icons/ResultsViewer-16.png"
      @resultsviewer_cmd.large_icon = Plugin.dir + "/lib/resources/icons/ResultsViewer-24.png"
      @resultsviewer_cmd.tooltip = "View Results"
      @resultsviewer_cmd.status_bar_text = "View simulation results"
      @resultsviewer_cmd.set_validation_proc { Plugin.dialog_manager.validate(RenderingSettingsInterface) if (Plugin.dialog_manager) }
      
      @openstudio_cmd = UI::Command.new("Launch Openstudio") do
        if Plugin.command_manager.save_openstudio
          openstudio_path = Plugin.model_manager.model_interface.openstudio_path
          if Plugin.platform == Platform_Windows
            # DLM: Used to use Thread.new here, suspect that was causing crashes when OpenStudio app was opened in plug-in
            # empty "" is required after start, first parameter is the name of the process to start
            # http://stackoverflow.com/questions/154075/using-the-dos-start-command-with-parameters-passed-to-the-started-program
            puts "start \"\" \"#{$OpenStudio_BinaryDir}OpenStudio\" \"#{openstudio_path}\""
            system("start \"\" \"#{$OpenStudio_BinaryDir}OpenStudio\" \"#{openstudio_path}\"")
          elsif Plugin.platform == Platform_Mac
            puts "open -a \"#{$OpenStudio_BinaryDir}OpenStudio.app\" --args \"#{openstudio_path}\""
            system("open -a \"#{$OpenStudio_BinaryDir}OpenStudio.app\" --args \"#{openstudio_path}\"")
          end
        end
      end
      @openstudio_cmd.small_icon = Plugin.dir + "/lib/resources/icons/os_16.png"
# replace this with 24pix icon
      @openstudio_cmd.large_icon = Plugin.dir + "/lib/resources/icons/os_24.png"
      @openstudio_cmd.tooltip = "Launch Openstudio"
      @openstudio_cmd.status_bar_text = "Launch Openstudio"
      @openstudio_cmd.set_validation_proc { enable_if_model_interface }
      
      @online_help_cmd = UI::Command.new("Online Help") { UI.openURL("http://openstudio.nrel.gov/google-sketchup-plug-getting-started")  }
      @online_help_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Help-16.png"
      @online_help_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Help-24.png"
      @online_help_cmd.tooltip = "Online OpenStudio Help"
      @online_help_cmd.status_bar_text = "View the Online OpenStudio Help"
      @online_help_cmd.set_validation_proc { MF_ENABLED } 
      
      @forum_cmd = UI::Command.new("Forum") { UI.openURL("http://openstudio.nrel.gov/forum") }
      @forum_cmd.set_validation_proc { MF_ENABLED } 

      @contact_cmd = UI::Command.new("Contact Us") { UI.openURL("http://openstudio.nrel.gov/webmaster") }
      @contact_cmd.set_validation_proc { MF_ENABLED }

      # Shortcuts for SketchUp Commands

#      @outliner_cmd = UI::Command.new("Outliner") { UI.show_inspector("Outliner") }
#      @outliner_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Outliner-16.png"
#      @outliner_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Outliner-24.png"
#      @outliner_cmd.tooltip = "Show Outliner Window"
#      @outliner_cmd.status_bar_text = "Show hierarchical outline of all SketchUp groups and components"
#      @outliner_cmd.set_validation_proc { MF_ENABLED } 

      @hide_rest_cmd = UI::Command.new("Hide Rest of Model") { 
        Sketchup.active_model.rendering_options["InactiveHidden"] = (not Sketchup.active_model.rendering_options["InactiveHidden"]) }
      @hide_rest_cmd.small_icon = Plugin.dir + "/lib/resources/icons/HideRest-16.png"
      @hide_rest_cmd.large_icon = Plugin.dir + "/lib/resources/icons/HideRest-24.png"
      @hide_rest_cmd.tooltip = "Hide Rest of Model"
      @hide_rest_cmd.status_bar_text = "Hide all inactive SketchUp groups and components"
      @hide_rest_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Sketchup.active_model) 
          if (Sketchup.active_model.rendering_options["InactiveHidden"])
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }

      @hidden_geom_cmd = UI::Command.new("View Hidden Geometry") {
        Sketchup.active_model.rendering_options["DrawHidden"] = (not Sketchup.active_model.rendering_options["DrawHidden"]) }
      @hidden_geom_cmd.small_icon = Plugin.dir + "/lib/resources/icons/SU_ViewHidden-16.png"
      @hidden_geom_cmd.large_icon = Plugin.dir + "/lib/resources/icons/SU_ViewHidden-24.png"
      @hidden_geom_cmd.tooltip = "View Hidden Geometry"
      @hidden_geom_cmd.status_bar_text = "View/hide hidden geometry"
      @hidden_geom_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Sketchup.active_model)  
          if (Sketchup.active_model.rendering_options["DrawHidden"])
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }

      @xray_cmd = UI::Command.new("X-ray Rendering") {
        Sketchup.active_model.rendering_options["ModelTransparency"] = (not Sketchup.active_model.rendering_options["ModelTransparency"]) }
      @xray_cmd.small_icon = Plugin.dir + "/lib/resources/icons/tbRenderTransparentSmall.png"
      @xray_cmd.large_icon = Plugin.dir + "/lib/resources/icons/tbRenderTransparentLarge.png"
      @xray_cmd.tooltip = "View Model in X-Ray Mode"
      @xray_cmd.status_bar_text = "Turn transparent x-ray mode on and off"
      @xray_cmd.set_validation_proc {
        result = MF_GRAYED
        if (Sketchup.active_model) 
          if (Sketchup.active_model.rendering_options["ModelTransparency"])
            result = MF_CHECKED
          else
            result = MF_UNCHECKED
          end
        end
        result
      }

      @shadow_settings_cmd = UI::Command.new("Shadow Settings") { UI.show_inspector("Shadows") }
      @shadow_settings_cmd.small_icon = Plugin.dir + "/lib/resources/icons/SUShadows-16x16.png"
      @shadow_settings_cmd.large_icon = Plugin.dir + "/lib/resources/icons/SUShadows-24x24.png"
      @shadow_settings_cmd.tooltip = "Show Shadow Settings Dialog"
      @shadow_settings_cmd.status_bar_text = "Show time of day and day of year for shadow settings"
      @shadow_settings_cmd.set_validation_proc { MF_ENABLED } 

      # Animation commands

      @anim_settings_cmd = UI::Command.new("Settings") { Plugin.dialog_manager.show(AnimationSettingsInterface) }
      @anim_settings_cmd.small_icon = Plugin.dir + "/lib/resources/icons/AnimationSettings~16.png"
      @anim_settings_cmd.large_icon = Plugin.dir + "/lib/resources/icons/AnimationSettings~24.png"
      @anim_settings_cmd.tooltip = "Animation Settings"
      @anim_settings_cmd.status_bar_text = "Show animation settings"
      @anim_settings_cmd.set_validation_proc { Plugin.dialog_manager.validate(AnimationSettingsInterface) if (Plugin.dialog_manager) }

      @rwd_to_start_cmd = UI::Command.new("Reverse To Marker") { Plugin.animation_manager.reverse_to_marker }
      @rwd_to_start_cmd.small_icon = Plugin.dir + "/lib/resources/icons/RewindFull16.png"
      @rwd_to_start_cmd.large_icon = Plugin.dir + "/lib/resources/icons/RewindFull24.png"
      @rwd_to_start_cmd.tooltip = "Reverse To Marker"
      @rwd_to_start_cmd.status_bar_text = "Reverse animation to previous marker"
      @rwd_to_start_cmd.set_validation_proc { Plugin.animation_manager.validate_reverse if (Plugin.animation_manager) }

      @rwd_anim_cmd = UI::Command.new("Reverse") { Plugin.animation_manager.reverse }
      @rwd_anim_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Rewind16.png"
      @rwd_anim_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Rewind24.png"
      @rwd_anim_cmd.tooltip = "Reverse Frame"
      @rwd_anim_cmd.status_bar_text = "Reverse animation by one frame"
      @rwd_anim_cmd.set_validation_proc { Plugin.animation_manager.validate_reverse if (Plugin.animation_manager) }

      @play_anim_cmd = UI::Command.new("Play") { Plugin.animation_manager.play }
      @play_anim_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Play16.png"
      @play_anim_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Play24.png"
      @play_anim_cmd.tooltip = "Play"
      @play_anim_cmd.status_bar_text = "Play animation"
      @play_anim_cmd.set_validation_proc { Plugin.animation_manager.validate_play_animation if (Plugin.animation_manager) }

      @fwd_anim_cmd = UI::Command.new("Forward") { Plugin.animation_manager.forward }
      @fwd_anim_cmd.small_icon = Plugin.dir + "/lib/resources/icons/Forward16.png"
      @fwd_anim_cmd.large_icon = Plugin.dir + "/lib/resources/icons/Forward24.png"
      @fwd_anim_cmd.tooltip = "Forward Frame"
      @fwd_anim_cmd.status_bar_text = "Forward animation by one frame"
      @fwd_anim_cmd.set_validation_proc { Plugin.animation_manager.validate_forward if (Plugin.animation_manager) }

      @fwd_to_end_cmd = UI::Command.new("Forward To Marker") { Plugin.animation_manager.forward_to_marker }
      @fwd_to_end_cmd.small_icon = Plugin.dir + "/lib/resources/icons/ForwardFull16.png"
      @fwd_to_end_cmd.large_icon = Plugin.dir + "/lib/resources/icons/ForwardFull24.png"
      @fwd_to_end_cmd.tooltip = "Forward To Marker"
      @fwd_to_end_cmd.status_bar_text = "Forward animation to next marker"
      @fwd_to_end_cmd.set_validation_proc { Plugin.animation_manager.validate_forward if (Plugin.animation_manager) }

      # Commands that don't go on the toolbar
  
      @prefs_cmd = UI::Command.new("Preferences") { Plugin.dialog_manager.show(PreferencesInterface) }
      @prefs_cmd.set_validation_proc { MF_ENABLED }
      
      @update_cmd = UI::Command.new("Check For Update") { Plugin.update_manager = PluginUpdateManager.new("SketchUp Plug-in", true) }
      @update_cmd.set_validation_proc { 
        if Plugin.update_manager.nil?
          MF_ENABLED
        else
          MF_GRAYED
        end
      }

      @about_cmd = UI::Command.new("About OpenStudio") { Plugin.dialog_manager.show(AboutInterface) }
      @about_cmd.set_validation_proc { MF_ENABLED } 
      
      @start_profile_cmd = UI::Command.new("Start Profile") { Plugin.start_profile }
      @start_profile_cmd.set_validation_proc { 
        if Plugin.profile_running 
          MF_GRAYED 
        else
          MF_ENABLED
        end
      }
      
      @stop_profile_cmd = UI::Command.new("Stop Profile") { Plugin.stop_profile }
      @stop_profile_cmd.set_validation_proc { 
        if Plugin.profile_running 
          MF_ENABLED 
        else
          MF_GRAYED
        end
      }
      
      @mem_profile_cmd = UI::Command.new("Memory Profile") { Plugin.mem_profile }
      @mem_profile_cmd.set_validation_proc { 
          MF_ENABLED 
      } 
      
      @logging_cmd = UI::Command.new("Logging") { 
        if Plugin.logging_enabled
          Plugin.disable_logging
        else
          Plugin.enable_logging
        end      
      }
      @logging_cmd.set_validation_proc { 
        if Plugin.logging_enabled 
          MF_CHECKED
        else
          MF_ENABLED
        end
      }
      
      @start_new_dialog_cmd = UI::Command.new("Start New Dialog") { OpenStudio::Openstudiolib::OSDocument.new(OpenStudio::Model::OptionalModel.new(OpenStudio::Plugin.model_manager.model_interface.openstudio_model)) }
      @start_new_dialog_cmd.set_validation_proc { MF_ENABLED } 
    end
    

    def create_menus

      # Add the plugin menu
      @plugin_menu = UI.menu("Plugins").add_submenu(OpenStudio::Plugin.name)

      id = @plugin_menu.add_item(@about_cmd)
      @plugin_menu.set_validation_proc(id) { MF_ENABLED }
      
      id = @plugin_menu.add_item(@prefs_cmd)
      @plugin_menu.set_validation_proc(id) { MF_ENABLED }

      # Add the Help menu
      @help_menu = @plugin_menu.add_submenu("Help")
      
      id = @help_menu.add_item(@online_help_cmd)
      @help_menu.set_validation_proc(id) { MF_ENABLED }
      
      id = @help_menu.add_item(@forum_cmd)
      @help_menu.set_validation_proc(id) { MF_ENABLED }
      
      id = @help_menu.add_item(@contact_cmd)
      @help_menu.set_validation_proc(id) { MF_ENABLED }
      
      id = @plugin_menu.add_item(@update_cmd)
      @plugin_menu.set_validation_proc(id) { MF_ENABLED }
      
      @plugin_menu.add_separator
      @plugin_menu.add_item(@openstudio_cmd)
      # turned off run in GUI but left code behind
      # @plugin_menu.add_item(@run_cmd)
      # @plugin_menu.add_item(@resultsviewer_cmd)
      
      @plugin_menu.add_separator
      
      # Add the file menu
      @file_menu = @plugin_menu.add_submenu("File")
      @file_menu.add_item(@new_cmd)
      @file_menu.add_item(@new_from_template_cmd)
      @file_menu.add_item(@open_cmd)
      @file_menu.add_item(@save_cmd)
      @file_menu.add_item(@save_as_cmd)
      
      # Add the Import menu
      @import_menu = @plugin_menu.add_submenu("Import")
      @import_menu.add_item(@import_openstudio_cmd)
      @import_menu.add_item(@import_constructions_cmd)
      @import_menu.add_item(@import_schedules_cmd)
      @import_menu.add_item(@import_space_types_cmd)      
      @import_menu.add_separator
      @import_menu.add_item(@import_idf_cmd)
      @import_menu.add_item(@import_idf_constructions_cmd)
      @import_menu.add_item(@import_idf_schedules_cmd)
      @import_menu.add_separator
      @import_menu.add_item(@import_gbxml_cmd)
      @import_menu.add_separator
      @import_menu.add_item(@import_sdd_cmd)
      
      # Add the Export menu
      @export_menu = @plugin_menu.add_submenu("Export")
      @export_menu.add_item(@export_openstudio_cmd)
      @export_menu.add_separator
      @export_menu.add_item(@export_untranslated_idf_cmd)
      @export_menu.add_item(@export_idf_cmd)
      @export_menu.add_separator
      @export_menu.add_item(@export_gbxml_cmd)
      @export_menu.add_separator
      @export_menu.add_item(@export_sdd_cmd)
      
      @plugin_menu.add_separator
      
      # Add the make new item menu
      @new_item_menu = @plugin_menu.add_submenu("Make New Object")
      @new_item_menu.add_item(@new_space_cmd)
      @new_item_menu.add_item(@new_shading_surface_group_cmd)
      @new_item_menu.add_item(@new_interior_partition_surface_cmd)
      @new_item_menu.add_item(@new_daylighting_cmd)
      @new_item_menu.add_item(@new_illuminance_cmd)
      @new_item_menu.add_item(@new_luminaire_cmd)
      @new_item_menu.add_item(@new_glare_cmd)
      
      # Add the modify model menu
      @modify_model_menu = @plugin_menu.add_submenu("Modify Model")
      @modify_model_menu.add_item(@surface_matching_cmd)
      @modify_model_menu.add_item(@space_attributes_cmd)
      @modify_model_menu.add_item(@space_diagram_cmd)
      @modify_model_menu.add_item(@loose_geometry_cmd)
      
      # Add the inspect model menu
      @inspect_model_menu = @plugin_menu.add_submenu("Inspect Model")
      @inspect_model_menu.add_item(@inspector_dialog_cmd)
      @inspect_model_menu.add_item(@surface_search_cmd) 
      @inspect_model_menu.add_item(@info_tool_cmd)
      @inspect_model_menu.add_item(@show_errors_cmd)

      # Add the Rendering menu
      @rendering_menu = @plugin_menu.add_submenu("Rendering")
      #@rendering_menu.add_item(@render_defaults_cmd)
      @rendering_menu.add_item(@render_by_class_cmd)
      @rendering_menu.add_item(@render_by_boundary_cmd)
      @rendering_menu.add_item(@render_by_construction_cmd)
      @rendering_menu.add_item(@render_by_space_type_cmd)
      @rendering_menu.add_item(@render_by_thermal_zone_cmd)
      @rendering_menu.add_item(@render_by_building_story_cmd)
      #@rendering_menu.add_item(@render_by_data_value_cmd)
      @rendering_menu.add_item(@render_by_layer_cmd)
      @rendering_menu.add_item(@render_by_surface_normal_cmd)
      #@rendering_menu.add_item(@color_scale_cmd)
      
      #@plugin_menu.add_separator
      
      # Add the Animation menu
      #@animation_menu = @plugin_menu.add_submenu("Animation")
      #@animation_menu.add_item(@rwd_to_start_cmd)
      #@animation_menu.add_item(@rwd_anim_cmd)
      #@animation_menu.add_item(@play_anim_cmd)
      #@animation_menu.add_item(@fwd_anim_cmd)
      #@animation_menu.add_item(@fwd_to_end_cmd)
      #@animation_menu.add_separator
      #@animation_menu.add_item(@anim_settings_cmd)
      
      @plugin_menu.add_separator

      # Add the SketchUp Tools Menu
      @sketchup_menu = @plugin_menu.add_submenu("SketchUp Tools")
      @sketchup_menu.add_item(@hide_rest_cmd)
      @sketchup_menu.add_item(@hidden_geom_cmd)
      @sketchup_menu.add_item(@xray_cmd)
      # @sketchup_menu.add_item(@outliner_cmd)
      @sketchup_menu.add_item(@shadow_settings_cmd)

      if not Plugin.read_pref("Disable OpenStudio User Scripts")

        # Loop through User Scripts to create commands and menus
        @user_scripts_menu = UI.menu("Plugins").add_submenu(Plugin.name + " User Scripts")
        
        # command to reload user scripts
        load_user_script_cmd = UI::Command.new("Load User Scripts")  { Plugin.user_script_runner.load_user_scripts }
        load_user_script_cmd.set_validation_proc { enable_if_model_interface }
        @user_scripts_menu.add_item(load_user_script_cmd)
        @user_scripts_menu.add_separator
        
        @user_script_cmds = Hash.new
        
        # add menu items for sub directories
           
      end
      
 
      if $OPENSTUDIO_SKETCHUPPLUGIN_DEVELOPER_MENU
        # add developer menu
        @developer_menu = UI.menu("Plugins").add_submenu(Plugin.name + " Developer")
        #@developer_menu.set_validation_proc { MF_ENABLED }
        @developer_menu.add_item(@start_profile_cmd)
        @developer_menu.add_item(@stop_profile_cmd)
        @developer_menu.add_item(@mem_profile_cmd)
        @developer_menu.add_item(@logging_cmd)
        @developer_menu.add_item(@start_new_dialog_cmd)
      end
      
    end
    
    def user_scripts_menu
      return @user_scripts_menu
    end
    
    #def disable_user_scripts
    #  # can't remove them, just disable
    #  @user_script_cmds.each_value do |user_script_cmd|
    #    user_script_cmd.set_validation_proc { MF_GRAYED }
    #  end
    #end
    
    def add_user_script(name, menu)
      if not Plugin.read_pref("Disable OpenStudio User Scripts")        

        user_script_cmd = @user_script_cmds[name]
        
        if user_script_cmd
          user_script_cmd.set_validation_proc { enable_if_model_interface }
        else
          user_script_cmd = UI::Command.new(name)  { Plugin.user_script_runner.run_user_script(name) }
          user_script_cmd.set_validation_proc { enable_if_model_interface }
          menu.add_item(user_script_cmd)
          @user_script_cmds[name] = user_script_cmd
        end
      end
    end
    
    
    def create_toolbars

      # Add the OpenStudio command toolbar

      @command_toolbar = UI::Toolbar.new("OpenStudio Tools")
      @command_toolbar.add_item(@new_from_template_cmd)
      @command_toolbar.add_item(@open_cmd)
      @command_toolbar.add_item(@save_cmd)
      @command_toolbar.add_item(@save_as_cmd)
      @command_toolbar.add_item(@import_idf_cmd)
      @command_toolbar.add_item(@export_idf_cmd)
      @command_toolbar.add_separator
      @command_toolbar.add_item(@new_space_cmd)
      @command_toolbar.add_item(@new_shading_surface_group_cmd)
      @command_toolbar.add_item(@new_interior_partition_surface_cmd)
      @command_toolbar.add_item(@new_daylighting_cmd)
      @command_toolbar.add_item(@new_illuminance_cmd)
      @command_toolbar.add_item(@new_luminaire_cmd)
      @command_toolbar.add_item(@new_glare_cmd)
      @command_toolbar.add_item(@surface_matching_cmd)
      @command_toolbar.add_item(@space_attributes_cmd)
      @command_toolbar.add_item(@space_diagram_cmd)
      @command_toolbar.add_item(@loose_geometry_cmd)
      @command_toolbar.add_separator
      @command_toolbar.add_item(@inspector_dialog_cmd)
      @command_toolbar.add_item(@surface_search_cmd)
      @command_toolbar.add_item(@info_tool_cmd)
      @command_toolbar.add_item(@show_errors_cmd)
      @command_toolbar.add_separator
      # turned off run in GUI but left code behind
      # @command_toolbar.add_item(@run_cmd)
      # @command_toolbar.add_item(@resultsviewer_cmd)
      @command_toolbar.add_item(@openstudio_cmd)
      @command_toolbar.add_item(@online_help_cmd)
      @command_toolbar.restore

      # Shortcuts to SketchUp commands

      @rendering_toolbar = UI::Toolbar.new("OpenStudio Rendering Toolbars")
      @rendering_toolbar.add_item(@render_by_class_cmd)
      @rendering_toolbar.add_item(@render_by_boundary_cmd)
      @rendering_toolbar.add_item(@render_by_construction_cmd)
      @rendering_toolbar.add_item(@render_by_space_type_cmd)
      @rendering_toolbar.add_item(@render_by_thermal_zone_cmd)
      @rendering_toolbar.add_item(@render_by_building_story_cmd)
      #@rendering_toolbar.add_item(@render_by_data_value_cmd)
      #@rendering_toolbar.add_item(@color_scale_cmd)
      @rendering_toolbar.add_separator
      @rendering_toolbar.add_item(@hide_rest_cmd)
      @rendering_toolbar.add_item(@hidden_geom_cmd)
      @rendering_toolbar.add_item(@xray_cmd)
      # @rendering_toolbar.add_item(@outliner_cmd)
      @rendering_toolbar.add_item(@shadow_settings_cmd)
      @rendering_toolbar.restore

      # Add the EnergyPlus Animation toolbar

      #@animation_toolbar = UI::Toolbar.new("OpenStudio Animation")
      #@animation_toolbar.add_item(@anim_settings_cmd)
      #@animation_toolbar.add_item(@rwd_to_start_cmd)
      #@animation_toolbar.add_item(@rwd_anim_cmd)
      #@animation_toolbar.add_item(@play_anim_cmd)
      #@animation_toolbar.add_item(@fwd_anim_cmd)
      #@animation_toolbar.add_item(@fwd_to_end_cmd)
      # I don't want to show this by default
      #@animation_toolbar.restore

    end
    
    
    def create_context_menu
      # This method could be cleaned up a bit

      floor_type_cmd = UI::Command.new('Floor') { change_surface_type_to('FLOOR') }
      floor_type_cmd.set_validation_proc { validate_surface_type('FLOOR') }
      
      wall_type_cmd = UI::Command.new('Wall') { change_surface_type_to('WALL') }
      wall_type_cmd.set_validation_proc { validate_surface_type('WALL') }
      
      roofceiling_type_cmd = UI::Command.new('RoofCeiling') { change_surface_type_to('ROOFCEILING') }
      roofceiling_type_cmd.set_validation_proc { validate_surface_type('ROOFCEILING') }

      fixed_window_type_cmd = UI::Command.new('FixedWindow') { change_subsurface_type_to('FIXEDWINDOW') }
      fixed_window_type_cmd.set_validation_proc { validate_subsurface_type('FIXEDWINDOW') }
      
      operable_window_type_cmd = UI::Command.new('OperableWindow') { change_subsurface_type_to('OPERABLEWINDOW') }
      operable_window_type_cmd.set_validation_proc { validate_subsurface_type('OPERABLEWINDOW') }
      
      door_type_cmd = UI::Command.new('Door') { change_subsurface_type_to('DOOR') }
      door_type_cmd.set_validation_proc { validate_subsurface_type('DOOR') }
      
      overhead_door_type_cmd = UI::Command.new('OverheadDoor') { change_subsurface_type_to('OVERHEADDOOR') }
      overhead_door_type_cmd.set_validation_proc { validate_subsurface_type('OVERHEADDOOR') }
      
      glassdoor_type_cmd = UI::Command.new('GlassDoor') { change_subsurface_type_to('GLASSDOOR') }
      glassdoor_type_cmd.set_validation_proc { validate_subsurface_type('GLASSDOOR') }
      
      skylight_type_cmd = UI::Command.new('Skylight') { change_subsurface_type_to('SKYLIGHT') }
      skylight_type_cmd.set_validation_proc { validate_subsurface_type('SKYLIGHT') }
      
      tdd_dome_type_cmd = UI::Command.new('TubularDaylightDome') { change_subsurface_type_to('TubularDaylightDome') }
      tdd_dome_type_cmd.set_validation_proc { validate_subsurface_type('TubularDaylightDome') }
      
      tdd_diffuser_type_cmd = UI::Command.new('TubularDaylightDiffuser') { change_subsurface_type_to('TDD:DIFFUSER') }
      tdd_diffuser_type_cmd.set_validation_proc { validate_subsurface_type('TDD:DIFFUSER') }

      # Add the OpenStudio context menu handler

      UI.add_context_menu_handler do |menu|

        if (drawing_interface = Plugin.model_manager.model_interface.selection_interface.selected_drawing_interface)
          menu.add_separator
          plugin_menu = menu.add_submenu(Plugin.name)
          plugin_menu.add_item(inspector_dialog_cmd)

          case (drawing_interface.class.to_s)
          when 'OpenStudio::Building'
          when 'OpenStudio::Surface'
            plugin_menu.add_separator
            type_menu = plugin_menu.add_submenu("Surface Type")
            type_menu.add_item(floor_type_cmd)
            type_menu.add_item(wall_type_cmd)
            type_menu.add_item(roofceiling_type_cmd)
          when 'OpenStudio::SubSurface'
            plugin_menu.add_separator
            type_menu = plugin_menu.add_submenu("SubSurface Type")
            type_menu.add_item(fixed_window_type_cmd)
            type_menu.add_item(operable_window_type_cmd)
            type_menu.add_item(door_type_cmd)
            type_menu.add_item(overhead_door_type_cmd)
            type_menu.add_item(glassdoor_type_cmd)
            type_menu.add_item(skylight_type_cmd)
            type_menu.add_item(tdd_dome_type_cmd)
            type_menu.add_item(tdd_diffuser_type_cmd)
          when 'OpenStudio::Space'
            #plugin_menu.add_separator
          when 'OpenStudio::ShadingSurfaceGroup'
            #plugin_menu.add_separator
          end

        end
      end

    end

    def change_surface_type_to(new_type)
      drawing_interface = Plugin.model_manager.model_interface.selection_interface.selected_drawing_interface
      drawing_interface.model_object.setSurfaceType(new_type)
      drawing_interface.paint_entity
    end

    def validate_surface_type(this_type)
      drawing_interface = Plugin.model_manager.model_interface.selection_interface.selected_drawing_interface
      if (drawing_interface.model_object.surfaceType.upcase == this_type.upcase)
        return(MF_CHECKED)
      else
        return(MF_UNCHECKED)
      end
    end
    
    def change_subsurface_type_to(new_type)
      drawing_interface = Plugin.model_manager.model_interface.selection_interface.selected_drawing_interface
      drawing_interface.model_object.setSubSurfaceType(new_type)
      drawing_interface.paint_entity
    end
    
    def validate_subsurface_type(this_type)
      drawing_interface = Plugin.model_manager.model_interface.selection_interface.selected_drawing_interface
      if (drawing_interface.model_object.subSurfaceType == this_type.upcase)
        return(MF_CHECKED)
      else
        return(MF_UNCHECKED)
      end
    end
    
    # some validation procs
    
    def validate_class(this_class)
      drawing_interface = Plugin.model_manager.model_interface.selection_interface.selected_drawing_interface
      if (drawing_interface.input_object.is_class_name?(this_class))
        return(MF_CHECKED)
      else
        return(MF_UNCHECKED)
      end
    end

  end
  
end
