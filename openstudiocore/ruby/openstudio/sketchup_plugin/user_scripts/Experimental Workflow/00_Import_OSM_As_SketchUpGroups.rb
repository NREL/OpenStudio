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
class ImportOsmAsGroups < OpenStudio::Ruleset::UtilityUserScript

  # override name to return the name of your script
  def name
    return "Import OSM file as SketchUp Groups"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments()
    result = OpenStudio::Ruleset::OSArgumentVector.new

    open_path = OpenStudio::Ruleset::OSArgument::makePathArgument("open_path", true, "osm")
    open_path.setDisplayName("Select OSM File to Import As Groups")
    result << open_path

    return result
  end

  # override run to implement the functionality of your script
  # background_osm_model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(runner, user_arguments)
    super(runner, user_arguments)

    if not runner.validateUserArguments(arguments,user_arguments)
      return false
    end

    open_path = runner.getStringArgumentValue("open_path",user_arguments)

    puts ""
    puts ">>import start"

    puts ""
    puts "File - " + open_path

    # Open OSM file
    background_osm_model = OpenStudio::Model::Model::load(OpenStudio::Path.new(open_path)).get

    # number of spaces
    spaces = background_osm_model.getSpaces
    puts "Model has " + spaces.size.to_s + " spaces"

    # number of base surfaces
    base_surfaces = background_osm_model.getSurfaces
    puts "Model has " + base_surfaces.size.to_s + " base surfaces"

    # number of base surfaces
    sub_surfaces = background_osm_model.getSubSurfaces
    puts "Model has " + sub_surfaces.size.to_s + " sub surfaces"

    # number of surfaces
    shading_surfaces = background_osm_model.getShadingSurfaces
    puts "Model has " + shading_surfaces.size.to_s + " shading surfaces"

    # number of surfaces
    partition_surfaces = background_osm_model.getInteriorPartitionSurfaces
    puts "Model has " + partition_surfaces.size.to_s + " interior partition surfaces"

    # get SketchUp model and entities
    skp_model = Sketchup.active_model
    entities = skp_model.active_entities

    # create layers matched to OpenStudio surface group types
    layers = skp_model.layers
    new_layer = layers.add("OpenStudio BackgroundModel SiteShadingGroup")
    new_layer = layers.add("OpenStudio BackgroundModel BuildingAndSpaceShadingGroup")
    new_layer = layers.add("OpenStudio BackgroundModel Space")
    new_layer = layers.add("OpenStudio BackgroundModel InteriorPartitionSurfaceGroup")

    # set render mode to color by layers (interior partition and shading can match OpenStudio, spaces should be something unique)?

    # use building rotation to set north direction in SketchUp
    building = background_osm_model.getBuilding
    rotation = building.northAxis # not sure of units
    info = skp_model.shadow_info
    info["NorthAngle"] = rotation*-1.0
    if rotation != 0.0
      info["DisplayNorth"] = rotation*-1.0
    end

    # create def to make group
    def make_group(parent,name,layer,xOrigin,yOrigin,zOrigin,rotation)
      group = parent.entities.add_group
      #set name and layer
      group.name=name
      group.layer=layer
      # set group origin, rotation
      point = Geom::Point3d.new "#{xOrigin}m".to_l,"#{yOrigin}m".to_l,"#{zOrigin}m".to_l
      t = Geom::Transformation.new point
      group.move! t
      # rotate
      tr = Geom::Transformation.rotation ["#{xOrigin}m".to_l,"#{yOrigin}m".to_l,"#{zOrigin}m".to_l], [0, 0, 1], rotation.degrees
      group.transform! tr

      return group

    end

    # create  def to make a surface
    def make_surface(group, vertices)
      entities = group.entities
      pts = []
      vertices.each do |pt|
        pts << ["#{pt.x}m".to_l,"#{pt.y}m".to_l,"#{pt.z}m".to_l]
      end
      face = entities.add_face pts
    end

    # loop through spaces
    spaces.each do |space|
      # create space
      # puts "space name: #{space.name}"
      group = make_group(Sketchup.active_model,space.name.get,"OpenStudio BackgroundModel Space",space.xOrigin,space.yOrigin,space.zOrigin,space.directionofRelativeNorth*-1)

      # loop through base surfaces
      base_surfaces = space.surfaces
      base_surfaces.each do |base_surface|
        # create base surface
        # puts "surface name: #{base_surface.name}"
        make_surface(group, base_surface.vertices)

        # loop through sub surfaces
        sub_surfaces = base_surface.subSurfaces
        sub_surfaces.each do |sub_surface|
          # create sub surface
          # puts "sub surface name: #{sub_surface.name}"
          make_surface(group, sub_surface.vertices)

        end # end of sub_surfaces.each do

      end # end of base_surfaces.each do

      # loop through space shading groups
      space_shading_groups = space.shadingSurfaceGroups
      space_shading_groups.each do |space_shading_group|
        # create group
        # puts "space shading group name: #{space_shading_group.name}"
        sub_group = make_group(group,space_shading_group.name.get,"OpenStudio BackgroundModel BuildingAndSpaceShadingGroup",space_shading_group.xOrigin,space_shading_group.yOrigin,space_shading_group.zOrigin,space_shading_group.directionofRelativeNorth*-1)

        #loop through shading surfaces
        shading_surfaces = space_shading_group.shadingSurfaces
        shading_surfaces.each do |shading_surface|
          # create shading surfaces
          # puts "space shading surface name: #{shading_surface.name}"
          make_surface(sub_group, shading_surface.vertices)

        end # end of shading_surfaces.each do

      end  # end of space_shading_groups.each do

      # loop through interior partition groups
      interior_partition_groups = space.interiorPartitionSurfaceGroups
      interior_partition_groups.each do |interior_partition_group|
        # create group
        # puts "interior partition group name: #{interior_partition_group.name}"
        sub_group = make_group(group,interior_partition_group.name.get,"OpenStudio BackgroundModel InteriorPartitionSurfaceGroup",interior_partition_group.xOrigin,interior_partition_group.yOrigin,interior_partition_group.zOrigin,interior_partition_group.directionofRelativeNorth*-1)

        #loop through interior partition surfaces
        interior_partition_surfaces = interior_partition_group.interiorPartitionSurfaces
        interior_partition_surfaces.each do |interior_partition_surface|
          # create interior partition surfaces
          # puts "interior partition surface name: #{interior_partition_surface.name}"
          make_surface(sub_group, interior_partition_surface.vertices)

        end # end of interior_partition_surfaces.each do

      end  # end of interior_partition_groups.each do

    end # end of spaces.each do

    #loop through shading surface groups, skip if space shading, those are imported with spaces
    shading_surface_groups = background_osm_model.getShadingSurfaceGroups
    shading_surface_groups.each do |shading_surface_group|
      # create group
      if not shading_surface_group.shadingSurfaceType == "Space"
        if shading_surface_group.shadingSurfaceType == "Building"
          group = make_group(Sketchup.active_model,shading_surface_group.name.get,"OpenStudio BackgroundModel BuildingAndSpaceShadingGroup",shading_surface_group.xOrigin,shading_surface_group.yOrigin,shading_surface_group.zOrigin,shading_surface_group.directionofRelativeNorth*-1)
        else
          group = make_group(Sketchup.active_model,shading_surface_group.name.get,"OpenStudio BackgroundModel SiteShadingGroup",shading_surface_group.xOrigin,shading_surface_group.yOrigin,shading_surface_group.zOrigin,shading_surface_group.directionofRelativeNorth*-1)
        end

        #loop through shading surfaces
        shading_surfaces = shading_surface_group.shadingSurfaces
        shading_surfaces.each do |shading_surface|
          # create shading surfaces
          # puts "shading surface name: #{shading_surface.name}"
          make_surface(group, shading_surface.vertices)

        end # end of shading_surfaces.each do

      end # end of if not shading_surface_group.type == "Space"

    end #end of shading_surface_groups.each do

    #todo - see why spaces are not passing manifold solid test. Seems like old SketchUp issue where if I exploded and re-make it then shows as solid. Maybe even just re-open it.

    #zoom extents
    view = Sketchup.active_model.active_view
    new_view = view.zoom_extents

  end # end of run

end

# this call registers your script with the OpenStudio SketchUp plug-in
ImportOsmAsGroups.new.registerWithApplication
