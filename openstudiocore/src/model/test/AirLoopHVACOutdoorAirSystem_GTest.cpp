/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/Model.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/ControllerOutdoorAir_Impl.hpp>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

TEST(AirLoopHVACOutdoorAirSystem,AirLoopHVACOutdoorAirSystem_AirLoopHVACOutdoorAirSystem)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model,controller);

  ASSERT_EQ( openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem, oaSystem.iddObjectType().value() );
}

TEST(AirLoopHVACOutdoorAirSystem,AirLoopHVACOutdoorAirSystem_controllerOutdoorAir)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model,controller);

  ASSERT_EQ( openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem.getControllerOutdoorAir().iddObjectType().value() );
}

TEST(AirLoopHVACOutdoorAirSystem,AirLoopHVACOutdoorAirSystem_clone)
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

