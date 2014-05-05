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

######################################################################
# == Synopsis 
#
#   Seed a local building component library with all of the schedules
#   in an OpenStudio Model file.
#
# == Usage
#
#  ruby CreateScheduleLibrary.rb ARGV[0] ARGV[1]
#
#  ARGV[0] - Path to OpenStudio Model (osm) file.
#  ARGV[1] - optional - Path location for project database.
#
# == Examples
#
#   ruby CreateScheduleLibrary.rb 'C:\path\to\osm\filename.osm' 'C:\my\database\location\filename'
#
######################################################################

require 'openstudio'

# load osm file
osmFilepath = OpenStudio::Path.new(ARGV[0])
oModel = OpenStudio::Model::Model.load(osmFilepath)
if oModel.empty?
  raise "Unable to load OpenStudio Model from path " + osmFilepath.to_s + "."
end
model = oModel.get

# create database
projectFilepath = OpenStudio::Path.new("./scheduleLibrary.osp")
if ARGV.size > 1
  projectFilepath = OpenStudio::Path.new(ARGV[1])
end
runmanagerFilepath = projectFilepath.parent_path() / 
    OpenStudio::Path.new(OpenStudio::toString(projectFilepath.stem()) + "-run.db")
puts projectFilepath
puts runmanagerFilepath
OpenStudio::remove(projectFilepath) if OpenStudio::exists(projectFilepath)   
OpenStudio::remove(runmanagerFilepath) if OpenStudio::exists(runmanagerFilepath) 
runmanager = OpenStudio::Runmanager::RunManager.new(runmanagerFilepath,true)
database = OpenStudio::Project::ProjectDatabase.new(projectFilepath,runmanager)

# create schedule components
schedules = model.getSchedules
puts "Creating components for " + schedules.size.to_s + " schedules."
components = OpenStudio::Model::ComponentVector.new
schedules.each { |schedule|
  newComponent = schedule.createComponent
  ok = newComponent.componentData.setName(schedule.name.get)
  raise "Unable to set Component name to '" + schedule.name.get + "'." if not ok
  components.push(newComponent)
}

# ETH@20121010 - For some reason, the Components are getting deleted out from under the 
# components vector. The problem does not happen in C++, and the Trace messages I added 
# to the Component and Component_Impl destructors do not appear in the log.
  
# save components to database
index = 1
puts "Saving " + components.size.to_s + " components to " + projectFilepath.to_s + "."
components.each { |component|
  puts "Begin saving component #{index}"
  path = projectFilepath.parent_path() / OpenStudio::Path.new("component" + index.to_s)
  puts "Component dir = #{path}"
  OpenStudio::remove_all(path) if OpenStudio::exists(path)
  path = path / OpenStudio::Path.new("component.osc")
  puts "Component path = #{path}"
  component.save(path,true)
  fileReference = OpenStudio::FileReference.new(path)
  fileReferenceRecord = OpenStudio::Project::FileReferenceRecord.new(fileReference,database)
  index = index + 1
}

database.save
