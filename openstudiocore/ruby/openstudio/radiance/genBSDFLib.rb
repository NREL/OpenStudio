#!/usr/bin/ruby

# genBSDFLib.rb
#
# Generate a library of BSDFs for OpenStudio Radiance 3-phase simulations
# v0.2 2014.02.11 	initial working version - Rob Guglielmetti, NREL (RPG)
# v0.3 2014.02.12 	supports diffuse mats - RPG
# v0.4 2014.02.12 	supports shades, thick walls - RPG
# v0.8 2014.03.12 	updated filenaming for BCL taxonomy - RPG
# v1.0 2014.03.25/rpg

# assumes you're on *nix, have a recent Radiance install (genBSDF), 8 cores, lots of time on your hands.
# edit arrays to your heart's content for data subsets

# todo: support the following matrix:
# [x] transmittance_type(clear/diffuse), transmittance (%), shadeType(blinds/shades)
# [x] add shading (blinds for now, one slat position)
# [x] add translucent glass
# [x] update matrix: vlt, specular_transmitance, shade_layer_type  
# [ ] add shadecloth BSDFs

# initialize
nTn = ""					# transmittance
ntn = ""					# transmissivity 
nTs = "" 					# transmitted specular (% of total incident)
sTn = ""					# pretty string for nTn
sTs = ""					# pretty string for nTs
inRadFrame = ""		
inRadGlass = ""
inRadShades = ""
inRadBlinds = ""
tRange = ""
matStringClr = ""
matStringDif = ""
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

# clear glass transmittance 01%-90%
# diffuse glass transmittance 01%-70% (0% Tn(spec), 100% Tn(diff))

# window geometry
wallThickness = 0.1524 # enter *positive* float value, in meters

revealDepth = wallThickness * -1
windowOffset = revealDepth * 0.5
shadeOffset = revealDepth * 0.1

# make window/skylight frame geometry
inRadFrame << "void plastic frame\n0\n0\n5\n0.3 0.3 0.3 0 0\n" 
inRadFrame << "frame polygon windowframe\n0\n0\n12\n1 0 0\n1 0 #{revealDepth}\n0 0 #{revealDepth}\n0 0 0\n"
inRadFrame << "frame polygon windowframe\n0\n0\n12\n0 0 #{revealDepth}\n0 1 #{revealDepth}\n0 1 0\n0 0 0\n"
inRadFrame << "frame polygon windowframe\n0\n0\n12\n1 1 0\n0 1 0\n0 1 #{revealDepth}\n1 1 #{revealDepth}\n"
inRadFrame << "frame polygon windowframe\n0\n0\n12\n1 0 #{revealDepth}\n1 0 0\n1 1 0\n1 1 #{revealDepth}"

# generate simple blinds for lazy user (slat angle blocks most direct sun all year)
inRadBlinds << "void plastic slatmetal\n0\n0\n5\n0.7 0.7 0.7 0 0\n"
inRadBlinds << "\!genblinds slatmetal blinds_VBA45 34 1000 1000 60 45 | xform -rz -90 -rx 90 -t 0 1000.0 0 -s 0.001 -t 0 0 #{shadeOffset}"

tRange = (40..41).to_a 		# testing
# tRange = (1..90).to_a 	# production

