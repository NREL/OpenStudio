/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "../DaylightRedirectionDevice.hpp"
#include "../DaylightRedirectionDevice_Impl.hpp"
#include "../ShadingControl.hpp"
#include "../Construction.hpp"
#include "../Schedule.hpp"
#include "../Blind.hpp"
#include "../SimpleGlazing.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DaylightRedirectionDevice) {
  Model model;

  Construction construction(model);

  SimpleGlazing glazing(model);
  EXPECT_TRUE(construction.insertLayer(0, glazing));

  DaylightRedirectionDevice drd(model);

  ShadingControl shadingControl(drd);
  EXPECT_FALSE(shadingControl.construction());
  ASSERT_TRUE(shadingControl.shadingMaterial());
  EXPECT_EQ(drd.handle(), shadingControl.shadingMaterial()->handle());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_EQ("AlwaysOn", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

}
