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

require 'openstudio'

class ImportImfSection < OpenStudio::Ruleset::WorkspaceUserScript

  def name
    return "Import Imf Section"
  end
  
  def arguments(workspace)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    imf_file_path = OpenStudio::Ruleset::OSArgument::makePathArgument("imf_file_path",true,"imf")
    imf_file_path.setDisplayName("Imf File Path")
    result << imf_file_path

    # not required because has default value. however, default value will cause error.
    imf_section_name = OpenStudio::Ruleset::OSArgument::makeStringArgument("imf_section_name",false)
    imf_section_name.setDisplayName("Imf Section Name")
    imf_section_name.setDefaultValue("listImfFileSectionNames")
    result << imf_section_name
    
    return result;  
  end
  
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)
    
    if not runner.validateUserArguments(arguments(workspace),user_arguments)
      return false
    end
    
    imf_file_path = runner.getPathArgumentValue("imf_file_path",user_arguments)
    imf_section_name = runner.getStringArgumentValue("imf_section_name",user_arguments)
    
    imfFile = OpenStudio::ImfFile::load(imf_file_path,"EnergyPlus".to_IddFileType)
    if imfFile.empty?
      runner.registerError("Unable to load ImfFile from " + imf_file_path.to_s + ".")
      return false
    end
    
    imfFile = imfFile.get
    objects = imfFile.section(imf_section_name)
    if objects.size == 0
      runner.registerWarning("Imf file section " + imf_section_name.valueAsString + 
                             " does not exist or contains 0 objects.")
    end
    wsObjects = workspace.addObjects(objects)
    if wsObjects.size < objects.size
      runner.registerWarning("Only added " + wsObjects.size.to_s + " of the " + objects.size.to_s + 
                             " objects in Imf file section " + 
                             imf_section_name.valueAsString + ".")
    end
    
    return true
  end

end

ImportImfSection.new.registerWithApplication

