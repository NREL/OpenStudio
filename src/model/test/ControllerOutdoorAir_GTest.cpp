/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../AirflowNetworkOutdoorAirflow.hpp"
#include "../AirflowNetworkOutdoorAirflow_Impl.hpp"
#include "../AirflowNetworkCrack.hpp"
#include "../AirflowNetworkCrack_Impl.hpp"
#include "../AirflowNetworkReferenceCrackConditions.hpp"
#include "../AirflowNetworkReferenceCrackConditions_Impl.hpp"

#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

/*
TEST_F(ModelFixture,ControllerOutdoorAir_IddKeys) {
  Model myModel;
  ControllerOutdoorAir controller(myModel);
  IddObject obj = controller.iddObject();
  OptionalIddField f = obj.getField(Controller_OutdoorAirFields::EconomizerControlType);
  ASSERT_TRUE(f);
  EXPECT_TRUE(f->properties().type == IddFieldType::ChoiceType);
  IddKeyVector keys = f->keys();
  EXPECT_EQ(static_cast<unsigned>(8),keys.size());
  LOG(Info,"Keys for field " << Controller_OutdoorAirFields::EconomizerControlType
      << ", " << f->name() << ", of IddObject " << obj.name() << ":");
  unsigned i = 0;
  for (IddKey& key : keys) {
    LOG(Info,"  Key " << i << ": " << key.name());
    ++i;
  }
}
*/

TEST_F(ModelFixture, FanZoneExhaust_AddAFNOutdoorAirflow) {
  Model model;
  ControllerOutdoorAir controller(model);

  EXPECT_FALSE(controller.airflowNetworkOutdoorAirflow());

  AirflowNetworkCrack crack(model, 1.0, 0.5);
  EXPECT_EQ(1, crack.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack.airMassFlowExponent());
  EXPECT_FALSE(crack.referenceCrackConditions());

  auto afnobject = controller.getAirflowNetworkOutdoorAirflow(crack);

  ASSERT_TRUE(afnobject.crack());
  EXPECT_EQ(crack, afnobject.crack().get());
}
