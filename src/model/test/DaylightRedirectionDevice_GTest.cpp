/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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