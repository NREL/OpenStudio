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

require 'minitest/autorun'

class Construction_Test < MiniTest::Unit::TestCase

  def test_ConstructionConstructors
  
    model = OpenStudio::Model::Model.new
    
    construction = OpenStudio::Model::Construction.new(model)
    construction.outputVariables
    material = OpenStudio::Model::StandardOpaqueMaterial.new(model)
    egValues = Array.new
    egValues.push(material.name.get)
    construction.pushExtensibleGroup(egValues)
    construction.pushExtensibleGroup(egValues)
    construction.pushExtensibleGroup(egValues)
    
    assert(construction.numExtensibleGroups == 3)
    oTarget = construction.getExtensibleGroup(1).to_ModelExtensibleGroup.get.getTarget(0)
    assert_equal(false,oTarget.empty?)
    assert_equal(false,oTarget.get.to_ModelObject.empty?)
    assert_equal(false,oTarget.get.to_ModelObject.get.to_StandardOpaqueMaterial.empty?)
    assert_equal(material,oTarget.get.to_ModelObject.get.to_StandardOpaqueMaterial.get)
    
    clonedModel = model.clone.to_Model
    assert_equal(2,clonedModel.numObjects)
  
  end
  
  def test_Construction_SetUFactor
  
    # same as test in src/model/test/Construction_GTest.cpp
    # has different behavior in C++ versus Ruby bindings
    # change ModelFixture logLevel to Trace to see messages showing failure of setting 
    # thickness < 0 in C++ and success of the same thing in Ruby
    
    soLogger = OpenStudio::Logger::instance.standardOutLogger
    level = soLogger.logLevel
    soLogger.setLogLevel(-3)
    
    model = OpenStudio::Model::exampleModel
    c = OpenStudio::Model::getConstructionByName(model,"Exterior Wall").get
    insulation = c.insulation.get.to_StandardOpaqueMaterial.get
    puts "original conductance = " + c.thermalConductance.get.to_s
    originalResistance = 1.0 / c.thermalConductance.get
    puts "original resistance = " + originalResistance.to_s
    insulationResistance = insulation.thermalResistance
    puts "insulation resistance = " + insulationResistance.to_s
    newResistance = 0.8 * (originalResistance - insulationResistance)
    puts "new conductance = " + (1.0/newResistance).to_s
    result = c.setConductance(1.0 / newResistance)
    assert((not result))
    if result
      assert(insulation.thickness < 0.0)
    else
      assert(insulation.thickness > 0.0)
    end
    
    soLogger.setLogLevel(level.get) if not level.empty?
  
  end
  
end

