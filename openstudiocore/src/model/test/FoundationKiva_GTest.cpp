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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../FoundationKiva.hpp"
#include "../FoundationKiva_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FoundationKiva_FoundationKiva) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a foundation kiva object to use
      FoundationKiva kiva(model);
      
      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );
    
  // create a model to use
  Model model;

  // create a foundation kiva object to use
  FoundationKiva kiva(model);

  EXPECT_TRUE(kiva.isInteriorHorizontalInsulationDepthDefaulted());
  EXPECT_TRUE(kiva.isExteriorHorizontalInsulationWidthDefaulted());
  EXPECT_TRUE(kiva.isWallHeightAboveGradeDefaulted());
  EXPECT_TRUE(kiva.isWallDepthBelowSlabDefaulted());
  EXPECT_TRUE(kiva.isFootingDepthDefaulted());

  ASSERT_EQ(0, kiva.interiorHorizontalInsulationDepth());
  ASSERT_EQ(0, kiva.exteriorHorizontalInsulationWidth());
  ASSERT_EQ(0.2, kiva.wallHeightAboveGrade());
  ASSERT_EQ(0, kiva.wallDepthBelowSlab());
  ASSERT_EQ(0.3, kiva.footingDepth());
}

// test setting and getting
TEST_F(ModelFixture, FoundationKiva_SetGetFields) {
  // create a model to use
  Model model;

  // create a foundation kiva object to use
  FoundationKiva kiva(model);

  // create a material object to use
  StandardOpaqueMaterial material(model);

  // create a construction object to use
  Construction construction(model);

  // set the fields
  kiva.setInteriorHorizontalInsulationMaterial(material);
  kiva.setInteriorHorizontalInsulationDepth(1);
  kiva.setInteriorHorizontalInsulationWidth(1);
  kiva.setInteriorVerticalInsulationMaterial(material);
  kiva.setInteriorVerticalInsulationDepth(1);
  kiva.setExteriorHorizontalInsulationMaterial(material);
  kiva.setExteriorHorizontalInsulationDepth(1);
  kiva.setExteriorHorizontalInsulationWidth(1);
  kiva.setExteriorVerticalInsulationMaterial(material);
  kiva.setExteriorVerticalInsulationDepth(1);
  kiva.setWallHeightAboveGrade(1);
  kiva.setWallDepthBelowSlab(1);
  kiva.setFootingWallConstruction(construction);
  kiva.setFootingMaterial(material);
  kiva.setFootingDepth(1);

  // check the fields
  boost::optional<Material> optinteriorhorizontalinsulationmaterial = kiva.interiorHorizontalInsulationMaterial();
  EXPECT_TRUE(optinteriorhorizontalinsulationmaterial);
  ASSERT_FALSE(kiva.isInteriorHorizontalInsulationDepthDefaulted());
  ASSERT_EQ(1, kiva.interiorHorizontalInsulationDepth());
  boost::optional<double> optinteriorhorizontalinsulationwidth = kiva.interiorHorizontalInsulationWidth();
  EXPECT_TRUE(optinteriorhorizontalinsulationwidth);
  auto interiorhorizontalinsulationwidth = optinteriorhorizontalinsulationwidth.get();
  ASSERT_EQ(1, interiorhorizontalinsulationwidth);
  boost::optional<Material> optinteriorverticalinsulationmaterial = kiva.interiorVerticalInsulationMaterial();
  EXPECT_TRUE(optinteriorverticalinsulationmaterial);
  boost::optional<double> optinteriorverticalinsulationdepth = kiva.interiorVerticalInsulationDepth();
  EXPECT_TRUE(optinteriorverticalinsulationdepth);
  auto interiorverticalinsulationdepth = optinteriorverticalinsulationdepth.get();
  ASSERT_EQ(1, interiorverticalinsulationdepth);
  boost::optional<Material> optexteriorhorizontalinsulationmaterial = kiva.exteriorHorizontalInsulationMaterial();
  EXPECT_TRUE(optexteriorhorizontalinsulationmaterial);
  boost::optional<double> optexteriorhorizontalinsulationdepth = kiva.exteriorHorizontalInsulationDepth();
  EXPECT_TRUE(optexteriorhorizontalinsulationdepth);
  auto exteriorhorizontalinsulationdepth = optexteriorhorizontalinsulationdepth.get();
  ASSERT_EQ(1, exteriorhorizontalinsulationdepth);
  ASSERT_FALSE(kiva.isExteriorHorizontalInsulationWidthDefaulted());
  ASSERT_EQ(1, kiva.exteriorHorizontalInsulationWidth());
  boost::optional<Material> optexteriorverticalinsulationmaterial = kiva.exteriorVerticalInsulationMaterial();
  EXPECT_TRUE(optexteriorverticalinsulationmaterial);
  boost::optional<double> optexteriorverticalinsulationdepth = kiva.exteriorVerticalInsulationDepth();
  EXPECT_TRUE(optexteriorverticalinsulationdepth);
  auto exteriorverticalinsulationdepth = optexteriorverticalinsulationdepth.get();
  ASSERT_EQ(1, exteriorverticalinsulationdepth);
  ASSERT_FALSE(kiva.isWallHeightAboveGradeDefaulted());
  ASSERT_EQ(1, kiva.wallHeightAboveGrade());
  ASSERT_FALSE(kiva.isWallDepthBelowSlabDefaulted());
  ASSERT_EQ(1, kiva.wallDepthBelowSlab());
  boost::optional<ConstructionBase> optfootingwallconstruction = kiva.footingWallConstruction();
  EXPECT_TRUE(optfootingwallconstruction);
  boost::optional<Material> optfootingmaterial = kiva.footingMaterial();
  EXPECT_TRUE(optfootingmaterial);
  ASSERT_FALSE(kiva.isFootingDepthDefaulted());
  ASSERT_EQ(1, kiva.footingDepth());

  // reset them one by one
  kiva.resetInteriorHorizontalInsulationMaterial();
  kiva.resetInteriorHorizontalInsulationDepth();
  kiva.resetInteriorVerticalInsulationMaterial();
  kiva.resetExteriorHorizontalInsulationMaterial();
  kiva.resetExteriorHorizontalInsulationWidth();
  kiva.resetExteriorVerticalInsulationMaterial();
  kiva.resetWallHeightAboveGrade();
  kiva.resetWallDepthBelowSlab();
  kiva.resetFootingWallConstruction();
  kiva.resetFootingMaterial();
  kiva.resetFootingDepth();

  EXPECT_FALSE(kiva.interiorHorizontalInsulationMaterial());
  EXPECT_TRUE(kiva.isInteriorHorizontalInsulationDepthDefaulted());
  EXPECT_FALSE(kiva.interiorVerticalInsulationMaterial());
  EXPECT_FALSE(kiva.exteriorHorizontalInsulationMaterial());
  EXPECT_TRUE(kiva.isExteriorHorizontalInsulationWidthDefaulted());
  EXPECT_FALSE(kiva.exteriorVerticalInsulationMaterial());
  EXPECT_TRUE(kiva.isWallHeightAboveGradeDefaulted());
  EXPECT_TRUE(kiva.isWallDepthBelowSlabDefaulted());
  EXPECT_FALSE(kiva.footingWallConstruction());
  EXPECT_FALSE(kiva.footingMaterial());
  EXPECT_TRUE(kiva.isFootingDepthDefaulted());
}

