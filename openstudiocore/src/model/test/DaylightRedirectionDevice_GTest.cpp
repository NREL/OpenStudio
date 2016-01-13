/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

TEST_F(ModelFixture, DaylightRedirectionDevice_Material) {
  Model model;

  DaylightRedirectionDevice drd(model);

  ShadingControl shadingControl(drd);
  EXPECT_FALSE(shadingControl.construction());
  ASSERT_TRUE(shadingControl.shadingMaterial());
  EXPECT_EQ(drd.handle(), shadingControl.shadingMaterial()->handle());
  EXPECT_EQ("InteriorDaylightRedirectionDevice", shadingControl.shadingType());
  EXPECT_EQ("AlwaysOn", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

}


TEST_F(ModelFixture, DaylightRedirectionDevice_Construction) {
  Model model;

  SimpleGlazing glazing(model);
  DaylightRedirectionDevice drd(model);

  {
    // don't allow between glass daylight redirection
    Construction construction(model);
    EXPECT_TRUE(construction.insertLayer(0, glazing));
    EXPECT_TRUE(construction.insertLayer(1, drd));
    EXPECT_TRUE(construction.insertLayer(2, glazing));

    EXPECT_THROW(ShadingControl shadingControl(construction), openstudio::Exception);
  }
  
  {
    // don't allow exterior daylight redirection
    Construction construction(model);
    EXPECT_TRUE(construction.insertLayer(0, drd));
    EXPECT_TRUE(construction.insertLayer(1, glazing));
    
    EXPECT_THROW(ShadingControl shadingControl(construction), openstudio::Exception);
  }
  

  // proper placement inside the glass
  Construction construction(model);
  EXPECT_TRUE(construction.insertLayer(0, glazing));
  EXPECT_TRUE(construction.insertLayer(1, drd));

  ShadingControl shadingControl(construction);
  
  EXPECT_FALSE(shadingControl.shadingMaterial());
  ASSERT_TRUE(shadingControl.construction());
  EXPECT_EQ(construction.handle(), shadingControl.construction()->handle());
  EXPECT_EQ("InteriorDaylightRedirectionDevice", shadingControl.shadingType());
  EXPECT_EQ("AlwaysOn", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

}