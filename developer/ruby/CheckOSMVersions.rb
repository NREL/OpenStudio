#!/usr/bin/env ruby

# This script ensures that the library OSM files are at the same version as
# openstudio itself (or rather will be, after build).
# It is meant as a check prior to an official release.
# Note that to update the library files themselves, you can use the script
# "UpdateHVACLibrary.rb"

require 'pathname'

ROOT_DIR = File.absolute_path(File.join(File.dirname(__FILE__), "../../"))


# Custom error class
class VersionError < StandardError
end

# Parse the CMakeLists.txt for the current version
# It greps for 'project(OpenStudio VERSION X.Y.Z)
# It will raise if something goes wrong
#
# @param cmake_path [String] The path to openstudiocore CMakeLists.txt
# @return [String] the resulting version, eg "2.8.0".
def parse_version_from_cmakelist(cmake_path)
  version_line = nil
  File.open(cmake_path) do |f|
    version_line = f.grep(/project\(OpenStudio VERSION/)
  end

  if version_line.nil?
    raise "Couldn't find 'project(OpenStudio VERSION' in #{cmake_path}"
  elsif !((version_line.class == Array) && (version_line.size == 1))
    raise "Expected to find only one version in #{cmake_path}, found: #{version_line}"
  end
  os_version_str = version_line[0].match(/VERSION\s+(\d+\.\d+\.\d+)/).captures[0]

  return os_version_str
end

# Find all OSMs of interest
#
# @param None
# @return [Array] Array of paths to the OSMs
def find_resource_osms()

  # Glob all OSMs
  path = File.join(ROOT_DIR, 'openstudiocore/**/*.osm')
  files = Dir.glob(path)
  # Only keep the one we're interested in
  files = files.grep(/openstudio_app\/Resources|sketchup_plugin\/resources\/templates|sketchup_plugin\/user_scripts/)
  return files

end


# Parse the Version object in an OSM
# Does this by loading the file and grepping the version object (does not rely
# on 'openstudio' bindings)
# Will raise if something goes wrong
#
# @param cmake_path [osm_path] The path to OSM
# @return [String] the resulting version, eg "2.8.0"
def parse_osm_version(osm_path)

  this_version = File.read(osm_path).scan(/OS:Version,.*?(\d+\.\d+\.\d+)\s*;/m)

  if !((this_version.class == Array) && (this_version.size == 1) &&
       (this_version[0].class == Array) && (this_version[0].size == 1))
    raise "Something went wrong looking for version in #{osm_path},"
          "found: #{this_version}"
  end

  osm_version_str = this_version[0][0]

  return osm_version_str
end



cmake_path = File.join(ROOT_DIR, 'openstudiocore/CMakeLists.txt')
os_version_str = parse_version_from_cmakelist(cmake_path)
puts "CMakeLists has OpenStudio at version #{os_version_str}"

all_osms = find_resource_osms()

has_mismatch = false
all_osms.each do |fpath|

  # Path from project root, for reporting
  frelpath = Pathname.new(fpath).relative_path_from(Pathname.new(ROOT_DIR)).to_s

  osm_version_str = parse_osm_version(fpath)

  if osm_version_str != os_version_str
    puts "Version mistmatch for #{frelpath}, found: #{osm_version_str}, expected #{os_version_str}"
    has_mismatch = true
  end
end

if has_mismatch
  raise VersionError, "Some of the library/resources OSMs have not been updated to #{os_version_str}"
end
