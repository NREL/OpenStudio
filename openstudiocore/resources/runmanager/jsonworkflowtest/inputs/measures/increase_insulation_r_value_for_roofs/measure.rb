#start the measure
class IncreaseInsulationRValueForRoofs < OpenStudio::Ruleset::ModelUserScript

  #define the name that a user will see
  def name
    return "Increase R-value of Insulation for Roofs to a Specific Value"
  end

  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make an argument insulation R-value
    r_value = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("r_value",true)
    r_value.setDisplayName("Insulation R-value (ft^2*h*R/Btu).")
    r_value.setDefaultValue(30.0)
    args << r_value

    #make bool argument to allow both increase and decrease in R value
    allow_reduction = OpenStudio::Ruleset::OSArgument::makeBoolArgument("allow_reduction",true)
    allow_reduction.setDisplayName("Allow both increase and decrease in R-value to reach requested target?")
    allow_reduction.setDefaultValue(false)
    args << allow_reduction

    #make an argument for material and installation cost
    material_cost_increase_ip = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("material_cost_increase_ip",true)
    material_cost_increase_ip.setDisplayName("Increase in Material and Installation Costs for Construction per Area Used ($/ft^2).")
    material_cost_increase_ip.setDefaultValue(0.0)
    args << material_cost_increase_ip

    #make an argument for demolition cost
    one_time_retrofit_cost_ip = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("one_time_retrofit_cost_ip",true)
    one_time_retrofit_cost_ip.setDisplayName("One Time Retrofit Cost to Add Insulation to Construction ($/ft^2).")
    one_time_retrofit_cost_ip.setDefaultValue(0.0)
    args << one_time_retrofit_cost_ip

    #make an argument for duration in years until costs start
    years_until_retrofit_cost = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("years_until_retrofit_cost",true)
    years_until_retrofit_cost.setDisplayName("Year to Incur One Time Retrofit Cost (whole years).")
    years_until_retrofit_cost.setDefaultValue(0)
    args << years_until_retrofit_cost

    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    #use the built-in error checking
    if not runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    #assign the user inputs to variables
    r_value = runner.getDoubleArgumentValue("r_value",user_arguments)
    allow_reduction = runner.getBoolArgumentValue("allow_reduction",user_arguments)
    material_cost_increase_ip = runner.getDoubleArgumentValue("material_cost_increase_ip",user_arguments)
    one_time_retrofit_cost_ip = runner.getDoubleArgumentValue("one_time_retrofit_cost_ip",user_arguments)
    years_until_retrofit_cost = runner.getIntegerArgumentValue("years_until_retrofit_cost",user_arguments)

    #set limit for minimum insulation. This is used to limit input and for inferring insulation layer in construction.
    min_expected_r_value_ip = 1 #ip units

    #check the R-value for reasonableness
    if r_value < 0 or r_value > 500
      runner.registerError("The requested roof insulation R-value of #{r_value} ft^2*h*R/Btu was above the measure limit.")
      return false
    elsif r_value > 60
      runner.registerWarning("The requested roof insulation R-value of #{r_value} ft^2*h*R/Btu is abnormally high.")
    elsif r_value < min_expected_r_value_ip
      runner.registerWarning("The requested roof insulation R-value of #{r_value} ft^2*h*R/Btu is abnormally low.")
    end

    #check lifecycle arguments for reasonableness
    if not years_until_retrofit_cost >= 0 and not years_until_retrofit_cost <= 100
      runner.registerError("Year to incur one time retrofit cost should be a non-negative integer less than or equal to 100.")
    end

    #short def to make numbers pretty (converts 4125001.25641 to 4,125,001.26 or 4,125,001). The definition be called through this measure
    def neat_numbers(number, roundto = 2) #round to 0 or 2)
      if roundto == 2
        number = sprintf "%.2f", number
      else
        number = number.round
      end
      #regex to add commas
      number.to_s.reverse.gsub(%r{([0-9]{3}(?=([0-9])))}, "\\1,").reverse
    end #end def neat_numbers

    #helper to make it easier to do unit conversions on the fly
    def unit_helper(number,from_unit_string,to_unit_string)
      converted_number = OpenStudio::convert(OpenStudio::Quantity.new(number, OpenStudio::createUnit(from_unit_string).get), OpenStudio::createUnit(to_unit_string).get).get.value
    end

    #convert r_value and material_cost to si for future use
    r_value_si = unit_helper(r_value, "ft^2*h*R/Btu","m^2*K/W")
    material_cost_increase_si = unit_helper(material_cost_increase_ip,"1/ft^2","1/m^2")

    #create an array of roofs and find range of starting construction R-value (not just insulation layer)
    surfaces = model.getSurfaces
    exterior_surfaces = []
    exterior_surface_constructions = []
    exterior_surface_construction_names = []
    roof_resistance = []
    surfaces.each do |surface|
      if surface.outsideBoundaryCondition == "Outdoors" and surface.surfaceType == "RoofCeiling"
        exterior_surfaces << surface
        roof_const = surface.construction.get
        #only add construction if it hasn't been added yet
        if not exterior_surface_construction_names.include?(roof_const.name.to_s)
          exterior_surface_constructions << roof_const.to_Construction.get
        end
        exterior_surface_construction_names << roof_const.name.to_s
        roof_resistance << 1/roof_const.thermalConductance.to_f
      end
    end

    # nothing will be done if there are no exterior surfaces
    if exterior_surfaces.empty?
      runner.registerAsNotApplicable("Model does not have any roofs.")
      return true
    end

    #report strings for initial condition
    initial_string = []
    exterior_surface_constructions.uniq.each do |exterior_surface_construction|
      #unit conversion of roof insulation from SI units (M^2*K/W) to IP units (ft^2*h*R/Btu)
      initial_conductance_ip = unit_helper(1/exterior_surface_construction.thermalConductance.to_f,"m^2*K/W", "ft^2*h*R/Btu")
      initial_string << "#{exterior_surface_construction.name.to_s} (R-#{(sprintf "%.1f",initial_conductance_ip)})"
    end
    runner.registerInitialCondition("The building had #{initial_string.size} roof constructions: #{initial_string.sort.join(", ")}.")

    #hashes to track constructions and materials made by the measure, to avoid duplicates
    constructions_hash_old_new = {}
    constructions_hash_new_old = {} #used to get netArea of new construction and then cost objects of construction it replaced
    materials_hash = {}

    #array and counter for new constructions that are made, used for reporting final condition
    final_constructions_array = []

    #loop through all constructions and materials used on roofs, edit and clone
    exterior_surface_constructions.each do |exterior_surface_construction|
      construction_layers = exterior_surface_construction.layers
      max_thermal_resistance_material = ""
      max_thermal_resistance_material_index = ""
      counter = 0
      thermal_resistance_values = []

      #loop through construction layers and infer insulation layer/material
      construction_layers.each do |construction_layer|
        construction_layer_r_value = construction_layer.to_OpaqueMaterial.get.thermalResistance
        if not thermal_resistance_values.empty?
          if construction_layer_r_value > thermal_resistance_values.max
            max_thermal_resistance_material = construction_layer
            max_thermal_resistance_material_index = counter
          end
        end
        thermal_resistance_values << construction_layer_r_value
        counter = counter + 1
      end

      if not thermal_resistance_values.max > unit_helper(min_expected_r_value_ip, "ft^2*h*R/Btu","m^2*K/W")
        runner.registerWarning("Construction '#{exterior_surface_construction.name.to_s}' does not appear to have an insulation layer and was not altered.")
      elsif not thermal_resistance_values.max < r_value_si and not allow_reduction
        runner.registerInfo("The insulation layer of construction #{exterior_surface_construction.name.to_s} exceeds the requested R-Value. It was not altered.")
      else
        #clone the construction
        final_construction = exterior_surface_construction.clone(model)
        final_construction = final_construction.to_Construction.get
        final_construction.setName("#{exterior_surface_construction.name.to_s} adj roof insulation")
        final_constructions_array << final_construction

        #loop through lifecycle costs getting total costs under "Construction" or "Salvage" category and add to counter if occurs during year 0
        const_LCCs = final_construction.lifeCycleCosts
        cost_added = false
        const_LCC_cat_const = false
        updated_cost_si = 0
        const_LCCs.each do |const_LCC|
          if const_LCC.category == "Construction" and material_cost_increase_si != 0
            const_LCC_cat_const = true #need this test to add proper lcc if it didn't exist to start with
            # if multiple LCC objects associated with construction only adjust the cost of one of them.
            if not cost_added
              const_LCC.setCost(const_LCC.cost + material_cost_increase_si)
            else
              runner.registerInfo("More than one LifeCycleCost object with a category of Construction was associated with #{final_construction.name}. Cost was only adjusted for one of the LifeCycleCost objects.")
            end
            updated_cost_si += const_LCC.cost
          end
        end #end of const_LCCs.each

        #add construction object if it didnt exist to start with and a cost increase was requested
        if const_LCC_cat_const == false and material_cost_increase_si != 0
          lcc_for_uncosted_const = OpenStudio::Model::LifeCycleCost.createLifeCycleCost("LCC_increase_insulation", final_construction, material_cost_increase_si, "CostPerArea", "Construction", 20, 0).get
          runner.registerInfo("No material or installation costs existed for #{final_construction.name}. Created a new LifeCycleCost object with a material and installation cost of #{neat_numbers(unit_helper(lcc_for_uncosted_const.cost,"1/m^2","1/ft^2"))} ($/ft^2). Assumed capitol cost in first year, an expected life of 20 years, and no O & M costs.")
        end

        if cost_added
          runner.registerInfo("Adjusting material and installation cost for #{final_construction.name} to #{neat_numbers(unit_helper(updated_cost_si,"1/m^2","1/ft^2"))} ($/ft^2).")
        end

        #add one time cost if requested
        if one_time_retrofit_cost_ip > 0
          one_time_retrofit_cost_si = unit_helper(one_time_retrofit_cost_ip,"1/ft^2","1/m^2")
          lcc_retrofit_specific = OpenStudio::Model::LifeCycleCost.createLifeCycleCost("LCC_retrofit_specific", final_construction, one_time_retrofit_cost_si, "CostPerArea", "Construction", 0, years_until_retrofit_cost).get #using 0 for repeat period since one time cost.
          runner.registerInfo("Adding one time cost of #{neat_numbers(unit_helper(lcc_retrofit_specific.cost,"1/m^2","1/ft^2"))} ($/ft^2) related to retrofit of roof insulation.")
        end

        #push to hashes
        constructions_hash_old_new[exterior_surface_construction.name.to_s] = final_construction
        constructions_hash_new_old[final_construction] = exterior_surface_construction #push the object to hash key vs. name        

        #find already cloned insulation material and link to construction
        target_material = max_thermal_resistance_material
        found_material = false
        materials_hash.each do |orig,new|
          if target_material.name.to_s == orig
            new_material = new
            materials_hash[max_thermal_resistance_material.name.to_s] = new_material
            final_construction.eraseLayer(max_thermal_resistance_material_index)
            final_construction.insertLayer(max_thermal_resistance_material_index,new_material)
            found_material = true
          end
        end

        #clone and edit insulation material and link to construction
        if found_material == false
          new_material = max_thermal_resistance_material.clone(model)
          new_material = new_material.to_OpaqueMaterial.get
          new_material.setName("#{max_thermal_resistance_material.name.to_s}_R-value #{r_value} (ft^2*h*R/Btu)")
          materials_hash[max_thermal_resistance_material.name.to_s] = new_material
          final_construction.eraseLayer(max_thermal_resistance_material_index)
          final_construction.insertLayer(max_thermal_resistance_material_index,new_material)
          runner.registerInfo("For construction'#{final_construction.name.to_s}', material'#{new_material.name.to_s}' was altered.")

          #edit insulation material
          new_material_matt = new_material.to_Material
          if not new_material_matt.empty?
            starting_thickness = new_material_matt.get.thickness
            target_thickness = starting_thickness * r_value_si / thermal_resistance_values.max
            final_thickness = new_material_matt.get.setThickness(target_thickness)
          end
          new_material_massless = new_material.to_MasslessOpaqueMaterial
          if not new_material_massless.empty?
            final_thermal_resistance = new_material_massless.get.setThermalResistance(r_value_si)
          end
          new_material_airgap = new_material.to_AirGap
          if not new_material_airgap.empty?
            final_thermal_resistance = new_material_airgap.get.setThermalResistance(r_value_si)
          end
        end #end of if found material is false
      end #end of if not thermal_resistance_values.max >
    end #end of loop through unique roof constructions

    #loop through construction sets used in the model
    default_construction_sets = model.getDefaultConstructionSets
    default_construction_sets.each do |default_construction_set|
      if default_construction_set.directUseCount > 0
        default_surface_const_set = default_construction_set.defaultExteriorSurfaceConstructions
        if not default_surface_const_set.empty?
          starting_construction = default_surface_const_set.get.roofCeilingConstruction

          #creating new default construction set
          new_default_construction_set = default_construction_set.clone(model)
          new_default_construction_set = new_default_construction_set.to_DefaultConstructionSet.get
          new_default_construction_set.setName("#{default_construction_set.name.to_s} adj roof insulation")

          #create new surface set and link to construction set
          new_default_surface_const_set = default_surface_const_set.get.clone(model)
          new_default_surface_const_set = new_default_surface_const_set.to_DefaultSurfaceConstructions.get
          new_default_surface_const_set.setName("#{default_surface_const_set.get.name.to_s} adj roof insulation")
          new_default_construction_set.setDefaultExteriorSurfaceConstructions(new_default_surface_const_set)

          #use the hash to find the proper construction and link to new_default_surface_const_set
          target_const = new_default_surface_const_set.roofCeilingConstruction
          if not target_const.empty?
            target_const = target_const.get.name.to_s
            found_const_flag = false
            constructions_hash_old_new.each do |orig,new|
              if target_const == orig
                final_construction = new
                new_default_surface_const_set.setRoofCeilingConstruction(final_construction)
                found_const_flag = true
              end
            end
            if found_const_flag == false # this should never happen but is just an extra test in case something goes wrong with the measure code
              runner.registerWarning("Measure couldn't find the construction named '#{target_const}' in the exterior surface hash.")
            end
          end

          #swap all uses of the old construction set for the new
          construction_set_sources = default_construction_set.sources
          construction_set_sources.each do |construction_set_source|
            building_source = construction_set_source.to_Building
            # if statement for each type of object than can use a DefaultConstructionSet
            if not building_source.empty?
              building_source = building_source.get
              building_source.setDefaultConstructionSet(new_default_construction_set)
            end
            building_story_source = construction_set_source.to_BuildingStory
            if not building_story_source.empty?
              building_story_source = building_story_source.get
              building_story_source.setDefaultConstructionSet(new_default_construction_set)
            end
            space_type_source = construction_set_source.to_SpaceType
            if not space_type_source.empty?
              space_type_source = space_type_source.get
              space_type_source.setDefaultConstructionSet(new_default_construction_set)
            end
            space_source = construction_set_source.to_Space
            if not space_source.empty?
              space_source = space_source.get
              space_source.setDefaultConstructionSet(new_default_construction_set)
            end
          end #end of construction_set_sources.each do

        end #end of if not default_surface_const_set.empty?
      end #end of if default_construction_set.directUseCount > 0
    end #end of loop through construction sets

    #link cloned and edited constructions for surfaces with hard assigned constructions
    exterior_surfaces.each do |exterior_surface|
      if not exterior_surface.isConstructionDefaulted and not exterior_surface.construction.empty?

        #use the hash to find the proper construction and link to surface
        target_const = exterior_surface.construction
        if not target_const.empty?
          target_const = target_const.get.name.to_s
          constructions_hash_old_new.each do |orig,new|
            if target_const == orig
              final_construction = new
              exterior_surface.setConstruction(final_construction)
            end
          end
        end

      end #end of if not exterior_surface.isConstructionDefaulted and not exterior_surface.construction.empty?
    end #end of exterior_surfaces.each do

    #report strings for final condition
    final_string = []   #not all exterior roof constructions, but only new ones made. If  roof didn't have insulation and was not altered we don't want to show it
    affected_area_si = 0
    totalCost_of_affected_area = 0
    yr0_capital_totalCosts = 0
    final_constructions_array.each do |final_construction|

      #unit conversion of roof insulation from SI units (M^2*K/W) to IP units (ft^2*h*R/Btu)
      final_conductance_ip = unit_helper(1/final_construction.thermalConductance.to_f,"m^2*K/W", "ft^2*h*R/Btu")
      final_string << "#{final_construction.name.to_s} (R-#{(sprintf "%.1f",final_conductance_ip)})"
      affected_area_si = affected_area_si + final_construction.getNetArea

      #loop through lifecycle costs getting total costs under "Construction" or "Salvage" category and add to counter if occurs during year 0
      const_LCCs = final_construction.lifeCycleCosts
      const_LCCs.each do |const_LCC|
        if const_LCC.category == "Construction" or const_LCC.category == "Salvage"
          if const_LCC.yearsFromStart == 0
            yr0_capital_totalCosts += const_LCC.totalCost
          end
        end
      end

    end  #end of final_constructions_array.each do

    #add not applicable test if there were exterior roof constructions but non of them were altered (already enough insulation or doesn't look like insulated wall)
    if affected_area_si == 0
      runner.registerAsNotApplicable("No roofs were altered.")
      return true
      # affected_area_ip = affected_area_si
    else
      #ip construction area for reporting
      affected_area_ip = unit_helper(affected_area_si,"m^2","ft^2")
    end

    #report final condition
    runner.registerFinalCondition("The existing insulation for roofs was changed to R-#{r_value}. This was accomplished for an initial cost of #{one_time_retrofit_cost_ip} ($/sf) and an increase of #{material_cost_increase_ip} ($/sf) for construction. This was applied to #{neat_numbers(affected_area_ip,0)} (ft^2) across #{final_string.size} roof constructions: #{final_string.sort.join(", ")}.")

    return true

  end #end the run method

end #end the measure

#this allows the measure to be used by the application
IncreaseInsulationRValueForRoofs.new.registerWithApplication
