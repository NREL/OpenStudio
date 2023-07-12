/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
