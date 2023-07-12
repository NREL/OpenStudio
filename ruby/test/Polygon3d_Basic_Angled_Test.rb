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

class Polygon3d_Basic_Angled_Test < Minitest::Test
  def test_Polygon3d_Basic_Angled
    testPolygon = OpenStudio::Polygon3d.new

    testPolygon.addPoint(OpenStudio::Point3d.new(7.620, 7.620, 1.876))
    testPolygon.addPoint(OpenStudio::Point3d.new(15.544, 15.544, -0.104))
    testPolygon.addPoint(OpenStudio::Point3d.new(15.544, 30.784, -0.104))
    testPolygon.addPoint(OpenStudio::Point3d.new(7.620, 22.860, 1.876))

    assert_in_delta(124.4746, testPolygon.grossArea,0.01)

    normal = testPolygon.outwardNormal
    assert_in_delta(0.24242, normal.x, 0.001)
    assert_in_delta(0, normal.y, 0.001)
    assert_in_delta(0.97017, normal.z, 0.001)
    assert_in_delta(1, normal.length, 0.001)

    perimeter = testPolygon.perimeter()
    assert_in_delta(53.2396,perimeter, 0.001)
  end
end
