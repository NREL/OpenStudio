########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF
#  USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
