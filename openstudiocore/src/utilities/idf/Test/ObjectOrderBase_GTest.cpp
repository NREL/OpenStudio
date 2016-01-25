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
#include "IdfFixture.hpp"

#include "../ObjectOrderBase.hpp"
#include <utilities/idd/IddEnums.hxx>

using openstudio::ObjectOrderBase;
using openstudio::IddObjectTypeVector;
using openstudio::OptionalIddObjectTypeVector;
using openstudio::IddObjectType;

TEST_F(IdfFixture,ObjectOrderBase_Constructors) {
  // default
  ObjectOrderBase defaultOrderer;
  EXPECT_TRUE(defaultOrderer.orderByIddEnum());
  EXPECT_FALSE(defaultOrderer.iddOrder());
  // order is taken from IddObjectType enum order
  EXPECT_TRUE(defaultOrderer.less(openstudio::IddObjectType::Building,
              openstudio::IddObjectType::AirflowNetwork_Distribution_Component_Coil));
  EXPECT_FALSE(defaultOrderer.less(openstudio::IddObjectType::Output_Diagnostics,openstudio::IddObjectType::Lights));
  EXPECT_TRUE(defaultOrderer.indexInOrder(openstudio::IddObjectType::Branch));

  // specified order of IddObjectTypes
  IddObjectTypeVector order;
  order.push_back(openstudio::IddObjectType::Lights);
  order.push_back(openstudio::IddObjectType::Zone);
  order.push_back(openstudio::IddObjectType::RunPeriod);
  order.push_back(openstudio::IddObjectType::Building);
  ObjectOrderBase userEnumOrder(order);
  EXPECT_FALSE(userEnumOrder.orderByIddEnum());
  ASSERT_TRUE(userEnumOrder.iddOrder());
  EXPECT_TRUE(order == *(userEnumOrder.iddOrder()));
  ASSERT_TRUE(userEnumOrder.indexInOrder(openstudio::IddObjectType::RunPeriod));
  EXPECT_EQ(static_cast<unsigned>(2),*(userEnumOrder.indexInOrder(openstudio::IddObjectType::RunPeriod)));
  ASSERT_TRUE(userEnumOrder.indexInOrder(openstudio::IddObjectType::Branch));
  EXPECT_EQ(static_cast<unsigned>(4),*(userEnumOrder.indexInOrder(openstudio::IddObjectType::Branch)));

  // derived class is handling order
  ObjectOrderBase cededControl(true);
  EXPECT_FALSE(cededControl.orderByIddEnum());
  EXPECT_FALSE(cededControl.iddOrder());
  EXPECT_FALSE(cededControl.indexInOrder(openstudio::IddObjectType::Building));
}

// test that when new type of order is set, others are disabled
TEST_F(IdfFixture,ObjectOrderBase_OrderSetters) {
  ObjectOrderBase orderer;

  IddObjectTypeVector order;
  order.push_back(openstudio::IddObjectType::Lights);
  order.push_back(openstudio::IddObjectType::Zone);
  order.push_back(openstudio::IddObjectType::RunPeriod);
  order.push_back(openstudio::IddObjectType::Building);

  orderer.setIddOrder(order);
  EXPECT_FALSE(orderer.orderByIddEnum());
  EXPECT_TRUE(orderer.less(openstudio::IddObjectType::Lights,openstudio::IddObjectType::Building));

  orderer.setOrderByIddEnum();
  EXPECT_TRUE(orderer.orderByIddEnum());
  EXPECT_FALSE(orderer.iddOrder());
  EXPECT_TRUE(orderer.less(openstudio::IddObjectType::Building,openstudio::IddObjectType::Lights));

  orderer.setDirectOrder();
  EXPECT_FALSE(orderer.orderByIddEnum());
  EXPECT_FALSE(orderer.iddOrder());
}

