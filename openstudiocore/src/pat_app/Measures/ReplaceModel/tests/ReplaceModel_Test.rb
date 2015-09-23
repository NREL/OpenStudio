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

require 'minitest/autorun'

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
    measures_json = arguments[1].clone
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_01.osm")
    assert(alternativeModelPath.setValue(path))
    argument_map["alternativeModelPath"] = alternativeModelPath
    argument_map["measures_json"] = measures_json
    
    # run the measure and test that it works
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Success")
    assert(result.errors.empty?)
    assert(result.warnings.empty?)
    
    assert_equal(0, model.getLifeCycleCosts.size)
  end
  
  def test_ReplaceModel_AltMeasures
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
    measures_json = arguments[1].clone
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_01.osm")
    assert(alternativeModelPath.setValue(path))
    assert(measures_json.setValue('[{"capitalCost":10,"description":"Random Measure Description","displayName":"Random Measure","taxonomyTag":"Envelope.Form","uuid":"{591f1102-f62f-4787-abcf-bff2299502ba}"},{"capitalCost":100,"description":"Other Random Measure Description","displayName":"Other Random Measure","taxonomyTag":"HVAC.Heating","uuid":"{9664f5c7-c6ff-4679-a931-4ac7c5b9baa8}"}]'))
    argument_map["alternativeModelPath"] = alternativeModelPath
    argument_map["measures_json"] = measures_json
    
    # run the measure and test that it works
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Success")
    assert(result.errors.empty?)
    assert(result.warnings.empty?)
    
    assert_equal(2, model.getLifeCycleCosts.size)
    
  end
  

end


