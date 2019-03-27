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

#include "../GeneratorPVWatts.hpp"
#include "../GeneratorPVWatts_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurface_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, GeneratorPVWatts_GeneratorPVWatts) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
  {
    // create a model to use
    Model model;

    // create a foundation kiva object to use
    GeneratorPVWatts generator(model, 1);

    exit(0);
  },
    ::testing::ExitedWithCode(0),
    ""
    );

  // create a model to use
  Model model;

  // create a pvwatts generator to use
  GeneratorPVWatts generator(model, 1);

  EXPECT_TRUE(generator.isModuleTypeDefaulted());
  EXPECT_TRUE(generator.isArrayTypeDefaulted());
  EXPECT_TRUE(generator.isSystemLossesDefaulted());
  EXPECT_TRUE(generator.isTiltAngleDefaulted());
  EXPECT_TRUE(generator.isAzimuthAngleDefaulted());
  EXPECT_TRUE(generator.isGroundCoverageRatioDefaulted());

  ASSERT_EQ("", generator.pvWattsVersion());
  ASSERT_EQ(1, generator.dcSystemCapacity());
  ASSERT_EQ("Standard", generator.moduleType());
  ASSERT_EQ("FixedOpenRack", generator.arrayType());
  ASSERT_EQ(0.14, generator.systemLosses());
  ASSERT_EQ(20, generator.tiltAngle());
  ASSERT_EQ(180, generator.azimuthAngle());
  ASSERT_EQ(0.4, generator.groundCoverageRatio());
}

// test setting and getting
TEST_F(ModelFixture, GeneratorPVWatts_SetGetFields) {
  // create a model to use
  Model model;

  // create a pvwatts generator object to use
  GeneratorPVWatts generator(model, 1);

  // set the fields
  generator.setDCSystemCapacity(2);
  generator.setModuleType("Premium");
  generator.setArrayType("OneAxis");
  generator.setSystemLosses(0.15);
  generator.setTiltAngle(25);
  generator.setAzimuthAngle(0);
  generator.setGroundCoverageRatio(0.5);

  // check the fields
  ASSERT_EQ(2, generator.dcSystemCapacity());
  ASSERT_FALSE(generator.isModuleTypeDefaulted());
  ASSERT_EQ("Premium", generator.moduleType());
  ASSERT_FALSE(generator.isArrayTypeDefaulted());
  ASSERT_EQ("OneAxis", generator.arrayType());
  ASSERT_FALSE(generator.isSystemLossesDefaulted());
  ASSERT_EQ(0.15, generator.systemLosses());
  ASSERT_FALSE(generator.isTiltAngleDefaulted());
  ASSERT_EQ(25, generator.tiltAngle());
  ASSERT_FALSE(generator.isAzimuthAngleDefaulted());
  ASSERT_EQ(0, generator.azimuthAngle());
  boost::optional<PlanarSurface> optplanarsurface = generator.surface();
  ASSERT_FALSE(optplanarsurface);
  ASSERT_FALSE(generator.isGroundCoverageRatioDefaulted());
  ASSERT_EQ(0.5, generator.groundCoverageRatio());

  // reset them one by one
  generator.resetModuleType();
  generator.resetArrayType();
  generator.resetSystemLosses();
  generator.resetTiltAngle();
  generator.resetAzimuthAngle();
  generator.resetSurface();
  generator.resetGroundCoverageRatio();

  EXPECT_TRUE(generator.isModuleTypeDefaulted());
  EXPECT_TRUE(generator.isArrayTypeDefaulted());
  EXPECT_TRUE(generator.isSystemLossesDefaulted());
  EXPECT_TRUE(generator.isTiltAngleDefaulted());
  EXPECT_TRUE(generator.isAzimuthAngleDefaulted());
  EXPECT_TRUE(generator.isGroundCoverageRatioDefaulted());
}

// test constructing with a surface
TEST_F(ModelFixture, GeneratorPVWatts_SurfaceConstruct) {
  Model model;
  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, model);
  GeneratorPVWatts generator(model, surface, 1);
  EXPECT_TRUE(generator.surface());
  generator.resetSurface();
  EXPECT_EQ(1u, model.getConcreteModelObjects<Surface>().size());
  EXPECT_FALSE(generator.surface());
}

// test constructing with a surface and bad dc system capacity
TEST_F(ModelFixture, GeneratorPVWatts_SurfaceConstructBadDCSystemCapacity) {
  Model model;
  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface surface(points, model);
  EXPECT_THROW(GeneratorPVWatts(model, surface, -1), openstudio::Exception);
}

