########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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

