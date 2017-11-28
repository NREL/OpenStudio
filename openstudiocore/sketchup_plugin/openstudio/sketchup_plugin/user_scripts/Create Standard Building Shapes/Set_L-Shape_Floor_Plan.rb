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

class SetLShapeFloorPlan < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set L-Shape Floor Plan"
  end
  
  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("length", false)
    length.setDisplayName("Building Length (m)")
    length.setDefaultValue(40.0)
    result << length
    
    width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("width", false)
    width.setDisplayName("Building Width (m)")
    width.setDefaultValue(40.0)
    result << width
    
    lower_end_width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("lower_end_width", false)
    lower_end_width.setDisplayName("Lower End Width (m)")
    lower_end_width.setDefaultValue(20.0)
    result << lower_end_width
    
    upper_end_length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("upper_end_length", false)
    upper_end_length.setDisplayName("Upper End Length (m)")
    upper_end_length.setDefaultValue(20.0)
    result << upper_end_length
    
    num_floors = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("num_floors", false)
    num_floors.setDisplayName("Number of Floors")
    num_floors.setDefaultValue(1.0)
    result << num_floors
    
    floor_to_floor_height = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("floor_to_floor_height", false)
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
    lower_end_width = runner.getDoubleArgumentValue("lower_end_width",user_arguments)
    upper_end_length = runner.getDoubleArgumentValue("upper_end_length",user_arguments)
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
    
    if lower_end_width <= 1e-4 or lower_end_width >= (width - 1e-4)
      runner.registerError("Lower end width must be greater than 0 and less than #{width}m.")
      return false
    end
    
    if upper_end_length <= 1e-4 or upper_end_length >= (length - 1e-4)
      runner.registerError("Upper end length must be greater than 0 and less than #{length}m.")
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
    
    shortest_side = [lower_end_width,upper_end_length].min
    if perimeter_zone_depth < 0 or 2*perimeter_zone_depth >= (shortest_side - 1e-4)
      runner.registerError("Perimeter zone depth must be greater than or equal to 0 and less than #{shortest_side/2}m.")
      return false
    end
    
    # Create progress bar
    runner.createProgressBar("Creating Spaces")
    num_total = perimeter_zone_depth>0 ? num_floors*8 : num_floors*2
    num_complete = 0
    
    # Loop through the number of floors
    for floor in (0..num_floors-1)
    
      z = floor_to_floor_height * floor
      
      #Create a new story within the building
      story = OpenStudio::Model::BuildingStory.new(model)
      story.setNominalFloortoFloorHeight(floor_to_floor_height)
      story.setName("Story #{floor+1}")
      
      
      nw_point = OpenStudio::Point3d.new(0,width,z)
      upper_ne_point = OpenStudio::Point3d.new(upper_end_length,width,z)
      upper_sw_point = OpenStudio::Point3d.new(upper_end_length,lower_end_width,z)
      lower_ne_point = OpenStudio::Point3d.new(length,lower_end_width,z)
      se_point = OpenStudio::Point3d.new(length,0,z)
      sw_point = OpenStudio::Point3d.new(0,0,z)
      
      # Identity matrix for setting space origins
      m = OpenStudio::Matrix.new(4,4,0)
        m[0,0] = 1
        m[1,1] = 1
        m[2,2] = 1
        m[3,3] = 1
      
      # Define polygons for a L-shape building with perimeter core zoning
      if perimeter_zone_depth > 0
        perimeter_nw_point = nw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_upper_ne_point = upper_ne_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_upper_sw_point = upper_sw_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_lower_ne_point = lower_ne_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_se_point = se_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_lower_sw_point = sw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
        
        west_perimeter_polygon  = OpenStudio::Point3dVector.new
          west_perimeter_polygon << sw_point
          west_perimeter_polygon << nw_point
          west_perimeter_polygon << perimeter_nw_point
          west_perimeter_polygon << perimeter_lower_sw_point
        west_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(west_perimeter_polygon, floor_to_floor_height, model)
        west_perimeter_space = west_perimeter_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        west_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_perimeter_space.setBuildingStory(story)
        west_perimeter_space.setName("Story #{floor+1} West Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        north_upper_perimeter_polygon = OpenStudio::Point3dVector.new
          north_upper_perimeter_polygon << nw_point
          north_upper_perimeter_polygon << upper_ne_point
          north_upper_perimeter_polygon << perimeter_upper_ne_point
          north_upper_perimeter_polygon << perimeter_nw_point
        north_upper_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_upper_perimeter_polygon, floor_to_floor_height, model)
        north_upper_perimeter_space = north_upper_perimeter_space.get
        m[0,3] = perimeter_nw_point.x
        m[1,3] = perimeter_nw_point.y
        m[2,3] = perimeter_nw_point.z
        north_upper_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_upper_perimeter_space.setBuildingStory(story)
        north_upper_perimeter_space.setName("Story #{floor+1} North Upper Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_upper_perimeter_polygon = OpenStudio::Point3dVector.new
          east_upper_perimeter_polygon << upper_ne_point
          east_upper_perimeter_polygon << upper_sw_point
          east_upper_perimeter_polygon << perimeter_upper_sw_point
          east_upper_perimeter_polygon << perimeter_upper_ne_point
        east_upper_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_upper_perimeter_polygon, floor_to_floor_height, model)
        east_upper_perimeter_space = east_upper_perimeter_space.get
        m[0,3] = perimeter_upper_sw_point.x
        m[1,3] = perimeter_upper_sw_point.y
        m[2,3] = perimeter_upper_sw_point.z
        east_upper_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_upper_perimeter_space.setBuildingStory(story)
        east_upper_perimeter_space.setName("Story #{floor+1} East Upper Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        north_lower_perimeter_polygon = OpenStudio::Point3dVector.new
          north_lower_perimeter_polygon << upper_sw_point
          north_lower_perimeter_polygon << lower_ne_point
          north_lower_perimeter_polygon << perimeter_lower_ne_point
          north_lower_perimeter_polygon << perimeter_upper_sw_point
        north_lower_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_lower_perimeter_polygon, floor_to_floor_height, model)
        north_lower_perimeter_space = north_lower_perimeter_space.get
        m[0,3] = perimeter_upper_sw_point.x
        m[1,3] = perimeter_upper_sw_point.y
        m[2,3] = perimeter_upper_sw_point.z
        north_lower_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_lower_perimeter_space.setBuildingStory(story)
        north_lower_perimeter_space.setName("Story #{floor+1} North Lower Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_lower_perimeter_polygon = OpenStudio::Point3dVector.new
          east_lower_perimeter_polygon << lower_ne_point
          east_lower_perimeter_polygon << se_point
          east_lower_perimeter_polygon << perimeter_se_point
          east_lower_perimeter_polygon << perimeter_lower_ne_point
        east_lower_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_lower_perimeter_polygon, floor_to_floor_height, model)
        east_lower_perimeter_space = east_lower_perimeter_space.get
        m[0,3] = perimeter_se_point.x
        m[1,3] = perimeter_se_point.y
        m[2,3] = perimeter_se_point.z
        east_lower_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_lower_perimeter_space.setBuildingStory(story)
        east_lower_perimeter_space.setName("Story #{floor+1} East Lower Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        south_perimeter_polygon = OpenStudio::Point3dVector.new
          south_perimeter_polygon << se_point
          south_perimeter_polygon << sw_point
          south_perimeter_polygon << perimeter_lower_sw_point
          south_perimeter_polygon << perimeter_se_point
        south_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(south_perimeter_polygon, floor_to_floor_height, model)
        south_perimeter_space = south_perimeter_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        south_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_perimeter_space.setBuildingStory(story)
        south_perimeter_space.setName("Story #{floor+1} South Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        west_core_polygon = OpenStudio::Point3dVector.new
          west_core_polygon << perimeter_lower_sw_point
          west_core_polygon << perimeter_nw_point
          west_core_polygon << perimeter_upper_ne_point
          west_core_polygon << perimeter_upper_sw_point
        west_core_space = OpenStudio::Model::Space::fromFloorPrint(west_core_polygon, floor_to_floor_height, model)
        west_core_space = west_core_space.get
        m[0,3] = perimeter_lower_sw_point.x
        m[1,3] = perimeter_lower_sw_point.y
        m[2,3] = perimeter_lower_sw_point.z
        west_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_core_space.setBuildingStory(story)
        west_core_space.setName("Story #{floor+1} West Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_core_polygon = OpenStudio::Point3dVector.new
          east_core_polygon << perimeter_upper_sw_point
          east_core_polygon << perimeter_lower_ne_point
          east_core_polygon << perimeter_se_point
          east_core_polygon << perimeter_lower_sw_point
        east_core_space = OpenStudio::Model::Space::fromFloorPrint(east_core_polygon, floor_to_floor_height, model)
        east_core_space = east_core_space.get
        m[0,3] = perimeter_lower_sw_point.x
        m[1,3] = perimeter_lower_sw_point.y
        m[2,3] = perimeter_lower_sw_point.z
        east_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_core_space.setBuildingStory(story)
        east_core_space.setName("Story #{floor+1} East Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
      # Minimal zones
      else
        west_polygon = OpenStudio::Point3dVector.new
          west_polygon << sw_point
          west_polygon << nw_point
          west_polygon << upper_ne_point
          west_polygon << upper_sw_point
        west_space = OpenStudio::Model::Space::fromFloorPrint(west_polygon, floor_to_floor_height, model)
        west_space = west_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        west_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_space.setBuildingStory(story)
        west_space.setName("Story #{floor+1} West Space")
  
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_polygon = OpenStudio::Point3dVector.new
          east_polygon << sw_point
          east_polygon << upper_sw_point
          east_polygon << lower_ne_point
          east_polygon << se_point
        east_space = OpenStudio::Model::Space::fromFloorPrint(east_polygon, floor_to_floor_height, model)
        east_space = east_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        east_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_space.setBuildingStory(story)
        east_space.setName("Story #{floor+1} East Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
      end
      
      #Set vertical story position
      story.setNominalZCoordinate(z)
      
    end #End of floor loop
    
    runner.destroyProgressBar
    
  end
  
end

SetLShapeFloorPlan.new.registerWithApplication
