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

class ExtendedRunner < OpenStudio::Ruleset::OSRunner
  
  # overload ctor
  def initialize 
    super
    @past_results = []
  end

  # overloaded method
  def prepareForUserScriptRun(userScript)
    @past_results << result
    super
  end
  
  # new method
  def past_results
    return @past_results
  end
end

class TestMeasure < OpenStudio::Ruleset::ModelUserScript
  def name
    return "Test Measure"
  end

  # return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    return result
  end

  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end
  end

end  
   
class OSRunner_Test < MiniTest::Unit::TestCase

  def test_OSRunner
    runner = ExtendedRunner.new
    assert(runner.is_a?(ExtendedRunner))
    assert(runner.is_a?(OpenStudio::Ruleset::OSRunner))
    assert_equal(0, runner.past_results.size)
    
    model = OpenStudio::Model::Model.new
    args = OpenStudio::Ruleset::OSArgumentMap.new
    test_measure = TestMeasure.new
    
    test_measure.run(model, runner, args)
    assert_equal(1, runner.past_results.size)

    test_measure.run(model, runner, args)
    assert_equal(2, runner.past_results.size)
     
    test_measure.run(model, runner, args)
    assert_equal(3, runner.past_results.size)
  end
  
end

