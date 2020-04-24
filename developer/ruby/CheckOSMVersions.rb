#!/usr/bin/env ruby

# This script ensures that the library OSM files are at the same version as
# openstudio itself (or rather will be, after build).
# It is meant as a check prior to an official release.
# Note that to update the library files themselves, you can use the script
# "UpdateOSMVersions.rb"

require_relative 'OSMVersionsLib.rb'


# Custom error class
class VersionError < StandardError
end

cmake_path = File.join(ROOT_DIR, 'CMakeLists.txt')
os_version_str = parse_version_from_cmakelist(cmake_path)
puts "CMakeLists has OpenStudio at version #{os_version_str}"

all_osms = find_resource_osms()

mismatches = check_all_osm_versions(all_osms, os_version_str)
if mismatches.size > 0
  raise VersionError, "#{mismatches.size} library/resources OSM(s) have not been updated to #{os_version_str}"
else
  puts "OK: all #{all_osms.size} OSMs are correctly at #{os_version_str}."
end
