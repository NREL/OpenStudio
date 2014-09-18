#!/usr/bin/ruby

windowGroups = File.open("mapping.rad")
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