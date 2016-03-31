
module Kernel
  # ":" is our root path to the embedded file system
  # make sure it is in the ruby load path
  $LOAD_PATH << ':'
  $LOAD_PATH << ':/ruby/2.2.0'
  $LOADED = []

  if defined?(gem_original_require) then
    # Ruby ships with a custom_require, override its require
    remove_method :require
  else
    ##
    alias gem_original_require require
    private :gem_original_require
  end

  def require path
    rb_path = path
    if File.extname(path).empty? then
      rb_path = path + '.rb'
    end 
    $LOAD_PATH.each do |p|
      if p.chars.first == ':' then
        embedded_path = p + '/' + rb_path
        if $LOADED.include?(embedded_path) then
          return true
        elsif EmbeddedScripting::hasFile(embedded_path) then
          $LOADED << embedded_path
          return eval(EmbeddedScripting::getFileAsString(embedded_path))
        end
      end
    end

    return gem_original_require path
  end

end

