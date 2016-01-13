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
#include "ModelFixture.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../Schedule.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_AirLoopHVACOutdoorAirSystem)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model,controller);

  ASSERT_EQ( openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem, oaSystem.iddObjectType().value() );
}

TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_controllerOutdoorAir)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model,controller);

  ASSERT_EQ( openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem.getControllerOutdoorAir().iddObjectType().value() );
}

TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_clone)
{
  Model model = Model();
  Model model2 = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model,controller);

  ASSERT_EQ( openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem.getControllerOutdoorAir().iddObjectType().value() );

  AirLoopHVACOutdoorAirSystem oaSystem2 = oaSystem.clone(model2).cast<AirLoopHVACOutdoorAirSystem>();

  ASSERT_EQ( openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem2.getControllerOutdoorAir().iddObjectType().value() );
}

//TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_edges)
//{
//  Model m;
//  Schedule s = m.alwaysOnDiscreteSchedule();
//  AirLoopHVAC airLoop(m);
//  ControllerOutdoorAir controllerOutdoorAir(m);
//  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m,controllerOutdoorAir);
//  CoilHeatingElectric coil(m, s);
//
//  Node supplyOutletNode = airLoop.supplyOutletNode();
//  outdoorAirSystem.addToNode(supplyOutletNode);
//  coil.addToNode(supplyOutletNode);
//  boost::optional<ModelObject> testObject = airLoop.supplyComponent(outdoorAirSystem.handle());
//  ASSERT_TRUE(testObject);
//  EXPECT_EQ(outdoorAirSystem, *testObject);
//  std::vector<HVACComponent> edges = outdoorAirSystem.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
//  ASSERT_EQ(1, edges.size());
//  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be CoilHeatingElectric
//  ASSERT_EQ(1, edges.size());
//  EXPECT_EQ(coil, edges[0]);
//}
