######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

def loadIdfFile(idfPathAndFilename)

  workspace = OpenStudio::Workspace.new("None".to_StrictnessLevel,"EnergyPlus".to_IddFileType)
  
  oIdfFile = OpenStudio::IdfFile.load(OpenStudio::Path.new(idfPathAndFilename),"EnergyPlus".to_IddFileType)
  if oIdfFile
    idfFile = oIdfFile.get()
    workspace = OpenStudio::Workspace.new(idfFile)
  end
  
  return workspace

end

def ensureSqlOutput(workspace)

  sqliteOutputObjects = workspace.getObjectsByType(OpenStudio::IddObjectType.new("Output:SQLite"))
  
  if (sqliteOutputObjects.size() == 0) 
    sqliteIdfObject = OpenStudio::IdfObject.new(OpenStudio::IddObjectType.new("Output:SQLite"))
    sqliteIdfObject.setString(0,"SimpleAndTabular")
    result = workspace.addObject(sqliteIdfObject)
    if not result
      puts "Unable to add Output:SQLite object to IDF workspace. SQL output will not be generated."
    end
  end
  
  return workspace

end