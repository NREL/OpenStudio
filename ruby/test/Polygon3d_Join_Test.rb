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
