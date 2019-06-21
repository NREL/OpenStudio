require 'minitest/autorun'
require 'openstudio'
require 'tmpdir'

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

  def test_encoding2
    test_string = "模型"
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


  # In some cases, including the use of Dir.pwd, on Windows the string might
  # not be UTF-8 encoded
  def test_encoding_external

    # Go into a temp directory
    Dir.chdir(Dir.tmpdir())
    this_dir = Dir.pwd

    s_utf8 = "AfolderwithspécialCHar#%ù/test.osm".encode(Encoding::UTF_8)
    s_windows_1252 = "Afolderwithsp\xE9cialCHar#%\xF9/test.osm".force_encoding(Encoding::Windows_1252)
    assert_equal(s_utf8, s_windows_1252.encode(Encoding::UTF_8))

    # Create a model with the UTF-8 One
    p_utf8 = OpenStudio::Path.new(s_utf8)
    File.delete(s_utf8) if File.exist?(s_utf8)
    assert(!File.exist?(s_utf8))

    m = OpenStudio::Model::Model.new
    m.save(p_utf8, true)
    assert(File.exist?(s_utf8))
    # use the utf-8 one to create a dummy model at the weird path above
    assert OpenStudio::exists(p_utf8)

    # Now we chdir into it, and read the value via pwd
    Dir.chdir(File.dirname(s_utf8))
    # Only Unix, dir_str is UTF-8
    dir_str = Dir.pwd

    if Gem.win_platform?
      assert_equal(dir_str.encoding, Encoding::Windows_1252)
    else
      assert_equal(dir_str.encoding, Encoding::UTF_8)
    end

    p_dir = OpenStudio::Path.new(dir_str)
    assert OpenStudio::exists(p_dir)
    model_path = p_dir / OpenStudio::toPath("test.osm")
    assert(OpenStudio::exists(model_path), "model_path doesn't exist: #{model_path}")
    translator = OpenStudio::OSVersion::VersionTranslator.new
    model = translator.loadModel(model_path)
    assert !model.empty?


    if Gem.win_platform?
      # Try directly with the windows 1252 path
      Dir.chdir(this_dir)
      # I would be VERY surpised if that worked on Linux.
      p_windows_1252 = OpenStudio::Path.new(s_windows_1252)
      assert OpenStudio::exists(p_windows_1252)
    end

  end

end
