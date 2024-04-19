########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Transformation_Test < Minitest::Test

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


