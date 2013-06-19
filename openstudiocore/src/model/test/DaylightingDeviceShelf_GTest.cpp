/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>
#include <model/Model_Impl.hpp>
#include <model/DaylightingDeviceShelf.hpp>
#include <model/DaylightingDeviceShelf_Impl.hpp>
#include <model/Space.hpp>
#include <model/Surface.hpp>
#include <model/SubSurface.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurface.hpp>
#include <model/InteriorPartitionSurfaceGroup.hpp>
#include <model/InteriorPartitionSurface.hpp>

#include <utilities/geometry/Point3d.hpp>
#include <boost/foreach.hpp>

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, DaylightingDeviceShelf)
{
  Model model;

  // triangle with unit area
  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  SubSurface window(points, model);

  DaylightingDeviceShelf shelf(window);
  EXPECT_EQ(window.handle(), shelf.subSurface().handle());
 
}