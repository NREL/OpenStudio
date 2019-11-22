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

#include "../RefrigerationSystem.hpp"
#include "../RefrigerationSystem_Impl.hpp"
#include "../RefrigerationSubcoolerMechanical.hpp"
#include "../RefrigerationSubcoolerMechanical_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_RefrigerationSubcoolerMechanical)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_iddObjectType)
{
  Model m;
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

  EXPECT_EQ( refrigerationSubcoolerMechanical.iddObjectType(), IddObjectType::OS_Refrigeration_Subcooler_Mechanical);
}

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_CapacityProvidingSystem)
{
  Model m;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  boost::optional<RefrigerationSystem> refrigerationSystem2 = refrigerationSubcoolerMechanical.capacityProvidingSystem();
  EXPECT_TRUE(refrigerationSystem2 != nullptr);
  EXPECT_EQ( refrigerationSystem.handle(), refrigerationSystem2.get().handle());
}

TEST_F(ModelFixture,RefrigerationSubcoolerMechanical_OutletControlTemperature)
{
  Model m;
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);

  EXPECT_EQ( 10.0, refrigerationSubcoolerMechanical.outletControlTemperature() );  // Brian's value

  refrigerationSubcoolerMechanical.setOutletControlTemperature(15.0);
  EXPECT_EQ( 15.0, refrigerationSubcoolerMechanical.outletControlTemperature() );

  refrigerationSubcoolerMechanical.setOutletControlTemperature(-15.0);
  EXPECT_EQ( -15.0, refrigerationSubcoolerMechanical.outletControlTemperature() );
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_Remove)
{
  Model model;
  RefrigerationSubcoolerMechanical testObject = RefrigerationSubcoolerMechanical(model);

  std::vector<RefrigerationSubcoolerMechanical> refrigerationMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(1, refrigerationMechanicalSubcoolers.size());

  testObject.remove();

  refrigerationMechanicalSubcoolers = model.getModelObjects<RefrigerationSubcoolerMechanical>();
  EXPECT_EQ(0, refrigerationMechanicalSubcoolers.size());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_RemoveRefrigerationSystem)
{
  Model model;
  RefrigerationSubcoolerMechanical testObject = RefrigerationSubcoolerMechanical(model);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(model);

  testObject.setCapacityProvidingSystem(refrigerationSystem);

  ASSERT_TRUE(testObject.capacityProvidingSystem());
  EXPECT_EQ(testObject.capacityProvidingSystem().get().handle(), refrigerationSystem.handle());

  refrigerationSystem.remove();

  ASSERT_FALSE(testObject.capacityProvidingSystem());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithDefaultData)
{
  Model m;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_EQ(10.0, refrigerationSubcoolerMechanical.outletControlTemperature());
}

TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneOneModelWithCustomData)
{
  Model m;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(m);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(m);
  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);
  refrigerationSubcoolerMechanical.setOutletControlTemperature(15.0);

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(m).cast<RefrigerationSubcoolerMechanical>();

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_EQ(15.0, refrigerationSubcoolerMechanicalClone.outletControlTemperature());
}


TEST_F(ModelFixture, RefrigerationSubcoolerMechanical_CloneTwoModelWithDefaultData)
{
  Model model;

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = RefrigerationSubcoolerMechanical(model);
  RefrigerationSystem refrigerationSystem = RefrigerationSystem(model);

  refrigerationSubcoolerMechanical.setCapacityProvidingSystem(refrigerationSystem);

  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone = refrigerationSubcoolerMechanical.clone(model).cast<RefrigerationSubcoolerMechanical>();

  Model model2;
  RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanicalClone2 = refrigerationSubcoolerMechanical.clone(model2).cast<RefrigerationSubcoolerMechanical>();

  std::vector<RefrigerationSystem> refrigerationSystems = model.getModelObjects<RefrigerationSystem>();
    ASSERT_EQ(1, refrigerationSystems.size());

    refrigerationSystems = model2.getModelObjects<RefrigerationSystem>();
    ASSERT_EQ(0, refrigerationSystems.size());

  EXPECT_NE(refrigerationSubcoolerMechanicalClone.handle(), refrigerationSubcoolerMechanical.handle());
  EXPECT_NE(refrigerationSubcoolerMechanicalClone2.handle(), refrigerationSubcoolerMechanical.handle());
  EXPECT_NE(refrigerationSubcoolerMechanicalClone2.handle(), refrigerationSubcoolerMechanicalClone.handle());

  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone.capacityProvidingSystem());
  EXPECT_FALSE(refrigerationSubcoolerMechanicalClone2.capacityProvidingSystem());
  EXPECT_EQ(10.0, refrigerationSubcoolerMechanicalClone2.outletControlTemperature());
}
