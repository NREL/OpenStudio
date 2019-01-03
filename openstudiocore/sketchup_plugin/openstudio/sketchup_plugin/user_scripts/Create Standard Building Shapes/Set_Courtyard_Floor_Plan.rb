########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

class SetCourtyardFloorPlan < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Courtyard Floor Plan"
  end

  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("length",false)
    length.setDisplayName("Building Length (m)")
    length.setDefaultValue(50.0)
    result << length

    width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("width",false)
    width.setDisplayName("Building Width (m)")
    width.setDefaultValue(30.0)
    result << width

    courtyard_length = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("courtyard_length",false)
    courtyard_length.setDisplayName("Courtyard Length (m)")
    courtyard_length.setDefaultValue(15.0)
    result << courtyard_length

    courtyard_width = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("courtyard_width",false)
    courtyard_width.setDisplayName("Courtyard Width (m)")
    courtyard_width.setDefaultValue(5.0)
    result << courtyard_width

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
    courtyard_length = runner.getDoubleArgumentValue("courtyard_length",user_arguments)
    courtyard_width = runner.getDoubleArgumentValue("courtyard_width",user_arguments)
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

    if courtyard_length <= 1e-4
      runner.registerError("Courtyard length must be greater than 0.")
      return false
    end

    if courtyard_width <= 1e-4
      runner.registerError("Courtyard width must be greater than 0.")
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
    if perimeter_zone_depth < 0 or 4*perimeter_zone_depth >= (shortest_side - 1e-4)
      runner.registerError("Perimeter zone depth must be greater than or equal to 0 and less than #{shortest_side/4}m.")
      return false
    end

    if courtyard_length >= (length - 4*perimeter_zone_depth - 1e-4)
      runner.registerError("Courtyard length must be less than #{length - 4*perimeter_zone_depth}m.")
      return false
    end

    if courtyard_width >= (width - 4*perimeter_zone_depth - 1e-4)
      runner.registerError("Courtyard width must be less than #{width - 4*perimeter_zone_depth}m.")
      return false
    end

    # Create progress bar
    runner.createProgressBar("Creating Spaces")
    num_total = perimeter_zone_depth>0 ? num_floors*12 : num_floors*4
    num_complete = 0

    # Loop through the number of floors
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

      courtyard_nw_point = OpenStudio::Point3d.new((length-courtyard_length)/2,(width-courtyard_width)/2+courtyard_width,z)
      courtyard_ne_point = OpenStudio::Point3d.new((length-courtyard_length)/2+courtyard_length,(width-courtyard_width)/2+courtyard_width,z)
      courtyard_se_point = OpenStudio::Point3d.new((length-courtyard_length)/2+courtyard_length,(width-courtyard_width)/2,z)
      courtyard_sw_point = OpenStudio::Point3d.new((length-courtyard_length)/2,(width-courtyard_width)/2,z)

      # Identity matrix for setting space origins
      m = OpenStudio::Matrix.new(4,4,0)
        m[0,0] = 1
        m[1,1] = 1
        m[2,2] = 1
        m[3,3] = 1

      # Define polygons for a building with a courtyard
      if perimeter_zone_depth > 0
        outer_perimeter_nw_point = nw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        outer_perimeter_ne_point = ne_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)
        outer_perimeter_se_point = se_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        outer_perimeter_sw_point = sw_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
        inner_perimeter_nw_point = courtyard_nw_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,perimeter_zone_depth,0)
        inner_perimeter_ne_point = courtyard_ne_point + OpenStudio::Vector3d.new(perimeter_zone_depth,perimeter_zone_depth,0)
        inner_perimeter_se_point = courtyard_se_point + OpenStudio::Vector3d.new(perimeter_zone_depth,-perimeter_zone_depth,0)
        inner_perimeter_sw_point = courtyard_sw_point + OpenStudio::Vector3d.new(-perimeter_zone_depth,-perimeter_zone_depth,0)

        west_outer_perimeter_polygon = OpenStudio::Point3dVector.new
          west_outer_perimeter_polygon << sw_point
          west_outer_perimeter_polygon << nw_point
          west_outer_perimeter_polygon << outer_perimeter_nw_point
          west_outer_perimeter_polygon << outer_perimeter_sw_point
        west_outer_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(west_outer_perimeter_polygon, floor_to_floor_height, model)
        west_outer_perimeter_space = west_outer_perimeter_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        west_outer_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_outer_perimeter_space.setBuildingStory(story)
        west_outer_perimeter_space.setName("Story #{floor+1} West Outer Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        north_outer_perimeter_polygon = OpenStudio::Point3dVector.new
          north_outer_perimeter_polygon << nw_point
          north_outer_perimeter_polygon << ne_point
          north_outer_perimeter_polygon << outer_perimeter_ne_point
          north_outer_perimeter_polygon << outer_perimeter_nw_point
        north_outer_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_outer_perimeter_polygon, floor_to_floor_height, model)
        north_outer_perimeter_space = north_outer_perimeter_space.get
        m[0,3] = outer_perimeter_nw_point.x
        m[1,3] = outer_perimeter_nw_point.y
        m[2,3] = outer_perimeter_nw_point.z
        north_outer_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_outer_perimeter_space.setBuildingStory(story)
        north_outer_perimeter_space.setName("Story #{floor+1} North Outer Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        east_outer_perimeter_polygon = OpenStudio::Point3dVector.new
          east_outer_perimeter_polygon << ne_point
          east_outer_perimeter_polygon << se_point
          east_outer_perimeter_polygon << outer_perimeter_se_point
          east_outer_perimeter_polygon << outer_perimeter_ne_point
        east_outer_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_outer_perimeter_polygon, floor_to_floor_height, model)
        east_outer_perimeter_space = east_outer_perimeter_space.get
        m[0,3] = outer_perimeter_se_point.x
        m[1,3] = outer_perimeter_se_point.y
        m[2,3] = outer_perimeter_se_point.z
        east_outer_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_outer_perimeter_space.setBuildingStory(story)
        east_outer_perimeter_space.setName("Story #{floor+1} East Outer Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        south_outer_perimeter_polygon = OpenStudio::Point3dVector.new
          south_outer_perimeter_polygon << se_point
          south_outer_perimeter_polygon << sw_point
          south_outer_perimeter_polygon << outer_perimeter_sw_point
          south_outer_perimeter_polygon << outer_perimeter_se_point
        south_outer_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(south_outer_perimeter_polygon, floor_to_floor_height, model)
        south_outer_perimeter_space = south_outer_perimeter_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        south_outer_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_outer_perimeter_space.setBuildingStory(story)
        south_outer_perimeter_space.setName("Story #{floor+1} South Outer Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        west_core_polygon = OpenStudio::Point3dVector.new
          west_core_polygon << outer_perimeter_sw_point
          west_core_polygon << outer_perimeter_nw_point
          west_core_polygon << inner_perimeter_nw_point
          west_core_polygon << inner_perimeter_sw_point
        west_core_space = OpenStudio::Model::Space::fromFloorPrint(west_core_polygon, floor_to_floor_height, model)
        west_core_space = west_core_space.get
        m[0,3] = outer_perimeter_sw_point.x
        m[1,3] = outer_perimeter_sw_point.y
        m[2,3] = outer_perimeter_sw_point.z
        west_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_core_space.setBuildingStory(story)
        west_core_space.setName("Story #{floor+1} West Core Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        north_core_polygon = OpenStudio::Point3dVector.new
          north_core_polygon << outer_perimeter_nw_point
          north_core_polygon << outer_perimeter_ne_point
          north_core_polygon << inner_perimeter_ne_point
          north_core_polygon << inner_perimeter_nw_point
        north_core_space = OpenStudio::Model::Space::fromFloorPrint(north_core_polygon, floor_to_floor_height, model)
        north_core_space = north_core_space.get
        m[0,3] = inner_perimeter_nw_point.x
        m[1,3] = inner_perimeter_nw_point.y
        m[2,3] = inner_perimeter_nw_point.z
        north_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_core_space.setBuildingStory(story)
        north_core_space.setName("Story #{floor+1} North Core Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        east_core_polygon = OpenStudio::Point3dVector.new
          east_core_polygon << outer_perimeter_ne_point
          east_core_polygon << outer_perimeter_se_point
          east_core_polygon << inner_perimeter_se_point
          east_core_polygon << inner_perimeter_ne_point
        east_core_space = OpenStudio::Model::Space::fromFloorPrint(east_core_polygon, floor_to_floor_height, model)
        east_core_space = east_core_space.get
        m[0,3] = inner_perimeter_se_point.x
        m[1,3] = inner_perimeter_se_point.y
        m[2,3] = inner_perimeter_se_point.z
        east_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_core_space.setBuildingStory(story)
        east_core_space.setName("Story #{floor+1} East Core Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        south_core_polygon = OpenStudio::Point3dVector.new
          south_core_polygon << outer_perimeter_se_point
          south_core_polygon << outer_perimeter_sw_point
          south_core_polygon << inner_perimeter_sw_point
          south_core_polygon << inner_perimeter_se_point
        south_core_space = OpenStudio::Model::Space::fromFloorPrint(south_core_polygon, floor_to_floor_height, model)
        south_core_space = south_core_space.get
        m[0,3] = outer_perimeter_sw_point.x
        m[1,3] = outer_perimeter_sw_point.y
        m[2,3] = outer_perimeter_sw_point.z
        south_core_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_core_space.setBuildingStory(story)
        south_core_space.setName("Story #{floor+1} South Core Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        west_inner_perimeter_polygon  = OpenStudio::Point3dVector.new
          west_inner_perimeter_polygon << inner_perimeter_sw_point
          west_inner_perimeter_polygon << inner_perimeter_nw_point
          west_inner_perimeter_polygon << courtyard_nw_point
          west_inner_perimeter_polygon << courtyard_sw_point
        west_inner_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(west_inner_perimeter_polygon, floor_to_floor_height, model)
        west_inner_perimeter_space = west_inner_perimeter_space.get
        m[0,3] = inner_perimeter_sw_point.x
        m[1,3] = inner_perimeter_sw_point.y
        m[2,3] = inner_perimeter_sw_point.z
        west_inner_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        west_inner_perimeter_space.setBuildingStory(story)
        west_inner_perimeter_space.setName("Story #{floor+1} West Inner Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        north_inner_perimeter_polygon  = OpenStudio::Point3dVector.new
          north_inner_perimeter_polygon << inner_perimeter_nw_point
          north_inner_perimeter_polygon << inner_perimeter_ne_point
          north_inner_perimeter_polygon << courtyard_ne_point
          north_inner_perimeter_polygon << courtyard_nw_point
        north_inner_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(north_inner_perimeter_polygon, floor_to_floor_height, model)
        north_inner_perimeter_space = north_inner_perimeter_space.get
        m[0,3] = courtyard_nw_point.x
        m[1,3] = courtyard_nw_point.y
        m[2,3] = courtyard_nw_point.z
        north_inner_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_inner_perimeter_space.setBuildingStory(story)
        north_inner_perimeter_space.setName("Story #{floor+1} North Inner Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        east_inner_perimeter_polygon  = OpenStudio::Point3dVector.new
          east_inner_perimeter_polygon << inner_perimeter_ne_point
          east_inner_perimeter_polygon << inner_perimeter_se_point
          east_inner_perimeter_polygon << courtyard_se_point
          east_inner_perimeter_polygon << courtyard_ne_point
        east_inner_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(east_inner_perimeter_polygon, floor_to_floor_height, model)
        east_inner_perimeter_space = east_inner_perimeter_space.get
        m[0,3] = courtyard_se_point.x
        m[1,3] = courtyard_se_point.y
        m[2,3] = courtyard_se_point.z
        east_inner_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_inner_perimeter_space.setBuildingStory(story)
        east_inner_perimeter_space.setName("Story #{floor+1} East Inner Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        south_inner_perimeter_polygon  = OpenStudio::Point3dVector.new
          south_inner_perimeter_polygon << inner_perimeter_se_point
          south_inner_perimeter_polygon << inner_perimeter_sw_point
          south_inner_perimeter_polygon << courtyard_sw_point
          south_inner_perimeter_polygon << courtyard_se_point
        south_inner_perimeter_space = OpenStudio::Model::Space::fromFloorPrint(south_inner_perimeter_polygon, floor_to_floor_height, model)
        south_inner_perimeter_space = south_inner_perimeter_space.get
        m[0,3] = inner_perimeter_sw_point.x
        m[1,3] = inner_perimeter_sw_point.y
        m[2,3] = inner_perimeter_sw_point.z
        south_inner_perimeter_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_inner_perimeter_space.setBuildingStory(story)
        south_inner_perimeter_space.setName("Story #{floor+1} South Inner Perimeter Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

      # Minimal zones
      else
        west_polygon = OpenStudio::Point3dVector.new
          west_polygon << sw_point
          west_polygon << nw_point
          west_polygon << courtyard_nw_point
          west_polygon << courtyard_sw_point
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

        north_polygon = OpenStudio::Point3dVector.new
          north_polygon << nw_point
          north_polygon << ne_point
          north_polygon << courtyard_ne_point
          north_polygon << courtyard_nw_point
        north_space = OpenStudio::Model::Space::fromFloorPrint(north_polygon, floor_to_floor_height, model)
        north_space = north_space.get
        m[0,3] = courtyard_nw_point.x
        m[1,3] = courtyard_nw_point.y
        m[2,3] = courtyard_nw_point.z
        north_space.changeTransformation(OpenStudio::Transformation.new(m))
        north_space.setBuildingStory(story)
        north_space.setName("Story #{floor+1} North Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        east_polygon = OpenStudio::Point3dVector.new
          east_polygon << ne_point
          east_polygon << se_point
          east_polygon << courtyard_se_point
          east_polygon << courtyard_ne_point
        east_space = OpenStudio::Model::Space::fromFloorPrint(east_polygon, floor_to_floor_height, model)
        east_space = east_space.get
        m[0,3] = courtyard_se_point.x
        m[1,3] = courtyard_se_point.y
        m[2,3] = courtyard_se_point.z
        east_space.changeTransformation(OpenStudio::Transformation.new(m))
        east_space.setBuildingStory(story)
        east_space.setName("Story #{floor+1} East Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

        south_polygon = OpenStudio::Point3dVector.new
          south_polygon << se_point
          south_polygon << sw_point
          south_polygon << courtyard_sw_point
          south_polygon << courtyard_se_point
        south_space = OpenStudio::Model::Space::fromFloorPrint(south_polygon, floor_to_floor_height, model)
        south_space = south_space.get
        m[0,3] = sw_point.x
        m[1,3] = sw_point.y
        m[2,3] = sw_point.z
        south_space.changeTransformation(OpenStudio::Transformation.new(m))
        south_space.setBuildingStory(story)
        south_space.setName("Story #{floor+1} South Space")

        num_complete += 1
        runner.updateProgress(100*num_complete/num_total)

      end

      #Set vertical story position
      story.setNominalZCoordinate(z)

    end #End of floor loop

    runner.destroyProgressBar

  end

end

SetCourtyardFloorPlan.new.registerWithApplication
