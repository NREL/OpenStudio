require 'minitest'
require 'minitest/autorun'

class MyTest < Minitest::Test
  def test_should_fail
    assert 0 == 1
  end
end
