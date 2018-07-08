require 'minitest/autorun'
require 'openstudio'

# test bundle capability in CLI
# currently CLI cannot do bundle install, rely on system bundle for that
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
    
    rm_if_exist('Gemfile.lock')
    rm_if_exist('./test_gems')
    rm_if_exist('./bundle')
    
    assert(system('bundle install --path ./test_gems'))
    assert(system('bundle lock --add_platform ruby'))
    assert(system('bundle lock --add_platform mswin'))
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