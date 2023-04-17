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