// test assigning a surface
TEST_F(ModelFixture, GeneratorPVWatts_SurfaceAssign) {
  Model model;
  GeneratorPVWatts generator(model, 1);
  EXPECT_FALSE(generator.surface());
  Point3dVector points;
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  ShadingSurface shadingsurface(points, model);
  generator.setSurface(shadingsurface);
  EXPECT_TRUE(generator.surface());
  boost::optional<PlanarSurface> optplanarsurface = generator.surface();
  ShadingSurface shadingsurface2 = optplanarsurface.get().cast<ShadingSurface>();
  shadingsurface2.remove();
  EXPECT_EQ(0, model.getConcreteModelObjects<ShadingSurface>().size());
  EXPECT_FALSE(generator.surface());
}

// test cloning it
TEST_F(ModelFixture, GeneratorPVWatts_Clone) {
  Model model;
  GeneratorPVWatts generator(model, 1);
  generator.setSystemLosses(0.1);

  // clone it into the same model
  GeneratorPVWatts generatorClone = generator.clone(model).cast<GeneratorPVWatts>();
  ASSERT_FALSE(generatorClone.isSystemLossesDefaulted());
  ASSERT_EQ(0.1, generatorClone.systemLosses());
  ASSERT_TRUE(generatorClone.isModuleTypeDefaulted());

  // clone it into a different model
  Model model2;
  GeneratorPVWatts generatorClone2 = generator.clone(model2).cast<GeneratorPVWatts>();
  ASSERT_FALSE(generatorClone2.isSystemLossesDefaulted());
  ASSERT_EQ(0.1, generatorClone2.systemLosses());
  ASSERT_TRUE(generatorClone2.isModuleTypeDefaulted());
}

// test that remove works
TEST_F(ModelFixture, GeneratorPVWatts_Remove) {
  Model model;
  //start with 0
  std::vector<GeneratorPVWatts> gens = model.getModelObjects<GeneratorPVWatts>();
  EXPECT_EQ(0u, gens.size());
  //add 1
  GeneratorPVWatts generator(model, 1);
  gens = model.getModelObjects<GeneratorPVWatts>();
  EXPECT_EQ(1u, gens.size());
  //remove
  EXPECT_FALSE(generator.remove().empty());
  gens = model.getModelObjects<GeneratorPVWatts>();
  EXPECT_EQ(0u, gens.size());
}

// test electric load center distribution
TEST_F(ModelFixture, GeneratorPVWatts_ElectricLoadCenterDistribution) {
  Model model;
  ElectricLoadCenterDistribution elcd(model);
  EXPECT_EQ(0, elcd.generators().size());
  GeneratorPVWatts generator(model, 1);
  elcd.addGenerator(generator);
  EXPECT_EQ(1u, elcd.generators().size());
  boost::optional<ElectricLoadCenterDistribution> optelcd = generator.electricLoadCenterDistribution();
  EXPECT_TRUE(optelcd);
  ElectricLoadCenterDistribution elcd2 = optelcd.get();
  EXPECT_EQ(elcd, elcd2);
  EXPECT_TRUE(elcd.removeGenerator(generator));
  EXPECT_FALSE(generator.electricLoadCenterDistribution());
  ASSERT_EQ(0, elcd.generators().size());
  ASSERT_EQ(0, elcd2.generators().size());
  elcd.addGenerator(generator);
  elcd.remove();
  EXPECT_FALSE(generator.electricLoadCenterDistribution());
}
TEST_F(ModelFixture, GeneratorPVWatts_ElectricLoadCenterDistribution2) {
  Model model;

  GeneratorPVWatts generator(model, 1);

  //should be 1 default ELCD attached to pvwatts
  std::vector<ElectricLoadCenterDistribution> elcd = model.getModelObjects<ElectricLoadCenterDistribution>();
  EXPECT_EQ(1u, elcd.size());
  EXPECT_EQ(1u, elcd[0].generators().size());
  std::vector<Generator> generators = elcd[0].generators();
  EXPECT_EQ(generators[0].handle(), generator.handle());
  EXPECT_TRUE(generator.electricLoadCenterDistribution());
  EXPECT_EQ(elcd[0].handle(), generator.electricLoadCenterDistribution().get().handle());
  //Add another ELCD
  ElectricLoadCenterDistribution elcd2(model);
  EXPECT_EQ(2, model.getModelObjects<ElectricLoadCenterDistribution>().size());
  //Add the pvwatts to it which should remove the existing one attached to pvwatts
  EXPECT_TRUE(elcd2.addGenerator(generator));
  EXPECT_EQ(0, elcd[0].generators().size());
  EXPECT_EQ(elcd2.handle(), generator.electricLoadCenterDistribution().get().handle());
}