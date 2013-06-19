######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

require 'test/unit'

class Result_Test < Test::Unit::TestCase
  
  def test_constructAndGetGoodResults
  
    voidResult = OpenStudio::StandardsInterface::VoidResult.new
    assert(voidResult.good?)
    
    boolResult = OpenStudio::StandardsInterface::BoolResult.new(true)
    assert(boolResult.good?)
    value = boolResult.get
    assert(value)
    
    integerResult = OpenStudio::StandardsInterface::IntegerResult.new(-1)
    assert(integerResult.good?)
    value = integerResult.get
    assert(value == -1)
    
    doubleResult = OpenStudio::StandardsInterface::DoubleResult.new(3.4)
    assert(doubleResult.good?)
    value = doubleResult.get
    assert(value == 3.4)
    
    stringResult = OpenStudio::StandardsInterface::StringResult.new("hello")
    assert(stringResult.good?)
    value = stringResult.get
    assert(value == "hello")
    
    model = OpenStudio::Model::Model.new
    building = model.getBuilding
    
    objectResult = OpenStudio::StandardsInterface::ModelObjectResult.new(building)
    assert(objectResult.good?)
    value = objectResult.get
    assert(value == building)
    
    objectVector = OpenStudio::Model::ModelObjectVector.new
    objectVector.push(building)
    
    objectVectorResult = OpenStudio::StandardsInterface::ModelObjectVectorResult.new(objectVector)
    assert(objectVectorResult.good?)
    value = objectVectorResult.get
    assert(value.size == objectVector.size)
    assert(value[0] == building)
    
  end
  
  def test_constructBadResultsAndGetMessages
  
    voidResult = OpenStudio::StandardsInterface::VoidResult.new(1,"Simulate first please.")
    assert_equal(false,voidResult.good?)
    assert(voidResult.what == "Simulate first please.")
    
    integerResult = OpenStudio::StandardsInterface::IntegerResult.new(2,"Haven't wrapped that yet.")
    assert_equal(false,integerResult.good?)
    assert(integerResult.what == "Haven't wrapped that yet.")
  
  end
  
end