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
#include <model/test/ModelFixture.hpp>
#include <model/SetpointManagerWarmest.hpp>
#include <model/SetpointManagerWarmest_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset) {
  Model model;

  SetpointManagerWarmest setpointManagerWarmest(model);

  AirLoopHVAC airLoopHVAC(model);

  Node node = airLoopHVAC.supplyOutletNode();

  ASSERT_TRUE(setpointManagerWarmest.addToNode(node));

  ASSERT_TRUE(node.setpointManagerWarmest());

  ASSERT_EQ(setpointManagerWarmest,node.setpointManagerWarmest().get());
}



