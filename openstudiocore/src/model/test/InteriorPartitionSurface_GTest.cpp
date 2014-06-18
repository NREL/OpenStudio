/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "ModelFixture.hpp"

#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurface_Impl.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../InteriorPartitionSurfaceGroup_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, InteriorPartitionSurface)
{
  Model model;
 
  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));

  EXPECT_NO_THROW(InteriorPartitionSurface(points, model));

  InteriorPartitionSurface interiorPartitionSurface(points, model);
  EXPECT_FALSE(interiorPartitionSurface.interiorPartitionSurfaceGroup());
}
