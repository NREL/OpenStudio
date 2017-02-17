
module RbConfig
  def RbConfig.ruby
    EmbeddedScripting::applicationFilePath;
  end
end

module OpenStudio
  def self.openstudio_path
    RbConfig.ruby
  end
end

BINDING = Kernel::binding()
Encoding.default_external = Encoding::ASCII

module Kernel
  # ":" is our root path to the embedded file system
  # make sure it is in the ruby load path
  $:.unshift(ENV['RUBYLIB']) if ENV['RUBYLIB']
  $LOAD_PATH << ':'
  $LOAD_PATH << ':/ruby/2.2.0'
  $LOAD_PATH << ':/ruby/2.2.0/x86_64-darwin15'
  $LOAD_PATH << ':/ruby/2.2.0/x64-mswin64_120'
  # DLM: now done in embedded gem initialization section in openstudio_cli.rb
  #$LOAD_PATH << EmbeddedScripting::findFirstFileByName('openstudio-standards.rb').gsub('/openstudio-standards.rb', '')
  #$LOAD_PATH << EmbeddedScripting::findFirstFileByName('openstudio-workflow.rb').gsub('/openstudio-workflow.rb', '')
  $LOADED = []

  alias :original_require_relative :require_relative
  alias :original_require :require

  def require path
    rb_path = path

    if path.include? 'openstudio/energyplus/find_energyplus'
      return false
    end

    extname = File.extname(path)
    if extname.empty? or extname != '.rb'
      rb_path = path + '.rb'
    end 

    if rb_path.chars.first == ':'
       if $LOADED.include?(rb_path) then
         return true
      else
        return require_embedded_absolute(rb_path)
      end
    elsif rb_path == 'openstudio.rb'
      return true
    else
      $LOAD_PATH.each do |p|
        if p.chars.first == ':' then
          embedded_path = p + '/' + rb_path
          if $LOADED.include?(embedded_path) then
            return true
          elsif EmbeddedScripting::hasFile(embedded_path) then
            return require_embedded_absolute(embedded_path)
          end
        end
      end
    end

    return original_require path
  end

  def require_embedded_absolute path
    $LOADED << path
    s = EmbeddedScripting::getFileAsString(path)
    
    # DLM: temporary hack to get around autoload
    s = s.gsub(/^\s*autoload\s*(:.*?),\s*(.*?)\s*$/, "current_module_ = Module.nesting[0].nil? ? Object : Module.nesting[0]
$autoload_hash[current_module_.to_s.to_sym] = {} if $autoload_hash[current_module_.to_s.to_sym].nil?
$autoload_hash[current_module_.to_s.to_sym][\\1] = [current_module_, \\2, false]")

    result = eval(s,BINDING,path)
    return result
  end

  def require_relative path
    absolute_path = File.dirname(caller_locations(1,1)[0].path) + '/' + path
    if absolute_path.chars.first == ':'
      absolute_path[0] = ''
      absolute_path = File.expand_path absolute_path
      
      # strip Windows drive letters
      if /[A-Z]\:/.match(absolute_path)
        absolute_path = absolute_path[2..-1]
      end
      absolute_path = ':' + absolute_path
    end
    return require absolute_path
  end
  
  # this function either reads a file from the embedded archive or from disk, returns file content as a string
  def load_resource_relative(path, mode='r')

    absolute_path = File.dirname(caller_locations(1,1)[0].path) + '/' + path
    if absolute_path.chars.first == ':'
      absolute_path[0] = ''
      absolute_path = File.expand_path absolute_path
      
      # strip Windows drive letters
      if /[A-Z]\:/.match(absolute_path)
        absolute_path = absolute_path[2..-1]
      end
      absolute_path = ':' + absolute_path
    end

    if EmbeddedScripting::hasFile(absolute_path)
      return EmbeddedScripting::getFileAsString(absolute_path)
    end
    
    result = ""
    if File.exists?(absolute_path)
      File.open(absolute_path, mode) do |file|
        result = file.read
      end
    elsif File.exists?(path)
      File.open(path, mode) do |file|
        result = file.read
      end
    end
    return result
  end
  
end

$autoload_hash = {}
$autoloading = false

class Module
  
  alias :original_const_missing :const_missing
  
  def const_missing(m)
    if caller_locations(1,1)[0].path.chars.first == ':'
      puts 'Module.const_missing'
      sym = self.to_s.to_sym
      puts sym
      puts m
      
      lookup = $autoload_hash[sym]
      if lookup.nil?
        if md = /\#\<Class\:(.*)\>/.match(self.to_s)
          puts 'trying again'
          new_sym = md[1].to_sym
          lookup = $autoload_hash[new_sym]
        end
      end
      
      raise "Cannot find #{sym}" if lookup.nil?
      puts "Found #{sym}!"
      puts "Now look for #{m}"
      
      raise "Cannot find #{m} for #{self}" if lookup[m].nil?
      mod = lookup[m][0]
      path = lookup[m][1]
      loaded = lookup[m][2]

      puts "requiring #{path} for #{m}, #{mod}"
      lookup[m][2] = true
      require path
      puts "done require"
      
      result = mod.const_get(m)
      puts "result = #{result}"
      puts
      return result
    end
    
    original_const_missing(m)
  end
end
