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
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require 'csv'

# joinAll fails on cases with an inner loop
# joinAll method that takes a list of polygons and returns a list of polygons
# Linux, Mac OS need additional review
class Polygon3d_JoinAll_1614_Test < Minitest::Test
  def test_Polygon3d_JoinAll_1614
    tol = 0.01

    #std::vector<Polygon3d>
    polygons = OpenStudio::Polygon3dVector.new

    poly1 = OpenStudio::Polygon3d.new
    poly1.addPoint(OpenStudio::Point3d.new(0, 70.0, 0))
    poly1.addPoint(OpenStudio::Point3d.new(0, 90.0, 0))
    poly1.addPoint(OpenStudio::Point3d.new(100.0, 90.0, 0))
    poly1.addPoint(OpenStudio::Point3d.new(100.0, 70.0, 0))

    poly2 = OpenStudio::Polygon3d.new
    poly2.addPoint(OpenStudio::Point3d.new(0, 10.0, 0))
    poly2.addPoint(OpenStudio::Point3d.new(0, 30.0, 0))
    poly2.addPoint(OpenStudio::Point3d.new(100.0, 30.0, 0))
    poly2.addPoint(OpenStudio::Point3d.new(100.0, 10.0, 0))

    poly3 = OpenStudio::Polygon3d.new
    poly3.addPoint(OpenStudio::Point3d.new(10.00, 0, 0))
    poly3.addPoint(OpenStudio::Point3d.new(10.0, 100.0, 0))
    poly3.addPoint(OpenStudio::Point3d.new(30.0, 100.0, 0))
    poly3.addPoint(OpenStudio::Point3d.new(30.0, 0, 0))

    poly4 = OpenStudio::Polygon3d.new
    poly4.addPoint(OpenStudio::Point3d.new(70.00, 0, 0))
    poly4.addPoint(OpenStudio::Point3d.new(70.00, 100.0, 0))
    poly4.addPoint(OpenStudio::Point3d.new(90.00, 100.0, 0))
    poly4.addPoint(OpenStudio::Point3d.new(90.00, 0, 0))

    polygons.push(poly1)
    polygons.push(poly4)
    polygons.push(poly2)
    polygons.push(poly3)

    result = OpenStudio::joinAll(polygons, tol)

    # Should return one polygon
    assert_equal(1, result.size)

    # The outer polygon should have 28 points
    assert_equal(28, result[0].getOuterPath.size)


    testPolygon = OpenStudio::Polygon3d.new
    testPolygon.addPoint(OpenStudio::Point3d.new(10.0, 10.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 10.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 30.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(10.0, 30.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(10.0, 70.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 70.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 90, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(10.0, 90, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(10.0, 100.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(30.0, 100.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(30.0, 90, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(70.0, 90, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(70.0, 100.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(90, 100.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(90, 90, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100.0, 90, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100.0, 70.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(90, 70.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(90, 30.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100.0, 30.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100.0, 10.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(90, 10.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(90, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(70.0, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(70.0, 10.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(30.0, 10.0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(30.0, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(10.0, 0, 0))

    testHole = OpenStudio::Point3dVector.new
    testHole.push(OpenStudio::Point3d.new(30.0, 30.0, 0))
    testHole.push(OpenStudio::Point3d.new(70.0, 30.0, 0))
    testHole.push(OpenStudio::Point3d.new(70.0, 70.0, 0))
    testHole.push(OpenStudio::Point3d.new(30.0, 70.0, 0))
    testPolygon.addHole(testHole)
    b1 = OpenStudio::circularEqual(result[0].getOuterPath, testPolygon.getOuterPath, 0.01)
    assert(b1)

    # The polygon should have one hole with 4 points
    assert_equal(1, result.first.getInnerPaths.size)
    assert_equal(4, result.first.getInnerPaths[0].size)

    b2 = OpenStudio::circularEqual(result[0].getInnerPaths[0], testPolygon.getInnerPaths[0], 0.01)
    assert(b2)

    grossArea = result.first.grossArea
    assert_in_delta(8000, grossArea, 0.01)

    netArea = result.first.netArea
    assert_in_delta(6400, netArea, 0.001)
    assert(grossArea > netArea)

    holeArea = OpenStudio::getArea(result.first.getInnerPaths.first)
    holeArea = holeArea.get
    assert_equal(grossArea, netArea + holeArea)

    # perimeter = result.first.getPerimeter
  end
end
