######################################################################
#  Copyright (c) 2008-2012, Alliance for Sustainable Energy.  
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

