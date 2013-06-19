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

class SimpleModel_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_SimpleModel
    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/SimpleModel.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get
    
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
    
    # create argument map, set argument values, and add new arguments
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr
    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(1.0))
    argument_map["sillHeight"] = sillHeight
    facade = arguments[2].clone
    assert(facade.setValue("North"))
    argument_map["facade"] = facade

    measure.run(model, runner, argument_map)
    result = runner.result
    result.errors.each {|error| puts "error = #{error.logMessage}"}
    assert_equal("Success", result.value.valueName)
    assert(result.errors.empty?)
  end
  

end


