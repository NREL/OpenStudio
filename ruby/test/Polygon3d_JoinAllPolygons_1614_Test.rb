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

# Linux, Mac OS need additional review
class Polygon3d_JoinAllPolygons_1614_Test < Minitest::Test
  def test_Polygon3d_JoinAllPolygons_1614
    tol = 0.01

    polygons = OpenStudio::Point3dVectorVector.new

    poly1 = OpenStudio::Point3dVector.new
    poly1.push(OpenStudio::Point3d.new(0, 70, 0))
    poly1.push(OpenStudio::Point3d.new(0, 90, 0))
    poly1.push(OpenStudio::Point3d.new(100, 90, 0))
    poly1.push(OpenStudio::Point3d.new(100, 70, 0))

    poly2 = OpenStudio::Point3dVector.new
    poly2.push(OpenStudio::Point3d.new(0, 10, 0))
    poly2.push(OpenStudio::Point3d.new(0, 30, 0))
    poly2.push(OpenStudio::Point3d.new(100, 30, 0))
    poly2.push(OpenStudio::Point3d.new(100, 10, 0))

    poly3 = OpenStudio::Point3dVector.new
    poly3.push(OpenStudio::Point3d.new(10, 0, 0))
    poly3.push(OpenStudio::Point3d.new(10, 100, 0))
    poly3.push(OpenStudio::Point3d.new(30, 100, 0))
    poly3.push(OpenStudio::Point3d.new(30, 0, 0))

    poly4 = OpenStudio::Point3dVector.new
    poly4.push(OpenStudio::Point3d.new(70, 0, 0))
    poly4.push(OpenStudio::Point3d.new(70, 100, 0))
    poly4.push(OpenStudio::Point3d.new(90, 100, 0))
    poly4.push(OpenStudio::Point3d.new(90, 0, 0))

    polygons.push(poly1)
    polygons.push(poly4)
    polygons.push(poly2)
    polygons.push(poly3)

    result = OpenStudio::joinAllPolygons(polygons, tol)

    # Should return one polygon
    assert_equal(1, result.size)

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
    assert_in_delta(8000, grossArea,0.01)

    netArea = result.first.netArea
    assert_in_delta(6400, netArea, 0.001)
    assert(grossArea > netArea)

    holeArea = OpenStudio::getArea(result.first.getInnerPaths.first)
    assert_equal(grossArea, netArea + holeArea.get)

    # perimeter = result.front().getPerimeter()
  end
end
