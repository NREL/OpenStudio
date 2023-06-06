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

class JoinAll_2527_Test < Minitest::Test
  def test_JoinAll_2527
    tol = 0.01

    polygons = OpenStudio::Point3dVectorVector.new

    # North
    poly1 = OpenStudio::Point3dVector.new
    poly1.push(OpenStudio::Point3d.new(40.869, 30439.131, 0))
    poly1.push(OpenStudio::Point3d.new(30439.131, 30439.131, 0))
    poly1.push(OpenStudio::Point3d.new(25867, 25867, 0))
    poly1.push(OpenStudio::Point3d.new(4612, 25867, 0))
    # East
    poly2 = OpenStudio::Point3dVector.new
    poly2.push(OpenStudio::Point3d.new(30439.131, 30439.131, 0))
    poly2.push(OpenStudio::Point3d.new(30439.131, 40.869, 0))
    poly2.push(OpenStudio::Point3d.new(25867, 4612, 0))
    poly2.push(OpenStudio::Point3d.new(25867, 25867, 0))
    # West
    poly3 = OpenStudio::Point3dVector.new
    poly3.push(OpenStudio::Point3d.new(40.869, 40.869, 0))
    poly3.push(OpenStudio::Point3d.new(40.869, 30439.131, 0))
    poly3.push(OpenStudio::Point3d.new(4612, 25867, 0))
    poly3.push(OpenStudio::Point3d.new(4612, 4612, 0))
    # Core
    poly4 = OpenStudio::Point3dVector.new
    poly4.push(OpenStudio::Point3d.new(25867, 4612, 0))
    poly4.push(OpenStudio::Point3d.new(4612, 4612, 0))
    poly4.push(OpenStudio::Point3d.new(4612, 25867, 0))
    poly4.push(OpenStudio::Point3d.new(25867, 25867, 0))
    # divide the bottom poly left to right, tri, quad, quad, tri
    poly5 = OpenStudio::Point3dVector.new
    poly5.push(OpenStudio::Point3d.new(4612, 4612, 0))
    poly5.push(OpenStudio::Point3d.new(4612, 40.869, 0))
    poly5.push(OpenStudio::Point3d.new(40.869, 40.869, 0))
    poly6 = OpenStudio::Point3dVector.new
    poly6.push(OpenStudio::Point3d.new(4612, 4612, 0))
    poly6.push(OpenStudio::Point3d.new(4612, 40.869, 0))
    poly6.push(OpenStudio::Point3d.new(15219.565, 40.869, 0))
    poly6.push(OpenStudio::Point3d.new(15219.565, 4612, 0))
    poly7 = OpenStudio::Point3dVector.new
    poly7.push(OpenStudio::Point3d.new(15219.565, 4612, 0))
    poly7.push(OpenStudio::Point3d.new(15219.565, 40.869, 0))
    poly7.push(OpenStudio::Point3d.new(25867, 40.869, 0))
    poly7.push(OpenStudio::Point3d.new(25867, 4612, 0))
    poly8 = OpenStudio::Point3dVector.new
    poly8.push(OpenStudio::Point3d.new(25867, 4612, 0))
    poly8.push(OpenStudio::Point3d.new(30439.131, 40.869, 0))
    poly8.push(OpenStudio::Point3d.new(25867, 40.869, 0))

    polyx = OpenStudio::Point3dVector.new
    polyx.push(OpenStudio::Point3d.new(30439.131, 40.869, 0))
    polyx.push(OpenStudio::Point3d.new(40.869, 40.869, 0))
    polyx.push(OpenStudio::Point3d.new(4612, 4612, 0))
    polyx.push(OpenStudio::Point3d.new(25867, 4612, 0))

    # puts "polyx size #{polyx.size}"

    polygons.push(poly1)
    polygons.push(poly2)
    polygons.push(poly3)
    polygons.push(poly4)
    polygons.push(poly5)
    polygons.push(poly6)
    polygons.push(poly7)
    polygons.push(poly8)


    # We know this fails because join all does not in fact join all
    test = OpenStudio::joinAll(polygons, tol)
    assert_equal(3, test.size)

    resultPoly = OpenStudio::joinAllPolygons(polygons, tol)
    # puts "Output of joinAllPolygons' Size is -> #{resultPoly.size}"
    assert_equal(1, resultPoly.size)

  end
end
