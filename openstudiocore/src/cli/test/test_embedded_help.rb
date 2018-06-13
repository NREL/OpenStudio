require 'minitest/autorun'

# test all the methods we override in embedded_help.rb
# these tests should all pass when run by ruby or openstudio CLI
class EmbeddedHelp_Test < Minitest::Test

  def test_dir_glob
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), '..'))
  
    # test things that should work in ruby or CLI
    no_block_glob = Dir["*.txt", "*.rb"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'embedded_help.rb')
    puts no_block_glob.index('test_embedded_help.rb')
    assert_nil(no_block_glob.index('test_embedded_help.rb'))
    
    no_block_glob = Dir.glob(["*.txt", "*.rb"])
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'embedded_help.rb')
    assert_nil(no_block_glob.index('test_embedded_help.rb'))

    no_block_glob = Dir.glob("*.txt")
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_nil(no_block_glob.index('embedded_help.rb'))
    assert_nil(no_block_glob.index('test_embedded_help.rb'))
    
    no_block_glob = Dir["**/*.txt", "**/*.rb"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, 'CMakeLists.txt')
    assert_includes(no_block_glob, 'embedded_help.rb')
    assert_includes(no_block_glob, 'test/test_embedded_help.rb')
    
    no_block_glob = Dir["./**/*.txt", "./**/*.rb"]
    assert_instance_of(Array, no_block_glob)
    assert_includes(no_block_glob, './CMakeLists.txt')
    assert_includes(no_block_glob, './embedded_help.rb')
    assert_includes(no_block_glob, './test/test_embedded_help.rb')
    
    block_glob = []
    Dir["*.txt", "*.rb"].each do |p|
      block_glob << p
    end    
    assert_includes(block_glob, 'CMakeLists.txt')
    assert_includes(block_glob, 'embedded_help.rb')
    assert_nil(block_glob.index('test_embedded_help.rb'))

    # test things that should only work in the CLI
    if defined?(OpenStudio::CLI) && OpenStudio::CLI

      no_block_glob = Dir[":/*.txt", ":/*.rb"]
      assert_instance_of(Array, no_block_glob)
      
      no_block_glob = Dir.glob(":/*.txt")
      assert_instance_of(Array, no_block_glob)
      
      no_block_glob = Dir[":/**/*.txt", ":/**/*.rb"]
      assert_instance_of(Array, no_block_glob)
      
      block_glob = []
      Dir[":/*.txt", ":/*.rb"].each do |p|
        block_glob << p
      end   
      assert_instance_of(Array, block_glob)      
    end
    
  ensure
    Dir.chdir(original_dir)
  end
  
end