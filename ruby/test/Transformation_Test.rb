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

require 'minitest/autorun'

class Transformation_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_multiplication
    points = OpenStudio::Point3dVector.new(3)
    points[0] = OpenStudio::Point3d.new(0, 0, 0)
    points[1] = OpenStudio::Point3d.new(1, 1, 1)
    points[2] = OpenStudio::Point3d.new(2, 2, 2)

    assert(points.size == 3)
    points.each_with_index do |value, i|
      assert(value.class == OpenStudio::Point3d)
      assert(value.x == i)
      assert(value.y == i)
      assert(value.z == i)
    end

    vectors = OpenStudio::Vector3dVector.new(3)
    vectors[0] = OpenStudio::Vector3d.new(0, 0, 0)
    vectors[1] = OpenStudio::Vector3d.new(1, 1, 1)
    vectors[2] = OpenStudio::Vector3d.new(2, 2, 2)

    assert(vectors.size == 3)
    vectors.each_with_index do |value, i|
      assert(value.class == OpenStudio::Vector3d)
      assert(value.x == i)
      assert(value.y == i)
      assert(value.z == i)
    end

    # identity
    t = OpenStudio::Transformation.new

    points = t*points

    # points is no longer a OpenStudio::Point3dVector, it is an Array

    assert(points.size == 3)
    points.each_with_index do |value, i|
      assert(value.class == OpenStudio::Point3d)
      assert(value.x == i)
      assert(value.y == i)
      assert(value.z == i)
    end

    vectors = t*vectors

    # points is no longer a OpenStudio::Vector3dVector, it is an Array

    assert(vectors.size == 3)
    vectors.each_with_index do |value, i|
      assert(value.class == OpenStudio::Vector3d)
      assert(value.x == i)
      assert(value.y == i)
      assert(value.z == i)
    end

    # make sure transformation * accepts Arrary types

    points = t*points

    # points is no longer a OpenStudio::Point3dVector, it is an Array

    assert(points.size == 3)
    points.each_with_index do |value, i|
      assert(value.class == OpenStudio::Point3d)
      assert(value.x == i)
      assert(value.y == i)
      assert(value.z == i)
    end

    vectors = t*vectors

    # points is no longer a OpenStudio::Vector3dVector, it is an Array

    assert(vectors.size == 3)
    vectors.each_with_index do |value, i|
      assert(value.class == OpenStudio::Vector3d)
      assert(value.x == i)
      assert(value.y == i)
      assert(value.z == i)
    end

  end

  def test_translation
    # the static method translation was being hidden by the instance method, created free function createTranslation to get around this

    vector = OpenStudio::Vector3d.new(1,2,3)

    transformation = OpenStudio::createTranslation(vector)

    translation = transformation.translation

    assert(translation.x == 1)
    assert(translation.y == 2)
    assert(translation.z == 3)
  end

end