tRange.each do |t|

	if t > 91
		puts "invalid transmittance model, quitting."
		exit
	end
	
 	if t < 1
	 	puts "you don't need a BSDF to describe an opaque surface, Donny."
	 	exit
 	end

	# transmittance=Tn
	# transmissivity=tn
	#
	# tn = (sqrt(.8402528435+.0072522239*Tn*Tn)-.9166530661)/.0036261119/Tn
	# or: 
	# tn = 1.0895 * Tn (big thanks to Axel Jacobs! (http://www.jaloxa.eu/resources/radiance/documentation/docs/radiance_cookbook.pdf, p. 21))
	 
	nTn = t / 100.0
	sTn = sprintf "%03i", t						
	ntn = 1.0895 * nTn						

	# define clear glass material
	nTs = 1.0
	sTs = sprintf "%03i", (nTs * 100)
 	matStringClr = "void glass winmat_#{sTn}_#{sTs}\n0\n0\n3\n#{ntn} #{ntn} #{ntn}\n"

	inRadGlass = ""
	inRadGlass << "#{matStringClr}"
	inRadGlass << "winmat_#{sTn}_#{sTs} polygon inputPolygon.winmat_#{sTn}_#{sTs}\n"
	inRadGlass << "0\n0\n12\n0 0 #{windowOffset}\n0 1 #{windowOffset}\n1 1 #{windowOffset}\n1 0 #{windowOffset}\n"
	
	sFilename = "OSRadWin_#{sTn}_#{sTs}_na" # _na (not applicable), _sh (shade), _bl (blinds)

	# save as input to genBSDF
	out_file = File.new("#{sFilename}.rad", "w")
	out_file.puts(inRadFrame)
	out_file.puts(inRadGlass)
	out_file.close

	puts "creating BSDF: #{sFilename}.bsdf"

	genBSDFcommand = "genBSDF -n 8 ./#{sFilename}.rad > #{sFilename}.bsdf"
	system(genBSDFcommand)

	#window+blinds
	sFilename = "OSRadWin_#{sTn}_#{sTs}_bl"

	out_file = File.new("#{sFilename}.rad", "w")
	out_file.puts(inRadFrame)
	out_file.puts(inRadGlass)
	out_file.puts(inRadBlinds)
	out_file.close
	
	puts "creating BSDF: #{sFilename}.bsdf"
	
	genBSDFcommand = "genBSDF -n 8 ./#{sFilename}.rad > #{sFilename}.bsdf"
	system(genBSDFcommand)

	# diffuse glass
 	if nTn < 71

		# trans formulae (from "Rendering with Radiance", sec. 5.2.6)
		# see also http://www.schorsch.com/en/software/rayfront/manual/transdef.html
		#
		# Td Diffuse Transmissivity (frac total)
		# Ts Transmitted Specularity (frac total)
		#
		# A7=Ts / ( Td+Ts )
		# A6=( Td+Ts ) / ( Rd+Td+Ts ) 	
		# A5=Sr																# Surface Roughness
		# A4=Rs																# Reflected Specularity
		# A3=Cb / ( (1-Rs)*(1-A6) ) 					# Diffuse Reflectance (blu)
		# A2=Cg / ( (1-Rs)*(1-A6) ) 					# Diffuse Reflectance (grn)
		# A1=Cr / ( (1-Rs)*(1-A6) )						# Diffuse Reflectance (red)

		# Unlike most other Radiance material primitives, the trans material is neither 
		# intuitive nor straightforward to apply...  -C. Ehrlich

		# set some constants
		nTs = 0.0 														# transmitted specularity
		sTs = sprintf "%03i", (nTs * 100)			# pretty print
		#rD = 0.95 														# diffuse reflectance (#not used for monochromatic trans mats#)
		sR = 0.0 															# surface roughness
		rS = 0.05 														# specular reflectance
		cRGB = (0.95 - nTn)

		# trans parameters
		transA7 = nTs / (nTn+nTs)
		# transA6 = (nTn + nTs) / (rD + nTn + nTs)
		transA6 = (nTn + nTs) / (cRGB + nTn + nTs)
		transA5 = sR
		transA4 = rS
		# transA3 = cRGB / (1-rS)*(1-transA6)
		# transA2 = cRGB / (1-rS)*(1-transA6)
		# transA1 = cRGB / (1-rS)*(1-transA6)
		transA3 = cRGB # monochromatic
		transA2 = cRGB # monochromatic
		transA1 = cRGB # monochromatic

		matStringDif = "void trans winmat_#{sTn}_#{sTs}\n0\n0\n7\n#{transA1} #{transA2} #{transA3} #{transA4} #{transA5} #{transA6} #{transA7} \n"
		# diffuse glass
		inRadGlass = ""
		inRadGlass << "#{matStringDif}"
		inRadGlass << "winmat_#{sTn}_#{sTs} polygon inputPolygon.winmat_#{sTn}_#{sTs}\n"
		inRadGlass << "0\n0\n12\n0 0 #{windowOffset}\n0 1 #{windowOffset}\n1 1 #{windowOffset}\n1 0 #{windowOffset}\n"
		sFilename = "OSRadWin_#{sTn}_#{sTs}_na" # _na (not applicable), _sh (shade), _bl (blinds)

		out_file = File.new("#{sFilename}.rad", "w")
		out_file.puts(inRadFrame)
		out_file.puts(inRadGlass)
		out_file.close
	
		puts "creating BSDF: #{sFilename}.bsdf"
		genBSDFcommand = "genBSDF -n 8 ./#{sFilename}.rad > #{sFilename}.bsdf"
		system(genBSDFcommand)

 	end # diffuse glass

 	#system("rm *.rad") 	# clean this mess up (else we'll all end up in jail)

 	inRadGlass = ""

end # EOF
 
 
 
