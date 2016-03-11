# Copies binaries and Ruby libs, tests, and examples to a package directory
# builds a Ruby gem and then transfers to the main build directory
#
# Inputs:
#   ARGV[0] - path to configured files

require  ARGV[0] + 'config'
require 'pathname'
require 'rubygems'
require 'rake' # for FileList
require 'fileutils'

include FileUtils

# check that called from command line directly
if not ($0 == __FILE__)
  puts "#{__FILE__} called from external script"
  exit
end

# figure out if this is msvc
msvc = /mswin/.match(RUBY_PLATFORM)

# figure out if this is windows
windows = msvc or /mingw/.match(RUBY_PLATFORM)

#~ # find products to include
if msvc
  binaries = Dir.glob("#{$ZeroKit_BinaryDir }*.dll")
else
  binaries = Dir.glob("#{$ZeroKit_BinaryDir}*.so")
end

# remove everything except lib
Dir.glob("*").each do |f|
  if not File.basename(f).match("lib")
    rm_rf(f)
  end
end

# remove everything except config.rb and test.rb (configured by CMake
Dir.glob("lib/*").each do |f|
  if not File.basename(f).match("config.rb") and not File.basename(f).match("test.rb")
    rm_rf(f)
  end
end

# make directories
mkdir_p("./lib")
mkdir_p("./lib/resources")
mkdir_p("./lib/resources/utilities")
mkdir_p("./lib/resources/energyplus")
mkdir_p("./lib/resources/radiance")
mkdir_p("./test")
mkdir_p("./ext")
mkdir_p("./doc")
mkdir_p("./doc/rdoc")
mkdir_p("./doc/cpp")

# copy files
cp("#{$ZeroKit_LibPath}README", ".")
cp("#{$ZeroKit_LibPath}LICENSE", ".")
cp_r(binaries, "./lib/.")
cp_r($ZeroKit_LibPath + ".", "./lib/.")

# copy resources, kind of ugly but this ignores paths with unicode characters that would otherwise crash
# also, fileutils kind of sucks at doing file operations
resources = Dir.glob("#{$ZeroKit_ResourceSourcePath}utilities/**/*.*") +
            Dir.glob("#{$ZeroKit_ResourceSourcePath}energyplus/**/*.*") +
            Dir.glob("#{$ZeroKit_ResourceSourcePath}radiance/**/*.*")
resources.each do |old_r|
  new_r = Pathname.new(old_r).sub("#{$ZeroKit_ResourceSourcePath}", "./lib/resources/")
  mkpath(new_r.dirname())
  cp(old_r, new_r)
end

# copy documentation
cp_r($ZeroKit_RDocPath + ".", "./doc/rdoc/.")
cp_r($ZeroKit_CppDocPath + ".", "./doc/cpp/.")

# remove all svn and CMakeLists.txt files
Dir.glob("./**/\.svn").each do |p|
  rm_rf(p)
end
Dir.glob("./**/CMakeLists.txt").each do |p|
  rm_f(p)
end
Dir.glob("./**/*.in").each do |p|
  rm_f(p)
end

