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
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require_relative 'model.rb'
require 'csv'


class Perimeter_Test < Minitest::Test
    def test_Perimeter

        puts "initiating Perimeter Test"

        @osm_name =  "floorplan_school.osm"
        @osm_dir = File.expand_path('../../.') + "/build/resources/model"
        # puts File.expand_path('../../.') + "/build/resources/model"
        # OpenStudio::Path.new(File.dirname(__FILE__) + "/output/Perimeter.log")

        tol = 1.0E-5

        #load model
        osm_path = File.join(@osm_dir, @osm_name)
        puts osm_path
        starting_Time = Time.now
        puts "Started at : #{starting_Time}"
        #model = ModelFile.load_model(osm_path)
        model = ModelFile.load_model_NoTranslator(osm_path)
        # model = load_model_NoTranslator(osm_path)
        # model = OpenStudio::Model::Model::load(osm_path)
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
    # def load_model_NoTranslator(osm_path)
    #     model_path = OpenStudio::Path.new(osm_path)
    #     #version_translator = OpenStudio::OSVersion::VersionTranslator.new
    #     model1 = OpenStudio::Model::Model
    #     if OpenStudio::exists(model_path)
    #       model = model1.load(model_path)
    #       if model.empty?
    #         puts "Model Load of #{model_path} failed."
    #         exists
    #             else
    #                 model = model.get
    #                 return model
    #             end
    #         else 
    #             puts "Model could not be found at #{model_path}."
    #             exit
    #     end 
    # end
end
