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

    # todo - eventually I want ao way to save as a new OSM if the user didn't start by importing one
    #save_path = OpenStudio::Ruleset::OSArgument::makePathArgument("save_path", true, "osm")
    #save_path.setDisplayName("Save New OSM")
    #result << save_path

    # todo - expose layers as arguments mapped to type of SO objects?

    return result
  end

  # override run to implement the functionality of your script
  # background_osm_model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    if not runner.validateUserArguments(arguments,user_arguments)
      return false
    end
    
    #save_path = runner.getStringArgumentValue("save_path",user_arguments)

    puts ""
    puts ">>merge start"

    puts ""
    #puts "File - " + save_path

    #puts "File - #{open_path}"

    #get OSM model from import script or ask user to create a new one.

    # get SketchUp model and entities
    skp_model = Sketchup.active_model
    entities = skp_model.active_entities

    # use north direction in SketchUp to set building rotation

    #get spaces shading groups and interior partition groups

    # create def to make space
    def make_space(name,xOrigin,yOrigin,zOrigin,rotation)

      space = name
      puts "making space named #{space}."

      # loop through spaces in model

      # if find a match then delete surfaces from space and return the space
      # if don't find a match then make a new space with group name and return it

      # in either case set transformation from SU group.

      return space
    end

    # create def to make space surface
    def make_space_surfaces(space, vertices)

      #loop though surfaces
      #if all or all but one edges are shared by another surface then put in hash with surface name as sub-surface

      #create surfaces as loop through

      #loop through array of sub-surfaces and make them

    end

    # create def to make shading surface group
    def make_shading_surface_group(name,xOrigin,yOrigin,zOrigin,rotation,parent)

      shading_surface_group = name
      puts "making shading surface group named named #{shading_surface_group}."

      # loop through shading surface groups in model

      # if find a match then delete surfaces from space and return the group
      # if don't find a match then make a new space with group name and return it

      # in either case set transformation from SU group.

      return shading_surface_group
    end

    # create def to make shading surface
    def make_shading_surfaces(group, vertices)

      #loop through surfaces to make OS surfaces

    end

    # create def to make interior partition group
    def make_interior_partition_group(name,xOrigin,yOrigin,zOrigin,rotation,parent)

      interior_partition_group = name
      puts "making interior partition surface group named #{interior_partition_group}."

      # loop through interior partition groups in model

      # if find a match then delete surfaces from space and return the group
      # if don't find a match then make a new space with group name and return it

      # in either case set transformation from SU group.

      return interior_partition_group
    end

    # create def to make interior partition surface
    def make_interior_partition_surfaces(group, vertices)

      #loop through surfaces to make OS surfaces

    end

    #array of space, shading groups, and interior partition groups. Later will delete objects in OSM that don't exist in SKP
    current_spaces = []
    current_make_shading_surface_groups = []
    current_interior_partition_groups = []

    #todo - convert components to groups using make unique

    #todo - address scaling of groups

    # Making array of groups and components
    groups = []
    entities.each do |entity|
      if entity.class.to_s == "Sketchup::Group" or entity.class.to_s == "Sketchup::ComponentInstance"
        entity.make_unique #this is only needed if a group was copied.
        groups << entity
      end
    end #end of entities.each.do

    groups.each do |group|

      # get transformation
      t = group.transformation

      if group.layer.name == "OpenStudio BackgroundModel ShadingGroup"

        #make or update group
        make_shading_surface_group(group.name,t.origin.x.to_m,t.origin.y.to_m,t.origin.z.to_m,"rotation",group.parent)

        #make surfaces
        #make_shading_surfaces()

        #add to array of shading groups
        current_make_shading_surface_groups << "group name"

      elsif group.layer.name == "OpenStudio BackgroundModel InteriorPartitionGroup"
        # somehow warn the user that these need to be in a space, not at the top level
        puts "Top level group on interior partition layer is not valid. It will not be translated"
      elsif group.layer.name == "OpenStudio BackgroundModel Space"

        #make or update space
        make_space(group.name,t.origin.x.to_m,t.origin.y.to_m,t.origin.z.to_m,"rotation")

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
            make_shading_surface_group(nested_group.name,nested_t.origin.x.to_m,nested_t.origin.y.to_m,nested_t.origin.z.to_m,"rotation",nested_group.parent)

            #make surfaces
            #make_shading_surfaces()

            #add to array of shading groups
            current_make_shading_surface_groups << "group name"

          elsif nested_group.layer.name == "OpenStudio BackgroundModel InteriorPartitionGroup"

            #make or update group
            make_interior_partition_group(nested_group.name,nested_t.origin.x.to_m,nested_t.origin.y.to_m,nested_t.origin.z.to_m,"rotation",nested_group.parent)

            #make surfaces
            #make_interior_partition_surfaces()

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

  end # end of run

end

# this call registers your script with the OpenStudio SketchUp plug-in
MergeSketchUpGroupsToOsm.new.registerWithApplication
