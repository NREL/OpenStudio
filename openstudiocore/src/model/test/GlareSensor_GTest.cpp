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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../GlareSensor.hpp"
#include "../GlareSensor_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GlareSensor_Clone)
{
  Model model;

  GlareSensor control(model);
  Space space(model);

  EXPECT_FALSE(control.space());
  EXPECT_EQ(0u, space.glareSensors().size());

  control.setSpace(space);

  ASSERT_TRUE(control.space());
  EXPECT_EQ(space.handle(), control.space()->handle());
  EXPECT_EQ(1u, space.glareSensors().size());

  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<GlareSensor>().size());
  ModelObject object = control.clone(model);
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<GlareSensor>().size());

  ASSERT_TRUE(object.optionalCast<GlareSensor>());
  EXPECT_EQ(space.handle(), control.space()->handle());
  EXPECT_EQ(space.handle(), object.cast<GlareSensor>().space()->handle());
  EXPECT_EQ(2u, space.glareSensors().size());

}
