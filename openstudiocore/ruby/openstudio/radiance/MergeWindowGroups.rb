#!/usr/bin/ruby

######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

######################################################################
# == Synopsis 
#
#    Merge window group illuminance files, based on solar trigger setpoints.
#    Also creates shade schedules for EnergyPlus based on same solar evaluation.
#
# == Usage 
#
#  (from [osm_support_dir]/run/1-Ruby-0/in/model/radiance/output/ts):
#  ruby MergeWindowGroups.rb
#
#  Intended to be run as a post-process on a Radiance annual daylight simulation,  
#  from the Radiance "ts" output directory.
#
#
######################################################################

require "FileUtils"

# help those poor Windows users out
catCommand = "cat"
osQuote = "\'"
if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  catCommand = "type"
  osQuote = "\""
end


windowGroups = File.open("../../bsdf/mapping.rad")
windowGroups.each do |wg|

  next if wg[0] == "#"
  windowGroup = wg.split(",")[0]
  next if windowGroup == "WG0"

  wgIllumFiles = Dir.glob("#{windowGroup}_*.ill")

  shadeControlSetpoint = wg.split(",")[3].to_f

  puts "generating shade schedule for window group '#{windowGroup}', setpoint: #{shadeControlSetpoint} lux..."  


  # separate header from data; so, so ugly. 
  header = []
  ill0 = []
  ill1 = []

  wgIllum_0 = File.open("#{wgIllumFiles[0]}").each_line do |line|
    if line.chomp! =~ /^\s?\d/
      ill0 << "#{line}\n"
    else 
      header << "#{line}\n"
    end

  end

  wgIllum_1 = File.open("#{wgIllumFiles[1]}").each_line do |line|
    if line.chomp! =~ /^\s?\d/
      ill1 << "#{line}\n"
    else 
      next
    end

  end

  # should be headerless file
  windowControls = File.open("window_controls.ill", "r")

  windowControls.each do |row|
    
    data = row.split(" ")

    wgMerge = []
    wgShadeSched = []

    data.each_index do |i|

      if data[i].to_f < shadeControlSetpoint
        wgMerge << ill0[i]
        wgShadeSched << "0\n"
      else
        wgMerge << ill1[i]
        wgShadeSched << "1\n"
      end
    
    end

    wgIllum = File.open("m_#{windowGroup}.ill", "w")
    wgShade = File.open("#{windowGroup}.shd", "w")
    header.each {|head| wgIllum.print "#{head}"}
    wgMerge.each {|ts| wgIllum.print "#{ts}"}
    wgShadeSched.each {|sh| wgShade.print "#{sh}"}
    wgIllum.close
    wgShade.close
    FileUtils.rm Dir.glob('*.tmp')

  end

end

# make whole-building illuminance file
mergedWindows = Dir.glob("m_*.ill")
addFiles = ""
mergedWindows.each do |file|
  addFiles << "+ #{file} "
end
system("rmtxop -fa WG0.ill #{addFiles} > model.ill")



