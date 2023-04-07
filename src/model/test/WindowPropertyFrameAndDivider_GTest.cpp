/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../WindowPropertyFrameAndDivider.hpp"
#include "../WindowPropertyFrameAndDivider_Impl.hpp"
#include "../SubSurface.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include <utilities/idd/OS_WindowProperty_FrameAndDivider_FieldEnums.hxx>

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, WindowPropertyFrameAndDivider) {
  Model model;

  WindowPropertyFrameAndDivider frameAndDivider(model);

  std::vector<Point3d> vertices{
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
  };

  SubSurface subSurface(vertices, model);

  EXPECT_FALSE(subSurface.windowPropertyFrameAndDivider());
  EXPECT_TRUE(subSurface.allowWindowPropertyFrameAndDivider());
  EXPECT_TRUE(subSurface.setWindowPropertyFrameAndDivider(frameAndDivider));
  ASSERT_TRUE(subSurface.windowPropertyFrameAndDivider());

  subSurface.resetWindowPropertyFrameAndDivider();
  EXPECT_FALSE(subSurface.windowPropertyFrameAndDivider());

  EXPECT_EQ("CurtainWall", frameAndDivider.nfrcProductTypeforAssemblyCalculations());
  EXPECT_TRUE(frameAndDivider.isNFRCProductTypeforAssemblyCalculationsDefaulted());
  EXPECT_TRUE(frameAndDivider.setNFRCProductTypeforAssemblyCalculations("CasementDouble"));
  EXPECT_EQ("CasementDouble", frameAndDivider.nfrcProductTypeforAssemblyCalculations());
  EXPECT_FALSE(frameAndDivider.isNFRCProductTypeforAssemblyCalculationsDefaulted());
}

TEST_F(ModelFixture, WindowPropertyFrameAndDivider_Name) {
  Model model;

  WindowPropertyFrameAndDivider frameAndDivider1(model);
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Window Property Frame And Divider 1", frameAndDivider1.name().get());

  WindowPropertyFrameAndDivider frameAndDivider2(model);
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Window Property Frame And Divider 2", frameAndDivider2.name().get());

  // setName api protects against empty names
  EXPECT_FALSE(frameAndDivider1.setName(""));
  EXPECT_FALSE(frameAndDivider2.setName(""));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Window Property Frame And Divider 1", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Window Property Frame And Divider 2", frameAndDivider2.name().get());

  // setName api protects against non-empty names that are equal
  EXPECT_TRUE(frameAndDivider1.setName("Frame"));
  EXPECT_TRUE(frameAndDivider2.setName("Frame"));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Frame", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Frame 1", frameAndDivider2.name().get());

  // setString api protects against non-empty names that are equal
  EXPECT_TRUE(frameAndDivider1.setString(OS_WindowProperty_FrameAndDividerFields::Name, "Divider"));
  EXPECT_TRUE(frameAndDivider2.setString(OS_WindowProperty_FrameAndDividerFields::Name, "Divider"));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Divider", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Divider 1", frameAndDivider2.name().get());

  // setString api protects against empty names that are equal
  EXPECT_FALSE(frameAndDivider1.setString(OS_WindowProperty_FrameAndDividerFields::Name, ""));
  EXPECT_FALSE(frameAndDivider2.setString(OS_WindowProperty_FrameAndDividerFields::Name, ""));
  ASSERT_TRUE(frameAndDivider1.name());
  EXPECT_EQ("Divider", frameAndDivider1.name().get());
  ASSERT_TRUE(frameAndDivider2.name());
  EXPECT_EQ("Divider 1", frameAndDivider2.name().get());
}

