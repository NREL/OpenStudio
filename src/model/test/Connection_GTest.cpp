/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Connection.hpp"
#include "../AirLoopHVAC.hpp"
#include "../Node.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneMixer.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/OS_AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Connection_Name) {
  openstudio::IdfObject airLoopIdfObject(openstudio::IddObjectType::OS_AirLoopHVAC);
  ASSERT_TRUE(airLoopIdfObject.name());
  EXPECT_EQ(airLoopIdfObject.name().get(), "");

  openstudio::IdfObject nodeIdfObject(openstudio::IddObjectType::OS_Node);
  ASSERT_TRUE(nodeIdfObject.name());
  EXPECT_EQ(nodeIdfObject.name().get(), "");

  openstudio::IdfObject connectionIdfObject(openstudio::IddObjectType::OS_Connection);
  EXPECT_FALSE(connectionIdfObject.name());

  openstudio::model::Model model;
  openstudio::model::Connection connection(model);

  EXPECT_FALSE(connection.name());
}
