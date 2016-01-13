/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include "../Model.hpp"
#include "../Connection.hpp"
#include "../AirLoopHVAC.hpp"
#include "../Node.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneMixer.hpp"
#include "../AirTerminalSingleDuctUncontrolled.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/OS_AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST(Connection, Name)
{
  openstudio::IdfObject airLoopIdfObject(openstudio::IddObjectType::OS_AirLoopHVAC);
  ASSERT_TRUE(airLoopIdfObject.name());
  EXPECT_EQ(airLoopIdfObject.name().get(),"");

  openstudio::IdfObject nodeIdfObject(openstudio::IddObjectType::OS_Node);
  ASSERT_TRUE(nodeIdfObject.name());
  EXPECT_EQ(nodeIdfObject.name().get(),"");

  openstudio::IdfObject connectionIdfObject(openstudio::IddObjectType::OS_Connection);
  ASSERT_TRUE(connectionIdfObject.name());
  EXPECT_EQ(connectionIdfObject.name().get(),"");

  openstudio::model::Model model;
  openstudio::model::Connection connection(model);

  ASSERT_TRUE(connection.name());
  EXPECT_NE(connection.name().get(),"");
}
