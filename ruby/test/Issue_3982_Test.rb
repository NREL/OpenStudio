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

class Issue_3982_Test < Minitest::Test
  def test_Issue_3982
    tol = 0.01
    model = OpenStudio::Model::Model.new
    sp1 = OpenStudio::Model::Space.new(model)
    # Create a rectangular surface and an overlapping triangular surface and intersect them
    faceVertices = OpenStudio::Point3dVector.new
    faceVertices.push(OpenStudio::Point3d.new(0, 0, 0))
    faceVertices.push(OpenStudio::Point3d.new(50, 0, 0))
    faceVertices.push(OpenStudio::Point3d.new(50, 10, 0))
    faceVertices.push(OpenStudio::Point3d.new(0, 10, 0))
    s1 = OpenStudio::Model::Surface.new(faceVertices,model)
    s1.setParent(sp1)

    sp2 = OpenStudio::Model::Space.new(model)
    otherFaceVertices = OpenStudio::Point3dVector.new
    otherFaceVertices.push(OpenStudio::Point3d.new(25, 0, 0))
    otherFaceVertices.push(OpenStudio::Point3d.new(37.50, 8, 0))
    otherFaceVertices.push(OpenStudio::Point3d.new(50, 0, 0))
    s2 = OpenStudio::Model::Surface.new(otherFaceVertices,model)
    s2.setParent(sp2)

    spaces = OpenStudio::Model::SpaceVector.new
    spaces.push(sp1)
    spaces.push(sp2)
    OpenStudio::Model::intersectSurfaces(spaces)

    space1Surfaces = sp1.surfaces()

    space1Surfaces = space1Surfaces.sort_by {|x| x.vertices.count}

    assert_equal(2, space1Surfaces.size())
    assert_equal(3, space1Surfaces[0].vertices().size())
    assert_equal(6, space1Surfaces[1].vertices().size())

    space2Surfaces = sp2.surfaces()
    assert_equal(1, space2Surfaces.size())
    assert_equal(3, space2Surfaces[0].vertices().size())
  end
end
