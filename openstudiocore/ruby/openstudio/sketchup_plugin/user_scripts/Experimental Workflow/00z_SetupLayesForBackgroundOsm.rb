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
class SetupLayersForBackgroundOsm < OpenStudio::Ruleset::UtilityUserScript

  # override name to return the name of your script
  def name
    return "Setup Layers for Background merge"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments()
    result = OpenStudio::Ruleset::OSArgumentVector.new

    return result
  end

  # override run to implement the functionality of your script
  # @background_osm_model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(runner, user_arguments)
    super(runner, user_arguments)

    if not runner.validateUserArguments(arguments,user_arguments)
      return false
    end

    puts ""
    puts ">>Setting expected layers for merge to background OSM"

    # get SketchUp model and entities
    skp_model = Sketchup.active_model
    entities = skp_model.active_entities

    # create layers matched to OpenStudio surface group types
    layers = skp_model.layers
    new_layer = layers.add("OpenStudio BackgroundModel SiteShadingGroup")
    new_layer = layers.add("OpenStudio BackgroundModel ShadingGroup")
    new_layer = layers.add("OpenStudio BackgroundModel Space")
    new_layer = layers.add("OpenStudio BackgroundModel InteriorPartitionSurfaceGroup")

  end # end of run

end

# this call registers your script with the OpenStudio SketchUp plug-in
SetupLayersForBackgroundOsm.new.registerWithApplication
