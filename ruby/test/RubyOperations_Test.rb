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

require 'set'

require 'minitest/autorun'

class RubyOperations_Test < MiniTest::Unit::TestCase

  def test_ModelObjectUniq

    model = OpenStudio::Model::Model.new

    # create two spaces
    OpenStudio::Model::Space.new(model)
    OpenStudio::Model::Space.new(model)

    array1 = []
    modelobjectvector1 = OpenStudio::Model::ModelObjectVector.new

    spaces = model.getSpaces
    assert(spaces.size == 2)
    a1 = spaces[0]
    b1 = spaces[1]
    array1 << spaces[0]
    array1 << spaces[1]
    modelobjectvector1 << spaces[0]
    modelobjectvector1 << spaces[1]

    spaces = model.getSpaces
    assert(spaces.size == 2)
    a2 = spaces[0]
    b2 = spaces[1]
    array1 << spaces[0]
    array1 << spaces[1]
    modelobjectvector1 << spaces[0]
    modelobjectvector1 << spaces[1]

    spaces = model.getSpaces
    assert(spaces.size == 2)
    a3 = spaces[0]
    b3 = spaces[1]
    array1 << spaces[0]
    array1 << spaces[1]
    modelobjectvector1 << spaces[0]
    modelobjectvector1 << spaces[1]

    assert(a1 == a2)
    assert(a1 == a3)
    assert(a1 != b1)
    assert(b1 == b2)
    assert(b1 == b3)

    assert(a1.hash == a2.hash)
    assert(a1.hash == a3.hash)
    assert(a1.hash != b1.hash)
    assert(b1.hash == b2.hash)
    assert(b1.hash == b3.hash)

    assert(a1.eql?(a2))
    assert(a1.eql?(a3))
    assert((not a1.eql?(b1)))
    assert(b1.eql?(b2))
    assert(b1.eql?(b3))

    assert((a1 <=> a2) == 0)
    assert((a1 <=> a3) == 0)
    assert((a1 <=> b1) != 0)
    assert((b1 <=> b2) == 0)
    assert((b1 <=> b3) == 0)

    array2 = array1.uniq
    puts array2

    assert(array2.length == 2)
    assert(array2[0] == a1)
    assert(array2[1] == b1)

    # DLM: would like to be able to do this too
    # modelobjectvector2 = modelobjectvector1.uniq
    # JMT:
    # ModelObjectVector is enumerable, not an array. Uniq is a method
    # on array. So, this works.
    modelobjectvector2 = modelobjectvector1.to_a.uniq
    puts modelobjectvector2

    assert(modelobjectvector2.length == 2)
    assert(modelobjectvector2[0] == a1)
    assert(modelobjectvector2[1] == b1)

    set1 = Set.new
    set1 << a1
    set1 << b1
    assert(set1.size == 2)
    assert(set1.include?(a1))
    assert(set1.include?(b1))
    assert(set1.include?(a2))
    assert(set1.include?(b2))
    assert(set1.include?(a3))
    assert(set1.include?(b3))

    set1 << a2
    set1 << b2
    assert(set1.size == 2)
    assert(set1.include?(a1))
    assert(set1.include?(b1))
    assert(set1.include?(a2))
    assert(set1.include?(b2))
    assert(set1.include?(a3))
    assert(set1.include?(b3))
  end

  def test_ModelObjectMarshal

    model = OpenStudio::Model::Model.new

    a = OpenStudio::Model::Space.new(model)

    str = Marshal.dump(a)

    b = Marshal.load(str)

    model2 = OpenStudio::Model::Model.new

    assert(model2.addObject(b))

  end

  def test_ModelObjectSort

    model = OpenStudio::Model::Model.new

    OpenStudio::Model::Space.new(model)
    OpenStudio::Model::Space.new(model)
    OpenStudio::Model::Space.new(model)
    OpenStudio::Model::Space.new(model)

    spaces = model.getSpaces
    assert(spaces.size == 4)
    a = spaces[0]
    b = spaces[1]
    c = spaces[2]
    d = spaces[3]

    array1 = [a, b, c, d]
    array2 = [d, c, b, a]

    array1 = array1.sort
    array2 = array2.sort

    array1.each_index do |i|
      assert(array1[i].eql?(array2[i]))
    end

  end

  def test_ImplicitPathSaveLoad

    model = OpenStudio::Model::Model.new

    model.save("./test.osm")

    model.save("./test.osm", true)

    m2 = OpenStudio::Model::Model::load("./test.osm")
    assert((not m2.empty?))

  end

end
