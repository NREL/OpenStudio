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

######################################################################
# == Synopsis 
#
#   Load an idf, write an idf, convert to OpenStudio model, save to osm and read from osm.
#
# == Usage
#
#   ruby Serialization.rb ARGV[0] 
# 
#   ARGV[0] - Path to EnergyPlus Idf file
#
# == Examples
#
#   ruby Serialization.rb 'C:\path\to\energyplus\dir\in.idf'
#
######################################################################

require 'openstudio'

# read in idf
idfPath = OpenStudio::Path.new(ARGV[0])
oIdfFile = OpenStudio::IdfFile.load(idfPath,"EnergyPlus".to_IddFileType)
if oIdfFile.empty?
  raise "Unable to load " + idfPath.to_s + "."
end 
idfFile = oIdfFile.get()

# save idf
File.open(idfPath.replace_extension('newidf').to_s, 'w') do |file|
  file << idfFile.to_s
end

# convert to workspace
workspace = OpenStudio::Workspace.new(idfFile)

# reverse translate EnergyPlus file
translator = OpenStudio::EnergyPlus::ReverseTranslator.new()

# convert to model
model = translator.translateWorkspace(workspace)

# save as osm
osmPath = idfPath.replace_extension('osm')
model.toIdfFile().save(osmPath,true)

# load from osm
oIdfFile = OpenStudio::IdfFile.load(osmPath)
if not oIdfFile
  raise "Loading Model from osm failed."
end
idfFile = oIdfFile.get()
model = OpenStudio::Model::Model.new(idfFile)
