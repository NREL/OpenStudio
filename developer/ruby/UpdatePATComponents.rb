######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
#   Update the PAT construction oscs to ensure that they include 
#   standards tags.
#
# == Usage
#
#   ruby UpdatePATComponents.rb ARGV[0]
#     
#     ARGV[0] - Path to PAT components folder.
#
######################################################################

require 'openstudio'

include OpenStudio

componentsPath = Path.new(ARGV[0])

# Update Walls
puts "Processing walls."
wallsPath = componentsPath / Path.new("constructions/exteriorwalls")
wallFiles = Dir.glob(wallsPath.to_s + "./**/*.osc")
wallFiles.each { |wallFile|

  wallComponent = Model::Component::load(toPath(wallFile))
  raise "Unable to load '" + wallFile + "'." if wallComponent.empty?
  wallComponent = wallComponent.get
  
  model = Model::Model.new
  cd = model.insertComponent(wallComponent)
  raise "Unable to insert '" + wallFile + "' component into an empty model." if cd.empty?
  cd = cd.get
  
  wallConstruction = cd.primaryComponentObject.to_Construction
  raise "Unable to cast primary object of '" + wallFile + "' to Construction." if wallConstruction.empty?
  wallConstruction = wallConstruction.get
  
  wallConstructionType = String.new
  if Regexp.new(/Mass/).match(wallFile)
    # "Mass" -> "MassLightWall" if construction.heatCapacity() < 7.0 Btu/ft^2*R
    # "Mass" -> "MassHeavyWall" if construction.heatCapacity() >= 15.0 Btu/ft^2*R
    # "Mass" -> "MassMediumWall" otherwise        
    heatCapacityUnitsInBtu = OpenStudio::BTUUnit.new(OpenStudio::BTUExpnt.new(1,-2,0,-1))
    lightMassCutoffInBtu = OpenStudio::Quantity.new(7.0,heatCapacityUnitsInBtu)
    lightMassCutoffInSI = OpenStudio::convert(lightMassCutoffInBtu,1) # 1 is enum value for SI
    heavyMassCutoffInBtu = OpenStudio::Quantity.new(15.0,heatCapacityUnitsInBtu)
    heavyMassCutoffInSI = OpenStudio::convert(heavyMassCutoffInBtu,1) # 1 is enum value for SI
          
    heatCapacity = wallConstruction.heatCapacity
    raise "Unable to determine heat capacity for " + wallConstruction.briefDescription + "." if heatCapacity.empty?
    heatCapacity = heatCapacity.get
    if (heatCapacity < lightMassCutoffInSI.value) 
      wallConstructionType = "MassLightWall"
      puts "Setting construction type of " + wallConstruction.briefDescription + 
           " to MassLightWall beacuse its heat capacity is " + heatCapacity.to_s + " < " + 
           lightMassCutoffInSI.to_s + "."
    elsif (heatCapacity >= heavyMassCutoffInSI.value) 
      wallConstructionType = "MassHeavyWall"
      puts "Setting construction type of " + wallConstruction.briefDescription + 
           " to MassHeavyWall because its heat capacity is " + heatCapacity.to_s + " >= " +
           heavyMassCutoffInSI.to_s + "."
    else
      wallConstructionType = "MassMediumWall"
      puts "Setting construction type of " + wallConstruction.briefDescription + 
	   " to MassMediumWall because its heat capacity is " + lightMassCutoffInSI.to_s + 
	   ">=" + heatCapacity.to_s + " < " + heavyMassCutoffInSI.to_s + "."
    end    
  elsif Regexp.new(/Metal Building/).match(wallFile)
    wallConstructionType = "MetalBuildingWall"
  elsif Regexp.new(/Steel-Framed/).match(wallFile)
    wallConstructionType = "MetalFramingWall"
  elsif Regexp.new(/Wood-Framed and Other/).match(wallFile)
    wallConstructionType = "WoodFramingandOtherWall"
  end
  
  wallConstruction.standardsInformation.setConstructionType(wallConstructionType)
  
  wallComponent = wallConstruction.createComponent
  wallComponent.save(toPath(wallFile),true)
  
}

# Update Roofs
puts "Processing roofs."
roofsPath = componentsPath / Path.new("constructions/roofs")
roofFiles = Dir.glob(roofsPath.to_s + "./**/*.osc")
roofFiles.each { |roofFile|

  roofComponent = Model::Component::load(toPath(roofFile))
  raise "Unable to load '" + roofFile + "'." if roofComponent.empty?
  roofComponent = roofComponent.get
  
  model = Model::Model.new
  cd = model.insertComponent(roofComponent)
  raise "Unable to insert '" + roofFile + "' component into an empty model." if cd.empty?
  cd = cd.get
  
  roofConstruction = cd.primaryComponentObject.to_Construction
  raise "Unable to cast primary object of '" + roofFile + "' to Construction." if roofConstruction.empty?
  roofConstruction = roofConstruction.get
  
  roofConstructionType = String.new
  if Regexp.new(/Metal Building/).match(roofFile)
    roofConstructionType = "MetalBuildingRoof"
  else
    roofConstructionType = "WoodFramingAndOtherRoof"
  end
  
  roofConstruction.standardsInformation.setConstructionType(roofConstructionType)
  
  roofComponent = roofConstruction.createComponent
  roofComponent.save(toPath(roofFile),true)
  
}
