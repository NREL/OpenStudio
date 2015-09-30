######################################################################
#  Copyright (c) 2008-2010, Alliance for Sustainable Energy.  
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
class SetInteriorPartitionHeightAboveFloor < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Set Interior Partition Height Above Floor"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    height = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("height",false)
    height.setDisplayName("Height (m)")
    height.setDefaultValue(1.7)
    result << height
    
    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # get the desired height   
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
   
    height = runner.getDoubleArgumentValue("height",user_arguments)
        
    # loop over all interior partitions
    interior_partition_surfaces = model.getInteriorPartitionSurfaces
    interior_partition_surfaces.each do |interior_partition_surface|
    
      # create a new set of vertices
      newVertices = OpenStudio::Point3dVector.new
      
      # get the existing vertices for this interior partition
      vertices = interior_partition_surface.vertices
      vertices.each do |vertex|
 
        # initialize new vertex to old vertex
        x = vertex.x      
        y = vertex.y
        z = vertex.z
        
        # if this z vertex is not on the z = 0 plane
        if (z - 0.0).abs > 0.01
          z = height
        end
      
        # add point to new vertices
        newVertices << OpenStudio::Point3d.new(x,y,z)
      end
      
      # set vertices to new vertices
      interior_partition_surface.setVertices(newVertices)
      
    end

  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
SetInteriorPartitionHeightAboveFloor.new.registerWithApplication
