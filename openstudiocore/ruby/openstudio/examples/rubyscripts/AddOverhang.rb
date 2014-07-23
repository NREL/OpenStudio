require 'openstudio'
require 'optparse'

# define the input parameters
options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on( '-d', '--depth DEPTH', Float, "Depth of Overhang (m), use with Offset" ) do |depth|
    options[:depth] = depth
  end
  
  opts.on( '-o', '--offset OFFSET', Float, "Offest of Overhang (m), use with Depth" ) do |offset|
    options[:offset] = offset
  end
  
  opts.on( '-p', '--projectionfactor PROJECTIONFACTOR', Float, "Projection Factor of Overhang, use with Offset Fraction" ) do |projectionfactor|
    options[:projectionfactor] = projectionfactor
  end
  
  opts.on( '-f', '--offsetfraction OFFSETFRACTION', Float, "Offset Fraction of Overhang, use with Projection Factor" ) do |offsetfraction|
    options[:offsetfraction] = offsetfraction
  end
  
end

optparse.parse!

if not options[:depth] and not options[:projectionfactor]
  puts optparse
  exit
end

if options[:depth] and options[:projectionfactor]
  puts optparse
  exit
end

# load the input file
input_path = OpenStudio::Path.new("in.osm")
model = OpenStudio::Model::Model::load(input_path).get

# add overhangs to subsurfaces
OpenStudio::Model::getSubSurfaces(model).each { |subSurface|
  if options[:depth]
    offset = 0.0
    if options[:offset]
      offset = options[:offset].to_f
    end 
    subSurface.addOverhang(options[:depth].to_f,offset)
  else
    offsetfraction = 0.0
    if options[:offsetfraction]
      offsetfraction = options[:offsetfraction].to_f
    end
    subSurface.addOverhangByProjectionFactor(options[:projectionfactor].to_f,offsetfraction)
  end
}

# write out the perturbed file
output_path = "out.osm"
File.open(output_path, 'w') do |f| 
  f.puts model.to_s
end
