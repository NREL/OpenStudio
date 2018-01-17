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

require 'openstudio'

require 'fileutils'

require 'minitest/autorun'

class AttributeRecord_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_AttributeRecord
  
    # create the run manager
    runPath = OpenStudio::Path.new("./run.db")
    FileUtils.rm(runPath.to_s) if OpenStudio::exists(runPath)
    runManager = OpenStudio::Runmanager::RunManager.new(runPath, true, false, false)

    # create the project database
    projectPath = OpenStudio::Path.new("./project.osp")
    FileUtils.rm(projectPath.to_s) if OpenStudio::exists(projectPath)
    database = OpenStudio::Project::ProjectDatabase.new(projectPath, runManager)
    
    model = OpenStudio::Project::FileReferenceRecord.new(
        OpenStudio::FileReference.new(OpenStudio::Path.new("./in.osm")), 
        database)
    
    attribute = OpenStudio::Attribute.new("name", "name")
    attributeRecord = OpenStudio::Project::AttributeRecord.new(attribute, model)
    # ETH@20110919 Inherited method name does not work, but attributeValueAsString does. 
    # Also, get similar RTTI error message concerning .to_ObjectRecord.get inserted into
    # the call. (RTTI about .to_ObjectRecord, not .name in that case.)
    assert_equal("name",attributeRecord.name)
    assert_equal("name",attributeRecord.attributeValueAsString)
  
    endUsesAttribute = OpenStudio::EndUses.new
    endUsesAttribute.addEndUse(1.0, "Electricity".to_EndUseFuelType, "InteriorLights".to_EndUseCategoryType)
    endUsesAttribute.addEndUse(1.0, "Gas".to_EndUseFuelType, "Heating".to_EndUseCategoryType, "General")
    endUsesAttributeRecord = OpenStudio::Project::AttributeRecord.new(endUsesAttribute.attribute, model)
    
    modelAttributeRecords = model.attributeRecords
    assert_equal(2, modelAttributeRecords.size)
    assert_equal("name",modelAttributeRecords[0].name)
    assert_equal("EndUses", modelAttributeRecords[1].name)
    
    assert((not model.getAttributeRecord("name").empty?))
    assert((not model.getAttributeRecord("EndUses").empty?))
   
    assert((OpenStudio::EndUses::fromAttribute(model.getAttributeRecord("name").get.attribute).empty?))
    assert((not OpenStudio::EndUses::fromAttribute(model.getAttributeRecord("EndUses").get.attribute).empty?))
  end

end


