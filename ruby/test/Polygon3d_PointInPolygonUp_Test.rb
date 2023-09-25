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

class Polygon3d_PointInPolygonUp_Test < Minitest::Test
  def test_Polygon3d_PointInPolygonUp
    tol = 0.01
    tol2 = tol / 2.0

    xmin = 0
    ymin = 0
    width = 1
    height = 1

    # Making rectangle Up
    result = OpenStudio::Point3dVector.new
    result.push(OpenStudio::Point3d.new(xmin, ymin, 0))
    result.push(OpenStudio::Point3d.new(xmin + width, ymin, 0))
    result.push(OpenStudio::Point3d.new(xmin + width, ymin + height, 0))
    result.push(OpenStudio::Point3d.new(xmin, ymin + height, 0))

    polygonUp = OpenStudio::Polygon3d.new(result)

    # center
    # asserting false to be true
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 0.5, 0), tol))

    # corners
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 1, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0, 1, 0), tol))

    # edges
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 0.5, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 1, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0, 0.5, 0), tol))

    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(1 - tol2, 0.5, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, 1 - tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(tol2, 0.5, 0), tol))

    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, -tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(1 + tol2, 0.5, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, 1 + tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(-tol2, 0.5, 0), tol))

    # outside
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(2, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 2, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(-1, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(-1, -1, 0), tol))

    # not on z = 0
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 0.5, 0.5), tol))
  end
end
