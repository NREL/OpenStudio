#!/usr/bin/ruby

# genBSDFLib.rb
#
# Generate a library of BSDFs for OpenStudio Radiance 3-phase simulations
# v0.2 2014.02.11 initial working version - Rob Guglielmetti, NREL (RPG)
# v0.3 2014.02.12 supports diffuse mats - RPG
# v0.4 2014.02.12 supports shades, thick walls - RPG

# assumes you're on unix, have a recent Radiance install (current genBSDF), 8 cores

# todo: support the following matrix:
# [x] transmittance_type(clear/diffuse), transmittance (%), shadeType(na/blinds/shades), shadeStatus(up/down)
# [x] add shading (blinds for now, one slat position)
# [x] add translucent glass

# initialize
nTn = ""
ntn = ""
inRad = ""
inRadShades = ""
tType = ""
sType = ""
tRange = ""
rMaterial = ""
matString = ""
transA1 = ""
transA2 = ""
transA3 = ""
transA4 = ""
transA5 = ""
transA6 = ""
transA7 = ""
wallThickness = ""
revealDepth = ""
windowOffset = ""
shadeOffset = ""

# clear glass transmittance 01%-91% (>91% is unrealistic for Radiance glass primitive)
# diffuse glass transmittance 01%-75% (0% spec, 100% diff)
# todo: add warnings to OS::Radiance::forwardTranslator for out of bounds window constructions

["cl", "df"].each do |tType|

  if tType == "cl" 

    tRange = (1..91).to_a 

  elsif tType == "df"

    tRange = (1..75).to_a 

  else

    puts "invalid transmittance model"
    exit false

  end
  
  tRange.each do |t|
  
    # (transmittance: Tn; transmissivity: tn)
    # tn = (sqrt(.8402528435+.0072522239*Tn*Tn)-.9166530661)/.0036261119/Tn
    # or: tn = 1.0895 * Tn (Thanks, Axel Jacobs! (http://www.jaloxa.eu/resources/radiance/documentation/docs/radiance_cookbook.pdf, Page 21))
        
    sType = ""
    nTn = t / 100.0
    ntn = 1.0895 * nTn
    visTransmissivity = ntn
    visTransmittance = "#{nTn}"
  
    # trans crap (from "Rendering with Radiance", sec 5.2.6):
    # A7=Ts / ( Td+Ts )
    # A6=( Td+Ts ) / ( Rd+Td+Ts ) 
    # A5=Sr
    # A4=Rs
    # A3=Cb / ( (1-Rs)*(1-A6) ) 
    # A2=Cg / ( (1-Rs)*(1-A6) ) 
    # A1=Cr / ( (1-Rs)*(1-A6) )
  
    # set some constants, let's not get crazy
    tS = 0.0 # transmitted specularity
    #rD = 0.95 # diffuse reflectance (not used, monochromatic trans mats)
    sR = 0.0 # surface roughness
    rS = 0.05 # specular reflectance
    cRGB = (0.95 - nTn)
    
    # trans parameters
    transA7 = tS / (nTn+tS)
    #transA6 = (nTn + tS) / (rD + nTn + tS)
    transA6 = (nTn + tS) / (cRGB + nTn + tS)
    transA5 = sR
    transA4 = rS
    #transA3 = cRGB / (1-rS)*(1-transA6)
    #transA2 = cRGB / (1-rS)*(1-transA6)
    #transA1 = cRGB / (1-rS)*(1-transA6)
    transA3 = cRGB # monochromatic
    transA2 = cRGB # monochromatic
    transA1 = cRGB # monochromatic
      
    if tType == "cl" 

      rMaterial = "glass"
      matString = "0\n0\n3\n#{ntn} #{ntn} #{ntn}\n"

    elsif tType == "df"

      rMaterial = "trans"
      matString = "0\n0\n7\n#{transA1} #{transA2} #{transA3} #{transA4} #{transA5} #{transA6} #{transA7} \n"

    else

      puts "invalid transmittance model"
      exit false

    end
    
    # window geometry vars
    wallThickness = 0.1524 # enter *positive* float value, in meters
    
    revealDepth = wallThickness * -1
    windowOffset = revealDepth * 0.5
    shadeOffset = revealDepth * 0.1
    
    # make glazing
    inRad << "void #{rMaterial} #{tType}_#{ntn}\n"
    inRad << "#{matString}"
    inRad << "#{tType}_#{ntn} polygon inputPolygon_#{tType}_#{nTn}\n"
    inRad << "0\n0\n12\n0 0 #{windowOffset}\n0 1 #{windowOffset}\n1 1 #{windowOffset}\n1 0 #{windowOffset}\n"

    # make wall
    inRad << "void plastic frame\n0\n0\n5\n0.3 0.3 0.3 0 0\n" 
    inRad << "frame polygon windowframe\n0\n0\n12\n1 0 0\n1 0 #{revealDepth}\n0 0 #{revealDepth}\n0 0 0\n"
    inRad << "frame polygon windowframe\n0\n0\n12\n0 0 #{revealDepth}\n0 1 #{revealDepth}\n0 1 0\n0 0 0\n"
    inRad << "frame polygon windowframe\n0\n0\n12\n1 1 0\n0 1 0\n0 1 #{revealDepth}\n1 1 #{revealDepth}\n"
    inRad << "frame polygon windowframe\n0\n0\n12\n1 0 #{revealDepth}\n1 0 0\n1 1 0\n1 1 #{revealDepth}"

    # save as input to genBSDF
    out_file = File.new("#{tType}_#{nTn}#{sType}.rad", "w")
    out_file.puts(inRad)
    out_file.close

    puts "creating BSDF: #{tType}_Tn#{nTn}#{sType}.xml"

    genBSDFcommand = "genBSDF -n 8 ./#{tType}_#{nTn}#{sType}.rad > #{tType}_Tn#{nTn}#{sType}.xml"
    system(genBSDFcommand)

    # window+blinds
    if tType == "cl" # only do for clear glass    

      sType = "_blinds"
      # generate simple blinds for lazy user (slat angle blocks direct sun all year)
      inRadShades << "void plastic slatmetal\n0\n0\n5\n0.7 0.7 0.7 0 0\n"
      inRadShades << "\!genblinds slatmetal blinds_VBA45 34 1000 1000 60 45 | xform -rz -90 -rx 90 -t 0 1000.0 0 -s 0.001 -t 0 0 #{shadeOffset}"
      out_file = File.new("#{tType}_#{nTn}#{sType}.rad", "w")
      out_file.puts(inRad)
      out_file.puts(inRadShades)
      out_file.close
      puts "creating BSDF: #{tType}_Tn#{nTn}#{sType}.xml"
      genBSDFcommand = "genBSDF -n 8 ./#{tType}_#{nTn}#{sType}.rad > #{tType}_Tn#{nTn}#{sType}.xml"
      system(genBSDFcommand)

    end # blinds
  
    system("rm *.rad")
    inRad = ""
    inRadShades = ""
  
  end # transmittance type
  
end # EOF
