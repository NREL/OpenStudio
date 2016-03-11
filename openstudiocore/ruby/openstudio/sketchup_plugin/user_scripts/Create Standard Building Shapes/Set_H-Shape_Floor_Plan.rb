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

class SetHShapeFloorPlan < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set H-Shape Floor Plan"
  end
  
  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("length", false)
    length.setDisplayName("Building Length (m)")
    length.setDefaultValue(40.0)
    result << length
    
    left_width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("left_width", false)
    left_width.setDisplayName("Left Width (m)")
    left_width.setDefaultValue(40.0)
    result << left_width
    
    center_width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("center_width", false)
    center_width.setDisplayName("Center Width (m)")
    center_width.setDefaultValue(10.0)
    result << center_width
    
    right_width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("right_width", false)
    right_width.setDisplayName("Right Width (m)")
    right_width.setDefaultValue(40.0)
    result << right_width
    
    left_end_length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("left_end_length")
    left_end_length.setDisplayName("Left End Length (m)")
    left_end_length.setDefaultValue(15.0)
    result << left_end_length
    
    right_end_length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("right_end_length", false)
    right_end_length.setDisplayName("Right End Length (m)")
    right_end_length.setDefaultValue(15.0)
    result << right_end_length
    
    left_upper_end_offset = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("left_upper_end_offset", false)
    left_upper_end_offset.setDisplayName("Left Upper End Offset (m)")
    left_upper_end_offset.setDefaultValue(15.0)
    result << left_upper_end_offset
    
    right_upper_end_offset = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("right_upper_end_offset", false)
    right_upper_end_offset.setDisplayName("Right Upper End Offset (m)")
    right_upper_end_offset.setDefaultValue(15.0)
    result << right_upper_end_offset
    
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
    left_width = runner.getDoubleArgumentValue("left_width",user_arguments)
    center_width = runner.getDoubleArgumentValue("center_width",user_arguments)
    right_width = runner.getDoubleArgumentValue("right_width",user_arguments)
    left_end_length = runner.getDoubleArgumentValue("left_end_length",user_arguments)
    right_end_length = runner.getDoubleArgumentValue("right_end_length",user_arguments)
    left_upper_end_offset = runner.getDoubleArgumentValue("left_upper_end_offset",user_arguments)
    right_upper_end_offset = runner.getDoubleArgumentValue("right_upper_end_offset",user_arguments)
    num_floors = runner.getDoubleArgumentValue("num_floors",user_arguments)
    floor_to_floor_height = runner.getDoubleArgumentValue("floor_to_floor_height",user_arguments)
    plenum_height = runner.getDoubleArgumentValue("plenum_height",user_arguments)
    perimeter_zone_depth = runner.getDoubleArgumentValue("perimeter_zone_depth",user_arguments)
    
    if length <= 1e-4
      runner.registerError("Length must be greater than 0.")
      return false
    end
    
    if left_width <= 1e-4
      runner.registerError("Left width must be greater than 0.")
      return false
    end
    
    if right_width <= 1e-4
      runner.registerError("Right width must be greater than 0.")
      return false
    end
    
    if center_width <= 1e-4 or center_width >= ([left_width,right_width].min - 1e-4)
      runner.registerError("Center width must be greater than 0 and less than #{[left_width,right_width].min}m.")
      return false
    end
    
    if left_end_length <= 1e-4 or left_end_length >= (length - 1e-4)
      runner.registerError("Left end length must be greater than 0 and less than #{length}m.")
      return false
    end
    
    if right_end_length <= 1e-4 or right_end_length >= (length - left_end_length - 1e-4)
      runner.registerError("Right end length must be greater than 0 and less than #{length - left_end_length}m.")
      return false
    end
    
    if left_upper_end_offset <= 1e-4 or left_upper_end_offset >= (left_width - center_width - 1e-4)
      runner.registerError("Left upper end offset must be greater than 0 and less than #{left_width - center_width}m.")
      return false
    end
    
    if right_upper_end_offset <= 1e-4 or right_upper_end_offset >= (right_width - center_width - 1e-4)
      runner.registerError("Right upper end offset must be greater than 0 and less than #{right_width - center_width}m.")
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
    
    shortest_side = [length/2,left_width,center_width,right_width,left_end_length,right_end_length].min
    if perimeter_zone_depth < 0 or 2*perimeter_zone_depth >= (shortest_side - 1e-4)
      runner.registerError("Perimeter zone depth must be greater than or equal to 0 and less than #{shortest_side/2}m.")
      return false
    end
    
    # Create progress bar
    runner.createProgressBar("Creating Spaces")
    num_total = perimeter_zone_depth>0 ? num_floors*15 : num_floors*3
    num_complete = 0
    
    # Loop through the number of floors
    for floor in (0..num_floors-1)
    
      z = floor_to_floor_height * floor
      
      #Create a new story within the building
      story = OpenStudio::Model::BuildingStory.new(model)
      story.setNominalFloortoFloorHeight(floor_to_floor_height)
      story.setName("Story #{floor+1}")
      
      
      left_origin = (right_width - right_upper_end_offset) > (left_width - left_upper_end_offset) ? (right_width - right_upper_end_offset) - (left_width - left_upper_end_offset) : 0
      
      left_nw_point = OpenStudio::Point3d.new(0,left_width + left_origin,z)
      left_ne_point = OpenStudio::Point3d.new(left_end_length,left_width + left_origin,z)
      left_se_point = OpenStudio::Point3d.new(left_end_length,left_origin,z)
      left_sw_point = OpenStudio::Point3d.new(0,left_origin,z)
      center_nw_point = OpenStudio::Point3d.new(left_end_length,left_ne_point.y - left_upper_end_offset,z)
      center_ne_point = OpenStudio::Point3d.new(length - right_end_length,center_nw_point.y,z)
      center_se_point = OpenStudio::Point3d.new(length - right_end_length,center_nw_point.y - center_width,z)
      center_sw_point = OpenStudio::Point3d.new(left_end_length,center_se_point.y,z)
      right_nw_point = OpenStudio::Point3d.new(length - right_end_length,center_ne_point.y + right_upper_end_offset,z)
      right_ne_point = OpenStudio::Point3d.new(length,right_nw_point.y,z)
      right_se_point = OpenStudio::Point3d.new(length,right_ne_point.y-right_width,z)
      right_sw_point = OpenStudio::Point3d.new(length - right_end_length,right_se_point.y,z)
      
      # Identity matrix for setting space origins
      m = OpenStudio::Matrix.new(4,4,0)
        m[0,0] = 1
        m[1,1] = 1
        m[2,2] = 1
        m[3,3] = 1
      
      # Define polygons for a L-shape building with perimeter core zoning
      if perimeter_zone_depth > 0
        perimeter_left_nw_point = left_nw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_left_ne_point = left_ne_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_left_se_point = left_se_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_left_sw_point = left_sw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_center_nw_point = center_nw_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_center_ne_point = center_ne_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_center_se_point = center_se_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_center_sw_point = center_sw_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_right_nw_point = right_nw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_right_ne_point = right_ne_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        perimeter_right_se_point = right_se_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        perimeter_right_sw_point = right_sw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
        
        west_left_perimeter_polygon = OpenStudio::Point3dVector.new
          west_left_perimeter_polygon << left_sw_point
          west_left_perimeter_polygon << left_nw_point
          west_left_perimeter_polygon << perimeter_left_nw_point
          west_left_perimeter_polygon << perimeter_left_sw_point
        west_left_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(west_left_perimeter_polygon, floor_to_floor_height, model)
        west_left_perimeter_space = west_left_perimeter_space.get
        m[0,3] = left_sw_point.x
        m[1,3] = left_sw_point.y
        m[2,3] = left_sw_point.z
        west_left_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_left_perimeter_space.setBuildingStory(story)
        west_left_perimeter_space.setName("Story #{floor+1} West Left Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        north_left_perimeter_polygon = OpenStudio::Point3dVector.new
          north_left_perimeter_polygon << left_nw_point
          north_left_perimeter_polygon << left_ne_point
          north_left_perimeter_polygon << perimeter_left_ne_point
          north_left_perimeter_polygon << perimeter_left_nw_point
        north_left_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_left_perimeter_polygon, floor_to_floor_height, model)
        north_left_perimeter_space = north_left_perimeter_space.get
        m[0,3] = perimeter_left_nw_point.x
        m[1,3] = perimeter_left_nw_point.y
        m[2,3] = perimeter_left_nw_point.z
        north_left_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_left_perimeter_space.setBuildingStory(story)
        north_left_perimeter_space.setName("Story #{floor+1} North Left Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_upper_left_perimeter_polygon = OpenStudio::Point3dVector.new
          east_upper_left_perimeter_polygon << left_ne_point
          east_upper_left_perimeter_polygon << center_nw_point
          east_upper_left_perimeter_polygon << perimeter_center_nw_point
          east_upper_left_perimeter_polygon << perimeter_left_ne_point
        east_upper_left_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_upper_left_perimeter_polygon, floor_to_floor_height, model)
        east_upper_left_perimeter_space = east_upper_left_perimeter_space.get
        m[0,3] = perimeter_center_nw_point.x
        m[1,3] = perimeter_center_nw_point.y
        m[2,3] = perimeter_center_nw_point.z
        east_upper_left_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_upper_left_perimeter_space.setBuildingStory(story)
        east_upper_left_perimeter_space.setName("Story #{floor+1} East Upper Left Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        north_center_perimeter_polygon = OpenStudio::Point3dVector.new
          north_center_perimeter_polygon << center_nw_point
          north_center_perimeter_polygon << center_ne_point
          north_center_perimeter_polygon << perimeter_center_ne_point
          north_center_perimeter_polygon << perimeter_center_nw_point
        north_center_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_center_perimeter_polygon, floor_to_floor_height, model)
        north_center_perimeter_space = north_center_perimeter_space.get
        m[0,3] = perimeter_center_nw_point.x
        m[1,3] = perimeter_center_nw_point.y
        m[2,3] = perimeter_center_nw_point.z
        north_center_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_center_perimeter_space.setBuildingStory(story)
        north_center_perimeter_space.setName("Story #{floor+1} North Center Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        west_upper_right_perimeter_polygon = OpenStudio::Point3dVector.new
          west_upper_right_perimeter_polygon << center_ne_point
          west_upper_right_perimeter_polygon << right_nw_point
          west_upper_right_perimeter_polygon << perimeter_right_nw_point
          west_upper_right_perimeter_polygon << perimeter_center_ne_point
        west_upper_right_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(west_upper_right_perimeter_polygon, floor_to_floor_height, model)
        west_upper_right_perimeter_space = west_upper_right_perimeter_space.get
        m[0,3] = center_ne_point.x
        m[1,3] = center_ne_point.y
        m[2,3] = center_ne_point.z
        west_upper_right_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_upper_right_perimeter_space.setBuildingStory(story)
        west_upper_right_perimeter_space.setName("Story #{floor+1} West Upper Right Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        north_right_perimeter_polygon = OpenStudio::Point3dVector.new
          north_right_perimeter_polygon << right_nw_point
          north_right_perimeter_polygon << right_ne_point
          north_right_perimeter_polygon << perimeter_right_ne_point
          north_right_perimeter_polygon << perimeter_right_nw_point
        north_right_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_right_perimeter_polygon, floor_to_floor_height, model)
        north_right_perimeter_space = north_right_perimeter_space.get
        m[0,3] = perimeter_right_nw_point.x
        m[1,3] = perimeter_right_nw_point.y
        m[2,3] = perimeter_right_nw_point.z
        north_right_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_right_perimeter_space.setBuildingStory(story)
        north_right_perimeter_space.setName("Story #{floor+1} North Right Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_right_perimeter_polygon = OpenStudio::Point3dVector.new
          east_right_perimeter_polygon << right_ne_point
          east_right_perimeter_polygon << right_se_point
          east_right_perimeter_polygon << perimeter_right_se_point
          east_right_perimeter_polygon << perimeter_right_ne_point
        east_right_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_right_perimeter_polygon, floor_to_floor_height, model)
        east_right_perimeter_space = east_right_perimeter_space.get
        m[0,3] = perimeter_right_se_point.x
        m[1,3] = perimeter_right_se_point.y
        m[2,3] = perimeter_right_se_point.z
        east_right_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_right_perimeter_space.setBuildingStory(story)
        east_right_perimeter_space.setName("Story #{floor+1} East Right Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        south_right_perimeter_polygon = OpenStudio::Point3dVector.new
          south_right_perimeter_polygon << right_se_point
          south_right_perimeter_polygon << right_sw_point
          south_right_perimeter_polygon << perimeter_right_sw_point
          south_right_perimeter_polygon << perimeter_right_se_point
        south_right_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(south_right_perimeter_polygon, floor_to_floor_height, model)
        south_right_perimeter_space = south_right_perimeter_space.get
        m[0,3] = right_sw_point.x
        m[1,3] = right_sw_point.y
        m[2,3] = right_sw_point.z
        south_right_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_right_perimeter_space.setBuildingStory(story)
        south_right_perimeter_space.setName("Story #{floor+1} South Right Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        west_lower_right_perimeter_polygon = OpenStudio::Point3dVector.new
          west_lower_right_perimeter_polygon << right_sw_point
          west_lower_right_perimeter_polygon << center_se_point
          west_lower_right_perimeter_polygon << perimeter_center_se_point
          west_lower_right_perimeter_polygon << perimeter_right_sw_point
        west_lower_right_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(west_lower_right_perimeter_polygon, floor_to_floor_height, model)
        west_lower_right_perimeter_space = west_lower_right_perimeter_space.get
        m[0,3] = right_sw_point.x
        m[1,3] = right_sw_point.y
        m[2,3] = right_sw_point.z
        west_lower_right_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_lower_right_perimeter_space.setBuildingStory(story)
        west_lower_right_perimeter_space.setName("Story #{floor+1} West Lower Right Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        south_center_perimeter_polygon = OpenStudio::Point3dVector.new
          south_center_perimeter_polygon << center_se_point
          south_center_perimeter_polygon << center_sw_point
          south_center_perimeter_polygon << perimeter_center_sw_point
          south_center_perimeter_polygon << perimeter_center_se_point
        south_center_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(south_center_perimeter_polygon, floor_to_floor_height, model)
        south_center_perimeter_space = south_center_perimeter_space.get
        m[0,3] = center_sw_point.x
        m[1,3] = center_sw_point.y
        m[2,3] = center_sw_point.z
        south_center_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_center_perimeter_space.setBuildingStory(story)
        south_center_perimeter_space.setName("Story #{floor+1} South Center Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_lower_left_perimeter_polygon = OpenStudio::Point3dVector.new
          east_lower_left_perimeter_polygon << center_sw_point
          east_lower_left_perimeter_polygon << left_se_point
          east_lower_left_perimeter_polygon << perimeter_left_se_point
          east_lower_left_perimeter_polygon << perimeter_center_sw_point
        east_lower_left_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_lower_left_perimeter_polygon, floor_to_floor_height, model)
        east_lower_left_perimeter_space = east_lower_left_perimeter_space.get
        m[0,3] = perimeter_left_se_point.x
        m[1,3] = perimeter_left_se_point.y
        m[2,3] = perimeter_left_se_point.z
        east_lower_left_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_lower_left_perimeter_space.setBuildingStory(story)
        east_lower_left_perimeter_space.setName("Story #{floor+1} East Lower Left Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        south_left_perimeter_polygon = OpenStudio::Point3dVector.new
          south_left_perimeter_polygon << left_se_point
          south_left_perimeter_polygon << left_sw_point
          south_left_perimeter_polygon << perimeter_left_sw_point
          south_left_perimeter_polygon << perimeter_left_se_point
        south_left_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(south_left_perimeter_polygon, floor_to_floor_height, model)
        south_left_perimeter_space = south_left_perimeter_space.get
        m[0,3] = left_sw_point.x
        m[1,3] = left_sw_point.y
        m[2,3] = left_sw_point.z
        south_left_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_left_perimeter_space.setBuildingStory(story)
        south_left_perimeter_space.setName("Story #{floor+1} South Left Perimeter Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        west_core_polygon = OpenStudio::Point3dVector.new
          west_core_polygon << perimeter_left_sw_point
          west_core_polygon << perimeter_left_nw_point
          west_core_polygon << perimeter_left_ne_point
          west_core_polygon << perimeter_center_nw_point
          west_core_polygon << perimeter_center_sw_point
          west_core_polygon << perimeter_left_se_point
        west_core_space = OpenStudio::Model::Space::fromFloorPrint(west_core_polygon, floor_to_floor_height, model)
        west_core_space = west_core_space.get
        m[0,3] = perimeter_left_sw_point.x
        m[1,3] = perimeter_left_sw_point.y
        m[2,3] = perimeter_left_sw_point.z
        west_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_core_space.setBuildingStory(story)
        west_core_space.setName("Story #{floor+1} West Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        center_core_polygon = OpenStudio::Point3dVector.new
          center_core_polygon << perimeter_center_sw_point
          center_core_polygon << perimeter_center_nw_point
          center_core_polygon << perimeter_center_ne_point
          center_core_polygon << perimeter_center_se_point
        center_core_space = OpenStudio::Model::Space::fromFloorPrint(center_core_polygon, floor_to_floor_height, model)
        center_core_space = center_core_space.get
        m[0,3] = perimeter_center_sw_point.x
        m[1,3] = perimeter_center_sw_point.y
        m[2,3] = perimeter_center_sw_point.z
        center_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        center_core_space.setBuildingStory(story)
        center_core_space.setName("Story #{floor+1} Center Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_core_polygon = OpenStudio::Point3dVector.new
          east_core_polygon << perimeter_right_sw_point
          east_core_polygon << perimeter_center_se_point
          east_core_polygon << perimeter_center_ne_point
          east_core_polygon << perimeter_right_nw_point
          east_core_polygon << perimeter_right_ne_point
          east_core_polygon << perimeter_right_se_point
        east_core_space = OpenStudio::Model::Space::fromFloorPrint(east_core_polygon, floor_to_floor_height, model)
        east_core_space = east_core_space.get
        m[0,3] = perimeter_right_sw_point.x
        m[1,3] = perimeter_right_sw_point.y
        m[2,3] = perimeter_right_sw_point.z
        east_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_core_space.setBuildingStory(story)
        east_core_space.setName("Story #{floor+1} East Core Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
      # Minimal zones
      else
        west_polygon = OpenStudio::Point3dVector.new
          west_polygon << left_sw_point
          west_polygon << left_nw_point
          west_polygon << left_ne_point
          west_polygon << center_nw_point
          west_polygon << center_sw_point
          west_polygon << left_se_point
        west_space = OpenStudio::Model::Space::fromFloorPrint(west_polygon, floor_to_floor_height, model)
        west_space = west_space.get
        m[0,3] = left_sw_point.x
        m[1,3] = left_sw_point.y
        m[2,3] = left_sw_point.z
        west_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_space.setBuildingStory(story)
        west_space.setName("Story #{floor+1} West Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        center_polygon = OpenStudio::Point3dVector.new
          center_polygon << center_sw_point
          center_polygon << center_nw_point
          center_polygon << center_ne_point
          center_polygon << center_se_point
        center_space = OpenStudio::Model::Space::fromFloorPrint(center_polygon, floor_to_floor_height, model)
        center_space = center_space.get
        m[0,3] = center_sw_point.x
        m[1,3] = center_sw_point.y
        m[2,3] = center_sw_point.z
        center_space.changeTransformation(OpenStudio::Transformation.new(m))
        center_space.setBuildingStory(story)
        center_space.setName("Story #{floor+1} Center Space")
        
        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)
        
        east_polygon = OpenStudio::Point3dVector.new
          east_polygon << right_sw_point
          east_polygon << center_se_point
          east_polygon << center_ne_point
          east_polygon << right_nw_point
          east_polygon << right_ne_point
          east_polygon << right_se_point
        east_space = OpenStudio::Model::Space::fromFloorPrint(east_polygon, floor_to_floor_height, model)
        east_space = east_space.get
        m[0,3] = right_sw_point.x
        m[1,3] = right_sw_point.y
        m[2,3] = right_sw_point.z
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

SetHShapeFloorPlan.new.registerWithApplication
