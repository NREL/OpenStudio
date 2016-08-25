######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class AddPhotovoltaics < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Add Photovoltaics"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    elcd_hash = Hash.new
    model.getElectricLoadCenterDistributions.each do |elcd|
      elcd_hash[elcd.name.get] = elcd
    end

    elcd_handles = OpenStudio::StringVector.new
    elcd_names = OpenStudio::StringVector.new
    
    elcd_handles << OpenStudio::toUUID("").to_s
    elcd_names << "*New Electric Load Center Distribution*"
    
    elcd_hash.sort.map do |elcd_name, elcd|
      elcd_handles << elcd.handle.to_s
      elcd_names << elcd_name
    end
    
    elcd = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("elcd", elcd_handles, elcd_names, true)
    elcd.setDisplayName("Choose Electric Load Center Distribution.")
    elcd.setDefaultValue("*New Electric Load Center Distribution*")
    result << elcd
    
    fraction_area = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("fraction_area", true)
    fraction_area.setDisplayName("Fraction of Surface Area with Active Solar Cells.")
    fraction_area.setDefaultValue(1.0)
    result << fraction_area
    
    efficiency = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("efficiency", true)
    efficiency.setDisplayName("Fixed Cell Efficiency.")
    efficiency.setDefaultValue(0.2)
    result << efficiency

    return result
  end
  

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model,runner,user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
    
    elcd = runner.getOptionalWorkspaceObjectChoiceValue("elcd",user_arguments,model) #model is passed in because of argument type
    
    # Flags to handle deletion of potentially empty ELCD and inverter
    create_elcd = false
    panel_fail = false
    
    if not elcd.empty? and not elcd.get.to_ElectricLoadCenterDistribution.empty?
      elcd = elcd.get.to_ElectricLoadCenterDistribution.get
    else
      create_elcd = true
      elcd = OpenStudio::Model::ElectricLoadCenterDistribution.new(model)
      inverter = OpenStudio::Model::ElectricLoadCenterInverterSimple.new(model)
      inverter.setInverterEfficiency(0.98)
      elcd.setInverter(inverter)
    end
    
    fraction_area = runner.getDoubleArgumentValue("fraction_area",user_arguments)
    efficiency = runner.getDoubleArgumentValue("efficiency",user_arguments)
    
    any_in_selection = false
    
    model.getPlanarSurfaces.each do |s|

      next if !runner.inSelection(s)
    
      next if !s.to_SubSurface.empty?
      
      next if !s.to_InteriorPartitionSurface.empty?
      
      next if !s.generatorPhotovoltaics.empty?
      
      any_in_selection = true
      
      panel = OpenStudio::Model::GeneratorPhotovoltaic::simple(model)
      performance = panel.photovoltaicPerformance.to_PhotovoltaicPerformanceSimple.get
      performance.setFractionOfSurfaceAreaWithActiveSolarCells(fraction_area)
      performance.setFixedEfficiency(efficiency)
      test = panel.setSurface(s)
      
      if test
        runner.registerInfo("Added GeneratorPhotovoltaic '" + panel.name.to_s + "' for PlanarSurface '" + s.name.to_s + "'")
        elcd.addGenerator(panel)
      else
        # At least one assignment of panel failed
        panel_fail = true
        runner.registerWarning("Could not add GeneratorPhotovoltaic for PlanarSurface '" + s.name.to_s + "'")
        panel.remove
      end
    
    end
    
    # If one panel assignment failed, check if the elcd and inverter are still used
    if panel_fail
      # If no generators, remove newly created inverter and ElectricLoadCenterDistribution
      if create_elcd && elcd.generators.empty?
        inverter = elcd.inverter
        if !inverter.empty?
          inverter.remove
        end
        elcd.remove
      end
    end
    
    if not any_in_selection
      runner.registerAsNotApplicable("No allowable surfaces in the current selection. Please select surfaces or shading surfaces to apply photovoltaics to.")
    end
    
    return(true)
  end
end

# this call registers your script with the OpenStudio SketchUp plug-in
AddPhotovoltaics.new.registerWithApplication
