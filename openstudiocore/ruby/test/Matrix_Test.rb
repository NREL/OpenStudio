########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

class Matrix_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_constructor

    m = OpenStudio::Matrix.new(0,0)
    assert_equal(0, m.size1())
    assert_equal(0, m.size2())

    m = OpenStudio::Matrix.new(2,0)
    assert_equal(2, m.size1())
    assert_equal(0, m.size2())

     m = OpenStudio::Matrix.new(0,2)
    assert_equal(0, m.size1())
    assert_equal(2, m.size2())

    m = OpenStudio::Matrix.new(2,2)
    assert_equal(2, m.size1())
    assert_equal(2, m.size2())

    m = OpenStudio::Matrix.new(2,2,-1)
    assert_equal(2, m.size1())
    assert_equal(2, m.size2())
    assert_equal(-1, m[0,0])
    assert_equal(-1, m[0,1])
    assert_equal(-1, m[1,0])
    assert_equal(-1, m[1,1])

    m[0,0] = 1
    m[0,1] = 2
    m[1,0] = 3
    m[1,1] = 4

    assert_equal(1, m[0,0])
    assert_equal(2, m[0,1])
    assert_equal(3, m[1,0])
    assert_equal(4, m[1,1])
  end

  def test_interp1

    x = OpenStudio::Vector.new(2)
    y = OpenStudio::Vector.new(2)
    v = OpenStudio::Matrix.new(2,2)

    x[0] = 0.0
    x[1] = 1.0
    y[0] = 0.0
    y[1] = 1.0
    v[0,0] = 0.0
    v[1,0] = 1.0
    v[0,1] = -1.0
    v[1,1] = 2.0

    assert_in_delta(0.0, OpenStudio::interp(x, y, v, 0.0, 0.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.0, OpenStudio::interp(x, y, v, 1.0, 0.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(-1.0, OpenStudio::interp(x, y, v, 0.0, 1.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(2.0, OpenStudio::interp(x, y, v, 1.0, 1.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.5, OpenStudio::interp(x, y, v, 0.5, 0.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(-0.5, OpenStudio::interp(x, y, v, 0.0, 0.5, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.5, OpenStudio::interp(x, y, v, 0.5, 1.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(1.5, OpenStudio::interp(x, y, v, 1.0, 0.5, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.5, OpenStudio::interp(x, y, v, 0.5, 0.5, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, v, 1.5, 0.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, v, -1.5, 0.0, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, v, 0.0, 1.5, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, v, 0.0, -1.5, OpenStudio::LinearInterp, OpenStudio::NoneExtrap), 0.000001)
  end

  def test_interp2

    v = OpenStudio::Matrix.new(10,10)
    for i in (0..v.size1()-1)
      for j in (0..v.size2()-1)
        v[i,j] = 10*i+j
      end
    end

    x = OpenStudio::linspace(-100, 0, 10)
    y = OpenStudio::linspace(0, 100, 10)

    assert_in_delta(0.0, OpenStudio::interp(x, y, v, -100.0, 0.0, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, v, -105.5, 0.0, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(0.0, OpenStudio::interp(x, y, v, -105.5, -5.5, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(9.0, OpenStudio::interp(x, y, v, -100.0, 100.0, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(9.0, OpenStudio::interp(x, y, v, -100.0, 105.5, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(90.0, OpenStudio::interp(x, y, v, 0.0, 0.0, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(90.0, OpenStudio::interp(x, y, v, 5.5, 0.0, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(99.0, OpenStudio::interp(x, y, v, -0.1, 99.9, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(99.0, OpenStudio::interp(x, y, v, 0.0, 100.0, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(99.0, OpenStudio::interp(x, y, v, 0.0, 105.5, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
    assert_in_delta(99.0, OpenStudio::interp(x, y, v, 5.5, 105.5, OpenStudio::NearestInterp, OpenStudio::NearestExtrap), 0.000001)
  end

  def test_log

    m = OpenStudio::Matrix.new(3,3)
    for i in (0..m.size1()-1)
      for j in (0..m.size2()-1)
        m[i,j] = 10.0**(i+j)
      end
    end

    logM = OpenStudio::log(m, 10.0)

    assert_equal(m.size1(), logM.size1())
    assert_equal(m.size2(), logM.size2())
    for i in (0..m.size1()-1)
      for j in (0..m.size2()-1)
        assert_in_delta(i+j, logM[i,j], 0.000001)
      end
    end
  end

  def test_randMatrix

    m = OpenStudio::randMatrix(1.0,1.0,10,10)
    assert_equal(1.0, OpenStudio::maximum(m))
    assert_equal(1.0, OpenStudio::minimum(m))

    m = OpenStudio::randMatrix(-1.0,1.0,10,10)
    assert(1.0 >= OpenStudio::maximum(m))
    assert(-1.0 <= OpenStudio::minimum(m))
  end

  def test_to_s
    assert(!OpenStudio::Matrix.new(2,2,-1).to_s.nil?)
  end

end


