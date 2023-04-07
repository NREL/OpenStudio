########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'

require 'pathname'

require 'minitest/autorun'

def changeTestPath(p)
  p  = OpenStudio::Path.new("./there")
  return p
end

class Path_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_path
    p = OpenStudio::Path.new("./here")
    assert_equal("./here", p.to_s())
  end

  def test_function_accepting_path_takes_a_string
    w = OpenStudio::Workspace::load('hello.idf')
    assert w.empty?
  end


  def DISABLED_test_funcOnlyTakesAPath

    functions = []
    functions << OpenStudio.method("funcOnlyTakesAPath")
    functions << OpenStudio.method("funcOnlyTakesAConstPath")
    functions << OpenStudio.method("funcOnlyTakesAConstPathRef")

    functions.each do |function|

      p = OpenStudio::Path.new("./here")
      q = function.call(p)
      assert_equal(p.to_s, q.to_s)

      # would like to be able to call with string
      s = p.to_s
      assert_equal(s.class, String)
      q = function.call(s)
      assert_equal(p.to_s, q.to_s)

      # do we need to be able to handle Pathname too?
      pn = Pathname.new(p.to_s)
      assert_equal(pn.class, Pathname)
      #q = function.call(pn)
      #assert_equal(p.to_s, q.to_s)
      assert_raises TypeError do
        function.call(pn)
      end

      # expected failure, pass double
      assert_raises TypeError do
        function.call(7)
      end

      # expected failure, pass nil
      assert_raises ArgumentError do
        function.call(nil)
      end

    end
  end

  def DISABLED_test_defaultArgFuncTakesAPath

    functions = []
    functions << OpenStudio.method("defaultArgFuncTakesAPath")
    functions << OpenStudio.method("defaultArgFuncTakesAConstPath")
    functions << OpenStudio.method("defaultArgFuncTakesAConstPathRef")

    second_args = [nil, true, false]

    functions.each do |function|

      p = OpenStudio::Path.new("./here")
      q = function.call(p)
      assert_equal(p.to_s, q.to_s)
      q = function.call(p, true)
      assert_equal(p.to_s, q.to_s)
      q = function.call(p, false)
      assert_equal("", q.to_s)

      # would like to be able to call with string
      s = p.to_s
      assert_equal(s.class, String)
      q = function.call(s)
      assert_equal(p.to_s, q.to_s)
      q = function.call(s, true)
      assert_equal(p.to_s, q.to_s)
     q = function.call(s, false)
      assert_equal("", q.to_s)

      # do we need to be able to handle Pathname too?
      pn = Pathname.new(p.to_s)
      assert_equal(pn.class, Pathname)
      #q = function.call(pn)
      #assert_equal(p.to_s, q.to_s)
      assert_raises ArgumentError do
        function.call(pn)
      end
      assert_raises ArgumentError do
        function.call(pn, true)
      end
      assert_raises ArgumentError do
        function.call(pn, false)
      end

      # expected failure, pass double
      assert_raises ArgumentError do
        function.call(7)
      end
      assert_raises ArgumentError do
        function.call(7, true)
      end
      assert_raises ArgumentError do
        function.call(7, false)
      end

      # expected failure, pass nil
      assert_raises ArgumentError do
        function.call(nil)
      end
      assert_raises ArgumentError do
        function.call(nil, true)
      end
      assert_raises ArgumentError do
        function.call(nil, false)
      end
    end

  end

  def test_PathCopy
    p = OpenStudio::Path.new("./here")
    assert_equal("./here", p.to_s())

    q = p
    assert_equal("./here", q.to_s())
    assert_equal("./here", p.to_s())

    q = OpenStudio::Path.new("./there")
    assert_equal("./there", q.to_s())
    assert_equal("./here", p.to_s())

    q = changeTestPath(p)
    assert_equal("./there", q.to_s())
    assert_equal("./here", p.to_s())

  end
end


