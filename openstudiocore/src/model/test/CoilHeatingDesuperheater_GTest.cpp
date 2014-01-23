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

#include <model/CoilHeatingDesuperheater.hpp>
#include <model/CoilHeatingDesuperheater_Impl.hpp>
#include <model/RefrigerationCondenserAirCooled.hpp>
#include <model/RefrigerationCondenserAirCooled_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

//Test construction of Coil:Heating:Desuperheater
TEST_F(ModelFixture, CoilHeatingDesuperheater_DefaultContructor)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    ASSERT_EXIT (
    {
        Model model;
        CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);

        exit(0);
    } ,
        ::testing::ExitedWithCode(0), "");
}

//Test removal of Coil:Heating:Desuperheater
TEST_F(ModelFixture, CoilHeatingDesuperheater_Remove)
{
    Model model;
    CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);
    RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
    testObject.setHeatingSource(condenser);

    std::vector<CoilHeatingDesuperheater> coilHeatingDesuperheaters = model.getModelObjects<CoilHeatingDesuperheater>();
    EXPECT_EQ(1, coilHeatingDesuperheaters.size());

    std::vector<RefrigerationCondenserAirCooled> refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
    EXPECT_EQ(1, refrigerationCondensers.size());

    testObject.remove();

    coilHeatingDesuperheaters = model.getModelObjects<CoilHeatingDesuperheater>();
    EXPECT_EQ(0, coilHeatingDesuperheaters.size());

    refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
    EXPECT_EQ(1, refrigerationCondensers.size());
}

//Test the methods that set and get the fields
TEST_F(ModelFixture, CoilHeatingDesuperheater_set_get)
{
    Model model;

    CoilHeatingDesuperheater desuperheater(model);

    desuperheater.setHeatReclaimRecoveryEfficiency(999.0);
    desuperheater.setParasiticElectricLoad(999.0);

    EXPECT_DOUBLE_EQ(desuperheater.heatReclaimRecoveryEfficiency(),999.0);
    EXPECT_DOUBLE_EQ(desuperheater.parasiticElectricLoad(),999.0);
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddToNode_AirLoop)
{
    Model model;
    CoilHeatingDesuperheater desuperheater(model);
    AirLoopHVAC airLoop = AirLoopHVAC(model);
    Node node = airLoop.supplyOutletNode();

    EXPECT_TRUE(desuperheater.addToNode(node));
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddBranchForHVACComponent_AirLoop)
{
    Model model;
    CoilHeatingDesuperheater desuperheater(model);
    AirLoopHVAC airLoop = AirLoopHVAC(model);

    EXPECT_FALSE(airLoop.addBranchForHVACComponent(desuperheater));
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddToNode_PlantLoop)
{
    Model model;
    CoilHeatingDesuperheater desuperheater(model);
    PlantLoop plantLoop = PlantLoop(model);
    Node node = plantLoop.supplyOutletNode();

    EXPECT_FALSE(desuperheater.addToNode(node));
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddDemandBranchForComponent_PlantLoop)
{
    Model model;
    CoilHeatingDesuperheater desuperheater(model);
    PlantLoop plantLoop = PlantLoop(model);

    EXPECT_FALSE(plantLoop.addDemandBranchForComponent(desuperheater));
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddSupplyBranchForComponent_PlantLoop)
{
    Model model;
    CoilHeatingDesuperheater desuperheater(model);
    PlantLoop plantLoop = PlantLoop(model);

    EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(desuperheater));
}

//Test clone model with default data
TEST_F(ModelFixture, CoilHeatingDesuperheater_CloneModelWithDefaultData)
{
    Model model;

    CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);

    CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(testObjectClone.heatReclaimRecoveryEfficiency(),0.8);
    EXPECT_DOUBLE_EQ(testObjectClone.parasiticElectricLoad(),0.0);
}

//Test clone model with custom data
TEST_F(ModelFixture, CoilHeatingDesuperheater_CloneModelWithCustomData)
{
    Model model;

    CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);
    RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);

    ScheduleCompact alwaysOn = ScheduleCompact(model);

    testObject.setHeatReclaimRecoveryEfficiency(0.5);
    testObject.setParasiticElectricLoad(1.0);
    testObject.setHeatingSource(condenser);
    testObject.setAvailabilitySchedule(alwaysOn);

    CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(testObjectClone.heatReclaimRecoveryEfficiency(),0.5);
    EXPECT_DOUBLE_EQ(testObjectClone.parasiticElectricLoad(),1.0);
    EXPECT_TRUE(testObjectClone.availabilitySchedule());
    EXPECT_EQ(alwaysOn, testObjectClone.availabilitySchedule().get());
    EXPECT_FALSE(testObjectClone.heatingSource());
}

//Test clone two model with custom data
TEST_F(ModelFixture, CoilHeatingDesuperheater_CloneTwoModelWithCustomData)
{
    Model model;

    CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);
    RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);

    ScheduleCompact alwaysOn = ScheduleCompact(model);

    testObject.setHeatReclaimRecoveryEfficiency(0.5);
    testObject.setParasiticElectricLoad(1.0);
    testObject.setHeatingSource(condenser);
    testObject.setAvailabilitySchedule(alwaysOn);

    CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

    Model model2;
    CoilHeatingDesuperheater testObjectClone2 = testObject.clone(model2).cast<CoilHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(testObjectClone2.heatReclaimRecoveryEfficiency(),0.5);
    EXPECT_DOUBLE_EQ(testObjectClone2.parasiticElectricLoad(),1.0);
    EXPECT_TRUE(testObjectClone2.availabilitySchedule());
    EXPECT_NE(alwaysOn, testObjectClone2.availabilitySchedule().get());
    EXPECT_FALSE(testObjectClone2.heatingSource());
    EXPECT_NE(testObjectClone2, testObjectClone);
    EXPECT_NE(testObjectClone, testObject);
}
