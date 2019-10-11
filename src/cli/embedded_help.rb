
module OpenStudio

  # check if this is CLI
  CLI = true

  def self.openstudio_path
    RbConfig.ruby
  end

  def self.get_absolute_path(p)
    absolute_path = ""
    if p.to_s.chars.first == ':' then
      p2 = String.new(p.to_s)
      p2[0] = ''
      absolute_path = File.expand_path p2

      # strip Windows drive letters
      if /[A-Z]\:/.match(absolute_path)
        absolute_path = absolute_path[2..-1]
      end
      absolute_path = ':' + absolute_path
    else
      absolute_path = File.expand_path p2
    end
    return absolute_path
  end

  def self.preprocess_ruby_script(s)
#puts "self.preprocess_ruby_script before: #{s}"
    # DLM: temporary hack to get around autoload
    s.gsub!(/^\s*autoload\s*(:.*?),\s*(.*?)\s*$/, "current_module_ = Module.nesting[0].nil? ? Object : Module.nesting[0]
    $autoload_hash[current_module_.to_s.to_sym] = {} if $autoload_hash[current_module_.to_s.to_sym].nil?
    $autoload_hash[current_module_.to_s.to_sym][\\1] = [current_module_, \\2]\n")
#puts "self.preprocess_ruby_script after: #{s}"
    return s
  end
end

BINDING = Kernel::binding()
#puts Encoding.list

# DLM: ignore for now
#Encoding.default_external = Encoding::ASCII

module Kernel
  # ":" is our root path to the embedded file system
  # make sure it is in the ruby load path
  if ENV['RUBYLIB']
    ENV['RUBYLIB'].split(File::PATH_SEPARATOR).each {|lib| $LOAD_PATH.unshift(lib)}
  end
  $LOAD_PATH << ':'
  $LOAD_PATH << ':/ruby/2.5.0'
  $LOAD_PATH << ':/ruby/2.5.0/x86_64-darwin16'
  $LOAD_PATH << ':/ruby/2.5.0/x64-mswin64_140'
  # DLM: now done in embedded gem initialization section in openstudio_cli.rb
  #$LOAD_PATH << EmbeddedScripting::findFirstFileByName('openstudio-standards.rb').gsub('/openstudio-standards.rb', '')
  #$LOAD_PATH << EmbeddedScripting::findFirstFileByName('openstudio-workflow.rb').gsub('/openstudio-workflow.rb', '')
  $LOADED = []

  alias :original_require_relative :require_relative
  alias :original_require :require
  alias :original_open :open

  def require path
    original_directory = Dir.pwd
    rb_path = path

    if path.include? 'openstudio/energyplus/find_energyplus'
      return false
    end
    
    if path.include? 'json/ext'
      if RUBY_PLATFORM =~ /mswin/
        raise LoadError.new("Preventing segfault if native JSON is built as MinGW")
      end
    end
    
    extname = File.extname(path)
    if extname.empty? or extname != '.rb'
      rb_path = path + '.rb'
    end

    if rb_path.to_s.chars.first == ':'
       if $LOADED.include?(rb_path) then
         return true
      else
        return require_embedded_absolute(rb_path)
      end
    elsif rb_path == 'openstudio.rb'
      return true
    else
      $LOAD_PATH.each do |p|
        if p.to_s.chars.first == ':' then
          embedded_path = p + '/' + rb_path
          if $LOADED.include?(embedded_path) then
            return true
          elsif EmbeddedScripting::hasFile(embedded_path) then
            return require_embedded_absolute(embedded_path)
          end
        end
      end
    end

    result = original_require path
    
    current_directory = Dir.pwd 
    if original_directory != current_directory
      Dir.chdir(original_directory)
      puts "Directory changed from '#{original_directory}' to '#{current_directory}' while requiring '#{path}', result = #{result}, restoring original_directory"
      STDOUT.flush
    end
    
    return result
  end

  def require_embedded_absolute path
    original_directory = Dir.pwd
    
    $LOADED << path
    s = EmbeddedScripting::getFileAsString(path)

    s = OpenStudio::preprocess_ruby_script(s)

    result = eval(s,BINDING,path)
    
    current_directory = Dir.pwd 
    if original_directory != current_directory
      Dir.chdir(original_directory)
      puts "Directory changed from '#{original_directory}' to '#{current_directory}' while require_embedded_absolute '#{path}', result = #{result}, restoring original_directory"
      STDOUT.flush
    end
    
    return result
  end

  def require_relative path
    absolute_path = File.dirname(caller_locations(1,1)[0].path) + '/' + path
    if absolute_path.to_s.chars.first == ':'
      absolute_path = OpenStudio.get_absolute_path(absolute_path)
    end
    return require absolute_path
  end

  # this function either reads a file from the embedded archive or from disk, returns file content as a string
  def load_resource_relative(path, mode='r')

    absolute_path = File.dirname(caller_locations(1,1)[0].path) + '/' + path
    if absolute_path.to_s.chars.first == ':'
      absolute_path = OpenStudio.get_absolute_path(absolute_path)
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

  # Finds files relative to a given location whose names
  # (not including file extension) match the regex
  # and whose extension matches the specified extension.
  # Recursively includes files in subdirectories below the given location.
  #
  # @param path [String] the directory to search relative to the current file.
  #   Use ./ for the current directory
  # @param file_name_regexp [Regexp] the Ruby regular expression to match the file name
  #   (including the file extension).
  # @return [Array<String>] an array of absolute file paths matching the specified string
  def embedded_files_relative(path, file_name_regexp)

    # Resolve the absolute path to the caller location
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

    # Loop through all the files in the embedded system
    matches = []
    EmbeddedScripting.allFileNamesAsString.split(';').each do |file|
      # Skip files outside of the specified directory
      next unless file.start_with?(absolute_path)
      # Skip files that don't match the file_name_pattern criterion
      next unless File.basename(file).match(file_name_regexp)
      # If here, found a match
      matches << file
    end

    return matches
  end

  def open(name, *args)
    #puts "I'm in Kernel.open!"
    #STDOUT.flush
    if name.to_s.chars.first == ':' then
      #puts "self.open(name, *args), name = #{name}, args = #{args}"
      absolute_path = OpenStudio.get_absolute_path(name)
      #puts "absolute_path = #{absolute_path}"
      if EmbeddedScripting::hasFile(absolute_path) then
        string = EmbeddedScripting::getFileAsString(absolute_path)
        #puts "string = #{string}"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = StringIO.open(string)
          begin
            result = yield(io)
          ensure
            io.close
          end
          return result
        else
          return StringIO.open(string)
        end
      else
        #puts "IO.open cannot find embedded file '#{absolute_path}' for '#{name}'"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = StringIO.open("")
          begin
            result = yield(io)
          ensure
            io.close
          end
          return result
        else
          return ""
        end
      end
    end

    if block_given?
      # if a block is given, then a new IO is created and closed
      io = original_open(name, *args)
      begin
        result = yield(io)
      ensure
        io.close
      end
      return result
    else
      return original_open(name, *args)
    end
  end

end

$autoload_hash = {}

class Module

  alias :original_const_missing :const_missing

  def const_missing(m)
    if caller_locations(1,1)[0].path.to_s.chars.first == ':'
      sym = self.to_s.to_sym

      lookup = $autoload_hash[sym]
      if lookup.nil?
        # this seems to occur if a class and a module have the same name?
        if md = /\#\<Class\:(.*)\>/.match(self.to_s)
          sym = md[1].to_sym
          lookup = $autoload_hash[sym]
        end
      end

      if lookup.nil?
        # check parent modules
        while md = /(.*)\:\:.*?/.match(sym.to_s)
          sym = md[1].to_sym
          lookup = $autoload_hash[sym]
          break if lookup
        end
      end

      return nil if lookup.nil?

      return nil if lookup[m].nil?
      mod = lookup[m][0]
      path = lookup[m][1]

      require path

      result = mod.const_get(m)

      return result
    end

    original_const_missing(m)
  end
end

class IO
  class << self
    alias :original_read :read
    alias :original_open :open
  end

  def self.read(name, *args)
    if name.to_s.chars.first == ':' then
      #puts "self.read(name, *args), name = #{name}, args = #{args}"
      #STDOUT.flush
      absolute_path = OpenStudio.get_absolute_path(name)
      #puts "absolute_path = #{absolute_path}"
      #STDOUT.flush
      if EmbeddedScripting::hasFile(absolute_path) then
        return EmbeddedScripting::getFileAsString(absolute_path)
      else
        puts "IO.read cannot find embedded file '#{absolute_path}' for '#{name}'"
        return ""
      end
    end

    #puts "self.original_read, name = #{name}, args = #{args}, block_given? = #{block_given?}"
    #STDOUT.flush
    return original_read(name, *args)
  end

  def self.open(name, *args)

    if name.to_s.chars.first == ':' then
      #puts "self.open(name, *args), name = #{name}, args = #{args}"
      absolute_path = OpenStudio.get_absolute_path(name)
      #puts "absolute_path = #{absolute_path}"
      if EmbeddedScripting::hasFile(absolute_path) then
        string = EmbeddedScripting::getFileAsString(absolute_path)
        #puts "string = #{string}"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = StringIO.open(string)
          begin
            result = yield(io)
          ensure
            io.close
          end
          return result
        else
          return StringIO.open(string)
        end
      else
        puts "IO.open cannot find embedded file '#{absolute_path}' for '#{name}'"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = StringIO.open("")
          begin
            result = yield(io)
          ensure
            io.close
          end
          return result
        else
          return ""
        end
      end
    end

    if block_given?
      # if a block is given, then a new IO is created and closed
      io = self.original_open(name, *args)
      begin
        result = yield(io)
      ensure
        io.close
      end
      return result
    else
      return self.original_open(name, *args)
    end
  end
end

class File
  class << self
    alias :original_expand_path :expand_path
    alias :original_realpath :realpath
    alias :original_absolute_path :absolute_path
    alias :original_directory? :directory?
    alias :original_file? :file?
  end

  def self.expand_path(file_name, *args)
    if file_name.to_s.chars.first == ':' then
      #puts "self.expand_path(file_name, *args), file_name = #{file_name}, args = #{args}"
      #STDOUT.flush
      return OpenStudio.get_absolute_path(file_name)
    elsif args.size == 1 && args[0].to_s.chars.first == ':' then
      #puts "2 self.expand_path(file_name, *args), file_name = #{file_name}, args = #{args}"
      #puts "x = #{File.join(args[0], file_name)}"
      #puts "y = #{OpenStudio.get_absolute_path(File.join(args[0], file_name))}"
      #STDOUT.flush
      #return original_expand_path(file_name, *args)
      return OpenStudio.get_absolute_path(File.join(args[0], file_name))
    end
    return original_expand_path(file_name, *args)
  end

  def self.absolute_path(file_name, *args)
    if file_name.to_s.chars.first == ':' then
      #puts "self.absolute_path(file_name, *args), file_name = #{file_name}, args = #{args}"
      #STDOUT.flush
      return OpenStudio.get_absolute_path(file_name)
    elsif args.size == 1 && args[0].to_s.chars.first == ':' then
      #puts "2 self.absolute_path(file_name, *args), file_name = #{file_name}, args = #{args}"
      #puts "x = #{File.join(args[0], file_name)}"
      #puts "y = #{OpenStudio.get_absolute_path(File.join(args[0], file_name))}"
      #STDOUT.flush
      #return original_absolute_path(file_name, *args)
      return OpenStudio.get_absolute_path(File.join(args[0], file_name))
    end
    return original_absolute_path(file_name, *args)
  end

  def self.realpath(file_name, *args)
    if file_name.to_s.chars.first == ':' then
      #puts "self.realpath(file_name, *args), file_name = #{file_name}, args = #{args}"
      #STDOUT.flush
      return OpenStudio.get_absolute_path(file_name)
    elsif args.size == 1 && args[0].to_s.chars.first == ':' then
      #puts "2 self.realpath(file_name, *args), file_name = #{file_name}, args = #{args}"
      #puts "x = #{File.join(args[0], file_name)}"
      #puts "y = #{OpenStudio.get_absolute_path(File.join(args[0], file_name))}"
      #STDOUT.flush
      #return original_realpath(file_name, *args)
      return OpenStudio.get_absolute_path(File.join(args[0], file_name))
    end
    return original_realpath(file_name, *args)
  end

  def self.directory?(file_name)
    if file_name.to_s.chars.first == ':' then
      absolute_path = OpenStudio.get_absolute_path(file_name)
      EmbeddedScripting.allFileNamesAsString.split(';').each do |file|
        # true if a file starts with this absolute path
        next unless file.start_with?(absolute_path)

        # if this is an exact match because then this is a file not a directory
        return false if (file == absolute_path)

        # If here, found a match
        return true
      end
      return false
    end
    return original_directory?(file_name)
  end

  def self.file?(file_name)
    if file_name.to_s.chars.first == ':' then
      absolute_path = OpenStudio.get_absolute_path(file_name)
      EmbeddedScripting.allFileNamesAsString.split(';').each do |file|
        # Skip files unless exact match
        next unless (file == absolute_path)

        # If here, found a match
        return true
      end
      return false
    end
    return original_file?(file_name)
  end

end

class Dir
  class << self
    alias :original_glob :glob
    alias :original_chdir :chdir
  end

  def self.[](*args)
     if block_given?
      return yield(self.glob(args, 0))
    else
      return self.glob(args, 0)
    end
  end

  def self.glob(pattern, *args)

    pattern_array = []
    if pattern.is_a? String
      pattern_array = [pattern]
    elsif pattern.is_a? Array
      pattern_array = pattern
    else
      pattern_array = pattern
    end

    #puts "Dir.glob pattern = #{pattern}, pattern_array = #{pattern_array}, args = #{args}"
    override_args_extglob = false

    result = []
    pattern_array.each do |pattern|

      if pattern.to_s.chars.first == ':'

        # DLM: seems like this is needed for embedded paths, possibly due to leading ':' character?
        override_args_extglob = true

        #puts "searching embedded files for #{pattern}"
        absolute_pattern = OpenStudio.get_absolute_path(pattern)
        #puts "absolute_pattern #{absolute_pattern}"

        # DLM: this does not appear to be swig'd correctly
        #EmbeddedScripting::fileNames.each do |name|
        EmbeddedScripting::allFileNamesAsString.split(';').each do |name|
          absolute_path = OpenStudio.get_absolute_path(name)

          if override_args_extglob
            if File.fnmatch( absolute_pattern, absolute_path, File::FNM_EXTGLOB )
              #puts "#{absolute_path} is a match!"
              result << absolute_path
            end
          else
            if File.fnmatch( absolute_pattern, absolute_path, *args )
              #puts "#{absolute_path} is a match!"
              result << absolute_path
            end
          end

        end

      else
        if override_args_extglob
          result.concat(self.original_glob(pattern, File::FNM_EXTGLOB))
        else
          result.concat(self.original_glob(pattern, *args))
        end
      end
    end

    if block_given?
      return yield(result)
    else
      return result
    end

  end

  def self.chdir(dir)
    if dir.to_s.chars.first == ':'
      # DLM: yeah sorry, don't know what to do here
      absolute_path = OpenStudio.get_absolute_path(dir)
      if block_given?
        return yield(absolute_path)
      else
        return absolute_path
      end
    end

    if block_given?
      return yield(self.original_chdir(dir))
    else
      return self.original_chdir(dir)
    end

  end
end

require 'fileutils'
module FileUtils
  class << self
    alias :original_cp_r :cp_r
    alias :original_cp :cp
  end

  def self.cp_r(src, dest, options = {})
    #puts "cp_r #{src} to #{dest}"
    if src.to_s.chars.first == ':' then
      absolute_path = OpenStudio.get_absolute_path(src)

      #puts "cp_r absolute_path = #{absolute_path}"

      # Loop through all he files in the embedded system
      matches = []
      EmbeddedScripting.allFileNamesAsString.split(';').each do |file|
        # Skip files outside of the specified directory
        next unless file.start_with?(absolute_path)

        # If here, found a match
        matches << file
      end

      if matches.count > 0 then
        matches.each do |absolute_path|
          #puts "do copy absolute_path = #{absolute_path}"
          s = EmbeddedScripting::getFileAsString(absolute_path)

          new_dest = dest
          if matches.count > 1
            #puts "dest is dir = #{absolute_path}"
            new_dest = File.join(dest, File.basename(absolute_path))
          end
          #puts "new_dest = #{new_dest}"
          FileUtils.mkdir_p(File.dirname(new_dest))

          File.open(new_dest, 'w') do |f|
            f.puts s
          end
        end
        return true
      else
        puts "FileUtils.cp_r cannot find embedded file '#{absolute_path}' for '#{src}'"
        return false
      end
    end

    self.original_cp_r(src, dest, options)
  end

  def self.cp(src, dest, options = {})
    #puts "cp #{src} to #{dest}"
    if src.to_s.chars.first == ':' then
      absolute_path = OpenStudio.get_absolute_path(src)

      # TODO: if src is a directory

      if EmbeddedScripting::hasFile(absolute_path) then
        s = EmbeddedScripting::getFileAsString(absolute_path)

        if File.directory?(dest)
          dest = File.join(dest, File.basename(src))
        end
        FileUtils.mkdir_p(File.dirname(dest))

        File.open(dest, 'w') do |f|
          f.puts s
        end
        return true
      else
        puts "FileUtils.cp cannot find embedded file '#{absolute_path}' for '#{src}'"
        return false
      end
    end

    self.original_cp(src, dest, options)
  end

  def self.copy(src, dest, options = {})
    return self.cp(src, dest, options)
  end
end

require 'find'
module Find
  class << self
    alias :original_find :find
  end

  def self.find(*paths, ignore_error: true)
    block_given? or return enum_for(__method__, *paths, ignore_error: ignore_error)

    fs_encoding = Encoding.find("filesystem")

    all_paths = paths.collect! do |d|

      # this is overriden to ignore files on embedded file system
      if d.to_s.chars.first == ':'
        #puts "Find.find skipping '#{d}'"
        #STDOUT.flush
        nil
        next
      end

      raise Errno::ENOENT unless File.exist?(d)
      d.dup
    end

    # this is overriden to ignore files on embedded file system
    all_paths.reject! {|x| x.nil?}

    all_paths.each do |path|
      #puts "all_paths path = '#{path}'"
      #STDOUT.flush
      path = path.to_path if path.respond_to? :to_path
      enc = path.encoding == Encoding::US_ASCII ? fs_encoding : path.encoding
      ps = [path]
      while file = ps.shift
        catch(:prune) do
          yield file.dup.taint
          begin
            s = File.lstat(file)
          rescue Errno::ENOENT, Errno::EACCES, Errno::ENOTDIR, Errno::ELOOP, Errno::ENAMETOOLONG
            raise unless ignore_error
            next
          end
          if s.directory? then
            begin
              fs = Dir.entries(file, encoding: enc)
            rescue Errno::ENOENT, Errno::EACCES, Errno::ENOTDIR, Errno::ELOOP, Errno::ENAMETOOLONG
              raise unless ignore_error
              next
            end
            fs.sort!
            fs.reverse_each {|f|
              next if f == "." or f == ".."
              f = File.join(file, f)
              ps.unshift f.untaint
            }
          end
        end
      end
    end
  end
end

require 'rbconfig'

module RbConfig
  def RbConfig.ruby
    EmbeddedScripting::applicationFilePath;
  end
end