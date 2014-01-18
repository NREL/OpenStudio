######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
class MergeSketchUpGroupsToOsm < OpenStudio::Ruleset::UtilityUserScript

  # override name to return the name of your script
  def name
    return "Merge SketchUp Groups to OSM"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments()
    result = OpenStudio::Ruleset::OSArgumentVector.new

    save_path = OpenStudio::Ruleset::OSArgument::makePathArgument("save_path", true, "osm")
    save_path.setDisplayName("Save New OSM")
    result << save_path

    # todo - expose layers as arguments mapped to type of SO objects?
    # todo - have way to pull in background model from other script.
    # todo - support save as to another or a new osm.

    return result
  end

  # override run to implement the functionality of your script
  # @background_osm_model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    if not runner.validateUserArguments(arguments,user_arguments)
      return false
    end
    
    #get path
    save_path = runner.getStringArgumentValue("save_path",user_arguments)

    # Open OSM file
    @background_osm_model = OpenStudio::Model::Model::load(OpenStudio::Path.new(save_path)).get

    # get surface groups from osm
    @spaces = @background_osm_model.getSpaces
    @shading_groups = @background_osm_model.getShadingSurfaceGroups
    @interior_partition_groups = @background_osm_model.getInteriorPartitionSurfaceGroups


    puts ""
    puts ">>merge start"

    puts ""

    # get SketchUp model and entities
    skp_model = Sketchup.active_model
    entities = skp_model.active_entities

    #use north direction in SketchUp to set building rotation

    #get spaces shading groups and interior partition groups

    #create def to make space
    def make_space(name,xOrigin,yOrigin,zOrigin,rotation)

      # todo - SketchUp doesn't enforce unique names but OpenStudio will handle that.

      #flag to create new space
      need_space = true

      #loop through spaces in model
      @spaces.each do |space|

        if space.name.to_s == name

          #delete existing OSM surfaces
          puts "removing surfaces from existing space named #{space.name}."
          surfaces = space.surfaces
          surfaces.each do |surface|
            surface.remove
          end

          need_space = false
          space.setXOrigin(xOrigin.to_f)
          space.setYOrigin(yOrigin.to_f)
          space.setZOrigin(zOrigin.to_f)

        end #end of if space.name = name

      end #end of spaces.each do

      #make space and set name
      if need_space

        #add a space to the model without any geometry or loads, want to make sure measure works or fails gracefully
        puts "making new space named #{name}"
        space = OpenStudio::Model::Space.new(@background_osm_model)
        space.setName(name)
        space.setXOrigin(xOrigin.to_f)
        space.setYOrigin(yOrigin.to_f)
        space.setZOrigin(zOrigin.to_f)
      end

      return space
    end #end of def make_space

    # create def to make space surface
    def make_space_surfaces(space, vertices)

      #loop though surfaces
      #if all or all but one edges are shared by another surface then put in hash with surface name as sub-surface

      #create surfaces as loop through

      #loop through array of sub-surfaces and make them

    end

    #create def to make shading surface group
    def make_shading_surface_group(name,xOrigin,yOrigin,zOrigin,rotation,parent)

      # todo - SketchUp doesn't enforce unique names but OpenStudio will handle that.

      #flag to create new group
      need_group = true

      #loop through groups in model
      @shading_groups.each do |shading_surface_group|

        if shading_surface_group.name.to_s == name

          #delete existing OSM surfaces
          puts "removing surfaces from existing shading surface group named #{shading_surface_group.name}."
          surfaces = shading_surface_group.shadingSurfaces
          surfaces.each do |surface|
            surface.remove
          end

          need_group = false
          shading_surface_group.setXOrigin(xOrigin.to_f)
          shading_surface_group.setYOrigin(yOrigin.to_f)
          shading_surface_group.setZOrigin(zOrigin.to_f)
          @result = shading_surface_group

          # todo - in addition to rotation, no I need to see if these should be re-parented?

        end #end of if space.name = name

      end #end of spaces.each do

      #make group and set name
      if need_group

        #add a space to the model without any geometry or loads, want to make sure measure works or fails gracefully
        puts "making new shading surface group named #{name}"
        shading_surface_group = OpenStudio::Model::ShadingSurfaceGroup.new(@background_osm_model)  #this can be at top level or in space
        shading_surface_group.setName(name)
        if not parent.to_s == ""
          shading_surface_group.setSpace(parent)
        end
        shading_surface_group.setXOrigin(xOrigin.to_f)
        shading_surface_group.setYOrigin(yOrigin.to_f)
        shading_surface_group.setZOrigin(zOrigin.to_f)
        @result = shading_surface_group
      end

      return @result
    end

    # create def to make shading surface
    def make_shading_surfaces(shading_surface_group, group)

      #loop through surfaces to make OS surfaces
      group.entities.each do |entity|
        if entity.class.to_s == "Sketchup::Face"
          vertices = entity.vertices

          # make an OpenStudio vector of of Point3d objects
          newVertices = []

          vertices.each do |vertex|  #todo - convert to metric
            x = vertex.position[0].to_m
            y = vertex.position[1].to_m
            z = vertex.position[2].to_m
            newVertices << OpenStudio::Point3d.new(x,y,z)
          end
          new_surface = OpenStudio::Model::ShadingSurface.new(newVertices,@background_osm_model)
          new_surface.setShadingSurfaceGroup(shading_surface_group)

          # todo - set group if necessary
        end
      end #end of entities.each.do

    end

    # create def to make interior partition group
    def make_interior_partition_group(name,xOrigin,yOrigin,zOrigin,rotation,parent)

      # todo - SketchUp doesn't enforce unique names but OpenStudio will handle that.

      #flag to create new group
      need_group = true

      #loop through spaces in model
      @interior_partition_groups.each do |interior_partition_group|

        if interior_partition_group.name.to_s == name

          #delete existing OSM surfaces
          puts "removing surfaces from existing interior partition group named #{interior_partition_group.name}."
          surfaces = interior_partition_group.interiorPartitionSurfaces
          surfaces.each do |surface|
            surface.remove
          end

          need_group = false
          interior_partition_group.setXOrigin(xOrigin.to_f)
          interior_partition_group.setYOrigin(yOrigin.to_f)
          interior_partition_group.setZOrigin(zOrigin.to_f)
          @result = interior_partition_group

          # todo - in addition to rotation, no I need to see if these should be re-parented?

        end #end of if space.name = name

      end #end of spaces.each do

      #make group and set name
      if need_group

        #add a space to the model without any geometry or loads, want to make sure measure works or fails gracefully
        puts "making new interior partition surface group named #{name}"
        interior_partition_group = OpenStudio::Model::InteriorPartitionSurfaceGroup.new(@background_osm_model)  #this needs to be the space it is in
        interior_partition_group.setName(name)
        interior_partition_group.setSpace(parent)
        interior_partition_group.setXOrigin(xOrigin.to_f)
        interior_partition_group.setYOrigin(yOrigin.to_f)
        interior_partition_group.setZOrigin(zOrigin.to_f)
        @result = interior_partition_group
      end

      return @result
    end

    # create def to make interior partition surface
    def make_interior_partition_surfaces(interior_partition_group, group)

      #loop through surfaces to make OS surfaces
      group.entities.each do |entity|
        if entity.class.to_s == "Sketchup::Face"
          vertices = entity.vertices

          # make an OpenStudio vector of of Point3d objects
          newVertices = []

          vertices.each do |vertex|  #todo - convert to metric
            x = vertex.position[0].to_m
            y = vertex.position[1].to_m
            z = vertex.position[2].to_m
            newVertices << OpenStudio::Point3d.new(x,y,z)
          end
          new_surface = OpenStudio::Model::InteriorPartitionSurface.new(newVertices,@background_osm_model)
          new_surface.setInteriorPartitionSurfaceGroup(interior_partition_group)

          # todo - set group if necessary
        end
      end #end of entities.each.do

    end

    #array of space, shading groups, and interior partition groups. Later will delete objects in OSM that don't exist in SKP
    current_spaces = []
    current_make_shading_surface_groups = []
    current_interior_partition_groups = []

    #todo - address scaling and other group transformation

    #making array of groups and components
    groups = []
    entities.each do |entity|
      if entity.class.to_s == "Sketchup::Group" or entity.class.to_s == "Sketchup::ComponentInstance"
        entity.make_unique #this is only needed if a group was copied.
        groups << entity
      end
    end #end of entities.each.do

    #loop through groups in SketchUp model
    groups.each do |group|

      # get transformation
      t = group.transformation

      if group.layer.name == "OpenStudio BackgroundModel ShadingGroup"

        #make or update group
        shading_surface_group = make_shading_surface_group(group.name,t.origin.x.to_m,t.origin.y.to_m,t.origin.z.to_m,"rotation","") #no parent for site and building shading
        #make surfaces
        shading_surfaces = make_shading_surfaces(shading_surface_group,group)

        #add to array of shading groups
        current_make_shading_surface_groups << "group name"

      elsif group.layer.name == "OpenStudio BackgroundModel InteriorPartitionSurfaceGroup"
        # somehow warn the user that these need to be in a space, not at the top level
        puts "Top level group on interior partition layer is not valid. It will not be translated"
      elsif group.layer.name == "OpenStudio BackgroundModel Space"

        #make or update space
        space = make_space(group.name,t.origin.x.to_m,t.origin.y.to_m,t.origin.z.to_m,"rotation")

        #make surfaces
        #make_space_surfaces()

        #add to array of shading groups
        current_spaces << "space name"

        #Making array of nested groups
        nested_groups = []
        group.entities.each do |group_entity|
          if group_entity.class.to_s == "Sketchup::Group" or group_entity.class.to_s == "Sketchup::ComponentInstance"
            group_entity.make_unique #this is only needed if a group was copied.
            nested_groups << group_entity
          end
        end #end of entities.each.do

        #process nested groups
        nested_groups.each do |nested_group|

          # get transformation
          nested_t = nested_group.transformation

          if nested_group.layer.name == "OpenStudio BackgroundModel ShadingGroup"

            #make or update group
            space_shading_group = make_shading_surface_group(nested_group.name,nested_t.origin.x.to_m,nested_t.origin.y.to_m,nested_t.origin.z.to_m,"rotation",space)
            #make surfaces
            shading_surfaces = make_shading_surfaces(space_shading_group,nested_group)

            #add to array of shading groups
            current_make_shading_surface_groups << "group name"

          elsif nested_group.layer.name == "OpenStudio BackgroundModel InteriorPartitionSurfaceGroup"

            #make or update group
            interior_partition_group = make_interior_partition_group(nested_group.name,nested_t.origin.x.to_m,nested_t.origin.y.to_m,nested_t.origin.z.to_m,"rotation",space)

            #make surfaces
            interior_partition_surfaces = make_interior_partition_surfaces(interior_partition_group,nested_group)

            #add to array of shading groups
            current_interior_partition_groups << "group name"

          else
            puts "A nested group was not on valid layer. It will not be translated."
          end #end of if nested_group.layer

        end #end of nested_groups.each do

      else
        # should groups on other layers become spaces, or should they be ignored?
        # I think for now they should become spaces. Otherwise I need a script o add layers for user
        puts "A group was not on valid layer. It will not be translated."
      end #end of if group layer

    end #end of groups.each do

    #todo - delete spaces that were no long in the SketchUp model

    #todo - warn user about loose top level surfaces

    #todo - also in loop through groups above warn if things are nested too deep

    #todo - could be nice to highlight things that are not valid by painting them or moving them to layer?

    #todo - would be nice to keep check similar to know if file was saved elsewhere since the import happened, offer to reload or cancel operation.

    #todo - close the model after saving. In the future I think I will try and leave it open all the time. As it is now if I run over and over seems slower, I assume due to bad cleanup.

    #save the model
    @spaces.each do |space|
      puts space.name
    end

    output_file_path = OpenStudio::Path.new('C:\Users\dgoldwas\Documents\aaaWorking\test.osm')  #todo - later change this to save over the original file.
    @background_osm_model.save(output_file_path,true)
    puts "model saved to #{output_file_path}"


  end # end of run

end

# this call registers your script with the OpenStudio SketchUp plug-in
MergeSketchUpGroupsToOsm.new.registerWithApplication
