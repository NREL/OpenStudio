########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

class SetRectangularFloorPlan < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Rectangular Floor Plan"
  end
  
  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("length",false)
    length.setDisplayName("Building Length (m)")
    length.setDefaultValue(40.0)
    result << length
    
    width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("width",false)
    width.setDisplayName("Building Width (m)")
    width.setDefaultValue(20.0)
    result << width
    
    num_floors = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("num_floors",false)
    num_floors.setDisplayName("Number of Floors")
    num_floors.setDefaultValue(1.0)
    result << num_floors
    
    floor_to_floor_height = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("floor_to_floor_height",false)
    floor_to_floor_height.setDisplayName("Floor to Floor Height (m)")
    floor_to_floor_height.setDefaultValue(3.8)
    result << floor_to_floor_height    
    
    plenum_height = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("plenum_height", false)
    plenum_height.setDisplayName("Plenum Height (m)")
    plenum_height.setDefaultValue(1.0)
    result << plenum_height     
    
    perimeter_zone_depth = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("perimeter_zone_depth", false)
    perimeter_zone_depth.setDisplayName("Perimeter Zone Depth (m)")
    perimeter_zone_depth.setDefaultValue(4.57)
    result << perimeter_zone_depth
    
    return result
  end
  
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
    
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    length = runner.getDoubleArgumentValue("length",user_arguments)
    width = runner.getDoubleArgumentValue("width",user_arguments)
    num_floors = runner.getDoubleArgumentValue("num_floors",user_arguments)
    floor_to_floor_height = runner.getDoubleArgumentValue("floor_to_floor_height",user_arguments)
    plenum_height = runner.getDoubleArgumentValue("plenum_height",user_arguments)
    perimeter_zone_depth = runner.getDoubleArgumentValue("perimeter_zone_depth",user_arguments)
        
    if length <= 1e-4
      runner.registerError("Length must be greater than 0.")
      return false
    end
    
    if width <= 1e-4
      runner.registerError("Width must be greater than 0.")
      return false
    end
    
    if num_floors <= 1e-4
      runner.registerError("Number of floors must be greater than 0.")
      return false
    end
    
    if floor_to_floor_height <= 1e-4
      runner.registerError("Floor to floor height must be greater than 0.")
      return false
    end
    
    if plenum_height < 0
      runner.registerError("Plenum height must be greater than or equal to 0.")
      return false
    end
    
    shortest_side = [length,width].min
    if perimeter_zone_depth < 0 or 2*perimeter_zone_depth >= (shortest_side - 1e-4)
      runner.registerError("Perimeter zone depth must be greater than or equal to 0 and less than #{shortest_side/2}m")
      return false
    end
    
    # Create progress bar
    runner.createProgressBar("Creating Spaces")
    num_total = perimeter_zone_depth>0 ? num_floors*5 : num_floors
    num_complete = 0
    
    #Loop through the number of floors
    for floor in (0..num_floors-1)
      
      z = floor_to_floor_height * floor
      
      #Create a new story within the building
      story = OpenStudio::Model::BuildingStory.new(model)
      story.setNominalFloortoFloorHeight(floor_to_floor_height)
      story.setName("Story #{floor+1}")
      
      
      nw_point = OpenStudio::Point3d.new(0,width,z)
      ne_point = OpenStudio::Point3d.new(length,width,z)
      se_point = OpenStudio::Point3d.new(length,0,z)
      sw_point = OpenStudio::Point3d.new(0,0,z)
      
      # Identity matrix for setting space origins
      m = OpenStudio::Matrix.new(4,4,0)
        m[0,0] = 1
        m[1,1] = 1
        m[2,2] = 1
        m[3,3] = 1
      
      #Define polygons for a rectangular building
      if perimeter_zone_depth > 0
        perimeter_nw_point = nw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_ne_point = ne_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_se_point = se_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_sw_point = sw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
      
        west_polygon = OpenStudio::Point3dVector.new
          west_polygon << sw_point
          west_polygon << nw_point
          west_polygon << perimeter_nw_point
          west_polygon << perimeter_sw_point
        west_space = OpenStudio::Model::Space::fromFloorPrint(west_polygon, floor_to_floor_height, model)
        west_space = west_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        west_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_space.setBuildingStory(story)
        west_space.setName("Story #{floor+1} West Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        north_polygon = OpenStudio::Point3dVector.new
          north_polygon << nw_point
          north_polygon << ne_point
          north_polygon << perimeter_ne_point
          north_polygon << perimeter_nw_point
        north_space = OpenStudio::Model::Space::fromFloorPrint(north_polygon, floor_to_floor_height, model)
        north_space = north_space.get
        m[0,3] = perimeter_nw_point.x
        m[1,3] = perimeter_nw_point.y
        m[2,3] = perimeter_nw_point.z
        north_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_space.setBuildingStory(story)
        north_space.setName("Story #{floor+1} North Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_polygon = OpenStudio::Point3dVector.new
          east_polygon << ne_point
          east_polygon << se_point
          east_polygon << perimeter_se_point
          east_polygon << perimeter_ne_point
        east_space = OpenStudio::Model::Space::fromFloorPrint(east_polygon, floor_to_floor_height, model)
        east_space = east_space.get
        m[0,3] = perimeter_se_point.x
        m[1,3] = perimeter_se_point.y
        m[2,3] = perimeter_se_point.z
        east_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_space.setBuildingStory(story)
        east_space.setName("Story #{floor+1} East Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        south_polygon = OpenStudio::Point3dVector.new
          south_polygon << se_point
          south_polygon << sw_point
          south_polygon << perimeter_sw_point
          south_polygon << perimeter_se_point
        south_space = OpenStudio::Model::Space::fromFloorPrint(south_polygon, floor_to_floor_height, model)
        south_space = south_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        south_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_space.setBuildingStory(story)
        south_space.setName("Story #{floor+1} South Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        core_polygon = OpenStudio::Point3dVector.new
          core_polygon << perimeter_sw_point
          core_polygon << perimeter_nw_point
          core_polygon << perimeter_ne_point
          core_polygon << perimeter_se_point
        core_space = OpenStudio::Model::Space::fromFloorPrint(core_polygon, floor_to_floor_height, model)
        core_space = core_space.get
        m[0,3] = perimeter_sw_point.x
        m[1,3] = perimeter_sw_point.y
        m[2,3] = perimeter_sw_point.z
        core_space.changeTransformation(OpenStudio::Transformation.new(m))
        core_space.setBuildingStory(story)
        core_space.setName("Story #{floor+1} Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
      # Minimal zones
      else
        core_polygon = OpenStudio::Point3dVector.new
          core_polygon << sw_point
          core_polygon << nw_point
          core_polygon << ne_point
          core_polygon << se_point
        core_space = OpenStudio::Model::Space::fromFloorPrint(core_polygon, floor_to_floor_height, model)
        core_space = core_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        core_space.changeTransformation(OpenStudio::Transformation.new(m))
        core_space.setBuildingStory(story)
        core_space.setName("Story #{floor+1} Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
      end
      
      #Set vertical story position
      story.setNominalZCoordinate(z)
      
    end #End of floor loop
    
    runner.destroyProgressBar
    
  end
  
end

SetRectangularFloorPlan.new.registerWithApplication
