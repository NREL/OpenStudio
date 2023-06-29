########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
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