TEST_F(IdfFixture,ObjectOrderBase_ManipulateIddObjectTypeOrder) {
  IddObjectTypeVector order;
  order.push_back(openstudio::IddObjectType::Lights);    // 0
  order.push_back(openstudio::IddObjectType::Zone);      // 1
  order.push_back(openstudio::IddObjectType::RunPeriod); // 2
  order.push_back(openstudio::IddObjectType::Building);  // 3

  ObjectOrderBase orderer(order);
  bool success;

  // push_back
  success = orderer.push_back(openstudio::IddObjectType::Schedule_Compact); // 4
  EXPECT_TRUE(success);
  EXPECT_EQ(static_cast<unsigned>(4),*(orderer.indexInOrder(openstudio::IddObjectType::Schedule_Compact)));
  EXPECT_EQ(static_cast<unsigned>(5),*(orderer.indexInOrder(openstudio::IddObjectType::Schedule_Day_Hourly)));
  EXPECT_TRUE(orderer.less(openstudio::IddObjectType::Schedule_Compact,
                           openstudio::IddObjectType::DesignSpecification_OutdoorAir));
  EXPECT_FALSE(orderer.less(openstudio::IddObjectType::DesignSpecification_OutdoorAir,
                            openstudio::IddObjectType::Schedule_Day_Hourly));

  // insert behind IddObjectType
  success = orderer.insert(openstudio::IddObjectType::Ceiling_Adiabatic,IddObjectType(IddObjectType::Building));
  EXPECT_TRUE(success);
  EXPECT_EQ(static_cast<unsigned>(3),*(orderer.indexInOrder(openstudio::IddObjectType::Ceiling_Adiabatic)));
  success = orderer.insert(IddObjectType::Daylighting_Controls,
                           IddObjectType(IddObjectType::AirflowNetwork_Distribution_Node));
  EXPECT_TRUE(success);
  EXPECT_EQ(orderer.iddOrder()->size() - 1,
            *(orderer.indexInOrder(openstudio::IddObjectType::Daylighting_Controls)));

  // insert at index
  success = orderer.insert(openstudio::IddObjectType::ThermalStorage_ChilledWater_Mixed,2);
  EXPECT_TRUE(success);
  EXPECT_EQ(static_cast<unsigned>(2),
            *(orderer.indexInOrder(openstudio::IddObjectType::ThermalStorage_ChilledWater_Mixed)));
  success = orderer.insert(openstudio::IddObjectType::Refrigeration_CompressorList,37891);
  EXPECT_TRUE(success);
  EXPECT_EQ(orderer.iddOrder()->size() - 1,
            *(orderer.indexInOrder(openstudio::IddObjectType::Refrigeration_CompressorList)));

  // move before IddObjectType
  unsigned n = orderer.iddOrder()->size();
  success = orderer.move(openstudio::IddObjectType::Refrigeration_CompressorList,
                         IddObjectType(IddObjectType::Lights));
  EXPECT_TRUE(success);
  EXPECT_EQ(static_cast<unsigned>(0),
            *(orderer.indexInOrder(openstudio::IddObjectType::Refrigeration_CompressorList)));
  success = orderer.move(openstudio::IddObjectType::ElectricLoadCenter_Generators,
                         IddObjectType(IddObjectType::Building));
  EXPECT_FALSE(success);
  success = orderer.move(openstudio::IddObjectType::Building,
                         IddObjectType(IddObjectType::ElectricLoadCenter_Generators));
  EXPECT_TRUE(success);
  EXPECT_EQ(orderer.iddOrder()->size() - 1,
            *(orderer.indexInOrder(openstudio::IddObjectType::Building)));
  EXPECT_EQ(n,orderer.iddOrder()->size());

  // move to index
  success = orderer.move(openstudio::IddObjectType::Building,0);
  EXPECT_TRUE(success);
  EXPECT_EQ(static_cast<unsigned>(0),
            *(orderer.indexInOrder(openstudio::IddObjectType::Building)));
  success = orderer.move(openstudio::IddObjectType::RunPeriod,18601);
  EXPECT_TRUE(success);
  EXPECT_EQ(orderer.iddOrder()->size() - 1,
            *(orderer.indexInOrder(openstudio::IddObjectType::RunPeriod)));
  success = orderer.move(openstudio::IddObjectType::ZoneControl_Humidistat,0);
  EXPECT_FALSE(success);
  EXPECT_EQ(n,orderer.iddOrder()->size());

  // swap
  unsigned i = *(orderer.indexInOrder(openstudio::IddObjectType::Lights));
  unsigned j = *(orderer.indexInOrder(openstudio::IddObjectType::Refrigeration_CompressorList));
  success = orderer.swap(openstudio::IddObjectType::Lights,
                         openstudio::IddObjectType::Refrigeration_CompressorList);
  EXPECT_TRUE(success);
  EXPECT_EQ(i,*(orderer.indexInOrder(openstudio::IddObjectType::Refrigeration_CompressorList)));
  EXPECT_EQ(j,*(orderer.indexInOrder(openstudio::IddObjectType::Lights)));
  EXPECT_EQ(n,orderer.iddOrder()->size());

  // erase
  success = orderer.erase(openstudio::IddObjectType::Refrigeration_CompressorList);
  EXPECT_TRUE(success);
  EXPECT_EQ(orderer.iddOrder()->size(),
            *(orderer.indexInOrder(openstudio::IddObjectType::Refrigeration_CompressorList)));
  success = orderer.erase(openstudio::IddObjectType::Refrigeration_CompressorList);
  EXPECT_FALSE(success);
  EXPECT_TRUE(orderer.iddOrder()->size() < n);
}
