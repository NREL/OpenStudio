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

