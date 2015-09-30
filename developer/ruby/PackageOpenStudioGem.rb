# Copies binaries and Ruby libs, tests, and examples to a package directory
# builds a Ruby gem and then transfers to the main build directory
#
# Inputs:
#   ARGV[0] - path to configured files

require ARGV[0] + 'config'
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

# check if up to date (no local modifications, splits, or partial checkouts)
if /^[0-9]+$/.match($ZeroKit_PatchVersion)
  patch = $ZeroKit_PatchVersion
else
  puts "ZeroKit_PatchVersion #{$ZeroKit_PatchVersion} incorrect, setting to 0"
  patch = 0
end

# basic specification
spec = Gem::Specification.new do |s|
  s.name = "zerokit"
  s.version = "#{$ZeroKit_MajorVersion}.#{$ZeroKit_MinorVersion}.#{patch}"
  s.author = "NREL Commercial Buildings Research"
  s.email = "zerokit@nrel.gov"
  s.homepage = "http://cbr.nrel.gov/zerokit/"
  s.rubyforge_project = 'zerokit'
  s.summary = "Zero Energy Research and Optimiziation ToolKit"
  s.platform = Gem::Platform::local
  s.files = FileList["lib/**/*"].to_a
  s.require_path = "lib"
  s.test_files = FileList["{test}/ZeroKit_Test.rb"].to_a
  s.has_rdoc = true
  s.extra_rdoc_files = FileList["doc/rdoc/**/*", "README", "LICENSE"].to_a
  s.add_dependency("rake")
end

# make gem
gem = Gem::Builder.new(spec).build

# copy file to build location
if msvc
  cp(gem, "#{$ZeroKit_BinaryDir}.")
else
  cp(gem, "#{$ZeroKit_BinaryDir}.")
end

# just copy to zerokit/gems and run 'index_gem_repository'


