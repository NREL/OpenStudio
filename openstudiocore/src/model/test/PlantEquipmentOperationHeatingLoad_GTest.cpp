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
#include "../PlantEquipmentOperationHeatingLoad.hpp"
#include "../PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,PlantEquipmentOperationHeatingLoad) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  // Almost all of this object's behavior is defined in the base PlantEquipmentOperationLoadScheme
  // and tested in PlantEquipmentOperationCoolingLoad. Here we test simply construction.
  ASSERT_EXIT (
    {
      model::Model m;
      model::PlantEquipmentOperationHeatingLoad plantOperationScheme(m);
      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );
}