// test cloning it
TEST_F(ModelFixture, FoundationKiva_Clone) {
  // create a model to use
  Model model;

  // create a foundation kiva object to use
  FoundationKiva kiva(model);

  // create a material object to use
  StandardOpaqueMaterial material(model);

  // change some of the fields
  kiva.setInteriorHorizontalInsulationDepth(2.5);

  // clone it into the same model
  FoundationKiva kivaClone = kiva.clone(model).cast<FoundationKiva>();
  ASSERT_FALSE(kivaClone.isInteriorHorizontalInsulationDepthDefaulted());
  ASSERT_EQ(2.5, kivaClone.interiorHorizontalInsulationDepth());
  ASSERT_TRUE(kivaClone.isExteriorHorizontalInsulationWidthDefaulted());

  // clone it into a different model
  Model model2;
  FoundationKiva kivaClone2 = kiva.clone(model2).cast<FoundationKiva>();
  ASSERT_FALSE(kivaClone2.isInteriorHorizontalInsulationDepthDefaulted());
  ASSERT_EQ(2.5, kivaClone2.interiorHorizontalInsulationDepth());
  ASSERT_TRUE(kivaClone2.isExteriorHorizontalInsulationWidthDefaulted());
}

// test setting on outside boundary of surface
TEST_F(ModelFixture, FoundationKiva_AdjacentFoundation) {
  Model model;
  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, model);
  FoundationKiva kiva(model);
  EXPECT_EQ(0, kiva.surfaces().size());
  EXPECT_FALSE(surface.adjacentFoundation());
  surface.setAdjacentFoundation(kiva);
  EXPECT_EQ("Foundation", surface.outsideBoundaryCondition());
  EXPECT_TRUE(surface.adjacentFoundation());
  EXPECT_EQ(1, kiva.surfaces().size());
}

// check that parent reset works
TEST_F(ModelFixture, FoundationKiva_ParentReset) {
  Model model;
  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, model);
  FoundationKiva kiva(model);
  surface.setAdjacentFoundation(kiva);
  surface.resetAdjacentFoundation();
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());
  EXPECT_FALSE(surface.adjacentFoundation());
  EXPECT_EQ(0, kiva.surfaces().size());
}

// check that remove works
TEST_F(ModelFixture, FoundationKiva_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  FoundationKiva kiva(model);
  EXPECT_FALSE(kiva.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}