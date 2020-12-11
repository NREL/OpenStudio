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

#include "../RefrigerationCondenserCascade.hpp"
#include "../RefrigerationCondenserCascade_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCondenserCascade_RefrigerationCondenserCascade) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_iddObjectType) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ(refrigerationCondenserCascade.iddObjectType(), IddObjectType::OS_Refrigeration_Condenser_Cascade);
}

//  -4.0,                    !- Rated Condensing Temperature {C}
TEST_F(ModelFixture, RefrigerationCondenserCascade_RatedCondensingTemperature) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ(-4.0, refrigerationCondenserCascade.ratedCondensingTemperature());  // Brian's value

  refrigerationCondenserCascade.setRatedCondensingTemperature(15.0);
  EXPECT_EQ(15.0, refrigerationCondenserCascade.ratedCondensingTemperature());
}

//  3.,                      !- Rated Approach Temperature Difference {DeltaC}
TEST_F(ModelFixture, RefrigerationCondenserCascade_RatedApproachTemperatureDifference) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ(3.0, refrigerationCondenserCascade.ratedApproachTemperatureDifference());  // Brian's value

  EXPECT_TRUE(refrigerationCondenserCascade.setRatedApproachTemperatureDifference(15.0));
  EXPECT_EQ(15.0, refrigerationCondenserCascade.ratedApproachTemperatureDifference());

  refrigerationCondenserCascade.resetRatedApproachTemperatureDifference();
  EXPECT_EQ(3.0, refrigerationCondenserCascade.ratedApproachTemperatureDifference());

  EXPECT_TRUE(refrigerationCondenserCascade.setRatedApproachTemperatureDifference(1.0));
  EXPECT_FALSE(refrigerationCondenserCascade.setRatedApproachTemperatureDifference(0.0));
  EXPECT_FALSE(refrigerationCondenserCascade.setRatedApproachTemperatureDifference(-1.0));
}

//  20000.,                  !- Rated Effective Total Heat Rejection Rate {W}
TEST_F(ModelFixture, RefrigerationCondenserCascade_RatedEffectiveTotalHeatRejectionRate) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ(20000.0, refrigerationCondenserCascade.ratedEffectiveTotalHeatRejectionRate());  // Brian's value

  EXPECT_TRUE(refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(1500.0));
  EXPECT_EQ(1500.0, refrigerationCondenserCascade.ratedEffectiveTotalHeatRejectionRate());

  EXPECT_TRUE(refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(1.0));
  EXPECT_FALSE(refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(0.0));
  EXPECT_FALSE(refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(-1.0));
}

//  Fixed;                   !- Condensing Temperature Control Type
TEST_F(ModelFixture, RefrigerationCondenserCascade_CondensingTemperatureControlType) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  EXPECT_EQ("Fixed", refrigerationCondenserCascade.condensingTemperatureControlType());

  EXPECT_TRUE(refrigerationCondenserCascade.setCondensingTemperatureControlType("Float"));
  EXPECT_EQ("Float", refrigerationCondenserCascade.condensingTemperatureControlType());

  EXPECT_TRUE(refrigerationCondenserCascade.setCondensingTemperatureControlType("Fixed"));
  EXPECT_EQ("Fixed", refrigerationCondenserCascade.condensingTemperatureControlType());

  refrigerationCondenserCascade.resetCondensingTemperatureControlType();
  EXPECT_EQ("Fixed", refrigerationCondenserCascade.condensingTemperatureControlType());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CondenserRefrigerantOperatingChargeInventory) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  refrigerationCondenserCascade.setCondenserRefrigerantOperatingChargeInventory(10.0);
  EXPECT_EQ(10.0, refrigerationCondenserCascade.condenserRefrigerantOperatingChargeInventory());

  refrigerationCondenserCascade.setCondenserRefrigerantOperatingChargeInventory(-10.0);
  EXPECT_EQ(-10.0, refrigerationCondenserCascade.condenserRefrigerantOperatingChargeInventory());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CondensateReceiverRefrigerantInventory) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  refrigerationCondenserCascade.setCondensateReceiverRefrigerantInventory(10.0);
  EXPECT_EQ(10.0, refrigerationCondenserCascade.condensateReceiverRefrigerantInventory());

  refrigerationCondenserCascade.setCondensateReceiverRefrigerantInventory(-10.0);
  EXPECT_EQ(-10.0, refrigerationCondenserCascade.condensateReceiverRefrigerantInventory());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CondensatePipingRefrigerantInventory) {
  Model m;
  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  refrigerationCondenserCascade.setCondensatePipingRefrigerantInventory(10.0);
  EXPECT_EQ(10.0, refrigerationCondenserCascade.condensatePipingRefrigerantInventory());

  refrigerationCondenserCascade.setCondensatePipingRefrigerantInventory(-10.0);
  EXPECT_EQ(-10.0, refrigerationCondenserCascade.condensatePipingRefrigerantInventory());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_Remove) {
  Model model;
  RefrigerationCondenserCascade testObject = RefrigerationCondenserCascade(model);

  std::vector<RefrigerationCondenserCascade> refrigerationCascadeCondensers = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(1, refrigerationCascadeCondensers.size());

  testObject.remove();

  refrigerationCascadeCondensers = model.getModelObjects<RefrigerationCondenserCascade>();
  EXPECT_EQ(0, refrigerationCascadeCondensers.size());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CloneOneModelWithDefaultData) {
  Model m;

  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);

  RefrigerationCondenserCascade refrigerationCondenserCascadeClone = refrigerationCondenserCascade.clone(m).cast<RefrigerationCondenserCascade>();

  EXPECT_NE(refrigerationCondenserCascadeClone.handle(), refrigerationCondenserCascade.handle());

  EXPECT_EQ(-4.0, refrigerationCondenserCascadeClone.ratedCondensingTemperature());
  EXPECT_EQ(3.0, refrigerationCondenserCascadeClone.ratedApproachTemperatureDifference());
  EXPECT_EQ(20000.0, refrigerationCondenserCascadeClone.ratedEffectiveTotalHeatRejectionRate());
  EXPECT_EQ("Fixed", refrigerationCondenserCascadeClone.condensingTemperatureControlType());
}

TEST_F(ModelFixture, RefrigerationCondenserCascade_CloneOneModelWithCustomData) {
  Model m;

  RefrigerationCondenserCascade refrigerationCondenserCascade = RefrigerationCondenserCascade(m);
  refrigerationCondenserCascade.setRatedCondensingTemperature(-3.0);
  refrigerationCondenserCascade.setRatedApproachTemperatureDifference(4.0);
  refrigerationCondenserCascade.setRatedEffectiveTotalHeatRejectionRate(21000.0);
  refrigerationCondenserCascade.setCondensingTemperatureControlType("Float");

  RefrigerationCondenserCascade refrigerationCondenserCascadeClone = refrigerationCondenserCascade.clone(m).cast<RefrigerationCondenserCascade>();

  EXPECT_NE(refrigerationCondenserCascadeClone.handle(), refrigerationCondenserCascade.handle());

  EXPECT_EQ(-3.0, refrigerationCondenserCascadeClone.ratedCondensingTemperature());
  EXPECT_EQ(4.0, refrigerationCondenserCascadeClone.ratedApproachTemperatureDifference());
  EXPECT_EQ(21000.0, refrigerationCondenserCascadeClone.ratedEffectiveTotalHeatRejectionRate());
  EXPECT_EQ("Float", refrigerationCondenserCascadeClone.condensingTemperatureControlType());
}
