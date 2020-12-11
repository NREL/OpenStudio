/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../RenderingColor.hpp"
#include "../RenderingColor_Impl.hpp"

#include "../../utilities/idf/WorkspaceWatcher.hpp"

#include <utilities/idd/OS_Rendering_Color_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RenderingColor) {

  Model model;
  RenderingColor color(model);
  EXPECT_GE(color.renderingRedValue(), 0);
  EXPECT_LE(color.renderingRedValue(), 255);
  EXPECT_GE(color.renderingGreenValue(), 0);
  EXPECT_LE(color.renderingGreenValue(), 255);
  EXPECT_GE(color.renderingBlueValue(), 0);
  EXPECT_LE(color.renderingBlueValue(), 255);
  EXPECT_EQ(255, color.renderingAlphaValue());
  EXPECT_TRUE(color.isRenderingAlphaValueDefaulted());
}

TEST_F(ModelFixture, RenderingColor2) {

  Model model;
  model.addObject(IdfObject(IddObjectType::OS_Rendering_Color));

  ASSERT_EQ(1u, model.getModelObjects<RenderingColor>().size());

  RenderingColor color = model.getModelObjects<RenderingColor>()[0];
  EXPECT_GE(color.renderingRedValue(), 0);
  EXPECT_LE(color.renderingRedValue(), 255);
  EXPECT_GE(color.renderingGreenValue(), 0);
  EXPECT_LE(color.renderingGreenValue(), 255);
  EXPECT_GE(color.renderingBlueValue(), 0);
  EXPECT_LE(color.renderingBlueValue(), 255);
  EXPECT_EQ(255, color.renderingAlphaValue());
  EXPECT_TRUE(color.isRenderingAlphaValueDefaulted());
}

TEST_F(ModelFixture, RenderingColor3) {

  IdfObject idfObject(IddObjectType::OS_Rendering_Color);
  EXPECT_TRUE(idfObject.setInt(OS_Rendering_ColorFields::RenderingRedValue, 42));
  EXPECT_TRUE(idfObject.setInt(OS_Rendering_ColorFields::RenderingGreenValue, 42));
  EXPECT_TRUE(idfObject.setInt(OS_Rendering_ColorFields::RenderingBlueValue, 42));
  EXPECT_TRUE(idfObject.setInt(OS_Rendering_ColorFields::RenderingAlphaValue, 42));

  Model model;
  model.addObject(idfObject);

  ASSERT_EQ(1u, model.getModelObjects<RenderingColor>().size());

  RenderingColor color = model.getModelObjects<RenderingColor>()[0];
  EXPECT_EQ(42, color.renderingRedValue());
  EXPECT_EQ(42, color.renderingGreenValue());
  EXPECT_EQ(42, color.renderingBlueValue());
  EXPECT_EQ(42, color.renderingAlphaValue());
}

TEST_F(ModelFixture, RenderingColor4) {

  Model model;

  for (int i = -1; i <= 256; ++i) {
    RenderingColor color(model);
    EXPECT_GE(color.renderingRedValue(), 0);
    EXPECT_LE(color.renderingRedValue(), 255);
    EXPECT_GE(color.renderingGreenValue(), 0);
    EXPECT_LE(color.renderingGreenValue(), 255);
    EXPECT_GE(color.renderingBlueValue(), 0);
    EXPECT_LE(color.renderingBlueValue(), 255);
    EXPECT_EQ(255, color.renderingAlphaValue());
    EXPECT_TRUE(color.isRenderingAlphaValueDefaulted());

    if (i < 0 || i > 255) {
      EXPECT_FALSE(color.setRenderingAlphaValue(i));
      EXPECT_EQ(255, color.renderingAlphaValue());
      EXPECT_TRUE(color.isRenderingAlphaValueDefaulted());
    } else {
      EXPECT_TRUE(color.setRenderingAlphaValue(i));
      EXPECT_EQ(i, color.renderingAlphaValue());
      EXPECT_FALSE(color.isRenderingAlphaValueDefaulted());
    }
  }
}

// the following tests demonstrate issues with initialization
// may need a similar test for another object that requires other model objects
// be connected during initializer

class RenderingColorWorkspaceWatcher : public openstudio::WorkspaceWatcher
{

 public:
  RenderingColorWorkspaceWatcher(const Workspace& workspace) : WorkspaceWatcher(workspace) {}

  virtual void onObjectAdd(const WorkspaceObject& addedObject) override {
    WorkspaceWatcher::onObjectAdd(addedObject);

    EXPECT_EQ(IddObjectType::OS_Rendering_Color, addedObject.iddObject().type().value());
    boost::optional<RenderingColor> color = addedObject.optionalCast<RenderingColor>();
    ASSERT_TRUE(color);
    ASSERT_NO_THROW(color->renderingRedValue());
    ASSERT_NO_THROW(color->renderingGreenValue());
    ASSERT_NO_THROW(color->renderingBlueValue());
    EXPECT_GE(color->renderingRedValue(), 0);
    EXPECT_LE(color->renderingRedValue(), 255);
    EXPECT_GE(color->renderingGreenValue(), 0);
    EXPECT_LE(color->renderingGreenValue(), 255);
    EXPECT_GE(color->renderingBlueValue(), 0);
    EXPECT_LE(color->renderingBlueValue(), 255);
  }
};

