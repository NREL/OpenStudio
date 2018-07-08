require 'minitest/autorun'
require 'openstudio'

# test bundle capability in CLI
# currently CLI cannot do bundle install, rely on system bundle for that for now
# in future, bundle install should be done like: openstudio --bundle_install --bundle_path ./test_gems
class Bundle_Test < Minitest::Test

  def rm_if_exist(p)
    if File.exist?(p)
      FileUtils.rm_rf(p)
    end
  end

  def test_bundle
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), 'bundle'))
    
    rm_if_exist('Gemfile.lock')
    rm_if_exist('./test_gems')
    rm_if_exist('./bundle')
    
    assert(system('bundle install --path ./test_gems'))
    assert(system('bundle lock --add_platform ruby'))
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --verbose test.rb"))
    
  ensure
    Dir.chdir(original_dir)  
  end
  
  def test_bundle_git
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), 'bundle_git'))
    
    rm_if_exist('Gemfile.lock')
    rm_if_exist('./test_gems')
    rm_if_exist('./bundle')
    
    assert(system('bundle install --path ./test_gems'))
    assert(system('bundle lock --add_platform ruby'))
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --verbose test.rb"))
    
  ensure
    Dir.chdir(original_dir)  
  end  
  
  def test_bundle_native
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), 'bundle_native'))
    
    if /mingw/.match(RUBY_PLATFORM) || /mswin/.match(RUBY_PLATFORM)
      skip("Native gems not supported on Windows") 
    end
    
    rm_if_exist('Gemfile.lock')
    rm_if_exist('./test_gems')
    rm_if_exist('./bundle')
    
    assert(system('bundle install --path ./test_gems'))
    assert(system('bundle lock --add_platform ruby'))
    if /mingw/.match(RUBY_PLATFORM) || /mswin/.match(RUBY_PLATFORM)
      assert(system('bundle lock --add_platform mswin64'))
    end    
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --verbose test.rb"))
    
  ensure
    Dir.chdir(original_dir)  
  end  
  
  def test_bundle_no_install
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), 'bundle_no_install'))
    
    rm_if_exist('Gemfile.lock')
    rm_if_exist('./test_gems')
    rm_if_exist('./bundle')
    
    #assert(system('bundle install --path ./test_gems'))
    #assert(system('bundle lock --add_platform ruby'))
    assert_equal(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --verbose test.rb"), false)
    
  ensure
    Dir.chdir(original_dir)  
  end  
  
  def test_no_bundle
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), 'no_bundle'))

    assert(system("'#{OpenStudio::getOpenStudioCLI}' --verbose test.rb"))
    
  ensure
    Dir.chdir(original_dir)  
  end   
  
end