########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'stringio'

module EmbeddedScripting
  @@fileNames = EmbeddedScripting::allFileNamesAsString.split(';')

  def self.fileNames
    @@fileNames
  end
end


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
      if /^[A-Za-z]\:/.match(absolute_path.strip)
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

# We patch Kernel.open, and IO.open
# to read embedded files as a StringIO, not as a FileIO
# But Gem.open_file for eg expects it to be a File, not a StringIO, and on
# Windows it will call File::flock (file lock) to protect access, but StringIO
# does not have this method
class FakeFileAsStringIO < StringIO
  def flock(operation)
    return 0
  end
end

module Kernel
  # ":" is our root path to the embedded file system
  # make sure it is in the ruby load path
  # TJC remove RUBYLIB env and use --include or -I via cli args instead
  #if ENV['RUBYLIB']
  #  ENV['RUBYLIB'].split(File::PATH_SEPARATOR).each {|lib| $LOAD_PATH.unshift(lib)}
  #end

  # ruby -e 'puts $LOAD_PATH', this comes from loadpath.c (ruby_initial_load_paths)
  # <ruby_dir>/lib/ruby/site_ruby/3.2.0
  # <ruby_dir>/lib/ruby/site_ruby/3.2.0/x86_64-linux
  # <ruby_dir>/lib/ruby/site_ruby
  # <ruby_dir>/lib/ruby/vendor_ruby/3.2.0
  # <ruby_dir>/lib/ruby/vendor_ruby/3.2.0/x86_64-linux
  # <ruby_dir>/lib/ruby/vendor_ruby
  # <ruby_dir>/lib/ruby/3.2.0
  # <ruby_dir>/lib/ruby/3.2.0/x86_64-linux

  # Change 3.2.2 to 3.2.0
  RUBY_V = (RUBY_VERSION.split('.')[0..1] + ['0']).join('.')
  # Debug
  # puts "Initial load path:
  # puts $LOAD_PATH

  # TODO: double check and update platform-specific includes
  $LOAD_PATH << ':'
  $LOAD_PATH << ":/ruby/site_ruby/#{RUBY_V}"
  $LOAD_PATH << ":/ruby/site_ruby/#{RUBY_V}/#{RUBY_PLATFORM}"
  $LOAD_PATH << ":/ruby/site_ruby"
  $LOAD_PATH << ":/ruby/vendor_ruby/#{RUBY_V}"
  $LOAD_PATH << ":/ruby/vendor_ruby/#{RUBY_V}/#{RUBY_PLATFORM}"
  $LOAD_PATH << ":/ruby/vendor_ruby"
  $LOAD_PATH << ":/ruby/#{RUBY_V}"
  $LOAD_PATH << ":/ruby/#{RUBY_V}/#{RUBY_PLATFORM}"

  # puts "==============="
  # puts "New load path:"
  # puts $LOAD_PATH

  # TODO: keep fallbacks?
  # $LOAD_PATH << ':/ruby/3.2.0'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin16'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin17'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin18'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin19'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin20'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin21'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-darwin22'
  # $LOAD_PATH << ':/ruby/3.2.0/arm64-darwin20'
  # $LOAD_PATH << ':/ruby/3.2.0/arm64-darwin21'
  # $LOAD_PATH << ':/ruby/3.2.0/arm64-darwin22'
  # $LOAD_PATH << ':/ruby/3.2.0/arm64-darwin23'
  # $LOAD_PATH << ':/ruby/3.2.0/x86_64-linux'
  # $LOAD_PATH << ':/ruby/3.2.0/aarch64-linux'
  # $LOAD_PATH << ':/ruby/3.2.0/x64-mswin64_140'
  # DLM: now done in embedded gem initialization section in openstudio_cli.rb
  #$LOAD_PATH << EmbeddedScripting::findFirstFileByName('openstudio-standards.rb').gsub('/openstudio-standards.rb', '')
  #$LOAD_PATH << EmbeddedScripting::findFirstFileByName('openstudio-workflow.rb').gsub('/openstudio-workflow.rb', '')
  $LOADED = []

  alias :original_require_relative :require_relative
  alias :original_require :require
  alias :original_open :open

  RUBY_FILE_EXTS = ['.rb', '.so', '.dll']
  # Consider moving this map into openstudio-gems project
  # and maintain this list from there
  EMBEDDED_EXT_INITS = {\
    'libll' => 'init_libll',\
    'liboga' => 'init_liboga',\
    'sqlite3/sqlite3_native' => 'init_sqlite3_native',\
    'jaro_winkler_ext' => 'init_jaro_winkler_ext',\
    'msgpack/msgpack' => 'init_msgpack',\
    'byebug/byebug' => 'init_byebug',\
    'json/ext/parser' => 'init_parser',\
    'json/ext/generator' => 'init_generator'
    #'cbor/cbor' => 'init_cbor',\
  }

  def require_embedded_extension path
    if EMBEDDED_EXT_INITS.has_key? path
      $LOADED << path
      EmbeddedScripting.send(EMBEDDED_EXT_INITS[path])
      return true
    else
      return false
    end
  end

  def require path
    result = false
    original_directory = Dir.pwd
    path_with_extension = path

    begin
      # Returns the extension
      # (the portion of file name in path starting from the last period).
      extname = File.extname(path)

      if extname.empty? or ! RUBY_FILE_EXTS.include? extname
        path_with_extension = path + '.rb'
      end

      if path.include? 'openstudio/energyplus/find_energyplus'
        return false
      elsif path_with_extension.to_s.chars.first == ':'
        # Give absolute embedded paths first priority
        if $LOADED.include?(path_with_extension)
           return true
        else
          return require_embedded_absolute(path_with_extension)
        end
      elsif path_with_extension == 'openstudio.rb'
        # OpenStudio is loaded by default and does not need to be required
        return true
      elsif require_embedded(path_with_extension, $LOAD_PATH)
        # Load embedded files that are no required using full paths now
        # This does not included the openstudio-gems set of default, baked in gems
        # because we want to give anything provided by --bundle the first chance
        return true
      else
        # This will pick up files from the normal filesystem,
        # including things in the --bundle
        result = original_require path
      end
    rescue Exception => e
      # This picks up the embedded gems
      # Important to do this now, so that --bundle has first chance
      # using rescue in normal program flow, might have poor performance
      # (it does in C++) but this is perhaps the only way
      # $EMBEDDED_GEM_PATH is populated in openstudio_cli.rb during startup
      result = require_embedded(path_with_extension, $EMBEDDED_GEM_PATH)

      # Finally try to load any supported native extensions
      if not result
        result = require_embedded_extension path
      end

      # Give up. original_require will throw and so
      # this will preserve that behavior
      if not result
        raise e
      end
    ensure
      current_directory = Dir.pwd
      if original_directory != current_directory
        Dir.chdir(original_directory)
        STDOUT.flush
      end
    end

    return result
  end

  def require_embedded(path, search_paths)
    search_paths = [] if not search_paths
    search_paths.each do |p|
      if p.to_s.chars.first == ':'
        embedded_path = p + '/' + path
        if $LOADED.include?(embedded_path)
          return true
        elsif EmbeddedScripting::hasFile(embedded_path)
          require_embedded_absolute(embedded_path)
          return true
        end
      end
    end
    return false
  end

  def require_embedded_absolute path
    original_directory = Dir.pwd

    $LOADED << path
    s = EmbeddedScripting::getFileAsString(path)
    s = OpenStudio::preprocess_ruby_script(s)

    result = Kernel::eval(s,BINDING,path)


    current_directory = Dir.pwd
    if original_directory != current_directory
      Dir.chdir(original_directory)
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
    if File.exist?(absolute_path)
      File.open(absolute_path, mode) do |file|
        result = file.read
      end
    elsif File.exist?(path)
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
    EmbeddedScripting::fileNames.each do |file|
      # Skip files outside of the specified directory
      next unless file.start_with?(absolute_path)
      # Skip files that don't match the file_name_pattern criterion
      next unless File.basename(file).match(file_name_regexp)
      # If here, found a match
      matches << file
    end

    return matches
  end

  def open(name, *args, **options)
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
          io = FakeFileAsStringIO.open(string)
          begin
            result = yield(io)
          ensure
            io.close
          end
          return result
        else
          return FakeFileAsStringIO.open(string)
        end
      else
        #puts "IO.open cannot find embedded file '#{absolute_path}' for '#{name}'"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = FakeFileAsStringIO.open("")
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
      io = original_open(name, *args, **options)
      begin
        result = yield(io)
      ensure
        io.close
      end
      return result
    else
      return original_open(name, *args, **options)
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

  # NOTES ruby2.7+ now issues warning: "Using the last argument as keyword parameters is deprecated"
  # https://www.ruby-lang.org/en/news/2019/12/12/separation-of-positional-and-keyword-arguments-in-ruby-3-0/
  # Fix by capturing keywords in options hsah

  def self.read(name, *args, **options)
    if name.to_s.chars.first == ':' then
      #puts "self.read(name, *args), name = #{name}, args = #{args}, options = #{options}"
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

    return original_read(name, *args, **options)
  end

  def self.open(name, *args, **options)

    if name.to_s.chars.first == ':' then
      #puts "self.open(name, *args), name = #{name}, args = #{args}, options = #{options}"
      absolute_path = OpenStudio.get_absolute_path(name)
      #puts "absolute_path = #{absolute_path}"
      if EmbeddedScripting::hasFile(absolute_path) then
        string = EmbeddedScripting::getFileAsString(absolute_path)
        #puts "string = #{string}"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = FakeFileAsStringIO.open(string)
          begin
            result = yield(io)
          ensure
            io.close
          end
          return result
        else
          return FakeFileAsStringIO.open(string)
        end
      else
        puts "IO.open cannot find embedded file '#{absolute_path}' for '#{name}'"
        if block_given?
          # if a block is given, then a new IO is created and closed
          io = FakeFileAsStringIO.open("")
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
      io = self.original_open(name, *args, **options)
      begin
        result = yield(io)
      ensure
        io.close
      end
      return result
    else
      return self.original_open(name, *args, **options)
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

  def self.expand_path(file_name, *args, **options)
    if file_name.to_s.chars.first == ':' then
      #puts "self.expand_path(file_name, *args), file_name = #{file_name}, args = #{args}, options = #{options}"
      #STDOUT.flush
      return OpenStudio.get_absolute_path(file_name)
    elsif args.size == 1 && args[0].to_s.chars.first == ':' then
      #puts "2 self.expand_path(file_name, *args), file_name = #{file_name}, args = #{args}, options = #{options}"
      #puts "x = #{File.join(args[0], file_name)}"
      #puts "y = #{OpenStudio.get_absolute_path(File.join(args[0], file_name))}"
      #STDOUT.flush
      #return original_expand_path(file_name, *args)
      return OpenStudio.get_absolute_path(File.join(args[0], file_name))
    end
    return original_expand_path(file_name, *args, **options)
  end

  def self.absolute_path(file_name, *args, **options)
    if file_name.to_s.chars.first == ':' then
      #puts "self.absolute_path(file_name, *args), file_name = #{file_name}, args = #{args}, options = #{options}"
      #STDOUT.flush
      return OpenStudio.get_absolute_path(file_name)
    elsif args.size == 1 && args[0].to_s.chars.first == ':' then
      #puts "2 self.absolute_path(file_name, *args), file_name = #{file_name}, args = #{args}, options = #{options}"
      #puts "x = #{File.join(args[0], file_name)}"
      #puts "y = #{OpenStudio.get_absolute_path(File.join(args[0], file_name))}"
      #STDOUT.flush
      #return original_absolute_path(file_name, *args)
      return OpenStudio.get_absolute_path(File.join(args[0], file_name))
    end
    return original_absolute_path(file_name, *args, **options)
  end

  def self.realpath(file_name, *args, **options)
    if file_name.to_s.chars.first == ':' then
      #puts "self.realpath(file_name, *args), file_name = #{file_name}, args = #{args}, options = #{options}"
      #STDOUT.flush
      return OpenStudio.get_absolute_path(file_name)
    elsif args.size == 1 && args[0].to_s.chars.first == ':' then
      #puts "2 self.realpath(file_name, *args), file_name = #{file_name}, args = #{args}, options = #{options}"
      #puts "x = #{File.join(args[0], file_name)}"
      #puts "y = #{OpenStudio.get_absolute_path(File.join(args[0], file_name))}"
      #STDOUT.flush
      #return original_realpath(file_name, *args)
      return OpenStudio.get_absolute_path(File.join(args[0], file_name))
    end
    return original_realpath(file_name, *args, **options)
  end

  def self.directory?(file_name)
    if file_name.to_s.chars.first == ':' then
      absolute_path = OpenStudio.get_absolute_path(file_name)
      EmbeddedScripting::fileNames.each do |file|
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
      EmbeddedScripting::fileNames.each do |file|
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

  def self.glob(pattern, *args, **options)

    pattern_array = []
    if pattern.is_a? String
      pattern_array = [pattern]
    elsif pattern.is_a? Array
      pattern_array = pattern
    else
      pattern_array = pattern
    end

    #puts "Dir.glob pattern = #{pattern}, pattern_array = #{pattern_array}, args = #{args}, options = #{options}"
    override_args_extglob = false

    result = []
    pattern_array.each do |pattern|

      if pattern.to_s.chars.first == ':'

        # DLM: seems like this is needed for embedded paths, possibly due to leading ':' character?
        override_args_extglob = true

        #puts "searching embedded files for #{pattern}"
        absolute_pattern = OpenStudio.get_absolute_path(pattern)
        #puts "absolute_pattern #{absolute_pattern}"

        EmbeddedScripting::fileNames.each do |name|
          absolute_path = OpenStudio.get_absolute_path(name)

          if override_args_extglob
            if File.fnmatch( absolute_pattern, absolute_path, File::FNM_EXTGLOB )
              #puts "#{absolute_path} is a match!"
              result << absolute_path
            end
          else
            if File.fnmatch( absolute_pattern, absolute_path, *args, **options )
              #puts "#{absolute_path} is a match!"
              result << absolute_path
            end
          end

        end

      else
        if override_args_extglob
          result.concat(self.original_glob(pattern, File::FNM_EXTGLOB))
        else
          result.concat(self.original_glob(pattern, *args, **options))
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

  def self.cp_r(src, dest, ...)
    #puts "cp_r #{src} to #{dest}"
    if src.to_s.chars.first == ':' then
      absolute_path = OpenStudio.get_absolute_path(src)

      #puts "cp_r absolute_path = #{absolute_path}"

      # Loop through all he files in the embedded system
      matches = []
      EmbeddedScripting::fileNames.each do |file|
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

    self.original_cp_r(src, dest, ...)
  end

  def self.cp(src, dest, ...)
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

    self.original_cp(src, dest, ...)
  end

  def self.copy(src, dest, ...)
    return self.cp(src, dest, ...)
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
          yield file.dup # .taint
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
              ps.unshift f
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
