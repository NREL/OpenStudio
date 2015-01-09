######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

class ReplaceModel_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_ReplaceModel
    # get the example model
    model = OpenStudio::Model::exampleModel
    
    # create an instance of the measure
    measure = ReplaceModel.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    
    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    
    alternativeModelPath = arguments[0].clone
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_01.osm")
    assert(alternativeModelPath.setValue(path))
    argument_map["alternativeModelPath"] = alternativeModelPath
    
    # run the measure and test that it works
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Success")
    assert(result.errors.empty?)
    assert(result.warnings.empty?)
  end
  

end


