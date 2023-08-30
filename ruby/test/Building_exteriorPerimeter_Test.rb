########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require_relative 'model.rb'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require 'csv'

class Building_exteriorPerimeter_Test < Minitest::Test
    def test_Building_exteriorPerimeter

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
        buildings = model.getBuilding
        
        # buildings.each do |building|
        perimeter = buildings.exteriorPerimeter
        assert_in_delta(1428.0,perimeter,0.01)
        #end
    end
end
