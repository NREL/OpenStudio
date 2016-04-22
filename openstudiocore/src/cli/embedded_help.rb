BINDING = Kernel::binding()

module Kernel
  # ":" is our root path to the embedded file system
  # make sure it is in the ruby load path
  $LOAD_PATH << ':'
  $LOAD_PATH << ':/ruby/2.2.0'
  $LOAD_PATH << ':/openstudio-workflow-1.0.0.alpha.0/lib'
  $LOADED = []

  $INCLUDE_STACK = []

  if defined?(gem_original_require) then
    # Ruby ships with a custom_require, override its require
    remove_method :require
  else
    ##
    alias gem_original_require require
    private :gem_original_require
  end

  def require path
    puts "requiring path: #{path}"

    rb_path = path

    jsonparser = 'json/ext/parser' 
    if path == jsonparser
      if $LOADED.include?(jsonparser) then
        return true
      else
        EmbeddedScripting::initJSONParser()
        $LOADED << jsonparser
        return true
      end
    end

    jsongenerator = 'json/ext/generator'
    if path == jsongenerator
      if $LOADED.include?(jsongenerator) then
        return true
      else
        EmbeddedScripting::initJSONGenerator()
        $LOADED << jsongenerator
        return true
      end
    end

    if File.extname(path).empty? then
      rb_path = path + '.rb'
    end 

    if rb_path.chars.first == ':'
       if $LOADED.include?(rb_path) then
         return true
      else
        return require_embedded_absolute(rb_path)
      end
    #elsif rb_path == 'openstudio.rb'
    #  return true
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

    return gem_original_require path
  end

  def require_embedded_absolute path
    $LOADED << path
    $INCLUDE_STACK.push path
    result = eval(EmbeddedScripting::getFileAsString(path),BINDING,path)
    $INCLUDE_STACK.pop
    return result
  end

  def require_relative path
    relative_path = "#{File.dirname($INCLUDE_STACK.last)}/#{path}"
    require(relative_path)
  end

end

