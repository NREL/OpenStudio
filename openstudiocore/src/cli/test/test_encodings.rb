require 'minitest/autorun'
require 'openstudio'

# test encoding of strings returned from OpenStudio Ruby bindings
class Encoding_Test < Minitest::Test

  def test_encoding
    test_string = "Hello"
    assert_equal("UTF-8", test_string.encoding.to_s)
    
    model = OpenStudio::Model::Model.new
    space = OpenStudio::Model::Space.new(model)
    
    space.setName(test_string)
    
    name_string = space.nameString
    assert_equal(test_string, name_string)
    assert_equal("UTF-8", name_string.encoding.to_s)
    
    optional_string = space.name
    assert(optional_string.is_initialized)
    assert_equal(test_string, optional_string.get)
    assert_equal("UTF-8", optional_string.get.encoding.to_s)
    
    path = OpenStudio::toPath(test_string)
    assert_equal(test_string, path.to_s)
    assert_equal("UTF-8", path.to_s.encoding.to_s)
  end   
  
end