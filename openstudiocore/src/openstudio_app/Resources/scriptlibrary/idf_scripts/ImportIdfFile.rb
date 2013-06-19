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

class ImportIdfFile < OpenStudio::Ruleset::WorkspaceUserScript

  def name
    return "Import Idf File"
  end
  
  def arguments(workspace)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    idf_file_path = OpenStudio::Ruleset::OSArgument::makePathArgument("idf_file_path",true,"idf")
    idf_file_path.setDisplayName("Idf File Path")
    result << idf_file_path
    
    return result;  
  end
  
  def run(workspace, runner, user_arguments)
    super(workspace, runner, user_arguments)
    
    if not runner.validateUserArguments(arguments(workspace),user_arguments)
      return false
    end
    
    idf_file_path = runner.getPathArgumentValue("idf_file_path",user_arguments)
       
    idfFile = OpenStudio::IdfFile::load(idf_file_path)
    if idfFile.empty?
      runner.registerError("Unable to load IdfFile from " + idf_file_path.valueAsPath.to_s + ".")
      return false
    end
    
    idfFile = idfFile.get
    objects = idfFile.objects
    wsObjects = workspace.addObjects(objects)
    if wsObjects.size < objects.size
      runner.registerWarning("Only added " + wsObjects.size + " of the " + objects.size + 
                             " objects in the IDF file " + idf_file_path.valueAsPath.to_s + ".")
    end
    
    return true
  end

end

ImportIdfFile.new.registerWithApplication

