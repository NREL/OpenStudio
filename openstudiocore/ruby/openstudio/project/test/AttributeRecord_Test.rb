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


