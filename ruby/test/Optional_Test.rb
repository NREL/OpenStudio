########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Optional_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_Optional

    # test can assign a value
    optional = OpenStudio::OptionalDouble.new
    assert_equal(false, optional.is_initialized)
    optional = 10.0
    if optional
      assert(true)
      assert(optional == 10.0)
      assert_equal(10.0, optional)
    else
      assert(false) # should not get here
    end

    # this syntax works but is not as nice
    optional = OpenStudio::OptionalDouble.new
    if optional.is_initialized
      assert(false) # should not get here
    else
      assert(true)
    end
    assert(optional.value_or(20.0).is_a?(Float))
    assert_equal(20.0, optional.value_or(20.0))
    assert(optional.value_or(20).is_a?(Float))
    assert_equal(20.0, optional.value_or(20))

    # would like to use this syntax to test if set
    optional = OpenStudio::OptionalDouble.new
    if !optional.empty?
      assert(false) # should not get here
    else
      assert(true)
    end

    optional = OpenStudio::OptionalDouble.new
    assert(!optional.is_initialized)
    assert_raises(RuntimeError) {optional.get}
    optional.set(10.0)
    assert(optional.is_initialized)
    assert_equal(10.0, optional.get)
    assert(optional.value_or(20.0).is_a?(Float))
    assert_equal(10.0, optional.value_or(20.0))
    assert(optional.value_or(20).is_a?(Float))
    assert_equal(10.0, optional.value_or(20))
  end

end
