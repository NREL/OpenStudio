########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class MoveSelectedSurfacesToNewSpace < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Move Selected Surfaces to New Space"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # make new space 
    newSpace = OpenStudio::Model::Space.new(model)
    
    # we will set the new space origin at the first space's origin
    haveSetOrigin = false
    
    # this doesn't show correctly until you reload model, also needs to be transformed.
    # change parent space
    model.getSurfaces.each do |surface|
    
      # is this surface in the selection
      next if not runner.inSelection(surface)
      
      # surface must belong to a space
      oldSpace = surface.space
      next if oldSpace.empty?
      oldSpace = oldSpace.get
    
      # is this space in the selection
      # might not want to allow space level selections
      #next if runner.inSelection(oldSpace)
        
      if not haveSetOrigin
        newSpace.setTransformation(oldSpace.transformation)
        haveSetOrigin = true
      end
      
      # transformation from old space coordinates to new space coordinates
      transformation = newSpace.transformation.inverse * oldSpace.transformation
      
      # re-assign surface to new space
      surface.setSpace(newSpace)
        
      # transform surface vertices
      newVertices = transformation * surface.vertices
      surface.setVertices(newVertices)
      
      # transform any sub surfaces too
      surface.subSurfaces.each do |subSurface|
        newVertices = transformation * subSurface.vertices
        subSurface.setVertices(newVertices)
      end
      
      # might want to remove the oldSpace if it is now empty
      #oldSpace.remove if oldSpace.surfaces.empty?
    end
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
MoveSelectedSurfacesToNewSpace.new.registerWithApplication



