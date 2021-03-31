########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

        #load model
        osm_path = File.join(File.expand_path('../../.') + "/build/resources/model/","floorplan_school.osm")
        puts osm_path
        starting_Time = Time.now
        puts "Started at : #{starting_Time}"
        # model = ModelFile.load_model(osm_path)
        model = ModelFile.load_model_NoTranslator(osm_path)
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