TEST_F(ModelFixture, WindowPropertyFrameAndDivider_GettersSetters) {
  Model m;
  WindowPropertyFrameAndDivider windowPropertyFrameAndDivider(m);

  windowPropertyFrameAndDivider.setName("My WindowPropertyFrameAndDivider");

  // Frame Width: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameWidthDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameWidth(0.75));
  EXPECT_EQ(0.75, windowPropertyFrameAndDivider.frameWidth());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameWidthDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameWidth(-10.0));
  EXPECT_EQ(0.75, windowPropertyFrameAndDivider.frameWidth());
  // Reset
  windowPropertyFrameAndDivider.resetFrameWidth();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameWidthDefaulted());

  // Frame Outside Projection: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameOutsideProjectionDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameOutsideProjection(0.4));
  EXPECT_EQ(0.4, windowPropertyFrameAndDivider.frameOutsideProjection());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameOutsideProjectionDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameOutsideProjection(-10.0));
  EXPECT_EQ(0.4, windowPropertyFrameAndDivider.frameOutsideProjection());
  // Reset
  windowPropertyFrameAndDivider.resetFrameOutsideProjection();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameOutsideProjectionDefaulted());

  // Frame Inside Projection: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameInsideProjectionDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameInsideProjection(0.417));
  EXPECT_EQ(0.417, windowPropertyFrameAndDivider.frameInsideProjection());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameInsideProjectionDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameInsideProjection(-10.0));
  EXPECT_EQ(0.417, windowPropertyFrameAndDivider.frameInsideProjection());
  // Reset
  windowPropertyFrameAndDivider.resetFrameInsideProjection();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameInsideProjectionDefaulted());

  // Frame Conductance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameConductanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameConductance(0.6));
  EXPECT_EQ(0.6, windowPropertyFrameAndDivider.frameConductance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameConductanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameConductance(-10.0));
  EXPECT_EQ(0.6, windowPropertyFrameAndDivider.frameConductance());
  // Reset
  windowPropertyFrameAndDivider.resetFrameConductance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameConductanceDefaulted());

  // Ratio of Frame-Edge Glass Conductance to Center-Of-Glass Conductance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(3.5));
  EXPECT_EQ(3.5, windowPropertyFrameAndDivider.ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(-10.0));
  EXPECT_EQ(3.5, windowPropertyFrameAndDivider.ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance());
  // Reset
  windowPropertyFrameAndDivider.resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted());

  // Frame Solar Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameSolarAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameSolarAbsorptance(0.889));
  EXPECT_EQ(0.889, windowPropertyFrameAndDivider.frameSolarAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameSolarAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameSolarAbsorptance(-10.0));
  EXPECT_EQ(0.889, windowPropertyFrameAndDivider.frameSolarAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetFrameSolarAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameSolarAbsorptanceDefaulted());

  // Frame Visible Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameVisibleAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameVisibleAbsorptance(0.9));
  EXPECT_EQ(0.9, windowPropertyFrameAndDivider.frameVisibleAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameVisibleAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameVisibleAbsorptance(-10.0));
  EXPECT_EQ(0.9, windowPropertyFrameAndDivider.frameVisibleAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetFrameVisibleAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameVisibleAbsorptanceDefaulted());

  // Frame Thermal Hemispherical Emissivity: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameThermalHemisphericalEmissivityDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setFrameThermalHemisphericalEmissivity(1.0));
  EXPECT_EQ(1.0, windowPropertyFrameAndDivider.frameThermalHemisphericalEmissivity());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isFrameThermalHemisphericalEmissivityDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setFrameThermalHemisphericalEmissivity(-10.0));
  EXPECT_EQ(1.0, windowPropertyFrameAndDivider.frameThermalHemisphericalEmissivity());
  // Reset
  windowPropertyFrameAndDivider.resetFrameThermalHemisphericalEmissivity();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isFrameThermalHemisphericalEmissivityDefaulted());

  // Divider Type: Optional String
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerTypeDefaulted());
  EXPECT_EQ("DividedLite", windowPropertyFrameAndDivider.dividerType());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerType("DividedLite"));
  EXPECT_EQ("DividedLite", windowPropertyFrameAndDivider.dividerType());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerTypeDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerType("BADENUM"));
  EXPECT_EQ("DividedLite", windowPropertyFrameAndDivider.dividerType());
  // Reset
  windowPropertyFrameAndDivider.resetDividerType();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerTypeDefaulted());

  // Divider Width: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerWidthDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerWidth(0.462));
  EXPECT_EQ(0.462, windowPropertyFrameAndDivider.dividerWidth());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerWidthDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerWidth(-10.0));
  EXPECT_EQ(0.462, windowPropertyFrameAndDivider.dividerWidth());
  // Reset
  windowPropertyFrameAndDivider.resetDividerWidth();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerWidthDefaulted());

  // Number of Horizontal Dividers: Optional Integer
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isNumberOfHorizontalDividersDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setNumberOfHorizontalDividers(13));
  EXPECT_EQ(13, windowPropertyFrameAndDivider.numberOfHorizontalDividers());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isNumberOfHorizontalDividersDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setNumberOfHorizontalDividers(-10));
  EXPECT_EQ(13, windowPropertyFrameAndDivider.numberOfHorizontalDividers());
  // Reset
  windowPropertyFrameAndDivider.resetNumberOfHorizontalDividers();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isNumberOfHorizontalDividersDefaulted());

  // Number of Vertical Dividers: Optional Integer
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isNumberOfVerticalDividersDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setNumberOfVerticalDividers(14));
  EXPECT_EQ(14, windowPropertyFrameAndDivider.numberOfVerticalDividers());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isNumberOfVerticalDividersDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setNumberOfVerticalDividers(-10));
  EXPECT_EQ(14, windowPropertyFrameAndDivider.numberOfVerticalDividers());
  // Reset
  windowPropertyFrameAndDivider.resetNumberOfVerticalDividers();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isNumberOfVerticalDividersDefaulted());

  // Divider Outside Projection: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerOutsideProjectionDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerOutsideProjection(0.469));
  EXPECT_EQ(0.469, windowPropertyFrameAndDivider.dividerOutsideProjection());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerOutsideProjectionDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerOutsideProjection(-10.0));
  EXPECT_EQ(0.469, windowPropertyFrameAndDivider.dividerOutsideProjection());
  // Reset
  windowPropertyFrameAndDivider.resetDividerOutsideProjection();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerOutsideProjectionDefaulted());

  // Divider Inside Projection: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerInsideProjectionDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerInsideProjection(0.471));
  EXPECT_EQ(0.471, windowPropertyFrameAndDivider.dividerInsideProjection());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerInsideProjectionDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerInsideProjection(-10.0));
  EXPECT_EQ(0.471, windowPropertyFrameAndDivider.dividerInsideProjection());
  // Reset
  windowPropertyFrameAndDivider.resetDividerInsideProjection();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerInsideProjectionDefaulted());

  // Divider Conductance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerConductanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerConductance(1.7));
  EXPECT_EQ(1.7, windowPropertyFrameAndDivider.dividerConductance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerConductanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerConductance(-10.0));
  EXPECT_EQ(1.7, windowPropertyFrameAndDivider.dividerConductance());
  // Reset
  windowPropertyFrameAndDivider.resetDividerConductance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerConductanceDefaulted());

  // Ratio of Divider-Edge Glass Conductance to Center-Of-Glass Conductance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(3.789));
  EXPECT_EQ(3.789, windowPropertyFrameAndDivider.ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(-10.0));
  EXPECT_EQ(3.789, windowPropertyFrameAndDivider.ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance());
  // Reset
  windowPropertyFrameAndDivider.resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted());

  // Divider Solar Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerSolarAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerSolarAbsorptance(0.95));
  EXPECT_EQ(0.95, windowPropertyFrameAndDivider.dividerSolarAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerSolarAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerSolarAbsorptance(-10.0));
  EXPECT_EQ(0.95, windowPropertyFrameAndDivider.dividerSolarAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetDividerSolarAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerSolarAbsorptanceDefaulted());

  // Divider Visible Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerVisibleAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerVisibleAbsorptance(0.952));
  EXPECT_EQ(0.952, windowPropertyFrameAndDivider.dividerVisibleAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerVisibleAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerVisibleAbsorptance(-10.0));
  EXPECT_EQ(0.952, windowPropertyFrameAndDivider.dividerVisibleAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetDividerVisibleAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerVisibleAbsorptanceDefaulted());

  // Divider Thermal Hemispherical Emissivity: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerThermalHemisphericalEmissivityDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setDividerThermalHemisphericalEmissivity(0.955));
  EXPECT_EQ(0.955, windowPropertyFrameAndDivider.dividerThermalHemisphericalEmissivity());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isDividerThermalHemisphericalEmissivityDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setDividerThermalHemisphericalEmissivity(-10.0));
  EXPECT_EQ(0.955, windowPropertyFrameAndDivider.dividerThermalHemisphericalEmissivity());
  // Reset
  windowPropertyFrameAndDivider.resetDividerThermalHemisphericalEmissivity();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isDividerThermalHemisphericalEmissivityDefaulted());

  // Outside Reveal Depth: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isOutsideRevealDepthDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setOutsideRevealDepth(1.913));
  EXPECT_EQ(1.913, windowPropertyFrameAndDivider.outsideRevealDepth());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isOutsideRevealDepthDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setOutsideRevealDepth(-10.0));
  EXPECT_EQ(1.913, windowPropertyFrameAndDivider.outsideRevealDepth());
  // Reset
  windowPropertyFrameAndDivider.resetOutsideRevealDepth();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isOutsideRevealDepthDefaulted());

  // Outside Reveal Solar Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isOutsideRevealSolarAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setOutsideRevealSolarAbsorptance(0.958));
  EXPECT_EQ(0.958, windowPropertyFrameAndDivider.outsideRevealSolarAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isOutsideRevealSolarAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setOutsideRevealSolarAbsorptance(-10.0));
  EXPECT_EQ(0.958, windowPropertyFrameAndDivider.outsideRevealSolarAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetOutsideRevealSolarAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isOutsideRevealSolarAbsorptanceDefaulted());

  // Inside Sill Depth: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideSillDepthDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setInsideSillDepth(1.92));
  EXPECT_EQ(1.92, windowPropertyFrameAndDivider.insideSillDepth());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isInsideSillDepthDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setInsideSillDepth(-10.0));
  EXPECT_EQ(1.92, windowPropertyFrameAndDivider.insideSillDepth());
  // Reset
  windowPropertyFrameAndDivider.resetInsideSillDepth();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideSillDepthDefaulted());

  // Inside Sill Solar Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideSillSolarAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setInsideSillSolarAbsorptance(0.962));
  EXPECT_EQ(0.962, windowPropertyFrameAndDivider.insideSillSolarAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isInsideSillSolarAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setInsideSillSolarAbsorptance(-10.0));
  EXPECT_EQ(0.962, windowPropertyFrameAndDivider.insideSillSolarAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetInsideSillSolarAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideSillSolarAbsorptanceDefaulted());

  // Inside Reveal Depth: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideRevealDepthDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setInsideRevealDepth(1.926));
  EXPECT_EQ(1.926, windowPropertyFrameAndDivider.insideRevealDepth());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isInsideRevealDepthDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setInsideRevealDepth(-10.0));
  EXPECT_EQ(1.926, windowPropertyFrameAndDivider.insideRevealDepth());
  // Reset
  windowPropertyFrameAndDivider.resetInsideRevealDepth();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideRevealDepthDefaulted());

  // Inside Reveal Solar Absorptance: Optional Double
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideRevealSolarAbsorptanceDefaulted());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setInsideRevealSolarAbsorptance(0.964));
  EXPECT_EQ(0.964, windowPropertyFrameAndDivider.insideRevealSolarAbsorptance());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isInsideRevealSolarAbsorptanceDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setInsideRevealSolarAbsorptance(-10.0));
  EXPECT_EQ(0.964, windowPropertyFrameAndDivider.insideRevealSolarAbsorptance());
  // Reset
  windowPropertyFrameAndDivider.resetInsideRevealSolarAbsorptance();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isInsideRevealSolarAbsorptanceDefaulted());

  // NFRC Product Type for Assembly Calculations: Optional String
  // Default value from IDD
  EXPECT_TRUE(windowPropertyFrameAndDivider.isNFRCProductTypeforAssemblyCalculationsDefaulted());
  EXPECT_EQ("CurtainWall", windowPropertyFrameAndDivider.nfrcProductTypeforAssemblyCalculations());
  // Set
  EXPECT_TRUE(windowPropertyFrameAndDivider.setNFRCProductTypeforAssemblyCalculations("CasementDouble"));
  EXPECT_EQ("CasementDouble", windowPropertyFrameAndDivider.nfrcProductTypeforAssemblyCalculations());
  EXPECT_FALSE(windowPropertyFrameAndDivider.isNFRCProductTypeforAssemblyCalculationsDefaulted());
  // Bad Value
  EXPECT_FALSE(windowPropertyFrameAndDivider.setNFRCProductTypeforAssemblyCalculations("BADENUM"));
  EXPECT_EQ("CasementDouble", windowPropertyFrameAndDivider.nfrcProductTypeforAssemblyCalculations());
  // Reset
  windowPropertyFrameAndDivider.resetNFRCProductTypeforAssemblyCalculations();
  EXPECT_TRUE(windowPropertyFrameAndDivider.isNFRCProductTypeforAssemblyCalculationsDefaulted());
}
