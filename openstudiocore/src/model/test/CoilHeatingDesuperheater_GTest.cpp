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

    std::vector<CoilHeatingDesuperheater>coilHeatingDesuperheaters = model.getModelObjects<CoilHeatingDesuperheater>();
    EXPECT_EQ(1, coilHeatingDesuperheaters.size());

    testObject.remove();

    coilHeatingDesuperheaters = model.getModelObjects<CoilHeatingDesuperheater>();
    EXPECT_EQ(0, coilHeatingDesuperheaters.size());
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

    testObject.setHeatReclaimRecoveryEfficiency(0.5);
    testObject.setParasiticElectricLoad(1.0);

    CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

    EXPECT_DOUBLE_EQ(testObjectClone.heatReclaimRecoveryEfficiency(),0.5);
    EXPECT_DOUBLE_EQ(testObjectClone.parasiticElectricLoad(),1.0);
}