TEST_F(ModelFixture, RenderingColor_Initializer1) {
  Model model;
  RenderingColorWorkspaceWatcher watcher(model);
  EXPECT_FALSE(watcher.objectAdded());

  RenderingColor color(model);
  EXPECT_TRUE(watcher.objectAdded());

  EXPECT_GE(color.renderingRedValue(), 0);
  EXPECT_LE(color.renderingRedValue(), 255);
  EXPECT_GE(color.renderingGreenValue(), 0);
  EXPECT_LE(color.renderingGreenValue(), 255);
  EXPECT_GE(color.renderingBlueValue(), 0);
  EXPECT_LE(color.renderingBlueValue(), 255);
}

TEST_F(ModelFixture, RenderingColor_Initializer2) {
  Model model;
  RenderingColorWorkspaceWatcher watcher(model);
  EXPECT_FALSE(watcher.objectAdded());

  IdfObject idfObject(IddObjectType::OS_Rendering_Color);
  model.addObject(idfObject);
  EXPECT_TRUE(watcher.objectAdded());

  std::vector<RenderingColor> colors = model.getModelObjects<RenderingColor>();
  ASSERT_EQ(1u, colors.size());

  RenderingColor color = colors[0];
  EXPECT_GE(color.renderingRedValue(), 0);
  EXPECT_LE(color.renderingRedValue(), 255);
  EXPECT_GE(color.renderingGreenValue(), 0);
  EXPECT_LE(color.renderingGreenValue(), 255);
  EXPECT_GE(color.renderingBlueValue(), 0);
  EXPECT_LE(color.renderingBlueValue(), 255);
}

TEST_F(ModelFixture, RenderingColor_ColorString) {
  Model model;
  boost::optional<RenderingColor> test;

  RenderingColor red(model);
  red.setRenderingRedValue(255);
  red.setRenderingGreenValue(0);
  red.setRenderingBlueValue(0);
  std::string redString = red.colorString();
  EXPECT_EQ("#FF0000", redString);
  test = RenderingColor::fromColorString(redString, model);
  ASSERT_TRUE(test);
  EXPECT_EQ(255, test->renderingRedValue());
  EXPECT_EQ(0, test->renderingGreenValue());
  EXPECT_EQ(0, test->renderingBlueValue());

  RenderingColor green(model);
  green.setRenderingRedValue(0);
  green.setRenderingGreenValue(255);
  green.setRenderingBlueValue(0);
  std::string greenString = green.colorString();
  EXPECT_EQ("#00FF00", greenString);
  test = RenderingColor::fromColorString(greenString, model);
  ASSERT_TRUE(test);
  EXPECT_EQ(0, test->renderingRedValue());
  EXPECT_EQ(255, test->renderingGreenValue());
  EXPECT_EQ(0, test->renderingBlueValue());

  RenderingColor blue(model);
  blue.setRenderingRedValue(0);
  blue.setRenderingGreenValue(0);
  blue.setRenderingBlueValue(255);
  std::string blueString = blue.colorString();
  EXPECT_EQ("#0000FF", blueString);
  test = RenderingColor::fromColorString(blueString, model);
  ASSERT_TRUE(test);
  EXPECT_EQ(0, test->renderingRedValue());
  EXPECT_EQ(0, test->renderingGreenValue());
  EXPECT_EQ(255, test->renderingBlueValue());

  RenderingColor black(model);
  black.setRenderingRedValue(0);
  black.setRenderingGreenValue(0);
  black.setRenderingBlueValue(0);
  std::string blackString = black.colorString();
  EXPECT_EQ("#000000", blackString);
  test = RenderingColor::fromColorString(blackString, model);
  ASSERT_TRUE(test);
  EXPECT_EQ(0, test->renderingRedValue());
  EXPECT_EQ(0, test->renderingGreenValue());
  EXPECT_EQ(0, test->renderingBlueValue());

  RenderingColor white(model);
  white.setRenderingRedValue(255);
  white.setRenderingGreenValue(255);
  white.setRenderingBlueValue(255);
  std::string whiteString = white.colorString();
  EXPECT_EQ("#FFFFFF", whiteString);
  test = RenderingColor::fromColorString(whiteString, model);
  ASSERT_TRUE(test);
  EXPECT_EQ(255, test->renderingRedValue());
  EXPECT_EQ(255, test->renderingGreenValue());
  EXPECT_EQ(255, test->renderingBlueValue());
}