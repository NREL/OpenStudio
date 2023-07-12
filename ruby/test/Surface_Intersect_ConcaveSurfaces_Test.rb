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

class Surface_Intersect_ConcaveSurfaces_Test < Minitest::Test
  def test_Surface_Intersect_ConcaveSurfaces
    model = OpenStudio::Model::Model.new
    sp1 = OpenStudio::Model::Space.new(model)

    z = 1

    top = OpenStudio::Point3dVector.new
    top.push(OpenStudio::Point3d.new(3, 0, z))
    top.push(OpenStudio::Point3d.new(3, 5, z))
    top.push(OpenStudio::Point3d.new(7, 5, z))
    top.push(OpenStudio::Point3d.new(7, 0, z))
    normTop = OpenStudio::getOutwardNormal(top)
    assert_in_delta(-1, normTop.get.z, delta = 0.01, msg = nil)
    s1 = OpenStudio::Model::Surface.new(top, model)
    s1.setParent(sp1)

    sp2 = OpenStudio::Model::Space.new(model)
    bottom = OpenStudio::Point3dVector.new
    bottom.push(OpenStudio::Point3d.new(0, 0, z))
    bottom.push(OpenStudio::Point3d.new(10, 0, z))
    bottom.push(OpenStudio::Point3d.new(10, 10, z))
    bottom.push(OpenStudio::Point3d.new(0, 10, z))
    normBottom = OpenStudio::getOutwardNormal(bottom)
    assert_in_delta(1, normBottom.get.z, delta = 0.01, msg= nil)
    s2 = OpenStudio::Model::Surface.new(bottom, model)
    s2.setParent(sp2)

    spaces = OpenStudio::Model::SpaceVector.new
    spaces.push(sp1)
    spaces.push(sp2)
    OpenStudio::Model::intersectSurfaces(spaces)
    OpenStudio::Model::matchSurfaces(spaces)

    assert_equal(1, sp1.surfaces.size)
    assert_equal(4, sp1.surfaces.first.vertices.size)

    space2Surfaces = sp2.surfaces
    space2Surfaces = space2Surfaces.sort_by {|x| x.vertices.count}
    assert_equal(2, sp2.surfaces.size)
    assert_equal(4, space2Surfaces.first.vertices.size)
    assert_equal(8, space2Surfaces.last.vertices.size)
  end
end
