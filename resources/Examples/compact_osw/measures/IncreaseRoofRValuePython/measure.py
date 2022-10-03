import openstudio
import typing


class IncreaseInsulationRValueForRoofsByPercentagePython(openstudio.measure.ModelMeasure):

    def name(self):
        """
        Return the human readable name.
        Measure name should be the title case of the class name.
        """
        return "Increase R-value of Insulation for Roofs by a Specified Percentage."

    def description(self):
        """
        human readable description
        """
        return "DESCRIPTION_TEXT"

    def modeler_description(self):
        """
        human readable description of modeling approach
        """
        return "MODELER_DESCRIPTION_TEXT"

    def arguments(self, model: typing.Optional[openstudio.model.Model] = None):
        """
        define what happens when the measure is run
        """
        args = openstudio.measure.OSArgumentVector()

        r_value = openstudio.measure.OSArgument.makeDoubleArgument("r_value", True)
        r_value.setDisplayName("Percentage Increase of R-value for Roof Insulation.")
        r_value.setDefaultValue(30.0)

        args.append(r_value)

        return args

    def run(self,
            model: openstudio.model.Model,
            runner: openstudio.measure.OSRunner,
            user_arguments: openstudio.measure.OSArgumentMap):
        """
        define what happens when the measure is run
        """
        super().run(model, runner, user_arguments)  # Do **NOT** remove this line

        if not(runner.validateUserArguments(self.arguments(model),
                                            user_arguments)):
            return False

        r_value = runner.getDoubleArgumentValue("r_value", user_arguments)

        # set limit for minimum insulation. This is used to limit input and for inferring insulation layer in construction.
        min_expected_r_value_ip = 1  # ip units
        min_expected_r_value_si = openstudio.convert(min_expected_r_value_ip, "ft^2*h*R/Btu","m^2*K/W").get()

        # check the R-value for reasonableness
        if r_value < -100:
            runner.registerError("Percentage increase less than -100% is not valid.")
            return False

        # create an array of roofs and find range of starting construction R-value (not just insulation layer)
        surfaces = model.getSurfaces()
        exterior_surfaces = []
        exterior_surface_constructions = []
        exterior_surface_construction_names = []
        roof_resistance = []
        for surface in surfaces:
            if surface.outsideBoundaryCondition() != "Outdoors" or surface.surfaceType() != "RoofCeiling":
                continue
            exterior_surfaces.append(surface)
            roof_const = surface.construction().get()
            # only add construction if it hasn't been added yet
            if not roof_const.nameString() in exterior_surface_construction_names:
                exterior_surface_constructions.append(roof_const.to_Construction().get())

            exterior_surface_construction_names.append(roof_const.nameString())
            roof_resistance.append(1.0 / roof_const.thermalConductance().get())

        # nothing will be done if there are no exterior surfaces
        if not exterior_surfaces:
            runner.registerAsNotApplicable("Model does not have any roofs.")
            return True

        # report strings for initial condition
        initial_string = []
        for exterior_surface_construction in exterior_surface_constructions:
            # unit conversion of roof insulation from SI units (M^2*K/W) to IP units (ft^2*h*R/Btu)
            initial_conductance_ip = openstudio.convert(
                1.0 / exterior_surface_construction.thermalConductance().get(),
                "m^2*K/W", "ft^2*h*R/Btu").get()
            initial_string.append(
                f"{exterior_surface_construction.nameString()} (R-{initial_conductance_ip:.1f})"
            )

        runner.registerInitialCondition(f"The building had {len(initial_string)} roof constructions: {', '.join(sorted(initial_string))}.")

        # hashes to track constructions and materials made by the measure, to avoid duplicates
        constructions_hash_old_new = {}
        constructions_hash_new_old = {} # used to get netArea of new construction and then cost objects of construction it replaced
        materials_hash = {}

        # array and counter for new constructions that are made, used for reporting final condition
        final_constructions_array = []

        # loop through all constructions and materials used on roofs, edit and clone
        for exterior_surface_construction in exterior_surface_constructions:
            construction_layers = exterior_surface_construction.layers()
            max_thermal_resistance_material = ""
            max_thermal_resistance_material_index = 0
            thermal_resistance_values = []

            # loop through construction layers and infer insulation layer/material
            for i, construction_layer in enumerate(construction_layers):
                construction_layer_r_value = construction_layer.to_OpaqueMaterial().get().thermalResistance()
                if thermal_resistance_values:
                    if construction_layer_r_value > max(thermal_resistance_values):
                        max_thermal_resistance_material = construction_layer
                        max_thermal_resistance_material_index = i

                thermal_resistance_values.append(construction_layer_r_value)

            if max(thermal_resistance_values) <= min_expected_r_value_si:
                runner.registerWarning(f"Construction '{exterior_surface_construction.nameString()}' does not appear to have an insulation layer and was not altered.")
                continue

            # clone the construction
            final_construction = exterior_surface_construction.clone(model)
            final_construction = final_construction.to_Construction().get()
            final_construction.setName(f"{exterior_surface_construction.nameString()} adj roof insulation")
            final_constructions_array.append(final_construction)

            # add construction object if it didnt exist to start with

            # push to hashes
            constructions_hash_old_new[exterior_surface_construction.nameString()] = final_construction
            constructions_hash_new_old[final_construction.nameString()] = exterior_surface_construction #push the object to hash key vs. name

            # find already cloned insulation material and link to construction
            target_material = max_thermal_resistance_material
            found_material = False
            for orig,new in materials_hash.items():
                if target_material.nameString() == orig:
                    new_material = new
                    materials_hash[max_thermal_resistance_material.nameString()] = new_material
                    final_construction.eraseLayer(max_thermal_resistance_material_index)
                    final_construction.insertLayer(max_thermal_resistance_material_index, new_material)
                    found_material = True
                    # TODO: break?

            # clone and edit insulation material and link to construction
            if not found_material:
                new_material = max_thermal_resistance_material.clone(model)
                new_material = new_material.to_OpaqueMaterial().get()
                new_material.setName(f"{max_thermal_resistance_material.nameString()}_R-value {r_value}% increase")
                materials_hash[max_thermal_resistance_material.nameString()] = new_material
                final_construction.eraseLayer(max_thermal_resistance_material_index)
                final_construction.insertLayer(max_thermal_resistance_material_index, new_material)
                runner.registerInfo(f"For construction'{final_construction.nameString()}', material '{new_material.nameString()}' was altered.")

                # edit insulation material
                new_material_matt = new_material.to_Material()
                if new_material_matt.is_initialized():
                    starting_thickness = new_material_matt.get().thickness()
                    target_thickness = starting_thickness * (1.0 + r_value / 100.0)
                    final_thickness = new_material_matt.get().setThickness(target_thickness)

                new_material_massless = new_material.to_MasslessOpaqueMaterial()
                if new_material_massless.is_initialized():
                    starting_thermal_resistance = new_material_massless.get().thermalResistance()
                    final_thermal_resistance = new_material_massless.get().setThermalResistance(starting_thermal_resistance)

                new_material_airgap = new_material.to_AirGap()
                if new_material_airgap.is_initialized():
                    starting_thermal_resistance = new_material_airgap.get().thermalResistance()
                    final_thermal_resistance = new_material_airgap.get().setThermalResistance(starting_thermal_resistance)


        # loop through construction sets used in the model
        default_construction_sets = model.getDefaultConstructionSets()
        for default_construction_set in default_construction_sets:
            if default_construction_set.directUseCount() == 0:
                continue
            default_surface_const_set = default_construction_set.defaultExteriorSurfaceConstructions()
            if not default_surface_const_set.is_initialized():
                continue

            starting_construction = default_surface_const_set.get().roofCeilingConstruction()

            # creating new default construction set
            new_default_construction_set = default_construction_set.clone(model).to_DefaultConstructionSet().get()
            new_default_construction_set.setName(f"{default_construction_set.nameString()} adj roof insulation")

            # create new surface set and link to construction set
            new_default_surface_const_set = default_surface_const_set.get().clone(model).to_DefaultSurfaceConstructions().get()
            new_default_surface_const_set.setName(f"{default_surface_const_set.get().nameString()} adj roof insulation")
            new_default_construction_set.setDefaultExteriorSurfaceConstructions(new_default_surface_const_set)

            # use the hash to find the proper construction and link to new_default_surface_const_set
            target_const = new_default_surface_const_set.roofCeilingConstruction()
            if target_const.is_initialized():
                target_const = target_const.get().nameString()
                found_const_flag = False
                for orig, new in constructions_hash_old_new.items():
                    if target_const == orig:
                        final_construction = new
                        new_default_surface_const_set.setRoofCeilingConstruction(final_construction)
                        found_const_flag = True

                if not found_const_flag: # this should never happen but is just an extra test in case something goes wrong with the measure code
                    runner.registerWarning(f"Measure couldn't find the construction named '{target_const}' in the exterior surface hash.")


            # swap all uses of the old construction set for the new
            construction_set_sources = default_construction_set.sources()
            for construction_set_source in construction_set_sources:
                building_source = construction_set_source.to_Building()
                # if statement for each type of object than can use a DefaultConstructionSet
                if building_source.is_initialized():
                    building_source = building_source.get()
                    building_source.setDefaultConstructionSet(new_default_construction_set)

                building_story_source = construction_set_source.to_BuildingStory()
                if building_story_source.is_initialized():
                    building_story_source = building_story_source.get()
                    building_story_source.setDefaultConstructionSet(new_default_construction_set)

                space_type_source = construction_set_source.to_SpaceType()
                if space_type_source.is_initialized():
                    space_type_source = space_type_source.get()
                    space_type_source.setDefaultConstructionSet(new_default_construction_set)

                space_source = construction_set_source.to_Space()
                if space_source.is_initialized():
                    space_source = space_source.get()
                    space_source.setDefaultConstructionSet(new_default_construction_set)

        # link cloned and edited constructions for surfaces with hard assigned constructions
        for exterior_surface in exterior_surfaces:
            if exterior_surface.isConstructionDefaulted() or not exterior_surface.construction().is_initialized():
                continue

            # use the hash to find the proper construction and link to surface
            target_const = exterior_surface.construction()
            if target_const.is_initialized():
                target_const = target_const.get().nameString()
                for orig, new in constructions_hash_old_new.items():
                    if target_const == orig:
                        final_construction = new

        # report strings for final condition
        final_string = []   # not all exterior roof constructions, but only new ones made. If  roof didn't have insulation and was not altered we don't want to show it
        affected_area_si = 0
        for final_construction in final_constructions_array:

            # unit conversion of roof insulation from SI units (M^2*K/W) to IP units (ft^2*h*R/Btu)
            final_conductance_ip = openstudio.convert(1.0 / final_construction.thermalConductance().get(),
                                                      "m^2*K/W", "ft^2*h*R/Btu").get()
            final_string.append(f"{final_construction.nameString()} (R-{final_conductance_ip:.1f})")
            affected_area_si = affected_area_si + final_construction.getNetArea()


        # add not applicable test if there were exterior roof constructions but none of them were altered (already enough insulation or doesn't look like insulated wall)
        if affected_area_si == 0:
            runner.registerAsNotApplicable("No roofs were altered.")
            return True
            # affected_area_ip = affected_area_si
        else:
            # ip construction area for reporting
            affected_area_ip = openstudio.convert(affected_area_si, "m^2", "ft^2").get()

        # report final condition
        runner.registerFinalCondition(
            f"The existing insulation for roofs was increased by {r_value}%. "
            f"This was applied to {affected_area_ip:,.0f} (ft^2) across "
            f"{len(final_string)} roof constructions: {', '.join(sorted(final_string))}.")

        return True


# this allows the measure to be used by the application
IncreaseInsulationRValueForRoofsByPercentagePython().registerWithApplication()
