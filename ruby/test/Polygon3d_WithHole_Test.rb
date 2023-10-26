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

class Polygon3d_WithHole_Test < Minitest::Test
  def test_Polygon3d_WithHole
    testPolygon = OpenStudio::Polygon3d.new
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100, 0, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(100, 100, 0))
    testPolygon.addPoint(OpenStudio::Point3d.new(0, 100, 0))

    hole = OpenStudio::Point3dVector.new
    hole.push(OpenStudio::Point3d.new(30, 30, 0))
    hole.push(OpenStudio::Point3d.new(30, 70, 0))
    hole.push(OpenStudio::Point3d.new(70, 70, 0))
    hole.push(OpenStudio::Point3d.new(70, 30, 0))

    testPolygon.addHole(hole)
    perimeter = testPolygon.perimeter
    assert_in_delta(400,perimeter,1)

    grossArea = testPolygon.grossArea
    assert_in_delta(10000,grossArea,1)

    netArea = testPolygon.netArea
    #assert_in_delta(grossArea,netArea)
    assert_equal(netArea,8400)
  end
end
