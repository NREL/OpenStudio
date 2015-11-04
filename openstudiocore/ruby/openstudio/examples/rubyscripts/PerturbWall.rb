require 'openstudio'
require 'optparse'

# define the input parameters
options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on( '-i', '--insulation_thickness INSULATIONTHICKNESS', Float, "Thickness of Wall Insulation" ) do |insulation_thickness|
    options[:insulation_thickness] = insulation_thickness
  end
  
  opts.on( '-c', '--cost COST', Float, "Cost per m2 of Wall" ) do |cost|
    options[:cost] = cost
  end
  
end

optparse.parse!

if not options[:insulation_thickness] or not options[:cost] 
  puts optparse
  exit
end

insulation_thickness = options[:insulation_thickness].to_f
cost = options[:cost].to_f

# load the input file
input_path = OpenStudio::Path.new("in.osm")
input_file = OpenStudio::IdfFile::load(input_path).get
model = OpenStudio::Model::Model.new(input_file)
    
# define the materials
materials = OpenStudio::Model::OpaqueMaterialVector.new

f08MetalSurface = OpenStudio::Model::StandardOpaqueMaterial.new(model)
f08MetalSurface.setName("F08 Metal surface")
f08MetalSurface.setRoughness("Smooth")
f08MetalSurface.setThickness(0.0008)
f08MetalSurface.setThermalConductivity(45.28)
f08MetalSurface.setDensity(7824.0)
f08MetalSurface.setSpecificHeat(500.0)

materials << f08MetalSurface

insulationBoard = OpenStudio::Model::StandardOpaqueMaterial.new(model)
insulationBoard.setName("Insulation board")
insulationBoard.setRoughness("MediumRough")
insulationBoard.setThickness(insulation_thickness)
insulationBoard.setThermalConductivity(0.03)
insulationBoard.setDensity(43.0)
insulationBoard.setSpecificHeat(1210.0)

materials << insulationBoard

f04WallAirSpaceResistance = OpenStudio::Model::AirGap.new(model)
f04WallAirSpaceResistance.setName("F04 Wall air space resistance")
f04WallAirSpaceResistance.setThermalResistance(0.15)

materials << f04WallAirSpaceResistance

g01a19mmGypsumBoard = OpenStudio::Model::StandardOpaqueMaterial.new(model)
g01a19mmGypsumBoard.setName("G01a 19mm gypsum board")
g01a19mmGypsumBoard.setRoughness("MediumSmooth")
g01a19mmGypsumBoard.setThickness(0.019)
g01a19mmGypsumBoard.setThermalConductivity(0.16)
g01a19mmGypsumBoard.setDensity(800.0)
g01a19mmGypsumBoard.setSpecificHeat(1090.0)

materials << g01a19mmGypsumBoard

# define the construction
wallConstruction = OpenStudio::Model::Construction.new(materials)
wallConstruction.setName("WallConstruction")

# define the construction cost
wallConstructionCost = OpenStudio::Model::LifeCycleCost.new(wallConstruction)
wallConstructionCost.setName("WallConstruction Cost")
wallConstructionCost.setCost(cost)
wallConstructionCost.setCostUnits("CostPerArea")

# set all exterior walls to use this construction
model.getSurfaces.each do |surface|
  if surface.surfaceType.downcase == "wall"
    if surface.outsideBoundaryCondition.downcase == "outdoors"
      surface.setConstruction(wallConstruction)
    end
  end
end

# write out the perturbed file
output_path = "out.osm"
File.open(output_path, 'w') do |f| 
  f.puts model.to_s
end

