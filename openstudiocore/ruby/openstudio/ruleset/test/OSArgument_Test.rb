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

require 'minitest/autorun'

class TestScript < OpenStudio::Ruleset::UtilityUserScript

  def name
    return "TestScript"
  end

  def run(runner,arguments)
    super(runner,arguments)
    puts "Number of arguments: " + arguments.size.to_s
  end

end

class OSArgument_Test < MiniTest::Unit::TestCase

  def test_DeprecatedNames

    # official names
    opt_arg = OpenStudio::Ruleset::OptionalOSArgument.new
    assert(opt_arg.empty?)
    arg_vec = OpenStudio::Ruleset::OSArgumentVector.new
    assert(arg_vec.empty?)
    arg_map = OpenStudio::Ruleset::OSArgumentMap.new
    assert(arg_map.empty?)    
    # deprecated names
    opt_arg = OpenStudio::Ruleset::OptionalUserScriptArgument.new
    assert(opt_arg.empty?)
    arg_vec = OpenStudio::Ruleset::UserScriptArgumentVector.new
    assert(arg_vec.empty?)
    arg_map = OpenStudio::Ruleset::UserScriptArgumentMap.new
    assert(arg_map.empty?)

    opt_arg = OpenStudio::Ruleset::UserScriptArgument::makeDoubleArgument("number")
    arg_vec << opt_arg
    arg_map["number"] = arg_vec[0]

    script = TestScript.new
    runner = OpenStudio::Ruleset::OSRunner.new
    script.run(runner,arg_map)
    result = runner.result
    assert(result.value.valueName == "Success")
    assert(result.value.value == 0)
  end

end

