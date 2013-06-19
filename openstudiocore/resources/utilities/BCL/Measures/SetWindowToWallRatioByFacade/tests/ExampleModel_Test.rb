######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'openstudio'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'test/unit'

class ExampleModel_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_ExampleModel
    # get the example model
    model = OpenStudio::Model::exampleModel
    
    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    assert_equal(3, arguments.size)
    assert_equal("wwr", arguments[0].name)
    assert_equal("sillHeight", arguments[1].name)
    assert_equal("facade", arguments[2].name)
    
    # try running the measure on the test model with no values
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
    assert((not result.errors.empty?))
    
    # set argument values to bad values and try again
    wwr = arguments[0].clone
    assert(wwr.setValue(40.0))
    argument_map["wwr"] = wwr
    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(-1.0))
    argument_map["sillHeight"] = sillHeight
    facade = arguments[2].clone
    assert((not facade.setValue("Up"))) # cannot set this choice argument to bad value
    argument_map["facade"] = facade
    
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
    assert((not result.errors.empty?))
    
    # correct argument values one at a time
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
    assert((not result.errors.empty?))
    
    # correct argument values one at a time
    assert(sillHeight.setValue(0.1))
    argument_map["sillHeight"] = sillHeight
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
    assert((not result.errors.empty?))
    
    # try with good argument values
    assert(facade.setValue("North"))
    argument_map["facade"] = facade
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Success")
    assert(result.errors.empty?)
    assert(result.warnings.empty?)
  end
  

end


