require 'minitest/autorun'
require 'openstudio'

# test bundle capability in CLI
# currently CLI cannot do bundle install, rely on system bundle for that for now
# in future, bundle install should be done like: openstudio --bundle_install Gemfile --bundle_path ./test_gems
class Bundle_Test < Minitest::Test

  def rm_if_exist(p)
    if File.exist?(p)
      # comment out if you want to test without rebundling
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
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --bundle_path './test_gems' --verbose test.rb"))
    
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
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --bundle_path './test_gems' --verbose test.rb"))
    
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
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --bundle_path './test_gems' --verbose test.rb"))
    
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
    
    # intentionally called with dependencies not found in the CLI, expected to fail
    assert_equal(system("'#{OpenStudio::getOpenStudioCLI}' --bundle Gemfile --verbose test.rb"), false)
    
  ensure
    Dir.chdir(original_dir)  
  end  
  
  def test_no_bundle
    original_dir = Dir.pwd
    Dir.chdir(File.join(File.dirname(__FILE__), 'no_bundle'))

    puts "'#{OpenStudio::getOpenStudioCLI}' --verbose test.rb"
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --verbose test.rb"))
    
  ensure
    Dir.chdir(original_dir)  
  end   
  
  def test_bundle_default
    
    original_dir = Dir.pwd
    
    if !defined?(OpenStudio::CLI) || !OpenStudio::CLI
      skip("Embedded gems not available unless CLI") 
    end
    
    Dir.chdir(File.join(File.dirname(__FILE__), 'bundle_default'))
    
    rm_if_exist('openstudio-gems.gemspec')
    rm_if_exist('Gemfile')
    rm_if_exist('Gemfile.lock')
    rm_if_exist('./test_gems')
    rm_if_exist('./bundle')
    
    assert(EmbeddedScripting::hasFile(':/openstudio-gems.gemspec'))
    assert(EmbeddedScripting::hasFile(':/Gemfile'))
    assert(EmbeddedScripting::hasFile(':/Gemfile.lock'))
    
    File.open(File.join(File.dirname(__FILE__), 'bundle_default', 'openstudio-gems.gemspec'), 'w') do |f|
      f.puts EmbeddedScripting::getFileAsString(':/openstudio-gems.gemspec')
      begin
        f.fsync
      rescue
        f.flush
      end      
    end
    File.open(File.join(File.dirname(__FILE__), 'bundle_default', 'Gemfile'), 'w') do |f|
      f.puts EmbeddedScripting::getFileAsString(':/Gemfile')
      begin
        f.fsync
      rescue
        f.flush
      end      
    end
    File.open(File.join(File.dirname(__FILE__), 'bundle_default', 'Gemfile.lock'), 'w') do |f|
      f.puts EmbeddedScripting::getFileAsString(':/Gemfile.lock')
      begin
        f.fsync
      rescue
        f.flush
      end      
    end

    # don't pass bundle_path since we want to use embedded gems
    assert(system("'#{OpenStudio::getOpenStudioCLI}' --bundle './Gemfile' --verbose test.rb"))
    
  ensure
    Dir.chdir(original_dir)  
  end   
  
end
