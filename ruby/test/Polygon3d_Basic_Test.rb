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

class Polygon3d_Basic_Test < Minitest::Test
  def test_Polygon3d_Basic
    testPolygon = OpenStudio::Polygon3d.new

    testPolygon.addPoint(OpenStudio::Point3d.new(0, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100, 100, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 100, 0))

    normal = testPolygon.outwardNormal
    perimeter = testPolygon.perimeter
    assert_in_delta(400, perimeter, 1)

    grossArea = testPolygon.grossArea
    assert_in_delta(10000, grossArea, 1)

    netArea = testPolygon.netArea
    assert_equal(grossArea, netArea)

    # notrmal shoudl eb the same as it would be calling the static method
    normal1 = OpenStudio::getOutwardNormal(testPolygon.getOuterPath())
    assert_equal(normal.x, normal1.get.x)
    assert_equal(normal.y, normal1.get.y)
    assert_equal(normal.z, normal1.get.z)

    # Check area
    assert_equal(grossArea, OpenStudio::getArea(testPolygon.getOuterPath).get)

  end
end
