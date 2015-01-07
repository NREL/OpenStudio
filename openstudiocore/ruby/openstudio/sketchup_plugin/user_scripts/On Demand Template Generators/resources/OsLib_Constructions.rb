module OsLib_Constructions

  # infer insulation layer from a construction
  def OsLib_Constructions.inferInsulationLayer(construction,minThermalResistance)

    construction_layers = construction.layers
    counter = 0
    max_resistance = 0
    thermal_resistance_array = []

    #loop through construction layers and infer insulation layer/material
    construction_layers.each do |construction_layer|
      construction_thermal_resistance = construction_layer.to_OpaqueMaterial.get.thermalResistance
      if not thermal_resistance_array.empty?
        if construction_thermal_resistance > max_resistance
          thermal_resistance_array = [construction_layer,counter,construction_thermal_resistance]
          max_resistance = construction_thermal_resistance
        end
      else
        thermal_resistance_array = [construction_layer,counter,construction_thermal_resistance]
      end
      counter = counter + 1
    end

    #test against minimum
    if max_resistance > minThermalResistance
      minTestPass = true
    else
      minTestPass = false
    end

    result = {
        "construction" => construction,
        "construction_layer" => thermal_resistance_array[0],
        "layer_index" => thermal_resistance_array[1],
        "construction_thermal_resistance" => thermal_resistance_array[2],
        "insulationFound" => minTestPass
    }

    return result
  end

  # change thermal resistance of opaque materials
  def OsLib_Constructions.setMaterialThermalResistance(material,thermalResistance,options = {})

    # set defaults to use if user inputs not passed in
    defaults = {
        "cloneMaterial" => true, # in future give user option to clone or change live material
        "name" => "#{material.name} - adj",
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    # clone input material
    new_material = material.clone(material.model)
    new_material = new_material.to_OpaqueMaterial.get
    new_material.setName(options["name"])

    #edit insulation material
    new_material_matt = new_material.to_Material
    if not new_material_matt.empty?
      starting_thickness = new_material_matt.get.thickness
      target_thickness = starting_thickness * thermalResistance / material.to_OpaqueMaterial.get.thermalResistance
      final_thickness = new_material_matt.get.setThickness(target_thickness)
    end
    new_material_massless = new_material.to_MasslessOpaqueMaterial
    if not new_material_massless.empty?
      final_thermal_resistance = new_material_massless.get.setThermalResistance(thermalResistance)
    end
    new_material_airgap = new_material.to_AirGap
    if not new_material_airgap.empty?
      final_thermal_resistance = new_material_airgap.get.setThermalResistance(thermalResistance)
    end

    result = new_material
    return result
  end

  # add new material to outside of a construction
  def OsLib_Constructions.addNewLayerToConstruction(construction,options = {})

    # set defaults to use if user inputs not passed in
    defaults = {
        "cloneConstruction" => false, # in future give user option to clone or change live construction
        "layerIndex" => 0, # 0 will be outside. Measure writer should validate any non 0 layerIndex passed in.
        "name" => "#{construction.name} - new material",
        "roughness" => nil,
        "thickness" => nil,
        "conductivity" => nil,
        "density" => nil,
        "specificHeat" => nil,
        "thermalAbsorptance" => nil,
        "solarAbsorptance" => nil,
        "visibleAbsorptance" => nil,
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    # todo - would be nice to grab surface properties from previous exposed material

    # make new material
    exposedMaterialNew = OpenStudio::Model::StandardOpaqueMaterial.new(construction.model)
    exposedMaterialNew.setName(options["name"])

    # set requested material properties
    if not options["roughness"].nil? then exposedMaterialNew.setRoughness(options["roughness"]) end
    if not options["thickness"].nil? then exposedMaterialNew.setThickness(options["thickness"]) end
    if not options["conductivity"].nil? then exposedMaterialNew.setConductivity( options["conductivity"]) end
    if not options["density"].nil? then exposedMaterialNew.setDensity(options["density"])end
    if not options["specificHeat"].nil? then exposedMaterialNew.setSpecificHeat(options["specificHeat"]) end
    if not options["thermalAbsorptance"].nil? then exposedMaterialNew.setThermalAbsorptance(options["thermalAbsorptance"]) end
    if not options["solarAbsorptance"].nil? then exposedMaterialNew.setSolarAbsorptance(options["solarAbsorptance"]) end
    if not options["visibleAbsorptance"].nil? then exposedMaterialNew.setVisibleAbsorptance(options["visibleAbsorptance"]) end

    # add material to construction
    construction.insertLayer(options["layerIndex"],exposedMaterialNew)

    result = exposedMaterialNew
    return result
  end

  # set material surface properties for specific layer in construction. this should work on OS:Material and OS:MasslessOpaqueMaterial
  def OsLib_Constructions.setConstructionSurfaceProperties(construction,options = {})

    # set defaults to use if user inputs not passed in
    defaults = {
        "cloneConstruction" => false, # in future give user option to clone or change live construction
        "nameConstruction" => "#{construction.name} - adj", # not currently used
        "cloneMaterial" => true,
        "roughness" => nil,
        "thermalAbsorptance" => nil,
        "solarAbsorptance" => nil,
        "visibleAbsorptance" => nil,
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    exposedMaterial = construction.to_LayeredConstruction.get.getLayer(0)
    if options["cloneMaterial"]

      #clone material
      exposedMaterialNew = exposedMaterial.clone(construction.model).to_StandardOpaqueMaterial.get # to_StandardOpaqueMaterial is needed to access roughness, otherwise to_OpaqueMaterial would have been fine.
      exposedMaterialNew.setName("#{exposedMaterial.name} - adj")

      # connect new material to original construction
      construction.eraseLayer(0)
      construction.insertLayer(0,exposedMaterialNew)

    else
      exposedMaterialNew = exposedMaterial.to_StandardOpaqueMaterial.get # not being cloned but still want to rename
      exposedMaterialNew.setName("#{exposedMaterial.name} - adj")
    end

    #todo - need to test with MasslessOpaqueMaterial. Add test if doesn't return anything when use to_StandardOpaqueMaterial.get

    # set requested material properties
    if not options["roughness"].nil? then exposedMaterialNew.setRoughness(options["roughness"]) end
    if not options["thermalAbsorptance"].nil? then exposedMaterialNew.setThermalAbsorptance(options["thermalAbsorptance"]) end
    if not options["solarAbsorptance"].nil? then exposedMaterialNew.setSolarAbsorptance(options["solarAbsorptance"]) end
    if not options["visibleAbsorptance"].nil? then exposedMaterialNew.setVisibleAbsorptance(options["visibleAbsorptance"]) end

    result = {"exposedMaterial" => exposedMaterial,"exposedMaterialNew" => exposedMaterialNew}
    return result

  end # end of OsLib_Constructions.setMaterialSurfaceProperties


  # similar to setMaterialSurfaceProperties but I just pass a material in. Needed this to set material properties for interior walls and both sides of interior partitions.
  def OsLib_Constructions.setMaterialSurfaceProperties(material,options = {})

    # set defaults to use if user inputs not passed in
    defaults = {
        "cloneMaterial" => true,
        "roughness" => nil,
        "thermalAbsorptance" => nil,
        "solarAbsorptance" => nil,
        "visibleAbsorptance" => nil,
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    if options["cloneMaterial"]
      #clone material
      materialNew = exposedMaterial.clone(construction.model).get
      materialNew.setName("#{materialNew.name} - adj")
    else
      materialNew = material # not being cloned
      materialNew.setName("#{materialNew.name} - adj")
    end

    # to_StandardOpaqueMaterial is needed to access roughness, otherwise to_OpaqueMaterial would have been fine.
    if not materialNew.to_StandardOpaqueMaterial.empty?
      materialNew = materialNew.to_StandardOpaqueMaterial.get
    elsif not materialNew.to_MasslessOpaqueMaterial.empty?
      materialNew = materialNew.to_MasslessOpaqueMaterial.get
    end

    # set requested material properties
    if not options["roughness"].nil? then materialNew.setRoughness(options["roughness"]) end
    if not options["thermalAbsorptance"].nil? then materialNew.setThermalAbsorptance(options["thermalAbsorptance"]) end
    if not options["solarAbsorptance"].nil? then materialNew.setSolarAbsorptance(options["solarAbsorptance"]) end
    if not options["visibleAbsorptance"].nil? then materialNew.setVisibleAbsorptance(options["visibleAbsorptance"]) end

    result = {"material" => material,"materialNew" => materialNew}
    return result

  end # end of OsLib_Constructions.setMaterialSurfaceProperties

  # sri of exposed surface of a construction (calculation from K-12 AEDG, derived from ASTM E1980 assuming medium wind speed)
  def OsLib_Constructions.getConstructionSRI(construction)
    exposedMaterial = construction.to_LayeredConstruction.get.getLayer(0)
    solarAbsorptance = exposedMaterial.to_OpaqueMaterial.get.solarAbsorptance
    thermalEmissivity = exposedMaterial.to_OpaqueMaterial.get.thermalAbsorptance
    # lines below just for testing
    # solarAbsorptance = 1 - 0.65
    # thermalEmissivity = 0.86

    x = (20.797 * solarAbsorptance - 0.603 * thermalEmissivity)/(9.5205 * thermalEmissivity + 12.0)
    sri = 123.97 - 141.35 * x + 9.6555 * x * x

    result = sri
    return result
  end # end of OsLib_Constructions.getConstructionSRI

  # create simple glazing material
  def OsLib_Constructions.createConstructionWithSimpleGlazing(model, runner = nil, options = {})

    # set defaults to use if user inputs not passed in
    defaults = {
        "constructionName" => nil,
        "materialName" => nil,
        "uFactor" => nil,
        "solarHeatGainCoef" => nil,
        "visibleTransmittance" => nil
    }

    # merge user inputs with defaults
    options = defaults.merge(options)

    # create construction and material and link them together
    construction = OpenStudio::Model::Construction.new(model)
    if not options["constructionName"].nil? then  construction.setName(options["constructionName"].to_s) end
    material = OpenStudio::Model::SimpleGlazing.new(model)
    if not options["materialName"].nil? then  material.setName(options["materialName"].to_s) end

    # add material to construction
    construction.insertLayer(0,material)

    # set material properties
    if not options["uFactor"].nil? then material.setUFactor(options["uFactor"]) end
    if not options["solarHeatGainCoef"].nil? then material.setSolarHeatGainCoefficient(options["solarHeatGainCoef"]) end
    if not options["visibleTransmittance"].nil? then material.setVisibleTransmittance(options["visibleTransmittance"]) end

    # create info message
    if not runner.nil? # todo - need to look for bad visible transmittance here
      uFactorSiToIpConversion = OpenStudio::convert(material.uFactor,"W/m^2*K","Btu/ft^2*h*R").get
      runner.registerInfo("Created #{construction.name} construction. U-factor: #{OpenStudio::toNeatString(uFactorSiToIpConversion,2,true)}(Btu/ft^2*h*R), SHGC: #{material.getSolarHeatGainCoefficient}, VT: #{material.getVisibleTransmittance.get}.")
    end

    result = construction
    return result
  end # end of OsLib_Constructions.getConstructionSRI

  # get cost of selected constructions
  def OsLib_Constructions.getTotalCostOfSelectedConstructions(constructionArray)

    envelope_cost = 0

    #loop through selected constructions
    constructionArray.each do |construction|
      next if construction.getNetArea == 0
      const_llcs = construction.lifeCycleCosts
      const_llcs.each do |const_llc|
        if const_llc.category == "Construction"
          envelope_cost += const_llc.totalCost
        end
      end
    end #end of constructions.each do

    result = envelope_cost
    return result
  end # end of OsLib_Constructions.getTotalCostOfSelectedConstructions

  # report names of constructions in a construction set
  def OsLib_Constructions.reportConstructionSetConstructions(constructionSet)

    constructionArray = []

    #populate exterior surfaces
    if constructionSet.defaultExteriorSurfaceConstructions.is_initialized
      surfaceSet = constructionSet.defaultExteriorSurfaceConstructions.get
      if surfaceSet.floorConstruction.is_initialized then constructionArray << surfaceSet.floorConstruction.get end
      if surfaceSet.wallConstruction.is_initialized then constructionArray << surfaceSet.wallConstruction.get end
      if surfaceSet.roofCeilingConstruction.is_initialized then constructionArray << surfaceSet.roofCeilingConstruction.get end
    end
    #populate interior surfaces
    if constructionSet.defaultInteriorSurfaceConstructions.is_initialized
      surfaceSet = constructionSet.defaultInteriorSurfaceConstructions.get
      if surfaceSet.floorConstruction.is_initialized then constructionArray << surfaceSet.floorConstruction.get end
      if surfaceSet.wallConstruction.is_initialized then constructionArray << surfaceSet.wallConstruction.get end
      if surfaceSet.roofCeilingConstruction.is_initialized then constructionArray << surfaceSet.roofCeilingConstruction.get end
    end
    #populate ground surfaces
    if constructionSet.defaultGroundContactSurfaceConstructions.is_initialized
      surfaceSet = constructionSet.defaultGroundContactSurfaceConstructions.get
      if surfaceSet.floorConstruction.is_initialized then constructionArray << surfaceSet.floorConstruction.get end
      if surfaceSet.wallConstruction.is_initialized then constructionArray << surfaceSet.wallConstruction.get end
      if surfaceSet.roofCeilingConstruction.is_initialized then constructionArray << surfaceSet.roofCeilingConstruction.get end
    end
    #populate exterior sub-surfaces
    if constructionSet.defaultExteriorSubSurfaceConstructions.is_initialized
      subSurfaceSet = constructionSet.defaultExteriorSubSurfaceConstructions.get
      if subSurfaceSet.fixedWindowConstruction.is_initialized then constructionArray << subSurfaceSet.fixedWindowConstruction.get end
      if subSurfaceSet.operableWindowConstruction.is_initialized then constructionArray << subSurfaceSet.operableWindowConstruction.get end
      if subSurfaceSet.doorConstruction.is_initialized then constructionArray << subSurfaceSet.doorConstruction.get end
      if subSurfaceSet.glassDoorConstruction.is_initialized then constructionArray << subSurfaceSet.glassDoorConstruction.get end
      if subSurfaceSet.overheadDoorConstruction.is_initialized then constructionArray << subSurfaceSet.overheadDoorConstruction.get end
      if subSurfaceSet.skylightConstruction.is_initialized then constructionArray << subSurfaceSet.skylightConstruction.get end
      if subSurfaceSet.tubularDaylightDomeConstruction.is_initialized then constructionArray << subSurfaceSet.tubularDaylightDomeConstruction.get end
      if subSurfaceSet.tubularDaylightDiffuserConstruction.is_initialized then constructionArray << subSurfaceSet.tubularDaylightDiffuserConstruction.get end
    end
    #populate interior sub-surfaces
    if constructionSet.defaultInteriorSubSurfaceConstructions.is_initialized
      subSurfaceSet = constructionSet.defaultInteriorSubSurfaceConstructions.get
      if subSurfaceSet.fixedWindowConstruction.is_initialized then constructionArray << subSurfaceSet.fixedWindowConstruction.get end
      if subSurfaceSet.operableWindowConstruction.is_initialized then constructionArray << subSurfaceSet.operableWindowConstruction.get end
      if subSurfaceSet.doorConstruction.is_initialized then constructionArray << subSurfaceSet.doorConstruction.get end
      if subSurfaceSet.glassDoorConstruction.is_initialized then constructionArray << subSurfaceSet.glassDoorConstruction.get end
      if subSurfaceSet.overheadDoorConstruction.is_initialized then constructionArray << subSurfaceSet.overheadDoorConstruction.get end
      if subSurfaceSet.skylightConstruction.is_initialized then constructionArray << subSurfaceSet.skylightConstruction.get end
      if subSurfaceSet.tubularDaylightDomeConstruction.is_initialized then constructionArray << subSurfaceSet.tubularDaylightDomeConstruction.get end
      if subSurfaceSet.tubularDaylightDiffuserConstruction.is_initialized then constructionArray << subSurfaceSet.tubularDaylightDiffuserConstruction.get end
    end
    #populate misc surfaces
    if constructionSet.interiorPartitionConstruction.is_initialized
      constructionArray << constructionSet.interiorPartitionConstruction.get
    end
    if constructionSet.spaceShadingConstruction.is_initialized
      constructionArray << constructionSet.spaceShadingConstruction.get
    end
    if constructionSet.buildingShadingConstruction.is_initialized
      constructionArray << constructionSet.buildingShadingConstruction.get
    end
    if constructionSet.siteShadingConstruction.is_initialized
      constructionArray << constructionSet.siteShadingConstruction.get
    end

    result = constructionArray
    return result
  end # end of OsLib_Constructions.reportConstructionSetConstructions

end