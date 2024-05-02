require 'minitest/autorun'
require 'json'


# test all the methods we override in embedded_help.rb
# these tests should all pass when run by ruby or openstudio CLI
class EmbeddedHelp_Test < Minitest::Test

  def test_dir_glob
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), '..'))

    # test things that should work in ruby or CLI
    no_block_glob = Dir["*.txt", "*.cpp"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'main.cpp')
    assert_nil(no_block_glob.index('test_main.cpp'))

    no_block_glob = Dir.glob(["*.txt", "*.cpp"])
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'main.cpp')
    assert_nil(no_block_glob.index('test_main.cpp'))

    no_block_glob = Dir.glob("*.txt")
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_nil(no_block_glob.index('main.cpp'))
    assert_nil(no_block_glob.index('test_main.cpp'))

    no_block_glob = Dir["**/*.txt", "**/*.cpp"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'main.cpp')
    assert_includes(no_block_glob, 'test/Logger_GTest.cpp')

    no_block_glob = Dir["*{.txt,.cpp}"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'main.cpp')
    assert_nil(no_block_glob.index('test_main.cpp'))
    assert_nil(no_block_glob.index('test/Logger_GTest.cpp'))

    no_block_glob = Dir["{,*,*/*}.{txt,cpp}"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'main.cpp')
    assert_includes(no_block_glob, 'test/Logger_GTest.cpp')

    no_block_glob = Dir.glob("{,*,*/*}.{txt,cpp}")
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'main.cpp')
    assert_includes(no_block_glob, 'test/Logger_GTest.cpp')

    no_block_glob = Dir["./**/*.txt", "./**/*.cpp"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, './CMakeLists.txt')
    assert_includes(no_block_glob, './main.cpp')
    assert_includes(no_block_glob, './test/Logger_GTest.cpp')

    block_glob = []
    Dir["*.txt", "*.cpp"].each do |p|
      block_glob << p
    end
    assert_includes(block_glob, 'CMakeLists.txt')
    assert_includes(block_glob, 'main.cpp')
    assert_nil(block_glob.index('test_main.cpp'))

    assert(File.fnmatch( "C:/test/help.rb", "C:/test/help.rb", 0))
    assert(File.fnmatch( "C:/test/help.rb", "C:/test/help.rb", File::FNM_EXTGLOB))
    assert(File.fnmatch( "C:/**/help.rb", "C:/test/help.rb", 0))
    assert(File.fnmatch( "C:/**/help.rb", "C:/test/help.rb", File::FNM_EXTGLOB))
    assert(!File.fnmatch( "C:/**/help{.rb,.txt}", "C:/test/help.rb", 0))
    assert(File.fnmatch( "C:/**/help{.rb,.txt}", "C:/test/help.rb", File::FNM_EXTGLOB))

    assert(File.fnmatch( ":/test/help.rb", ":/test/help.rb", 0))
    assert(File.fnmatch( ":/test/help.rb", ":/test/help.rb", File::FNM_EXTGLOB))
    assert(File.fnmatch( ":/**/help.rb", ":/test/help.rb", 0))
    assert(File.fnmatch( ":/**/help.rb", ":/test/help.rb", File::FNM_EXTGLOB))
    assert(!File.fnmatch( ":/**/help{.rb,.txt}", ":/test/help.rb", 0))
    assert(File.fnmatch( ":/**/help{.rb,.txt}", ":/test/help.rb", File::FNM_EXTGLOB))

    # test things that should only work in the CLI
    if defined?(OpenStudio::CLI) && OpenStudio::CLI

      no_block_glob = Dir[":/*.txt", ":/*.rb"]
      assert_instance_of(Array, no_block_glob)
      assert(no_block_glob.size > 0)

      no_block_glob = Dir.glob(":/*.txt")
      assert_instance_of(Array, no_block_glob)
      assert(no_block_glob.size == 0)

      no_block_glob = Dir[":/**/*.txt", ":/**/*.rb"]
      assert_instance_of(Array, no_block_glob)
      assert(no_block_glob.size > 0)

      no_block_glob = Dir[":/{,*,*/*}.rb"]
      assert_instance_of(Array, no_block_glob)
      assert(no_block_glob.size > 0)

      block_glob = []
      Dir[":/*.txt", ":/*.rb"].each do |p|
        block_glob << p
      end
      assert_instance_of(Array, block_glob)
      assert(block_glob.size > 0)
    end

  ensure
    Dir.chdir(original_dir)
  end


  def test_file_read
    test_json = { :test_int => 10, :test_string => 'String'}

    if File.exist?('test.json')
      FileUtils.rm('test.json')
    end
    assert(!File.exist?('test.json'))

    File.open('test.json', 'w') do |f|
      f.puts JSON.generate(test_json)
      # make sure data is written to the disk one way or the other
      begin
        f.fsync
      rescue
        f.flush
      end
    end
    assert(File.exist?('test.json'))

    test_json2 = JSON.parse(File.read('test.json'), symbolize_names: true)
    assert_equal(test_json2[:test_int], 10)
    assert_equal(test_json2[:test_string], 'String')

    test_json3 = nil
    File.open('test.json', 'r') do |f|
      test_json3 = JSON.parse(f.read, symbolize_names: true)
    end
    assert_equal(test_json3[:test_int], 10)
    assert_equal(test_json3[:test_string], 'String')

  ensure
    if File.exist?('test.json')
      FileUtils.rm('test.json')
    end
  end


  def test_fileutils_mv
    test_json = { :test_int => 10, :test_string => 'String'}

    if File.exist?('mv_test.json')
      FileUtils.rm('mv_test.json')
    end
    assert(!File.exist?('mv_test.json'))

    if File.exist?('mv_test2.json')
      FileUtils.rm('mv_test2.json')
    end
    assert(!File.exist?('mv_test2.json'))

    File.open('mv_test.json', 'w') do |f|
      f.puts JSON.generate(test_json)
      # make sure data is written to the disk one way or the other
      begin
        f.fsync
      rescue
        f.flush
      end
    end
    assert(File.exist?('mv_test.json'))
    assert(!File.exist?('mv_test2.json'))

    FileUtils.mv('mv_test.json', 'mv_test2.json', force: true)
    assert(!File.exist?('mv_test.json'))
    assert(File.exist?('mv_test2.json'))

  ensure
    if File.exist?('mv_test.json')
      FileUtils.rm('mv_test.json')
    end
    if File.exist?('mv_test2.json')
      FileUtils.rm('mv_test2.json')
    end
  end

end
