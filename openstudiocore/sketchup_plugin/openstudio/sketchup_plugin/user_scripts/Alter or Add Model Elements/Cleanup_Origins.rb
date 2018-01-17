########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
class CleanupOrigins < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Cleanup Origins"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    return result
  end

  def cleanup_group(group)
      boundingBox = group.transformation*group.boundingBox

      if boundingBox.isEmpty
        return
      end

      matrix = OpenStudio::Matrix.new(4,4,0)
      matrix[0,0] = 1
      matrix[1,1] = 1
      matrix[2,2] = 1
      matrix[3,3] = 1
      matrix[0,3] = boundingBox.minX.get
      matrix[1,3] = boundingBox.minY.get
      matrix[2,3] = boundingBox.minZ.get
      translation = OpenStudio::Transformation.new(matrix)
      group.changeTransformation(translation)
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    # do spaces first as these may contain other groups
    model.getSpaces.each do |space|
      next if not runner.inSelection(space)
      cleanup_group(space)

      space.shadingSurfaceGroups.each do |group|
        cleanup_group(group)
      end

      space.interiorPartitionSurfaceGroups.each do |group|
        cleanup_group(group)
      end
    end

    # now do shading surfaces
    model.getShadingSurfaceGroups.each do |group|
      next if not runner.inSelection(group)
      cleanup_group(group)
    end

    # now do interior partition surface groups
    model.getInteriorPartitionSurfaceGroups.each do |group|
      next if not runner.inSelection(group)
      cleanup_group(group)
    end
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
CleanupOrigins.new.registerWithApplication



