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

require("openstudio/sketchup_plugin/lib/ModelManager")


module OpenStudio

   class IdfImporter < Sketchup::Importer

     # This method is called by SketchUp to determine the description that
     # appears in the File > Import dialog's pulldown list of valid
     # importers.
     def description
       return "EnergyPlus Files (*.idf)"
     end

     # This method is called by SketchUp to determine what file extension
     # is associated with your importer.
     def file_extension
       return "idf"
     end

     # This method is called by SketchUp to get a unique importer id.
     def id
       return "com.sketchup.importers.idf"
     end

     # This method is called by SketchUp to determine if the "Options"
     # button inside the File > Import dialog should be enabled while your
     # importer is selected.
     def supports_options?
       return true
     end

     # This method is called by SketchUp when the user clicks on the
     # "Options" button inside the File > Import dialog. You can use it to
     # gather and store settings for your importer.
     def do_options
       # In a real use you would probably store this information in an
       # instance variable.
       prompts = ["Import Options"]
       defaults = ["Entire Model"]
       list = ["Entire Model|Constructions|Schedules"]
       @options = UI.inputbox(prompts, defaults, list, "Import Options.")
     end

     # This method is called by SketchUp after the user has selected a file
     # to import. This is where you do the real work of opening and
     # processing the file.
     def load_file(file_path, status)

       file_path.gsub!(/\\/, '/')

       Plugin.write_pref("Last Idf Import Dir", File.dirname(file_path))  # Save the dir so we can start here next time

       workspace = Plugin.model_manager.workspace_from_idf_path(file_path)
       if not workspace
         return(0)
       end

       if not @options
        @options = ["Entire Model"]
       end

       case @options[0]
       when "Entire Model"
         if (Plugin.command_manager.prompt_to_continue_import("EnergyPlus Idf"))
           openstudio_model, errors, warnings, untranslated_idf_objects = Plugin.model_manager.model_from_workspace(workspace)

           # import does not set path
           Plugin.model_manager.attach_openstudio_model(openstudio_model, Sketchup.active_model, nil, false, true, errors, warnings, untranslated_idf_objects)
         end
       when "Constructions"
         Plugin.model_manager.model_interface.import_idf_constructions(workspace)
       when "Schedules"
         Plugin.model_manager.model_interface.import_idf_schedules(workspace)
       end

       return 0 # 0 is the code for a successful import
     end
   end

   Sketchup.register_importer(IdfImporter.new)

end
