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

windowGroups = File.open("../../bsdf/mapping.rad")
windowGroups.each do |wg|

  next if wg[0] == "#"
  windowGroup = wg.split(",")[0]
  next if windowGroup == "WG0"

  wgIllumFiles = Dir.glob("#{windowGroup}_*.ill")

  shadeControlSetpoint = wg.split(",")[3].to_f

  puts "generating shade schedule for window group '#{windowGroup}', setpoint: #{shadeControlSetpoint} lux..."  

  windowControls = File.open("window_controls.ill", "r")

  windowControls.each do |row|
    data = row.split(" ")
    wgIllum_0 = File.readlines("#{wgIllumFiles[0]}")
    wgIllum_1 = File.readlines("#{wgIllumFiles[1]}")

    wgMerge = []
    wgShadeSched = []

    data.each_index do |i|

      if data[i].to_f < shadeControlSetpoint
        wgMerge << wgIllum_0[i]
        wgShadeSched << "0\n"
      else
        wgMerge << wgIllum_1[i]
        wgShadeSched << "1\n"
      end
    
    end

    wgIllum = File.open("#{windowGroup}.ill", "w")
    wgShade = File.open("#{windowGroup}.shd", "w")
    wgMerge.each {|ts| wgIllum.print "#{ts}"}
    wgShadeSched.each {|sh| wgShade.print "#{sh}"}
    wgIllum.close
    wgShade.close

  end

end