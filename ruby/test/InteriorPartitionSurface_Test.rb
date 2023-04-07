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

require 'minitest/autorun'

class InteriorPartitionSurface_Test < MiniTest::Unit::TestCase

  def test_hierarchy
    workspace = OpenStudio::Workspace.new
    model = OpenStudio::Model::Model.new

    # add a zone
    space = OpenStudio::Model::Space.new(model)

    # add a interior partition surface group
    group = OpenStudio::Model::InteriorPartitionSurfaceGroup.new(model)
    assert(group.setSpace(space))
    assert((not group.space().empty?))
    assert_equal(space.handle(),group.space().get.handle())

    # make a new interior partition surface
    points = OpenStudio::Point3dVector.new
    points << OpenStudio::Point3d.new(0, 0, 1)
    points << OpenStudio::Point3d.new(0, 0, 0)
    points << OpenStudio::Point3d.new(1, 0, 0)
    points << OpenStudio::Point3d.new(1, 0, 1)
    surface = OpenStudio::Model::InteriorPartitionSurface.new(points, model)
    assert(surface.setInteriorPartitionSurfaceGroup(group))
    assert((not surface.interiorPartitionSurfaceGroup().empty?))
    assert_equal(group.handle(), surface.interiorPartitionSurfaceGroup().get.handle())

    # check children from group
    assert_equal(1, group.children().size())
    assert_equal(surface.handle(), group.children()[0].handle())

    # now check children from zone
    assert_equal(1, space.children().size())
    assert_equal(group.handle(), space.children()[0].handle())
    assert((not space.children()[0].to_ParentObject().empty?()))
    assert_equal(1, space.children()[0].to_ParentObject().get().children().size())
    assert_equal(surface.handle(), space.children()[0].to_ParentObject().get().children()[0].handle())

    # check parent from group
    assert((not group.parent().empty?))
    assert_equal(space.handle(), group.parent().get.handle())

    # check parent from surface
    assert((not surface.parent().empty?))
    assert_equal(group.handle(), surface.parent().get.handle())
    assert((not surface.parent().get.parent().empty?))
    assert_equal(space.handle(), surface.parent().get.parent().get.handle())

    # clone the space
    clone = space.clone(model)
    spaceClone = model.getSpace(clone.handle())
    assert((not spaceClone.empty?))
    assert_equal(1, spaceClone.get.children().size())
    assert(group.handle().to_s != spaceClone.get.children()[0].handle().to_s)
    assert((not spaceClone.get.children()[0].to_ParentObject().empty?()))
    assert_equal(1, spaceClone.get.children()[0].to_ParentObject().get().children().size())
    assert(surface.handle().to_s != spaceClone.get.children()[0].to_ParentObject().get().children()[0].handle().to_s)
  end

end
