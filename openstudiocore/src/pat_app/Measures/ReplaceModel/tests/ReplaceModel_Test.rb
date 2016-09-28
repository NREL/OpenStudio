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


