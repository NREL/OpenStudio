########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

# add binary dir to system path
original_path = ENV['PATH']
if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)

  if defined?(RubyInstaller::Runtime)
    require 'pathname'
    # This is an absolute path
    cur_location = Pathname.new(__dir__)
    build_type = cur_location.basename.to_s
    if build_type == "Debug" or build_type == "Release" or build_type == "RelWithDebugInfo" or build_type == "RelMinSize"
      # Multi configuration build dir:
      # We're at `build/Products/ruby/Release/openstudio.rb`
      # and we want openstudiolib.dll in `Products/Release`
      abs_dll_path = cur_location.parent.parent.join(build_type)
      # First thing is done in there is to foward to DllDirectory, which does
      # File.expand_path anyways in the initialize method
      # puts "build_type=#{build_type}, abs_dll_path=#{abs_dll_path}"
      RubyInstaller::Runtime.add_dll_directory(abs_dll_path)
    elsif cur_location.parent.basename.to_s == "Products"
      # This is a build dir execution, but without a multi configuration
      # generator such as Ninja.
      # We're at `build/Products/ruby` and we want `build/Products/`
      abs_dll_path =  cur_location.parent
      # puts "cur_location.parent.basename=#{cur_location.parent.basename}, abs_dll_path=#{abs_dll_path}"
      RubyInstaller::Runtime.add_dll_directory(abs_dll_path)
    else
      # install dir execution. We at `install/Ruby/openstudio.rb'
      # and we want `install/bin`
      ['bin', 'lib'].each do |p|
        d = cur_location.join('..', p)
        if d.exist?
          RubyInstaller::Runtime.add_dll_directory(d)
        end
      end
    end
  end
else
  # Do something here for Mac OSX environments
  ENV['PATH'] = "#{__dir__}:#{original_path}"
end

# require openstudio.so
require_relative 'openstudio.so'

# restore original path
ENV['PATH'] = original_path

# add this directory to Ruby load path
$:.unshift(__dir__)
