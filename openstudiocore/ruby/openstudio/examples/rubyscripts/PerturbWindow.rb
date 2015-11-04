require 'openstudio'
require 'optparse'

# define the input parameters
options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on( '-u', '--uvalue UValue', Float, "UValue of Wall" ) do |uvalue|
    options[:uvalue] = uvalue
  end
  
  opts.on( '-c', '--cost COST', Float, "Cost per m2 of Wall" ) do |cost|
    options[:cost] = cost
  end
  
end

optparse.parse!

if not options[:uvalue] or not options[:cost] 
  puts optparse
  exit
end

uvalue = options[:uvalue].to_f
cost = options[:cost].to_f

# load the input file
input_path = OpenStudio::Path.new("in.osm")
input_file = OpenStudio::IdfFile::load(input_path).get
model = OpenStudio::Model::Model.new(input_file)

# define the material
windowMaterial = OpenStudio::Model::SimpleGlazing.new(model)
windowMaterial.setName("WindowMaterial")
windowMaterial.setUFactor(uvalue)
windowMaterial.setSolarHeatGainCoefficient(0.7)
windowMaterial.setVisibleTransmittance(0.7)

materials = OpenStudio::Model::FenestrationMaterialVector.new(1,windowMaterial)
 
# define the construction
windowConstruction = OpenStudio::Model::Construction.new(materials)
windowConstruction.setName("WindowConstruction")

# define the construction cost
windowConstructionCost = OpenStudio::Model::LifeCycleCost.new(windowConstruction)
windowConstructionCost.setName("WindowConstruction Cost")
windowConstructionCost.setCost(cost)
windowConstructionCost.setCostUnits("CostPerArea")

# set all windows on exterior walls to use this construction
model.getSurfaces.each do |surface|
  if surface.surfaceType.downcase == "wall"
    if surface.outsideBoundaryCondition.downcase == "outdoors"
      surface.subSurfaces.each do |subSurface|
        if subSurface.subSurfaceType.downcase.match(/window/)
          subSurface.setConstruction(windowConstruction)
        end
      end
    end
  end
end

# write out the perturbed file
output_path = "out.osm"
File.open(output_path, 'w') do |f| 
  f.puts model.to_s
end

