######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
    assert_not_equal(group.handle(), spaceClone.get.children()[0].handle())
    assert((not spaceClone.get.children()[0].to_ParentObject().empty?()))
    assert_equal(1, spaceClone.get.children()[0].to_ParentObject().get().children().size())
    assert_not_equal(surface.handle(), spaceClone.get.children()[0].to_ParentObject().get().children()[0].handle());
  end

end
