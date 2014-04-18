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


require("openstudio/sketchup_plugin/lib/dialogs/PreferencesDialog")

module OpenStudio

  class CommandManager

    def initialize
      # This is an unsual class; it is all methods, no data.
      # Should this just be a Mixin?  or module?
    end

    def new_openstudio
      if (prompt_to_continue_open)
        Plugin.model_manager.new_from_template(Sketchup.active_model)
      end
    end
    
    def new_openstudio_from_template
      if (prompt_to_continue_open)
        default_template_dir = Plugin.default_template_dir
        if (path = UI.open_panel("New OpenStudio Model From Template", default_template_dir, "*.osm"))  # bug in SU7 prevents file filters from working

          success = Plugin.model_manager.open_openstudio(path, Sketchup.active_model, false, false)
          
          if success and (not Plugin.default_template_path or Plugin.default_template_path.empty?)
            result = UI.messagebox("Would you like to set this file as your default OpenStudio template model?", MB_YESNO)

            if result == 6 # YES
              Plugin.write_pref("Default Template Path", path)
            end 

          end
        end
      end
    end
    
    def open_openstudio
      result = false
      if (prompt_to_continue_open)

        openstudio_dir = Plugin.read_pref("Last OpenStudio Dir")
        if Plugin.model_manager.model_interface
          openstudio_dir = Plugin.model_manager.model_interface.openstudio_dir
        end

        if (path = UI.open_panel("Open OpenStudio Model File", openstudio_dir, "*.osm"))  # bug in SU7 prevents file filters from working
          
          success = Plugin.model_manager.open_openstudio(path, Sketchup.active_model)
          
          if success
            # Save the dir so we can start here next time
            Plugin.write_pref("Last OpenStudio Dir", File.dirname(path))  
          else
            UI.messagebox("Failed to open #{path}")
          end
          
        end
      end
      return(result)
    end
    

    def save_openstudio
      result = nil
      
      path = Plugin.model_manager.model_interface.openstudio_path
      if (path.nil?)
        result = save_openstudio_as
      else
        # check if writable?
        result = Plugin.model_manager.model_interface.export_openstudio(path)
        if not result 
          UI.messagebox("Failed to save OpenStudio Model to '#{path}'")
        end
      end
      
      return result
    end


    def save_openstudio_as
      result = false

      if (path = UI.save_panel("Save OpenStudio Model", Plugin.model_manager.model_interface.openstudio_dir, Plugin.model_manager.model_interface.openstudio_name))

        extension = OpenStudio::toString(OpenStudio::Path.new(path).extension)
        stem = OpenStudio::toString(OpenStudio::Path.new(path).stem)

        if extension.empty?
        
          path = path + '.osm'
          
          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return save_openstudio_as
            end
          end      
        
        elsif extension != '.osm'
        
          path = OpenStudio::Path.new(path).replace_extension(OpenStudio::Path.new("osm").string).to_s
          new_name = OpenStudio::toString(OpenStudio::Path.new(path).filename)
        
          result = UI.messagebox("File name must have .osm extension. Do you want to rename your file to #{new_name}?",MB_YESNO)
          if result == 6 # YES
            # save file
          else
            return save_openstudio_as
          end        
          
          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return save_openstudio_as
            end
          end      
          
        end   
          
        # store copy for restore
        old_path = Plugin.model_manager.model_interface.openstudio_path

        # do save
        result = Plugin.model_manager.model_interface.export_openstudio(path)

        if result
          # Save the dir so we can start here next time
          Plugin.write_pref("Last OpenStudio Dir", File.dirname(path))  
            
          # set path again to establish path watcher as file now exists
          Plugin.model_manager.model_interface.openstudio_path = path
        else
          UI.messagebox("Failed to save OpenStudio Model to '#{path}'")
          
          # will create a new path watcher for old path
          Plugin.model_manager.model_interface.openstudio_path = old_path
        end
       
      end
      
      return result
    end
    
    def select_default_template
      default_template_dir = Plugin.default_template_dir
      if (path = UI.open_panel("Select Default OpenStudio Template", default_template_dir, "*.osm"))  # bug in SU7 prevents file filters from working
        
        idf = nil
        if $OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS
          progress_dialog = ProgressDialog.new("Verifying OpenStudio Model")
          idf = OpenStudio::IdfFile::load(OpenStudio::Path.new(path), "OpenStudio".to_IddFileType, progress_dialog)
          progress_dialog.destroy
        else
          idf = OpenStudio::IdfFile::load(OpenStudio::Path.new(path), "OpenStudio".to_IddFileType)
        end
        
        if not idf.empty? and idf.get.isValid("Draft".to_StrictnessLevel)
          # only keep if good
          Plugin.write_pref("Default Template Path", path)
        else
          UI.messagebox("Failed to import OpenStudio model from #{path}")
        end
      end
    end
    
    def import_openstudio
      if (prompt_to_continue_import("OpenStudio Model"))
        openstudio_dir = Plugin.read_pref("Last OpenStudio Import Dir")
        if not openstudio_dir
          openstudio_dir = Plugin.model_manager.model_interface.openstudio_dir
        end

        if (path = UI.open_panel("Import OpenStudio Model", openstudio_dir, "*.osm"))  # bug in SU7 prevents file filters from working
          
          success = Plugin.model_manager.open_openstudio(path, Sketchup.active_model, false, true)
          
          if success
            # Save the dir so we can start here next time
            Plugin.write_pref("Last OpenStudio Import Dir", File.dirname(path))
          else
            UI.messagebox("Failed to import #{path}.")
          end

        end
      end
    end
    
    def import_constructions
      dir = Plugin.read_pref("Last Constructions Import Dir")
      if not dir
        dir = Plugin.model_manager.model_interface.openstudio_dir
      end

      if (path = UI.open_panel("Import Constructions", dir, "*.osm"))  # bug in SU7 prevents file filters from working
        # Save the dir so we can start here next time
        Plugin.write_pref("Last Constructions Import Dir", File.dirname(path))  
          
        success = false
        openstudio_model = Plugin.model_manager.model_from_openstudio_path(path)
        if openstudio_model
          Plugin.model_manager.model_interface.import_constructions(openstudio_model)
          success = true
        end

        if not success 
          UI.messagebox("Failed to import #{path}")
        end
      end
    end
    
    def import_schedules
      dir = Plugin.read_pref("Last Schedules Import Dir")
      if not dir
        dir = Plugin.model_manager.model_interface.openstudio_dir
      end

      if (path = UI.open_panel("Import Schedules", dir, "*.osm"))  # bug in SU7 prevents file filters from working
        # Save the dir so we can start here next time
        Plugin.write_pref("Last Schedules Import Dir", File.dirname(path))  
        
        success = false
        openstudio_model = Plugin.model_manager.model_from_openstudio_path(path)
        if openstudio_model
          Plugin.model_manager.model_interface.import_schedules(openstudio_model)
          success = true
        end
        
        if not success 
          UI.messagebox("Failed to import #{path}")
        end
      end
    end
    
    def import_space_types
      dir = Plugin.read_pref("Last Space Loads Import Dir")
      if not dir
        dir = Plugin.model_manager.model_interface.openstudio_dir
      end

      if (path = UI.open_panel("Import Space Loads", dir, "*.osm"))  # bug in SU7 prevents file filters from working
        # Save the dir so we can start here next time
        Plugin.write_pref("Last Space Loads Import Dir", File.dirname(path))  
        
        success = false
        openstudio_model = Plugin.model_manager.model_from_openstudio_path(path)
        if openstudio_model
          Plugin.model_manager.model_interface.import_space_types(openstudio_model)
          success = true
        end

        if not success 
          UI.messagebox("Failed to import #{path}")
        end
      end
    end
    
    def import_idf
      if (prompt_to_continue_import("EnergyPlus Idf"))
        idf_dir = Plugin.read_pref("Last Idf Import Dir")
        if not idf_dir
          idf_dir = Plugin.model_manager.model_interface.openstudio_dir
        end

        if (path = UI.open_panel("Import EnergyPlus Idf", idf_dir, "*.idf"))  # bug in SU7 prevents file filters from working

          success = false
          
          workspace = Plugin.model_manager.workspace_from_idf_path(path)
          if workspace
            openstudio_model, errors, warnings, untranslated_idf_objects = Plugin.model_manager.model_from_workspace(workspace)
            Plugin.model_manager.attach_openstudio_model(openstudio_model, Sketchup.active_model, nil, false, true, errors, warnings, untranslated_idf_objects)
            success = true
          end
          
          if success
            # Save the dir so we can start here next time
            Plugin.write_pref("Last Idf Import Dir", File.dirname(path))
          else
            UI.messagebox("Failed to import #{path}.  If this is an older EnergyPlus Idf please upgrade to version 8.1 using the EnergyPlus transition program.")
          end
          
        end
      end
    end

    def import_idf_constructions
      dir = Plugin.read_pref("Last Idf Constructions Import Dir")
      if not dir
        dir = Plugin.model_manager.model_interface.openstudio_dir
      end

      if (path = UI.open_panel("Import EnergyPlus Idf Constructions", dir, "*.idf; *.imf"))  # bug in SU7 prevents file filters from working
        # Save the dir so we can start here next time
        Plugin.write_pref("Last Idf Constructions Import Dir", File.dirname(path))  
          
        success = false  
        workspace = Plugin.model_manager.workspace_from_idf_path(path)
        if workspace
          Plugin.model_manager.model_interface.import_idf_constructions(workspace)
          success = true
        end

        if not success 
          UI.messagebox("Failed to import #{path}")
        end
      end
    end
    
    def import_idf_schedules
      dir = Plugin.read_pref("Last Idf Schedules Import Dir")
      if not dir
        dir = Plugin.model_manager.model_interface.openstudio_dir
      end

      if (path = UI.open_panel("Import EnergyPlus Idf Schedules", dir, "*.idf; *.imf"))  # bug in SU7 prevents file filters from working
        # Save the dir so we can start here next time
        Plugin.write_pref("Last Idf Schedules Import Dir", File.dirname(path))  
        
        success = false  
        workspace = Plugin.model_manager.workspace_from_idf_path(path)
        if workspace
          Plugin.model_manager.model_interface.import_idf_schedules(workspace)
          success = true
        end
        
        if not success 
          UI.messagebox("Failed to import #{path}")
        end
      end
    end
    
    def import_gbxml
      if (prompt_to_continue_import("gbXML Model"))
        gbxml_dir = Plugin.read_pref("Last gbXML Import Dir")
        if not gbxml_dir
          gbxml_dir = Plugin.model_manager.model_interface.openstudio_dir
        end

        if (path = UI.open_panel("Import gbXML Model", gbxml_dir, "*.xml"))  # bug in SU7 prevents file filters from working

          success = false
          
          openstudio_model, errors, warnings = Plugin.model_manager.model_from_gbXML_path(path)
          if openstudio_model
            Plugin.model_manager.attach_openstudio_model(openstudio_model, Sketchup.active_model, nil, false, true, errors, warnings)
            success = true
          end
          
          if success
            # Save the dir so we can start here next time
            Plugin.write_pref("Last gbXML Import Dir", File.dirname(path))
          else
            UI.messagebox("Failed to import gbXML file at #{path}.")
          end
          
        end
      end
    end

    def import_sdd
      if (prompt_to_continue_import("SDD Model"))
        sdd_dir = Plugin.read_pref("Last SDD Import Dir")
        if not sdd_dir
          sdd_dir = Plugin.model_manager.model_interface.openstudio_dir
        end

        if (path = UI.open_panel("Import SDD Model", sdd_dir, "*.xml"))  # bug in SU7 prevents file filters from working

          success = false
          
          openstudio_model, errors, warnings = Plugin.model_manager.model_from_sdd_path(path)
          if openstudio_model
            Plugin.model_manager.attach_openstudio_model(openstudio_model, Sketchup.active_model, nil, false, true, errors, warnings)
            success = true
          end
          
          if success
            # Save the dir so we can start here next time
            Plugin.write_pref("Last SDD Import Dir", File.dirname(path))
          else
            UI.messagebox("Failed to import SDD file at #{path}.")
          end
          
        end
      end
    end
    
    def export_openstudio
      
      if (path = UI.save_panel("Export OpenStudio Model", Plugin.model_manager.model_interface.openstudio_dir, Plugin.model_manager.model_interface.openstudio_name))

        extension = OpenStudio::toString(OpenStudio::Path.new(path).extension)
        stem = OpenStudio::toString(OpenStudio::Path.new(path).stem)

        if extension.empty?
        
          path = path + '.osm'
          
          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_openstudio
            end
          end      
        
        elsif extension != '.osm'
        
          path = OpenStudio::Path.new(path).replace_extension(OpenStudio::Path.new("osm").string).to_s
          new_name = OpenStudio::toString(OpenStudio::Path.new(path).filename)
        
          result = UI.messagebox("File name must have .osm extension. Do you want to rename your file to #{new_name}?",MB_YESNO)
          if result == 6 # YES
            # save file
          else
            return export_openstudio
          end        
          
          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_openstudio
            end
          end      
          
        end   

        result = Plugin.model_manager.model_interface.export_openstudio(path)
        
        if not result 
          UI.messagebox("Failed to export OpenStudio Model to #{path}")
        end
        
      end
    end

    def export_untranslated_idf

      if (path = UI.save_panel("Export Untranslated EnergyPlus Idf", Plugin.model_manager.model_interface.openstudio_dir, Plugin.model_manager.model_interface.openstudio_name.gsub(/\.osm$/, "-Untranslated.idf")))

        extension = OpenStudio::toString(OpenStudio::Path.new(path).extension)
        stem = OpenStudio::toString(OpenStudio::Path.new(path).stem)

        if extension.empty?

          path = path + '.idf'

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_untranslated_idf
            end
          end      

        elsif extension != '.idf'

          path = OpenStudio::Path.new(path).replace_extension(OpenStudio::Path.new("idf").string).to_s
          new_name = OpenStudio::toString(OpenStudio::Path.new(path).filename)

          result = UI.messagebox("File name must have .idf extension. Do you want to rename your file to #{new_name}?",MB_YESNO)
          if result == 6 # YES
            # save file
          else
            return export_untranslated_idf
          end        

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_untranslated_idf
            end
          end      
        end   
        
        result = Plugin.model_manager.model_interface.export_untranslated_idf(path)

        if not result
          UI.messagebox("Failed to export Untranslated EnergyPlus Idf to #{path}")
        end
      end

    end
    
    def export_idf
      if Plugin.read_pref('Warn on Idf Export')
        # YESNO messagebox, user can cancel at save_panel
        result = UI.messagebox("Translation of OpenStudio Model to EnergyPlus Idf may result in loss of information for unsupported object types.  Choose Yes to ignore this warning from now on or No to continue showing this warning.", MB_YESNO)
        if result == 6 # YES
          Plugin.write_pref('Warn on Idf Export', false)
        end
      end

      if (path = UI.save_panel("Export EnergyPlus Idf", Plugin.model_manager.model_interface.openstudio_dir, Plugin.model_manager.model_interface.openstudio_name.gsub(/\.osm$/, ".idf")))

        extension = OpenStudio::toString(OpenStudio::Path.new(path).extension)
        stem = OpenStudio::toString(OpenStudio::Path.new(path).stem)

        if extension.empty?

          path = path + '.idf'

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_idf
            end
          end      

        elsif extension != '.idf'

          path = OpenStudio::Path.new(path).replace_extension(OpenStudio::Path.new("idf").string).to_s
          new_name = OpenStudio::toString(OpenStudio::Path.new(path).filename)

          result = UI.messagebox("File name must have .idf extension. Do you want to rename your file to #{new_name}?",MB_YESNO)
          if result == 6 # YES
            # save file
          else
            return export_idf
          end        

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_idf
            end
          end      
        end   
        
        result = Plugin.model_manager.model_interface.export_idf(path)

        if not result
          UI.messagebox("Failed to export EnergyPlus Idf to #{path}")
        end
      end

    end

    def export_gbxml
      if Plugin.read_pref('Warn on gbXML Export')
        # YESNO messagebox, user can cancel at save_panel
        result = UI.messagebox("Translation of OpenStudio Model to gbXML may result in loss of information for unsupported object types.  Choose Yes to ignore this warning from now on or No to continue showing this warning.", MB_YESNO)
        if result == 6 # YES
          Plugin.write_pref('Warn on gbXML Export', false)
        end
      end

      if (path = UI.save_panel("Export gbXML Model", Plugin.model_manager.model_interface.openstudio_dir, Plugin.model_manager.model_interface.openstudio_name.gsub(/\.osm$/, ".xml")))

        extension = OpenStudio::toString(OpenStudio::Path.new(path).extension)
        stem = OpenStudio::toString(OpenStudio::Path.new(path).stem)

        if extension.empty?

          path = path + '.xml'

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_gbxml
            end
          end      

        elsif extension != '.xml'

          path = OpenStudio::Path.new(path).replace_extension(OpenStudio::Path.new("xml").string).to_s
          new_name = OpenStudio::toString(OpenStudio::Path.new(path).filename)

          result = UI.messagebox("File name must have .xml extension. Do you want to rename your file to #{new_name}?",MB_YESNO)
          if result == 6 # YES
            # save file
          else
            return export_gbxml
          end        

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_gbxml
            end
          end      
        end   
        
        result = Plugin.model_manager.model_interface.export_gbxml(path)

        if not result
          UI.messagebox("Failed to export gbXML model to #{path}")
        end
      end

    end

    def export_sdd
      if Plugin.read_pref('Warn on SDD Export')
        # YESNO messagebox, user can cancel at save_panel
        result = UI.messagebox("Translation of OpenStudio Model to SDD may result in loss of information for unsupported object types.  Choose Yes to ignore this warning from now on or No to continue showing this warning.", MB_YESNO)
        if result == 6 # YES
          Plugin.write_pref('Warn on SDD Export', false)
        end
      end

      if (path = UI.save_panel("Export SDD Model", Plugin.model_manager.model_interface.openstudio_dir, Plugin.model_manager.model_interface.openstudio_name.gsub(/\.osm$/, ".xml")))

        extension = OpenStudio::toString(OpenStudio::Path.new(path).extension)
        stem = OpenStudio::toString(OpenStudio::Path.new(path).stem)

        if extension.empty?

          path = path + '.xml'

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_sdd
            end
          end      

        elsif extension != '.xml'

          path = OpenStudio::Path.new(path).replace_extension(OpenStudio::Path.new("xml").string).to_s
          new_name = OpenStudio::toString(OpenStudio::Path.new(path).filename)

          result = UI.messagebox("File name must have .xml extension. Do you want to rename your file to #{new_name}?",MB_YESNO)
          if result == 6 # YES
            # save file
          else
            return export_sdd
          end        

          # see if file exists and prompt user if they want to overwrite it
          # only needed because not caught by sketchup save dialog
          if File.exist?(path)
            result = UI.messagebox("File named " + path + " exists. Do you want to overwrite it?",MB_YESNO)
            if result == 6 # YES
              # save file
            else
              return export_sdd
            end
          end      
        end   
        
        result = Plugin.model_manager.model_interface.export_sdd(path)

        if not result
          UI.messagebox("Failed to export SDD model to #{path}")
        end
      end

    end
    
    def check_site(source_name, new_name, new_lat, new_lon, new_time, new_elev)

      # if we already have site set check distance between site and weather file
      if site = Plugin.model_manager.model_interface.site

        site_name = site.model_object.name.get
        site_lat = site.model_object.latitude
        site_lon = site.model_object.longitude
        site_elev = site.model_object.elevation

        distance = OpenStudio::getDistanceLatLon(new_lat, new_lon, site_lat, site_lon) / 1000.0 # km
        delta_elev = (new_elev - site_elev).abs

        if distance > 50 # 50 km
          result = UI.messagebox("Distance between #{source_name} location and site location is #{distance.round} km.  Do you want to set site location to #{source_name} location?", MB_YESNOCANCEL)
          if result == 6 # Yes
            # set the model object fields
            site.model_object.setName(new_name.gsub(',', '-'))
            site.model_object.setLatitude(new_lat)
            site.model_object.setLongitude(new_lon)
            site.model_object.setTimeZone(new_time)
            site.model_object.setElevation(new_elev)

            # delta_elev is now 0
            delta_elev = 0

          elsif result == 2 # Cancel
            # no-op
            return
          end
        end

        if delta_elev > 200 # 200 m
         result = UI.messagebox("Distance between #{source_name} elevation and site elevation is #{delta_elev.round} m.  Do you want to set site location to #{source_name} location?", MB_YESNOCANCEL)
          if result == 6 # Yes
            # set the model object fields
            site.model_object.setName(new_name.gsub(',', '-'))
            site.model_object.setLatitude(new_lat)
            site.model_object.setLongitude(new_lon)
            site.model_object.setTimeZone(new_time)
            site.model_object.setElevation(new_elev)

          elsif result == 2 # Cancel
            # no-op
            return
          end
        end

      else
        # no site offer to set it
        result = UI.messagebox("This model does not yet specify a site, do you want to set it to the #{source_name} location", MB_YESNO)
        if result == 6 # Yes

          # create the site
          site = Site.new
          site.create_entity
          site.create_model_object
          site.update_model_object
          site.add_watcher
          site.add_observers
          
          # set the model object fields
          site.model_object.setName(new_name.gsub(',', '-'))
          site.model_object.setLatitude(new_lat)
          site.model_object.setLongitude(new_lon)
          site.model_object.setTimeZone(new_time)
          site.model_object.setElevation(new_elev)
        end

      end
    end
    
    
    def prompt_to_continue_open
      result = false

      if Plugin.model_manager.model_interface and Plugin.model_manager.model_interface.openstudio_model
        button = UI.messagebox("Would you like to save your current OpenStudio Model? " , MB_YESNOCANCEL )
        if (button == 6)  # YES
          result = save_openstudio
        elsif (button == 7) # NO
          result = true
        end
      else
        result = true
      end
      
      return result
    end
    
    
    def prompt_to_continue_import(name)
      result = false

      if Plugin.model_manager.model_interface
        button = UI.messagebox("Would you like to save your current OpenStudio Model before importing an #{name}? " , MB_YESNOCANCEL )
        if (button == 6)  # YES
          result = save_openstudio
        elsif (button == 7) # NO
          result = true
        end
      else
        result = true
      end

      return result
    end
    

  end

end
