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
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF
#  USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require 'csv'

class Polygon3d_PointInPolygonUp_Test < Minitest::Test
  def test_Polygon3d_PointInPolygonUp
    tol = 0.01
    tol2 = tol / 2.0

    xmin = 0
    ymin = 0
    width = 1
    height = 1

    # Making rectangle Up
    result = OpenStudio::Point3dVector.new
    result.push(OpenStudio::Point3d.new(xmin, ymin, 0))
    result.push(OpenStudio::Point3d.new(xmin + width, ymin, 0))
    result.push(OpenStudio::Point3d.new(xmin + width, ymin + height, 0))
    result.push(OpenStudio::Point3d.new(xmin, ymin + height, 0))

    polygonUp = OpenStudio::Polygon3d.new(result)

    # center
    # asserting false to be true
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 0.5, 0), tol))

    # corners
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 1, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0, 1, 0), tol))

    # edges
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 0.5, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 1, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0, 0.5, 0), tol))

    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(1 - tol2, 0.5, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, 1 - tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(tol2, 0.5, 0), tol))

    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, -tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(1 + tol2, 0.5, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(0.5, 1 + tol2, 0), tol))
    assert(!polygonUp.within(OpenStudio::Point3d.new(-tol2, 0.5, 0), tol))

    # outside
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(2, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(1, 2, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(-1, 0, 0), tol))
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(-1, -1, 0), tol))

    # not on z = 0
    assert(!polygonUp.pointInPolygon(OpenStudio::Point3d.new(0.5, 0.5, 0.5), tol))
  end
end
