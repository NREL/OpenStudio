######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class ExportSpaces < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Export Selected Spaces to a new External Model"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    save_path = OpenStudio::Ruleset::OSArgument::makePathArgument("save_path", false, "osm", false)
    save_path.setDisplayName("Save Export Spaces As ")
    save_path.setDefaultValue("ExportedSpaces.osm")
    result << save_path
    
    begin
      SKETCHUP_CONSOLE.show
    rescue => e
    end
    
    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
  
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    osmPath_2 = runner.getStringArgumentValue("save_path",user_arguments)
    
    # stop script if no spaces are selected.
    anyInSelection = false
    model.getSpaces.each do |space|
      if runner.inSelection(space)
        anyInSelection = true
        break
      end
    end
    
    if not anyInSelection
      runner.registerAsNotApplicable("No spaces selected.")
      return true
    end

    # create a new empty model
    model_2 = OpenStudio::Model::Model.new
    
    # loop through and clone spaces
    count = 0
    model.getSpaces.each do |space|
      if runner.inSelection(space)
        runner.registerInfo("Adding " + space.briefDescription + " to " + osmPath_2.to_s + ".")
        space.clone(model_2)
        count += 1
      end
    end
    
    # save as osm
    model_2.save(OpenStudio::Path.new(osmPath_2),true)
    runner.registerFinalCondition("File named '"+ osmPath_2 + "' created with " + count.to_s + " spaces.")
    puts 'File named "'+ osmPath_2 + '" has been generated'
    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
ExportSpaces.new.registerWithApplication


