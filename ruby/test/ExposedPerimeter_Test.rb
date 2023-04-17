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
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
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

class ExposedPerimeter_Test < Minitest::Test
    def test_ExposedPerimeter
        puts "initiating Exposed Perimeter Test"

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
        surfaces = OpenStudio::Model::SurfaceVector.new

        # Iterate over spaces, get their surfaces and transform to model coordinates,
        # find the surfaces with downward facing normal at elevation of 0
        model.getSpaces.each do |space|
            spaceTransformation = space.transformation
            spacename = space.name.get
            # puts spacename
            space.surfaces.each do |surface|
                added = false
                points = spaceTransformation * surface.vertices
                normal = OpenStudio::getOutwardNormal(points)
                if normal
                    if normal.get.z < 0 
                        if points[0].z == 0 
                            polygons.push(points)
                            spaces.push(space)
                            if !added
                                added = true
                                surfaces.push(surface)
                            end
                        end
                    end
                end
            end
        end
        assert_equal(spaces.size, 20)

        # Join all those Polygons into one
        result2 = OpenStudio::joinAllPolygons(polygons,0.01)
        assert_equal(1, result2.size)
        assert_equal(12, result2[0].getOuterPath.size)
        footprint = result2.first

        surfaces.each do |surface|
            surfname = surface.name.get
            exposedPerimeter = 0
            nOverlaps = 0
            space = surface.space
            if !space 
                next
            end
            space = space.get
            spacename = space.name.get
            spaceTransformation = space.transformation
            points = spaceTransformation * surface.vertices

            # Find points/edges on the perimeter that points on the surface are coincident with
            for a in 0..points.size-1 do
                p1 = points[a]
                p2 = points[(a+1)%points.size]
                line = OpenStudio::Point3dVector.new
                line.push(p1)
                line.push(p2)
                overlaps = footprint.overlap(line)
                overlaps.each do |overlap|
                    exposedPerimeter += OpenStudio::getDistance(overlap[0], overlap[1])
                    nOverlaps = nOverlaps + 1
                end            
            end

            # Get the space perimeter from the space and compare
            spacePerimeter = space.exposedPerimeter(footprint)
            assert_equal(exposedPerimeter, spacePerimeter)

            if spacename == "Artroom 103" 
                assert_equal(nOverlaps, 1)
                assert_in_delta(60, exposedPerimeter, 0.01)
            elsif spacename == "Bathroom 115"
                assert_equal(nOverlaps, 1)
                assert_in_delta(32, exposedPerimeter, 0.01)
            elsif spacename == "Classroom 101" || spacename == "Classroom 105" || spacename == "Classroom 109" || spacename == "Classroom 111"
                assert_equal(nOverlaps, 2)
                assert_in_delta(66, exposedPerimeter, 0.01)
            elsif spacename == "Classroom 102" || spacename == "Lobby 113"
                assert_equal(nOverlaps, 1)
                assert_in_delta(62, exposedPerimeter, 0.01)
            elsif spacename == "Classroom 106" || spacename == "Classroom 108" || spacename == "Classroom 112"
                assert_equal(nOverlaps, 1)
                assert_in_delta(122, exposedPerimeter, 0.01)
            elsif spacename == "Gym 118"
                assert_equal(nOverlaps, 1)
                assert_in_delta(54, exposedPerimeter, 0.01)
            elsif spacename == "Kitchen 119"
                assert_equal(nOverlaps, 1)
                assert_in_delta(26, exposedPerimeter, 0.01)
            elsif spacename == "Mechanical 114"
                assert_equal(nOverlaps, 0)
                assert_in_delta(0, exposedPerimeter, 0.01)
            elsif spacename == "Media Center 116"
                assert_equal(nOverlaps, 3)
                assert_in_delta(198, exposedPerimeter, 0.01)
            elsif spacename == "Offices 117"
                assert_equal(nOverlaps, 2)
                assert_in_delta(138, exposedPerimeter, 0.01)
            elsif spacename == "Corridor 104" || spacename == "Corridor 110"
                assert_equal(nOverlaps, 1)
                assert_in_delta(10, exposedPerimeter, 0.01)
            elsif spacename == "Corridor 107"
                assert_equal(nOverlaps, 1)
                assert_in_delta(28, exposedPerimeter, 0.01)
            elsif spacename == "Cafeteria 120" 
                assert_equal(nOverlaps, 2)
                assert_in_delta(118, exposedPerimeter, 0.01)
            else 
                # Makes sure we've checked all 20 spaces and got the names right
                assert(false);
            end
        end
    end
end

