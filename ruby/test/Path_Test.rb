########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'pathname'

require 'minitest/autorun'

def changeTestPath(p)
  p  = OpenStudio::Path.new("./there")
  return p
end

class Path_Test < Minitest::Test

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


