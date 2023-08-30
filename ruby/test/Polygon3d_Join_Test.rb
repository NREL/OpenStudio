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

# Simple test of join using Polygons. Joins a U shape polygon
# with a rectangle to create a rectangle with a hole.
#
# Validates number of holes, hole dimension, outer perimeter dimension
# gross and net area and preimeter
#
# +----------------------------+
# |       Polygon A            |
# |                            |
# |        +---------+         |
# |        |         |         |
# |        |         |         |
# |        |         |         |
# |--------+---------+---------|
# |                            |
# |       Polygon B            |
# |                            |
# |                            |
# +----------------------------+
#
class Polygon3d_Join_Test < Minitest::Test
  def test_Polygon3d_Join

    polygonA = OpenStudio::Polygon3d.new
    polygonA.addPoint(OpenStudio::Point3d.new(0, 20, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(0, 40, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(30, 40, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(30, 20, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(20, 20, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(20, 30, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(10, 30, 0))
    polygonA.addPoint(OpenStudio::Point3d.new(10, 20, 0))

    polygonB = OpenStudio::Polygon3d.new
    polygonB.addPoint(OpenStudio::Point3d.new(0, 0, 0))
    polygonB.addPoint(OpenStudio::Point3d.new(0, 20, 0))
    polygonB.addPoint(OpenStudio::Point3d.new(30, 20, 0))
    polygonB.addPoint(OpenStudio::Point3d.new(30, 0, 0))

    result = OpenStudio::join(polygonA, polygonB)

    # We should have a result
    assert(result != nil)
    res = result.get
    # The outer should have 4 points
    assert(res.getOuterPath.size == 4)
    # Check the points

    # There should be one hole
    assert(res.getInnerPaths.size == 1)
    # Check the points
    hole = res.getInnerPaths.first
    assert(hole.size == 4)

    perimeter = res.perimeter
    assert_in_delta(140,perimeter, 0.001)

    grossArea = res.grossArea
    assert_in_delta(1200, grossArea,  0.001)

    netArea = res.netArea
    assert_in_delta(1100, netArea, 0.001)
  end
end
