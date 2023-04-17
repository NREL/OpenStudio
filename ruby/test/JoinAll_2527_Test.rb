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
