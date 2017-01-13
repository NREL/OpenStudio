########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class AssignBuildingStories < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Assign Building Stories"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    return result
  end
  
  # find the first story with z coordinate, create one if needed
  def getStoryForNominalZCoordinate(model, minz)
  
    model.getBuildingStorys.each do |story|
      z = story.nominalZCoordinate
      if not z.empty?
        if minz == z.get
          return story
        end
      end
    end
    
    story = OpenStudio::Model::BuildingStory.new(model)
    story.setNominalZCoordinate(minz)
    return story
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)    
    super(model, runner, user_arguments)
  
    # get all spaces
    spaces = model.getSpaces
    
    runner.createProgressBar("Assigning Stories to Spaces")
  
    # make has of spaces and minz values
    sorted_spaces = Hash.new
    spaces.each do |space|
      # loop through space surfaces to find min z value
      z_points = []
      space.surfaces.each do |surface|
        surface.vertices.each do |vertex|
          z_points << vertex.z
        end
      end
      minz = z_points.min + space.zOrigin
      sorted_spaces[space] = minz
    end
  
    # pre-sort spaces
    sorted_spaces = sorted_spaces.sort{|a,b| a[1]<=>b[1]} 
  
    num_total = spaces.size
    num_complete = 0
    
    # this should take the sorted list and make and assign stories
    sorted_spaces.each do |space|
      space_obj = space[0]
      space_minz = space[1]
      if space_obj.buildingStory.empty?
          
        story = getStoryForNominalZCoordinate(model, space_minz)
        runner.registerInfo("Setting story of Space " + space_obj.name.get + " to " + story.to_s + ".")
        space_obj.setBuildingStory(story)
        
        num_complete += 1
        runner.updateProgress((100*num_complete)/num_total)
      end
    end
    
    runner.destroyProgressBar
    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
AssignBuildingStories.new.registerWithApplication
