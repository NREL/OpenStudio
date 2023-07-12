/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefrigerationSystem.hpp"
#include "../RefrigerationSystem_Impl.hpp"
#include "../RefrigerationSubcoolerMechanical.hpp"
#include "../RefrigerationSubcoolerMechanical_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_RefrigerationSubcoolerMechanical) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_iddObjectType) {
  Model m;
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

  EXPECT_EQ(refrigerationSubcoolerMechanical.iddObjectType(), IddObjectType::OS_Refrigeration_Subcooler_Mechanical);
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CapacityProvidingSystem) {
  Model m;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  boost::optional<RefrigerationSystem> refrigerationSystem2 = refrigerationSubcoolerMechanical.capacityProvidingSystem();
  EXPECT_TRUE(refrigerationSystem2.has_value());
  EXPECT_EQ(refrigerationSystem.handle(), refrigerationSystem2.get().handle());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_OutletControlTemperature) {
  Model m;
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

  EXPECT_EQ(10.0, refrigerationSubcoolerMechanical.outletControlTemperature().get());  // Brian's value

  refrigerationSubcoolerMechanical.setOutletControlTemperature(15.0);
  EXPECT_EQ(15.0, refrigerationSubcoolerMechanical.outletControlTemperature().get());

  refrigerationSubcoolerMechanical.setOutletControlTemperature(-15.0);
  EXPECT_EQ(-15.0, refrigerationSubcoolerMechanical.outletControlTemperature().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_Remove) {
  Model model;
  RefrigerationSubcoolerMechanical testObject = RefrigerationSubcoolerMechanical(model);

  std::vector<RefrigerationSubcoolerMechanical> refrigerationMechanicalSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, refrigerationMechanicalSubcoolers.size());

  testObject.remove();

  refrigerationMechanicalSubcoolers = model.getConcreteModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(0, refrigerationMechanicalSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_RemoveRefrigerationSystem) {
  Model model;
  RefrigerationSubcoolerMechanical testObject = RefrigerationSubcoolerMechanical(model);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(model);

  testObject.setCapacityProvidingSystem(refrigerationSystem);

  ASSERT_TRUE(testObject.capacityProvidingSystem());
  EXPECT_EQ(testObject.capacityProvidingSystem().get().handle(), refrigerationSystem.handle());

  refrigerationSystem.remove();

  ASSERT_FALSE(testObject.capacityProvidingSystem());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithDefaultData) {
  Model m;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  auto refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_EQ(10.0, refrigerationSubcoolerMechanical.outletControlTemperature().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithCustomData) {
  Model m;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);
  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);
  refrigerationSubcoolerMechanical.setOutletControlTemperature(15.0);

  auto refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_EQ(15.0, refrigerationSubcoolerMechanicalClone.outletControlTemperature().get());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneTwoModelWithDefaultData) {
  Model model;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(model);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(model);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  auto refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(model).cast<RefrigerationSubcoolerMechanical>();

  Model model2;
  auto refrigerationSubcoolerMechanicalClone2 = refrigerationSubcoolerMechanical.clone(model2).cast<RefrigerationSubcoolerMechanical>();

  std::vector<RefrigerationSystem> refrigerationSystems = model.getConcreteModelObjects<RefrigerationSystem>();
  ASSERT_EQ(1, refrigerationSystems.size());

  refrigerationSystems = model2.getConcreteModelObjects<RefrigerationSystem>();
  ASSERT_EQ(0, refrigerationSystems.size());

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());
  EXPECT_NE(refrigerationSubcoolerMechanicalClone2.handle(), refrigerationSubcoolerMechanical.handle());
  EXPECT_NE(refrigerationSubcoolerMechanicalClone2.handle(), refrigerationSubcoolerMechanicalClone.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone2.capacityProvidingSystem());
  EXPECT_EQ(10.0, refrigerationSubcoolerMechanicalClone2.outletControlTemperature().get());
}
