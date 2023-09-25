########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require_relative 'model.rb'
require 'csv'

class Polygon3d_Overlap_Test < Minitest::Test
  def test_Polygon3d_Overlap
    p = OpenStudio::Polygon3d.new
    p.addPoint(OpenStudio::Point3d.new(0, 0, 0))
    p.addPoint(OpenStudio::Point3d.new(0, 70, 0))
    p.addPoint(OpenStudio::Point3d.new(158, 70, 0))
    p.addPoint(OpenStudio::Point3d.new(158, 98, 0))
    p.addPoint(OpenStudio::Point3d.new(0, 98, 0))
    p.addPoint(OpenStudio::Point3d.new(0, 168, 0))
    p.addPoint(OpenStudio::Point3d.new(158, 168, 0))
    p.addPoint(OpenStudio::Point3d.new(158, 268, 0))
    p.addPoint(OpenStudio::Point3d.new(220, 268, 0))
    p.addPoint(OpenStudio::Point3d.new(220, 200, 0))
    p.addPoint(OpenStudio::Point3d.new(288, 200, 0))
    p.addPoint(OpenStudio::Point3d.new(288, 0, 0))

    # NOTE: LINE is the line being tested to overlap and EDGE on the polygon
    # 1 - line/edge start and end points are the same

    line = OpenStudio::Point3dVector.new
    line.push(OpenStudio::Point3d.new(158,98,0))
    line.push(OpenStudio::Point3d.new(0,98,0))
    #overlap = OpenStudio::Point3dVectorVector.new
    overlap = p.overlap(line)
    assert(overlap.size == 1)
    assert(OpenStudio::getDistance(overlap[0][0], OpenStudio::Point3d.new(158, 98, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[0][1], OpenStudio::Point3d.new(0, 98, 0)) < 0.1)


    # 2 - line/edge partial overlap from the start (line.sp, edge.sp, line.ep, edge.ep)
    line.clear()
    line.push(OpenStudio::Point3d.new(200, 200, 0))
    line.push(OpenStudio::Point3d.new(250, 200, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 1)
    assert(OpenStudio::getDistance(overlap[0][0], OpenStudio::Point3d.new(220, 200, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[0][1], OpenStudio::Point3d.new(250, 200, 0)) < 0.1)

    # 3 - line/edge partially overlap past the end (edge.sp, line.sp, edge.ep, line.ep)
    line.clear();
    line.push(OpenStudio::Point3d.new(288, 125, 0))
    line.push(OpenStudio::Point3d.new(288, -25 , 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 1)
    assert(OpenStudio::getDistance(overlap[0][0], OpenStudio::Point3d.new(288, 125, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[0][1], OpenStudio::Point3d.new(288, 0, 0)) < 0.1)

    # 4 - edge is fully enclosed in line
    line.clear()
    line.push(OpenStudio::Point3d.new(150, 268, 0))
    line.push(OpenStudio::Point3d.new(250, 268, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 1)
    assert(OpenStudio::getDistance(overlap[0][0], OpenStudio::Point3d.new(158, 268, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[0][1], OpenStudio::Point3d.new(220, 268, 0)) < 0.1)

    # 5 - line is fully enclosed in edge
    line.clear()
    line.push(OpenStudio::Point3d.new(50, 168, 0))
    line.push(OpenStudio::Point3d.new(100, 168, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 1)
    assert(OpenStudio::getDistance(overlap[0][0], OpenStudio::Point3d.new(50, 168, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[0][1], OpenStudio::Point3d.new(100, 168, 0)) < 0.1)

    # 6 - Line overlaps two edges
    line.clear()
    line.push(OpenStudio::Point3d.new(158, 25, 0))
    line.push(OpenStudio::Point3d.new(158, 275, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 2)
    assert(OpenStudio::getDistance(overlap[0][0], OpenStudio::Point3d.new(158, 70, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[0][1], OpenStudio::Point3d.new(158, 98, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[1][0], OpenStudio::Point3d.new(158, 168, 0)) < 0.1)
    assert(OpenStudio::getDistance(overlap[1][1], OpenStudio::Point3d.new(158, 268, 0)) < 0.1)

    # 7 - No overlap
    line.clear()
    line.push(OpenStudio::Point3d.new(50, 50, 0))
    line.push(OpenStudio::Point3d.new(250, 50, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 0)

    # 8 - No overlap
    line.clear()
    line.push(OpenStudio::Point3d.new(50, 50, 0))
    line.push(OpenStudio::Point3d.new(250, 250, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 0)

    # 9 - No overlap (External corner to 90 deg on external edge)
    line.clear();
    line.push(OpenStudio::Point3d.new(158, 168, 0))
    line.push(OpenStudio::Point3d.new(220, 160, 0))
    overlap = p.overlap(line)
    assert(overlap.size() == 0)
  end
end
