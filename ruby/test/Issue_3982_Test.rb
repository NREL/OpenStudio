########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
