########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require_relative 'model.rb'
require 'csv'

class Perimeter_Test < Minitest::Test
    def test_Perimeter

        puts "initiating Perimeter Test"

        tol = 1.0E-5
        model = OpenStudio::Model::Model.new
        tz1 = OpenStudio::Model::ThermalZone.new(model)
        story1 = OpenStudio::Model::BuildingStory.new(model)  
        p1 = OpenStudio::Point3dVector.new

  
        p1.push(OpenStudio::Point3d.new(36, 168, 0))
        p1.push(OpenStudio::Point3d.new(36, 138, 0))
        p1.push(OpenStudio::Point3d.new(0, 138, 0))
        p1.push(OpenStudio::Point3d.new(0, 168, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 101")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(220, 168, 0))
        p1.push(OpenStudio::Point3d.new(220, 150, 0))
        p1.push(OpenStudio::Point3d.new(220, 124, 0))
        p1.push(OpenStudio::Point3d.new(220, 70, 0))
        p1.push(OpenStudio::Point3d.new(220, 30, 0))
        p1.push(OpenStudio::Point3d.new(200, 30, 0))
        p1.push(OpenStudio::Point3d.new(200, 168, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Mechanical 114")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(288, 70, 0))
        p1.push(OpenStudio::Point3d.new(288, 0, 0))
        p1.push(OpenStudio::Point3d.new(220, 0, 0))
        p1.push(OpenStudio::Point3d.new(220, 30, 0))
        p1.push(OpenStudio::Point3d.new(220, 70, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Offices 117")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(288, 124, 0))
        p1.push(OpenStudio::Point3d.new(288, 70, 0))
        p1.push(OpenStudio::Point3d.new(220, 70, 0))
        p1.push(OpenStudio::Point3d.new(220, 124, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Gym 118")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 168, 0))
        p1.push(OpenStudio::Point3d.new(158, 138, 0))
        p1.push(OpenStudio::Point3d.new(98, 138, 0))
        p1.push(OpenStudio::Point3d.new(98, 168, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Artroom 103")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(288, 150, 0))
        p1.push(OpenStudio::Point3d.new(288, 124, 0))
        p1.push(OpenStudio::Point3d.new(220, 124, 0))
        p1.push(OpenStudio::Point3d.new(220, 150, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Kitchen 119")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(288, 200, 0))
        p1.push(OpenStudio::Point3d.new(288, 150, 0))
        p1.push(OpenStudio::Point3d.new(220, 150, 0))
        p1.push(OpenStudio::Point3d.new(220, 168, 0))
        p1.push(OpenStudio::Point3d.new(220, 200, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Cafeteria 120")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 138, 0))
        p1.push(OpenStudio::Point3d.new(158, 128, 0))
        p1.push(OpenStudio::Point3d.new(36, 128, 0))
        p1.push(OpenStudio::Point3d.new(0, 128, 0))
        p1.push(OpenStudio::Point3d.new(0, 138, 0))
        p1.push(OpenStudio::Point3d.new(36, 138, 0))
        p1.push(OpenStudio::Point3d.new(98, 138, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Corridor 104")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(36, 128, 0))
        p1.push(OpenStudio::Point3d.new(36, 98, 0))
        p1.push(OpenStudio::Point3d.new(0, 98, 0))
        p1.push(OpenStudio::Point3d.new(0, 128, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 105")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 128, 0))
        p1.push(OpenStudio::Point3d.new(158, 98, 0))
        p1.push(OpenStudio::Point3d.new(36, 98, 0))
        p1.push(OpenStudio::Point3d.new(36, 128, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 106")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 30, 0))
        p1.push(OpenStudio::Point3d.new(158, 40, 0))
        p1.push(OpenStudio::Point3d.new(158, 70, 0))
        p1.push(OpenStudio::Point3d.new(158, 98, 0))
        p1.push(OpenStudio::Point3d.new(158, 128, 0))
        p1.push(OpenStudio::Point3d.new(158, 138, 0))
        p1.push(OpenStudio::Point3d.new(158, 168, 0))
        p1.push(OpenStudio::Point3d.new(200, 168, 0))
        p1.push(OpenStudio::Point3d.new(200, 30, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Corridor 107")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(36, 40, 0))
        p1.push(OpenStudio::Point3d.new(36, 70, 0))
        p1.push(OpenStudio::Point3d.new(158, 70, 0))
        p1.push(OpenStudio::Point3d.new(158, 40, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 108")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(0, 40, 0))
        p1.push(OpenStudio::Point3d.new(0, 70, 0))
        p1.push(OpenStudio::Point3d.new(36, 70, 0))
        p1.push(OpenStudio::Point3d.new(36, 40, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 109")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(98, 168, 0))
        p1.push(OpenStudio::Point3d.new(98, 138, 0))
        p1.push(OpenStudio::Point3d.new(36, 138, 0))
        p1.push(OpenStudio::Point3d.new(36, 168, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 102")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 40, 0))
        p1.push(OpenStudio::Point3d.new(158, 30, 0))
        p1.push(OpenStudio::Point3d.new(36, 30, 0))
        p1.push(OpenStudio::Point3d.new(0, 30, 0))
        p1.push(OpenStudio::Point3d.new(0, 40, 0))
        p1.push(OpenStudio::Point3d.new(36, 40, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Corridor 110")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(36, 30, 0))
        p1.push(OpenStudio::Point3d.new(36, 0, 0))
        p1.push(OpenStudio::Point3d.new(0, 0, 0))
        p1.push(OpenStudio::Point3d.new(0, 30, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 111")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 30, 0))
        p1.push(OpenStudio::Point3d.new(158, 0, 0))
        p1.push(OpenStudio::Point3d.new(36, 0, 0))
        p1.push(OpenStudio::Point3d.new(36, 30, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Classroom 112")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(158, 0, 0))
        p1.push(OpenStudio::Point3d.new(158, 30, 0))
        p1.push(OpenStudio::Point3d.new(200, 30, 0))
        p1.push(OpenStudio::Point3d.new(220, 30, 0))
        p1.push(OpenStudio::Point3d.new(220, 0, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Lobby 113")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(220, 200, 0))
        p1.push(OpenStudio::Point3d.new(220, 168, 0))
        p1.push(OpenStudio::Point3d.new(200, 168, 0))
        p1.push(OpenStudio::Point3d.new(158, 168, 0))
        p1.push(OpenStudio::Point3d.new(158, 200, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Bathroom 115")

        p1.clear()
        p1.push(OpenStudio::Point3d.new(220, 268, 0))
        p1.push(OpenStudio::Point3d.new(220, 200, 0))
        p1.push(OpenStudio::Point3d.new(158, 200, 0))
        p1.push(OpenStudio::Point3d.new(158, 268, 0))
        sp1 = OpenStudio::Model::Space::fromFloorPrint(p1, 13.125, model)
        sp1 = sp1.get
        sp1.setBuildingStory(story1)
        sp1.setThermalZone(tz1)
        sp1.setName("Media Center 116")




        assert(model)

        polygons = Array.new #OpenStudio::Point3dVector.new 
        spaces = OpenStudio::Model::SpaceVector.new
        spaceNames = Array.new
        surfaces = OpenStudio::Model::SurfaceVector.new

        model.getSpaces.each do |space|
            spacename = space.name.get
            #puts spacename
            spaceTransformation = space.transformation
            space.surfaces.each do |surface|
                surfname = surface.name.get
                points = spaceTransformation * surface.vertices
                normal = OpenStudio::getOutwardNormal(points)
                if normal 
                    if normal.get.z == -1 
                        if  points[0].z == 0 
                            polygons.push(points)
                            spaces.push(space)
                            spaceNames.push(spacename)
                        end
                    end
                end
            end
        end

        # The traditional method
        result1 = OpenStudio::joinAll(polygons, 0.01)
        assert_equal(1, result1.size)
        assert_equal(12, result1[0].size)
      
        # Using polygons
        result2 = OpenStudio::joinAllPolygons(polygons, 0.01)
        assert_equal(1, result2.size)
        assert_equal(12, result2[0].getOuterPath.size)

        # Calculate perimeter
        footprint = result2[0]
        perimeter = footprint.perimeter
        assert_in_delta(1428.0, perimeter, delta = 0.01, msg = nil)

        # # Compare points list to polygon
        assert(OpenStudio::circularEqual(result1.first, footprint.getOuterPath))
        
    end
end
